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
		size_t	s;	//	Stride
		iVector( F* p, size_t n, size_t s = 1 ) : m( p ), n( n ), s( s ) {}
		virtual F
		operator []( size_t p ) const {
			return m[ p * s ];
		}
	};
	
	template	< typename F >	std::ostream&
	operator << ( std::ostream& s, const iVector< F >& p ) {
		for ( auto i = 0; i < p.n; i++ ) s << '\t' << p[ i ];
		return s;
	}

	namespace Accelerate {

		inline	void	Clear( float* p, vDSP_Stride sP, vDSP_Length pLength )																	{ vDSP_vclr			( p, sP, pLength ); }
		inline	void	Clear( double* p, vDSP_Stride sP, vDSP_Length pLength )																	{ vDSP_vclrD		( p, sP, pLength ); }
		inline	void	Ramp( float pInit, float pStep, float* p, vDSP_Stride sP, vDSP_Length pLength )											{ vDSP_vramp		( &pInit, &pStep, p, sP, pLength ); }
		inline	void	Ramp( double pInit, double pStep, double* p, vDSP_Stride sP, vDSP_Length pLength )										{ vDSP_vrampD		( &pInit, &pStep, p, sP, pLength ); }
		inline	void	Sum( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength )													{ vDSP_sve			( p, sP, &v, pLength ); }
		inline	void	Sum( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )													{ vDSP_sveD			( p, sP, &v, pLength ); }
		inline	void	Mean( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength )													{ vDSP_meanv		( p, sP, &v, pLength ); }
		inline	void	Mean( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )													{ vDSP_meanvD		( p, sP, &v, pLength ); }
		inline	void	L1Norm( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength )													{ vDSP_svemg		( p, sP, &v, pLength ); }
		inline	void	L1Norm( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )												{ vDSP_svemgD		( p, sP, &v, pLength ); }
		inline	void	L2NormQ( const float* p, vDSP_Stride sP, float& v, vDSP_Length pLength )												{ vDSP_svesq		( p, sP, &v, pLength ); }
		inline	void	L2NormQ( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )												{ vDSP_svesqD		( p, sP, &v, pLength ); }
		inline	void	DistanceQ( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float& v, vDSP_Length pLength )				{ vDSP_distancesq	( l, sL, r, sR, &v, pLength ); }
		inline	void	DistanceQ( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double& v, vDSP_Length pLength )			{ vDSP_distancesqD	( l, sL, r, sR, &v, pLength ); }
		inline	void	Add( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vadd			( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Add( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vaddD		( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Add( const float* l, vDSP_Stride sL, const float& r, float* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsadd		( l, sL, &r, v, sV, pLength ); }
		inline	void	Add( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsaddD		( l, sL, &r, v, sV, pLength ); }
		inline	void	Sub( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vsub			( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Sub( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vsubD		( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Sub( const float* l, vDSP_Stride sL, const float& r, float* v, vDSP_Stride sV, vDSP_Length pLength )					{ auto w = -r;	vDSP_vsadd	( l, sL, &w, v, sV, pLength ); }
		inline	void	Sub( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ auto w = -r;	vDSP_vsaddD	( l, sL, &w, v, sV, pLength ); }
		inline	void	Mul( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vmul			( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Mul( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vmulD		( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Mul( const float* l, vDSP_Stride sL, const float& r, float* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsmul		( l, sL, &r, v, sV, pLength ); }
		inline	void	Mul( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsmulD		( l, sL, &r, v, sV, pLength ); }
		inline	void	Div( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vdiv			( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Div( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vdivD		( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Div( const float* l, vDSP_Stride sL, const float& r, float* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsdiv		( l, sL, &r, v, sV, pLength ); }
		inline	void	Div( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsdivD		( l, sL, &r, v, sV, pLength ); }
		inline	void	Neg( const float* p, vDSP_Stride sP, float* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vneg			( p, sP, v, sV, pLength ); }
		inline	void	Neg( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vnegD		( p, sP, v, sV, pLength ); }
		inline	void	Abs( const float* p, vDSP_Stride sP, float* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vabs			( p, sP, v, sV, pLength ); }
		inline	void	Abs( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vabsD		( p, sP, v, sV, pLength ); }
		inline	void	Rec( const float* p, float* v, int pLength )																			{ vvrecf			( v, p, &pLength ); }
		inline	void	Rec( const double* p, double* v, int pLength )																			{ vvrec				( v, p, &pLength ); }
		inline	void	Exp( const float* p, float* v, int pLength )																			{ vvexpf			( v, p, &pLength ); }
		inline	void	Exp( const double* p, double* v, int pLength )																			{ vvexp				( v, p, &pLength ); }
		inline	void	Dot( const float* l, vDSP_Stride sL, const float* r, vDSP_Stride sR, float& v, vDSP_Length pLength )					{ vDSP_dotpr		( r, sR, l, sL, &v, pLength ); }
		inline	void	Dot( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double& v, vDSP_Length pLength )					{ vDSP_dotprD		( r, sR, l, sL, &v, pLength ); }

		template	< typename F >	struct
		Vector 	: iVector< F > {
			~
			Vector() {
				delete[] iVector< F >::m;
			}
			Vector( size_t n, size_t s = 1 )
			:	iVector< F >( new F[ n * s ](), n, s ) {
			}
			Vector( F* p, size_t n, size_t s = 1 )
			:	iVector< F >( new F[ n * s ], n, s ) {
				for ( auto i = 0; i < n * s; i += s ) iVector< F >::m[ i ] = p[ i ];
			}
			Vector( F p, size_t n, size_t s = 1 )
			:	iVector< F >( new F[ n * s ], n, s ) {
				for ( auto i = 0; i < n * s; i += s ) iVector< F >::m[ i ] = p;
			}
			Vector( F( *p )(), size_t n, size_t s = 1 )
			:	iVector< F >( new F[ n * s ], n, s ) {
				for ( auto i = 0; i < n * s; i += s ) iVector< F >::m[ i ] = p();
			}
			Vector( const Vector< F >& p )
			:	iVector< F >( new F[ p.n * p.s ], p.n, p.s ) {
				for ( auto i = 0; i < p.n * p.s; i += p.s ) iVector< F >::m[ i ] = p.m[ i ];
			}
			Vector( const iVector< F >& p )
			:	iVector< F >( new F[ p.n * p.s ], p.n, p.s ) {
				for ( auto i = 0; i < p.n * p.s; i += p.s ) iVector< F >::m[ i ] = p.m[ i ];
			}
			const Vector< F >&
			operator =( const Vector< F >& p ) {
				delete[] iVector< F >::m;
				iVector< F >::m = new F[ p.n * p.s ];
				iVector< F >::n = p.n;
				iVector< F >::s = p.s;
				for ( auto i = 0; i < p.n * p.s; i += p.s ) iVector< F >::m[ i ] = p.m[ i ];
				return *this;
			}
			const Vector< F >&
			operator =( const iVector< F >& p ) {
				delete[] iVector< F >::m;
				iVector< F >::m = new F[ p.n * p.s ];
				iVector< F >::n = p.n;
				iVector< F >::s = p.s;
				for ( auto i = 0; i < p.n * p.s; i += p.s ) iVector< F >::m[ i ] = p.m[ i ];
				return *this;
			}
			F&
			operator []( size_t p ) {
				return iVector< F >::m[ p * iVector< F >::s ];
			}
			void
			Clear() {
				Accelerate::Clear( iVector< F >::m, iVector< F >::s, iVector< F >::n );
			}
			const Vector< F >&
			operator +=( const iVector< F >& p ) {
				Add( iVector< F >::m, iVector< F >::s, p.m, p.s, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
			const Vector< F >&
			operator -=( const iVector< F >& p ) {
				Sub( iVector< F >::m, iVector< F >::s, p.m, p.s, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
			const Vector< F >&
			operator *=( const iVector< F >& p ) {
				Mul( iVector< F >::m, iVector< F >::s, p.m, p.s, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
			const Vector< F >&
			operator /=( const iVector< F >& p ) {
				Div( iVector< F >::m, iVector< F >::s, p.m, p.s, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
			const Vector< F >&
			operator +=( F p ) {
				Add( iVector< F >::m, iVector< F >::s, p, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
			const Vector< F >&
			operator -=( F p ) {
				Sub( iVector< F >::m, iVector< F >::s, p, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
			const Vector< F >&
			operator *=( F p ) {
				Mul( iVector< F >::m, iVector< F >::s, p, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
			const Vector< F >&
			operator /=( F p ) {
				Div( iVector< F >::m, iVector< F >::s, p, iVector< F >::m, iVector< F >::s, iVector< F >::n );
				return *this;;
			}
		};

		//	() -> Vector
		template	< typename F >	std::vector< F >	Rampv			( size_t p, F pInit = 0, F pStep = 1 )	{	std::vector< F > v( p );	Ramp( pInit, pStep, &v[ 0 ], 1, p );	return v;	}
		template	< typename F >	Vector< F >			Ramp			( size_t p, F pInit = 0, F pStep = 1 )	{	Vector< F > v( p );			Ramp( pInit, pStep, v.m, v.s, p );		return v;	}

		//	Vector -> Scalar
		template	< typename F >	F					Sum				( const std::vector< F >& p )			{	F	v = 0;					Sum( &p[ 0 ], 1, v, p.size() );			return v;	}
		template	< typename F >	F					Sum				( const iVector< F >& p )				{	F	v = 0;					Sum( p.m, p.s, v, p.n );				return v;	}

		template	< typename F >	F					Mean			( const std::vector< F >& p )			{	F	v = 0;					Mean( &p[ 0 ], 1, v, p.size() );		return v;	}
		template	< typename F >	F					Mean			( const iVector< F >& p )				{	F	v = 0;					Mean( p.m, p.s, v, p.n );				return v;	}

		template	< typename F >	F					L1Norm			( const std::vector< F >& p )			{	F	v = 0;					L1Norm( &p[ 0 ], 1, v, p.size() );		return v;	}
		template	< typename F >	F					L1Norm			( const iVector< F >& p )				{	F	v = 0;					L1Norm( p.m, p.s, v, p.n );				return v;	}

		template	< typename F >	F					L2NormQ			( const std::vector< F >& p )			{	F	v = 0;					L2NormQ( &p[ 0 ], 1, v, p.size() );		return v;	}
		template	< typename F >	F					L2NormQ			( const iVector< F >& p )				{	F	v = 0;					L2NormQ( p.m, p.s, v, p.n );			return v;	}

		template	< typename F >	F					L2Norm			( const std::vector< F >& p )			{	return sqrt( L2NormQ( p ) );	}
		template	< typename F >	F					L2Norm			( const iVector< F >& p )				{	return sqrt( L2NormQ( p ) );	}
		//	( Vector, Vector ) -> Vector
		//	( Scalar, Vector ) -> Vector
		//	( Vector, Scalar ) -> Vector

		template	< typename F >	std::vector< F >	operator +	( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Add( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator +	( const std::vector< F >& p, F scalar ) {
			std::vector< F > v( p.size(), scalar );
			Add( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator +	( F scalar, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), scalar );
			Add( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	Vector< F >			operator +	( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Add( l.m, l.s, r.m, r.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator +	( const iVector< F >& p, F scalar ) {
			Vector< F > v( scalar, p.n );
			Add( p.m, p.s, v.m, v.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator +	( F scalar, const iVector< F >& p ) {
			Vector< F > v( scalar, p.n );
			Add( v.m, v.s, p.m, p.s, v.m, v.s, v.n );
			return v;
		}

		template	< typename F >	std::vector< F >	operator -	( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Sub( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator -	( const std::vector< F >& p, F scalar ) {
			std::vector< F > v( p.size(), scalar );
			Sub( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator -	( F scalar, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), scalar );
			Sub( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	Vector< F >			operator -	( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Sub( l.m, l.s, r.m, r.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator -	( const iVector< F >& p, F scalar ) {
			Vector< F > v( scalar, p.n );
			Sub( p.m, p.s, v.m, v.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator -	( F scalar, const iVector< F >& p ) {
			Vector< F > v( scalar, p.n );
			Sub( v.m, v.s, p.m, p.s, v.m, v.s, v.n );
			return v;
		}

		template	< typename F >	std::vector< F >	operator *	( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Mul( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator *	( const std::vector< F >& p, F scalar ) {
			std::vector< F > v( p.size(), scalar );
			Mul( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator *	( F scalar, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), scalar );
			Mul( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}

		template	< typename F >	Vector< F >			operator *( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Mul( l.m, l.s, r.m, r.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator *( const iVector< F >& p, F scalar ) {
			Vector< F > v( scalar, p.n );
			Mul( p.m, p.s, v.m, v.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator *( F scalar, const iVector< F >& p ) {
			Vector< F > v( scalar, p.n );
			Mul( v.m, v.s, p.m, p.s, v.m, v.s, v.n );
			return v;
		}

		template	< typename F >	std::vector< F >	operator /( const std::vector< F >& l, const std::vector< F >& r ) {
			assert( l.size() == r.size() );
			std::vector< F > v( l.size() );
			Div( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator /( const std::vector< F >& p, F scalar ) {
			std::vector< F > v( p.size(), scalar );
			Div( &p[ 0 ], 1, &v[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	std::vector< F >	operator /( F scalar, const std::vector< F >& p ) {
			std::vector< F > v( p.size(), scalar );
			Div( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}

		template	< typename F >	Vector< F >			operator /( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			Vector< F > v( l.n );
			Div( l.m, l.s, r.m, r.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator /( const iVector< F >& p, F scalar ) {
			Vector< F > v( scalar, p.n );
			Div( p.m, p.s, v.m, v.s, v.m, v.s, v.n );
			return v;
		}
		template	< typename F >	Vector< F >			operator /( F scalar, const iVector< F >& p ) {
			Vector< F > v( scalar, p.n );
			Div( v.m, v.s, p.m, p.s, v.m, v.s, v.n );
			return v;
		}

		//	Vector -> Vector

		template	< typename F >	std::vector< F >	UnitVector( const std::vector< F >& p )	{	return p / L2Norm( p );	}
		template	< typename F >	Vector< F >			UnitVector( const iVector< F >& p )		{	return p / L2Norm( p );	}

		template	< typename F >	std::vector< F >	operator -( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Neg( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	Vector< F >			operator -( const iVector< F >& p ) {
			Vector< F > v( p );
			Neg( p.m, p.s, v.m, v.s, v.n );
			return v;
		}

		template	< typename F >	std::vector< F >	Abs( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Abs( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );
			return v;
		}
		template	< typename F >	Vector< F >			Abs( const iVector< F >& p ) {
			Vector< F > v( p );
			Abs( p.m, p.s, v.m, v.s, v.n );
			return v;
		}

		template	< typename F >	std::vector< F >	Rec( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Rec( &p[ 0 ], &v[ 0 ], (int)v.size() );
			return v;
		}
		template	< typename F >	Vector< F >			Rec( const iVector< F >& p ) {
			assert( p.s == 1 );	//	Don't support strided
			Vector< F > v( p );
			Rec( p.m, v.m, (int)v.n );
			return v;
		}

		template	< typename F >	std::vector< F >	Exp( const std::vector< F >& p ) {
			std::vector< F > v( p.size() );
			Exp( &p[ 0 ], &v[ 0 ], (int)v.size() );
			return v;
		}
		template	< typename F >	Vector< F >			Exp( const iVector< F >& p ) {
			assert( p.s == 1 );	//	Don't support strided
			Vector< F > v( p );
			Exp( p.m, v.m, (int)v.n );
			return v;
		}

		//	( Vector, Vector ) -> Scalar

		template	< typename F >	F					Dot( const std::vector< F >& l, const std::vector< F >& r ){
			assert( l.size() == r.size() );
			F	v = 0;
			Dot( &l[ 0 ], 1, &r[ 0 ], 1, v, l.size() );
			return v;
		}
		template	< typename F >	F					Dot( const iVector< F >& l, const iVector< F >& r ) {
			assert( l.n == r.n );
			F	v = 0;
			Dot( l.m, l.s, r.m, r.s, v, l.n );
			return v;
		}
	}
}
