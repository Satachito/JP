//  Written by Satoru Ogura, Tokyo on 2016/12/26.
//

func
Square( _ p: Float ) -> Float {
	return p * p
}

func
Cube( _ p: Float ) -> Float {
	return p * p * p
}

func
Gaussian() -> Float {
	return
		sqrt( -2 * log( Float.random( in: 0 ..< 1 ) ) )
	*	sin( 2 * .pi * Float.random( in: 0 ..< 1 ) )
}

import Accelerate

//
//	VECTOR
//

func
Ramp( _ p: Int, _ pInit: Float = 0, _ pStep: Float = 1 ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p )
	var	wInit = pInit
	var	wStep = pStep
	vDSP_vramp( &wInit, &wStep, &v, 1, vDSP_Length( p ) )
	return v
}

infix operator ++: AdditionPrecedence
infix operator --: AdditionPrecedence

//
//	VECTOR - SCALAR
//	SCALAR - VECTOR
//

func +( _ p: [ Float ], _ s: Float ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsadd( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func +( _ s: Float, _ p: [ Float ] ) -> [ Float ] { return p + s }

func -( _ p: [ Float ], _ s: Float ) -> [ Float ] { return p + -s }
func -( _ s: Float, _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfill( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vsub( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func *( _ p: [ Float ], _ s: Float ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsmul( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func *( _ s: Float, _ p: [ Float ] ) -> [ Float ] { return p * s }

func /( _ p: [ Float ], _ s: Float ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsdiv( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func /( _ s: Float, _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfill( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vdiv( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return v
}

//
//	VECTOR - VECTOR
//
func +( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vadd( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func -( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vsub( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func *( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vmul( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func /( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vdiv( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return v
}

func
DotProduct( _ l: [ Float ], _ r: [ Float ] ) -> Float {
	guard l.count == r.count else { fatalError() }
	var	v: Float = 0.0
	vDSP_dotpr( r, 1, l, 1, &v, vDSP_Length( l.count ) )
	return v
}

//
//	Mat
//

@objc class
Mat: NSObject {

	var	u		: [ Float ]
	var	degrees	: [ Int ]

	init( _ p: [ Float ], _ pDegrees: [ Int ] ) {
		u = p
		degrees = pDegrees
	}

	init( _ p: [ [ Float ] ] ) {
		var	wNumAll = 0
		degrees = [ Int ]( repeating: 0, count: p.count + 1 )
		for i in 0 ..< p.count {
			wNumAll += p[ i ].count
			degrees[ i + 1 ] = wNumAll
		}
		u = [ Float ]( repeating: 0, count: wNumAll )
		for i in 0 ..< p.count {
			u[ degrees[ i ] ..< degrees[ i + 1 ] ] = p[ i ][ 0 ..< degrees[ i + 1 ] - degrees[ i ] ]
		}
	}

	subscript( _ r: Int ) -> [ Float ] {
		get {
			guard r < numR else { fatalError() }
			return Array( u[ degrees[ r ] ..< degrees[ r + 1 ] ] )
		}
		set {
			guard r < numR else { fatalError() }
			u[ degrees[ r ] ..< degrees[ r + 1 ] ] = newValue[ 0 ..< degrees[ r + 1 ] - degrees[ r ] ]
		}
	}

	lazy var
	count: Int = {
		return degrees.last!
	}()
	
	lazy var
	numR: Int = {
		return degrees.count - 1
	}()
}

func
== ( l: Mat, r: Mat ) -> Bool {
	if l.degrees != r.degrees { return false }
	return l.u == r.u
}

//
//	MAT - SCALAR
//	SCALAR - MAT
//

func +( _ p: Mat, _ s: Float ) -> Mat {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsadd( p.u, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return Mat( v, p.degrees )
}
func +( _ s: Float, _ p: Mat ) -> Mat { return p + s }

func -( _ p: Mat, _ s: Float ) -> Mat { return p + -s }
func -( _ s: Float, _ p: Mat ) -> Mat {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfill( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vsub( p.u, 1, v, 1, &v, 1, vDSP_Length( v.count ) )
	return Mat( v, p.degrees )
}

func *( _ p: Mat, _ s: Float ) -> Mat {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsmul( p.u, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return Mat( v, p.degrees )
}
func *( _ s: Float, _ p: Mat ) -> Mat { return p * s }

func /( _ p: Mat, _ s: Float ) -> Mat {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsdiv( p.u, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return Mat( v, p.degrees )
}
func /( _ s: Float, _ p: Mat ) -> Mat {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfill( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vdiv( p.u, 1, v, 1, &v, 1, vDSP_Length( v.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return Mat( v, p.degrees )
}


//
//	MAT - MAT
//
func +( _ l: Mat, _ r: Mat ) -> Mat {
	guard l.degrees == r.degrees else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vadd( r.u, 1, l.u, 1, &v, 1, vDSP_Length( v.count ) )
	return Mat( v, l.degrees )
}

func -( _ l: Mat, _ r: Mat ) -> Mat {
	guard l.degrees == r.degrees else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vsub( r.u, 1, l.u, 1, &v, 1, vDSP_Length( v.count ) )
	return Mat( v, l.degrees )
}

func *( _ l: Mat, _ r: Mat ) -> Mat {
	guard l.degrees == r.degrees else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vmul( r.u, 1, l.u, 1, &v, 1, vDSP_Length( v.count ) )
	return Mat( v, l.degrees )
}

func /( _ l: Mat, _ r: Mat ) -> Mat {
	guard l.degrees == r.degrees else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vdiv( r.u, 1, l.u, 1, &v, 1, vDSP_Length( v.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return Mat( v, l.degrees )
}


//
//	Matrix
//
@objc class
Matrix: NSObject {
	var	numR	:	Int
	var	numC	:	Int
	var	u		:	[ Float ]
	
	func
	Dump() {
		print( "\( numR ):\( numC ):\( u )" )
 	}

	init( _ pNumR: Int = 0, _ pNumC: Int = 0, _ pInitial: Float = 0 ) {
		numR = pNumR
		numC = pNumC
		u = [ Float ]( repeating: pInitial, count: numR * numC )
	}
	init( _ pNumR: Int, _ pNumC: Int, _ p: [ Float ] ) {
		guard pNumR * pNumC == p.count else { fatalError() }
		numR = pNumR
		numC = pNumC
		u = p
	}
	init( _ p: [ [ Float ] ] ) {
		numR = p.count
		guard numR > 0 else { fatalError( "Need to have more than one element." ) }
		numC = p[ 0 ].count
		u = [ Float ]( repeating: 0, count: numR * numC )
		for i in 0 ..< numR {
			guard p[ i ].count == numC else { fatalError( "All the rows must have same count." ) }
			let	w = i * numC
			u[ w ..< w + numC ] = ArraySlice( p[ i ] )
		}
	}

	subscript( _ r: Int ) -> ArraySlice< Float > {
		get {
			guard r < numR else { fatalError() }
			let	w = r * numC
			return u[ w ..< w + numC ]
		}
		set {
			guard r < numR else { fatalError() }
			let	w = r * numC
			u[ w ..< w + numC ] = newValue
		}
	}

	subscript( _ r: Int, _ c: Int ) -> Float {
		get {
			guard r < numR && c < numC else { fatalError() }
			return u[ r * numC + c ]
		}
		set {
			guard r < numR && c < numC else { fatalError() }
			u[ r * numC + c ] = newValue
		}
	}
	
	func
	Rows( _ p: Range<Int> ) -> Matrix {
		let w = Array( u[ p.lowerBound * numC ..< p.upperBound * numC ] )
		return Matrix( p.count, numC, w )
	}
	func
	Cols( _ p: Range<Int> ) -> Matrix {
		let	v = Matrix( numR, p.count )
		for i in 0 ..< numR {
			let	wVS = i * p.count
			let	wSS = i * numC
			v.u[ wVS ..< wVS + p.count ] = u[ wSS + p.lowerBound ..< wSS + p.upperBound ]
		}
		return v
	}
}

func
RandomMatrix( _ numR: Int, _ numC: Int, _ range: Range< Float > = 0 ..< 1 ) -> Matrix {
	var	v = [ Float ]( repeating: 0, count: numR * numC )
	for i in 0 ..< v.count { v[ i ] = Float.random( in: range ) }
	return Matrix( numR, numC, v )
}

func
Reshape( _ p: [ Float ], _ numR: Int, _ numC: Int ) -> Matrix {
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
	let	v = Matrix( p[ 0 ].numR, wNumC )
	for i in 0 ..< v.numR {
		var	wVS = i * v.numC
		for j in 0 ..< p.count {
			let	wSpan = p[ j ].numC
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
	let	v = Matrix( wNumR, p[ 0 ].numC )
	var	wVS = 0
	for i in 0 ..< p.count {
		let	wSize = p[ i ].numR * p[ i ].numC
		v.u[ wVS ..< wVS + wSize ] = ArraySlice( p[ i ].u )
		wVS += wSize
	}
	return v
}

prefix func
~ ( p: Matrix ) -> Matrix {
	let	v = Matrix( p.numC, p.numR )
	vDSP_mtrans( p.u, 1, &v.u, 1, vDSP_Length( v.numC ), vDSP_Length( v.numR ) )
	return v
}
/*
prefix func
! ( p: Matrix ) -> Matrix {
	var v = p	//	Confirmed that u's entity is copied.
	var pivot : __CLPK_integer = 0
	var workspace = 0.0
	var error : __CLPK_integer = 0

	var N = __CLPK_integer( sqrt( Float( v.u.count ) ) )
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

func +( p: Matrix, s: Float ) -> Matrix { return Matrix( p.numR, p.numC, p.u + s ) }
func +( s: Float, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s + p.u ) }

func -( p: Matrix, s: Float ) -> Matrix { return Matrix( p.numR, p.numC, p.u - s ) }
func -( s: Float, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s - p.u ) }

func *( p: Matrix, s: Float ) -> Matrix { return Matrix( p.numR, p.numC, p.u * s ) }
func *( s: Float, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s * p.u ) }

func /( p: Matrix, s: Float ) -> Matrix { return Matrix( p.numR, p.numC, p.u / s ) }
func /( s: Float, p: Matrix ) -> Matrix { return Matrix( p.numR, p.numC, s / p.u ) }

func ^ ( p: Matrix, s: Float ) -> Matrix {
	let	t = Matrix( p.numR, p.numC )
	var	w = s
	vDSP_vfill( &w, &t.u, 1, vDSP_Length( t.u.count ) )
	let	v = Matrix( p.numR, p.numC )
	let	wM = p
	var	wCount = Int32( v.u.count )
	vvpowsf( &v.u, &t.u, &wM.u, &wCount )
	return v
}

func
+( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u + r.u )
}

func
-( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u - r.u )
}

func
*( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u * r.u )
}

func
/( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u / r.u )
}

func
OutOfPlace( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.numC == r.numR else { fatalError() }
	let	v = Matrix( l.numR, r.numC )
	vDSP_mmul( l.u, 1, r.u, 1, &v.u, 1, vDSP_Length( l.numR ), vDSP_Length( r.numC ), vDSP_Length( l.numC ) )
	return v
}

func
I( _ p: Int ) -> Matrix {
	let	v = Matrix( p, p )
	for i in 0 ..< v.u.count { v.u[ i ] = 1 }
	return v
}

func
JPMatrixTest() {

	let	w22 = [ Float( 2 ), -2.0 ];

	guard w22 + 2 == [ 4,  0 ] else { fatalError() }
	guard w22 - 2 == [ 0, -4 ] else { fatalError() }
	guard w22 * 2 == [ 4, -4 ] else { fatalError() }
	guard w22 / 2 == [ 1, -1 ] else { fatalError() }

	guard 2 + w22 == [ 4,  0 ] else { fatalError() }
	guard 2 - w22 == [ 0,  4 ] else { fatalError() }
	guard 2 *  w22 == [ 4, -4 ] else { fatalError() }
	guard 2 /  w22 == [ 0.99999994, -0.99999994 ] else { fatalError() }

	guard w22 + w22 == [ 4, -4 ] else { fatalError() }
	guard w22 - w22 == [ 0,  0 ] else { fatalError() }
	guard w22 *  w22 == [ 4,  4 ] else { fatalError() }
	guard w22 /  w22 == [ 0.99999994, 0.99999994 ] else { fatalError() }
	
	guard DotProduct( [ -4, -9 ], [ -1, 2 ] ) == -14 else { fatalError() }
	
	let	wMat = Mat( [ [ 1, 2, 3 ], [ 4, 5 ], [ 6 ] ] )
	guard wMat +    2 == Mat( [ [  3.0,  4  ,  5   ], [  6  ,  7   ], [  8   ] ] ) else { fatalError() }
	guard wMat -    2 == Mat( [ [ -1  ,  0  ,  1   ], [  2  ,  3   ], [  4   ] ] ) else { fatalError() }
	guard wMat *    2 == Mat( [ [  2  ,  4  ,  6   ], [  8  , 10   ], [ 12   ] ] ) else { fatalError() }
	guard wMat /    2 == Mat( [ [  0.5,  1  ,  1.5 ], [  2  ,  2.5 ], [  3   ] ] ) else { fatalError() }
	guard    2 + wMat == Mat( [ [  3  ,  4  ,  5   ], [  6  ,  7   ], [  8   ] ] ) else { fatalError() }
	guard    2 - wMat == Mat( [ [  1  ,  0  , -1   ], [ -2  , -3   ], [ -4   ] ] ) else { fatalError() }
	guard    2 * wMat == Mat( [ [  2  ,  4  ,  6   ], [  8  , 10   ], [ 12   ] ] ) else { fatalError() }
//	guard    2 / wMat == Mat( [ [  2  ,  1  ,  Float( 2.0/3.0 ) ], [  0.5, 0.4, ], [ Float( 2.0/6.0 ) ] ] ) else { fatalError() }
	guard wMat + wMat == Mat( [ [  2  ,  4  ,  6   ], [  8  , 10   ], [ 12   ] ] ) else { fatalError() }
	guard wMat - wMat == Mat( [ [  0  ,  0  ,  0   ], [  0  ,  0   ], [  0   ] ] ) else { fatalError() }
	guard wMat * wMat == Mat( [ [  1  ,  4  ,  9   ], [ 16  ,  25  ], [ 36   ] ] ) else { fatalError() }
//	guard wMat / wMat == Mat( [ [  1  ,  1  ,  1   ], [  1  ,  1   ], [  1   ] ] ) else { fatalError() }

	let	wM = Matrix( 2, 3, [ 1, 2, 3, 4, 5, 6 ] )
//	let	wM2 = Matrix( 3, [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] )
//	print( wM2.u )
//	wM2.Dump()
//	guard wM == wM2 else { fatalError() }
	guard wM +  2 == Matrix( 2, 3, [  3  ,  4  ,  5  ,  6  ,  7  ,  8 ] ) else { fatalError() }
	guard wM -  2 == Matrix( 2, 3, [ -1  ,  0  ,  1  ,  2  ,  3  ,  4 ] ) else { fatalError() }
	guard wM *  2 == Matrix( 2, 3, [  2  ,  4  ,  6  ,  8  , 10  , 12 ] ) else { fatalError() }
	guard wM /  2 == Matrix( 2, 3, [  0.5,  1  ,  1.5,  2  ,  2.5,  3 ] ) else { fatalError() }
	guard  2 + wM == Matrix( 2, 3, [  3  ,  4  ,  5  ,  6  ,  7  ,  8 ] ) else { fatalError() }
	guard  2 - wM == Matrix( 2, 3, [  1  ,  0  , -1  , -2  , -3  , -4 ] ) else { fatalError() }
	guard  2 * wM == Matrix( 2, 3, [  2  ,  4  ,  6  ,  8  , 10  , 12 ] ) else { fatalError() }
//	guard  2 / wM == Matrix( 2, 3, [  2  ,  1  ,  Float( 2.0/3.0 ),  0.5, 0.4, Float( 2.0/6.0 ) ] ) else { fatalError() }
	guard wM + wM == Matrix( 2, 3, [  2  ,  4  ,  6  ,  8  , 10  , 12 ] ) else { fatalError() }
	guard wM - wM == Matrix( 2, 3, [  0  ,  0  ,  0  ,  0  ,  0  ,  0 ] ) else { fatalError() }
	guard wM * wM == Matrix( 2, 3, [  1  ,  4  ,  9  , 16  , 25  , 36 ] ) else { fatalError() }
//	guard wM / wM == Matrix( 2, 3, [  1  ,  1  ,  1  ,  1  ,  1  ,  1 ] ) else { fatalError() }
	
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

func
Abs( _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	vDSP_vabs( p, 1, &v, 1, vDSP_Length( p.count ) )
	return v
}

func
Sum( _ p: [ Float ] ) -> Float {
	var	v: Float = 0.0
	vDSP_sve( p, 1, &v, vDSP_Length( p.count ) )
	return v
}

func
L1Norm( _ p: [ Float ] ) -> Float {
	var	w = [ Float ]( repeating: 0, count: p.count )
	vDSP_vabs( p, 1, &w, 1, vDSP_Length( p.count ) )
	var	v: Float = 0.0
	vDSP_sve( w, 1, &v, vDSP_Length( p.count ) )
	return v
}

func
L2NormSquare( _ p: [ Float ] ) -> Float {
	var	v: Float = 0.0
	vDSP_svesq( p, 1, &v, vDSP_Length( p.count ) )
	return v
}

func
L2Norm( _ p: [ Float ] ) -> Float {
	return sqrt( L2NormSquare( p ) )
}

func
Normalize( _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	wM = Float( 0 )
	var	wSD = Float( 0 )
	vDSP_normalize( p, 1, &v, 1, &wM, &wSD, vDSP_Length( p.count ) )
	return v
}

func
Abs( _ p: Matrix ) -> Matrix {
	return Matrix( p.numR, p.numC, Abs( p.u ) )
}

func
Sum( _ p: Matrix ) -> Float {
	return Sum( p.u )
}

func
L1Norm( _ p: Matrix ) -> Float {
	return L1Norm( p.u )
}

func
L2NormSquare( _ p: Matrix ) -> Float {
	return L2NormSquare( p.u )
}

func
L2Norm( _ p: Matrix ) -> Float {
	return L2Norm( p.u )
}

func
Normalize( _ p: Matrix ) -> Matrix {
	var	v = [ Float ]( repeating: 0, count: p.u.count )
	var	wM = Float( 0 )
	var	wSD = Float( 0 )
	for i in 0 ..< p.numR {
		vDSP_normalize( &p.u[ i * p.numC ], 1, &v + i * p.numC, 1, &wM, &wSD, vDSP_Length( p.numC ) )
	}
	return Matrix( p.numR, p.numC, v )
}

