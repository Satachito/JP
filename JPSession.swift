import Foundation

enum
TLSMode: Int {
	case ValidateCertificate	=	1
	case BypassValidation		=	2
}

enum
JPSessionError	: Error {
case		NOSESSION
case		ILLEGAL_REQUEST(URLRequest)
}


class
JPSession: NSObject, StreamDelegate {

	var	buffer			= Data()

	var	input			: InputStream
	var	output			: OutputStream

	var	openHandler		: ( Stream )		-> ()
	var	inputHandler	: ( InputStream )	-> ()
	var	errorHandler	: ( Stream )		-> ()
	var	endHandler		: ( Stream )		-> ()

	deinit {
		input.close()
		input.delegate = nil
		output.close()
		output.delegate = nil
	}
	
	private func
	_Write() {
		if buffer.count > 0 {
			let w = buffer.withUnsafeBytes { p in
				return output.write( p, maxLength: buffer.count )
			}
			if w > 0 { buffer = buffer.subdata( in: 0 ..< w ) }
		}
	}

	private func
	Setup( _ tlsMode: TLSMode? ) {
		CFReadStreamSetProperty( input, CFStreamPropertyKey( kCFStreamPropertyShouldCloseNativeSocket ), kCFBooleanTrue )
		CFWriteStreamSetProperty( output, CFStreamPropertyKey( kCFStreamPropertyShouldCloseNativeSocket ), kCFBooleanTrue )

		input.delegate = self
		output.delegate = self

		if let wTLSMode = tlsMode {
			CFWriteStreamSetProperty(
				output
			,	CFStreamPropertyKey( kCFStreamSocketSecurityLevelNegotiatedSSL )
			,	kCFStreamPropertySocketSecurityLevel
			)
			CFWriteStreamSetProperty(
				output
			,	CFStreamPropertyKey( kCFStreamPropertySSLSettings )
			,	[ kCFStreamSSLValidatesCertificateChain as String: ( wTLSMode == .ValidateCertificate ? true : false ) ] as CFTypeRef!
			)
		}

		input.schedule( in: RunLoop.current, forMode: .defaultRunLoopMode )
		output.schedule( in: RunLoop.current, forMode: .defaultRunLoopMode )

		input.open()
		output.open()
	}

	private func
	stream(
		theStream	: Stream
	,	handleEvent	: Stream.Event
	) {
		switch handleEvent {
		case Stream.Event.openCompleted		: openHandler( theStream )
		case Stream.Event.hasBytesAvailable	: inputHandler( input )
		case Stream.Event.hasSpaceAvailable	: _Write()
		case Stream.Event.errorOccurred		: errorHandler( theStream )
		case Stream.Event.endEncountered	: endHandler( theStream )
		default								: assert( false )
		}
	}
	
	init(
		host			: String
	, _	port			: Int
	, _	tlsMode			: TLSMode?							= nil
	, _	openHandler		: @escaping ( Stream )		-> ()	= { _ in }
	, _	errorHandler	: @escaping ( Stream )		-> ()	= { _ in }
	, _	endHandler		: @escaping ( Stream )		-> ()	= { _ in }
	, 	inputHandler	: @escaping ( InputStream )	-> ()
	) throws {
		self.openHandler	= openHandler
		self.inputHandler	= inputHandler
		self.errorHandler	= errorHandler
		self.endHandler		= endHandler
		
		var wIS	: InputStream?
		var	wOS	: OutputStream?

		Stream.getStreamsToHost(
			withName		: host
		,	port			: port
		,	inputStream		: &wIS
		,	outputStream	: &wOS
		)

		if wIS == nil { throw JPSessionError.NOSESSION }
		if wOS == nil { throw JPSessionError.NOSESSION }
		
		input = wIS!
		output = wOS!
		
		super.init()
		
		Setup( tlsMode )
	}

	init(
		socket			: CFSocketNativeHandle
	, _	tlsMode			: TLSMode?
	, _	openHandler		: @escaping ( Stream )		-> ()
	, _	errorHandler	: @escaping ( Stream )		-> ()
	, _	endHandler		: @escaping ( Stream )		-> ()
	,	inputHandler	: @escaping ( InputStream )	-> ()
	) throws {
		self.openHandler	= openHandler
		self.inputHandler	= inputHandler
		self.errorHandler	= errorHandler
		self.endHandler		= endHandler

		var	wRS		: Unmanaged<CFReadStream>?
		var	wWS		: Unmanaged<CFWriteStream>?

		CFStreamCreatePairWithSocket(
			nil
		,	socket
		,	&wRS
		,	&wWS
		)

		if wRS == nil { throw JPSessionError.NOSESSION }
		if wWS == nil { throw JPSessionError.NOSESSION }
		
		input	= wRS!.takeRetainedValue()
		output	= wWS!.takeRetainedValue()

		super.init()

		Setup( tlsMode )
	}

	func
	Write( _ p: Data ) {
		buffer.append( p )
		if output.hasSpaceAvailable { _Write() }
	}
}

func
Fetch(
  _	request	: URLRequest
, _	tlsMode	: TLSMode? = nil
,	cookies	: [ HTTPCookie ]? = nil
, _	error	: @escaping ( Error? ) -> ()
,	handler	: @escaping ( Data, URLResponse ) -> ()
) throws {

	if	let wURL = request.url
	,	let	wHost = wURL.host
	,	let	wMethod = request.httpMethod {
		var	wPort: Int = 80
		if let w = wURL.port {
			wPort = w
		} else {
			if let w = wURL.scheme, w == "https" { wPort = 443 }
		}
		
		var	wResponse	= Data()
		var	wData		= Data()

		let	wMessage = CFHTTPMessageCreateEmpty( nil, false ).takeRetainedValue()
		let	wSession = try JPSession(
			host: wHost
		,	wPort
		,	tlsMode
		,	{ p in }
		,	{ p in error( p.streamError ) }
		,	{ p in }
		) {	p in
			var w = [ UInt8 ]( repeating: 0, count: 1024 )
			let	wRead = p.read( &w, maxLength: w.count )
			var	wIndex = 0
			while !CFHTTPMessageIsHeaderComplete( wMessage ) && wIndex < wRead {
				if !CFHTTPMessageAppendBytes( wMessage, &w[ wIndex ], 1 ) { assert( false ) }
				wIndex += 1
			}
			if wIndex > 0 {
				let	w2 = w[ 0 ..< wIndex ]
				wResponse.append( Array( w2 ), count: w2.count )
				let	w3 = w[ wIndex ..< wRead ]
				wData.append( Array( w3 ), count: w3.count )
			} else {
				wData.append( w, count: wRead )
			}
		}
		let	wRequest = CFHTTPMessageCreateRequest(
			nil
		,	wMethod as CFString
		,	wURL as CFURL
		,	kCFHTTPVersion1_1
		).takeRetainedValue()

		if let wAllHTTPHeaderFields = request.allHTTPHeaderFields {
			for ( wKey, wValue ) in wAllHTTPHeaderFields {
				CFHTTPMessageSetHeaderFieldValue( wRequest, wKey as CFString, wValue as CFString )
			}
		}
		CFHTTPMessageSetHeaderFieldValue( wRequest, "Host" as CFString, wHost as CFString )
		CFHTTPMessageSetHeaderFieldValue( wRequest, "User-Agent" as CFString, UserDefaults.standard.string( forKey: "UserAgent" ) as CFString? )
		CFHTTPMessageSetHeaderFieldValue( wRequest, "Accept" as CFString, "*/*" as CFString )
		CFHTTPMessageSetHeaderFieldValue( wRequest, "Connection" as CFString, "close" as CFString )
		
		if let wCookies = cookies {
			var wCStr = ""
			for w in wCookies {
				wCStr = "\(wCStr)\(w.name)=\(w.value);"
			}
			CFHTTPMessageSetHeaderFieldValue( wRequest, "Cookie" as CFString, wCStr as CFString )
		}
		if let wBody = request.httpBody { CFHTTPMessageSetBody( wRequest, wBody as CFData ) }
		
		wSession.Write( CFHTTPMessageCopySerializedMessage( wRequest )!.takeRetainedValue() as Data )
	} else {
		throw JPSessionError.ILLEGAL_REQUEST( request )
	}
}

