import Foundation

import Accelerate

func
+ ( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
    var v = [ Double ]( repeating: 0, count: l.count )
     vDSP_vaddD( l, 1, r, 1, &v, 1, vDSP_Length( v.count ) )
    return v
}

func
+ ( _ p: [ Double ], _ s: Double ) -> [ Double ] {
    var v = [ Double ]( repeating: 0, count: p.count )
    var w = s
    vDSP_vsaddD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
    return v
}

func
- ( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
    guard l.count == r.count else { fatalError() }
    var v = [ Double ]( repeating: 0, count: l.count )
    vDSP_vsubD( r, 1, l, 1, &v, 1, vDSP_Length( v.count ) )
    return v
}

func
* ( _ l: [ Double ], _ r: [ Double ] ) -> [ Double ] {
    guard l.count == r.count else { fatalError() }
    var v = [ Double ]( repeating: 0, count: l.count )
    vDSP_vmulD( l, 1, r, 1, &v, 1, vDSP_Length( v.count ) )
    return v
}

func
* ( _ s: Double, _ p: [ Double ] ) -> [ Double ] {
    var v = [ Double ]( repeating: 0, count: p.count )
    var w = s
    vDSP_vsmulD( p, 1, &w, &v, 1, vDSP_Length( v.count ) )
    return v
}

func
Dot( _ l: [ Double ], _ r: [ Double ] ) -> Double {
	var v: Double = 0
	vDSP_dotprD( r.withUnsafeBufferPointer { $0.baseAddress! }, 1, l.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( l.count ) )
	return v
}

func
DistanceQ( _ p: [ Double ] ) -> Double {
    var v = 0.0
    vDSP_svesqD( p, 1, &v, vDSP_Length( p.count ) )
    return v
}

func
GradientDescent(
  _	pInitialValue	: [ Double ]
, _	pLearningRate	: [ Double ]
, _	pDerivative		: ( [ Double ] ) -> [ Double ]
, _	pCallback		: ( [ Double ] ) -> Bool
) {
	var w = pInitialValue
	repeat { w = w - pLearningRate * pDerivative( w ) } while pCallback( w )
}

func
F1() {
	var i = 0
	GradientDescent(
		[ 1.0, 1.0 ]
	,	[ 0.1, 0.1 ]
	,	{ p in 2 * p }	//  x^2 + y^2 + n の導関数
	) { p in						//  コールバック
		if i % 10 == 0 { print( i, p ) }
		i += 1
		return i < 100				//  とりあえず 40 回で終わり
	}
}
F1()

func
F2() {
	//	x^2 + 2xy + y^2
	var i = 0
	GradientDescent(
		[ 1.0, 1.0 ]
	,	[ 0.1, 0.1 ]
	,	{ p in 2 * p + 2 * [ p[ 1 ], p[ 0 ] ] }
	) { p in
		if i % 10 == 0 { print( i, p ) }
		i += 1
		return i < 100				//  とりあえず 40 回で終わり
	}
}
F2()


/*
ID	貫数	客数	お勘定
0	3貫	2	8$
1	4貫	3	11$

お勘定＝貫数＊マグロ単価＋客数＊お通し単価

答
	マグロ単価	2$
	お通し		1$

単価を仮定
	マグロ単価	3$
	お通し		2$

loss = ( お勘定 - ( 貫数＊仮マグロ単価＋客数＊仮お通し単価 ) )^2
x: 仮マグロ単価
y: 仮お通し単価

	(	+3x	+2y	- 8	)^2
+	(	+4x	+3y	-11	)^2
+9x^2	+4y^2	+ 64		+12xy	-48x		-32y
+16x^2	+9y^2	+121		+24xy	-88x		-66y
-----------------------------------------------------
+25x^2	+13y^2	+185		+36xy	-136x	-98y



( 8 - ( 3x + 2y ) )^2 + ( 11 - ( 4x + 3y ) )^2
 +64 -16( 3x +2y ) +9x^2 + 12xy + 4y^2 +121 -22( 4x + 3y ) + 16x^2 + 24xy + 9y^2

 +16x^2 +9x^2 -48x -88x +12xy +24xy -66y -32y +4y^2 +9y^2 +64 +121
 +25x^2 -136x +36xy -98y +13y^2 +64 +121

 +64 -48x -32y  +9x^2 + 12xy + 4y^2
+121 -88x -66y +16x^2 + 24xy + 9y^2

  ( ( p[0]W[0] + p[1]W[1] ) - t )^2

+p0^2W0^2 +p1^2W1^2 +2p0W0p1W1 +t^2 -2tp0W0 -2tp1W1
∂W0		+2p0^2W0	+2p0W1p1	+2tp0
*/

/*
	FORWARD
	3x + 2y	= A
*/

func
F3() {
	let	t = [ 8.0, 11.0 ]
	let	a = [ 3.0, 2.0 ]
	let	b = [ 4.0, 3.0 ]
	let	tab: [ Double ] = 2 * ( [ t[ 0 ] * a[ 0 ] + t[ 1 ] * b[ 0 ], t[ 0 ] * a[ 1 ] + t[ 1 ] * b[ 1 ] ] )
	var i = 0
	var	current = [ 3.0, 2.0 ]
	GradientDescent(
		current
	,	[ 0.01, 0.01 ]
	,	{ p -> [ Double ] in
			let aabb: [ Double ] = 2 * ( a * a + b * b )
			let a0b1: Double = 2 * ( a[ 0 ] * a[ 1 ] + b[ 0 ] * b[ 1 ] )
			let v = aabb * p + a0b1 * [ p[ 1 ], p[ 0 ] ] - tab
			print( v )
			return v
//			return [
//				2.0 * ( ( a[ 0 ] * a[ 0 ] + b[ 0 ] * b[ 0 ] ) * p[ 0 ] + ( ( a[ 0 ] * a[ 1 ] ) + ( b[ 0 ] * b[ 1 ] ) ) * p[ 1 ] - ( t[ 0 ] * a[ 0 ] + t[ 1 ] * b[ 0 ] ) )
//			,	2.0 * ( ( a[ 1 ] * a[ 1 ] + b[ 1 ] * b[ 1 ] ) * p[ 1 ] + ( ( a[ 0 ] * a[ 1 ] ) + ( b[ 0 ] * b[ 1 ] ) ) * p[ 0 ] - ( t[ 0 ] * a[ 1 ] + t[ 1 ] * b[ 1 ] ) )
//			]
//			return [
//				50 * x + 36 * y - 136
//			,	36 * x + 26 * y -  98
//			]
		}
	) { p in
		if i >= 100000 {
			print( i, p, "規定回数に達しました" )
			return false
		} else if DistanceQ( p - current ) < 1E-32 {
			print( i, p, "見つけました" )
			return false
		} else {
			for w in p {
				if w == .infinity || w == -.infinity {
					print( i, p, "発散しました" )
					return false
				}
			}
		}
		i += 1
		current = p
		return true
	}
}
F3()

/*
ID	マグロ	ハマチ	客数	お勘定
0	3貫		2貫		2	10$
1	4貫		1貫		3	12$
1	2貫		1貫		1	6$

お勘定＝貫数＊マグロ単価＋客数＊お通し単価

答
	マグロ単価	2$
	ハマチ単価	1$
	お通し		1$

単価を仮定
	マグロ単価	3$
	ハマチ単価	0$
	お通し		2$

loss = ( お勘定 - ( マグロ貫数＊仮マグロ単価＋ハマチ貫数＊仮ハマチ単価＋客数＊仮お通し単価 ) )^2
+	( 10 - ( 3 * W.0 + 2 * W.1 + 2 * W.2 ) )^2
+	( 12 - ( 4 * W.0 + 1 * W.1 + 3 * W.2 ) )^2
+	(  6 - ( 3 * W.0 + 1 * W.1 + 1 * W.2 ) )^2

( a + b ) ^ 2 = a^2 + 2ab + b^2
( a + b + c ) ^ 2 = ( ( a + b ) + c ) ^2 = a^2 + 2ab + b^2 + 2c( a + b ) + c^2 = a^2 + b^2 + c^2 + 2ab + 2ac + 2ab
( a + b + c + d ) ^ 2 = ( ( a + b ) + ( c + d ) ) ^2 = a^2 + 2ab + b^2 + 2( a + b )( c + d ) + c^2 + 2cd + d^2 = a^2 + b^2 + c^2 + d^2 + 2ab + 2ac + 2ad + 2bc + 2bd + 2cd
( t - ( p0W0 + p1W1 + p2W2 ) )^2
+t^2 -2tp0W0 -2tp1W1 -2tp2W2 +(p0W0)^2 +(p1W1)^2 +(p2W2)^2 +2p0W0p1W1 +2p0W0p2W2 +2p1W1p2W2
+(p0W0)^2 +(p1W1)^2 +(p2W2)^2 -2tp0W0 -2tp1W1 -2tp2W2 +2p0W0p1W1 +2p0W0p2W2 +2p1W1p2W2 +t^2

 +(p0W0)^2 -2tp0W0 + 2p0W0p1W1 + 2p0W0p2W2
 +2p0^2W0 + ( -2tp0 +2p0p1W1 + 2p0p2W2 )
 +2p0( +p0W0 +p1W1 +p2W2 -t )
*/
func
F4() {
	let	t = [ 10.0, 12.0, 6.0 ]
	let	p = [ 9.0, 4.0, 6.0 ]
	var i = 0
	var	current = [ 3.0, 0.0, 2.0 ]
	GradientDescent(
		current
	,	[ 0.01, 0.01, 0.01 ]
	,	{ W -> [ Double ] in
			let w = 2 * Dot( p, W );
			return 2 * p * ( [ w, w, w ] - t )
		}
	) { p in
		if i >= 100000 {
			print( i, p, "規定回数に達しました" )
			return false
		} else if DistanceQ( p - current ) < 1E-32 {
			print( i, p, "見つけました" )
			return false
		} else {
			for w in p {
				if w == .infinity || w == -.infinity {
					print( i, p, "発散しました" )
					return false
				}
			}
		}
		i += 1
		current = p
		return true
	}
}
F4()

