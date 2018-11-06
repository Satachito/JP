//	Written by Satoru Ogura, Tokyo.
//
func
Gaussian() -> Float {
	return
		sqrt( -2 * log( Float.random( in: 0 ..< 1 ) ) )
	*	sin( 2 * .pi * Float.random( in: 0 ..< 1 ) )
}

func
Gaussian() -> Double {
	return
		sqrt( -2 * log( Double.random( in: 0 ..< 1 ) ) )
	*	sin( 2 * .pi * Double.random( in: 0 ..< 1 ) )
}

func
Square<T: Numeric>( _ p: T ) -> T {
	return p * p
}

func
Cube<T: Numeric>( _ p: T ) -> T {
	return p * p * p
}

import Accelerate

prefix func
-( _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	vDSP_vneg( p, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func
+( _ p: [ Float ], _ s: Float ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsadd( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
+( _ p: [ Double ], _ s: Double ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsaddD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
+( _ s: Float, _ p: [ Float ] ) -> [ Float ] { return p + s }
func
+( _ s: Double, _ p: [ Double ] ) -> [ Double ] { return p + s }

func
-( _ p: [ Float ], _ s: Float ) -> [ Float ] { return p + -s }
func
-( _ p: [ Double ], _ s: Double ) -> [ Double ] { return p + -s }
func
-( _ s: Float, _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: s, count: p.count )
	vDSP_vsub( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
-( _ s: Double, _ p: [ Double ] ) -> [ Double ] {
	var	v = [ Double ]( repeating: s, count: p.count )
	vDSP_vsubD( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func
*( _ p: [ Float ], _ s: Float ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsmul( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
*( _ p: [ Double ], _ s: Double ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsmulD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
*( _ s: Float, _ p: [ Float ] ) -> [ Float ] { return p * s }
func
*( _ s: Double, _ p: [ Double ] ) -> [ Double ] { return p * s }

func
/( _ p: [ Float ], _ s: Float ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsdiv( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
/( _ p: [ Double ], _ s: Double ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vsdivD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
/( _ s: Float, _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfill( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vdiv( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
/( _ s: Double, _ p: [ Double ] ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	w = s
	vDSP_vfillD( &w, &v, 1, vDSP_Length( v.count ) )
	vDSP_vdivD( p, 1, v, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

//
//	VECTOR - VECTOR
//
func
+( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vadd( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
+( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vaddD( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func
-( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vsub( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
-( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vsubD( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func
*( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vmul( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
*( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vmulD( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func
/( _ l: [ Float ], _ r: [ Float ] ) -> [ Float ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Float ]( repeating: 0, count: l.count )
	vDSP_vdiv( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}
func
/( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
	guard l.count == r.count else { fatalError() }
	var	v = [ Double ]( repeating: 0, count: l.count )
	vDSP_vdivD( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
	return v
}

func
DotProduct( _ l: [ Float ], _ r: [ Float ] ) -> Float {
	guard l.count == r.count else { fatalError() }
	var	v: Float = 0.0
	vDSP_dotpr( r, 1, l, 1, &v, vDSP_Length( l.count ) )
	return v
}
func
DotProduct( _ l: [ Double ], _ r: [ Double ] ) -> Double {
	guard l.count == r.count else { fatalError() }
	var	v: Double = 0.0
	vDSP_dotprD( r, 1, l, 1, &v, vDSP_Length( l.count ) )
	return v
}

func
RandomVector( _ p: Int, _ range: Range<Int> ) -> [ Int ] {
	var	v = [ Int ]( repeating: 0, count: p )
	for i in 0 ..< v.count { v[ i ] = Int.random( in: range ) }
	return v
}
func
RandomVector( _ p: Int, _ range: Range<Float> ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p )
	for i in 0 ..< v.count { v[ i ] = Float.random( in: range ) }
	return v
}
func
RandomVector( _ p: Int, _ range: Range<Double> ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p)
	for i in 0 ..< v.count { v[ i ] = Double.random( in: range ) }
	return v
}
func
RandomVector( _ p: Int, _ range: ClosedRange<Int> ) -> [ Int ] {
	var	v = [ Int ]( repeating: 0, count: p )
	for i in 0 ..< v.count { v[ i ] = Int.random( in: range ) }
	return v
}
func
RandomVector( _ p: Int, _ range: ClosedRange<Float> ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p )
	for i in 0 ..< v.count { v[ i ] = Float.random( in: range ) }
	return v
}
func
RandomVector( _ p: Int, _ range: ClosedRange<Double> ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p)
	for i in 0 ..< v.count { v[ i ] = Double.random( in: range ) }
	return v
}



func
Ramp( _ p: Int, _ pInit: Float = 0, _ pStep: Float = 1 ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p )
	var	wInit = pInit
	var	wStep = pStep
	vDSP_vramp( &wInit, &wStep, &v, 1, vDSP_Length( p ) )
	return v
}
func
Ramp( _ p: Int, _ pInit: Double = 0, _ pStep: Double = 1 ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p )
	var	wInit = pInit
	var	wStep = pStep
	vDSP_vrampD( &wInit, &wStep, &v, 1, vDSP_Length( p ) )
	return v
}

func
Abs( _ p: [ Float ] ) -> [ Float ] {
	var	v = [ Float ]( repeating: 0, count: p.count )
	vDSP_vabs( p, 1, &v, 1, vDSP_Length( p.count ) )
	return v
}
func
Abs( _ p: [ Double ] ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	vDSP_vabsD( p, 1, &v, 1, vDSP_Length( p.count ) )
	return v
}

func
Sum( _ p: [ Float ] ) -> Float {
	var	v: Float = 0.0
	vDSP_sve( p, 1, &v, vDSP_Length( p.count ) )
	return v
}
func
Sum( _ p: [ Double ] ) -> Double {
	var	v: Double = 0.0
	vDSP_sveD( p, 1, &v, vDSP_Length( p.count ) )
	return v
}

func
Mean( _ p: [ Float ] ) -> Float {
	var	v: Float = 0.0
	vDSP_meanv( p, 1, &v, vDSP_Length( p.count ) )
	return v
}
func
Mean( _ p: [ Double ] ) -> Double {
	var	v: Double = 0.0
	vDSP_meanvD( p, 1, &v, vDSP_Length( p.count ) )
	return v
}

func
L1Norm( _ p: [ Float ] ) -> Float {
	var	v: Float = 0.0
	vDSP_svemg( p, 1, &v, vDSP_Length( p.count ) )
	return v
}
func
L1Norm( _ p: [ Double ] ) -> Double {
	var	v: Double = 0.0
	vDSP_svemgD( p, 1, &v, vDSP_Length( p.count ) )
	return v
}

func
L2NormSquare( _ p: [ Float ] ) -> Float {
	var	v: Float = 0.0
	vDSP_svesq( p, 1, &v, vDSP_Length( p.count ) )
	return v
}
func
L2NormSquare( _ p: [ Double ] ) -> Double {
	var	v: Double = 0.0
	vDSP_svesqD( p, 1, &v, vDSP_Length( p.count ) )
	return v
}

func
L2Norm( _ p: [ Float ] ) -> Float {
	return sqrt( L2NormSquare( p ) )
}
func
L2Norm( _ p: [ Double ] ) -> Double {
	return sqrt( L2NormSquare( p ) )
}

func
UnitVector( _ p: [ Float ] ) -> [ Float ] {
	return p / L2Norm( p )
}

func
UnitVector( _ p: [ Double ] ) -> [ Double ] {
	return p / L2Norm( p )
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
Normalize( _ p: [ Double ] ) -> [ Double ] {
	var	v = [ Double ]( repeating: 0, count: p.count )
	var	wM = Double( 0 )
	var	wSD = Double( 0 )
	vDSP_normalizeD( p, 1, &v, 1, &wM, &wSD, vDSP_Length( p.count ) )
	return v
}
