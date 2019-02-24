//	Written by Satoru Ogura, Tokyo.
//
#pragma	once

#include	"JPVector.h"

namespace JP {

	template	< typename F >	struct
	vMatrix {
				F*						m;
				size_t					nR;
				size_t					nC;

										vMatrix	( F* p, size_t nR, size_t nC ) : m( p ), nR( nR ), nC( nC ) {}
#ifndef	COL_ROW
				F			operator	()		( size_t pR, size_t pC	) const	{ return m[ pC * nR + pR ];								}
				F&			operator	()		( size_t pR, size_t pC	)		{ return m[ pC * nR + pR ];								}
				vVector< F >			Row		( size_t p				) const	{ return vVector< F >( &m[ p ], nC, nR	);				}
				vVector< F >			Col		( size_t p				) const	{ return vVector< F >( &m[ p * nR ], nR	);				}
#else
				F			operator	()		( size_t pR, size_t pC	) const	{ return m[ pR * nC + pC ];								}
				F&			operator	()		( size_t pR, size_t pC	)		{ return m[ pR * nC + pC ];								}
				vVector< F >			Row		( size_t p				) const	{ return vVector< F >( &m[ p * nC ], nC	);				}
				vVector< F >			Col		( size_t p				) const	{ return vVector< F >( &m[ p ], nR, nC	);				}
#endif
				void					Clear	(						) const	{ _Clr( m, 1, nR * nC );								}
		const	vMatrix&	operator	+=		( const vMatrix& p		) const	{ _Add( m, 1, p.m, 1, m, 1, nR * nC	); return *this;	}
		const	vMatrix&	operator	-=		( const vMatrix& p		) const	{ _Sub( m, 1, p.m, 1, m, 1, nR * nC	); return *this;	}
		const	vMatrix&	operator	*=		( const vMatrix& p		) const	{ _Mul( m, 1, p.m, 1, m, 1, nR * nC	); return *this;	}
		const	vMatrix&	operator	/=		( const vMatrix& p		) const	{ _Div( m, 1, p.m, 1, m, 1, nR * nC	); return *this;	}
		const	vMatrix&	operator	+=		( F p					) const	{ _Add( m, 1,  p	, m, 1, nR * nC	); return *this;	}
		const	vMatrix&	operator	-=		( F p					) const	{ _Add( m, 1, -p	, m, 1, nR * nC	); return *this;	}
		const	vMatrix&	operator	*=		( F p					) const	{ _Mul( m, 1,  p	, m, 1, nR * nC	); return *this;	}
		const	vMatrix&	operator	/=		( F p					) const	{ _Div( m, 1,  p	, m, 1, nR * nC	); return *this;	}
	};
	
	template	< typename F >	bool			operator
	==	( const vMatrix< F >& l, const vMatrix< F >& r ) {
		if ( l.nR != r.nR ) return false;
		if ( l.nC != r.nC ) return false;
		for ( auto i = 0; i < l.nR * l.nC; i++ ) if ( l.m[ i ] != r.m[ i ] ) return false;
		return true;
	}
	template	< typename F >	bool			operator
	!=	( const vMatrix< F >& l, const vMatrix< F >& r ) {
		return !( l == r );
	}

	template	< typename F >	std::ostream&	operator
	<<	( std::ostream& s, const vMatrix< F >& p ) {
		for ( auto r = 0; r < p.nR; r++ ) {
			for ( auto c = 0; c < p.nC; c++ ) s << '\t' << p( r, c );
			s << std::endl;
		}
		return s;
	}

	template	< typename F >	struct
	Matrix	: vMatrix< F > {
	~	Matrix() { delete[] vMatrix< F >::m; }
		
		Matrix(									)			: vMatrix< F >( 0					, 0		, 0		) {																				}
		Matrix( size_t nR, size_t nC			)			: vMatrix< F >( new F[ nR * nC ]()	, nR	, nC	) {																				}
		Matrix( size_t nR, size_t nC, F p		)			: vMatrix< F >( new F[ nR * nC ]	, nR	, nC	) { for ( auto i = 0; i < nR * nC;		i++ ) vMatrix< F >::m[ i ] = p;			}
		Matrix( size_t nR, size_t nC, F* p	 	)			: vMatrix< F >( new F[ nR * nC ]	, nR	, nC	) { for ( auto i = 0; i < nR * nC;		i++ ) vMatrix< F >::m[ i ] = p[ i ];	}
		Matrix( size_t nR, size_t nC, F( *p )() )			: vMatrix< F >( new F[ nR * nC ]	, nR	, nC	) { for ( auto i = 0; i < nR * nC;		i++ ) vMatrix< F >::m[ i ] = p();		}

		Matrix( size_t nR, size_t nC, std::initializer_list< F > p )
		:	vMatrix< F >( new F[ nR * nC ], nR, nC ) {
			assert( nR * nC == p.size() );
			for ( auto i = 0; i < p.size();	i++ ) vMatrix< F >::m[ i ] = p.begin()[ i ];
		}

		Matrix( Matrix&& p						) noexcept	: vMatrix< F >( p.m					, p.nR	, p.nC	) { p.m = 0; p.nR = 0; p.nC = 0;												}
		Matrix( const Matrix& p					)			: vMatrix< F >( new F[ p.nR * p.nC ], p.nR	, p.nC	) { for ( auto i = 0; i < p.nR * p.nC;	i++ ) vMatrix< F >::m[ i ] = p.m[ i ];	}
		Matrix( const vMatrix< F >& p			)			: vMatrix< F >( new F[ p.nR * p.nC ], p.nR	, p.nC	) { for ( auto i = 0; i < p.nR * p.nC;	i++ ) vMatrix< F >::m[ i ] = p.m[ i ];	}

		Matrix&	operator
		=	( Matrix&& p ) noexcept {
			if ( this != &p ) {
				delete[] vMatrix< F >::m;
				vMatrix< F >::m = p.m;		p.m = 0;
				vMatrix< F >::nR = p.nR;	p.nR = 0;
				vMatrix< F >::nC = p.nC;	p.nC = 0;
			}
			return *this;
		}
		Matrix&	operator
		=	( const Matrix& p ) {
			return operator= ( (const vMatrix< F >&)p );
		}
		Matrix&	operator
		=	( const vMatrix< F >& p ) {
			auto	wSize = p.nR * p.nC;
			if ( vMatrix< F >::nR * vMatrix< F >::nC != wSize ) {
				auto w = vMatrix< F >::m;
				vMatrix< F >::m = new F[ wSize ];
				delete[] w;
				vMatrix< F >::nR = p.nR;
				vMatrix< F >::nC = p.nC;
			}
			for ( auto i = 0; i < wSize; i++ ) vMatrix< F >::m[ i ] = p.m[ i ];
			return *this;
		}
#ifndef	COL_ROW
		void	SetRow		( size_t pR, const vVector< F >& p	) { _Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pR ]						, vMatrix< F >::nR	, vMatrix< F >::nC ); }
		void	SetCol		( size_t pC, const vVector< F >& p	) { _Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pC * vMatrix< F >::nR ]	, 1					, vMatrix< F >::nR ); }
#else
		void	SetRow		( size_t pR, const vVector< F >& p	) { _Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pR * vMatrix< F >::nC ]	, 1					, vMatrix< F >::nC ); }
		void	SetCol		( size_t pC, const vVector< F >& p	) { _Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pC ]						, vMatrix< F >::nC	, vMatrix< F >::nR ); }
#endif
	};
	
	template	< typename F >	Matrix< F >
	IdentityMatrix( size_t p ) {
		Matrix< F >	v( p, p );
		for ( auto i = 0; i < p; i++ ) v( i, i ) = 1;
		return v;
	}

	template	< typename F >	Matrix< F >		operator
	~( const vMatrix< F >& p ) {
		Matrix< F > v( p.nC, p.nR );
#ifndef	COL_ROW
		_Trans( p.m, 1, v.m, 1, v.nC, v.nR );
#else
		_Trans( p.m, 1, v.m, 1, v.nR, v.nC );
#endif
		return v;
	}

	template	< typename F >	Matrix< F >			operator
	-( const vMatrix< F >& p ) {
		Matrix< F > v( p );
		_Neg( p.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}

	template	< typename F >	Matrix< F >
	Dot( const vMatrix< F >& l, const vMatrix< F >& r ) {
		assert( l.nC == r.nR );
		Matrix< F >	v( l.nR, r.nC );
		for ( auto iR = 0; iR < v.nR; iR++ ) {
			auto wRow = l.Row( iR );
			for ( auto iC = 0; iC < v.nC; iC++ ) v( iR, iC ) = Dot( wRow, r.Col( iC ) );
		}
		return v;
	}
	template	< typename F >	Vector< F >
	Dot( const vMatrix< F >& l, const vVector< F >& r ) {	//	treat vector as vertical
		assert( l.nC == r.n );
		Vector< F >	v( l.nR );
		for ( auto i = 0; i < v.n; i++ ) v[ i ] = Dot( l.Row( i ), r );
		return v;
	}
	template	< typename F >	Vector< F >
	Dot( const vVector< F >& l, const vMatrix< F >& r ) {	//	treat vector as horizontal
		assert( l.n == r.nR );
		Vector< F >	v( r.nC );
		for ( auto i = 0; i < v.n; i++ ) v[ i ] += Dot( l, r.Col( i ) );
		return v;
	}

	template	< typename F >	Matrix< F >
	Spread( const vVector< F >& l, const vVector< F >& r ) {
		Matrix< F >	v( l.n, r.n );
		for ( auto iR = 0; iR < v.nR; iR++ ) _Mul( r.m, r.s, l.m[ iR ], v.m + iR * v.nC, 1, v.nC );
		return v;
	}

	template	< typename F >	Vector< F >
	ForRow( const vMatrix< F >& p, F ( *f )( const vVector< F >& ) ) {
		Vector< F >	v( p.nR );
		for ( auto iR = 0; iR < v.n; iR++ ) v[ iR ] = f( p.Row( iR ) );
		return v;
	}
	template	< typename F >	Vector< F >
	ForCol( const vMatrix< F >& p, F ( *f )( const vVector< F >& ) ) {
		Vector< F >	v( p.nC );
		for ( auto iC = 0; iC < v.n; iC++ ) v[ iC ] = f( p.Col( iC ) );
		return v;
	}
#ifndef	COL_ROW
	template	< typename F >	Matrix< F >
	AddRow( const vMatrix< F >& l, const vVector< F >& r ) {
		assert( l.nC == r.n );
		Matrix< F >	v( l.nR + 1, l.nC );
		for ( auto i = 0; i < l.nR * l.nC; i++ ) v.m[ i ] = l.m[ i ];
		for ( auto i = 0; i < r.n; i++ ) v.m[ l.nR * l.nC + i ] = r.m[ i ];
		return v;
	}
#else
	template	< typename F >	Matrix< F >
	AddCol( const vMatrix< F >& l, const vVector< F >& r ) {
		assert( l.nR == r.n );
		Matrix< F >	v( l.nR, l.nC + 1 );
		for ( auto i = 0; i < l.nR * l.nC; i++ ) v.m[ i ] = l.m[ i ];
		for ( auto i = 0; i < r.n; i++ ) v.m[ l.nR * l.nC + i ] = r.m[ i ];
		return v;
	}
#endif
	template	< typename F >	Matrix< F >
	operator +( const vMatrix< F >& l, const vMatrix< F >& r ) {
		assert( l.nR == r.nR && l.nC == r.nC );
		Matrix< F >	v( l.nR, l.nC );
		_Add( l.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator +( const vMatrix< F >& l, F r ) {
		Matrix< F >	v( l.nR, l.nC, r );
		_Add( l.m, 1, v.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator +( F l, const vMatrix< F >& r ) {
		Matrix< F >	v( r.nR, r.nC, l );
		_Add( v.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}

	template	< typename F >	Matrix< F >
	operator -( const vMatrix< F >& l, const vMatrix< F >& r ) {
		assert( l.nR == r.nR && l.nC == r.nC );
		Matrix< F >	v( l.nR, l.nC );
		_Sub( l.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator -( const vMatrix< F >& l, F r ) {
		Matrix< F >	v( l.nR, l.nC, r );
		_Sub( l.m, 1, v.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator -( F l, const vMatrix< F >& r ) {
		Matrix< F >	v( r.nR, r.nC, l );
		_Sub( v.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}

	template	< typename F >	Matrix< F >
	operator *( const vMatrix< F >& l, const vMatrix< F >& r ) {
		assert( l.nR == r.nR && l.nC == r.nC );
		Matrix< F >	v( l.nR, l.nC );
		_Mul( l.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator *( const vMatrix< F >& l, F r ) {
		Matrix< F >	v( l.nR, l.nC, r );
		_Mul( l.m, 1, v.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator *( F l, const vMatrix< F >& r ) {
		Matrix< F >	v( r.nR, r.nC, l );
		_Mul( v.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}

	template	< typename F >	Matrix< F >
	operator /( const vMatrix< F >& l, const vMatrix< F >& r ) {
		assert( l.nR == r.nR && l.nC == r.nC );
		Matrix< F >	v( l.nR, l.nC );
		_Div( l.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator /( const vMatrix< F >& l, F r ) {
		Matrix< F >	v( l.nR, l.nC, r );
		_Div( l.m, 1, v.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}
	template	< typename F >	Matrix< F >
	operator /( F l, const vMatrix< F >& r ) {
		Matrix< F >	v( r.nR, r.nC, l );
		_Div( v.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
		return v;
	}

	template	< typename F >	Matrix< F >
	Exp( const vMatrix< F >& p ) {
		Matrix< F > v( p );
		_Exp( p.m, v.m, (int)( v.nR * v.nC ) );
		return v;
	}
}
