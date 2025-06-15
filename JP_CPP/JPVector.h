//	Written by Satoru Ogura, Fokyo.
//
#pragma once

#include	"JPFloat.h"

#include	<vector>

namespace JP {

	template	< typename F >	struct
	vVector {
				F*						m;
				size_t					n;
				size_t					s;	//	Stride

										vVector	( F* p, size_t n, size_t s = 1 ) : m( p ), n( n ), s( s ) {}

				F			operator	[]		( size_t p			) const	{ return m[ p * s ];								}
				F&			operator	[]		( size_t p			)		{ return m[ p * s ];								}

				void					Clear	(					) const	{ Clr( m, s, n );									}
		const	vVector&	operator	+=		( const vVector& p	) const	{ Add( m, s, p.m, p.s	, m, s, n ); return *this;	}
		const	vVector&	operator	-=		( const vVector& p	) const	{ Sub( m, s, p.m, p.s	, m, s, n ); return *this;	}
		const	vVector&	operator	*=		( const vVector& p	) const	{ Mul( m, s, p.m, p.s	, m, s, n ); return *this;	}
		const	vVector&	operator	/=		( const vVector& p	) const	{ Div( m, s, p.m, p.s	, m, s, n ); return *this;	}
		const	vVector&	operator	+=		( F p				) const	{ Add( m, s,  p			, m, s, n ); return *this;	}
		const	vVector&	operator	-=		( F p				) const	{ Add( m, s, -p			, m, s, n ); return *this;	}
		const	vVector&	operator	*=		( F p				) const	{ Mul( m, s,  p			, m, s, n ); return *this;	}
		const	vVector&	operator	/=		( F p				) const	{ Div( m, s,  p			, m, s, n ); return *this;	}

		struct
		I {
			const	vVector&	m;
					size_t		index = 0;

			I( const vVector& p, size_t index ) : m( p ), index( index ) {}

			I&			operator ++	()						{ ++index; return *this; }
			F			operator *	()						{ return m[ index ]; }
			bool		operator !=	( const I& p )	const	{ return index != p.index; }
		};
		I	begin()	const { return I( *this, 0 ); }
		I	end()	const { return I( *this, n ); }
	};

	template	< typename F >	bool			operator
	==	( const vVector< F >& l, const vVector< F >& r ) {
		if ( l.n != r.n ) return false;
		for ( auto i = 0; i < l.n; i++ ) if ( l[ i ] != r[ i ] ) return false;
		return true;
	}
	template	< typename F >	bool			operator
	!=	( const vVector< F >& l, const vVector< F >& r ) {
		return !( l == r );
	}
	template	< typename F >	std::ostream&	operator
	<<	( std::ostream& s, const vVector< F >& p ) {
		for ( auto i = 0; i < p.n; i ++ ) s << '\t' << p[ i ];
		return s;
	}

	template	< typename F >	struct
	Vector 	: vVector< F > {
	~	Vector() { delete[] vVector< F >::m; }

		Vector(									)			: vVector< F >( 0					, 0			) {																					}
		Vector( size_t n						)			: vVector< F >( new F[ n ]()		, n			) {																					}
		Vector( size_t n, F p					)			: vVector< F >( new F[ n ]			, n			) { for ( auto i = 0; i < n;		i ++ ) vVector< F >::m[ i ] = p;				}
		Vector( size_t n, F* p					)			: vVector< F >( new F[ n ]			, n			) { for ( auto i = 0; i < n;		i ++ ) vVector< F >::m[ i ] = p[ i ];			}
		Vector( size_t n, F( *p )()				)			: vVector< F >( new F[ n ]			, n			) { for ( auto i = 0; i < n;		i ++ ) vVector< F >::m[ i ] = p();				}
		Vector( std::initializer_list< F > p	)			: vVector< F >( new F[ p.size() ]	, p.size()	) { for ( auto i = 0; i < p.size();	i ++ ) vVector< F >::m[ i ] = p.begin()[ i ];	}
		Vector( Vector&& p						) noexcept	: vVector< F >( p.m					, p.n		) { p.m = 0; p.n = 0;																}
		Vector( const Vector& p					)			: vVector< F >( new F[ p.n ]		, p.n		) { for ( auto i = 0; i < p.n;		i ++ ) vVector< F >::m[ i ] = p[ i ];			}
		Vector( const vVector< F >& p			)			: vVector< F >( new F[ p.n ]		, p.n		) { for ( auto i = 0; i < p.n;		i ++ ) vVector< F >::m[ i ] = p[ i ];			}

		Vector&	operator
		=	( Vector&& p ) noexcept {
			if ( this != &p ) {
				delete[] vVector< F >::m;
				vVector< F >::m = p.m;	p.m = 0;
				vVector< F >::n = p.n;	p.n = 0;
			}
			return *this;
		}
		Vector&	operator
		=	( const Vector& p ) {
			return operator= ( (const vVector< F >&)p );
		}
		Vector&	operator
		=	( const vVector< F >& p ) {
			if ( vVector< F >::n != p.n ) {
				auto w = vVector< F >::m;
				vVector< F >::m = new F[ p.n ];
				delete[] w;
				vVector< F >::n = p.n;
			}
			for ( auto i = 0; i < p.n; i++ ) vVector< F >::m[ i ] = p[ i ];
			return *this;
		}
	};

	template	< typename F >	std::vector< F >	operator +		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Add			( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Sub			( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator *		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Mul			( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator /		( const std::vector< F >& l	, const std::vector< F >& r	) { std::vector< F > v( l.size() );		Div			( &l[ 0 ], 1, &r[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}

	template	< typename F >	std::vector< F >	operator +		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Add			( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Add			( &p[ 0 ], 1, -s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator *		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Mul			( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator /		( const std::vector< F >& p	, F s						) { std::vector< F > v( p.size() );		Div			( &p[ 0 ], 1,  s, &v[ 0 ], 1, v.size() );			return v;	}

	template	< typename F >	std::vector< F >	operator +		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		Add			( &p[ 0 ], 1, s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator -		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size(), s );	Sub			( &v[ 0 ], 1, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );	return v;	}
	template	< typename F >	std::vector< F >	operator *		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		Mul			( &p[ 0 ], 1, s, &v[ 0 ], 1, v.size() );			return v;	}
	template	< typename F >	std::vector< F >	operator /		( F s						, const std::vector< F >& p	) { std::vector< F > v( p.size() );		Div			( s, &p[ 0 ], 1, &v[ 0 ], 1, v.size() );			return v;	}

	template	< typename F >	std::vector< F >	RampArray		( size_t p					, F pInit = 0, F pStep = 1	) { std::vector< F > v( p );			Ramp		( pInit, pStep, &v[ 0 ], 1, p );					return v;	}
	template	< typename F >	F					Sum				( const std::vector< F >& p								) { F v = 0;							Sve			( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					Mean			( const std::vector< F >& p								) { F v = 0;							Mean		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					Max				( const std::vector< F >& p								) { F v = 0;							Max			( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					Min				( const std::vector< F >& p								) { F v = 0;							Min			( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					L1Norm			( const std::vector< F >& p								) { F v = 0;							Svemg		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					L2NormQ			( const std::vector< F >& p								) { F v = 0;							Svesq		( &p[ 0 ], 1, v, p.size() );						return v;	}
	template	< typename F >	F					L2Norm			( const std::vector< F >& p								) {	return sqrt( L2NormQ( p ) );																					}
	template	< typename F >	std::vector< F >	UnitVector		( const std::vector< F >& p								) {	return p / L2Norm( p );																							}
	template	< typename F >	std::vector< F >	operator -		( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Neg			( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );				return v;	}
	template	< typename F >	std::vector< F >	Abs				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Abs			( &p[ 0 ], 1, &v[ 0 ], 1, v.size() );				return v;	}
	template	< typename F >	std::vector< F >	Rec				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Rec			( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v;	}
	template	< typename F >	std::vector< F >	Exp				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Exp			( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v;	}
	template	< typename F >	std::vector< F >	Log				( const std::vector< F >& p								) { std::vector< F > v( p.size() );		Log			( &p[ 0 ], &v[ 0 ], (int)v.size() );				return v;	}
	template	< typename F >	F					Dot				( const std::vector< F >& l	, const std::vector< F >& r	) { F v = 0;							Dot			( &l[ 0 ], 1, &r[ 0 ], 1, v, l.size() );			return v;	}
	template	< typename F >	F					DistanceQ		( const std::vector< F >& l	, const std::vector< F >& r	) { F v = 0;							Distancesq	( &l[ 0 ], 1, &r[ 0 ], 1, v, l.size() );			return v;	}



	template	< typename F >	Vector< F >			operator +		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Add			( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator -		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Sub			( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator *		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Mul			( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator /		( const vVector< F >& l		, const vVector< F >& r		) { Vector< F > v( l.n );				Div			( l.m, l.s, r.m, r.s, v.m, v.s, v.n );				return v;	}

	template	< typename F >	Vector< F >			operator +		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Add			( p.m, p.s,  s, v.m, v.s, v.n );					return v;	}
	template	< typename F >	Vector< F >			operator -		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Add			( p.m, p.s, -s, v.m, v.s, v.n );					return v;	}
	template	< typename F >	Vector< F >			operator *		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Mul			( p.m, p.s,  s, v.m, v.s, v.n );					return v;	}
	template	< typename F >	Vector< F >			operator /		( const vVector< F >& p		, F s						) { Vector< F > v( p.n );				Div			( p.m, p.s,  s, v.m, v.s, v.n );					return v;	}

	template	< typename F >	Vector< F >			operator +		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				Add			( p.m, p.s, s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	Vector< F >			operator -		( F s						, const vVector< F >& p		) { Vector< F > v( p.n, s );			Sub			( v.m, v.s, p.m, p.s, v.m, v.s, v.n );				return v;	}
	template	< typename F >	Vector< F >			operator *		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				Mul			( p.m, p.s, s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	Vector< F >			operator /		( F s						, const vVector< F >& p		) { Vector< F > v( p.n );				Div			( s, p.m, p.s, v.m, v.s, v.n );						return v;	}

	template	< typename F >	Vector< F >			RampVector		( size_t p					, F pInit = 0, F pStep = 1	) { Vector< F > v( p );					Ramp		( pInit, pStep, v.m, v.s, p );						return v;	}
	template	< typename F >	F					Sum				( const vVector< F >& p									) { F v = 0;							Sve			( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					Mean			( const vVector< F >& p									) { F v = 0;							Mean		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					Max				( const vVector< F >& p									) { F v = 0;							Max			( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					Min				( const vVector< F >& p									) { F v = 0;							Min			( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					L1Norm			( const vVector< F >& p									) { F v = 0;							Svemg		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					L2NormQ			( const vVector< F >& p									) { F v = 0;							Svesq		( p.m, p.s, v, p.n );								return v;	}
	template	< typename F >	F					L2Norm			( const vVector< F >& p									) { return sqrt( L2NormQ( p ) );																					}
	template	< typename F >	Vector< F >			UnitVector		( const vVector< F >& p									) { return p / L2Norm( p );																							}
	template	< typename F >	Vector< F >			operator -		( const vVector< F >& p									) {	Vector< F > v( p );					Neg			( p.m, p.s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	Vector< F >			Abs				( const vVector< F >& p									) {	Vector< F > v( p );					Abs			( p.m, p.s, v.m, v.s, v.n );						return v;	}
	template	< typename F >	Vector< F >			Rec				( const vVector< F >& p									) {	Vector< F > v( p );					Rec			( p.m, v.m, (int)v.n );	assert( p.s == 1 );			return v;	}
	template	< typename F >	Vector< F >			Exp				( const vVector< F >& p									) {	Vector< F > v( p );					Exp			( p.m, v.m, (int)v.n );	assert( p.s == 1 );			return v;	}
	template	< typename F >	Vector< F >			Log				( const vVector< F >& p									) {	Vector< F > v( p );					Log			( p.m, v.m, (int)v.n );	assert( p.s == 1 );			return v;	}
	template	< typename F >	F					Dot				( const vVector< F >& l		, const vVector< F >& r		) { F v = 0;							Dot			( l.m, l.s, r.m, r.s, v, l.n );						return v;	}
	template	< typename F >	F					DistanceQ		( const vVector< F >& l		, const vVector< F >& r		) { F v = 0;							Distancesq	( l.m, l.s, r.m, r.s, v, l.n );						return v;	}
}
