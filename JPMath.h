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

template < typename T >	T
Sigmoid( const T& p ) {
    return 1 / ( 1 + exp( -p ) );
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
Ramp( float pInit, float pStep, float* p, size_t pLength ) {
	vDSP_vramp( &pInit, &pStep, p, 1, pLength );
}
inline	void
Ramp( double pInit, double pStep, double* p, size_t pLength ) {
	vDSP_vrampD( &pInit, &pStep, p, 1, pLength );
}
template	< typename T >	std::vector< T >
Ramp( size_t p, T pInit = 0, T pStep = 1 ) {
	std::vector< T > v( p );
	Ramp( pInit, pStep, &v[ 0 ], p );
	return v;
}

//	Vector -> Scalar

inline	void
Sum( const float* p, float& v, size_t pLength ) {
	vDSP_sve( p, 1, &v, pLength );
}
inline	void
Sum( const double* p, double& v, size_t pLength ) {
	vDSP_sveD( p, 1, &v, pLength );
}
template	< typename T >	T
Sum( const std::vector< T >& p ) {
	T	v = 0;
	Sum( &p[ 0 ], v, p.size() );
	return v;
}

inline	void
Mean( const float* p, float& v, size_t pLength ) {
	vDSP_meanv( p, 1, &v, pLength );
}
inline	void
Mean( const double* p, double& v, size_t pLength ) {
	vDSP_meanvD( p, 1, &v, pLength );
}
template	< typename T >	T
Mean( const std::vector< T >& p ) {
	T	v = 0;
	Mean( &p[ 0 ], v, p.size() );
	return v;
}

inline	void
L1Norm( const float* p, float& v, size_t pLength ) {
	vDSP_svemg( p, 1, &v, pLength );
}
inline	void
L1Norm( const double* p, double& v, size_t pLength ) {
	vDSP_svemgD( p, 1, &v, pLength );
}
template	< typename T >	T
L1Norm( const std::vector< T >& p ) {
	T	v = 0;
	L1Norm( &p[ 0 ], v, p.size() );
	return v;
}

inline	void
L2NormSquare( const float* p, float& v, size_t pLength ) {
	vDSP_svesq( p, 1, &v, pLength );
}
inline	void
L2NormSquare( const double* p, double& v, size_t pLength ) {
	vDSP_svesqD( p, 1, &v, pLength );
}
template	< typename T >	T
L2NormSquare( const std::vector< T >& p ) {
	T	v = 0;
	L2NormSquare( &p[ 0 ], v, p.size() );
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
Add( const float* l, const float* r, float* v, size_t pLength ) {
	vDSP_vadd( l, 1, r, 1, v, 1, pLength );
}
inline	void
Add( const double* l, const double* r, double* v, size_t pLength ) {
	vDSP_vaddD( l, 1, r, 1, v, 1, pLength );
}
template	< typename T >	std::vector< T >
operator +( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Add( &l[ 0 ], &r[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator +( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Add( &p[ 0 ], &v[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator +( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Add( &v[ 0 ], &p[ 0 ], &v[ 0 ], v.size() );
	return v;
}

inline	void
Sub( const float* l, const float* r, float* v, size_t pLength ) {
	vDSP_vsub( r, 1, l, 1, v, 1, pLength );
}
inline	void
Sub( const double* l, const double* r, double* v, size_t pLength ) {
	vDSP_vsubD( r, 1, l, 1, v, 1, pLength );
}
template	< typename T >	std::vector< T >
operator -( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Sub( &l[ 0 ], &r[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator -( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Sub( &p[ 0 ], &v[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator -( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Sub( &v[ 0 ], &p[ 0 ], &v[ 0 ], v.size() );
	return v;
}

//	Hadamard
inline	void
Mul( const float* l, const float* r, float* v, size_t pLength ) {
	vDSP_vmul( l, 1, r, 1, v, 1, pLength );
}
inline	void
Mul( const double* l, const double* r, double* v, size_t pLength ) {
	vDSP_vmulD( l, 1, r, 1, v, 1, pLength );
}
template	< typename T >	std::vector< T >
operator *( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Mul( &l[ 0 ], &r[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator *( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Mul( &p[ 0 ], &v[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator *( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Mul( &v[ 0 ], &p[ 0 ], &v[ 0 ], v.size() );
	return v;
}

inline	void
Div( const float* l, const float* r, float* v, size_t pLength ) {
	vDSP_vdiv( r, 1, l, 1, v, 1, pLength );
}
inline	void
Div( const double* l, const double* r, double* v, size_t pLength ) {
	vDSP_vdivD( r, 1, l, 1, v, 1, pLength );
}
template	< typename T >	std::vector< T >
operator /( const std::vector< T >& l, const std::vector< T >& r ) {
	assert( l.size() == r.size() );
	std::vector< T > v( l.size(), 0 );
	Div( &l[ 0 ], &r[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator /( const std::vector< T >& p, T s ) {
	std::vector< T > v( p.size(), s );
	Div( &p[ 0 ], &v[ 0 ], &v[ 0 ], v.size() );
	return v;
}
template	< typename T >	std::vector< T >
operator /( T s, const std::vector< T >& p ) {
	std::vector< T > v( p.size(), s );
	Div( &v[ 0 ], &p[ 0 ], &v[ 0 ], v.size() );
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
