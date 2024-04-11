#include <iostream>
#include <cmath>

template	< typename F >	F
DiffQ( F xS, F yS, F xC, F yC, F xE, F yE ) {
	auto	wCx = xC - xS;
	auto	wCy = yC - yS;
	if ( xS == xE && yS == yE ) return wCx * wCx + wCy * wCy;
	auto	wEx = xE - xS;
	auto	wEy = yE - yS;
	auto	w = wEy * wCx - wEx * wCy;
	return w * w / ( wEx * wEx + wEy * wEy );
}

template	< typename F >	struct
Coord {
	F	x;
	F	y;
		Coord( F x = 0, F y = 0 ) : x( x ), y( y ) {}
	F	L2NormQ() const { return x * x + y * y; }
};
template < typename F > Coord< F > operator
-( const Coord< F >& l, const Coord< F >& r ) { return Coord< F >( l.x - r.x, l.y - r.y ); }
template < typename F > Coord< F > operator
+( const Coord< F >& l, const Coord< F >& r ) { return Coord< F >( l.x + r.x, l.y + r.y ); }
template < typename F > Coord< F > operator
*( F l, const Coord< F >& r ) { return Coord< F >( l * r.x, l * r.y ); }
template < typename F > Coord< F > operator
/( const Coord< F >& l, F r ) { return Coord< F >( l.x / r, l.y / r ); }

template	< typename F >	F
Bezier2DLength( const Coord< F >& c, const Coord< F >& e ) {
	auto wC = ( 2.0 * c + e ) / 4.0;
	auto wD = e.L2NormQ();
	if ( wD != 0 ) {
		auto wN = e.x * wC.y - e.y * wC.x;
		if ( wN * wN / wD < 0.5 * 0.5 ) return sqrt( wD );
	}
	return Bezier2DLength( c / 2.0, wC ) + Bezier2DLength( ( c + e ) / 2.0 - wC, e - wC );
}
template	< typename F >	F
Bezier2DLength( F xS, F yS, F xC, F yC, F xE, F yE  ) {
	return Bezier2DLength( Coord< F >( xC - xS, yC - yS ), Coord< F >( xE - xS, yE - yS ) );
}
template	< typename F >	F
Bezier3DLength( const Coord< F >& c1, const Coord< F >& c2, const Coord< F >& e ) {
	auto wC = ( 3.0 * c1 + 3.0 * c2 + e ) / 8.0;
	auto wD = e.L2NormQ();
	if ( wD != 0 ) {
		auto wN1 = e.x * c1.y - e.y * c1.x;
		auto wN2 = e.x * c2.y - e.y * c2.x;
		if ( wN1 * wN2 >= 0 ) {
			auto wN = e.x * wC.y - e.y * wC.x;
			if ( wN * wN / wD < 0.5 * 0.5 ) return sqrt( wD );
		}
	}
	return
		Bezier3DLength( c1 / 2.0, ( c1 + c1 + c2 ) / 4.0, wC )
	+	Bezier3DLength( ( c1 + c2 + c2 + e ) / 4.0 - wC, ( c2 + e ) / 2.0 - wC, e - wC )
	;
}
template	< typename F >	F
Bezier3DLength( F xS, F yS, F xC1, F yC1, F xC2, F yC2, F xE, F yE  ) {
	return Bezier3DLength( Coord< F >( xC1 - xS, yC1 - yS ), Coord< F >( xC2 - xS, yC2 - yS ), Coord< F >( xE - xS, yE - yS ) );
}


//template	< typename F >	F
//Bezier2DLength( F xS, F yS, F xC, F yC, F xE, F yE  ) {
//	if ( DiffQ( xS, yS, xC, yC, xE, yE ) < 0.5 * 0.5 ) {
//		auto	wEx = xE - xS;
//		auto	wEy = yE - yS;
//		return sqrt( wEx * wEx + wEy * wEy );
//	}
//	auto	wCx = ( xS + xC + xC + xE ) / 4.0;
//	auto	wCy = ( yS + yC + yC + yE ) / 4.0;
//	return
//		Bezier2DLength( xS, yS, ( xS + xC ) / 2, ( yS + yC ) / 2, wCx, wCy )
//	+	Bezier2DLength( wCx, wCy, ( xC + xE ) / 2, ( yC + yE ) / 2, xE, yE )
//	;
//}

using namespace std;
int main(int argc, const char * argv[]) {
	cout << Bezier2DLength< double >( 0, 100, 100, 100, 100, 0 ) * 2 << endl;
	auto	K = ( -24 + sqrt( 24 * 24 + 64 * 9 ) ) / 18;
	cout << K * 100 << endl;
	cout << Bezier3DLength< double >( 0, 100, K * 100, 100, 100, K * 100, 100, 0 ) * 2 << endl;
	cout << Bezier3DLength< double >( 0, 0, 0, 100, 100, 0, 100, 100 ) << endl;
	return 0;
}

//	( 0, 1 ) - ( N, 1 ) - ( 1, N ) - ( 1, 0 )
//	x: ( 3N + 4 ) / 8, y: ( 3N + 4 ) / 8
//	9N^2 + 24N - 16 = 0
//	-24 +- sqrt( 24 * 24 + 64 * 9 ) / 18
//	24 * 24 = 576

//	N : 1
//	( 0, 0 ) - ( 0, 1 ) - ( 0, 1 ) - ( 1, 1 )
//	x = 1 / 8
