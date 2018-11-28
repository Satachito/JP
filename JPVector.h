//	Written by Satoru Ogura, Fokyo.
//
#pragma once

#include	"JP.h"

#include	<cmath>
#include	<Accelerate/Accelerate.h>
#include	<vector>

namespace JP {

	template	< typename F >	struct
	vVector {
		F*		m;
		size_t	n;
		size_t	s;	//	Stride
		vVector( F* p, size_t n, size_t s = 1 ) : m( p ), n( n ), s( s ) {}
		virtual F
		operator []( size_t p ) const {
			return m[ p * s ];
		}
	};

	template	< typename F >	bool
	operator ==	( const vVector< F >& l, const vVector< F >& r ) {
		if ( l.n != r.n ) return false;
		for ( auto i = 0; i < l.n; i++ ) if ( l[ i ] != r[ i ] ) return false;
		return true;
	}
	template	< typename F >	bool
	operator !=	( const vVector< F >& l, const vVector< F >& r ) {
		return !( l == r );
	}

	template	< typename F >	std::ostream&
	operator << ( std::ostream& s, const vVector< F >& p ) {
		for ( auto i = 0; i < p.n; i ++ ) s << '\t' << p[ i ];
		return s;
	}


	namespace Accelerate {

		inline	void	Clear		( float * p, vDSP_Stride sP, vDSP_Length pLength )																		{ vDSP_vclr			( p, sP, pLength ); }
		inline	void	Clear		( double* p, vDSP_Stride sP, vDSP_Length pLength )																		{ vDSP_vclrD		( p, sP, pLength ); }
		inline	void	Ramp		( float  pInit, float  pStep, float * p, vDSP_Stride sP, vDSP_Length pLength )											{ vDSP_vramp		( &pInit, &pStep, p, sP, pLength ); }
		inline	void	Ramp		( double pInit, double pStep, double* p, vDSP_Stride sP, vDSP_Length pLength )											{ vDSP_vrampD		( &pInit, &pStep, p, sP, pLength ); }
		inline	void	Sum			( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )														{ vDSP_sve			( p, sP, &v, pLength ); }
		inline	void	Sum			( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )														{ vDSP_sveD			( p, sP, &v, pLength ); }
		inline	void	Mean		( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )														{ vDSP_meanv		( p, sP, &v, pLength ); }
		inline	void	Mean		( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )														{ vDSP_meanvD		( p, sP, &v, pLength ); }
		inline	void	Max			( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )														{ vDSP_maxv			( p, sP, &v, pLength ); }
		inline	void	Max			( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )														{ vDSP_maxvD		( p, sP, &v, pLength ); }
		inline	void	Min			( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )														{ vDSP_minv			( p, sP, &v, pLength ); }
		inline	void	Min			( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )														{ vDSP_minvD		( p, sP, &v, pLength ); }
		inline	void	L1Norm		( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )														{ vDSP_svemg		( p, sP, &v, pLength ); }
		inline	void	L1Norm		( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )														{ vDSP_svemgD		( p, sP, &v, pLength ); }
		inline	void	L2NormQ		( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )														{ vDSP_svesq		( p, sP, &v, pLength ); }
		inline	void	L2NormQ		( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )														{ vDSP_svesqD		( p, sP, &v, pLength ); }
		inline	void	DistanceQ	( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float & v, vDSP_Length pLength )					{ vDSP_distancesq	( l, sL, r, sR, &v, pLength ); }
		inline	void	DistanceQ	( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double& v, vDSP_Length pLength )					{ vDSP_distancesqD	( l, sL, r, sR, &v, pLength ); }
		inline	void	Neg			( const float * p, vDSP_Stride sP, float * v, vDSP_Stride sV, vDSP_Length pLength )										{ vDSP_vneg			( p, sP, v, sV, pLength ); }
		inline	void	Neg			( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length pLength )										{ vDSP_vnegD		( p, sP, v, sV, pLength ); }
		inline	void	Abs			( const float * p, vDSP_Stride sP, float * v, vDSP_Stride sV, vDSP_Length pLength )										{ vDSP_vabs			( p, sP, v, sV, pLength ); }
		inline	void	Abs			( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length pLength )										{ vDSP_vabsD		( p, sP, v, sV, pLength ); }
		inline	void	Rec			( const float * p, float * v, int pLength )																				{ vvrecf			( v, p, &pLength ); }
		inline	void	Rec			( const double* p, double* v, int pLength )																				{ vvrec				( v, p, &pLength ); }
		inline	void	Exp			( const float * p, float * v, int pLength )																				{ vvexpf			( v, p, &pLength ); }
		inline	void	Exp			( const double* p, double* v, int pLength )																				{ vvexp				( v, p, &pLength ); }
		inline	void	Add			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vadd			( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Add			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vaddD		( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Add			( const float * l, vDSP_Stride sL, const float & r, float * v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsadd		( l, sL, &r, v, sV, pLength ); }
		inline	void	Add			( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsaddD		( l, sL, &r, v, sV, pLength ); }
		inline	void	Sub			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vsub			( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Sub			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vsubD		( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Mul			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vmul			( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Mul			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vmulD		( l, sL, r, sR, v, sV, pLength ); }
		inline	void	Mul			( const float * l, vDSP_Stride sL, const float & r, float * v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsmul		( l, sL, &r, v, sV, pLength ); }
		inline	void	Mul			( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsmulD		( l, sL, &r, v, sV, pLength ); }
		inline	void	Div			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vdiv			( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Div			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vdivD		( r, sR, l, sL, v, sV, pLength ); }
		inline	void	Div			( const float * l, vDSP_Stride sL, const float & r, float * v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsdiv		( l, sL, &r, v, sV, pLength ); }
		inline	void	Div			( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_vsdivD		( l, sL, &r, v, sV, pLength ); }
		inline	void	Div			( const float & l, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_svdiv		( &l, r, sR, v, sV, pLength ); }
		inline	void	Div			( const double& l, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )					{ vDSP_svdivD		( &l, r, sR, v, sV, pLength ); }
		inline	void	Dot			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float & v, vDSP_Length pLength )					{ vDSP_dotpr		( r, sR, l, sL, &v, pLength ); }
		inline	void	Dot			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double& v, vDSP_Length pLength )					{ vDSP_dotprD		( r, sR, l, sL, &v, pLength ); }
		inline	void	Trans		( const float * p, vDSP_Stride sP, float * v, vDSP_Stride sV, vDSP_Length nR, vDSP_Length nC )							{ vDSP_mtrans		( p, sP, v, sV, nR, nC ); }
		inline	void	Trans		( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length nR, vDSP_Length nC )							{ vDSP_mtransD		( p, sP, v, sV, nR, nC ); }


		template	< typename F >	struct
		Vector 	: vVector< F > {
			~
			Vector() { delete[] vVector< F >::m; }

			Vector( 				size_t n	) : vVector< F >( new F[ n ]()	, n		) {}
			Vector( F* p		,	size_t n	) : vVector< F >( new F[ n ]	, n		) { for ( auto i = 0; i < n;	i ++ ) vVector< F >::m[ i ] = p[ i ]; }
			Vector( F p			,	size_t n	) : vVector< F >( new F[ n ]	, n		) { for ( auto i = 0; i < n;	i ++ ) vVector< F >::m[ i ] = p; }
			Vector( F( *p )()	,	size_t n	) : vVector< F >( new F[ n ]	, n		) { for ( auto i = 0; i < n;	i ++ ) vVector< F >::m[ i ] = p(); }
			Vector( const Vector< F >& p		) : vVector< F >( new F[ p.n ]	, p.n	) { for ( auto i = 0; i < p.n;	i ++ ) vVector< F >::m[ i ] = p.m[ i * p.s ]; }
			Vector( const vVector< F >& p		) : vVector< F >( new F[ p.n ]	, p.n	) { for ( auto i = 0; i < p.n;	i ++ ) vVector< F >::m[ i ] = p.m[ i * p.s ]; }
			
			const Vector< F >&	Substitution( const vVector< F >& p ) {
				delete[] vVector< F >::m;
				vVector< F >::m = new F[ p.n ];
				vVector< F >::n = p.n;
				vVector< F >::s = 1;
				for ( auto i = 0; i < p.n; i++ ) vVector< F >::m[ i ] = p.m[ i * p.s ];
				return *this;
			}
			const Vector< F >&	operator =	( const Vector< F >& p		) { return Substitution( p ); }
			const Vector< F >&	operator =	( const vVector< F >& p		) { return Substitution( p ); }
			F&					operator []	( size_t p					) { return vVector< F >::m[ p * vVector< F >::s ]; }
			void				Clear		(							) { Accelerate::Clear( vVector< F >::m, vVector< F >::s, vVector< F >::n ); }
			const Vector< F >&	operator +=	( const vVector< F >& p		) { Add( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this; }
			const Vector< F >&	operator -=	( const vVector< F >& p		) { Sub( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this; }
			const Vector< F >&	operator *=	( const vVector< F >& p		) { Mul( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this; }
			const Vector< F >&	operator /=	( const vVector< F >& p		) { Div( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this; }
			const Vector< F >&	operator +=	( F p						) { Add( vVector< F >::m, vVector< F >::s,  p, vVector< F >::m, vVector< F >::s, vVector< F >::n );			return *this; }
			const Vector< F >&	operator -=	( F p						) { Add( vVector< F >::m, vVector< F >::s, -p, vVector< F >::m, vVector< F >::s, vVector< F >::n );			return *this; }
			const Vector< F >&	operator *=	( F p						) { Mul( vVector< F >::m, vVector< F >::s,  p, vVector< F >::m, vVector< F >::s, vVector< F >::n );			return *this; }
			const Vector< F >&	operator /=	( F p						) { Div( vVector< F >::m, vVector< F >::s,  p, vVector< F >::m, vVector< F >::s, vVector< F >::n );			return *this; }
		};


		template	< typename F >	std::vector< F >	operator +		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Add		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v; }
		template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Sub		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v; }
		template	< typename F >	std::vector< F >	operator *		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Mul		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v; }
		template	< typename F >	std::vector< F >	operator /		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Div		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v; }

		template	< typename F >	std::vector< F >	operator +		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Add		( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v; }
		template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Add		( &p[ 0 ], 1, -s, &v[ 0 ], 1, v.size() );			return v; }
		template	< typename F >	std::vector< F >	operator *		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Mul		( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v; }
		template	< typename F >	std::vector< F >	operator /		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Div		( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v; }

		template	< typename F >	std::vector< F >	operator +		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		Add		( &p[ 0 ], 1, s, &v[ 0 ], 1, v.size() );			return v; }
		template	< typename F >	std::vector< F >	operator -		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size(), s );	Sub		( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v; }
		template	< typename F >	std::vector< F >	operator *		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		Mul		( &p[ 0 ], 1, s, &v[ 0 ], 1, v.size() );			return v; }
		template	< typename F >	std::vector< F >	operator /		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		Div		( s, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );			return v; }

		template	< typename F >	std::vector< F >	RampArray		( size_t p					, F pInit = 0, F pStep = 1	) { std::vector< F > v( p );			Ramp	( pInit, pStep, &v[ 0 ], 1, p );					return v; }
		template	< typename F >	F					Sum				( const std::vector< F >& p								) { F v = 0;							Sum		( &p[ 0 ], 1, v, p.size() );						return v; }
		template	< typename F >	F					Mean			( const std::vector< F >& p								) { F v = 0;							Mean	( &p[ 0 ], 1, v, p.size() );						return v; }
		template	< typename F >	F					Max				( const std::vector< F >& p								) { F v = 0;							Max		( &p[ 0 ], 1, v, p.size() );						return v; }
		template	< typename F >	F					Min				( const std::vector< F >& p								) { F v = 0;							Min		( &p[ 0 ], 1, v, p.size() );						return v; }
		template	< typename F >	F					L1Norm			( const std::vector< F >& p								) { F v = 0;							L1Norm	( &p[ 0 ], 1, v, p.size() );						return v; }
		template	< typename F >	F					L2NormQ			( const std::vector< F >& p								) { F v = 0;							L2NormQ	( &p[ 0 ], 1, v, p.size() );						return v; }
		template	< typename F >	F					L2Norm			( const std::vector< F >& p								) {																									return sqrt( L2NormQ( p ) ); }
		template	< typename F >	std::vector< F >	UnitVector		( const std::vector< F >& p								) {																									return p / L2Norm( p ); }
		template	< typename F >	std::vector< F >	Abs				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Abs		( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );				return v; }
		template	< typename F >	std::vector< F >	Rec				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Rec		( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v; }
		template	< typename F >	std::vector< F >	Exp				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Exp		( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v; }
		template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Neg		( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );				return v; }
		template	< typename F >	F					Dot				( const std::vector< F >& l	, const std::vector< F >& r	) { F v = 0;							Dot		( &l[ 0 ], 1, &r[ 0 ], 1, v, l.size() );			return v; }



		template	< typename F >	Vector< F >			operator +		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Add		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v; }
		template	< typename F >	Vector< F >			operator -		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Sub		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v; }
		template	< typename F >	Vector< F >			operator *		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Mul		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v; }
		template	< typename F >	Vector< F >			operator /		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Div		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v; }

		template	< typename F >	Vector< F >			operator +		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Add		( p.m, p.s,  s, v.m, v.s, v.n );					return v; }
		template	< typename F >	Vector< F >			operator -		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Add		( p.m, p.s, -s, v.m, v.s, v.n );					return v; }
		template	< typename F >	Vector< F >			operator *		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Mul		( p.m, p.s,  s, v.m, v.s, v.n );					return v; }
		template	< typename F >	Vector< F >			operator /		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Div		( p.m, p.s,  s, v.m, v.s, v.n );					return v; }

		template	< typename F >	Vector< F >			operator +		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				Add		( p.m, p.s, s, v.m, v.s, v.n );						return v; }
		template	< typename F >	Vector< F >			operator -		( F s						, const vVector< F >& p		) { Vector< F > v( s, p.n );			Sub		( v.m, v.s, p.m, p.s, v.m, v.s, v.n );				return v; }
		template	< typename F >	Vector< F >			operator *		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				Mul		( p.m, p.s, s, v.m, v.s, v.n );						return v; }
		template	< typename F >	Vector< F >			operator /		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				Div		( s, p.m, p.s, v.m, v.s, v.n );						return v; }

		template	< typename F >	Vector< F >			RampVector		( size_t p					, F pInit = 0, F pStep = 1	) { Vector< F > v( p );					Ramp	( pInit, pStep, v.m, v.s, p );						return v; }
		template	< typename F >	F					Sum				( const vVector< F >& p									) { F v = 0;							Sum		( p.m, p.s, v, p.n );								return v; }
		template	< typename F >	F					Mean			( const vVector< F >& p									) { F v = 0;							Mean	( p.m, p.s, v, p.n );								return v; }
		template	< typename F >	F					Max				( const vVector< F >& p									) { F v = 0;							Max		( p.m, p.s, v, p.n );								return v; }
		template	< typename F >	F					Min				( const vVector< F >& p									) { F v = 0;							Min		( p.m, p.s, v, p.n );								return v; }
		template	< typename F >	F					L1Norm			( const vVector< F >& p									) { F v = 0;							L1Norm	( p.m, p.s, v, p.n );								return v; }
		template	< typename F >	F					L2NormQ			( const vVector< F >& p									) { F v = 0;							L2NormQ	( p.m, p.s, v, p.n );								return v; }
		template	< typename F >	F					L2Norm			( const vVector< F >& p									) {																									return sqrt( L2NormQ( p ) ); }
		template	< typename F >	Vector< F >			UnitVector		( const vVector< F >& p									) {																									return p / L2Norm( p ); }
		template	< typename F >	Vector< F >			Abs				( const vVector< F >& p									) {	Vector< F > v( p );					Abs		( p.m, p.s, v.m, v.s, v.n );						return v; }
		template	< typename F >	Vector< F >			Rec				( const vVector< F >& p			) {	assert( p.s == 1 );	Vector< F > v( p );	assert( v.s == 1 );	Rec		( p.m, v.m, (int)v.n );								return v; }
		template	< typename F >	Vector< F >			Exp				( const vVector< F >& p			) {	assert( p.s == 1 );	Vector< F > v( p );	assert( v.s == 1 );	Exp		( p.m, v.m, (int)v.n );								return v; }
		template	< typename F >	Vector< F >			operator -		( const vVector< F >& p									) {	Vector< F > v( p );					Neg		( p.m, p.s, v.m, v.s, v.n );						return v; }
		template	< typename F >	F					Dot				( const vVector< F >& l		, const vVector< F >& r		) { F v = 0;							Dot		( l.m, l.s, r.m, r.s, v, l.n );						return v; }
	}
}
