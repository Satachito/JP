#include <iostream>
#include <vector>
using namespace std;

#include	"JPMatrix.h"
using namespace JP;
/*
template	< typename F, typename Derivative, typename Callback >	void
GradientDescent(
	F			pInitialValue
,	F			pLearningRate
,	Derivative	pDerivative
,	Callback		pCallback
) {
	auto w = pInitialValue;
    do {
		w = w - pLearningRate * pDerivative( w );
	} while ( pCallback( w ) );
}
*/

template	< typename F >	void
GradientDescent(
	const F&							pInitialValue
,	const F&							pLearningRate
,	const function< F	( const F& ) >&	pDerivative
,	const function< bool( const F& ) >&	pCallback
) {
	auto w = pInitialValue;
    do {
    	w -= pLearningRate * pDerivative( w );
	} while ( pCallback( w ) );
}

template	< typename F >	Vector< F >
NumericalGradient(
	const vVector< F >&								p
,	const function< F ( const vVector< F >& ) >&	f
) {
	Vector< F >	w = p;
	Vector< F > v( p.n );
	for ( auto i = 0; i < p.n; i++ ) {
		w[ i ] = p[ i ] - 0.0001;
		auto l = f( w );
		w[ i ] = p[ i ] + 0.0001;
		auto r = f( w );
		w[ i ] = p[ i ];
		v[ i ] = ( r - l ) / 0.0002;
	}
	return v;
}

template	< typename F >	void
Main2D() {
	auto i = 0;
	GradientDescent< F >(
		1.0
	,	0.01
	,	[]( const F& p ) -> F { return 2.0 * p; } //  x^2 + n の導関数
	,	[&]( F p ) -> bool {
			cerr << i << ':' << p << endl;
			i += 1;
			return i < 100 ;
		}
	);
}

template	< typename F >	void
ByABS() {
	auto i = 0;
	GradientDescent< F >(
		1.0
	,	0.1
	,	[]( const F& p ) -> F { return ( p - 3 ) < 0 ? -1: 1; } //  abs( x ) の導関数
	,	[&]( F p ) -> bool {
			i += 1;
			cerr << i << ':' << p << endl;
			return i < 100 ;
		}
	);
}

/*
	abs( ax + by - S )
 +	abs( cx + dy - T )
 
 2x + 3y - 8 = 0
 4x + 5y - 14 = 0
 
これはうまくみつからない。
Graph を書いて確認すればわかるが、局所解がたくさん現れる。
*/

template	< typename F >	int
Sign( const F& p ) {
	return p == 0 ? 0 : p < 0 ? -1 : 1;
}

template	< typename F >	void
LossByABS() {
	cerr << ( 2 * 1.09 + 3 * 1.94 - 8 ) << ':' << ( 4 * 1.09 + 5 * 1.94 - 14 ) << endl;
//	cerr << ( 2 * 1.36 + 3 * 1.82 - 8 ) << ':' << ( 4 * 1.36 + 5 * 1.82 - 14 ) << endl;
	auto i = 0;
	Vector< F >	current{ 0, 0 };
	GradientDescent< Vector< F > >(
		current
	,	Vector< F >{ 0.001, 0.001 }
	,	[&]( const vVector< F >& p ) -> Vector< F > {
			auto x = p[ 0 ];
			auto y = p[ 1 ];
			F	a = 2;
			F	b = 3;
			F	c = 4;
			F	d = 5;
			auto dX = ( a * Sign( a*x + b*y - 8 ) ) + ( c * Sign( c*x + d*y - 14 ) );
			auto dY = ( b * Sign( a*x + b*y - 8 ) ) + ( d * Sign( c*x + d*y - 14 ) );
//			cerr << Vector< F >{ dX, dY } << endl;
//			cerr << NumericalGradient< F >(
//				p
//			,	[&]( const vVector< F >& p ) {
//					auto x = p[ 0 ];
//					auto y = p[ 1 ];
//					return abs( a*x + b*y - 8 ) + abs( c*x + d*y - 14 );
//				}
//			) << endl;
			return Vector< F >{ dX, dY };
		} //  abs( x ) の導関数
	,	[&]( const vVector< F >& p ) -> bool {
			for ( auto w: p ) {
				if ( isinf( w ) ) {
					cerr << i << ':' << p << ':' <<  "発散しました" <<  endl;
					return false;
				}
			}
			if ( DistanceQ( p, current ) < 1E-32 ) {
				cerr << i << ':' << p << ':' << "見つけました" <<  endl;
				return false;
			}
			if ( i >= 100000 ) {
				cerr << i << ':' << p << ':' << "規定回数に達しました" <<  endl;
				return false;
			}
			if ( i % 10000 == 0 ) cerr << i << ':' << p << ':' <<  endl;
			i += 1;
			current = p;
			return true;
		}
	);
}


/*
	( ax + y - S )^2
 +	( bx + y - T )^2
 
 	a^2x^2 +y^2 +S^2 +2axy -2axS -2yS
 	b^2x^2 +y^2 +T^2 +2bxy -2bxT -2yT

 	(a^2+b^2)x^2 +2y^2 +2(a+b)xy -2(aS+bT)x -2y(S+T) +S^2 +T^2

	2(a^2+b^2)x	+2(a+b)y	-2(aS+bT)
	4y			+2(a+b)x	-2(S+T)

 	2ax^2 +2x(b -t)
	2b + 2(ax-t)
 
	x: 1 -> t:2
	x: 2 -> t:3
*/
template	< typename F >	void
LossBy2D() {
	auto i = 0;
	Vector< F >	X{ 1, 2 };
	Vector< F >	A{ 2, 3 };
	Vector< F >	current{ 0, 0 };
	//	a: 1, b: 1
	GradientDescent< Vector< F > >(
		current
	,	Vector< F >{ 0.01, 0.01 }
	,	[&]( const vVector< F >& p ) -> Vector< F > {
			auto	a = X[ 0 ];
			auto	b = X[ 1 ];
			auto	S = A[ 0 ];
			auto	T = A[ 1 ];
			auto	x = p[ 0 ];
			auto	y = p[ 1 ];
			return Vector< F >{
				2 * ( a * a + b * b )	* x	+ 2 * ( a + b ) * y	- 2 * ( a * S + b * T )
			,	2 * ( a + b )			* x	+ 4				* y	- 2 * ( S + T )
			};
		}
	,	[&]( const vVector< F >& p ) -> bool {
			for ( auto w: p ) {
				if ( isinf( w ) ) {
					cerr << i << ':' << p << ':' <<  "発散しました" <<  endl;
					return false;
				}
			}
			if ( DistanceQ( p, current ) < 1E-32 ) {
				cerr << i << ':' << p << ':' << "見つけました" <<  endl;
				return false;
			}
			if ( i >= 100000 ) {
				cerr << i << ':' << p << ':' << "規定回数に達しました" <<  endl;
				return false;
			}
			if ( i % 10000 == 0 ) cerr << i << ':' << p << ':' <<  endl;
			i += 1;
			current = p;
			return true;
		}
	);
}

/*
	( ax + by - S )^2
 +	( cx + dy - T )^2
 
 	aax^2 +bby^2 +S^2 +2axby -2axS -2byS
 	ccx^2 +ddy^2 +T^2 +2cxdy -2cxT -2dyT

	+(aa+cc)x^2 +(bb+dd)y^2 +2(ab+cd)xy -2(aS+cT)x -2y(bS+dT) +S^2 +T^2

	+2(aa+cc)x	+2(ab+cd)y	-2(aS+cT)
	+2(ab+cd)x	+2(bd+dd)y	-2(bS+bT)
*/
/*
ID	マグロ	ハマチ	客数	お勘定
0	3貫		2貫		2	10$
1	4貫		1貫		3	12$
2	2貫		1貫		1	6$

お勘定＝貫数＊マグロ単価＋客数＊お通し単価

答
	マグロ単価	2$
	ハマチ単価	1$
	お通し		1$

単価を仮定
	マグロ単価	3$
	ハマチ単価	0$
	お通し		2$
 
	( 3x + 2y + 2z - 10 )^2		//	( ax + by + cz - S )^2
+	( 4x + 1y + 3z - 12 )^2		//	( dx + ey + fz - T )^2
+	( 2x + 1y + 1z -  6 )^2		//	( gx + hy + iz - U )^2
	+ 9x^2 +4y^2 + 4z^2 +100 +12xy +12xz - 60x + 8yz -40y - 40z
	+16x^2 + y^2 + 9z^2 +144 + 8xy +24xz - 96x + 6yz -24y - 72z
	+ 4x^2 + y^2 +  z^2 + 36 + 4xy + 4xz - 24x + 2yz -12y - 12z
 
	+29x^2		+6y^2		+14z^2		+280			+24xy		+40xz		+16yz		-180x		-76y			-124z
 	+(aa+dd+gg)x^2	+(bb+ee+hh)y^2	+(cc+ff+ii)z^2	+(SS+TT+UU)	+2(ab+de+gh)xy	+2(ac+df+gi)xz	+2(bc+ef+hi)yz	-2(aS+dT+gU)x	-2(bS+eT+hU)y	-2(cS+fT+iU)z

∂x	58x +24y +40z -180
∂y	12y +24x +16z - 76
∂z	28z +40x +16y -124
 
 		a		b		c
 		d		e		f
 		g		h		i
a d g 	aa+dd+gg	ab+de+gh	ac+df+gi
b e h 	ab+de+gh	bb+ee+hh	bc+ef+hi
c f i 	ac+df+gi	bc+ef+hi	cc+ff+ii

	tab:
2(a.t0+d.t1+g.t2)
2(b.t0+e.t1+h.t2)
2(c.t0+f.t1+i.t2)
									x
									y
									z
2(aa+dd+gg)	2(ab+de+gh)	2(ac+df+gi)	+2(aa+dd+gg)x+2(ab+de+gh)y+2(ac+df+gi)z
2(ab+de+gh)	2(bb+ee+hh)	2(bc+ef+hi)	+2(ab+de+gh)x+2(bb+ee+hh)y+2(bc+ef+hi)z
2(ac+df+gi)	2(bc+ef+hi)	2(cc+ff+ii)	+2(ac+df+gi)x+2(bc+ef+hi)y+2(cc+ff+ii)z
*/
template	< typename F >	void
SolveAffine(
	const vMatrix< F >& X
,	const vVector< F >& A
,	const vVector< F >& p
,	const vVector< F >& η
) {
	auto sp = 2.0 * Dot( ~X, X );
	Vector< F > tab = 2.0 * Dot( A, X );
	Vector< F >	current = p;
	auto		i = 0;
	GradientDescent< Vector< F > >(
		current
	,	η
	,	[&]( const vVector< F >& p ) -> Vector< F > {
			return Dot( sp, p ) - tab;
		}
	,	[&]( const vVector< F >& p ) -> bool {
			for ( auto w: p ) {
				if ( isinf( w ) ) {
					cerr << i << ':' << p << ':' <<  "発散しました" <<  endl;
					return false;
				}
			}
			if ( DistanceQ( p, current ) < 1E-32 ) {
				cerr << i << ':' << p << ':' << Dot( X, p ) << ':' << "見つけました" <<  endl;
				return false;
			}
			if ( i >= 100000 ) {
				cerr << i << ':' << p << ':' << "規定回数に達しました" <<  endl;
				return false;
			}
			if ( i % 10000 == 0 ) cerr << i << ':' << p << ':' <<  endl;
			i += 1;
			current = p;
			return true;
		}
	);
}

template	< typename F >	struct
Aff {
			Matrix< F >		W;
	const	vMatrix< F >*	i;
			Matrix< F >		o;
			Matrix< F >		dW;
	Aff( const vMatrix< F > W )
	:	W( W )
	,	dW( W.nR, W.nC ) {
	}
	const Matrix< F >&
	Forward( const vMatrix< F >& p ) {
		i = &p;
		o = Dot( p, W );
		return o;
	}
	Matrix< F >
	Backward( const vMatrix< F >& p ) {
		auto sp = 2.0 * Dot( ~W, w );
		Vector< F > tab = 2.0 * Dot( A, X );
		return Dot( sp, p ) - tab;
	}
};

template	< typename F >	struct
Affine {
			Matrix< F >		W;
			Vector< F >		b;

	const	vMatrix< F >*	i;
			Matrix< F >		o;

			Matrix< F >		dW;
			Vector< F >		db;

	Affine( const vMatrix< F > W, const vVector< F > b )
	:	W( W )
	,	b( b )
	,	dW( W.nR, W.nC )
	,	db( b.n ) {
	}
	const Matrix< F >&
	Forward( const vMatrix< F >& p ) {
		i = &p;
		o = Dot( p, W );
		for ( auto iR = 0; iR < o.nR; iR++ ) o.Row( iR ) += b;
		return o;
	}
	Matrix< F >
	Backward( const vMatrix< F >& p ) {
		auto v = Dot( o, ~p );
		dW = Dot( ~*i, p );
		db = V( p, Sum );
		return v;
	}
};
/*
	1	2	3	4
	5	6	7	8
1 2	11	14	17	20
3 4	23	30	37	44
5 6	35	46	57	68
+	1	2	3	4
----------------------
	12	16	20	24
	24	32	40	48
	36	48	60	72
 
		a
		b
		c
x y z	ax+by+cz
+		d
----------------------
		ax+by+cz + d
*/
template	< typename F >	void
TestAffine() {
	Affine< F > w( Matrix< F >( 2, 4, { 1, 2, 3, 4, 5, 6, 7, 8 } ), Vector< F >{ 1, 2, 3, 4 } );
	cerr << w.Forward( Matrix< F >( 3, 2, { 1, 2, 3, 4, 5, 6 } ) ) << endl;
	cerr << w.Backward( - w.o / 3.0 ) << endl;
	cerr << w.dW << endl;
	cerr << w.db << endl;
	w.W += w.dW;
	w.b += w.db;
	cerr << w.Forward( Matrix< F >( 3, 2, { 1, 2, 3, 4, 5, 6 } ) ) << endl;
}

int
main( int argc, char* argv[] ) {
//	Vector< double > T{ 1, 2, 3 };
//	Matrix< double > X( 3, 2, { 1, 2, 3, 4, 5, 6 } );
//	cout << Dot( T, X ) << endl;	//	1, nR, nR, nC	-> 1, nC
//	cout << Dot( ~X, T ) << endl;	//	nC, nR, nR, 1	-> nC, 1
//	LossByABS< double >();
//	Main2DLoss< double >();
	SolveAffine(
		Matrix< double >( 2, 2, { 3, 2, 4, 3 } )
	,	Vector< double >{ 8, 11 }
	,	Vector< double >{ 3, 2 }
	,	Vector< double >{ 0.01, 0.01 }
	);
	SolveAffine(
		Matrix< double >( 3, 3, { 3, 2, 2, 4, 1, 3, 2, 1, 1 } )
	,	Vector< double >{ 10, 12, 6 }
	,	Vector< double >{ 1000, 100, -1000 }
	,	Vector< double >{ 0.01, 0.01, 0.01 }
	);
	TestAffine< double >();
}

