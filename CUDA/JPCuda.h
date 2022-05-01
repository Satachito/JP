#pragma once

#include	<iostream>

#include	<mma.h>

inline void 
_C( cudaError_t _, const char* file, int line ) {
	if ( _ ) {
		std::cerr << file << ':' << line << ':' << _ << ':' << cudaGetErrorString( _ ) << std::endl;
		throw 0;
	}
}

#define _C( _ )	_C( _, __FILE__, __LINE__ )

inline cudaDeviceProp
CudaDeviceProp( int ID = 0 ) {
	cudaDeviceProp _;
	cudaGetDeviceProperties( &_, ID );
	return _;
}

#ifdef	CUDA_MANAGED
template	< typename F >	struct
CUDAMemory {

	size_t	_;
	F*		$;

	~
	CUDAMemory() {
		_C( cudaFree( $ ) );
	}
	CUDAMemory( size_t _ )
	:	_( _ ) {
		_C( cudaMallocManaged( &$, _ * sizeof( F ) ) );
	}
	F*
	Host() {
		return $;
	}

	void
	HtoD() {
		_C( cudaDeviceSynchronize() );
	}
	void
	DtoH() {
		_C( cudaDeviceSynchronize() );
	}
	F
	operator()( size_t _ ) {
		return $[ _ ];
	}

	void
	Zeroset() {
		_C( cudaMemset( $, _ * sizeof( F ), 0 ) );
	}
	template < int N > void
	Dump( ) {
		auto index = size_t( 0 );
		while ( index < _ ) {
			std::cerr << '\t' << float( $[ index ] );
			if ( ++index % N == 0 ) std::cerr << std::endl;
		}
		std::cerr << std::endl;
	}
};
#endif

#ifdef	CUDA_LEGACY
template	< typename F >	struct
CUDAMemory {

	size_t	_;
	F*		$;
	F*		host;

	~
	CUDAMemory() {
		_C( cudaFreeHost( host ) );
		_C( cudaFree( $ ) );
	}
	CUDAMemory( size_t _ )
	:	_( _ ) {
		_C( cudaMallocHost( &host, _ * sizeof( F ) ) );
		_C( cudaMalloc( &$, _ * sizeof( F ) ) );
	}
	F*
	Host() {
		return host;
	}

	void
	HtoD() {
		_C( cudaMemcpy( $, host, _ * sizeof( F ), cudaMemcpyHostToDevice ) );
	}
	void
	DtoH() {
		_C( cudaMemcpy( host, $, _ * sizeof( F ), cudaMemcpyDeviceToHost ) );
	}
	F
	operator()( size_t _ ) {
		return host[ _ ];
	}

	void
	Zeroset() {
		_C( cudaMemset( $, _ * sizeof( F ), 0 ) );
	}
	template < int N > void
	Dump() {
		auto index = size_t( 0 );
		while ( index < _ ) {
			std::cerr << '\t' << float( host[ index ] );
			if ( ++index % N == 0 ) std::cerr << std::endl;
		}
		std::cerr << std::endl;
	}
};
#endif

__global__	void
DummyHalfs( half* _, size_t N ) {
	int $ = blockDim.x * blockIdx.x + threadIdx.x;
	if ( $ < N ) _[ $ ] = half( (double)$ / (double)N );
}
void
DummyData( const CUDAMemory< half >& _ ) {
	DummyHalfs<<< ( _._ + 1023 ) / 1024, 1024 >>>( _.$, _._ );
}
__global__	void
DummyFloats( float* _, size_t N ) {
	int $ = blockDim.x * blockIdx.x + threadIdx.x;
	if ( $ < N ) _[ $ ] = (double)$ / (double)N;
}
void
DummyData( const CUDAMemory< float >& _ ) {
	DummyFloats<<< ( _._ + 1023 ) / 1024, 1024 >>>( _.$, _._ );
}
__global__	void
DummyDoubles( double* _, size_t N ) {
	int $ = blockDim.x * blockIdx.x + threadIdx.x;
	if ( $ < N ) _[ $ ] = (double)$ / (double)N;
}
void
DummyData( const CUDAMemory< double >& _ ) {
	DummyDoubles<<< ( _._ + 1023 ) / 1024, 1024 >>>( _.$, _._ );
}

template < typename Fi, typename Fo >	__global__	void
ConvertFPs( Fo* out, Fi* in, size_t N ) {
	int $ = blockDim.x * blockIdx.x + threadIdx.x;
	if ( $ < N ) out[ $ ] = in[ $ ];
}

#include <curand.h>

struct
CURand {

	static void
	Check( curandStatus_t _, const char *file, int line ) {
		if ( _ != CURAND_STATUS_SUCCESS ) std::cerr << "cuRand Error: " << _ << ' ' << file << ':' << line << std::endl;
	}

	curandGenerator_t	$;
	~
	CURand() {
		Check( curandDestroyGenerator( $ ), __FILE__, __LINE__ );
	}

	CURand( curandRngType_t _ = CURAND_RNG_PSEUDO_DEFAULT ) {
		Check( curandCreateGenerator( &$, _ ), __FILE__, __LINE__ );
		Check( curandSetPseudoRandomGeneratorSeed( $, 828ULL ), __FILE__, __LINE__ );
	}
	void
	Randomize( const CUDAMemory< float >& _ ) {
		Check( curandGenerateUniform( $, _.$, _._ ), __FILE__, __LINE__ );
	}
	void
	Randomize( const CUDAMemory< double >& _ ) {
		Check( curandGenerateUniformDouble( $, _.$, _._ ), __FILE__, __LINE__ );
	}
	void
	Randomize( const CUDAMemory< half >& _ ) {
		//	TODO:	wait for curand install it.
	}
};

