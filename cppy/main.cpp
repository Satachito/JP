namespace JP {
	namespace Accelerate {
	}
	namespace CPU {
	}
}
using namespace JP;

#define	USE_ACCELERATE
#ifdef USE_ACCELERATE
using namespace JP::Accelerate;
#endif
#ifdef USE_CPU
using namespace JP::CPU;
#endif

#include	"JPVector.h"
#include	"JPMatrix.h"

#include	<iostream>
#include	<vector>
#include	<set>
using namespace std;

vector< float >
vectorS( float p1, float p2, float p3 ) {
	return vector< float > { p1, p2, p3 };
}

vector< double >
vectorD( double p1, double p2, double p3 ) {
	return vector< double > { p1, p2, p3 };
}


void
v() {
	vector< float >		vS { 1, 2, 3 };
	vector< double >	vD { 1, 2, 3 };
	

	assert( vS + vS		== vectorS( 2, 4, 6 ) );
	assert( vD + vD		== vectorD( 2, 4, 6 ) );
	assert( vS + 1.0f	== vectorS( 2, 3, 4 ) );
	assert( vD + 1.0	== vectorD( 2, 3, 4 ) );
	assert( 1.0f + vS	== vectorS( 2, 3, 4 ) );
	assert( 1.0 + vD	== vectorD( 2, 3, 4 ) );
	
	assert( vS - vS		== vectorS( 0, 0, 0 ) );
	assert( vD - vD		== vectorD( 0, 0, 0 ) );
	assert( vS - 1.0f	== vectorS( 0, 1, 2 ) );
	assert( vD - 1.0	== vectorD( 0, 1, 2 ) );
	assert( 1.0f - vS	== vectorS( 0, -1, -2 ) );
	assert( 1.0 - vD	== vectorD( 0, -1, -2 ) );
	
	assert( vS * vS		== vectorS( 1, 4, 9 ) );
	assert( vD * vD		== vectorD( 1, 4, 9 ) );
	assert( vS * 1.0f	== vectorS( 1, 2, 3 ) );
	assert( vD * 1.0	== vectorD( 1, 2, 3 ) );
	assert( 1.0f * vS	== vectorS( 1, 2, 3 ) );
	assert( 1.0 * vD	== vectorD( 1, 2, 3 ) );

#ifdef	USE_CPU	
	assert( vS / vS		== vectorS( 1, 1, 1 ) );
	assert( 1.0f / vS	== vectorS( 1, 0.5, 1.0 / 3.0 ) );
#endif
#ifdef	USE_ACCELERATE
	assert( 1.0f / vS	== vectorS( 0.99999994, 0.49999997, 0.333333313 ) );
	assert( vS / vS		== vectorS( 0.99999994, 0.99999994, 0.99999994 ) );
#endif
	assert( vD / vD		== vectorD( 1, 1, 1 ) );
	assert( vS / 1.0f	== vectorS( 1, 2, 3 ) );
	assert( vD / 1.0	== vectorD( 1, 2, 3 ) );
	assert( 1.0 / vD	== vectorD( 1, 0.5, 0.33333333333333331 ) );

	assert( RampArray< float >( 3, 1, 2 ) == vectorS( 1, 3, 5 ) );
	assert( RampArray< double >( 3, 1, 2 ) == vectorD( 1, 3, 5 ) );

	assert( Sum( vS ) == 6 );
	assert( Sum( vD ) == 6 );

	assert( Mean( vS ) == 2 );
	assert( Mean( vD ) == 2 );

	assert( L1Norm( vS ) == 6 );
	assert( L1Norm( vD ) == 6 );

	assert( L2NormQ( vS ) == 14 );
	assert( L2NormQ( vD ) == 14 );

	assert( -vS == vectorS( -1, -2, -3 ) );
	assert( -vD == vectorD( -1, -2, -3 ) );

	assert( Abs( vS ) == vectorS( 1, 2, 3 ) );
	assert( Abs( vD ) == vectorD( 1, 2, 3 ) );

	assert( UnitVector( vS ) == vectorS( 0.267261237, 0.534522474, 0.80178368 ) );
	assert( UnitVector( vD ) == vectorD( 0.2672612419124244, 0.53452248382484879, 0.80178372573727319 ) );

	assert( Dot( vS, vS ) == 14 );
	assert( Dot( vD, vD ) == 14 );
}

float	sS2[ 6 ] = { 1, 0, 2, 0, 3, 0 };
vVector< float >	vS2( sS2, 3, 2 );

float	sS3[ 9 ] = { 1, 0, 0, 2, 0, 0, 3, 0, 0 };
vVector< float >	vS3( sS3, 3, 3 );

double	sD2[ 6 ] = { 1, 0, 2, 0, 3, 0 };
vVector< double >	vD2( sD2, 3, 2 );

double	sD3[ 9 ] = { 1, 0, 0, 2, 0, 0, 3, 0, 0 };
vVector< double >	vD3( sD3, 3, 3 );

template	< typename T >	Vector< T >
Vector1( T p1, T p2, T p3 ) {
	T	v[] = { p1, p2, p3 };
	return Vector< T >( v, 3 );
}


void
V() {
	assert( vS2 == vS3 );
	{ Vector< float  > wS = vS2;	wS.Clear();	assert( wS == Vector1< float  >( 0, 0, 0 ) ); }
	{ Vector< double > wD = vD2;	wD.Clear();	assert( wD == Vector1< double >( 0, 0, 0 ) ); }
	{ Vector< float  > wS = vS2;	wS += vS3;	assert( wS == Vector1< float  >( 2, 4, 6 ) ); }
	{ Vector< double > wD = vD2;	wD += vD3;	assert( wD == Vector1< double >( 2, 4, 6 ) ); }
	{ Vector< float  > wS = vS2;	wS -= vS3;	assert( wS == Vector1< float  >( 0, 0, 0 ) ); }
	{ Vector< double > wD = vD2;	wD -= vD3;	assert( wD == Vector1< double >( 0, 0, 0 ) ); }
	{ Vector< float  > wS = vS2;	wS *= vS3;	assert( wS == Vector1< float  >( 1, 4, 9 ) ); }
	{ Vector< double > wD = vD2;	wD *= vD3;	assert( wD == Vector1< double >( 1, 4, 9 ) ); }
#ifdef	USE_CPU	
	{ Vector< float  > wS = vS2;	wS /= vS3;	assert( wS == Vector1< float  >( 1, 1, 1 ) ); }
#endif
#ifdef	USE_ACCELERATE
	{ Vector< float  > wS = vS2;	wS /= vS3;	assert( wS == Vector1< float  >( 0.99999994, 0.99999994, 0.99999994 ) ); }
#endif
	{ Vector< double > wD = vD2;	wD /= vD3;	assert( wD == Vector1< double >( 1, 1, 1 ) ); }
	{ Vector< float  > wS = vS2;	wS += 1;	assert( wS == Vector1< float  >( 2, 3, 4 ) ); }
	{ Vector< double > wD = vD2;	wD += 1;	assert( wD == Vector1< double >( 2, 3, 4 ) ); }
	{ Vector< float  > wS = vS2;	wS -= 1;	assert( wS == Vector1< float  >( 0, 1, 2 ) ); }
	{ Vector< double > wD = vD2;	wD -= 1;	assert( wD == Vector1< double >( 0, 1, 2 ) ); }
	{ Vector< float  > wS = vS2;	wS *= 2;	assert( wS == Vector1< float  >( 2, 4, 6 ) ); }
	{ Vector< double > wD = vD2;	wD *= 2;	assert( wD == Vector1< double >( 2, 4, 6 ) ); }
	{ Vector< float  > wS = vS2;	wS /= 2;	assert( wS == Vector1< float  >( 0.5, 1, 1.5 ) ); }
	{ Vector< double > wD = vD2;	wD /= 2;	assert( wD == Vector1< double >( 0.5, 1, 1.5 ) ); }

	assert( vS2 + vS3	== Vector1< float  >( 2, 4, 6 ) );
	assert( vD2 + vD3	== Vector1< double >( 2, 4, 6 ) );
	assert( vS2 + 1.0f	== Vector1< float  >( 2, 3, 4 ) );
	assert( vD2 + 1.0	== Vector1< double >( 2, 3, 4 ) );
	assert( 1.0f + vS2	== Vector1< float  >( 2, 3, 4 ) );
	assert( 1.0 + vD2	== Vector1< double >( 2, 3, 4 ) );
	
	assert( vS2 - vS3	== Vector1< float  >( 0, 0, 0 ) );
	assert( vD2 - vD3	== Vector1< double >( 0, 0, 0 ) );
	assert( vS2 - 1.0f	== Vector1< float  >( 0, 1, 2 ) );
	assert( vD2 - 1.0	== Vector1< double >( 0, 1, 2 ) );
	assert( 1.0f - vS2	== Vector1< float  >( 0, -1, -2 ) );
	assert( 1.0 - vD2	== Vector1< double >( 0, -1, -2 ) );
	
	assert( vS2 * vS3	== Vector1< float  >( 1, 4, 9 ) );
	assert( vD2 * vD3	== Vector1< double >( 1, 4, 9 ) );
	assert( vS2 * 1.0f	== Vector1< float  >( 1, 2, 3 ) );
	assert( vD2 * 1.0	== Vector1< double >( 1, 2, 3 ) );
	assert( 1.0f * vS2	== Vector1< float  >( 1, 2, 3 ) );
	assert( 1.0 * vD2	== Vector1< double >( 1, 2, 3 ) );
	
#ifdef	USE_CPU	
	assert( vS2 / vS3	== Vector1< float  >( 1, 1, 1 ) );
	assert( 1.0f / vS2	== Vector1< float  >( 1, 0.5, 1.0 / 3.0 ) );
#endif
#ifdef	USE_ACCELERATE
	assert( vS2 / vS3	== Vector1< float  >( 0.99999994, 0.99999994, 0.99999994 ) );
	assert( 1.0f / vS2	== Vector1< float  >( 0.99999994, 0.49999997, 0.333333313 ) );
#endif
	assert( vD2 / vD3	== Vector1< double >( 1, 1, 1 ) );
	assert( vS2 / 1.0f	== Vector1< float  >( 1, 2, 3 ) );
	assert( vD2 / 1.0	== Vector1< double >( 1, 2, 3 ) );
	assert( 1.0 / vD2	== Vector1< double >( 1, 0.5, 0.33333333333333331 ) );

	assert( RampVector< float  >( 3, 1, 2 ) == Vector1< float  >( 1, 3, 5 ) );
	assert( RampVector< double >( 3, 1, 2 ) == Vector1< double >( 1, 3, 5 ) );

	assert( Sum( vS2 ) == 6 );
	assert( Sum( vD2 ) == 6 );

	assert( Mean( vS2 ) == 2 );
	assert( Mean( vD2 ) == 2 );

	assert( Max( vS2 ) == 3 );
	assert( Max( vD2 ) == 3 );

	assert( Min( vS2 ) == 1 );
	assert( Min( vD2 ) == 1 );

	assert( L1Norm( vS2 ) == 6 );
	assert( L1Norm( vD2 ) == 6 );

	assert( L2NormQ( vS2 ) == 14 );
	assert( L2NormQ( vD2 ) == 14 );

	assert( UnitVector( vS2 ) == Vector1< float  >( 0.267261237		, 0.534522474			, 0.80178368			) );
	assert( UnitVector( vD2 ) == Vector1< double >( 0.2672612419124244	, 0.53452248382484879	, 0.80178372573727319	) );

	assert( Abs( vS2 ) == Vector1< float  >( 1, 2, 3 ) );
	assert( Abs( vD2 ) == Vector1< double >( 1, 2, 3 ) );

	assert( -vS2 == Vector1< float  >( -1, -2, -3 ) );
	assert( -vD2 == Vector1< double >( -1, -2, -3 ) );

	assert( Dot( vS2, vS3 ) == 14 );
	assert( Dot( vD2, vD3 ) == 14 );
}

template	< typename T >	vector< T >
Sigmoid( const vector< T >& p ) {
	vector< T >	v( p.size() );
	for ( auto i = 0; i < p.size(); i++ ) {
		auto w = exp( p[ i ] );
		v[ i ] = w / ( 1 + w );
	}
	return v;
}

template	< typename T >	Vector< T >
Sigmoid( const vVector< T >& p ) {
	Vector< T >	v( p.n );
	for ( auto i = 0; i < p.n; i++ ) {
		auto w = exp( p[ i ] );
		v[ i ] = w / ( 1 + w );
	}
	return v;
}

int main(int argc, const char * argv[]) {
//	for ( auto i = 0; i < 10; i++ ) cerr << UniformRandomFloat< float >() << endl;
//	cerr << endl;
//	for ( auto i = 0; i < 10; i++ ) cerr << NormalRandom< float >() << endl;

	{	auto	vS = Sigmoid( RampArray< float >( 7, -3, 1 ) );
		for ( auto i = 0; i < vS.size(); i++ ) cerr << '\t' << vS[ i ];
		cerr << endl;

		auto	vD = Sigmoid( RampArray< double >( 7, -3, 1 ) );
		for ( auto i = 0; i < vD.size(); i++ ) cerr << '\t' << vD[ i ];
		cerr << endl;
	}

	{	auto	vS = Sigmoid( RampVector< float >( 7, -3, 1 ) );
		cerr << vS << endl;

		auto	vD = Sigmoid( RampVector< double >( 7, -3, 1 ) );
		cerr << vS << endl;
	}

	cerr << "Test v start" <<  endl;
	v();
	cerr << "Test V start" <<  endl;
	V();
	cerr << "over" <<  endl;
	return 0;
}
