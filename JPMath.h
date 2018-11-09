//	Written by Satoru Ogura, Tokyo.
//
#pragma once

#include	"JP.h"
#include	"math.h"
#include	<Accelerate/Accelerate.h>
#include	<vector>

//	Scalar -> Scalar

template < typename T >	T
Square( const T& p ) {
	return p * p;
}

template < typename T >	T
Cube( const T& p ) {
	return p * p * p;
}

//	() -> Scalar

template < typename T >	T
Gaussian() {
	return
		sqrt( -2 * log( JP::UniformRandomFloat< T >() ) )
	*	sin( 2 * M_PI * JP::UniformRandomFloat< T >() )
	;
}

//	() -> Vector

inline	void
Ramp( float pInit, float pStep, std::vector< float >& p, size_t pLength ) {
	vDSP_vramp( &pInit, &pStep, &p[ 0 ], 1, vDSP_Length( pLength ) );
}
inline	void
Ramp( double pInit, double pStep, std::vector< double >& p, size_t pLength ) {
	vDSP_vrampD( &pInit, &pStep, &p[ 0 ], 1, vDSP_Length( pLength ) );
}
template	< typename T >	std::vector< T >
Ramp( size_t p, T pInit = 0, T pStep = 1 ) {
	std::vector< T > v( p );
	Ramp( pInit, pStep, v, p );
	return v;
}

//	Vector -> Scalar

inline	void
Sum( const std::vector< float >& p, float& v ) {
	vDSP_sve( &p[ 0 ], 1, &v, p.size() );
}
inline	void
Sum( const std::vector< double >& p, double& v ) {
	vDSP_sveD( &p[ 0 ], 1, &v, p.size() );
}
template	< typename T >	T
Sum( const std::vector< T >& p ) {
	T	v = 0;
	Sum( p, v );
	return v;
}

inline	void
Mean( const std::vector< float >& p, float& v ) {
	vDSP_meanv( &p[ 0 ], 1, &v, p.size() );
}
inline	void
Mean( const std::vector< double >& p, double& v ) {
	vDSP_meanvD( &p[ 0 ], 1, &v, p.size() );
}
template	< typename T >	T
Mean( const std::vector< T >& p ) {
	T	v = 0;
	Mean( p, v );
	return v;
}

inline	void
L1Norm( const std::vector< float >& p, float& v ) {
	vDSP_svemg( &p[ 0 ], 1, &v, p.size() );
}
inline	void
L1Norm( const std::vector< double >& p, double& v ) {
	vDSP_svemgD( &p[ 0 ], 1, &v, p.size() );
}
template	< typename T >	T
L1Norm( const std::vector< T >& p ) {
	T	v = 0;
	L1Norm( p, v );
	return v;
}

inline	void
L2NormSquare( const std::vector< float >& p, float& v ) {
	vDSP_svesq( &p[ 0 ], 1, &v, p.size() );
}
inline	void
L2NormSquare( const std::vector< double >& p, double& v ) {
	vDSP_svesqD( &p[ 0 ], 1, &v, p.size() );
}
template	< typename T >	T
L2NormSquare( const std::vector< T >& p ) {
	T	v = 0;
	L2NormSquare( p, v );
	return v;
}
template	< typename T >	T
L2Norm( const std::vector< T >& p ) {
	return sqrt( L2NormSquare( p ) );
}


//	( Vector, Vector ) -> Vector
//	( Scalar, Vector ) -> Vector
//	( Vector, Scalar ) -> Vector

inline	void
Add( const std::vector< float >& l, const std::vector< float >& r, std::vector< float >& v ) {
	vDSP_vadd( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
inline	void
Add( const std::vector< double >& l, const std::vector< double >& r, std::vector< double >& v ) {
	vDSP_vaddD( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
template	< typename T >	std::vector< T >
operator +( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Add( l, r, v );
	return v;
}
template	< typename T >	std::vector< T >
operator +( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Add( p, v, v );
	return v;
}
template	< typename T >	std::vector< T >
operator +( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Add( v, p, v );
	return v;
}

inline	void
Sub( const std::vector< float >& l, const std::vector< float >& r, std::vector< float >& v ) {
	vDSP_vsub( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
inline	void
Sub( const std::vector< double >& l, const std::vector< double >& r, std::vector< double >& v ) {
	vDSP_vsubD( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
template	< typename T >	std::vector< T >
operator -( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Sub( l, r, v );
	return v;
}
template	< typename T >	std::vector< T >
operator -( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Sub( p, v, v );
	return v;
}
template	< typename T >	std::vector< T >
operator -( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Sub( v, p, v );
	return v;
}

//	Hadamard
inline	void
Mul( const std::vector< float >& l, const std::vector< float >& r, std::vector< float >& v ) {
	vDSP_vmul( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
inline	void
Mul( const std::vector< double >& l, const std::vector< double >& r, std::vector< double >& v ) {
	vDSP_vmulD( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
template	< typename T >	std::vector< T >
operator *( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Mul( l, r, v );
	return v;
}
template	< typename T >	std::vector< T >
operator *( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Mul( p, v, v );
	return v;
}
template	< typename T >	std::vector< T >
operator *( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Mul( v, p, v );
	return v;
}

inline	void
Div( const std::vector< float >& l, const std::vector< float >& r, std::vector< float >& v ) {
	vDSP_vdiv( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
inline	void
Div( const std::vector< double >& l, const std::vector< double >& r, std::vector< double >& v ) {
	vDSP_vdivD( &r[ 0 ], 1, &l[ 0 ], 1, &v[ 0 ], 1, v.size() );
}
template	< typename T >	std::vector< T >
operator /( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Div( l, r, v );
	return v;
}
template	< typename T >	std::vector< T >
operator /( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Div( p, v, v );
	return v;
}
template	< typename T >	std::vector< T >
operator /( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Div( v, p, v );
	return v;
}

//	Vector -> Vector

inline	void
Neg( const std::vector< float >& p, std::vector< float >& v ) {
	vDSP_vneg( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
}
inline	void
Neg( const std::vector< double >& p, std::vector< double >& v ) {
	vDSP_vnegD( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
}
template	< typename T >	std::vector< T >
operator -( const std::vector< T >& p ) {
	std::vector< T > v( p.size(), 0 );
	Neg( p, v );
	return v;
}

inline	void
Abs( const std::vector< float >& p, std::vector< float >& v ) {
	vDSP_vabs( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
}
inline	void
Abs( const std::vector< double >& p, std::vector< double >& v ) {
	vDSP_vabsD( &p[ 0 ], 1, &v[ 0 ], 1, p.size() );
}
template	< typename T >	std::vector< T >
Abs( const std::vector< T >& p ) {
	std::vector< T > v( p.size(), 0 );
	Abs( p, v );
	return v;
}

template	< typename T >	std::vector< T >
UnitVector( const std::vector< T >& p ) {
	return p / L2Norm( p );
}

//	( Vector, Vector ) -> Scalar

inline	void
Dot( const std::vector< float >& l, const std::vector< float >& r, float& v ) {
	vDSP_dotpr( &r[ 0 ], 1, &l[ 0 ], 1, &v, l.size() );
}
inline	void
Dot( const std::vector< double >& l, const std::vector< double >& r, double& v ) {
	vDSP_dotprD( &r[ 0 ], 1, &l[ 0 ], 1, &v, l.size() );
}
template	< typename T >	T
Dot( const std::vector< T >& l, const std::vector< T >& r ){
	assert( l.size() == r.size() );
	T	v = 0;
	Dot( l, r, v );
	return v;
}
