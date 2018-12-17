//	Written by Satoru Ogura, Tokyo.
//
#pragma	once

#include	"JPVector.h"

namespace JP {

	template	< typename F >	struct
	vMatrix {
		F*		m;
		size_t	nR;
		size_t	nC;

		vMatrix( F* p, size_t nR, size_t nC ) : m( p ), nR( nR ), nC( nC ) {}

		F		operator		()	( size_t pR, size_t pC	) const { return m[ pR * nC + pC ];						}
		const	vVector< F >	Row	( size_t p				) const { return vVector< F >( (F*)&m[ p * nC ], nC );	}
		const	vVector< F >	Col	( size_t p				) const { return vVector< F >( (F*)&m[ p ], nR, nC );	}
	};
	
	template	< typename F >	bool
	operator ==	( const vMatrix< F >& l, const vMatrix< F >& r ) {
		if ( l.nR != r.nR ) return false;
		if ( l.nC != r.nC ) return false;
		for ( auto i = 0; i < l.nR * l.nC; i++ ) if ( l.m[ i ] != r.m[ i ] ) return false;
		return true;
	}
	template	< typename F >	bool
	operator !=	( const vMatrix< F >& l, const vMatrix< F >& r ) {
		return !( l == r );
	}

	template	< typename F >	std::ostream&
	operator << ( std::ostream& s, const vMatrix< F >& p ) {
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
		Matrix( Matrix&& p						) noexcept	: vMatrix< F >( p.m					, p.nR	, p.nC	) { p.m = 0; p.nR = 0; p.nC = 0;												}
		Matrix( const Matrix& p					)			: vMatrix< F >( new F[ p.nR * p.nC ], p.nR	, p.nC	) { for ( auto i = 0; i < p.nR * p.nC;	i++ ) vMatrix< F >::m[ i ] = p.m[ i ];	}
		Matrix( const vMatrix< F >& p			)			: vMatrix< F >( new F[ p.nR * p.nC ], p.nR	, p.nC	) { for ( auto i = 0; i < p.nR * p.nC;	i++ ) vMatrix< F >::m[ i ] = p.m[ i ];	}

		Matrix&
		_Substitution( const vMatrix< F >& p ) {
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
		Matrix&	operator =	( Matrix&& p ) noexcept {
			if ( this != &p ) {
				delete[] vVector< F >::m;
				vVector< F >::m = p.m;		p.m = 0;
				vVector< F >::nR = p.nR;	p.nR = 0;
				vVector< F >::nC = p.nC;	p.nC = 0;
			}
			return *this;
		}
		Matrix&	operator =	( const Matrix& p					) { return _Substitution( p );																						}
		Matrix&	operator =	( const vMatrix< F >& p				) { return _Substitution( p );																						}
		F&		operator ()	( size_t pR, size_t pC				) { return vMatrix< F >::m[ pR * vMatrix< F >::nC + pC ];															}
		void	Clear		(									) { _Clr( vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC );												}
		Matrix&	operator +=	( const vMatrix< F >& p				) { _Add( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		Matrix&	operator -=	( const vMatrix< F >& p				) { _Sub( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		Matrix&	operator *=	( const vMatrix< F >& p				) { _Mul( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		Matrix&	operator /=	( const vMatrix< F >& p				) { _Div( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		Matrix&	operator +=	( F p								) { _Add( vMatrix< F >::m, 1, p		, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		Matrix&	operator -=	( F p								) { _Add( vMatrix< F >::m, 1, -p	, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		Matrix&	operator *=	( F p								) { _Mul( vMatrix< F >::m, 1, p		, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		Matrix&	operator /=	( F p								) { _Div( vMatrix< F >::m, 1, p		, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC	); return *this;	}
		void	SetRow		( size_t pR, const vVector< F >& p	) { _Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pR * vMatrix< F >::nC ], 1, vMatrix< F >::nC		);					}
		void	SetCol		( size_t pC, const vVector< F >& p	) { _Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pC ], vMatrix< F >::nC, vMatrix< F >::nR			);					}
	};
	
	template	< typename F >	Matrix< F >
	IdentityMatrix( size_t p ) {
		Matrix< F >	v( p, p );
		for ( auto i = 0; i < p; i++ ) v( i, i ) = 1;
		return v;
	}

	template	< typename F >	Matrix< F >
	T( const vMatrix< F >& p ) {
		Matrix< F > v( p.nC, p.nR );
		_Trans( p.m, 1, v.m, 1, v.nR, v.nC );
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
	template	< typename F >	Matrix< F >
	DotAdd( const vMatrix< F >& l, const vMatrix< F >& r, vVector< F >& p ) {
		assert( l.nC == r.nR );
		assert( r.nC == p.n );
		Matrix< F >	v( l.nR, r.nC );
		for ( auto iR = 0; iR < v.nR; iR++ ) {
			auto wRow = l.Row( iR );
			for ( auto iC = 0; iC < v.nC; iC++ ) v( iR, iC ) = Dot( wRow, r.Col( iC ) );
			_Add( v.m + iR * v.nC, 1, p.m, 1, v.m + iR * v.nC, 1, v.nC );
		}
		return v;
	}
	template	< typename F >	Vector< F >
	Dot( const vMatrix< F >& l, const vVector< F >& r ) {	//	treat vector as vertical
		assert( l.nC == r.n );
		Vector< F >	v( l.nR );
		for ( auto iR = 0; iR < v.nR; iR++ ) v[ iR ] = Dot( l.Row( iR ), r );
		return v;
	}
	template	< typename F >	Vector< F >
	Dot( const vVector< F >& l, const vMatrix< F >& r ) {	//	treat vector as horizontal
		assert( l.n == r.nR );
		Vector< F >	v( r.nC );
		for ( auto iC = 0; iC < v.nC; iC++ ) v[ iC ] += Dot( l, r.Col( iC ) );
		return v;
	}

	template	< typename F >	Matrix< F >
	Spread( const vVector< F >& l, const vVector< F >& r ) {
		Matrix< F >	v( l.n, r.n );
		for ( auto iR = 0; iR < v.nR; iR++ ) _Mul( r.m, r.s, l.m[ iR ], v.m + iR * v.nC, 1, v.nC );
		return v;
	}

	template	< typename F >	Vector< F >
	H( const vMatrix< F >& p, F ( *f )( const vVector< F >& ) ) {
		Vector< F >	v( p.nR );
		for ( auto iR = 0; iR < v.n; iR++ ) v[ iR ] = f( p.Row( iR ) );
		return v;
	}
	template	< typename F >	Vector< F >
	V( const vMatrix< F >& p, F ( *f )( const vVector< F >& ) ) {
		Vector< F >	v( p.nC );
		for ( auto iC = 0; iC < v.n; iC++ ) v[ iC ] = f( p.Col( iC ) );
		return v;
	}

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
