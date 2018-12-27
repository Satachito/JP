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
	F									pInitialValue
,	F									pLearningRate
,	const function< F	( const F& ) >&	pDerivative
,	const function< bool( const F& ) >&	pCallback
) {
	auto w = pInitialValue;
    do {
		w = w - pLearningRate * pDerivative( w );
	} while ( pCallback( w ) );
}

template	< typename F >	void
Main2D() {
	auto i = 0;
	GradientDescent< F >(
		1.0
	,	0.1
	,	[]( const F& p ) -> F { return 2.0 * p; } //  x^2 + n の導関数
	,	[&]( F p ) -> bool {
			cerr << i << ':' << p << endl;
			i += 1;
			return i < 40 ;          //  とりあえず 40 回で終わり
		}
	);
}

/*
+p0^2W0^2 +p1^2W1^2 +2p0W0p1W1 +t^2 -2tp0W0 -2tp1W1
∂W0		+2p0^2W0	+2p0W1p1	+2tp0
*/

/*
	FORWARD
	3x + 2y	= A
*/

template	< typename F >	void
F4() {
	auto		i = 0;
	Vector< F >	T = { 8.0, 11.0 };
	Matrix< F >	X( 2, 2, { 3.0, 2.0, 4.0, 3.0 } );
	auto a = X.Row( 0 );
	auto b = X.Row( 1 );
	auto aabb = 2.0 * V( X * X, Sum );				//	( a * a + b * b );
	auto a0b1 = 2.0 * ( a[ 0 ] * a[ 1 ] + b[ 0 ] * b[ 1 ] );
	Vector< F > tab = 2.0 * Dot( T, X );			//	Vector< F >{ T[ 0 ] * a[ 0 ] + T[ 1 ] * b[ 0 ], T[ 0 ] * a[ 1 ] + T[ 1 ] * b[ 1 ] };
	Vector< F >	current{ 3.0, 2.0 };
//	Vector< F >	T{ 8.0, 11.0 };
//	Matrix< F >	X( 2, 2, { 3, 2, 4, 3 } );
	GradientDescent< Vector< F > >(
		current
	,	Vector< F >{ 0.01, 0.01 }
	,	[&]( const vVector< F >& p ) -> Vector< F > {
			return aabb * p + a0b1 * Vector< F >{ p[ 1 ], p[ 0 ] } - tab;
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
			if ( i % 1000 == 0 ) cerr << i << ':' << p << ':' <<  endl;
			i += 1;
			current = p;
			return true;
		}
	);
}



template	< typename F >	struct
Affine {

	const	Matrix< F >	W;
	const	Vector< F >	b;

	const	vMatrix< F >	i;
	const	Matrix< F >		o;

	Matrix< F >
	Forward( const vMatrix< F >& p ) {
		o = Dot( p, W );
		for ( auto iR = 0; iR < o.nR; iR++ ) {
			
	}
};



int
main( int argc, char* argv[] ) {
//	Main2D< double >();
	F4< double >();
}

