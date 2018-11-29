import Foundation


func
Equal< N: Numeric > ( _ l: ArraySlice< N >, _ r: ArraySlice< N > ) {
	guard l == r else { fatalError() }
}

func
vectorS( _ p1: Float, _ p2: Float, _ p3: Float ) -> ArraySlice< Float > {
	return [ p1, p2, p3 ]
}
func
vectorD( _ p1: Double, _ p2: Double, _ p3: Double ) -> ArraySlice< Double > {
	return [ p1, p2, p3 ]
}

func
v() {
	let	vS: ArraySlice< Float  > = [ 1, 2, 3 ]
	let	vD: ArraySlice< Double > = [ 1, 2, 3 ]

	Equal( vS + vS	, vectorS( 2, 4, 6 ) )
	Equal( vD + vD	, vectorD( 2, 4, 6 ) )
	Equal( vS + 1.0	, vectorS( 2, 3, 4 ) )
	Equal( vD + 1.0	, vectorD( 2, 3, 4 ) )
	Equal( 1.0 + vS	, vectorS( 2, 3, 4 ) )
	Equal( 1.0 + vD	, vectorD( 2, 3, 4 ) )
	
	Equal( vS - vS	, vectorS( 0, 0, 0 ) )
	Equal( vD - vD	, vectorD( 0, 0, 0 ) )
	Equal( vS - 1.0	, vectorS( 0, 1, 2 ) )
	Equal( vD - 1.0	, vectorD( 0, 1, 2 ) )
	Equal( 1.0 - vS	, vectorS( 0, -1, -2 ) )
	Equal( 1.0 - vD	, vectorD( 0, -1, -2 ) )
	
	Equal( vS * vS	, vectorS( 1, 4, 9 ) )
	Equal( vD * vD	, vectorD( 1, 4, 9 ) )
	Equal( vS * 1.0	, vectorS( 1, 2, 3 ) )
	Equal( vD * 1.0	, vectorD( 1, 2, 3 ) )
	Equal( 1.0 * vS	, vectorS( 1, 2, 3 ) )
	Equal( 1.0 * vD	, vectorD( 1, 2, 3 ) )
	
	Equal( vS / vS	, vectorS( 0.99999994, 0.99999994, 0.99999994 ) )
	Equal( vD / vD	, vectorD( 1, 1, 1 ) )
	Equal( vS / 1.0	, vectorS( 1, 2, 3 ) )
	Equal( vD / 1.0	, vectorD( 1, 2, 3 ) )
	Equal( 1.0 / vS	, vectorS( 0.99999994, 0.49999997, 0.333333313 ) )
	Equal( 1.0 / vD	, vectorD( 1, 0.5, 0.33333333333333331 ) )

	Equal( RampArray( 3, Float( 1 ), Float( 2 ) ), vectorS( 1, 3, 5 ) )
	Equal( RampArray( 3, 1.0, 2.0 ), vectorD( 1, 3, 5 ) )

	guard Sum( vS ) == 6 else { fatalError() }
	guard Sum( vD ) == 6 else { fatalError() }

	guard Mean( vS ) == 2 else { fatalError() }
	guard Mean( vD ) == 2 else { fatalError() }

	guard L2NormQ( vS ) == 14 else { fatalError() }
	guard L2NormQ( vD ) == 14 else { fatalError() }

	Equal( UnitVector( vS ), vectorS( 0.267261237, 0.534522474, 0.80178368 ) )
	Equal( UnitVector( vD ), vectorD( 0.2672612419124244, 0.53452248382484879, 0.80178372573727319 ) )

	Equal( Abs( vS ), vectorS( 1, 2, 3 ) )
	Equal( Abs( vD ), vectorD( 1, 2, 3 ) )

	Equal( -vS, vectorS( -1, -2, -3 ) )
	Equal( -vD, vectorD( -1, -2, -3 ) )

	guard Dot( vS, vS ) == 14 else { fatalError() }
	guard Dot( vD, vD ) == 14 else { fatalError() }

}

print( "Test v start" )
v()

func
Equal( _ l: Vector< Float >, _ r: Vector< Float > ) {
	guard l == r else { fatalError() }
}
func
Equal( _ l: Vector< Double >, _ r: Vector< Double > ) {
	guard l == r else { fatalError() }
}
func
VectorS( _ p1: Float, _ p2: Float, _ p3: Float ) -> Vector< Float > {
	return Vector( [ p1, p2, p3 ] )
}
func
VectorD( _ p1: Double, _ p2: Double, _ p3: Double ) -> Vector< Double > {
	return Vector( [ p1, p2, p3 ] )
}

func
V() {
	let	vS2 = Vector< Float  >( [ 1, 0, 2, 0, 3 ], 3, 2 )
	let	vS3 = Vector< Float  >( [ 1, 0, 0, 2, 0, 0, 3 ], 3, 3 )
	let	vD2 = Vector< Double >( [ 1, 0, 2, 0, 3 ], 3, 2 )
	let	vD3 = Vector< Double >( [ 1, 0, 0, 2, 0, 0, 3 ], 3, 3 )

	Equal( vS2 + vS3	, VectorS( 2, 4, 6 ) )
	Equal( vD2 + vD3	, VectorD( 2, 4, 6 ) )
	Equal( vS2 + 1.0	, VectorS( 2, 3, 4 ) )
	Equal( vD2 + 1.0	, VectorD( 2, 3, 4 ) )
	Equal( 1.0 + vS2	, VectorS( 2, 3, 4 ) )
	Equal( 1.0 + vD2	, VectorD( 2, 3, 4 ) )

	Equal( vS2 - vS3	, VectorS( 0, 0, 0 ) )
	Equal( vD2 - vD3	, VectorD( 0, 0, 0 ) )
	Equal( vS2 - 1.0	, VectorS( 0, 1, 2 ) )
	Equal( vD2 - 1.0	, VectorD( 0, 1, 2 ) )
	Equal( 1.0 - vS2	, VectorS( 0, -1, -2 ) )
	Equal( 1.0 - vD2	, VectorD( 0, -1, -2 ) )

	Equal( vS2 * vS3	, VectorS( 1, 4, 9 ) )
	Equal( vD2 * vD3	, VectorD( 1, 4, 9 ) )
	Equal( vS2 * 1.0	, VectorS( 1, 2, 3 ) )
	Equal( vD2 * 1.0	, VectorD( 1, 2, 3 ) )
	Equal( 1.0 * vS2	, VectorS( 1, 2, 3 ) )
	Equal( 1.0 * vD2	, VectorD( 1, 2, 3 ) )

	Equal( vS2 / vS3	, VectorS( 0.99999994, 0.99999994, 0.99999994 ) )
	Equal( vD2 / vD3	, VectorD( 1, 1, 1 ) )
	Equal( vS2 / 1.0	, VectorS( 1, 2, 3 ) )
	Equal( vD2 / 1.0	, VectorD( 1, 2, 3 ) )
	Equal( 1.0 / vS2	, VectorS( 0.99999994, 0.49999997, 0.333333313 ) )
	Equal( 1.0 / vD2	, VectorD( 1, 0.5, 0.33333333333333331 ) )

	Equal( RampVector( 3, Float( 1 ), Float( 2 ) ), VectorS( 1, 3, 5 ) )
	Equal( RampVector( 3, 1.0, 2.0 ), VectorD( 1, 3, 5 ) )

	guard Sum( vS2 ) == 6 else { fatalError() }
	guard Sum( vD2 ) == 6 else { fatalError() }

	guard Mean( vS2 ) == 2 else { fatalError() }
	guard Mean( vD2 ) == 2 else { fatalError() }

	guard L2NormQ( vS2 ) == 14 else { fatalError() }
	guard L2NormQ( vD2 ) == 14 else { fatalError() }

	Equal( UnitVector( vS2 ), VectorS( 0.267261237, 0.534522474, 0.80178368 ) )
	Equal( UnitVector( vD2 ), VectorD( 0.2672612419124244, 0.53452248382484879, 0.80178372573727319 ) )

	Equal( Abs( vS2 ), VectorS( 1, 2, 3 ) )
	Equal( Abs( vD2 ), VectorD( 1, 2, 3 ) )

	Equal( -vS2, VectorS( -1, -2, -3 ) )
	Equal( -vD2, VectorD( -1, -2, -3 ) )

	guard Dot( vS2, vS3 ) == 14 else { fatalError() }
	guard Dot( vD2, vD3 ) == 14 else { fatalError() }
}

print( "Test V start" )
V()


func
MakeMatrix< N: Numeric > ( _ p: [ [ N ] ] ) -> Matrix< N > {
	guard p.count > 0 else { fatalError( "Need to have at least one element." ) }
	var	v = Matrix< N >( p.count, p[ 0 ].count )
	for i in 0 ..< v.nR {
		guard p[ i ].count == v.nC else { fatalError( "All the rows must have same size." ) }
		let	w = i * v.nC
		v.m[ w ..< w + v.nC ] = ArraySlice( p[ i ] )
	}
	return v
}


func
TestMatrix() {
//	print( Matrix<Float>( nR: 1, nC: 1, u: [ 3 ] ).u[ 0 ] )

	guard Matrix<Float>( [ 1, 2, 3, 4, 5, 6 ], 2, 3 ).m[ 4 ] == 5 else { fatalError() }
	guard Matrix<Float>( 5, 2, 3 ).m[ 4 ] == 5 else { fatalError() }
	
	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).m[ 4 ] == 5 else { fatalError() }
	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] )[ 1 , 1 ] == 5 else { fatalError() }

	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Row( 1 ) == Vector( [ 4, 5, 6 ] ) else { fatalError() }
	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Col( 1 ) == Vector( [ 2, 5 ] ) else { fatalError() }

	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Rows( 1 ..< 2 ) == Matrix( [ 4, 5, 6 ], 1, 3 ) else { fatalError() }
	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Rows( 0 ... 1 ) == Matrix( [ 1, 2, 3, 4, 5, 6 ], 2, 3 ) else { fatalError() }
	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Cols( 1 ..< 2 ) == Matrix( [ 2, 5 ], 2, 1 ) else { fatalError() }
	guard MakeMatrix( [ [ 1, 2, 3 ], [ 4, 5, 6 ] ] ).Cols( 1 ... 2 ) == Matrix( [ 2, 3, 5, 6 ], 2, 2 ) else { fatalError() }

//	print( RandomMatrix( 2, 3, 0 ..< 10 ).ToString() )
//	print( RandomMatrix( 2, 3, 0 ... 10 ).ToString() )
//	print( RandomMatrix( 2, 3, 0 ..< Float( 10 ) ).ToString() )
//	print( RandomMatrix( 2, 3, 0 ... Float( 10 ) ).ToString() )
//	print( RandomMatrix( 2, 3, 0 ..< 10.0 ).ToString() )
//	print( RandomMatrix( 2, 3, 0 ... 10.0 ).ToString() )
}

print( "TestMatrix start" )
TestMatrix()

func
JPMatrixTestF() {

	let	w22 = ArraySlice( [ Float( 2 ), -2.0 ] );

	guard w22 + 2 == [ 4,  0 ] else { fatalError() }
	guard w22 - 2 == [ 0, -4 ] else { fatalError() }
	guard w22 * 2 == [ 4, -4 ] else { fatalError() }
	guard w22 / 2 == [ 1, -1 ] else { fatalError() }

	guard 2 + w22 == [ 4,  0 ] else { fatalError() }
	guard 2 - w22 == [ 0,  4 ] else { fatalError() }
	guard 2 * w22 == [ 4, -4 ] else { fatalError() }
	guard 2 / w22 == [ 0.99999994, -0.99999994 ] else { fatalError() }

	guard w22 + w22 == [ 4, -4 ] else { fatalError() }
	guard w22 - w22 == [ 0,  0 ] else { fatalError() }
	guard w22 * w22 == [ 4,  4 ] else { fatalError() }
	guard w22 / w22 == [ 0.99999994, 0.99999994 ] else { fatalError() }
	
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
	guard  2 / wM == Matrix( [  1.9999999  ,  0.99999994  ,  0.6666666,  0.49999997, 0.39999998, 0.3333333 ], 2, 3 ) else { fatalError() }
	guard wM + wM == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
	guard wM - wM == Matrix( [  0  ,  0  ,  0  ,  0  ,  0  ,  0 ], 2, 3 ) else { fatalError() }
	guard wM * wM == Matrix( [  1  ,  4  ,  9  , 16  , 25  , 36 ], 2, 3 ) else { fatalError() }
	guard wM / wM == Matrix( [  0.99999994  ,  0.99999994  ,  0.99999994  ,  0.99999994  ,  0.99999994  ,  0.99999994 ], 2, 3 ) else { fatalError() }
	
	guard Dot(
		wM
	,	Matrix( [ 1, 2, 3, 4, 5, 6 ], 3, 2 )
	) == Matrix<Float>( [ 22, 28, 49, 64 ], 2, 2 ) else {
		fatalError()
	}
	guard VDiv( wM, Vector( [ 1, 2 ] ) ) == Matrix( [ 1, 2, 3, 2, 2.5, 3 ], 2, 3 ) else { fatalError() }
	guard HDiv( wM, Vector( [ 1, 2, 3 ] ) ) == Matrix( [ 0.99999994, 0.99999994, 0.99999994, 3.9999998, 2.4999998, 1.9999999 ], 2, 3 ) else { fatalError() }

}

print( "JPMatrixTestF start" )
JPMatrixTestF()

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
	guard  2 / wM == Matrix( [  2  ,  1  ,  2.0/3.0	,  0.5, 0.4, 2.0/6.0 ], 2, 3 ) else { fatalError() }
	guard wM + wM == Matrix( [  2  ,  4  ,  6  ,  8  , 10  , 12 ], 2, 3 ) else { fatalError() }
	guard wM - wM == Matrix( [  0  ,  0  ,  0  ,  0  ,  0  ,  0 ], 2, 3 ) else { fatalError() }
	guard wM * wM == Matrix( [  1  ,  4  ,  9  , 16  , 25  , 36 ], 2, 3 ) else { fatalError() }
	guard wM / wM == Matrix( [  1  ,  1  ,  1  ,  1  ,  1  ,  1 ], 2, 3 ) else { fatalError() }
	
	guard Dot(
		wM
	,	Matrix( [ 1, 2, 3, 4, 5, 6 ], 3, 2 )
	) == Matrix( [ 22, 28, 49, 64 ], 2, 2 ) else {
		fatalError()
	}
	guard VDiv( wM, Vector( [ 1, 2 ] ) ) == Matrix( [ 1, 2, 3, 2, 2.5, 3 ], 2, 3 ) else { fatalError() }
	guard HDiv( wM, Vector( [ 1, 2, 3 ] ) ) == Matrix( [ 1, 1, 1, 4, 2.5, 2 ], 2, 3 ) else { fatalError() }
}

print( "JPMatrixTestD start" )
JPMatrixTestD()

print( "Over" )

/*
import	Accelerate

func sum( _ p: ArraySlice<Float>) -> Float {
    return sum(p, summer: vDSP_sve)
}

func sum( _ p: ArraySlice<Double>) -> Double {
    return sum(p, summer: vDSP_sveD)
}

func sum<N: Numeric>(_ p: ArraySlice<N>, summer: (UnsafePointer<N>, vDSP_Stride, UnsafeMutablePointer<N>, vDSP_Length) -> Void ) -> N {
    var v: N = 0
    summer(  p.withUnsafeBufferPointer { $0.baseAddress! }
        ,   vDSP_Stride( 1 )
        ,   &v
        ,   vDSP_Length( p.count )
    )
    return v
}

print( sum( [ 1.0, 2.0, 3.0 ] ) )
*/
print( Sum( [ 1.0, 2.0, 3.0 ] ) )
