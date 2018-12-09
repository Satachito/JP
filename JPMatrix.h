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
		vMatrix( F* p, size_t nR, size_t nC )
		:	m( p )
		,	nR( nR )
		,	nC( nC ) {
		}
		F
		operator()( size_t pR, size_t pC ) const {
			return m[ pR * nC + pC ];
		}
		const vVector< F >
		Row( size_t p ) const {
			return vVector< F >( (F*)&m[ p * nC ], nC );
		}
		const vVector< F >
		Col( size_t p ) const {
			return vVector< F >( (F*)&m[ p ], nR, nC );
		}
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
			for ( auto c = 0; c < p.nC; c++ ) std::cerr << '\t' << p( r, c );
			std::cerr << std::endl;
		}
		return s;
	}

	template	< typename F >	struct
	Matrix	: vMatrix< F > {
		~
		Matrix() {
			delete[] vMatrix< F >::m;
		}
		Matrix( size_t nR, size_t nC )
		:	vMatrix< F >( new F[ nR * nC ](), nR, nC ) {
		}
		Matrix( size_t nR, size_t nC, F p )
		:	vMatrix< F >( new F[ nR * nC ], nR, nC ) {
			for ( auto i = 0; i < nR * nC; i++ ) vMatrix< F >::m[ i ] = p;
		}
		Matrix( F* p, size_t nR, size_t nC )
		:	vMatrix< F >( new F[ nR * nC ], nR, nC ) {
			for ( auto i = 0; i < nR * nC; i++ ) vMatrix< F >::m[ i ] = p[ i ];
		}
		Matrix( F( *p )(), size_t nR, size_t nC )
		:	vMatrix< F >( new F[ nR * nC ], nR, nC ) {
			for ( auto i = 0; i < nR * nC; i++ ) vMatrix< F >::m[ i ] = p();
		}
		Matrix( const Matrix< F >& p )
		:	vMatrix< F >( new F[ p.nR * p.nC ], p.nR, p.nC ) {
			for ( auto i = 0; i < p.nR * p.nC; i++ ) vMatrix< F >::m[ i ] = p.m[ i ];
		}
		Matrix( const vMatrix< F >& p )
		:	vMatrix< F >( new F[ p.nR * p.nC ], p.nR, p.nC ) {
			for ( auto i = 0; i < p.nR * p.nC; i++ ) vMatrix< F >::m[ i ] = p.m[ i ];
		}
		const Matrix< F >&
		Substitution( const vMatrix< F >& p ) {
			if ( vMatrix< F >::nR != p.nR || vMatrix< F >::nC != p.nC ) {
				delete[] vMatrix< F >::m;
				vMatrix< F >::m = new F[ p.nR * p.nC ];
				vMatrix< F >::nR = p.nR;
				vMatrix< F >::nC = p.nC;
			}
			for ( auto i = 0; i < p.nR * p.nC; i++ ) vMatrix< F >::m[ i ] = p.m[ i ];
			return *this;
		}
		const Matrix< F >&
		operator =( const Matrix< F >& p ) {
			return Substitution( p );
		}
		const Matrix< F >&
		operator =( const vMatrix< F >& p ) {
			return Substitution( p );
		}
		F&
		operator()( size_t pR, size_t pC ) {
			return vMatrix< F >::m[ pR * vMatrix< F >::nC + pC ];
		}
		void
		SetRow( size_t pR, const vVector< F >& p ) {
			assert( vMatrix< F >::nC == p.n );
			_Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pR * vMatrix< F >::nC ], 1, vMatrix< F >::nC );
		}
		void
		SetCol( size_t pC, const vVector< F >& p ) {
			assert( vMatrix< F >::nR == p.n );
			_Add( p.m, p.s, F( 0 ), &vMatrix< F >::m[ pC ], vMatrix< F >::nC, vMatrix< F >::nR );
		}
		const Matrix< F >&
		operator +=( const vMatrix< F >& p ) {
			assert( vMatrix< F >::nR == p.nR && vMatrix< F >::nC == p.nC );
			_Add( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC );
			return *this;
		}
		const Matrix< F >&
		operator -=( const vMatrix< F >& p ) {
			assert( vMatrix< F >::nR == p.nR && vMatrix< F >::nC == p.nC );
			_Sub( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC );
			return *this;
		}
		const Matrix< F >&
		operator *=( const vMatrix< F >& p ) {
			assert( vMatrix< F >::nR == p.nR && vMatrix< F >::nC == p.nC );
			_Mul( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC );
			return *this;
		}
		const Matrix< F >&
		operator /=( const vMatrix< F >& p ) {
			assert( vMatrix< F >::nR == p.nR && vMatrix< F >::nC == p.nC );
			_Div( vMatrix< F >::m, 1, p.m, 1, vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC );
			return *this;
		}
		void
		Clear() {
			_Clr( vMatrix< F >::m, 1, vMatrix< F >::nR * vMatrix< F >::nC );
		}
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
	Mul( const vMatrix< F >& l, const vMatrix< F >& r ) {
		assert( l.nC == r.nR );
		Matrix< F >	v( l.nR, r.nC );
		for ( auto iR = 0; iR < l.nR; iR++ ) {
			for ( auto iC = 0; iC < r.nC; iC++ ) {
				v( iR, iC ) = Dot( l.Row( iR ), r.Col( iC ) );
			}
		}
		return v;
	}
	template	< typename F >	Matrix< F >
	MulAdd( const vMatrix< F >& l, const vMatrix< F >& r, vVector< F >& p ) {
		assert( l.nC == r.nR );
		assert( r.nC == p.n );
		Matrix< F >	v( l.nR, r.nC );
		for ( auto iR = 0; iR < l.nR; iR++ ) {
			for ( auto iC = 0; iC < r.nC; iC++ ) {
				v( iR, iC ) = Dot( l.Row( iR ), r.Col( iC ) );
			}
//			VMA( l.m + l.nC * iR, 1, r.m, 1, v.m, 1, v.m, 1, v.n );
			_Add( v.m + iR * v.nC, 1, p.m, 1, v.m + iR * v.nC, 1, v.nC );
		}
		return v;
	}
	template	< typename F >	Vector< F >
	Mul( const vMatrix< F >& l, const vVector< F >& r ) {	//	treat vector as vertical
		assert( l.nC == r.n );
		Vector< F >	v( l.nR );
		for ( auto iR = 0; iR < l.nR; iR++ ) {
			for ( auto iC = 0; iC < l.nC; iC++ ) {
				v[ iR ] += l( iR, iC ) * r[ iC ];
			}
//			VMA( l.m + l.nC * iR, 1, r.m, 1, v.m, 1, v.m, 1, v.n );
		}
		return v;
	}
	template	< typename F >	Vector< F >
	Mul( const vVector< F >& l, const vMatrix< F >& r ) {	//	treat vector as horizontal
		assert( l.n == r.nR );
		Vector< F >	v( r.nC );
		for ( auto iR = 0; iR < r.nR; iR++ ) {
			for ( auto iC = 0; iC < r.nC; iC++ ) {
				v[ iC ] += l[ iR ] * r( iR, iC );
			}
		}
		return v;
	}

	template	< typename F >	Matrix< F >
	MulVH( const vVector< F >& l, const vVector< F >& r ) {
		Matrix< F >	v( l.n, r.n );
//		for ( auto iR = 0; iR < v.nR; iR++ ) {
//			for ( auto iC = 0; iC < v.nC; iC++ ) {
//				v( iR, iC ) = l[ iR ] * r[ iC ];
//			}
//		}
		for ( auto iC = 0; iC < v.nC; iC++ ) {
			_Mul( l.m, l.s, r.m[ iC ], v.m + iC, r.n * r.s, l.n );
		}
		return v;
	}

	template	< typename F >	Vector< F >
	SumH( const vMatrix< F >& p ) {
		Vector< F >	v( p.nR );
		for ( auto iR = 0; iR < p.nR; iR++ ) v[ iR ] = Sum( p[ iR ] );
		return v;
	}

	template	< typename F >	Vector< F >
	SumV( const vMatrix< F >& p ) {
		Vector< F >	v( p.nC );
		for ( auto iC = 0; iC < p.nC; iC++ ) {
			for ( auto iR = 0; iR < p.nR; iR++ ) {
				v[ iC ] += p( iR, iC );
			}
		}
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
