#include	"JPMath.h"

namespace JP {
	template	< typename T >	struct
	iVector {
		T*		m;
		size_t	count;
		iVector( T* p, size_t count ) : m( p ), count( count ) {}
	};

	template	< typename T >	struct
	iMatrix {
		T*		m;
		size_t	nR;
		size_t	nC;
		iMatrix( T* p, size_t nR, size_t nC )
		:	m( p )
		,	nR( nR )
		,	nC( nC ) {
		}
		const T&
		operator()( size_t pR, size_t pC ) const {
			return m[ pR * nC + pC ];
		}
		T&
		operator()( size_t pR, size_t pC ) {
			return m[ pR * nC + pC ];
		}
		iVector< T >
		operator[]( size_t pR ) const {
			return iVector< T >( (T*)&m[ pR * nC ], nC );
		}
	};
	template	< typename T >	struct
	Matrix	: iMatrix< T > {
		~
		Matrix() {
			delete[] iMatrix< T >::m;
		}
		Matrix( size_t nR, size_t nC )
		:	iMatrix< T >( new T[ nR * nC ], nR, nC ) {
		}
		Matrix( const Matrix< T >& p )
		:	iMatrix< T >( new T[ p.nR * p.nC ], p.nR, p.nC ) {
			for ( auto i = 0; i < p.nR * p.nC; i++ ) iMatrix< T >::m[ i ] = p.m[ i ];
		}
		const Matrix< T >&
		operator =( const Matrix< T >& p ) {
			delete[] iMatrix< T >::m;
			iMatrix< T >::m = new T[ p.nR * p.nC ];
			iMatrix< T >::nR = p.nR;
			iMatrix< T >::nC = p.nC;
			for ( auto i = 0; i < p.nR * p.nC; i++ ) iMatrix< T >::m[ i ] = p.m[ i ];
			return p;
		}
	};
	void
	SetR( iMatrix< float >& m, size_t pR, const iVector< float >& p ) {
		assert( m.nC == p.count );
		float w = 0;
		vDSP_vsadd( p.m, 1, &w, &m.m[ pR * m.nC ], 1, m.nC );
	}
	void
	SetC( iMatrix< float >& m, size_t pC, const iVector< float >& p ) {
		assert( m.nR == p.count );
		float w = 0;
		vDSP_vsadd( p.m, 1, &w, &m.m[ pC ], m.nC, m.nR );
	}
}
