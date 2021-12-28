#include	<iostream>

void 
C( cudaError_t p, const char *const file, int const line ) {
	if ( p ) {
		std::cerr << "CUDA error '" << p << "' (" << cudaGetErrorString( p ) << ") at :" << file << ':' << line << ':' << std::endl;
		throw 0;
	} 
}

#define C( p )	C( p, __FILE__, __LINE__ )

template < typename F > F*
MallocManaged( size_t N ) {
    F*  _;
    cudaMallocManaged( &_, N * sizeof( F ) );
    return _;
}

inline  int
maxThreadsPerBlock( int ID = 0 ) {
    cudaDeviceProp _;
    cudaGetDeviceProperties( &_, ID );
    return _.maxThreadsPerBlock;
}
