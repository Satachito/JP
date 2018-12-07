//	Written by Satoru Ogura, Tokyo.
//
import Accelerate

struct
Matrix< N: Numeric > {
	var	nR	:	Int
	var	nC	:	Int
	var	m	:	ArraySlice< N >

	init( _ nR: Int, _ nC: Int, _ initial: N = 0 ) {
		self.nR = nR
		self.nC = nC
		self.m = ArraySlice( [ N ]( repeating: initial, count: nR * nC ) )
	}

	init( _ m: ArraySlice< N >, _ nR: Int, _ nC: Int ) {
		self.m = m
		self.nR = nR
		self.nC = nC
	}

	subscript( _ r: Int, _ c: Int ) -> N {
		get {
			guard r < nR && c < nC else { fatalError() }
			return m[ m.startIndex + r * nC + c ]
		}
		set {
			guard r < nR && c < nC else { fatalError() }
			m[ m.startIndex + r * nC + c ] = newValue
		}
	}

	func
	S() -> String {
		var	v = ""
		for iR in 0 ..< nR {
			for iC in 0 ..< nC { v += "\t\( m[ m.startIndex + iR * nC + iC ] )" }
			v += "\n"
		}
		return v
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
		return Vector( m[ c ..< m.count ], nR, nC )
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

func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range		<   Int> ) -> Matrix<   Int> { return Matrix( ArraySlice( RandomArray( nR * nC, range ) ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range		< Float> ) -> Matrix< Float> { return Matrix( ArraySlice( RandomArray( nR * nC, range ) ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: Range		<Double> ) -> Matrix<Double> { return Matrix( ArraySlice( RandomArray( nR * nC, range ) ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange<   Int> ) -> Matrix<   Int> { return Matrix( ArraySlice( RandomArray( nR * nC, range ) ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange< Float> ) -> Matrix< Float> { return Matrix( ArraySlice( RandomArray( nR * nC, range ) ), nR, nC ) }
func	RandomMatrix( _ nR: Int, _ nC: Int, _ range: ClosedRange<Double> ) -> Matrix<Double> { return Matrix( ArraySlice( RandomArray( nR * nC, range ) ), nR, nC ) }

func
HStack<T>( _ p: ArraySlice< Matrix<T> > ) -> Matrix<T> {
	guard p.count > 0 else { fatalError() }
	let	wNumR = p[ p.startIndex ].nR
	let	wNumC = p.reduce( 0 ) { v, p in v + p.nC }
	var	v = [ T ]( repeating: 0, count: wNumR * wNumC )
	for i in 0 ..< wNumR {
		var	wVS = i * wNumC
		for w in p {
			let	wSpan = w.nC
			let	wPS = i * wSpan
			v[ wVS ..< wVS + wSpan ] = w.m[ w.m.startIndex + wPS ..< w.m.startIndex + wPS + wSpan ]
			wVS += wSpan
		}
	}
	return Matrix( ArraySlice( v ), wNumR, wNumC )
}

func
VStack<T>( _ p: ArraySlice< Matrix<T> > ) -> Matrix<T> {
	guard p.count > 0 else { fatalError() }
	let	wNumC = p[ p.startIndex ].nC
	let	wNumR = p.reduce( 0 ) { v, p in v + p.nR }
	var	v = [ T ]( repeating: 0, count: wNumR * wNumC )
	var	wVS = 0
	for w in p {
		let	wSize = w.nR * w.nC
		v[ wVS ..< wVS + wSize ] = w.m
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
HSum( _ p: Matrix<Float> ) -> Vector< Float > {
	var	v = ArraySlice< Float >( repeating: 0, count: p.nC )
	for iR in 0 ..< p.nR {
		let	wOffset = p.nC * iR
		vDSP_vadd( p.m.withUnsafeBufferPointer { $0.baseAddress! } + wOffset, 1, v.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( v.count ) )
	}
	return Vector( v )
}

func
HSum( _ p: Matrix<Double> ) -> Vector< Double > {
	var	v = ArraySlice< Double >( repeating: 0, count: p.nC )
	for iR in 0 ..< p.nR {
		let	wOffset = p.nC * iR
		vDSP_vaddD( p.m.withUnsafeBufferPointer { $0.baseAddress! } + wOffset, 1, v.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( v.count ) )
	}
	return Vector( v )
}

func
Dot( _ l: Matrix< Float>, _ r: Matrix< Float> ) -> Matrix< Float> {
	guard l.nC == r.nR else { fatalError() }
	var	v = [  Float ]( repeating: 0, count: l.nR * r.nC )
	vDSP_mmul( l.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, 1, vDSP_Length( l.nR ), vDSP_Length( r.nC ), vDSP_Length( l.nC ) )
	return Matrix( ArraySlice( v ), l.nR, r.nC )
}

func
Dot( _ l: Matrix<Double>, _ r: Matrix<Double> ) -> Matrix<Double> {
	guard l.nC == r.nR else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.nR * r.nC )
	vDSP_mmulD( l.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.m.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, 1, vDSP_Length( l.nR ), vDSP_Length( r.nC ), vDSP_Length( l.nC ) )
	return Matrix( ArraySlice( v ), l.nR, r.nC )
}

func
VAdd( _ l: Matrix<Float>, _ r: Vector< Float > ) -> Matrix<Float> {
	guard l.nR == r.n else { fatalError() }
	var v = Array( l.m )
	for i in 0 ..< l.nC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vadd ( wV, l.nC, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, wV, l.nC, vDSP_Length( l.nR ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}
func
VAdd( _ l: Matrix<Double>, _ r: Vector< Double > ) -> Matrix<Double> {
	guard l.nR == r.n else { fatalError() }
	var v = Array( l.m )
	for i in 0 ..< l.nC {
		let	wV = UnsafeMutablePointer( &v ) + i
		vDSP_vaddD( wV, l.nC, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, wV, l.nC, vDSP_Length( l.nR ) )
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
		vDSP_vdiv ( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! } + i, 1, &v[ i ], 1, vDSP_Length( r.n ) )
	}
	return Matrix( ArraySlice( v ), l.nR, l.nC )
}
func
HDiv ( _ l: Matrix<Double>, _ r: Vector< Double > ) -> Matrix<Double> {
	guard l.nC == r.n else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.m.count )
	for i in stride( from: 0, to: v.count, by: r.n ) {
		vDSP_vdivD( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! } + i, 1, &v[ i ], 1, vDSP_Length( r.n ) )
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
