//	Written by Satoru Ogura, Tokyo.
//
import Accelerate

//	NEED JPMath

struct
Matrix< T: Numeric > {
	var	nR	:	Int
	var	nC	:	Int
	var	u		:	[ T ]

	init( _ nR: Int, _ nC: Int, _ u: ArraySlice< T > ) {
		guard nR * nC == u.count else { fatalError( "Size unmatch" ) }
		self.nR = nR
		self.nC = nC
		self.u = Array( u )
	}

	init( _ nR: Int, _ nC: Int, _ initial: T = 0 ) {
		self.nR = nR
		self.nC = nC
		self.u = [ T ]( repeating: initial, count: nR * nC )
	}

	init( _ p: [ [ T ] ] = [ [ T ] ]() ) {
		guard p.count > 0 else { fatalError( "Need to have more than one element." ) }
		self.init( p.count, p[ 0 ].count )
		for i in 0 ..< nR {
			guard p[ i ].count == nC else { fatalError( "All the rows must have same size." ) }
			let	w = i * nC
			u[ w ..< w + nC ] = ArraySlice( p[ i ] )
		}
	}
	/*
	subscript( _ r: Int ) -> ArraySlice< T > {
		get {
			guard r < nR else { fatalError() }
			let	w = r * nC
			return u[ w ..< w + nC ]
		}
		set {
			guard r < nR else { fatalError() }
			let	w = r * nC
			u[ w ..< w + nC ] = newValue
		}
	}
	*/
	subscript( _ r: Int, _ c: Int ) -> T {
		get {
			guard r < nR && c < nC else { fatalError() }
			return u[ r * nC + c ]
		}
		set {
			guard r < nR && c < nC else { fatalError() }
			u[ r * nC + c ] = newValue
		}
	}

	func
	Row( _ r: Int ) -> [ T ] {
		return Array( u[ r * nC ..< ( r + 1 ) * nC ] )
	}
	mutating func
	SetRow( _ r: Int, _ p: ArraySlice<T> ) {
		guard nC == p.count else { fatalError() }
		u[ r * nC ..< ( r + 1 ) * nC ] = p
	}
	func
	Rows( _ p: Range<Int> ) -> Matrix {
		return Matrix( p.count, nC, u[ p.lowerBound * nC ..< p.upperBound * nC ] )
	}
	func
	Rows( _ p: ClosedRange<Int> ) -> Matrix {
		return Rows( p.lowerBound ..< p.upperBound + 1 )
	}
	
	func
	Col( _ c: Int ) -> [ T ] {
		var	v = [ T ]( repeating: 0, count: nR )
		for r in 0 ..< nR { v[ r ] = u[ r * nC + c ] }
		return v
	}
	mutating func
	SetCol( _ c: Int, _ p: ArraySlice<T> ) {
		guard nR == p.count else { fatalError() }
		for r in 0 ..< nR { u[ r * nC + c ] = p[ c ] }
	}
	func
	Cols( _ p: Range<Int> ) -> Matrix {
		var	v = [ T ]( repeating: 0, count: nR * p.count )
		for i in 0 ..< nR {
			let	wVS = i * p.count
			let	wSS = i * nC
			v[ wVS ..< wVS + p.count ] = u[ wSS + p.lowerBound ..< wSS + p.upperBound ]
		}
		return Matrix( nR, p.count, ArraySlice( v ) )
	}
	func
	Cols( _ p: ClosedRange<Int> ) -> Matrix {
		return Cols( p.lowerBound ..< p.upperBound + 1 )
	}
}

func
RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range<Int> ) -> Matrix<Int> {
	var	v = [ Int ]( repeating: 0, count: nR * nC )
	for i in 0 ..< v.count { v[ i ] = Int.random( in: range ) }
	return Matrix( nR, nC, ArraySlice( v ) )
}
func
RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange<Int> ) -> Matrix<Int> {
	var	v = [ Int ]( repeating: 0, count: nR * nC )
	for i in 0 ..< v.count { v[ i ] = Int.random( in: range ) }
	return Matrix( nR, nC, ArraySlice( v ) )
}
func
RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range<Float> ) -> Matrix<Float> {
	var	v = [ Float ]( repeating: 0, count: nR * nC )
	for i in 0 ..< v.count { v[ i ] = Float.random( in: range ) }
	return Matrix( nR, nC, ArraySlice( v ) )
}
func
RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange<Float> ) -> Matrix<Float> {
	var	v = [ Float ]( repeating: 0, count: nR * nC )
	for i in 0 ..< v.count { v[ i ] = Float.random( in: range ) }
	return Matrix( nR, nC, ArraySlice( v ) )
}
func
RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range<Double> ) -> Matrix<Double> {
	var	v = [ Double ]( repeating: 0, count: nR * nC )
	for i in 0 ..< v.count { v[ i ] = Double.random( in: range ) }
	return Matrix( nR, nC, ArraySlice( v ) )
}
func
RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange<Double> ) -> Matrix<Double> {
	var	v = [ Double ]( repeating: 0, count: nR * nC )
	for i in 0 ..< v.count { v[ i ] = Double.random( in: range ) }
	return Matrix( nR, nC, ArraySlice( v ) )
}

func
HStack<T>( _ p: [ Matrix<T> ] ) -> Matrix<T> {
	guard p.count > 0 else { fatalError() }
	let	wNumR = p[ 0 ].nR
	let	wNumC = p.reduce( 0 ) { v, p in v + p.nC }
	var	v = [ T ]( repeating: 0, count: wNumR * wNumC )
	for i in 0 ..< wNumR {
		var	wVS = i * wNumC
		for j in 0 ..< p.count {
			let	wSpan = p[ j ].nC
			let	wPS = i * wSpan
			v[ wVS ..< wVS + wSpan ] = p[ j ].u[ wPS ..< wPS + wSpan ]
			wVS += wSpan
		}
	}
	return Matrix( wNumR, wNumC, ArraySlice( v ) )
}

func
VStack<T>( _ p: [ Matrix<T> ] ) -> Matrix<T> {
	guard p.count > 0 else { fatalError() }
	let	wNumC = p[ 0 ].nC
	let	wNumR = p.reduce( 0 ) { v, p in v + p.nR }
	var	v = [ T ]( repeating: 0, count: wNumR * wNumC )
	var	wVS = 0
	for i in 0 ..< p.count {
		let	wSize = p[ i ].nR * p[ i ].nC
		v[ wVS ..< wVS + wSize ] = ArraySlice( p[ i ].u )
		wVS += wSize
	}
	return Matrix( wNumR, wNumC, ArraySlice( v ) )
}

prefix func
~ ( p: Matrix<Float> ) -> Matrix<Float> {
	var	v = [  Float ]( repeating: 0, count: p.nR * p.nC )
	vDSP_mtrans( p.u, 1, &v, 1, vDSP_Length( p.nC ), vDSP_Length( p.nR ) )
	return Matrix( p.nC, p.nR, ArraySlice( v ) )
}
prefix func
~ ( p: Matrix<Double> ) -> Matrix<Double> {
	var	v = [ Double ]( repeating: 0, count: p.nR * p.nC )
	vDSP_mtransD( p.u, 1, &v, 1, vDSP_Length( p.nC ), vDSP_Length( p.nR ) )
	return Matrix( p.nC, p.nR, ArraySlice( v ) )
}

func
==<T> ( l: Matrix<T>, r: Matrix<T> ) -> Bool {
	if l.nR != r.nR { return false }
	if l.nC != r.nC { return false }
	return l.u == r.u
}

func +( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) + s ) }
func +( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) + s ) }
func +( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.nR, p.nC, s + ArraySlice( p.u ) ) }
func +( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.nR, p.nC, s + ArraySlice( p.u ) ) }

func -( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) - s ) }
func -( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) - s ) }
func -( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.nR, p.nC, s - ArraySlice( p.u ) ) }
func -( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.nR, p.nC, s - ArraySlice( p.u ) ) }

func *( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) * s ) }
func *( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) * s ) }
func *( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.nR, p.nC, s * ArraySlice( p.u ) ) }
func *( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.nR, p.nC, s * ArraySlice( p.u ) ) }

func /( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) / s ) }
func /( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.nR, p.nC, ArraySlice( p.u ) / s ) }
func /( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.nR, p.nC, s / ArraySlice( p.u ) ) }
func /( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.nR, p.nC, s / ArraySlice( p.u ) ) }

func
+( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) + ArraySlice( r.u ) )
}
func
+( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) + ArraySlice( r.u ) )
}

func
-( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) - ArraySlice( r.u ) )
}
func
-( _ l: Matrix< Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) - ArraySlice( r.u ) )
}

func
*( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) * ArraySlice( r.u ) )
}
func
*( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) * ArraySlice( r.u ) )
}

func
/( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) / ArraySlice( r.u ) )
}
func
/( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( l.nR, l.nC, ArraySlice( l.u ) / ArraySlice( r.u ) )
}

func
OutOfPlace( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nC == r.nR else { fatalError() }
	var	v = [  Float ]( repeating: 0, count: l.nR * r.nC )
	vDSP_mmul( l.u, 1, r.u, 1, &v, 1, vDSP_Length( l.nR ), vDSP_Length( r.nC ), vDSP_Length( l.nC ) )
	return Matrix( l.nR, r.nC, ArraySlice( v ) )
}

func
OutOfPlace( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nC == r.nR else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.nR * r.nC )
	vDSP_mmulD( l.u, 1, r.u, 1, &v, 1, vDSP_Length( l.nR ), vDSP_Length( r.nC ), vDSP_Length( l.nC ) )
	return Matrix( l.nR, r.nC, ArraySlice( v ) )
}

func
VAdd( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.nR == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< l.nC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vadd( wV, l.nC, r, 1, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}
func
VAdd( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.nR == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< l.nC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vaddD( wV, l.nC, r, 1, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}

func
HAdd( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.nC == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vsadd( wV, l.nC, &w, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}
func
HAdd( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.nC == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vsaddD( wV, l.nC, &w, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}

func
VDiv ( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.nR == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.u.count )
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wI = i * l.nC
		vDSP_vsdiv( UnsafePointer( l.u ) + wI, 1, &w, &v[ wI ], 1, vDSP_Length( l.nC ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}
func
VDiv ( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.nR == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.u.count )
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wI = i * l.nC
		vDSP_vsdivD( UnsafePointer( l.u ) + wI, 1, &w, &v[ wI ], 1, vDSP_Length( l.nC ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}

func
HDiv ( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.nC == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.u.count )
	for i in stride( from: 0, to: v.count, by: r.count ) {
		vDSP_vdiv( r, 1, UnsafePointer( l.u ) + i, 1, &v[ i ], 1, vDSP_Length( r.count ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}
func
HDiv ( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.nC == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.u.count )
	for i in stride( from: 0, to: v.count, by: r.count ) {
		vDSP_vdivD( r, 1, UnsafePointer( l.u ) + i, 1, &v[ i ], 1, vDSP_Length( r.count ) )
	}
	return Matrix( l.nR, l.nC, ArraySlice( v ) )
}


func
Abs( _ p: Matrix<Float> ) -> Matrix<Float> {
	return Matrix( p.nR, p.nC, Abs( ArraySlice( p.u ) ) )
}
func
Abs( _ p: Matrix<Double> ) -> Matrix<Double> {
	return Matrix( p.nR, p.nC, Abs( ArraySlice( p.u ) ) )
}

func
Sum( _ p: Matrix<Float> ) -> Float {
	return Sum( ArraySlice( p.u ) )
}
func
Sum( _ p: Matrix<Double> ) -> Double {
	return Sum( ArraySlice( p.u ) )
}

func
L2NormQ( _ p: Matrix<Float> ) -> Float {
	return L2NormQ( ArraySlice( p.u ) )
}
func
L2NormQ( _ p: Matrix<Double> ) -> Double {
	return L2NormQ( ArraySlice( p.u ) )
}

func
L2Norm( _ p: Matrix<Float> ) -> Float {
	return L2Norm( ArraySlice( p.u ) )
}
func
L2Norm( _ p: Matrix<Double> ) -> Double {
	return L2Norm( ArraySlice( p.u ) )
}

func
TestMatrix() {
//	print( Matrix<Float>( nR: 1, nC: 1, u: [ 3 ] ).u[ 0 ] )

	print( Matrix<Float>( 2, 3, [ 1, 2, 3, 4, 5, 6 ] ).u[ 4 ] )
	print( Matrix<Float>( 2, 3, 5 ).u[ 4 ] )
	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).u[ 4 ] )

	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Row( 1 ) )
	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] )[ 1 , 1] )

	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Rows( 1 ..< 2 ) )
	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Rows( 0 ... 1 ) )
	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Cols( 1 ..< 2 ) )
	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Cols( 1 ... 2 ) )

	print( RandomMatrix( 2, 3, 0 ..< 10 ) )
	print( RandomMatrix( 2, 3, 0 ... 10 ) )
	print( RandomMatrix( 2, 3, 0 ..< Float( 10 ) ) )
	print( RandomMatrix( 2, 3, 0 ... Float( 10 ) ) )
	print( RandomMatrix( 2, 3, 0 ..< 10.0 ) )
	print( RandomMatrix( 2, 3, 0 ... 10.0 ) )
}

func
JPMatrixTestF() {

	let	w22 = ArraySlice( [ Float( 2 ), -2.0 ] );

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
	
	guard DotProduct( [ Float( -4 ), -9 ], [ -1, 2 ] ) == -14 else { fatalError() }

	let	wM = Matrix<Float>( 2, 3, [ 1, 2, 3, 4, 5, 6 ] )
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
	guard VDiv( wM, [ 1, 2 ] ) == Matrix( 2, 3, [ 1, 2, 3, 2, 2.5, 3 ] ) else { fatalError() }
	print( HDiv( wM, [ 1, 2, 3 ] ) )
	guard HDiv( wM, [ 1, 2, 3 ] ) == Matrix( 2, 3, [0.99999994, 0.99999994, 0.99999994, 3.9999998, 2.4999998, 1.9999999] ) else { fatalError() }

}

func
JPMatrixTestD() {

	let	w22 = ArraySlice( [ Double( 2 ), -2.0 ] );

	guard w22 + 2 == [ 4,  0 ] else { fatalError() }
	guard w22 - 2 == [ 0, -4 ] else { fatalError() }
	guard w22 * 2 == [ 4, -4 ] else { fatalError() }
	guard w22 / 2 == [ 1, -1 ] else { fatalError() }

	guard 2 + w22 == [ 4,  0 ] else { fatalError() }
	guard 2 - w22 == [ 0,  4 ] else { fatalError() }
	guard 2 *  w22 == [ 4, -4 ] else { fatalError() }
	guard 2 /  w22 == [ 1.0, -1.0 ] else { fatalError() }

	guard w22 + w22 == [ 4, -4 ] else { fatalError() }
	guard w22 - w22 == [ 0,  0 ] else { fatalError() }
	guard w22 *  w22 == [ 4,  4 ] else { fatalError() }
	guard w22 /  w22 == [ 1.0, 1.0 ] else { fatalError() }
	
	guard DotProduct( [ Double( -4 ), -9 ], [ -1, 2 ] ) == -14 else { fatalError() }

	let	wM = Matrix<Double>( 2, 3, [ 1, 2, 3, 4, 5, 6 ] )
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

