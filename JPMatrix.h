#include	"JPMath.h"

template	< typename T >	struct
matrix {
	vector< T >		m;
	size_t	m0;
	size_t	m1;
	matrix( size_t p0, size_t p1, const T& p = 0 )
	:	m( p0 * p1, p )
	,	m0( p0 )
	,	m1( p1 ) {
	}
	const T&
	operator()( size_t p0, size_t p1 ) const {
		return m[ p0 * m1 + p1 ];
	}
	T&
	operator()( size_t p0, size_t p1 ) {
		return m[ p0 * m1 + p1 ];
	}
	vector< T >
	operator[]( size_t p ) const {
		vector< T >	v( m1, 0 );
		for ( auto i1 = 0; i1 < m1; i1++ ) v[ i1 ] = operator()( p, i1 );
		return v;
	}
};

void
Set0( matrix< float > m, size_t i0, const vector< float >& p ) {
	float w = 0;
	vDSP_vsadd( &p[ 0 ], 1, &w, &m.m[ i0 * m.m1 ], 1, m.m1 );
}
void
Set1( matrix< float > m, size_t i1, const vector< float >& p ) {
	float w = 0;
	vDSP_vsadd( &p[ 0 ], 1, &w, &m.m[ i1 ], m.m0, m.m1 );
}

