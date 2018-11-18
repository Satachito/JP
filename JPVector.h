//	Written by Satoru Ogura, Fokyo.
//
#pragma once

#include	"JP.h"
#include	<cmath>
#include	<Accelerate/Accelerate.h>
#include	<vector>

namespace JP {
	template	< typename F >	struct
	iVector {
		F*		m;
		size_t	n;
		iVector( F* p, size_t n ) : m( p ), n( n ) {}
		virtual F
		operator []( size_t p ) const {
			return m[ p ];
		}
	};
	template	< typename F >	struct
	Vector 	: iVector< F > {
		~
		Vector() {
			delete[] iVector< F >::m;
		}
		Vector( size_t n )
		:	iVector< F >( new F[ n ](), n ) {
		}
		Vector( size_t n, F p )
		:	iVector< F >( new F[ n ], n ) {
			for ( auto i = 0; i < n; i++ ) iVector< F >::m[ i ] = p;
		}
		Vector( size_t n, F( *p )() )
		:	iVector< F >( new F[ n ], n ) {
			for ( auto i = 0; i < n; i++ ) iVector< F >::m[ i ] = p();
		}
		Vector( const iVector< F >& p )
		:	iVector< F >( new F[ p.n ], p.n ) {
			for ( auto i = 0; i < p.n; i++ ) iVector< F >::m[ i ] = p.m[ i ];
		}
		const Vector< F >&
		operator =( const Vector< F >& p ) {
			delete[] iVector< F >::m;
			iVector< F >::m = new F[ p.n ];
			iVector< F >::n = p.n;
			for ( auto i = 0; i < p.n; i++ ) iVector< F >::m[ i ] = p.m[ i ];
			return p;
		}
		F&
		operator []( size_t p ) {
			return iVector< F >::m[ p ];
		}
	};

	namespace Accelerate {

		//	() -> Vector

		inline	void
		Ramp( float pInit, float pStep, float* p, vDSP_Stride sP, vDSP_Length pLength ) {
			vDSP_vramp( &pInit, &pStep, p, sP, pLength );
		}
		inline	void
		Ramp( double pInit, double pStep, double* p, vDSP_Stride sP, vDSP_Length pLength ) {
			vDSP_vrampD( &pInit, &pStep, p, sP, pLength );
		}
		template	< typename F >	std::vector< F >
		Rampv( size_t p, F pInit = 0, F pStep = 1 ) {
			std::vector< F > v( p );
			Ramp( pInit, pStep, &v[ 0 ], 1, p );
			return v;
		}
		template	< typename F >	Vector< F >
		Ramp( size_t p, F pInit = 0, F pStep = 1 ) {
			Vector< F > v( p );
			Ramp( pInit, pStep, v.m, 1, p );
			return v;
		}

		//	Vector -> Scalar

		inline	void
		Sum( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength ) {
			vDSP_sve( p, sP, &v, pLength );
		}
		inline	void
		Sum( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength ) {
			vDSP_sveD( p, sP, &v, pLength );
		}
		template	< typename F >	F
		Sum( const std::vector< F >& p ) {
			F	v = 0;
			Sum( &p[ 0 ], 1, v, p.size() );
			return v;
		}
		template	< typename F >	F
		Sum( const iVector< F >& p ) {
			F	v = 0;
			Sum( p.m, 1, v, p.n );
			return v;
		}


		inline	void
		Mean( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength ) {
			vDSP_meanv( p, sP, &v, pLength );
		}
		inline	void
		Mean( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength ) {
			vDSP_meanvD( p, sP, &v, pLength );
		}
		template	< typename F >	F
		Mean( const std::vector< F >& p ) {
			F	v = 0;
			Mean( &p[ 0 ], 1, v, p.size() );
			return v;
		}
		template	< typename F >	F
		Mean( const iVector< F >& p ) {
			F	v = 0;
			Mean( p.m, 1, v, p.n );
			return v;
		}

		inline	void
		L1Norm( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength ) {
			vDSP_svemg( p, sP, &v, pLength );
		}
		inline	void
		L1Norm( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength ) {
			vDSP_svemgD( p, sP, &v, pLength );
		}
		template	< typename F >	F
		L1Norm( const std::vector< F >& p ) {
			F	v = 0;
			L1Norm( &p[ 0 ], 1, v, p.size() );
			return v;
		}
		template	< typename F >	F
		L1Norm( const iVector< F >& p ) {
			F	v = 0;
			L1Norm( p.m, 1, v, p.n );
			return v;
		}

		inline	void
		L2NormSquare( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength ) {
			vDSP_svesq( p, sP, &v, pLength );
		}
		inline	void
		L2NormSquare( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength ) {
			vDSP_svesqD( p, sP, &v, pLength );
		}
		template	< typename F >	F
		L2NormSquare( const std::vector< F >& p ) {
			F	v = 0;
			L2NormSquare( &p[ 0 ], 1, v, p.size() );
			return v;
		}
		template	< typename F >	F
		L2NormSquare( const iVector< F >& p ) {
			F	v = 0;
			L2NormSquare( p.m, 1, v, p.n );
			return v;
		}

		template	< typename F >	F
		L2Norm( const std::vector< F >& p ) {
			return sqrt( L2NormSquare( p ) );
		}
		template	< typename F >	F
		L2Norm( const iVector< F >& p ) {
			return sqrt( L2NormSquare( p ) );
		}
		//	( Vector, Vector ) -> Vector
		//	( Scalar, Vector ) -> Vector
		//	( Vector, Scalar ) -> Vector

		inline	void
		Add( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vadd( r, sR, l, sL, v, sV, pLength );
		}
		inline	void
		Add( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vaddD( r, sR, l, sL, v, sV, pLength );
		}
		template	< typename F >	std::vector< F >
		operator +( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Add( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator +( const std::vector< F >& p, F s ) {
			std::vector< F > v( p.size(), s );
			Add( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator +( F s, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), s );
			Add( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	Vector< F >
		operator +( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Add( l.m, 1, r.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator +( const iVector< F >& p, F s ) {
			Vector< F > v( p.n, s );
			Add( p.m, 1, v.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator +( F s, const iVector< F >& p ) {
			Vector< F > v( p.n, s );
			Add( v.m, 1, p.m, 1, v.m, 1, v.n );
			return v;
		}

		inline	void
		Sub( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vsub( r, sR, l, sL, v, sV, pLength );
		}
		inline	void
		Sub( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vsubD( r, sR, l, sL, v, sV, pLength );
		}
		template	< typename F >	std::vector< F >
		operator -( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Sub( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator -( const std::vector< F >& p, F s ) {
			std::vector< F > v( p.size(), s );
			Sub( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator -( F s, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), s );
			Sub( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	Vector< F >
		operator -( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Sub( l.m, 1, r.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator -( const iVector< F >& p, F s ) {
			Vector< F > v( p.n, s );
			Sub( p.m, 1, v.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator -( F s, const iVector< F >& p ) {
			Vector< F > v( p.n, s );
			Sub( v.m, 1, p.m, 1, v.m, 1, v.n );
			return v;
		}

		//	Hadamard
		inline	void
		Mul( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vmul( r, sR, l, sL, v, sV, pLength );
		}
		inline	void
		Mul( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vmulD( r, sR, l, sL, v, sV, pLength );
		}
		template	< typename F >	std::vector< F >
		operator *( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Mul( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator *( const std::vector< F >& p, F s ) {
			std::vector< F > v( p.size(), s );
			Mul( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator *( F s, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), s );
			Mul( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}

		template	< typename F >	Vector< F >
		operator *( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Mul( l.m, 1, r.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator *( const iVector< F >& p, F s ) {
			Vector< F > v( p.n, s );
			Mul( p.m, 1, v.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator *( F s, const iVector< F >& p ) {
			Vector< F > v( p.n, s );
			Mul( v.m, 1, p.m, 1, v.m, 1, v.n );
			return v;
		}

		inline	void
		Div( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vdiv( r, sR, l, sL, v, sV, pLength );
		}
		inline	void
		Div( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vdivD( r, sR, l, sL, v, sV, pLength );
		}
		template	< typename F >	std::vector< F >
		operator /( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Div( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator /( const std::vector< F >& p, F s ) {
			std::vector< F > v( p.size(), s );
			Div( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >
		operator /( F s, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), s );
			Div( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}

		template	< typename F >	Vector< F >
		operator /( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Div( l.m, 1, r.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator /( const iVector< F >& p, F s ) {
			Vector< F > v( p.n, s );
			Div( p.m, 1, v.m, 1, v.m, 1, v.n );
			return v;
		}
		template	< typename F >	Vector< F >
		operator /( F s, const iVector< F >& p ) {
			Vector< F > v( p.n, s );
			Div( v.m, 1, p.m, 1, v.m, 1, v.n );
			return v;
		}

		//	Vector -> Vector

		template	< typename F >	std::vector< F >
		UnitVector( const std::vector< F >& p ) {
			return p / L2Norm( p );
		}
		template	< typename F >	Vector< F >
		UnitVector( const iVector< F >& p ) {
			return p / L2Norm( p );
		}

		inline	void
		Neg( const float* p, float* v, vDSP_Length pLength ) {
			vDSP_vneg( p, 1, v, 1, pLength );
		}
		inline	void
		Neg( const double* p, double* v, vDSP_Length pLength ) {
			vDSP_vnegD( p, 1, v, 1, pLength );
		}
		template	< typename F >	std::vector< F >
		operator -( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Neg( &p[ 0 ], &v[ 0 ], v.size() );
			return v;
		}
		template	< typename F >	Vector< F >
		operator -( const iVector< F >& p ) {
			Vector< F > v( p );
			Neg( p.m, v.m, v.n );
			return v;
		}

		inline	void
		Abs( const float* p, float* v, vDSP_Length pLength ) {
			vDSP_vabs( p, 1, v, 1, pLength );
		}
		inline	void
		Abs( const double* p, double* v, vDSP_Length pLength ) {
			vDSP_vabsD( p, 1, v, 1, pLength );
		}
		template	< typename F >	std::vector< F >
		Abs( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Abs( &p[ 0 ], &v[ 0 ], v.size() );
			return v;
		}
		template	< typename F >	Vector< F >
		Abs( const iVector< F >& p ) {
			Vector< F > v( p );
			Abs( p.m, v.m, v.n );
			return v;
		}

		inline	void
		Rec( const float* p, float* v, int pLength ) {
			vvrecf( v, p, &pLength );
		}
		inline	void
		Rec( const double* p, double* v, int pLength ) {
			vvrec( v, p, &pLength );
		}
		template	< typename F >	std::vector< F >
		Rec( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Rec( &p[ 0 ], &v[ 0 ], (int)v.size() );
			return v;
		}
		template	< typename F >	Vector< F >
		Rec( const iVector< F >& p ) {
			Vector< F > v( p );
			Rec( p.m, v.m, (int)v.n );
			return v;
		}

		inline	void
		Exp( const float* p, float* v, int pLength ) {
			vvexpf( v, p, &pLength );
		}
		inline	void
		Exp( const double* p, double* v, int pLength ) {
			vvexp( v, p, &pLength );
		}
		template	< typename F >	std::vector< F >
		Exp( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Exp( &p[ 0 ], &v[ 0 ], (int)v.size() );
			return v;
		}
		template	< typename F >	Vector< F >
		Exp( const iVector< F >& p ) {
			Vector< F > v( p );
			Exp( p.m, v.m, (int)v.n );
			return v;
		}
		template	< typename F >	std::vector< F >
		Sigmoid( const std::vector< F >& p ) {
			auto w = Exp( p );
			return w / ( F( 1 ) + w );
		}
		template	< typename F >	Vector< F >
		Sigmoid( const iVector< F >& p ) {
			auto w = Exp( p );
			return w / ( F( 1 ) + w );
	//		return Rec( Exp( - p ) + F( 1 ) );
		}
		//	( Vector, Vector ) -> Scalar

		inline	void
		Dot( const float* l, const float* r, float& v, vDSP_Length pLength ) {
			vDSP_dotpr( r, 1, l, 1, &v, pLength );
		}
		inline	void
		Dot( const double* l, const double* r, double& v, vDSP_Length pLength ) {
			vDSP_dotprD( r, 1, l, 1, &v, pLength );
		}
		template	< typename F >	F
		Dot( const std::vector< F >& l, const std::vector< F >& r ){
			assert( l.size() == r.size() );
			F	v = 0;
			Dot( &l[ 0 ], &r[ 0 ], v, l.size() );
			return v;
		}
		template	< typename F >	F
		Dot( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			F	v = 0;
			Dot( l.m, r.m, v, l.n );
			return v;
		}
	}
}
