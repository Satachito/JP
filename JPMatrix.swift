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
}

func
+ ( l: Matrix, r: Double ) -> Matrix {
	var	v = Matrix( l.m, l.n )
	var	w = r
	vDSP_vsaddD( l.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
* ( l: Matrix, r: Double ) -> Matrix {
	var	v = Matrix( l.m, l.n )
	var	w = r
	vDSP_vsmulD( l.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
	return v
}

func
/ ( l: Matrix, r: Double ) -> Matrix {
	var	v = Matrix( l.m, l.n )
	var	w = r
	vDSP_vsdivD( l.u, 1, &w, &v.u, 1, vDSP_Length( v.u.count ) )
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
	if ( l.m == 1 && r.m == 1 ) || ( l.n == 1 && r.n == 1 ) {
		guard l.m == r.m && l.n == r.n else { fatalError() }
		var	v = Matrix( l.m, l.n )
		vDSP_vmulD( l.u, 1, r.u, 1, &v.u, 1, vDSP_Length( v.u.count ) )
		return v
	} else {
		guard l.m == r.n && l.n == r.m else { fatalError() }
		var	v = Matrix( l.m, r.n )
		vDSP_mmulD( l.u, 1, r.u, 1, &v.u, 1, l.m, r.n, l.n )
		return v
	}
}

func
/ ( l: Matrix, r: Matrix ) -> Matrix {
	guard l.m == r.m && l.n == r.n else { fatalError() }
	var	v = Matrix( l.m, l.n )
	vDSP_vdivD( r.u, 1, l.u, 1, &v.u, 1, vDSP_Length( v.u.count ) )	//	Note. In vDSP_vdivD, order of A and B is different from other vDSP_XX
	return v
}

func
DotProduct( l: Matrix, r: Matrix ) -> Double {
	guard ( l.m == 1 && r.m == 1 ) || ( l.n == 1 && r.n == 1 ) else { fatalError() }
	var	v = 0.0
	vDSP_dotprD( r.u, 1, l.u, 1, &v, vDSP_Length( l.u.count ) )
	return v
}

func
Transpose( p: Matrix ) -> Matrix {
	var	v = Matrix( p.n, p.m )
	vDSP_mtransD( p.u, 1, &v.u, 1, v.m, v.n )
	return v
}

prefix func
! ( p: Matrix ) -> Matrix {
	var v = p	//	TODO 実態コピーされるか確認
	var pivot : __CLPK_integer = 0
	var workspace = 0.0
	var error : __CLPK_integer = 0

	var N = __CLPK_integer( sqrt( Double( v.u.count ) ) )
	dgetrf_( &N, &N, &v.u, &N, &pivot, &error )

	guard error == 0 else { fatalError() }

	dgetri_( &N, &v.u, &N, &pivot, &workspace, &N, &error )
	return v
}
/*
func
Test() {
	print( Matrix( 2, 1, [ 1, 2 ] ) + 3 )
	print( Matrix( 2, 1, [ 1, 2 ] ) * 3 )
	print( Matrix( 2, 1, [ 1, 2 ] ) / 3 )
	print( Matrix( 2, 1, [ 2, 5 ] ) + Matrix( 2, 1, [ 3, 4 ] ) )
	print( Matrix( 2, 1, [ 2, 5 ] ) * Matrix( 2, 1, [ 3, 4 ] ) )
	print( Matrix( 2, 1, [ 3, 4 ] ) / Matrix( 2, 1, [ 2, 5 ] ) )
	print(
		Matrix( 3, 2, [ 3.0, 2.0, 4.0, 5.0, 6.0, 7.0 ] )
	*	Matrix( 2, 3, [ 10.0, 20.0, 30.0, 30.0, 40.0, 50.0 ] )
	)
	print(
		Matrix( 4, 3, [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 ] )
	*	Matrix( 3, 4, [ 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 ] )
	)
	print( !Matrix( 2, 2, [ 1, 2, 3, 4 ] ) )
}
*/
