//  Written by Satoru Ogura, Tokyo on 2016/12/26.

import Accelerate

//
//	VECTOR - SCALAR
//

infix operator ++: AdditionPrecedence
infix operator --: AdditionPrecedence

func ++( _ p: [ Double ], _ s: Double ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsaddD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func ++( _ s: Double, _ p: [ Double ] ) -> [ Double ] { return p ++ s }

func --( _ p: [ Double ], _ s: Double ) -> [ Double ] { return p ++ -s }
func --( _ s: Double, _ p: [ Double ] ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfillD( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vsubD( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func * ( _ p: [ Double ], _ s: Double ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsmulD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func * ( _ s: Double, _ p: [ Double ] ) -> [ Double ] { return p * s }

func / ( _ p: [ Double ], _ s: Double ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsdivD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func / ( _ s: Double, _ p: [ Double ] ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfillD( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vdivD( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return v
}

//
//	VECTOR - VECTOR
//
func ++( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vaddD( l, 1, r, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func --( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vsubD( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func * ( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vmulD( l, 1, r, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func / ( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vdivD( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return v
}

func
DotProduct( _ l: [ Double ], _ r: [ Double ] ) -> Double {
	guard l.count == r.count else { fatalError() }
	var	v = 0.0
	vDSP_dotprD( r, 1, l, 1, &v, vDSP_Length( l.count ) )
	return v
}

struct
Matrix {
	var	m	:	UInt
	var	n	:	UInt
	var	u	:	[ Double ]
	init( _ m: UInt, _ n: UInt ) {
		self.m = m
		self.n = n
		u = [ Double ]( repeating: 0, count: Int( m * n ) )
	}
	init( _ m: UInt, _ n: UInt, _ u: [ Double ] ) {
		guard Int( m * n ) == u.count else { fatalError() }
		self.m = m
		self.n = n
		self.u = u
	}
	
	func
	Get( _ r: UInt, _ c: UInt ) -> Double {
		return u[ Int( r * n + c ) ]
	}
	
	func
	Row( _ p: CountableRange<UInt> ) -> Matrix {
		let w = Array( u[ Int( p.lowerBound * n ) ..< Int( p.upperBound * n ) ] )
		return Matrix( UInt( p.count ), n, w )
	}
	func
	Row( _ p: CountableClosedRange<UInt> ) -> Matrix {
		let w = Array( u[ Int( p.lowerBound * n ) ..< Int( ( p.upperBound + 1 ) * n ) ] )
		return Matrix( UInt( p.count ), n, w )
	}
	func
	Col( _ p: CountableRange<UInt> ) -> Matrix {
		var	v = Matrix( m, UInt( p.count ) )
		for i in 0 ..< Int( m ) {
			let	wVS = i * p.count
			let	wSS = i * Int( n )
			v.u[ wVS ..< ( wVS + p.count ) ] = u[ wSS + Int( p.lowerBound ) ..< wSS + Int( p.upperBound ) ]
		}
		return v
	}
	func
	Col( _ p: CountableClosedRange<UInt> ) -> Matrix {
		var	v = Matrix( m, UInt( p.count ) )
		for i in 0 ..< Int( m ) {
			let	wVS = i * p.count
			let	wSS = i * Int( n )
			v.u[ wVS ..< ( wVS + p.count ) ] = u[ wSS + Int( p.lowerBound ) ... wSS + Int( p.upperBound ) ]
		}
		return v
	}
}

prefix func
~ ( p: Matrix ) -> Matrix {
	var	v = Matrix( p.n, p.m )
	vDSP_mtransD( p.u, 1, &v.u, 1, v.m, v.n )
	return v
}
/*
prefix func
! ( p: Matrix ) -> Matrix {
	var v = p	//	Confirmed that u's entity is copied.
	var pivot : __CLPK_integer = 0
	var workspace = 0.0
	var error : __CLPK_integer = 0

	var N = __CLPK_integer( sqrt( Double( v.u.count ) ) )
	dgetrf_( &N, &N, &v.u, &N, &pivot, &error )
	guard error == 0 else { fatalError() }

	dgetri_( &N, &v.u, &N, &pivot, &workspace, &N, &error )
	guard error == 0 else { fatalError() }

	return v
}
*/
func
== ( l: Matrix, r: Matrix ) -> Bool {
	if l.m != r.m { return false }
	if l.n != r.n { return false }
	return l.u == r.u
}

func ++( p: Matrix, s: Double ) -> Matrix { return Matrix( p.m, p.n, p.u ++ s ) }
func ++( s: Double, p: Matrix ) -> Matrix { return Matrix( p.m, p.n, s ++ p.u ) }

func --( p: Matrix, s: Double ) -> Matrix { return Matrix( p.m, p.n, p.u -- s ) }
func --( s: Double, p: Matrix ) -> Matrix { return Matrix( p.m, p.n, s -- p.u ) }

func * ( p: Matrix, s: Double ) -> Matrix { return Matrix( p.m, p.n, p.u * s ) }
func * ( s: Double, p: Matrix ) -> Matrix { return Matrix( p.m, p.n, s * p.u ) }

func / ( p: Matrix, s: Double ) -> Matrix { return Matrix( p.m, p.n, p.u / s ) }
func / ( s: Double, p: Matrix ) -> Matrix { return Matrix( p.m, p.n, s / p.u ) }

func ^ ( p: Matrix, s: Double ) -> Matrix {
	var	t = Matrix( p.m, p.n )
	var	w = s
	vDSP_vfillD( &w, &t.u, 1, vDSP_Length( t.u.count ) )
	var	v = Matrix( p.m, p.n )
	var	wM = p
	var	wCount = Int32( v.u.count )
	vvpow( &v.u, &t.u, &wM.u, &wCount )
	return v
}

func
++( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	return Matrix( l.m, l.n, l.u ++ r.u )
}

func
--( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	return Matrix( l.m, l.n, l.u -- r.u )
}

func
* ( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	return Matrix( l.m, l.n, l.u * r.u )
}

func
/ ( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	return Matrix( l.m, l.n, l.u / r.u )
}

func
OutOfPlace( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.n == r.m else { fatalError() }
	var	v = Matrix( l.m, r.n )
	vDSP_mmulD( l.u, 1, r.u, 1, &v.u, 1, l.m, r.n, l.n )
	return v
}

func
I( _ p: UInt ) -> Matrix {
	var	v = Matrix( p, p )
	let	w = Int( p )
	for i in 0 ..< w {
		v.u[ i * w + i ] = 1
	}
	return v
}

func
JPMatrixTest() {

	let	w22 = [ 2.0, -2.0 ];

	guard w22 ++ 2 == [ 4,  0 ] else { fatalError() }
	guard w22 -- 2 == [ 0, -4 ] else { fatalError() }
	guard w22 *  2 == [ 4, -4 ] else { fatalError() }
	guard w22 /  2 == [ 1, -1 ] else { fatalError() }

	guard 2 ++ w22 == [ 4,  0 ] else { fatalError() }
	guard 2 -- w22 == [ 0,  4 ] else { fatalError() }
	guard 2 *  w22 == [ 4, -4 ] else { fatalError() }
	guard 2 /  w22 == [ 1, -1 ] else { fatalError() }

	guard w22 ++ w22 == [ 4, -4 ] else { fatalError() }
	guard w22 -- w22 == [ 0,  0 ] else { fatalError() }
	guard w22 *  w22 == [ 4,  4 ] else { fatalError() }
	guard w22 /  w22 == [ 1,  1 ] else { fatalError() }
	
	guard DotProduct( [ -4, -9 ], [ -1, 2 ] ) == -14 else { fatalError() }
	
	let	wM = Matrix( 2, 3, [ 1, 2, 3, 4, 5, 6 ] )
	guard wM ++ 2 == Matrix( 2, 3, [  3,  4,  5,  6,  7,  8 ] ) else { fatalError() }
	guard wM -- 2 == Matrix( 2, 3, [ -1,  0,  1,  2,  3,  4 ] ) else { fatalError() }
	guard wM *  2 == Matrix( 2, 3, [  2,  4,  6,  8, 10, 12 ] ) else { fatalError() }
	guard wM /  2 == Matrix( 2, 3, [  0.5,  1,  1.5,  2, 2.5, 3 ] ) else { fatalError() }
	guard 2 ++ wM == Matrix( 2, 3, [  3,  4,  5,  6,  7,  8 ] ) else { fatalError() }
	guard 2 -- wM == Matrix( 2, 3, [  1,  0, -1, -2, -3, -4 ] ) else { fatalError() }
	guard 2 *  wM == Matrix( 2, 3, [  2,  4,  6,  8, 10, 12 ] ) else { fatalError() }
	guard 2 /  wM == Matrix( 2, 3, [  2,  1,  2.0/3.0,  0.5, 0.4, 2.0/6.0 ] ) else { fatalError() }
	guard wM ++ wM == Matrix( 2, 3, [  2,  4,  6,  8, 10, 12 ] ) else { fatalError() }
	guard wM -- wM == Matrix( 2, 3, [  0,  0,  0,  0,  0,  0 ] ) else { fatalError() }
	guard wM *  wM == Matrix( 2, 3, [  1,  4,  9, 16, 25, 36 ] ) else { fatalError() }
	guard wM /  wM == Matrix( 2, 3, [  1,  1,  1,  1,  1,  1 ] ) else { fatalError() }
	
	guard OutOfPlace(
		wM
	,	Matrix( 3, 2, [ 1, 2, 3, 4, 5, 6 ] )
	) == Matrix( 2, 2, [ 22, 28, 49, 64 ] ) else {
		fatalError()
	}
	
}

/*
		1 2
1 2 3			1 +  6 + 15		2 +  8 + 18
		3 4
4 5 6			4 + 15 + 30		8 + 20 + 36
		5 6
*/

