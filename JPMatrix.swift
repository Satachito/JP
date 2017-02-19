//  Written by Satoru Ogura, Tokyo on 2016/12/26.

import Cocoa
import Accelerate

struct
Matrix {
	var	m	:	UInt
	var	n	:	UInt
	var	u	:	[ Double ]
	init( _ m: UInt, _ n: UInt ) {
		self.m = m
		self.n = n
		u = [ Double ]( repeating: 0, count: Int( m * n ) )
	}
	init( _ m: UInt, _ n: UInt, _ u: [ Double ] ) {
		guard Int( m * n ) == u.count else { fatalError() }
		self.m = m
		self.n = n
		self.u = u
	}
	
	func
	Get( _ r: UInt, _ c: UInt ) -> Double {
		return u[ Int( r * n + c ) ]
	}
	
	func
	Row( _ p: CountableRange<UInt> ) -> Matrix {
		let w = Array( u[ Int( p.lowerBound * n ) ..< Int( p.upperBound * n ) ] )
		return Matrix( UInt( p.count ), n, w )
	}
	func
	Row( _ p: CountableClosedRange<UInt> ) -> Matrix {
		let w = Array( u[ Int( p.lowerBound * n ) ..< Int( ( p.upperBound + 1 ) * n ) ] )
		return Matrix( UInt( p.count ), n, w )
	}
	func
	Col( _ p: CountableRange<UInt> ) -> Matrix {
		var	v = Matrix( m, UInt( p.count ) )
		for i in 0 ..< Int( m ) {
			let	wVS = i * p.count
			let	wSS = i * Int( n )
			v.u[ wVS ..< ( wVS + p.count ) ] = u[ wSS + Int( p.lowerBound ) ..< wSS + Int( p.upperBound ) ]
		}
		return v
	}
	func
	Col( _ p: CountableClosedRange<UInt> ) -> Matrix {
		var	v = Matrix( m, UInt( p.count ) )
		for i in 0 ..< Int( m ) {
			let	wVS = i * p.count
			let	wSS = i * Int( n )
			v.u[ wVS ..< ( wVS + p.count ) ] = u[ wSS + Int( p.lowerBound ) ... wSS + Int( p.upperBound ) ]
		}
		return v
	}
}

prefix func
~ ( p: Matrix ) -> Matrix {
	var	v = Matrix( p.n, p.m )
	vDSP_mtransD( p.u, 1, &v.u, 1, v.m, v.n )
	return v
}

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

func
== ( l: Matrix, r: Matrix ) -> Bool {
	if l.m != r.m { return false }
	if l.n != r.n { return false }
	return l.u == r.u
}

func
+ ( m: Matrix, s: Double ) -> Matrix {
	var	v = Matrix( m.m, m.n )
	var	w = s
	vDSP_vsaddD( m.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
- ( m: Matrix, s: Double ) -> Matrix {
	var	v = Matrix( m.m, m.n )
	var	w = -s
	vDSP_vsaddD( m.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
* ( m: Matrix, s: Double ) -> Matrix {
	var	v = Matrix( m.m, m.n )
	var	w = s
	vDSP_vsmulD( m.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
/ ( m: Matrix, s: Double ) -> Matrix {
	var	v = Matrix( m.m, m.n )
	var	w = s
	vDSP_vsdivD( m.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
^ ( m: Matrix, s: Double ) -> Matrix {
	var	t = Matrix( m.m, m.n )
	var	w = s
	vDSP_vfillD( &w, &t.u, 1, vDSP_Length( t.u.count ) )
	var	v = Matrix( m.m, m.n )
	var	wM = m
	var	wCount = Int32( v.u.count )
	vvpow( &v.u, &t.u, &wM.u, &wCount )
	return v
}

func
+ ( s: Double, m: Matrix ) -> Matrix {
	var	v = Matrix( m.m, m.n )
	var	w = s
	vDSP_vsaddD( m.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
- ( s: Double, m: Matrix ) -> Matrix {
	var	v = Matrix( m.m, m.n )
	var	w = -s
	vDSP_vsaddD( m.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
* ( s: Double, m: Matrix ) -> Matrix {
	var	v = Matrix( m.m, m.n )
	var	w = s
	vDSP_vsmulD( m.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}


func
+ ( l: Matrix, r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	var	v = Matrix( l.m, l.n )
	vDSP_vaddD( l.u, 1, r.u, 1, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
* ( l: Matrix, r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	var	v = Matrix( l.m, l.n )
	vDSP_vmulD( l.u, 1, r.u, 1, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
/ ( l: Matrix, r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	var	v = Matrix( l.m, l.n )
	vDSP_vdivD( r.u, 1, l.u, 1, &v.u, 1, vDSP_Length( v.u.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return v
}

func
Mul ( _ l: Matrix, _ r: Matrix ) -> Matrix {
	guard l.n == r.m else { fatalError() }
	var	v = Matrix( l.m, r.n )
	vDSP_mmulD( l.u, 1, r.u, 1, &v.u, 1, l.m, r.n, l.n )
	return v
}

func
DP( _ l: Matrix, _ r: Matrix ) -> Double {
	guard ( l.m == 1 && r.n == 1 ) || ( l.n == r.m ) else { fatalError() }
	var	v = 0.0
	vDSP_dotprD( r.u, 1, l.u, 1, &v, vDSP_Length( l.u.count ) )
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

