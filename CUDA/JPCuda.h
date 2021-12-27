#include	<iostream>

void 
C( cudaError_t p, const char *const file, int const line ) {
	if ( p ) {
		std::cerr << "CUDA error '" << p << "' (" << cudaGetErrorString( p ) << ") at :" << file << ':' << line << ':' << std::endl;
		throw 0;
	} 
}

#define C( p )	C( p, __FILE__, __LINE__ )

