#include	<stdio.h>

int
_U( int p, const char* file, int line ) {
	if ( p == -1 ) {
		fprintf( stderr, "%s:%d:%s\n", file, line, strerror( errno ) );
		throw 0;
	}
	return p;
}
#define	U( p ) _U( p, __FILE__, __LINE__ )

template	< typename T >	T*
_N( T* p, const char* file, int line ) {
	if ( ! p ) {
		fprintf( stderr, "%s:%d:NULL\n", file, line );
		throw 0;
	}
	return p;
}
#define	N( p ) _N( p, __FILE__, __LINE__ )

