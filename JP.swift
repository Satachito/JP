//	Written by Satoru Ogura, Tokyo.
//
import	Foundation
import	CoreGraphics

enum
JPError	: Error {
	case e(String)
}

func
HexChar( _ p: Int ) -> Character {
	return [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" ][ p & 0x0f ]
}

func
HexString( _ p: UnsafePointer<UInt8>, _ count: Int ) -> String {
	var	v = ""
	for i in 0 ..< 16 {
		v.append( HexChar( Int( p[ i ] ) >> 4 ) )
		v.append( HexChar( Int( p[ i ] ) ) )
	}
	return v
}

func
HexString( _ p: Data ) -> String {
	var	v = ""
	p.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) in
		v = HexString( q, p.count )
	}
	return v
}

func
RandomData( _ p: Int ) -> Data {
	let wFD = open( "/dev/random", O_RDONLY )
	assert( wFD > 2 )
	var w = [ UInt8 ]( repeating: 0, count: p )
	read( wFD, &w, Int( p ) )
	close( wFD )
	return Data( bytes: w, count: p )
}

func
RandomIndices( _ p: Int ) -> [ Int ] {
	var	v = [ Int ]( repeating: 0, count: p )
	for i in 0 ..< p { v[ i ] = i }
	for i in 0 ..< p {
		let j = Int( arc4random_uniform( UInt32( p - i ) ) ) + i
		( v[ i ], v[ j ] ) = ( v[ j ], v[ i ] )
	}
	return v
}

func
ToArray<T>( _ start: UnsafePointer<T>, _ count: Int ) -> [ T ] {
	return Array( UnsafeBufferPointer( start: UnsafePointer<T>( start ), count: count ) )
}
//	USAGE: let wArray : [ Int16 ] = ToArray( data.bytes, data.length / sizeof( Int16 ) )

func
UTF8Length( _ p: String ) -> Int {
	return p.lengthOfBytes( using: .utf8 )
}

func
UTF8Data( _ p: String ) -> Data? {
	return p.data( using: .utf8 )
}

func
UTF8String( _ p: Data ) -> String? {
	return String( data:p, encoding: .utf8 )
}

func
UTF8String( _ p: UnsafeRawPointer, _ count: Int ) -> String? {
	return UTF8String( Data( bytes: p, count: count ) )
}

func
Base64String( _ p: Data, _ options: Data.Base64EncodingOptions = [] ) -> String {
	return p.base64EncodedString( options: options )
}

func
Base64Data( _ p: String, _ options: Data.Base64DecodingOptions = [] ) -> Data? {
	return Data( base64Encoded: p, options: options )
}

func
EncodeJSON( _ p: Any, _ options: JSONSerialization.WritingOptions = [] ) throws -> Data {
	return try JSONSerialization.data( withJSONObject: p, options: options )
}

func
DecodeJSON( _ p: Data, _ options: JSONSerialization.ReadingOptions = [] ) throws -> Any {
	return try JSONSerialization.jsonObject( with: p, options: options )
}

func
JSONString( _ from: Any ) -> String? {
	if #available(iOS 11.0, *) {
		if let w = try? EncodeJSON( from, [ .prettyPrinted, .sortedKeys ] ) { return UTF8String( w ) } else { return nil }
	} else {
		if let w = try? EncodeJSON( from, [ .prettyPrinted ] ) { return UTF8String( w ) } else { return nil }
	}
}

func
JSONObject( _ from: String ) -> Any? {
	guard let wJSONData = UTF8Data( from ) else { return nil }
	guard let v = try? DecodeJSON( wJSONData ) else { return nil }
	return v
}

class
Cell<T>	{
	var
	m			: T
	let
	next		: Cell?
	init(	_ a	: T, _ pNext: Cell? = nil ) { m = a; next = pNext }
}

func
IsNull( _ p: Any? ) -> Bool {
	if p == nil { return true }
	return p is NSNull
}

func
AsInt( _ p: Any? ) -> Int? {
	if let w = p as? NSNumber { return w.intValue }
	if let w = p as? String { return Int( w ) }
	return nil
}

//	Assuming the Data is BD
//	Search: A Result: []
//	Search: B Result: [ 0 ]
//	Search: C Result: [ 0, 1 ]
//	Search: D Result: [ 1 ]
//	Search: E Result: []

func
BinarySearch< T: Comparable >( _ a: T, _ b: [ T ] ) -> [ Int ] {
	switch b.count {
	case 0: return []
	case 1: return a == b[ 0 ] ? [ 0 ] : []
	default:
		if a < b.first! || a > b.last! { return [] }
		if a == b.first! { return [ 0 ] }
		if a == b.last! { return [ b.count - 1 ] }
		
		var ( l, h ) = ( 0, b.count - 1 )
		while true {
			if h - l == 1 { return [ l, h ] }
			let m = ( l + h ) / 2
			if a == b[ m ] { return [ m ] }
			if a < b[ m ] { h = m } else { l = m }
		}
	}
	
}

func
JPTest() {
	let	wData = RandomData( 16 )

	var	wArray = [ UInt8 ]();
	wData.withUnsafeBytes { ( p: UnsafePointer<UInt8> ) in
		wArray = ToArray( p, 16 )
	}
	var	wStr = ""
	for i in 0 ..< 16 { wStr += String( format: "%02x", wArray[ i ] ) }
	assert( wStr == HexString( wData ) )
	
	wStr = "今日は、Alberto López.☕️";
	assert( wStr == UTF8String( UTF8Data( wStr )! ) )
	assert( wData == Base64Data( Base64String( wData ) )! )
	
	assert( IsNull( nil ) )
	assert( IsNull( NSNull() ) )

	assert( AsInt( "123" ) == 123 )
	assert( AsInt( 123 ) == 123 )
	assert( AsInt( NSNumber( value: 123 ) ) == 123 )
	
	assert( BinarySearch( 1, [ 2, 4 ] ) == [] )
	assert( BinarySearch( 2, [ 2, 4 ] ) == [ 0 ] )
	assert( BinarySearch( 3, [ 2, 4 ] ) == [ 0, 1 ] )
	assert( BinarySearch( 4, [ 2, 4 ] ) == [ 1 ] )
	assert( BinarySearch( 5, [ 2, 4 ] ) == [] )
	assert( BinarySearch( 1, [ 2, 4, 6 ] ) == [] )
	assert( BinarySearch( 2, [ 2, 4, 6 ] ) == [ 0 ] )
	assert( BinarySearch( 3, [ 2, 4, 6 ] ) == [ 0, 1 ] )
	assert( BinarySearch( 4, [ 2, 4, 6 ] ) == [ 1 ] )
	assert( BinarySearch( 5, [ 2, 4, 6 ] ) == [ 1, 2 ] )
	assert( BinarySearch( 6, [ 2, 4, 6 ] ) == [ 2 ] )
	assert( BinarySearch( 7, [ 2, 4, 6 ] ) == [] )
	print( "RandomData, HexString, HexChar, ToArray<T>, UTF8String, UTF8Data, Base64Data, Base64String, IsNull, AsInt" );
}

enum
ReaderError	: Error {
case		eod
}

class
Reader< T > {
	var
	_unread : Cell< T >?
	func
	_Read() throws -> T { throw ReaderError.eod }
	func
	Read() throws -> T {
		if let v = _unread { _unread = v.next ; return v.m }
		return try _Read()
	}
	func
	Unread( _ p: T ) { _unread = Cell< T >( p, _unread ) }
}

class
StdinUnicodeReader: Reader< UnicodeScalar > {
	var
	m	= String.UnicodeScalarView()
	override func
	_Read() throws -> UnicodeScalar {
		while m.count == 0 {
			if let w = readLine( strippingNewline: false ) { m = w.unicodeScalars } else { throw ReaderError.eod }
		}
		let v = m.first!
		m = String.UnicodeScalarView( m.dropFirst() )
		return v
	}
}

class
StringUnicodeReader	: Reader< UnicodeScalar > {
	var
	m	: String.UnicodeScalarView
	init( _ a: String ) { m = a.unicodeScalars }
	override func
	_Read() throws -> UnicodeScalar {
		if m.count == 0 { throw ReaderError.eod }
		let v = m.first!
		m = String.UnicodeScalarView( m.dropFirst() )
		return v
	}
}

func
SkipWhite( _ r: Reader< UnicodeScalar > ) throws {
	while true {
		let u = try r.Read()
		if !CharacterSet.whitespacesAndNewlines.contains( u ) {
			r.Unread( u )
			break
		}
	}
}

func
Notify( _ name: String, _ p: @escaping ( Notification ) -> () ) -> NSObjectProtocol {
	return NotificationCenter.default.addObserver(
		forName	: Notification.Name( rawValue: name )
	,	object	: nil
	,	queue	: nil
	,	using	: p
	)
}

func
Main( _ d: @escaping () -> () ) {
	DispatchQueue.main.async( execute: d )
}

func
Sub( _ d: @escaping () -> () ) {
	DispatchQueue.global().async( execute: d )
}

func
Periodical( _ p: TimeInterval, d: @escaping () -> () ) -> Timer {
	return Timer.scheduledTimer(
		timeInterval: TimeInterval( p )
	,	target		: BlockOperation( block: { d() } )
	,	selector	: #selector( Operation.main )
	,	userInfo	: nil
	,	repeats		: true
	)
}

/*
func
Delay( _ p: TimeInterval, d: @escaping () -> () ) -> Timer {
	return Timer.scheduledTimer(
		timeInterval: TimeInterval( p )
	,	target		: BlockOperation( block: { d() } )
	,	selector	: #selector( Operation.main )
	,	userInfo	: nil
	,	repeats		: false
	)
}
*/

func
Delay( _ p: TimeInterval, _ queue: DispatchQueue = DispatchQueue.main, d: @escaping () -> () ) {
	queue.asyncAfter(
		deadline	: DispatchTime.now() + .nanoseconds( Int( p * Double( NSEC_PER_SEC ) ) )
	,	execute		: d
	)
}

func
ResourcePath( _ resource: String, _ type: String = "" ) -> String? {
	return Bundle.main.path( forResource: resource, ofType: type )
}

func
ResourceURL( _ resource: String, _ type: String = "" ) -> URL? {
	return Bundle.main.url( forResource: resource, withExtension: type )
}

func
DocumentDirectoryURLs() -> [ URL ] {
	return FileManager.default.urls( for: .documentDirectory, in: .userDomainMask ) as [ URL ]
}

func
DocumentDirectoryPathes() -> [ String ] {
	return NSSearchPathForDirectoriesInDomains(
		.documentDirectory
	,	.userDomainMask
	,	true
	) as [ String ]
}

func
Dist2( _ left: CGPoint, _ right: CGPoint ) -> Double {
	let w = Double( right.x - left.x )
	let h = Double( right.y - left.y )
	return w * w + h * h
}

func
Center( _ p: CGRect ) -> CGPoint {
	return CGPoint( x: p.midX, y: p.midY )
}

typealias	JSONDict = [ String: Any ]

func
FetchJSON( _ url: String ) -> Any? {
	if	let	wURL = URL( string: url ) {
		return try? DecodeJSON( Data( contentsOf: wURL ) )
	} else {
		return nil
	}
}

func
FetchArrayJSON( _ url: String ) -> [ Any ]? {
	return FetchJSON( url ) as? [ Any ]
}

func
FetchDictJSON( _ url: String ) -> JSONDict? {
	return FetchJSON( url ) as? JSONDict
}

func
BalancedPosition( _ p: Data ) -> Int? {

	var	wBalance = 0
	var	wInString = false
	var	wInBackSlash = false

	for i in 0 ..< p.count {
		if wInString {
			if wInBackSlash {
				wInBackSlash = false
			} else {
				switch p[ i ] {
				case 0x5c: //	\
					wInBackSlash = true
				case 0x22:
					wInString = false
				default:
					break
				}
			}
		} else {
			switch p[ i ] {
			case 0x5b, 0x7b: //	[	{
				wBalance = wBalance + 1
			case 0x5d, 0x7d: //	]	}
				if wBalance == 0 { return nil }
				wBalance = wBalance - 1
				if wBalance == 0 { return i + 1 }
			case 0x22:
				wInString = true
				wInBackSlash = false
			default:
				break
			}
		}
	}
	return nil
}

func
JSONForAll( _ data: Data, _ p: (Any) -> () ) {
	var	w = data
	while let wBP = BalancedPosition( w ) {
		do {
			p( try DecodeJSON( w.subdata( in: Range( uncheckedBounds: ( 0, wBP ) ) ) ) )
		} catch {
		}
		w = w.subdata( in: Range( uncheckedBounds: ( wBP, w.count ) ) )
	}
}

func
OnHTML(
  _	uri		: String
, _	method	: String
, _	body	: Data? = nil
, _	er		: @escaping ( Error ) -> () = { e in }
, _	ex		: @escaping ( HTTPURLResponse, Data ) -> () = { r, d in }
, _	ed		: @escaping ( Data ) -> () = { p in }
) {
	var	wR = URLRequest( url: URL( string: uri )! )
	wR.httpMethod = method
	if body != nil { wR.httpBody = body! }
	URLSession.shared.dataTask( with: wR ) { d, r, e in
		if let wE = e { er( wE ) }
		else {
			if	let	wR = r as? HTTPURLResponse
			,	let wD = d {
				switch wR.statusCode {
				case 200:
					ed( wD )
				default:
					ex( wR, wD )
				}
			} else {
				assert( false )
			}
		}
	}
}

func
OnJSON(
  _	uri		: String
, _	method	: String = "GET"
, _	json	: Any? = nil
, _	er		: @escaping ( Error ) -> () = { e in }
, _	ex		: @escaping ( HTTPURLResponse, Data ) -> () = { r, d in }
, _	ed		: @escaping ( Any ) -> () = { p in }
) {
	do {
		var	wBody	: Data?
		if let wJSON = json { wBody = try EncodeJSON( wJSON ) }
		OnHTML( uri, method, wBody, er, ex ) { p in
			do {
				ed( try DecodeJSON( p ) )
			} catch let e as NSError {
				er( e )
			} catch {
				assert( false )
			}
		}
	} catch let e as NSError {
		er( e )
	} catch {
		assert( false )
	}
}


func
ShowSharedCookies() {
	if let wCs = HTTPCookieStorage.shared.cookies {
		for w in wCs { print( w ) }
	}
}

func
DeleteSharedCookies() {
	let	wCS = HTTPCookieStorage.shared
	if let wCs = wCS.cookies {
		for w in wCs { wCS.deleteCookie( w ) }
	}
}

func
Request( _ p: String ) -> URLRequest? {
	if let w = URL( string: p ) {
		return URLRequest( url: w )
	} else {
		return nil
	}
}

func
LazyUTF8Data( _ p: Data ) -> Data {
	var v = Data()
	var	wIndex = 1
	var	wRemain = 0
	var	wBytes = [ UInt8 ]( repeating: 0, count: 4 )
	for var w in [ UInt8 ]( p ) {
		switch w {
		case 0 ..< 0x80:
			v.append( &w, count: 1 )
			wRemain = 0
		case 0xc2 ..< 0xe0:
			wBytes[ 0 ] = w
			wIndex = 1
			wRemain = 1
		case 0xe0 ..< 0xf0:
			wBytes[ 0 ] = w
			wIndex = 1
			wRemain = 2
		case 0xf0 ..< 0xf8:
			wBytes[ 0 ] = w
			wIndex = 1
			wRemain = 3
		case 0xf8 ..< 0xfc:
			wRemain = 0
//			wBytes[ 0 ] = w
//			wRemain = 4
		case 0xfc ..< 0xfe:
			wRemain = 0
//			wBytes[ 0 ] = w
//			wRemain = 5
		case 0x80 ..< 0xc0:
			if wRemain > 0 {
				wBytes[ wIndex ] = w
				wIndex += 1
				wRemain -= 1
				if wRemain == 0 { v.append( wBytes, count: wIndex ) }
			}
		default:
			wRemain = 0
		}
	}
	return v
}

/*
func
LazyUTF8String( _ p: Data ) -> String {
	var v = ""
	var	wRemain = 0
	var	wScalar = 0
	for w in [UInt8]( p ) {
		switch w {
		case 0 ..< 0x80:
			v += String( UnicodeScalar( w ) )
			wRemain = 0
		case 0xc2 ..< 0xe0:
			wScalar = Int( w ) & 0x1f
			wRemain = 1
		case 0xe0 ..< 0xf0:
			wScalar = Int( w ) & 0x0f
			wRemain = 2
		case 0xf0 ..< 0xf8:
			wScalar = Int( w ) & 0x07
			wRemain = 3
		case 0xf8 ..< 0xfc:
			wScalar = Int( w ) & 0x03
			wRemain = 4
		case 0xfc ..< 0xfe:
			wScalar = Int( w ) & 0x01
			wRemain = 5
		case 0x80 ..< 0xc0:
			if wRemain > 0 {
				wScalar = ( wScalar << 6 ) | ( Int( w ) & 0x3f )
				wRemain -= 1
				if wRemain == 0 { v += String( UnicodeScalar( w ) ) }
			}
		default:
			break
		}
	}
	return v
}
*/
