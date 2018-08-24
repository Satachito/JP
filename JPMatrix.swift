import Accelerate

struct
Matrix< T: Numeric > {
	var	numR	:	Int
	var	numC	:	Int
	var	u		:	[ T ]

	init( _ numR: Int, _ numC: Int, _ u: [ T ] ) {
		guard numR * numC == u.count else { fatalError( "Size unmatch" ) }
		self.numR = numR
		self.numC = numC
		self.u = u
	}

	init( _ numR: Int, _ numC: Int, _ initial: T = 0 ) {
		self.numR = numR
		self.numC = numC
		self.u = [ T ]( repeating: initial, count: numR * numC )
	}

	init( _ p: [ [ T ] ] ) {
		guard p.count > 0 else { fatalError( "Need to have more than one element." ) }
		self.init( p.count, p[ 0 ].count )
		for i in 0 ..< numR {
			guard p[ i ].count == numC else { fatalError( "All the rows must have same size." ) }
			let	w = i * numC
			u[ w ..< w + numC ] = ArraySlice( p[ i ] )
		}
	}
	/*
	subscript( _ r: Int ) -> ArraySlice< T > {
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
	*/
	subscript( _ r: Int, _ c: Int ) -> T {
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
	Row( _ r: Int ) -> [ T ] {
		return Array( u[ r * numC ..< ( r + 1 ) * numC ] )
	}
	mutating func
	SetRow( _ r: Int, _ p: ArraySlice<T> ) {
		guard numC == p.count else { fatalError() }
		u[ r * numC ..< ( r + 1 ) * numC ] = p
	}
	func
	Rows( _ p: Range<Int> ) -> Matrix {
		return Matrix( p.count, numC, Array( u[ p.lowerBound * numC ..< p.upperBound * numC ] ) )
	}
	func
	Rows( _ p: ClosedRange<Int> ) -> Matrix {
		return Rows( p.lowerBound ..< p.upperBound + 1 )
	}
	
	func
	Col( _ c: Int ) -> [ T ] {
		var	v = [ T ]( repeating: 0, count: numR )
		for r in 0 ..< numR { v[ r ] = u[ r * numC + c ] }
		return v
	}
	mutating func
	SetCol( _ c: Int, _ p: ArraySlice<T> ) {
		guard numR == p.count else { fatalError() }
		for r in 0 ..< numR { u[ r * numC + c ] = p[ c ] }
	}
	func
	Cols( _ p: Range<Int> ) -> Matrix {
		var	v = [ T ]( repeating: 0, count: numR * p.count )
		for i in 0 ..< numR {
			let	wVS = i * p.count
			let	wSS = i * numC
			v[ wVS ..< wVS + p.count ] = u[ wSS + p.lowerBound ..< wSS + p.upperBound ]
		}
		return Matrix( numR, p.count, v )
	}
	func
	Cols( _ p: ClosedRange<Int> ) -> Matrix {
		return Cols( p.lowerBound ..< p.upperBound + 1 )
	}
}

func
<<<<<<< HEAD
RandomMatrix( _ numR: Int, _ numC: Int, _ range: Range<Int> ) -> Matrix<Int> {
	var	v = [ Int ]( repeating: 0, count: numR * numC )
	for i in 0 ..< v.count { v[ i ] = Int.random( in: range ) }
	return Matrix( numR, numC, v )
}
func
RandomMatrix( _ numR: Int, _ numC: Int, _ range: ClosedRange<Int> ) -> Matrix<Int> {
	var	v = [ Int ]( repeating: 0, count: numR * numC )
	for i in 0 ..< v.count { v[ i ] = Int.random( in: range ) }
	return Matrix( numR, numC, v )
}
func
RandomMatrix( _ numR: Int, _ numC: Int, _ range: Range<Float> ) -> Matrix<Float> {
	var	v = [ Float ]( repeating: 0, count: numR * numC )
	for i in 0 ..< v.count { v[ i ] = Float.random( in: range ) }
	return Matrix( numR, numC, v )
}
func
RandomMatrix( _ numR: Int, _ numC: Int, _ range: ClosedRange<Float> ) -> Matrix<Float> {
	var	v = [ Float ]( repeating: 0, count: numR * numC )
	for i in 0 ..< v.count { v[ i ] = Float.random( in: range ) }
	return Matrix( numR, numC, v )
}
func
RandomMatrix( _ numR: Int, _ numC: Int, _ range: Range<Double> ) -> Matrix<Double> {
	var	v = [ Double ]( repeating: 0, count: numR * numC )
	for i in 0 ..< v.count { v[ i ] = Double.random( in: range ) }
	return Matrix( numR, numC, v )
}
func
RandomMatrix( _ numR: Int, _ numC: Int, _ range: ClosedRange<Double> ) -> Matrix<Double> {
	var	v = [ Double ]( repeating: 0, count: numR * numC )
	for i in 0 ..< v.count { v[ i ] = Double.random( in: range ) }
	return Matrix( numR, numC, v )
}

func
HStack<T>( _ p: [ Matrix<T> ] ) -> Matrix<T> {
	guard p.count > 0 else { fatalError() }
	let	wNumR = p[ 0 ].numR
	let	wNumC = p.reduce( 0 ) { v, p in v + p.numC }
	var	v = [ T ]( repeating: 0, count: wNumR * wNumC )
	for i in 0 ..< wNumR {
		var	wVS = i * wNumC
		for j in 0 ..< p.count {
			let	wSpan = p[ j ].numC
			let	wPS = i * wSpan
			v[ wVS ..< wVS + wSpan ] = p[ j ].u[ wPS ..< wPS + wSpan ]
			wVS += wSpan
		}
	}
	return Matrix( wNumR, wNumC, v )
}

func
VStack<T>( _ p: [ Matrix<T> ] ) -> Matrix<T> {
	guard p.count > 0 else { fatalError() }
	let	wNumC = p[ 0 ].numC
	let	wNumR = p.reduce( 0 ) { v, p in v + p.numR }
	var	v = [ T ]( repeating: 0, count: wNumR * wNumC )
	var	wVS = 0
	for i in 0 ..< p.count {
		let	wSize = p[ i ].numR * p[ i ].numC
		v[ wVS ..< wVS + wSize ] = ArraySlice( p[ i ].u )
		wVS += wSize
	}
	return Matrix( wNumR, wNumC, v )
}

prefix func
~ ( p: Matrix<Float> ) -> Matrix<Float> {
	var	v = [  Float ]( repeating: 0, count: p.numR * p.numC )
	vDSP_mtrans( p.u, 1, &v, 1, vDSP_Length( p.numC ), vDSP_Length( p.numR ) )
	return Matrix( p.numC, p.numR, v )
}
prefix func
~ ( p: Matrix<Double> ) -> Matrix<Double> {
	var	v = [ Double ]( repeating: 0, count: p.numR * p.numC )
	vDSP_mtransD( p.u, 1, &v, 1, vDSP_Length( p.numC ), vDSP_Length( p.numR ) )
	return Matrix( p.numC, p.numR, v )
}

func
==<T> ( l: Matrix<T>, r: Matrix<T> ) -> Bool {
	if l.numR != r.numR { return false }
	if l.numC != r.numC { return false }
	return l.u == r.u
}

func +( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.numR, p.numC, p.u + s ) }
func +( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.numR, p.numC, p.u + s ) }
func +( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.numR, p.numC, s + p.u ) }
func +( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.numR, p.numC, s + p.u ) }

func -( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.numR, p.numC, p.u - s ) }
func -( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.numR, p.numC, p.u - s ) }
func -( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.numR, p.numC, s - p.u ) }
func -( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.numR, p.numC, s - p.u ) }

func *( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.numR, p.numC, p.u * s ) }
func *( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.numR, p.numC, p.u * s ) }
func *( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.numR, p.numC, s * p.u ) }
func *( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.numR, p.numC, s * p.u ) }

func /( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( p.numR, p.numC, p.u / s ) }
func /( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( p.numR, p.numC, p.u / s ) }
func /( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( p.numR, p.numC, s / p.u ) }
func /( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( p.numR, p.numC, s / p.u ) }

func
+( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u + r.u )
}
func
+( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u + r.u )
}

func
-( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u - r.u )
}
func
-( _ l: Matrix< Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u - r.u )
}

func
*( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u * r.u )
}
func
*( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u * r.u )
}

func
/( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u / r.u )
}
func
/( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.numR == r.numR && l.numC == r.numC else { fatalError() }
	return Matrix( l.numR, l.numC, l.u / r.u )
}

func
OutOfPlace( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.numC == r.numR else { fatalError() }
	var	v = [  Float ]( repeating: 0, count: l.numR * r.numC )
	vDSP_mmul( l.u, 1, r.u, 1, &v, 1, vDSP_Length( l.numR ), vDSP_Length( r.numC ), vDSP_Length( l.numC ) )
	return Matrix( l.numR, r.numC, v )
}

func
OutOfPlace( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.numC == r.numR else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.numR * r.numC )
	vDSP_mmulD( l.u, 1, r.u, 1, &v, 1, vDSP_Length( l.numR ), vDSP_Length( r.numC ), vDSP_Length( l.numC ) )
	return Matrix( l.numR, r.numC, v )
}

func
VAdd( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.numR == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< l.numC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vadd( wV, l.numC, r, 1, wV, l.numC, vDSP_Length( l.numR ) )
	}
	return Matrix( l.numR, l.numC, v )
}
func
VAdd( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.numR == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< l.numC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vaddD( wV, l.numC, r, 1, wV, l.numC, vDSP_Length( l.numR ) )
	}
	return Matrix( l.numR, l.numC, v )
}

func
HAdd( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.numC == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vsadd( wV, l.numC, &w, wV, l.numC, vDSP_Length( l.numR ) )
	}
	return Matrix( l.numR, l.numC, v )
}
func
HAdd( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.numC == r.count else { fatalError() }
	var v = l.u
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vsaddD( wV, l.numC, &w, wV, l.numC, vDSP_Length( l.numR ) )
	}
	return Matrix( l.numR, l.numC, v )
}

func
VDiv ( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.numR == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.u.count )
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wI = i * l.numC
		vDSP_vsdiv( UnsafePointer( l.u ) + wI, 1, &w, &v[ wI ], 1, vDSP_Length( l.numC ) )
	}
	return Matrix( l.numR, l.numC, v )
}
func
VDiv ( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.numR == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.u.count )
	for i in 0 ..< r.count {
		var	w = r[ i ]
		let	wI = i * l.numC
		vDSP_vsdivD( UnsafePointer( l.u ) + wI, 1, &w, &v[ wI ], 1, vDSP_Length( l.numC ) )
	}
	return Matrix( l.numR, l.numC, v )
}

func
HDiv ( _ l: Matrix<Float>, _ r: [ Float ] ) -> Matrix<Float> {
	guard l.numC == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.u.count )
	for i in stride( from: 0, to: v.count, by: r.count ) {
		vDSP_vdiv( r, 1, UnsafePointer( l.u ) + i, 1, &v[ i ], 1, vDSP_Length( r.count ) )
	}
	return Matrix( l.numR, l.numC, v )
}
func
HDiv ( _ l: Matrix<Double>, _ r: [ Double ] ) -> Matrix<Double> {
	guard l.numC == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.u.count )
	for i in stride( from: 0, to: v.count, by: r.count ) {
		vDSP_vdivD( r, 1, UnsafePointer( l.u ) + i, 1, &v[ i ], 1, vDSP_Length( r.count ) )
	}
	return Matrix( l.numR, l.numC, v )
}


func
Abs( _ p: Matrix<Float> ) -> Matrix<Float> {
	return Matrix( p.numR, p.numC, Abs( p.u ) )
}
func
Abs( _ p: Matrix<Double> ) -> Matrix<Double> {
	return Matrix( p.numR, p.numC, Abs( p.u ) )
}

func
Sum( _ p: Matrix<Float> ) -> Float {
	return Sum( p.u )
}
func
Sum( _ p: Matrix<Double> ) -> Double {
	return Sum( p.u )
}

func
L1Norm( _ p: Matrix<Float> ) -> Float {
	return L1Norm( p.u )
}
func
L1Norm( _ p: Matrix<Double> ) -> Double {
	return L1Norm( p.u )
}

func
L2NormSquare( _ p: Matrix<Float> ) -> Float {
	return L2NormSquare( p.u )
}
func
L2NormSquare( _ p: Matrix<Double> ) -> Double {
	return L2NormSquare( p.u )
}

func
L2Norm( _ p: Matrix<Float> ) -> Float {
	return L2Norm( p.u )
}
func
L2Norm( _ p: Matrix<Double> ) -> Double {
	return L2Norm( p.u )
}

func
TestMatrix() {
//	print( Matrix<Float>( numR: 1, numC: 1, u: [ 3 ] ).u[ 0 ] )

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

	let	w22 = [ Double( 2 ), -2.0 ];

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

