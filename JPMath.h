#pragma once

#include	"JP.h"
#include	"math.h"
#include	<Accelerate/Accelerate.h>
#include	<vector>


struct
JPMath {

	static	double
	Gaussian() {
		return
			sqrt( -2 * log( JP::Random() ) )
		*	sin( 2 * M_PI * JP::Random() )
		;
	}

	template < typename T >	T
	Square( const T& p ) {
		return p * p;
	}

	template < typename T >	T
	Cube( const T& p ) {
		return p * p * p;
	}

	static	float
	Sum( const std::vector< float >& p ) {
		float	v = 0;
		vDSP_sve( &p[ 0 ], 1, &v, p.size() );
		return v;
	}
	static	double
	Sum( const std::vector< double >& p ) {
		double	v = 0;
		vDSP_sveD( &p[ 0 ], 1, &v, p.size() );
		return v;
	}

	static	float
	Mean( const std::vector< float >& p ) {
		float	v = 0;
		vDSP_meanv( &p[ 0 ], 1, &v, p.size() );
		return v;
	}
	static	double
	Mean( const std::vector< double >& p ) {
		double	v = 0;
		vDSP_meanvD( &p[ 0 ], 1, &v, p.size() );
		return v;
	}

	static	float
	L1Norm( const std::vector< float >& p ) {
		float	v = 0;
		vDSP_svemg( &p[ 0 ], 1, &v, p.size() );
		return v;
	}
	static	double
	L1Norm( const std::vector< double >& p ) {
		double	v = 0;
		vDSP_svemgD( &p[ 0 ], 1, &v, p.size() );
		return v;
	}

	static	float
	L2NormSquare( const std::vector< float >& p ) {
		float	v = 0;
		vDSP_svesq( &p[ 0 ], 1, &v, p.size() );
		return v;
	}
	static	double
	L2NormSquare( const std::vector< double >& p ) {
		double	v = 0;
		vDSP_svesqD( &p[ 0 ], 1, &v, p.size() );
		return v;
	}

	static	float
	L2Norm( const std::vector< float >& p ) {
		return sqrt( L2NormSquare( p ) );
	}
	static	double
	L2Norm( const std::vector< double >& p ) {
		return sqrt( L2NormSquare( p ) );
	}

	static	std::vector< float >
	Neg( const std::vector< float >& p ) {
		std::vector< float > v( p.size(), 0 );
		vDSP_vneg( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Neg( const std::vector< double >& p ) {
		std::vector< double > v( p.size(), 0 );
		vDSP_vnegD( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Abs( const std::vector< float >& p ) {
		std::vector< float > v( p.size(), 0 );
		vDSP_vabs( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Abs( const std::vector< double >& p ) {
		std::vector< double > v( p.size(), 0 );
		vDSP_vabsD( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Normalize( const std::vector< float >& p ) {
		std::vector< float > v( p.size(), 0 );
		float	wM = 0;
		float	wSD = 0;
		vDSP_normalize( &p[ 0 ], 1, &v[ 0 ], 1, &wM, &wSD, p.size() );
		return v;
	}
	static	std::vector< double >
	Normalize( const std::vector< double >& p ) {
		std::vector< double > v( p.size(), 0 );
		double	wM = 0;
		double	wSD = 0;
		vDSP_normalizeD( &p[ 0 ], 1, &v[ 0 ], 1, &wM, &wSD, p.size() );
		return v;
	}

	static	std::vector< float >
	Add( const std::vector< float >& l, const std::vector< float >& r ) {
		assert( l.size() == r.size() );
		std::vector< float > v( l.size(), 0 );
		vDSP_vadd( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}
	static	std::vector< double >
	Add( const std::vector< double >& l, const std::vector< double >& r ) {
		assert( l.size() == r.size() );
		std::vector< double > v( l.size(), 0 );
		vDSP_vaddD( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}

	static	std::vector< float >
	Add( const std::vector< float >& p, float s ) {
		std::vector< float > v( p.size(), s );
		vDSP_vadd( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Add( const std::vector< double >& p, double s ) {
		std::vector< double > v( p.size(), s );
		vDSP_vaddD( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Add( float s, const std::vector< float >& p ) {
		std::vector< float > v( p.size(), s );
		vDSP_vadd( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Add( double s, const std::vector< double >& p ) {
		std::vector< double > v( p.size(), s );
		vDSP_vaddD( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Sub( const std::vector< float >& l, const std::vector< float >& r ) {
		assert( l.size() == r.size() );
		std::vector< float > v( l.size(), 0 );
		vDSP_vsub( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}
	static	std::vector< double >
	Sub( const std::vector< double >& l, const std::vector< double >& r ) {
		assert( l.size() == r.size() );
		std::vector< double > v( l.size(), 0 );
		vDSP_vsubD( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}

	static	std::vector< float >
	Sub( const std::vector< float >& p, float s ) {
		std::vector< float > v( p.size(), s );
		vDSP_vsub( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Sub( const std::vector< double >& p, double s ) {
		std::vector< double > v( p.size(), s );
		vDSP_vsubD( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Sub( float s, const std::vector< float >& p ) {
		std::vector< float > v( p.size(), s );
		vDSP_vsub( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Sub( double s, const std::vector< double >& p ) {
		std::vector< double > v( p.size(), s );
		vDSP_vsubD( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Mul( const std::vector< float >& l, const std::vector< float >& r ) {
		assert( l.size() == r.size() );
		std::vector< float > v( l.size(), 0 );
		vDSP_vmul( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}
	static	std::vector< double >
	Mul( const std::vector< double >& l, const std::vector< double >& r ) {
		assert( l.size() == r.size() );
		std::vector< double > v( l.size(), 0 );
		vDSP_vmulD( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}

	static	std::vector< float >
	Mul( const std::vector< float >& p, float s ) {
		std::vector< float > v( p.size(), s );
		vDSP_vmul( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Mul( const std::vector< double >& p, double s ) {
		std::vector< double > v( p.size(), s );
		vDSP_vmulD( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Mul( float s, const std::vector< float >& p ) {
		std::vector< float > v( p.size(), s );
		vDSP_vmul( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Mul( double s, const std::vector< double >& p ) {
		std::vector< double > v( p.size(), s );
		vDSP_vmulD( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Div( const std::vector< float >& l, const std::vector< float >& r ) {
		assert( l.size() == r.size() );
		std::vector< float > v( l.size(), 0 );
		vDSP_vdiv( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}
	static	std::vector< double >
	Div( const std::vector< double >& l, const std::vector< double >& r ) {
		assert( l.size() == r.size() );
		std::vector< double > v( l.size(), 0 );
		vDSP_vdivD( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, l.size() );
		return v;
	}

	static	std::vector< float >
	Div( const std::vector< float >& p, float s ) {
		std::vector< float > v( p.size(), s );
		vDSP_vdiv( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Div( const std::vector< double >& p, double s ) {
		std::vector< double > v( p.size(), s );
		vDSP_vdivD( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	static	std::vector< float >
	Div( float s, const std::vector< float >& p ) {
		std::vector< float > v( p.size(), s );
		vDSP_vdiv( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}
	static	std::vector< double >
	Div( double s, const std::vector< double >& p ) {
		std::vector< double > v( p.size(), s );
		vDSP_vdivD( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, p.size() );
		return v;
	}

	float
	DotProduct( const std::vector< float >& l, const std::vector< float >& r ){
		assert( l.size() == r.size() );
		float	v = 0;
		vDSP_dotpr( &l[ 0 ], 1, &r[ 0 ], 1, &v, l.size() );
		return v;
	}
	double
	DotProduct( const std::vector< double >& l, const std::vector< double >& r ) {
		assert( l.size() == r.size() );
		double	v = 0;
		vDSP_dotprD( &l[ 0 ], 1, &r[ 0 ], 1, &v, l.size() );
		return v;
	}

	static	std::vector< float >
	Ramp( vDSP_Length p, float pInit = 0, float pStep = 1 ) {
		std::vector< float > v( p );
		vDSP_vramp( &pInit, &pStep, &v[ 0 ], 1, p );
		return v;
	}
	static	std::vector< double >
	Ramp( vDSP_Length p, double pInit = 0, double pStep = 1 ) {
		std::vector< double > v( p );
		vDSP_vrampD( &pInit, &pStep, &v[ 0 ], 1, p );
		return v;
	}
	
	static	std::vector< float >
	UnitVector( const std::vector< float >& p ) {
		return Div( p, L2Norm( p ) );
	}
	static	std::vector< double >
	UnitVector( const std::vector< double >& p ) {
		return Div( p, L2Norm( p ) );
	}
};
