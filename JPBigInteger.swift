//	Written by Satoru Ogura, Tokyo.
//
import Foundation

typealias	E	= UInt32
typealias	E2	= UInt64

//typealias	E	= UInt8
//typealias	E2	= UInt16

let	NUM_BITS	= MemoryLayout< E >.size * 8 - 1
let	BORDER		= E( 1 << NUM_BITS )

enum
BigIntegerError	: Error {
	case	numberFormat( String )
	case	divideByZero
}

class
EsUtil {
	var	m		: [ E ]
	init()							{ m = [ E ]() }
	init( _ p	: ArraySlice< E > )	{ m = Array( p ) }
	init( _ p	: Int )				{ m = Array( repeating: 0, count: p ) }

	func
	AddDigit( _ p: Int, _ radix: Int ) {
		var	wCarry = E2( p )
		for i in 0 ..< m.count {
			let w = E2( m[ i ] ) * E2( radix ) + wCarry
			m[ i ] = E( w % E2( BORDER ) )
			wCarry = w >> E2( NUM_BITS )
		}
		if wCarry > 0 { m.append( E( wCarry ) ) }
	}
	func
	CarryAll() {
		if m.count > 0 {
			for i in 1 ..< m.count {
				if m[ i - 1 ] >= BORDER {
					m[ i - 1 ] %= BORDER
					m[ i ] += 1
				}
			}
			if m[ m.count - 1 ] >= BORDER {
				m[ m.count - 1 ] %= BORDER
				m.append( 1 )
			}
		}
	}
	func
	Normalize() {
		while m.last == 0 { m.removeLast() }
	}
	func
	BorrowAll() {
		for i in 1 ..< m.count {
			if m[ i - 1 ] >= BORDER {
				m[ i - 1 ] %= BORDER
				m[ i ] = m[ i ] > 0 ? m[ i ] - 1 : E( BORDER ) - 1 + E( BORDER )
			}
		}
		Normalize()
	}
	func
	Add( _ p: E, _ at: Int = 0 ) {
		var	wCarry = p
		for i in at ..< m.count {
			m[ i ] += wCarry
			wCarry = m[ i ] >> E( NUM_BITS )
			m[ i ] %= BORDER
		}
		if wCarry > 0 { m.append( E( wCarry ) ) }
	}
	func
	Add2N( _ p: Int ) {
		let	wAt = p / NUM_BITS
		while m.count <= wAt { m.append( 0 ) }
		Add( 1 << E( p - wAt * NUM_BITS ), wAt )
	}

	func
	Minus( _ p: E, _ at: Int = 0 ) {
		if m[ at ] >= p { m[ at ] -= p } else { m[ at ] = m[ at ] + BORDER - p + BORDER }
		var	i = at + 1
		while i < m.count {
			if m[ i - 1 ] >= BORDER {
				m[ i - 1 ] %= BORDER
				m[ i ] += 1
				i += 1
			} else {
				break
			}
		}
		if m[ m.count - 1 ] >= BORDER {
			m[ m.count - 1 ] %= BORDER
			m.append( 1 )
		}
	}
	func
	Div2N( _ p: Int ) {
		for _ in 0 ..< p / NUM_BITS { m.removeFirst() }
		let wNumBits = E( p % NUM_BITS )
		if wNumBits > 0 {
			m[ 0 ] >>= wNumBits
			for i in 1 ..< m.count {
				m[ i - 1 ] |= ( m[ i ] << ( E( NUM_BITS ) - wNumBits ) ) % BORDER
				m[ i ] >>= wNumBits
			}
			Normalize()
		}
	}
	func
	Mul2N( _ p: Int ) {
		let wNumBits = E( p % NUM_BITS )
		if wNumBits > 0 {
			m.append( 0 )
			for i in ( 1 ..< m.count ).reversed() {
				m[ i ] |= ( m[ i - 1 ] >> ( E( NUM_BITS ) - wNumBits ) ) % BORDER
				m[ i - 1 ] = ( m[ i - 1 ] << wNumBits ) % BORDER
			}
			Normalize()
		}
		for _ in 0 ..< Int( p ) / NUM_BITS { m.insert( 0, at:0 ) }
	}
}

func
NumBits( _ p: ArraySlice< E > ) -> Int {
	if p.count == 0 { return 0 }
	var v = p.count * NUM_BITS
	var	w = p[ p.startIndex + p.count - 1 ]
	for _ in 0 ..< NUM_BITS {
		w <<= E( 1 )
		if w & BORDER > 0 { break }
		v -= 1
	}
	return v
}

func
FloatValue( _ p: ArraySlice< E > ) -> Float64 {
	var	v = 0 as Float64
	for w in p.reversed() { v = v * Float64( BORDER ) + Float64( w ) }
	return v
}

func
Digits( _ p: ArraySlice< E >, _ radix: Int ) -> String {
	if p.count == 0 { return "0" }
	var wEs = Array( p )
	var	v = ""
	while wEs.count > 0 {
		var i = wEs.count - 1
		var	wBorrow = E2( wEs[ i ] ) % E2( radix )
		wEs[ i ] /= E( radix )
		if wEs[ i ] == 0 { wEs.removeLast() }
		while i > 0 {
			i -= 1;
			let w = ( wBorrow << E2( NUM_BITS ) ) + E2( wEs[ i ] )
			wEs[ i ] = E( w / E2( radix ) )
			wBorrow = w % E2( radix )
		}
		if wBorrow < 10 {
			v = "\(String(describing: UnicodeScalar( Int( wBorrow ) + Int( ( "0" as UnicodeScalar ).value ) )!))" + v
		} else {
			v = "\(String(describing: UnicodeScalar( Int( wBorrow ) - 10 + Int( ( "A" as UnicodeScalar ).value ) )!))" + v
		}
	}
	return v
}

open	class
BigInteger : CustomStringConvertible, CustomDebugStringConvertible {

	var	m		: [ E ]
	let	minus	: Bool

	init( _ p: [ E ] = [ E ](), _ minus: Bool = false ) {
		m = p
		self.minus = minus
	}
	
	open	func
	StringRepresentation( _ radix: Int ) -> String {
		let	v = Digits( ArraySlice( m ), radix )
		return minus ? "-" + v : v
	}
	open	var
	description		: String {
		return StringRepresentation( 10 )
	}
	open	var
	debugDescription: String {
		return StringRepresentation( 10 )
	}
	var
	NativeFloat		: Float64 {
		let	v = FloatValue( ArraySlice( m ) )
		return minus ? -v : v
	}
	var
	NativeInt		: Int {
		let	v = m.count == 0 ? 0 : Int( m[ 0 ] )
		return minus ? -v : v
	}
}

func
MakeBigInteger( _ p: Int ) -> BigInteger {
	var	w		= p
	var	wMinus	= false
	if w < 0 {
		w = -w
		wMinus = true
	}

	let	v = EsUtil();
	while w > 0 {
		v.m.append( E( w % Int( BORDER ) ) )
		w /= Int( BORDER )
	}
	return BigInteger( v.m, wMinus )
}

func
MakeBigInteger( _ p: String, _ radix: Int = 10 ) -> BigInteger? {

	let	wR = StringUnicodeReader( p )

	var	wMinus = false
	do {
		let w = try wR.Read()
		switch w {
		case "0" ... "9"	: wR.Unread( w )
		case "+"			: break
		case "-"			: wMinus = true
		default				: return nil
		}
	} catch {
		return nil
	}

	let	wEU = EsUtil();
	repeat {
		do {
			let w = try wR.Read()
			switch w {
			case "0" ... "9":
				let	w = Int( w.value ) - Int( ( "0" as UnicodeScalar ).value )
				guard w < radix else { return nil }
				wEU.AddDigit( w, radix )
			case "A" ... "Z":
				let	w = Int( w.value ) - Int( ( "A" as UnicodeScalar ).value ) + 10
				guard w < radix else { return nil }
				wEU.AddDigit( w, radix )
			case "a" ... "z":
				let	w = Int( w.value ) - Int( ( "a" as UnicodeScalar ).value ) + 10
				guard w < radix else { return nil }
				wEU.AddDigit( w, radix )
			default:
				return nil
			}
		} catch ReaderError.eod {
			return BigInteger( wEU.m, wMinus )
		} catch {
			return nil
		}
	} while true
}

func
_Compare( _ l: ArraySlice< E >, _ r: ArraySlice< E > ) -> Int {
	if l.count == r.count {
		for i in ( 0 ..< l.count ).reversed() {
			if l[ l.startIndex + i ] > r[ r.startIndex + i ] { return  1 }
			if l[ l.startIndex + i ] < r[ r.startIndex + i ] { return -1 }
		}
		return 0
	} else {
		return l.count > r.count ? 1 : -1
	}
}

public	func
Compare( _ l: BigInteger, _ r: BigInteger ) -> Int {
	if l.m.count == 0 && r.m.count == 0 { return 0 }
	return l.minus
	?	r.minus ? -_Compare( ArraySlice( l.m ) , ArraySlice( r.m )  ) : -1
	:	r.minus ?  1 :  _Compare( ArraySlice( l.m ) , ArraySlice( r.m )  )
}

func
_IsZero( _ p: ArraySlice< E > ) -> Bool {
	return p.count == 0
}

public	func
IsZero( _ p: BigInteger ) -> Bool {
	return _IsZero( ArraySlice( p.m ) )
}

public	func
==( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) == 0 }

public	func
!=( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) != 0 }

public	func
>( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) == 1 }

public	func
>=( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) != -1 }

public	func
<( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) == -1 }

public	func
<=( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) != 1 }

func
_Plus( _ b: ArraySlice< E >, _ s: ArraySlice< E > ) -> [ E ] {
	let	v = EsUtil( b )
	for ( i, w ) in s.enumerated() { v.m[ i ] += w }
	v.CarryAll()
	return v.m
}

func
Plus( _ l: ArraySlice< E >, _ r: ArraySlice< E > ) -> [ E ] {
	return l.count >= r.count ? _Plus( l, r ) : _Plus( r, l )
}

func
_Minus( _ b: ArraySlice< E >, _ s: ArraySlice< E > ) -> [ E ] {
	let	v = EsUtil( b )
	for ( i, w ) in s.enumerated() {
		if v.m[ i ] >= w {
			v.m[ i ] -= w
		} else {
			v.m[ i ] = v.m[ i ] + E( BORDER ) - w + E( BORDER )
		}
	}
	v.BorrowAll()
	return v.m
}
func
Minus( _ l: ArraySlice< E >, _ r: ArraySlice< E > ) -> ( reverse: Bool, [ E ] ) {
	return _Compare( l, r ) >= 0
	?	( false, _Minus( l, r ) )
	:	(  true, _Minus( r, l ) )
}

func
Mul( _ l: ArraySlice< E >, _ r: ArraySlice< E > ) -> [ E ] {
	let v = EsUtil( l.count + r.count )
	for il in 0 ..< l.count {
		for ir in 0 ..< r.count {
			let w = E2( l[ l.startIndex + il ] ) * E2( r[ r.startIndex + ir ] )
			v.Add( E( w % E2( BORDER ) ), il + ir )
			v.Add( E( w >> E2( NUM_BITS ) ), il + ir + 1 )
		}
	}
	v.Normalize()
	return v.m
}

func
QR( _ l: ArraySlice< E >, _ r: ArraySlice< E > ) -> ( [ E ], [ E ] ) {

	let vQuotient = EsUtil()
	let	vRemainder = EsUtil( l )
	let wRNB = NumBits( r )
	
	while true {
		let	wLNB = NumBits( ArraySlice( vRemainder.m ) )
		if wLNB > wRNB {
			let	wR = EsUtil( r )
			let	wNB = wLNB - wRNB
			wR.Mul2N( wNB )
			if _Compare( ArraySlice( vRemainder.m ), ArraySlice( wR.m ) ) == -1 {
				vQuotient.Add2N( wNB - 1 )
				wR.Div2N( 1 )
			} else {
				vQuotient.Add2N( wNB )
			}
			vRemainder.m = _Minus( ArraySlice( vRemainder.m ), ArraySlice( wR.m ) )
		} else { break }
	}
	if _Compare( ArraySlice( vRemainder.m ), r ) != -1 {
		vQuotient.Add( 1 )
		vRemainder.m = _Minus( ArraySlice( vRemainder.m ), r )
	}
	return ( vQuotient.m, vRemainder.m )
}

public	func
+( l: BigInteger, r: BigInteger ) -> BigInteger {
	if r.m.count == 0 { return l }
	if l.m.count == 0 { return r }
	switch ( l.minus, r.minus ) {
	case ( false, false ):	//	+	+
		return BigInteger( Plus( ArraySlice( l.m ), ArraySlice( r.m ) ), false )
	case ( false, true ):	//	+	-
		let ( wRev, wEs ) = Minus( ArraySlice( l.m ), ArraySlice( r.m ) )
		return BigInteger( wEs, wRev )
	case ( true, false ):	//	-	+
		let ( wRev, wEs ) = Minus( ArraySlice( l.m ), ArraySlice( r.m ) )
		return BigInteger( wEs, !wRev )
	case ( true, true ):	//	-	-
		return BigInteger( Plus( ArraySlice( l.m ), ArraySlice( r.m ) ), true )
	}
}

public	func
-( l: BigInteger, r: BigInteger ) -> BigInteger {
	if r.m.count == 0 { return l }
	if l.m.count == 0 { return BigInteger( r.m, !r.minus ) }
	switch ( l.minus, r.minus ) {
	case ( false, false ):	//	+	+
		let ( wRev, wEs ) = Minus( ArraySlice( l.m ), ArraySlice( r.m ) )
		return BigInteger( wEs, wRev )
	case ( false, true ):	//	+	-
		return BigInteger( Plus( ArraySlice( l.m ), ArraySlice( r.m ) ), false )
	case ( true, false ):	//	-	+
		return BigInteger( Plus( ArraySlice( l.m ), ArraySlice( r.m ) ), true )
	case ( true, true ):	//	-	-
		let ( wRev, wEs ) = Minus( ArraySlice( l.m ), ArraySlice( r.m ) )
		return BigInteger( wEs, !wRev )
	}
}

public	func
*( l: BigInteger, r: BigInteger ) -> BigInteger {
	if r.m.count == 0 { return r }
	if l.m.count == 0 { return l }
	return BigInteger( Mul( ArraySlice( l.m ), ArraySlice( r.m ) ), l.minus != r.minus )
}

public	func
/( l: BigInteger, r: BigInteger ) throws -> BigInteger {
	if r.m.count == 0 { throw BigIntegerError.divideByZero }
	if l.m.count == 0 { return l }
	return BigInteger( QR( ArraySlice( l.m ), ArraySlice( r.m ) ).0, l.minus != r.minus )
}

public	func
%( l: BigInteger, r: BigInteger ) throws -> BigInteger {
	if r.m.count == 0 { throw BigIntegerError.divideByZero }
	if l.m.count == 0 { return l }
	return BigInteger( QR( ArraySlice( l.m ), ArraySlice( r.m ) ).1, l.minus != r.minus )
}

func
And( _ b: ArraySlice< E >, _ s: ArraySlice< E > ) -> [ E ] {
	let	v = EsUtil( b )
	for i in 0 ..< s.count { v.m[ i ] &= s[ s.startIndex + i ] }
	v.Normalize()
	return v.m
}
public	func
&( l: BigInteger, r: BigInteger ) -> BigInteger {
	let	v = l.m.count >= r.m.count ? And( ArraySlice( l.m ), ArraySlice( r.m ) ) : And( ArraySlice( r.m ), ArraySlice( l.m ) )
	return BigInteger( v, l.minus && r.minus )
}

func
Or( _ b: ArraySlice< E >, _ s: ArraySlice< E > ) -> [ E ] {
	var	v = Array( b )
	for i in 0 ..< s.count { v[ i ] |= s[ s.startIndex + i ] }
	return v
}
public	func
|( l: BigInteger, r: BigInteger ) -> BigInteger {
	let	v = l.m.count >= r.m.count ? Or( ArraySlice( l.m ), ArraySlice( r.m ) ) : Or( ArraySlice( r.m ), ArraySlice( l.m ) )
	return BigInteger( v, l.minus || r.minus )
}

func
XOr( _ b: ArraySlice< E >, _ s: ArraySlice< E > ) -> [ E ] {
	let	v = EsUtil( b )
	for i in 0 ..< s.count { v.m[ i ] ^= s[ s.startIndex + i ] }
	v.Normalize()
	return v.m
}
public	func
^( l: BigInteger, r: BigInteger ) -> BigInteger {
	let	v = l.m.count >= r.m.count ? XOr( ArraySlice( l.m ), ArraySlice( r.m ) ) : XOr( ArraySlice( r.m ), ArraySlice( l.m ) )
	return BigInteger( v, l.minus != r.minus )
}
