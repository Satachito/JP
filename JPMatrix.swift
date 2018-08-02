//  Written by Satoru Ogura, Tokyo on 2016/12/26.

import Accelerate

//
//	VECTOR
//

func
Arange( _ p: Int ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p )
	for i in 0 ..< p { v[ i ] = Double( i ) }
	return v
}

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
	var	numR	:	UInt
	var	numC	:	UInt
	var	u		:	[ Double ]
	init( _ pNumR: UInt, _ pNumC: UInt ) {
		self.numR = pNumR
		self.numC = pNumC
		u = [ Double ]( repeating: 0, count: Int( pNumR * pNumC ) )
	}
	init( _ pNumR: UInt, _ pNumC: UInt, _ u: [ Double ] ) {
		guard Int( pNumR * pNumC ) == u.count else { fatalError() }
		self.numR = pNumR
		self.numC = pNumC
		self.u = u
	}

	subscript( _ r: UInt, _ c: UInt ) -> Double {
		get {
			guard r < numR && c < numR else { fatalError() }
			return u[ Int( r * numC + c ) ]
		}
		set {
			guard r < numR && c < numR else { fatalError() }
			u[ Int( r * numC + c ) ] = newValue
		}
	}
	
	func
	Row( _ p: CountableRange<UInt> ) -> Matrix {
		let w = Array( u[ Int( p.lowerBound * numC ) ..< Int( p.upperBound * numC ) ] )
		return Matrix( UInt( p.count ), numC, w )
	}
	func
	Row( _ p: CountableClosedRange<UInt> ) -> Matrix {
		let w = Array( u[ Int( p.lowerBound * numC ) ... Int( p.upperBound * numC ) ] )
		return Matrix( UInt( p.count ), numC, w )
	}
	func
	Col( _ p: CountableRange<UInt> ) -> Matrix {
		var	v = Matrix( numR, UInt( p.count ) )
		for i in 0 ..< Int( numR ) {
			let	wVS = i * p.count
			let	wSS = i * Int( numC )
			v.u[ wVS ..< wVS + p.count ] = u[ wSS + Int( p.lowerBound ) ..< wSS + Int( p.upperBound ) ]
		}
		return v
	}
	func
	Col( _ p: CountableClosedRange<UInt> ) -> Matrix {
		var	v = Matrix( numR, UInt( p.count ) )
		for i in 0 ..< Int( numR ) {
			let	wVS = i * p.count
			let	wSS = i * Int( numC )
			v.u[ wVS ... wVS + p.count ] = u[ wSS + Int( p.lowerBound ) ... wSS + Int( p.upperBound ) ]
		}
		return v
	}
}

func
RandomMatrix( _ numR: UInt, _ numC: UInt ) -> Matrix {
	var	v = [ Double ]( repeating: 0, count: Int( numR * numC ) )
	for i in 0 ..< v.count { v[ i ] = Double.random( in: 0 ..< 1 ) }
	return Matrix( numR, numC, v )
}

func
Reshape( _ p: [ Double ], _ numR: UInt, _ numC: UInt ) -> Matrix {
	guard numR * numC == p.count else { fatalError() }
	return Matrix( numR, numC, p )
}

func
HStack( _ p: [ Matrix ] ) -> Matrix {
	guard p.count > 0 else { fatalError() }
	var	wNumC = p[ 0 ].numC
	for i in 1 ..< p.count {
		guard p[ 0 ].numR == p[ i ].numR else { fatalError() }
		wNumC += p[ i ].numC
	}
	var	v = Matrix( p[ 0 ].numR, wNumC )
	for i in 0 ..< Int( v.numR ) {
		var	wVS = i * Int( v.numC )
		for j in 0 ..< p.count {
			let	wSpan = Int( p[ j ].numC )
			let	wPS = i * wSpan
			v.u[ wVS ..< wVS + wSpan ] = p[ j ].u[ wPS ..< wPS + wSpan ]
			wVS += wSpan
		}
	}
	return v
}

func
VStack( _ p: [ Matrix ] ) -> Matrix {
	guard p.count > 0 else { fatalError() }
	var	wNumR = p[ 0 ].numR
	for i in 1 ..< p.count {
		guard p[ 0 ].numC == p[ i ].numC else { fatalError() }
		wNumR += p[ i ].numR
	}
	var	v = Matrix( wNumR, p[ 0 ].numC )
	var	wVS = 0
	for i in 0 ..< p.count {
		let	wSize = Int( p[ i ].numR * p[ i ].numC )
		v.u[ wVS ..< wVS + wSize ] = ArraySlice( p[ i ].u )
		wVS += wSize
	}
	return v
}

prefix func
~ ( p: Matrix ) -> Matrix {
	var	v = Matrix( p.numC, p.numR )
	vDSP_mtransD( p.u, 1, &v.u, 1, v.numC, v.numR )
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
	if l.numR != r.numR { return false }
	if l.numC != r.numC { return false }
	return l.u == r.u
}

func ++( p: Matrix, s: Double ) -> Matrix { return Matrix( p.numR, p.numC, p.u ++ s ) }
func ++( s: Double, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s ++ p.u ) }

func --( p: Matrix, s: Double ) -> Matrix { return Matrix( p.numR, p.numC, p.u -- s ) }
func --( s: Double, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s -- p.u ) }

func * ( p: Matrix, s: Double ) -> Matrix { return Matrix( p.numR, p.numC, p.u * s ) }
func * ( s: Double, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s * p.u ) }

func / ( p: Matrix, s: Double ) -> Matrix { return Matrix( p.numR, p.numC, p.u / s ) }
func / ( s: Double, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s / p.u ) }

func ^ ( p: Matrix, s: Double ) -> Matrix {
	var	t = Matrix( p.numR, p.numC )
	var	w = s
	vDSP_vfillD( &w, &t.u, 1, vDSP_Length( t.u.count ) )
	var	v = Matrix( p.numR, p.numC )
	var	wM = p
	var	wCount = Int32( v.u.count )
	vvpow( &v.u, &t.u, &wM.u, &wCount )
	return v
}

func
++( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u ++ r.u )
}

func
--( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u -- r.u )
}

func
* ( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u * r.u )
}

func
/ ( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u / r.u )
}

func
OutOfPlace( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numC == r.numR else { fatalError() }
	var	v = Matrix( l.numR, r.numC )
	vDSP_mmulD( l.u, 1, r.u, 1, &v.u, 1, l.numR, r.numC, l.numC )
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

