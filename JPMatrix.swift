//	Written by Satoru Ogura, Tokyo.
//
import Accelerate

//	NEED JPMath

struct
Matrix< N: Numeric > {
	var	nR	:	Int
	var	nC	:	Int
	var	m	:	ArraySlice< N >

	init( _ m: ArraySlice< N >, _ nR: Int, _ nC: Int ) {
		guard nR * nC == m.count else { fatalError( "Size unmatch" ) }
		self.m = m
		self.nR = nR
		self.nC = nC
	}

	init( _ nR: Int, _ nC: Int, _ initial: N = 0 ) {
		self.nR = nR
		self.nC = nC
		self.m = ArraySlice( [ N ]( repeating: initial, count: nR * nC ) )
	}

	init( _ p: [ [ N ] ] = [ [ N ] ]() ) {
		guard p.count > 0 else { fatalError( "Need to have at least one element." ) }
		self.init( p.count, p[ 0 ].count )
		for i in 0 ..< nR {
			guard p[ i ].count == nC else { fatalError( "All the rows must have same size." ) }
			let	w = i * nC
			m[ w ..< w + nC ] = ArraySlice( p[ i ] )
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
	subscript( _ r: Int, _ c: Int ) -> N {
		get {
			guard r < nR && c < nC else { fatalError() }
			return m[ r * nC + c ]
		}
		set {
			guard r < nR && c < nC else { fatalError() }
			m[ r * nC + c ] = newValue
		}
	}

	func
	Row( _ r: Int ) -> Vector< N > {
		return Vector( m[ r * nC ..< ( r + 1 ) * nC ] )
	}
	mutating func
	SetRow( _ r: Int, _ p: Vector< N > ) {
		guard nC == p.n else { fatalError() }
		m[ r * nC ..< ( r + 1 ) * nC ] = p.ToArraySlice()
	}
	func
	Rows( _ p: Range<Int> ) -> Matrix {
		return Matrix( m[ p.lowerBound * nC ..< p.upperBound * nC ], p.count, nC )
	}
	func
	Rows( _ p: ClosedRange<Int> ) -> Matrix {
		return Rows( p.lowerBound ..< p.upperBound + 1 )
	}
	
	func
	Col( _ c: Int ) -> Vector< N > {
		return Vector( ArraySlice( m ), c, nR )
	}
	mutating func
	SetCol( _ c: Int, _ p: Vector< N > ) {
		guard nR == p.n else { fatalError() }
		for r in 0 ..< nR { m[ r * nC + c ] = p[ c ] }
	}
	func
	Cols( _ p: Range<Int> ) -> Matrix {
		var	v = [ N ]( repeating: 0, count: nR * p.count )
		for i in 0 ..< nR {
			let	wVS = i * p.count
			let	wSS = i * nC
			v[ wVS ..< wVS + p.count ] = m[ wSS + p.lowerBound ..< wSS + p.upperBound ]
		}
		return Matrix( ArraySlice( v ), nR, p.count )
	}
	func
	Cols( _ p: ClosedRange<Int> ) -> Matrix {
		return Cols( p.lowerBound ..< p.upperBound + 1 )
	}
}

func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range		<   Int> ) -> Matrix<   Int> { return Matrix( RandomArray( nR * nC, range ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range		< Float> ) -> Matrix< Float> { return Matrix( RandomArray( nR * nC, range ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range		<Double> ) -> Matrix<Double> { return Matrix( RandomArray( nR * nC, range ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange<   Int> ) -> Matrix<   Int> { return Matrix( RandomArray( nR * nC, range ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange< Float> ) -> Matrix< Float> { return Matrix( RandomArray( nR * nC, range ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange<Double> ) -> Matrix<Double> { return Matrix( RandomArray( nR * nC, range ), nR, nC ) }

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
			v[ wVS ..< wVS + wSpan ] = p[ j ].m[ wPS ..< wPS + wSpan ]
			wVS += wSpan
		}
	}
	return Matrix( ArraySlice( v ), wNumR, wNumC )
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
		v[ wVS ..< wVS + wSize ] = ArraySlice( p[ i ].m )
		wVS += wSize
	}
	return Matrix( ArraySlice( v ), wNumR, wNumC )
}

prefix func
~ ( p: Matrix<Float> ) -> Matrix<Float> {
	var	v = [ Float  ]( repeating: 0, count: p.nR * p.nC )
	vDSP_mtrans( p.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, 1, vDSP_Length( p.nC ), vDSP_Length( p.nR ) )
	return Matrix( ArraySlice( v ), p.nC, p.nR )
}
prefix func
~ ( p: Matrix<Double> ) -> Matrix<Double> {
	var	v = [ Double ]( repeating: 0, count: p.nR * p.nC )
	vDSP_mtransD( p.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, 1, vDSP_Length( p.nC ), vDSP_Length( p.nR ) )
	return Matrix( ArraySlice( v ), p.nC, p.nR )
}

func
==< T > ( l: Matrix< T >, r: Matrix< T > ) -> Bool {
	if l.nR != r.nR { return false }
	if l.nC != r.nC { return false }
	return l.m == r.m
}

func +( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( ArraySlice( p.m ) + s, p.nR, p.nC ) }
func +( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( ArraySlice( p.m ) + s, p.nR, p.nC ) }
func +( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( s + ArraySlice( p.m ), p.nR, p.nC ) }
func +( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( s + ArraySlice( p.m ), p.nR, p.nC ) }

func -( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( ArraySlice( p.m ) - s, p.nR, p.nC ) }
func -( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( ArraySlice( p.m ) - s, p.nR, p.nC ) }
func -( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( s - ArraySlice( p.m ), p.nR, p.nC ) }
func -( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( s - ArraySlice( p.m ), p.nR, p.nC ) }

func *( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( ArraySlice( p.m ) * s, p.nR, p.nC ) }
func *( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( ArraySlice( p.m ) * s, p.nR, p.nC ) }
func *( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( s * ArraySlice( p.m ), p.nR, p.nC ) }
func *( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( s * ArraySlice( p.m ), p.nR, p.nC ) }

func /( p: Matrix< Float>, s:  Float ) -> Matrix< Float> { return Matrix( ArraySlice( p.m ) / s, p.nR, p.nC ) }
func /( p: Matrix<Double>, s: Double ) -> Matrix<Double> { return Matrix( ArraySlice( p.m ) / s, p.nR, p.nC ) }
func /( s:  Float, p: Matrix< Float> ) -> Matrix< Float> { return Matrix( s / ArraySlice( p.m ), p.nR, p.nC ) }
func /( s: Double, p: Matrix<Double> ) -> Matrix<Double> { return Matrix( s / ArraySlice( p.m ), p.nR, p.nC ) }

func
+( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) + ArraySlice( r.m ), l.nR, l.nC )
}
func
+( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) + ArraySlice( r.m ), l.nR, l.nC )
}

func
-( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) - ArraySlice( r.m ), l.nR, l.nC )
}
func
-( _ l: Matrix< Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) - ArraySlice( r.m ), l.nR, l.nC )
}

func
*( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) * ArraySlice( r.m ), l.nR, l.nC )
}
func
*( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) * ArraySlice( r.m ), l.nR, l.nC )
}

func
/( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) / ArraySlice( r.m ), l.nR, l.nC )
}
func
/( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nR == r.nR && l.nC == r.nC else { fatalError() }
	return Matrix( ArraySlice( l.m ) / ArraySlice( r.m ), l.nR, l.nC )
}

func
Dot( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nC == r.nR else { fatalError() }
	var	v = [  Float ]( repeating: 0, count: l.nR * r.nC )
	vDSP_mmul( l.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, 1, vDSP_Length( l.nR ), vDSP_Length( r.nC ), vDSP_Length( l.nC ) )
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}

func
Dot( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nC == r.nR else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.nR * r.nC )
	vDSP_mmulD( l.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, 1, vDSP_Length( l.nR ), vDSP_Length( r.nC ), vDSP_Length( l.nC ) )
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}

func
VAdd( _ l: Matrix<Float>, _ r: Vector< Float > ) -> Matrix<Float> {
	guard l.nR == r.n else { fatalError() }
	var v = Array( l.m )
	for i in 0 ..< l.nC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vadd ( wV, l.nC, r.m.withUnsafeBufferPointer { $0.baseAddress! } + r.o, r.s, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}
func
VAdd( _ l: Matrix<Double>, _ r: Vector< Double > ) -> Matrix<Double> {
	guard l.nR == r.n else { fatalError() }
	var v = Array( l.m )
	for i in 0 ..< l.nC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vaddD( wV, l.nC, r.m.withUnsafeBufferPointer { $0.baseAddress! } + r.o, r.s, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}

func
HAdd( _ l: Matrix<Float>, _ r: Vector< Float > ) -> Matrix<Float> {
	guard l.nC == r.n else { fatalError() }
	var v = Array( l.m )
	for i in 0 ..< r.n {
		var	w = r[ i ]
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vsadd ( wV, l.nC, &w, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}

func
HAdd( _ l: Matrix<Double>, _ r: Vector< Double > ) -> Matrix<Double> {
	guard l.nC == r.n else { fatalError() }
	var v = Array( l.m )
	for i in 0 ..< r.n {
		var	w = r[ i ]
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vsaddD( wV, l.nC, &w, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}

func
VDiv ( _ l: Matrix<Float>, _ r: Vector< Float > ) -> Matrix<Float> {
	guard l.nR == r.n else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.m.count )
	for i in 0 ..< r.n {
		var	w = r[ i ]
		let	wI = i * l.nC
		vDSP_vsdiv( l.m.withUnsafeBufferPointer { $0.baseAddress! } + wI, 1, &w, &v[ wI ], 1, vDSP_Length( l.nC ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}

func
VDiv ( _ l: Matrix<Double>, _ r: Vector< Double > ) -> Matrix<Double> {
	guard l.nR == r.n else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.m.count )
	for i in 0 ..< r.n {
		var	w = r[ i ]
		let	wI = i * l.nC
		vDSP_vsdivD( l.m.withUnsafeBufferPointer { $0.baseAddress! } + wI, 1, &w, &v[ wI ], 1, vDSP_Length( l.nC ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}

func
HDiv ( _ l: Matrix<Float>, _ r: Vector< Float >) -> Matrix<Float> {
	guard l.nC == r.n else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.m.count )
	for i in stride( from: 0, to: v.count, by: r.n ) {
		vDSP_vdiv ( r.m.withUnsafeBufferPointer { $0.baseAddress! } + r.o, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! } + i, 1, &v[ i ], 1, vDSP_Length( r.n ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}
func
HDiv ( _ l: Matrix<Double>, _ r: Vector< Double > ) -> Matrix<Double> {
	guard l.nC == r.n else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.m.count )
	for i in stride( from: 0, to: v.count, by: r.n ) {
		vDSP_vdivD( r.m.withUnsafeBufferPointer { $0.baseAddress! } + r.o, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! } + i, 1, &v[ i ], 1, vDSP_Length( r.n ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}


func	Abs		( _ p: Matrix< Float> ) -> Matrix< Float> { return Matrix( Abs( ArraySlice( p.m ) ), p.nR, p.nC ) }
func	Abs		( _ p: Matrix<Double> ) -> Matrix<Double> { return Matrix( Abs( ArraySlice( p.m ) ), p.nR, p.nC ) }

func	Sum		( _ p: Matrix< Float> ) ->  Float { return Sum( ArraySlice( p.m ) ) }
func	Sum		( _ p: Matrix<Double> ) -> Double { return Sum( ArraySlice( p.m ) ) }

func	L2NormQ	( _ p: Matrix< Float> ) ->  Float { return L2NormQ( ArraySlice( p.m ) ) }
func	L2NormQ	( _ p: Matrix<Double> ) -> Double { return L2NormQ( ArraySlice( p.m ) ) }

func	L2Norm	( _ p: Matrix< Float> ) ->  Float { return L2Norm( ArraySlice( p.m ) ) }
func	L2Norm	( _ p: Matrix<Double> ) -> Double { return L2Norm( ArraySlice( p.m ) ) }

func
TestMatrix() {
//	print( Matrix<Float>( nR: 1, nC: 1, u: [ 3 ] ).u[ 0 ] )

	print( Matrix<Float>( [ 1, 2, 3, 4, 5, 6 ], 2, 3 ).m[ 4 ] )
	print( Matrix<Float>( 2, 3, 5 ).m[ 4 ] )
	print( Matrix<Float>( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).m[ 4 ] )

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
	
	guard Dot( [ Float( -4 ), -9 ], [ -1, 2 ] ) == -14 else { fatalError() }

	let	wM = Matrix<Float>( [ 1, 2, 3, 4, 5, 6 ], 2, 3 )
//	let	wM2 = Matrix( 3, [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] )
//	print( wM2.u )
//	wM2.Dump()
//	guard wM == wM2 else { fatalError() }
	guard wM +  2 == Matrix( [  3  ,  4  ,  5  ,  6  ,  7  ,  8 ], 2, 3 ) else { fatalError() }
	guard wM -  2 == Matrix( [ -1  ,  0  ,  1  ,  2  ,  3  ,  4 ], 2, 3 ) else { fatalError() }
	guard wM *  2 == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
	guard wM /  2 == Matrix( [  0.5,  1  ,  1.5,  2  ,  2.5,  3 ], 2, 3 ) else { fatalError() }
	guard  2 + wM == Matrix( [  3  ,  4  ,  5  ,  6  ,  7  ,  8 ], 2, 3 ) else { fatalError() }
	guard  2 - wM == Matrix( [  1  ,  0  , -1  , -2  , -3  , -4 ], 2, 3 ) else { fatalError() }
	guard  2 * wM == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
//	guard  2 / wM == Matrix( 2, 3, [  2  ,  1  ,  Float( 2.0/3.0 ),  0.5, 0.4, Float( 2.0/6.0 ) ] ) else { fatalError() }
	guard wM + wM == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
	guard wM - wM == Matrix( [  0  ,  0  ,  0  ,  0  ,  0  ,  0 ], 2, 3 ) else { fatalError() }
	guard wM * wM == Matrix( [  1  ,  4  ,  9  , 16  , 25  , 36 ], 2, 3 ) else { fatalError() }
//	guard wM / wM == Matrix( 2, 3, [  1  ,  1  ,  1  ,  1  ,  1  ,  1 ] ) else { fatalError() }
	
	guard Dot(
		wM
	,	Matrix( [ 1, 2, 3, 4, 5, 6 ], 3, 2 )
	) == Matrix( [ 22, 28, 49, 64 ], 2, 2 ) else {
		fatalError()
	}
	guard VDiv( wM, Vector( [ 1, 2 ] ) ) == Matrix( [ 1, 2, 3, 2, 2.5, 3 ], 2, 3 ) else { fatalError() }
	print( HDiv( wM, Vector( [ 1, 2, 3 ] ) ) )
	guard HDiv( wM, Vector( [ 1, 2, 3 ] ) ) == Matrix( [ 0.99999994, 0.99999994, 0.99999994, 3.9999998, 2.4999998, 1.9999999 ], 2, 3 ) else { fatalError() }

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
	guard 2 * w22 == [ 4, -4 ] else { fatalError() }
	guard 2 / w22 == [ 1.0, -1.0 ] else { fatalError() }

	guard w22 + w22 == [ 4, -4 ] else { fatalError() }
	guard w22 - w22 == [ 0,  0 ] else { fatalError() }
	guard w22 * w22 == [ 4,  4 ] else { fatalError() }
	guard w22 / w22 == [ 1.0, 1.0 ] else { fatalError() }
	
	guard Dot( [ Double( -4 ), -9 ], [ -1, 2 ] ) == -14 else { fatalError() }

	let	wM = Matrix<Double>( [ 1, 2, 3, 4, 5, 6 ], 2, 3 )
//	let	wM2 = Matrix( 3, [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] )
//	print( wM2.u )
//	wM2.Dump()
//	guard wM == wM2 else { fatalError() }
	guard wM +  2 == Matrix( [  3  ,  4  ,  5  ,  6  ,  7  ,  8 ], 2, 3 ) else { fatalError() }
	guard wM -  2 == Matrix( [ -1  ,  0  ,  1  ,  2  ,  3  ,  4 ], 2, 3 ) else { fatalError() }
	guard wM *  2 == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
	guard wM /  2 == Matrix( [  0.5,  1  ,  1.5,  2  ,  2.5,  3 ], 2, 3 ) else { fatalError() }
	guard  2 + wM == Matrix( [  3  ,  4  ,  5  ,  6  ,  7  ,  8 ], 2, 3 ) else { fatalError() }
	guard  2 - wM == Matrix( [  1  ,  0  , -1  , -2  , -3  , -4 ], 2, 3 ) else { fatalError() }
	guard  2 * wM == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
//	guard  2 / wM == Matrix( 2, 3, [  2  ,  1  ,  Float( 2.0/3.0 ),  0.5, 0.4, Float( 2.0/6.0 ) ] ) else { fatalError() }
	guard wM + wM == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
	guard wM - wM == Matrix( [  0  ,  0  ,  0  ,  0  ,  0  ,  0 ], 2, 3 ) else { fatalError() }
	guard wM * wM == Matrix( [  1  ,  4  ,  9  , 16  , 25  , 36 ], 2, 3 ) else { fatalError() }
//	guard wM / wM == Matrix( 2, 3, [  1  ,  1  ,  1  ,  1  ,  1  ,  1 ] ) else { fatalError() }
	
	guard Dot(
		wM
	,	Matrix( [ 1, 2, 3, 4, 5, 6 ], 3, 2 )
	) == Matrix( [ 22, 28, 49, 64 ], 2, 2 ) else {
		fatalError()
	}
	
}
