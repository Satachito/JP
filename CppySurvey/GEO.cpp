#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
using namespace std;

template	< typename F >	F
DistanceR( F latL, F lonL, F latR, F lonR ) {
    return 6378137.0 * 2 * asin(
		sqrt(
			pow( sin( ( latL - latR ) / 2 ), 2 )
		+	cos( latL ) * cos( latR ) * pow( sin( ( lonL - lonR ) / 2 ), 2 )
		)
	);
}
template	< typename F >	F
DistanceD( F latL, F lonL, F latR, F lonR ) {
	return DistanceR(
		latL / 180 * M_PI
	,	lonL / 180 * M_PI
	,	latR / 180 * M_PI
	,	lonR / 180 * M_PI
	);
}

int
main( int argc, char* argv[] ) {
	cerr << DistanceD( 35.68944, 139.69167, 35.85694, 139.64889 ) << endl;
	cerr << DistanceD( 35.68944, 139.69167, 43.06417,141.34694 ) << endl;
	cerr << DistanceD( 35.68944, 139.69167, 26.2125,127.68111 ) << endl;
	cerr << DistanceD( 43.06417,141.34694, 26.2125,127.68111 ) << endl;
}

