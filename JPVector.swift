//	Written by Satoru Ogura, Tokyo.
//

import Accelerate

//	() -> Array

func
RandomArray( _ p: Int, _ range: Range<Int> ) -> ArraySlice< Int > {
	return ArraySlice( ( 0 ..< p ).map { _ in Int.random( in: range ) } )
}
func
RandomArray( _ p: Int, _ range: Range<Float> ) -> ArraySlice< Float > {
	return ArraySlice( ( 0 ..< p ).map { _ in Float.random( in: range ) } )
}
func
RandomArray( _ p: Int, _ range: Range<Double> ) -> ArraySlice< Double > {
	return ArraySlice( ( 0 ..< p ).map { _ in Double.random( in: range ) } )
}
func
RandomArray( _ p: Int, _ range: ClosedRange<Int> ) -> ArraySlice< Int > {
	return ArraySlice( ( 0 ..< p ).map { _ in Int.random( in: range ) } )
}
func
RandomArray( _ p: Int, _ range: ClosedRange<Float> ) -> ArraySlice< Float > {
	return ArraySlice( ( 0 ..< p ).map { _ in Float.random( in: range ) } )
}
func
RandomArray( _ p: Int, _ range: ClosedRange<Double> ) -> ArraySlice< Double > {
	return ArraySlice( ( 0 ..< p ).map { _ in Double.random( in: range ) } )
}

func
Ramp( _ p: Int, _ pInit: Float = 0, _ pStep: Float = 1 ) -> ArraySlice< Float > {
	var	v = [ Float ]( repeating: 0, count: p )
	var	wInit = pInit
	var	wStep = pStep
	vDSP_vramp( &wInit, &wStep, &v, 1, vDSP_Length( p ) )
	return ArraySlice( v )
}
func
Ramp( _ p: Int, _ pInit: Double = 0, _ pStep: Double = 1 ) -> ArraySlice< Double > {
	var	v = [ Double ]( repeating: 0, count: p )
	var	wInit = pInit
	var	wStep = pStep
	vDSP_vrampD( &wInit, &wStep, &v, 1, vDSP_Length( p ) )
	return ArraySlice( v )
}

//	Vector -> Scalar

func
Sum( _ p: ArraySlice< Float  > ) -> Float  {
	var	v: Float  = 0
	p.withUnsafeBufferPointer { vDSP_sve ( $0.baseAddress!, 1, &v, vDSP_Length( p.count ) ) }
	return v
}
func
Sum( _ p: ArraySlice< Double > ) -> Double {
	var	v: Double  = 0
	p.withUnsafeBufferPointer { vDSP_sveD( $0.baseAddress!, 1, &v, vDSP_Length( p.count ) ) }
	return v
}
func
Mean( _ p: ArraySlice< Float  > ) -> Float  {
	var	v: Float  = 0
	p.withUnsafeBufferPointer { vDSP_meanv ( $0.baseAddress!, 1, &v, vDSP_Length( p.count ) ) }
	return v
}
func
Mean( _ p: ArraySlice< Double > ) -> Double {
	var	v: Double  = 0
	p.withUnsafeBufferPointer { vDSP_meanvD( $0.baseAddress!, 1, &v, vDSP_Length( p.count ) ) }
	return v
}
func
L2NormQ( _ p: ArraySlice< Float  > ) -> Float  {
	var	v: Float  = 0
	p.withUnsafeBufferPointer { vDSP_svesq ( $0.baseAddress!, 1, &v, vDSP_Length( p.count ) ) }
	return v
}
func
L2NormQ( _ p: ArraySlice< Double > ) -> Double {
	var	v: Double  = 0
	p.withUnsafeBufferPointer { vDSP_svesqD( $0.baseAddress!, 1, &v, vDSP_Length( p.count ) ) }
	return v
}
func
L2Norm( _ p: ArraySlice< Float  > ) -> Float  {
	return sqrt( L2NormQ( p ) )
}
func
L2Norm( _ p: ArraySlice< Double > ) -> Double {
	return sqrt( L2NormQ( p ) )
}
func
DistanceQ( _ l: ArraySlice< Float  >, _ r: ArraySlice< Float  > ) -> Float  {
	var	v: Float  = 0
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_distancesq ( pL.baseAddress!, 1, pR.baseAddress!, 1, &v, vDSP_Length( l.count ) )
		}
	}
	return v
}
func
DistanceQ( _ l: ArraySlice< Double >, _ r: ArraySlice< Double > ) -> Double {
	var	v: Double  = 0
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_distancesqD( pL.baseAddress!, 1, pR.baseAddress!, 1, &v, vDSP_Length( l.count ) )
		}
	}
	return v
}

//	Vector -> Vector

func
UnitVector( _ p: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	return p / L2Norm( p )
}
func
UnitVector( _ p: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	return p / L2Norm( p )
}

func
Abs( _ p: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	var	v = [ Float ]( repeating: 0, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vabs( $0.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
func
Abs( _ p: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	var	v = [ Double ]( repeating: 0, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vabsD( $0.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}

prefix func
-( _ p: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	var	v = [ Float ]( repeating: 0, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vneg( $0.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
prefix func
-( _ p: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	var	v = [ Double ]( repeating: 0, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vnegD( $0.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}

//	( Vector, Vector ) -> Vector
//	( Scalar, Vector ) -> Vector
//	( Vector, Scalar ) -> Vector

func
+( _ l: ArraySlice< Float  >, _ r: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vadd( pL.baseAddress!, 1, pR.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}
func
+( _ l: ArraySlice< Double  >, _ r: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vaddD( pL.baseAddress!, 1, pR.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}

func
+( _ p: ArraySlice< Float  >, _ s: Float ) -> ArraySlice< Float  > {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	p.withUnsafeBufferPointer { vDSP_vsadd( $0.baseAddress!, 1, &w, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
func
+( _ p: ArraySlice< Double  >, _ s: Double ) -> ArraySlice< Double  > {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	p.withUnsafeBufferPointer { vDSP_vsaddD( $0.baseAddress!, 1, &w, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
func
+( _ s: Float, _ p: ArraySlice< Float  > ) -> ArraySlice< Float  > { return p + s }
func
+( _ s: Double, _ p: ArraySlice< Double  > ) -> ArraySlice< Double  > { return p + s }

func
-( _ l: ArraySlice< Float  >, _ r: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vsub( pR.baseAddress!, 1, pL.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}
func
-( _ l: ArraySlice< Double  >, _ r: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vsubD( pR.baseAddress!, 1, pL.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}

func
-( _ p: ArraySlice< Float  >, _ s: Float ) -> ArraySlice< Float  > { return p + -s }
func
-( _ p: ArraySlice< Double  >, _ s: Double ) -> ArraySlice< Double  > { return p + -s }

func
-( _ s: Float, _ p: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	var	v = [ Float ]( repeating: s, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vsub( $0.baseAddress!, 1, v, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
func
-( _ s: Double, _ p: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	var	v = [ Double ]( repeating: s, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vsubD( $0.baseAddress!, 1, v, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}

func
*( _ l: ArraySlice< Float  >, _ r: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vmul( pL.baseAddress!, 1, pR.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}
func
*( _ l: ArraySlice< Double  >, _ r: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vmulD( pL.baseAddress!, 1, pR.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}

func
*( _ p: ArraySlice< Float  >, _ s: Float ) -> ArraySlice< Float  > {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	p.withUnsafeBufferPointer { vDSP_vsmul( $0.baseAddress!, 1, &w, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
func
*( _ p: ArraySlice< Double  >, _ s: Double ) -> ArraySlice< Double  > {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	p.withUnsafeBufferPointer { vDSP_vsmulD( $0.baseAddress!, 1, &w, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}

func
*( _ s: Float, _ p: ArraySlice< Float  > ) -> ArraySlice< Float  > { return p * s }
func
*( _ s: Double, _ p: ArraySlice< Double  > ) -> ArraySlice< Double  > { return p * s }

func
/( _ l: ArraySlice< Float  >, _ r: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vdiv( pR.baseAddress!, 1, pL.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}
func
/( _ l: ArraySlice< Double  >, _ r: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_vdivD( pR.baseAddress!, 1, pL.baseAddress!, 1, &v, 1, vDSP_Length( v.count ) )
		}
	}
	return ArraySlice( v )
}

func
/( _ p: ArraySlice< Float  >, _ s: Float ) -> ArraySlice< Float  > {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	p.withUnsafeBufferPointer { vDSP_vsdiv( $0.baseAddress!, 1, &w, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
func
/( _ p: ArraySlice< Double  >, _ s: Double ) -> ArraySlice< Double  > {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	p.withUnsafeBufferPointer { vDSP_vsdivD( $0.baseAddress!, 1, &w, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}

func
/( _ s: Float, _ p: ArraySlice< Float  > ) -> ArraySlice< Float  > {
	var	v = [ Float ]( repeating: s, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vdiv( $0.baseAddress!, 1, v, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}
func
/( _ s: Double, _ p: ArraySlice< Double  > ) -> ArraySlice< Double  > {
	var	v = [ Double ]( repeating: s, count: p.count )
	p.withUnsafeBufferPointer { vDSP_vdivD( $0.baseAddress!, 1, v, 1, &v, 1, vDSP_Length( v.count ) ) }
	return ArraySlice( v )
}

//	( Vector, Vector ) -> Scalar

func
DotProduct( _ l: ArraySlice< Float  >, _ r: ArraySlice< Float  > ) -> Float {
	guard l.count == r.count else { fatalError() }
	var	v: Float = 0.0
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_dotpr( pR.baseAddress!, 1, pL.baseAddress!, 1, &v, vDSP_Length( l.count ) )
		}
	}
	return v
}
func
DotProduct( _ l: ArraySlice< Double  >, _ r: ArraySlice< Double  > ) -> Double {
	guard l.count == r.count else { fatalError() }
	var	v: Double = 0.0
	l.withUnsafeBufferPointer { pL in
		r.withUnsafeBufferPointer { pR in
			vDSP_dotprD( pR.baseAddress!, 1, pL.baseAddress!, 1, &v, vDSP_Length( l.count ) )
		}
	}
	return v
}

