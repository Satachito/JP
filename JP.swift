//	Written by Satoru Ogura, Tokyo.
//
import	Foundation
import	CoreGraphics

enum
JPError: Error {
	case e(String)
}

func
HexString( _ p: [ UInt8 ] ) -> String {
	func
	HexChar( _ p: Int ) -> String {
		return [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" ][ p ]
	}
	return p.reduce( "" ) {  $0 + HexChar( Int( $1 ) >> 4 ) + HexChar( Int( $1 ) ) }
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
ShuffledIndices( _ p: Int ) -> [ Int ] {
	return ( 0 ..< p ).shuffled()
}

func
ToArray<T>( _ start: UnsafePointer<T>, _ count: Int ) -> [ T ] {
	return Array( UnsafeBufferPointer( start: start, count: count ) )
}
//	USAGE: let wArray : [ Int16 ] = ToArray( data.bytes, data.length / sizeof( Int16 ) )

func
Size( file path: String ) -> Int? {
	return ( try? FileManager.default.attributesOfItem( atPath: path ) )?[ .size ] as? Int
}

func
LengthByUTF8( _ p: String ) -> Int {
	return p.lengthOfBytes( using: .utf8 )
}

func
DataByUTF8( _ p: String ) -> Data? {
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
DataByBase64( _ p: String, _ options: Data.Base64DecodingOptions = [] ) -> Data? {
	return Data( base64Encoded: p, options: options )
}

func
JSONData( _ p: Any, _ options: JSONSerialization.WritingOptions = [] ) throws -> Data {
	return try JSONSerialization.data( withJSONObject: p, options: options )
}

func
JSONString( _ p: Any, _ options: JSONSerialization.WritingOptions = [ .prettyPrinted, .sortedKeys ]  ) -> String? {
	guard let v = try? JSONData( p, options ) else { return nil }
	return UTF8String( v )
}

func
DecodeJSON( _ p: Data, _ options: JSONSerialization.ReadingOptions = [] ) throws -> Any {
	return try JSONSerialization.jsonObject( with: p, options: options )
}

func
DecodeJSON( _ p: String, _ options: JSONSerialization.ReadingOptions = [] ) -> Any? {
	guard let v = DataByUTF8( p ) else { return nil }
	return try? DecodeJSON( v, options )
}

typealias	JSONDict = [ String: Any ]

class
Chain< T > {
	var
	m			: T
	let
	next		: Chain?
	init(	_ p	: T, _ pNext: Chain? = nil ) { m = p; next = pNext }
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
/*
	var	wArray = [ UInt8 ]();
	wData.withUnsafeBytes { ( p: UnsafePointer<UInt8> ) in
		wArray = ToArray( p, 16 )
	}

	var	wArray = wData.withUnsafeBytes{ ToArray( $0.load( as: [ UInt8 ].self ), 16 ) }
	var	wStr = ""
	for i in 0 ..< 16 { wStr += String( format: "%02x", wArray[ i ] ) }
*/


	let wStr = "今日は、Alberto López.☕️";
	assert( wStr == UTF8String( DataByUTF8( wStr )! ) )
	assert( wData == DataByBase64( Base64String( wData ) )! )
	
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


class
Reader< T > {
	var
	_unread : Chain< T >?
	func
	_Read() -> T? {
		fatalError()
	}
	func
	Read() -> T? {
		if let v = _unread { _unread = v.next ; return v.m }
		return _Read()
	}
	func
	Unread( _ p: T ) { _unread = Chain< T >( p, _unread ) }
}

func
SkipWhite( _ p: Reader< UnicodeScalar > ) {
	while true {
		guard let w = p.Read() else { break }
		if !CharacterSet.whitespacesAndNewlines.contains( w ) {
			p.Unread( w )
			return
		}
	}
}

class
StdinUnicodeReader: Reader< UnicodeScalar > {
	var
	m		: String.UnicodeScalarView.Iterator?
	override func
	_Read() -> UnicodeScalar? {
		repeat {
			if m == nil {
				guard let w = readLine( strippingNewline: false ) else { return nil }
				m = w.unicodeScalars.makeIterator()
			}
			if let v = m!.next() { return v }
			m = nil
		} while true
	}
}

class
StringUnicodeReader: Reader< UnicodeScalar > {
	var
	m		: String.UnicodeScalarView.Iterator

	init( _ p: String ) {
		m = p.unicodeScalars.makeIterator()
	}

	override func
	_Read() -> UnicodeScalar? {
		return m.next()
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
Main( _ d: () -> () ) {
	DispatchQueue.main.sync( execute: d )
}

func
Delay( _ d: @escaping () -> () ) {
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

func
After( _ p: TimeInterval, _ queue: DispatchQueue = DispatchQueue.main, d: @escaping () -> () ) {
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
		if let wJSON = json { wBody = try JSONData( wJSON ) }
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

func
NormalRandom() -> Float {
	return
		sqrt( -2 * log( Float.random( in: 0 ..< 1 ) ) )
	*	sin( 2 * .pi * Float.random( in: 0 ..< 1 ) )
}
func
NormalRandom() -> Double {
	return
		sqrt( -2 * log( Double.random( in: 0 ..< 1 ) ) )
	*	sin( 2 * .pi * Double.random( in: 0 ..< 1 ) )
}

