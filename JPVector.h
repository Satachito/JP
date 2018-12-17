//	Written by Satoru Ogura, Fokyo.
//
#pragma once

#include	"JPFloat.h"

#include	<cmath>
#include	<vector>

namespace JP {

	template	< typename F >	struct
	vVector {
		F*		m;
		size_t	n;
		size_t	s;	//	Stride

		vVector( F* p, size_t n, size_t s = 1 ) : m( p ), n( n ), s( s ) {}

		F	operator []( size_t p ) const { return m[ p * s ]; }
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

	template	< typename F >	struct
	Vector 	: vVector< F > {
	~	Vector() { delete[] vVector< F >::m; }

		Vector(							)			: vVector< F >( 0				, 0		) {																		}
		Vector( size_t n				)			: vVector< F >( new F[ n ]()	, n		) {																		}
		Vector( size_t n, F p			)			: vVector< F >( new F[ n ]		, n		) { for ( auto i = 0; i < n;	i ++ ) vVector< F >::m[ i ] = p;		}
		Vector( size_t n, F* p			)			: vVector< F >( new F[ n ]		, n		) { for ( auto i = 0; i < n;	i ++ ) vVector< F >::m[ i ] = p[ i ];	}
		Vector( size_t n, F( *p )()		)			: vVector< F >( new F[ n ]		, n		) { for ( auto i = 0; i < n;	i ++ ) vVector< F >::m[ i ] = p();		}
		Vector( Vector&& p				) noexcept	: vVector< F >( p.m				, p.n	) { p.m = 0; p.n = 0;													}
		Vector( const Vector& p			)			: vVector< F >( new F[ p.n ]	, p.n	) { for ( auto i = 0; i < p.n;	i ++ ) vVector< F >::m[ i ] = p[ i ];	}
		Vector( const vVector< F >& p	)			: vVector< F >( new F[ p.n ]	, p.n	) { for ( auto i = 0; i < p.n;	i ++ ) vVector< F >::m[ i ] = p[ i ];	}

		Vector&
		_Substitution( const vVector< F >& p ) {
			if ( vVector< F >::n != p.n ) {
				auto w = vVector< F >::m;
				vVector< F >::m = new F[ p.n ];
				delete[] w;
				vVector< F >::n = p.n;
			}
			for ( auto i = 0; i < p.n; i++ ) vVector< F >::m[ i ] = p[ i ];
			return *this;
		}
		Vector&	operator =	( Vector&& p ) noexcept {
			if ( this != &p ) {
				delete[] vVector< F >::m;
				vVector< F >::m = p.m;	p.m = 0;
				vVector< F >::n = p.n;	p.n = 0;
			}
			return *this;
		}
		Vector&	operator =	( const Vector& p		) { return _Substitution( p );																								}
		Vector&	operator =	( const vVector< F >& p	) { return _Substitution( p );																								}
		F&		operator []	( size_t p				) { return vVector< F >::m[ p * vVector< F >::s ];																			}
		void	Clear		(						) { _Clr( vVector< F >::m, vVector< F >::s, vVector< F >::n );																}
		Vector&	operator +=	( const vVector< F >& p	) { _Add( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
		Vector&	operator -=	( const vVector< F >& p	) { _Sub( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
		Vector&	operator *=	( const vVector< F >& p	) { _Mul( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
		Vector&	operator /=	( const vVector< F >& p	) { _Div( vVector< F >::m, vVector< F >::s, p.m, p.s, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
		Vector&	operator +=	( F p					) { _Add( vVector< F >::m, vVector< F >::s,  p		, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
		Vector&	operator -=	( F p					) { _Add( vVector< F >::m, vVector< F >::s, -p		, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
		Vector&	operator *=	( F p					) { _Mul( vVector< F >::m, vVector< F >::s,  p		, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
		Vector&	operator /=	( F p					) { _Div( vVector< F >::m, vVector< F >::s,  p		, vVector< F >::m, vVector< F >::s, vVector< F >::n );	return *this;	}
	};

	template	< typename F >	std::vector< F >	operator +		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		_Add		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		_Sub		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator *		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		_Mul		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator /		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		_Div		( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}

	template	< typename F >	std::vector< F >	operator +		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		_Add		( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		_Add		( &p[ 0 ], 1, -s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator *		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		_Mul		( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator /		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		_Div		( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v;	}

	template	< typename F >	std::vector< F >	operator +		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		_Add		( &p[ 0 ], 1, s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator -		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size(), s );	_Sub		( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator *		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		_Mul		( &p[ 0 ], 1, s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator /		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		_Div		( s, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );			return v;	}

	template	< typename F >	std::vector< F >	RampArray		( size_t p					, F pInit = 0, F pStep = 1	) { std::vector< F > v( p );			_Ramp		( pInit, pStep, &v[ 0 ], 1, p );					return v;	}
	template	< typename F >	F					Sum				( const std::vector< F >& p								) { F v = 0;							_Sve		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					Mean			( const std::vector< F >& p								) { F v = 0;							_Mean		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					Max				( const std::vector< F >& p								) { F v = 0;							_Max		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					Min				( const std::vector< F >& p								) { F v = 0;							_Min		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					L1Norm			( const std::vector< F >& p								) { F v = 0;							_Svemg		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					L2NormQ			( const std::vector< F >& p								) { F v = 0;							_Svesq		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					L2Norm			( const std::vector< F >& p								) {	return sqrt( L2NormQ( p ) );																					}
	template	< typename F >	std::vector< F >	UnitVector		( const std::vector< F >& p								) {	return p / L2Norm( p );																							}
	template	< typename F >	std::vector< F >	Abs				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		_Abs		( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );				return v;	}
	template	< typename F >	std::vector< F >	Rec				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		_Rec		( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v;	}
	template	< typename F >	std::vector< F >	Exp				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		_Exp		( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v;	}
	template	< typename F >	std::vector< F >	Log				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		_Log		( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v;	}
	template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& p								) { std::vector< F > v( p.size() );		_Neg		( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );				return v;	}
	template	< typename F >	F					Dot				( const std::vector< F >& l	, const std::vector< F >& r	) { F v = 0;							_Dot		( &l[ 0 ], 1, &r[ 0 ], 1, v, l.size() );			return v;	}
	template	< typename F >	F					DistanceQ		( const std::vector< F >& l	, const std::vector< F >& r	) { F v = 0;							_Distancesq	( &l[ 0 ], 1, &r[ 0 ], 1, v, l.size() );			return v;	}



	template	< typename F >	Vector< F >			operator +		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				_Add		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator -		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				_Sub		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator *		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				_Mul		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator /		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				_Div		( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}

	template	< typename F >	Vector< F >			operator +		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				_Add		( p.m, p.s,  s, v.m, v.s, v.n );					return v;	}
	template	< typename F >	Vector< F >			operator -		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				_Add		( p.m, p.s, -s, v.m, v.s, v.n );					return v;	}
	template	< typename F >	Vector< F >			operator *		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				_Mul		( p.m, p.s,  s, v.m, v.s, v.n );					return v;	}
	template	< typename F >	Vector< F >			operator /		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				_Div		( p.m, p.s,  s, v.m, v.s, v.n );					return v;	}

	template	< typename F >	Vector< F >			operator +		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				_Add		( p.m, p.s, s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	Vector< F >			operator -		( F s						, const vVector< F >& p		) { Vector< F > v( p.n, s );			_Sub		( v.m, v.s, p.m, p.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator *		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				_Mul		( p.m, p.s, s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	Vector< F >			operator /		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				_Div		( s, p.m, p.s, v.m, v.s, v.n );						return v;	}

	template	< typename F >	Vector< F >			RampVector		( size_t p					, F pInit = 0, F pStep = 1	) { Vector< F > v( p );					_Ramp		( pInit, pStep, v.m, v.s, p );						return v;	}
	template	< typename F >	F					Sum				( const vVector< F >& p									) { F v = 0;							_Sve		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					Mean			( const vVector< F >& p									) { F v = 0;							_Mean		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					Max				( const vVector< F >& p									) { F v = 0;							_Max		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					Min				( const vVector< F >& p									) { F v = 0;							_Min		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					L1Norm			( const vVector< F >& p									) { F v = 0;							_Svemg		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					L2NormQ			( const vVector< F >& p									) { F v = 0;							_Svesq		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					L2Norm			( const vVector< F >& p									) { return sqrt( L2NormQ( p ) );																					}
	template	< typename F >	Vector< F >			UnitVector		( const vVector< F >& p									) { return p / L2Norm( p );																							}
	template	< typename F >	Vector< F >			Abs				( const vVector< F >& p									) {	Vector< F > v( p );					_Abs		( p.m, p.s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	Vector< F >			Rec				( const vVector< F >& p									) {	Vector< F > v( p );					_Rec		( p.m, v.m, (int)v.n );	assert( p.s == 1 );			return v;	}
	template	< typename F >	Vector< F >			Exp				( const vVector< F >& p									) {	Vector< F > v( p );					_Exp		( p.m, v.m, (int)v.n );	assert( p.s == 1 );			return v;	}
	template	< typename F >	Vector< F >			Log				( const vVector< F >& p									) {	Vector< F > v( p );					_Log		( p.m, v.m, (int)v.n );	assert( p.s == 1 );			return v;	}
	template	< typename F >	Vector< F >			operator -		( const vVector< F >& p									) {	Vector< F > v( p );					_Neg		( p.m, p.s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	F					Dot				( const vVector< F >& l		, const vVector< F >& r		) { F v = 0;							_Dot		( l.m, l.s, r.m, r.s, v, l.n );						return v;	}
	template	< typename F >	F					DistanceQ		( const vVector< F >& l		, const vVector< F >& r		) { F v = 0;							_Distancesq	( l.m, l.s, r.m, r.s, v, l.n );						return v;	}
}
