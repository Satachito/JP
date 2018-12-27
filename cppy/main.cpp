
#include	<iostream>
#include	<vector>
#include	<set>
using namespace std;

//#define	JP_USE_CPU
#include	"JPVector.h"
#include	"JPMatrix.h"
using namespace JP;

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
#else
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

	assert( Rec( vS ) == vectorS( 1, 0.5, 1.0/3.0 ) );
	assert( Rec( vD ) == vectorD( 1, 0.5, 1.0/3.0 ) );

	assert( Exp( vS ) == vectorS( exp( 1 ), exp( 2 ), exp( 3 ) ) );
	assert( Exp( vD ) == vectorD( exp( 1 ), exp( 2 ), exp( 3 ) ) );

	assert( UnitVector( vS ) == vectorS( 0.267261237, 0.534522474, 0.80178368 ) );
	assert( UnitVector( vD ) == vectorD( 0.2672612419124244, 0.53452248382484879, 0.80178372573727319 ) );

	assert( Dot( vS, vS ) == 14 );
	assert( Dot( vD, vD ) == 14 );

	assert( DistanceQ( vS, vS ) == 0 );
	assert( DistanceQ( vD, vD ) == 0 );
}

template	< typename F >	Vector< F >
NewVector() {
	Vector< F >	v{ 0, 1, 2 };
	return v;
}

template	< typename F >	void
Test() {
	F	w[ 3 ] = { 1, 2, 3 };
	vVector< F >	wv3( w, 3 );
	Vector< F >		w1( wv3 );
	Vector< F >		w2 = wv3;
	Vector< F >		w3( vVector< F >( w, 3 ) );
	Vector< F >		w4 = vVector< F >( w, 3 );
	Vector< F >		w5( w3 );
	Vector< F >		w6 = w4;
	vVector< F >	wv2( w, 2 );
	w1 = Vector< F >( wv2 );
	w2 = move( w1 );
	w1 = wv3;
	w2 = w1;

	Vector< F >	w7 = NewVector< F >();
	Vector< F >	w8 = move( w7 );
	w8 = vVector< F >( w, 1 );
}

float	sS1[ 3 ] = { 1, 2, 3 };
vVector< float >	vS1( sS1, 3, 1 );

float	sS2[ 6 ] = { 1, 0, 2, 0, 3, 0 };
vVector< float >	vS2( sS2, 3, 2 );

float	sS3[ 9 ] = { 1, 0, 0, 2, 0, 0, 3, 0, 0 };
vVector< float >	vS3( sS3, 3, 3 );

double	sD1[ 3 ] = { 1, 2, 3 };
vVector< double >	vD1( sD1, 3, 1 );

double	sD2[ 6 ] = { 1, 0, 2, 0, 3, 0 };
vVector< double >	vD2( sD2, 3, 2 );

double	sD3[ 9 ] = { 1, 0, 0, 2, 0, 0, 3, 0, 0 };
vVector< double >	vD3( sD3, 3, 3 );

template	< typename T >	Vector< T >
Vector2( T p1, T p2 ) {
	return Vector< T >{ p1, p2 };
}
template	< typename T >	Vector< T >
Vector3( T p1, T p2, T p3 ) {
	return Vector< T >{ p1, p2, p3 };
}


void
V() {
	Test< float >();
	Test< double >();
	assert( vS2 == vS3 );
	{ Vector< float  > wS = vS2;	wS.Clear();	assert( wS == Vector3< float  >( 0, 0, 0 ) ); }
	{ Vector< double > wD = vD2;	wD.Clear();	assert( wD == Vector3< double >( 0, 0, 0 ) ); }
	{ Vector< float  > wS = vS2;	wS += vS3;	assert( wS == Vector3< float  >( 2, 4, 6 ) ); }
	{ Vector< double > wD = vD2;	wD += vD3;	assert( wD == Vector3< double >( 2, 4, 6 ) ); }
	{ Vector< float  > wS = vS2;	wS -= vS3;	assert( wS == Vector3< float  >( 0, 0, 0 ) ); }
	{ Vector< double > wD = vD2;	wD -= vD3;	assert( wD == Vector3< double >( 0, 0, 0 ) ); }
	{ Vector< float  > wS = vS2;	wS *= vS3;	assert( wS == Vector3< float  >( 1, 4, 9 ) ); }
	{ Vector< double > wD = vD2;	wD *= vD3;	assert( wD == Vector3< double >( 1, 4, 9 ) ); }
#ifdef	USE_CPU
	{ Vector< float  > wS = vS2;	wS /= vS3;	assert( wS == Vector3< float  >( 1, 1, 1 ) ); }
#else
	{ Vector< float  > wS = vS2;	wS /= vS3;	assert( wS == Vector3< float  >( 0.99999994, 0.99999994, 0.99999994 ) ); }
#endif
	{ Vector< double > wD = vD2;	wD /= vD3;	assert( wD == Vector3< double >( 1, 1, 1 ) ); }
	{ Vector< float  > wS = vS2;	wS += 1;	assert( wS == Vector3< float  >( 2, 3, 4 ) ); }
	{ Vector< double > wD = vD2;	wD += 1;	assert( wD == Vector3< double >( 2, 3, 4 ) ); }
	{ Vector< float  > wS = vS2;	wS -= 1;	assert( wS == Vector3< float  >( 0, 1, 2 ) ); }
	{ Vector< double > wD = vD2;	wD -= 1;	assert( wD == Vector3< double >( 0, 1, 2 ) ); }
	{ Vector< float  > wS = vS2;	wS *= 2;	assert( wS == Vector3< float  >( 2, 4, 6 ) ); }
	{ Vector< double > wD = vD2;	wD *= 2;	assert( wD == Vector3< double >( 2, 4, 6 ) ); }
	{ Vector< float  > wS = vS2;	wS /= 2;	assert( wS == Vector3< float  >( 0.5, 1, 1.5 ) ); }
	{ Vector< double > wD = vD2;	wD /= 2;	assert( wD == Vector3< double >( 0.5, 1, 1.5 ) ); }

	assert( vS2 + vS3	== Vector3< float  >( 2, 4, 6 ) );
	assert( vD2 + vD3	== Vector3< double >( 2, 4, 6 ) );
	assert( vS2 + 1.0f	== Vector3< float  >( 2, 3, 4 ) );
	assert( vD2 + 1.0	== Vector3< double >( 2, 3, 4 ) );
	assert( 1.0f + vS2	== Vector3< float  >( 2, 3, 4 ) );
	assert( 1.0 + vD2	== Vector3< double >( 2, 3, 4 ) );
	
	assert( vS2 - vS3	== Vector3< float  >( 0, 0, 0 ) );
	assert( vD2 - vD3	== Vector3< double >( 0, 0, 0 ) );
	assert( vS2 - 1.0f	== Vector3< float  >( 0, 1, 2 ) );
	assert( vD2 - 1.0	== Vector3< double >( 0, 1, 2 ) );
	assert( 1.0f - vS2	== Vector3< float  >( 0, -1, -2 ) );
	assert( 1.0 - vD2	== Vector3< double >( 0, -1, -2 ) );
	
	assert( vS2 * vS3	== Vector3< float  >( 1, 4, 9 ) );
	assert( vD2 * vD3	== Vector3< double >( 1, 4, 9 ) );
	assert( vS2 * 1.0f	== Vector3< float  >( 1, 2, 3 ) );
	assert( vD2 * 1.0	== Vector3< double >( 1, 2, 3 ) );
	assert( 1.0f * vS2	== Vector3< float  >( 1, 2, 3 ) );
	assert( 1.0 * vD2	== Vector3< double >( 1, 2, 3 ) );
	
#ifdef	USE_CPU
	assert( vS2 / vS3	== Vector3< float  >( 1, 1, 1 ) );
	assert( 1.0f / vS2	== Vector3< float  >( 1, 0.5, 1.0 / 3.0 ) );
#else
	assert( vS2 / vS3	== Vector3< float  >( 0.99999994, 0.99999994, 0.99999994 ) );
	assert( 1.0f / vS2	== Vector3< float  >( 0.99999994, 0.49999997, 0.333333313 ) );
#endif
	assert( vD2 / vD3	== Vector3< double >( 1, 1, 1 ) );
	assert( vS2 / 1.0f	== Vector3< float  >( 1, 2, 3 ) );
	assert( vD2 / 1.0	== Vector3< double >( 1, 2, 3 ) );
	assert( 1.0 / vD2	== Vector3< double >( 1, 0.5, 0.33333333333333331 ) );

	assert( RampVector< float  >( 3, 1, 2 ) == Vector3< float  >( 1, 3, 5 ) );
	assert( RampVector< double >( 3, 1, 2 ) == Vector3< double >( 1, 3, 5 ) );

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

	assert( UnitVector( vS2 ) == Vector3< float  >( 0.267261237			, 0.534522474			, 0.80178368			) );
	assert( UnitVector( vD2 ) == Vector3< double >( 0.2672612419124244	, 0.53452248382484879	, 0.80178372573727319	) );

	assert( -vS2 == Vector3< float  >( -1, -2, -3 ) );
	assert( -vD2 == Vector3< double >( -1, -2, -3 ) );

	assert( Abs( vS2 ) == Vector3< float  >( 1, 2, 3 ) );
	assert( Abs( vD2 ) == Vector3< double >( 1, 2, 3 ) );

	assert( Rec( vS1 ) == Vector3< float  >( 1, 0.5, 1.0/3.0 ) );
	assert( Rec( vD1 ) == Vector3< double >( 1, 0.5, 1.0/3.0 ) );

	assert( Exp( vS1 ) == Vector3< float  >( exp( 1 ), exp( 2 ), exp( 3 ) ) );
	assert( Exp( vD1 ) == Vector3< double >( exp( 1 ), exp( 2 ), exp( 3 ) ) );

	assert( Log( vS1 ) == Vector3< float  >( log( 1 )	, log( 2 )	, log( 3 ) ) );
	assert( Log( vD1 ) == Vector3< double >( log( 1.0 )	, log( 2.0 ), 1.0986122886681096 ) );

	assert( Dot( vS2, vS3 ) == 14 );
	assert( Dot( vD2, vD3 ) == 14 );

	assert( DistanceQ( vS2, vS3 ) == 0 );
	assert( DistanceQ( vD2, vD3 ) == 0 );
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


template	< typename F >	Matrix< F >
MM22( F p00, F p01, F p10, F p11 ) {
	Matrix< F >	v( 2, 2, { p00, p01, p10, p11 } );
	return v;
}
template	< typename F >	Matrix< F >
MM23( F p00, F p01, F p02, F p10, F p11, F p12 ) {
	Matrix< F >	v( 2, 3, { p00, p01, p02, p10, p11, p12 } );
	return v;
}

void
TestMatrix() {
//	print( Matrix<Float>( nR: 1, nC: 1, u: [ 3 ] ).u[ 0 ] )

	double	w[ 6 ] = { 1, 2, 3, 4, 5, 6 };
	assert( vMatrix<double>( w, 2, 3 ).m[ 4 ] == 5 );
	assert( Matrix<double>( 2, 3, { 5, 5, 5, 5, 5, 5 } ) ( 1, 1 ) == 5 );
	
	assert( T( vMatrix<double>( w, 3, 2 ) ) == MM23<double>( 1, 3, 5, 2, 4, 6 ) );

	assert( vMatrix<double>( w, 2, 3 ).Row( 1 ) == vVector<double>( w + 3, 3 ) );
	assert( vMatrix<double>( w, 2, 3 ).Col( 1 ) == vVector<double>( w + 1, 2, 3 ) );
}

void
JPMatrixTestF() {

	float	w[ 6 ] = { 1, 2, 3, 4, 5, 6 };
	auto	wM = vMatrix<float>( w, 2, 3 );
	assert( wM +  (float)2 == MM23<float>(   3  ,  4  ,  5  ,  6  ,  7  ,  8 ) );
	assert( wM -  (float)2 == MM23<float>(  -1  ,  0  ,  1  ,  2  ,  3  ,  4 ) );
	assert( wM *  (float)2 == MM23<float>(   2  ,  4  ,  6  ,  8  , 10  , 12 ) );
#ifdef	USE_CPU
	assert( wM /  (float)2 == MM23<float>(   0.5, 1, 1.5, 2, 2.5, 3 ) );
	assert(  (float)2 / wM == MM23<float>(   2  ,  1  ,  2.0/3.0,  0.5, 0.4, 1.0/3.0 ) );
	assert( wM / wM == MM23<float>(   1, 1, 1, 1, 1, 1 ) );
#else
	assert( wM /  (float)2 == MM23<float>(   0.49999997,  0.99999994  ,  1.49999988,  1.99999988  ,  2.49999976,  2.99999976 ) );
	assert(  (float)2 / wM == MM23<float>(   1.9999999  ,  0.99999994  ,  0.6666666,  0.49999997, 0.39999998, 0.3333333 ) );
	assert( wM / wM == MM23<float>(   0.99999994  ,  0.99999994  ,  0.99999994  ,  0.99999994  ,  0.99999994  ,  0.99999994 ) );
#endif
	assert( (float)2 + wM == MM23<float>(   3  ,  4  ,  5  ,  6  ,  7  ,  8 ) );
	assert( (float)2 - wM == MM23<float>(   1  ,  0  , -1  , -2  , -3  , -4 ) );
	assert( (float)2 * wM == MM23<float>(   2  ,  4  ,  6  ,  8  , 10  , 12 ) );
	assert( wM + wM == MM23<float>(   2  ,  4  ,  6  ,  8  , 10  , 12 ) );
	assert( wM - wM == MM23<float>(   0  ,  0  ,  0  ,  0  ,  0  ,  0 ) );
	assert( wM * wM == MM23<float>(   1  ,  4  ,  9  , 16  , 25  , 36 ) );
	
	assert( Dot( wM, vMatrix<float>( w, 3, 2 ) ) == MM22<float>( 22, 28, 49, 64 ) );
//	Vector< float > wV = V< float, Sum >( wM );
//	assert( V<float, []( const vVector& p ) { Exp( p ); }>( wM ) == MM23<float>( 1, 2, 3, 2, 2.5, 3 ) );
//	assert( HDiv( wM, vVector<float>( w, 3 ) ) == MM23<float>( 0.99999994, 0.99999994, 0.99999994, 3.9999998, 2.4999998, 1.9999999 ) );
	assert( H( wM, Sum ) == Vector2< float >( 6, 15 ) );
	assert( V( wM, Sum ) == Vector3< float >( 5, 7, 9 ) );
	assert( Spread( vVector< float >( w, 2 ), vVector<float>( w, 2 ) ) == MM22<float>( 1, 2, 2, 4 ) );
}

void
JPMatrixTestD() {
	double	w[ 6 ] = { 1, 2, 3, 4, 5, 6 };
	auto	wM = vMatrix<double>( w, 2, 3 );
	assert( wM +  (double)2 == MM23<double>(   3  ,  4  ,  5  ,  6  ,  7  ,  8 ) );
	assert( wM -  (double)2 == MM23<double>(  -1  ,  0  ,  1  ,  2  ,  3  ,  4 ) );
	assert( wM *  (double)2 == MM23<double>(   2  ,  4  ,  6  ,  8  , 10  , 12 ) );
	assert( wM /  (double)2 == MM23<double>(   0.5, 1, 1.5, 2, 2.5, 3 ) );
	assert(  (double)2 + wM == MM23<double>(   3  ,  4  ,  5  ,  6  ,  7  ,  8 ) );
	assert(  (double)2 - wM == MM23<double>(   1  ,  0  , -1  , -2  , -3  , -4 ) );
	assert(  (double)2 * wM == MM23<double>(   2  ,  4  ,  6  ,  8  , 10  , 12 ) );
	assert(  (double)2 / wM == MM23<double>(   2  ,  1  ,  2.0/3.0,  0.5, 0.4, 1.0/3.0 ) );
	assert( wM + wM == MM23<double>(   2  ,  4  ,  6  ,  8  , 10  , 12 ) );
	assert( wM - wM == MM23<double>(   0  ,  0  ,  0  ,  0  ,  0  ,  0 ) );
	assert( wM * wM == MM23<double>(   1  ,  4  ,  9  , 16  , 25  , 36 ) );
	assert( wM / wM == MM23<double>(   1, 1, 1, 1, 1, 1 ) );
	
	assert( Dot( wM, vMatrix<double>( w, 3, 2 ) ) == MM22<double>( 22, 28, 49, 64 ) );
	assert( H( wM, Sum ) == Vector2< double >( 6, 15 ) );
	assert( V( wM, Sum ) == Vector3< double >( 5, 7, 9 ) );
	assert( Spread( vVector< double >( w, 2 ), vVector<double>( w, 2 ) ) == MM22<double>( 1, 2, 2, 4 ) );
}

void
Main() {
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
	cerr << "TestMatrix start" <<  endl;
	TestMatrix();
	cerr << "JPMatrixTestF start" <<  endl;
	JPMatrixTestF();
	cerr << "JPMatrixTestD start" <<  endl;
	JPMatrixTestD();

	cerr << "over" <<  endl;
}

int
main(int argc, const char * argv[]) {
#ifdef	JP_USE_CPU
	cerr << "CPU" << endl;
#else
	cerr << "Accelerate" << endl;
#endif
	Main();
	return 0;
}

//	1 2
//	3 4
//   5 6
//	1 3 5
//	2 4 6
