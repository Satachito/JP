#include	<iostream>

void 
C( cudaError_t _, const char *const file, int const line ) {
	if ( _ ) {
		std::cerr << file << ':' << line << ':' << _ << ':' << cudaGetErrorString( _ ) << std::endl;
		throw 0;
	}
}

#define C( _ )	C( _, __FILE__, __LINE__ )

inline	cudaDeviceProp
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
		C( cudaFree( $ ) );
	}
	CUDAMemory( size_t _ )
	:	_( _ ) {
		C( cudaMallocManaged( &$, _ * sizeof( F ) ) );
	}

	void
	HtoD() {
	}
	void
	DtoH() {
	}
	F
	operator()( size_t _ ) {
		return $[ _ ];
	}

	void
	Zeroset() {
		C( cudaMemset( $, _ * sizeof( F ), 0 ) );
	}
	void
	Dump16( ) {
		auto index = size_t( 0 );
		while ( index < _ ) {
			std::cerr << '\t' << float( $[ index ] );
			index++;
			if ( index % 16 == 0 ) std::cerr << std::endl;
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
		C( cudaFreeHost( host ) );
		C( cudaFree( $ ) );
	}
	CUDAMemory( size_t _ )
	:	_( _ ) {
		C( cudaMallocHost( &host, _ * sizeof( F ) ) );
		C( cudaMalloc( &$, _ * sizeof( F ) ) );
	}

	void
	HtoD() {
		C( cudaMemcpy( $, host, _ * sizeof( F ), cudaMemcpyHostToDevice ) );
	}
	void
	DtoH() {
		C( cudaMemcpy( host, $, _ * sizeof( F ), cudaMemcpyDeviceToHost ) );
	}
	F
	operator()( size_t _ ) {
		return host[ _ ];
	}

	void
	Zeroset() {
		C( cudaMemset( $, _ * sizeof( F ), 0 ) );
	}
	void
	Dump16() {
		auto index = size_t( 0 );
		while ( index < _ ) {
			std::cerr << '\t' << float( host[ index ] );
			index++;
			if ( index % 16 == 0 ) std::cerr << std::endl;
		}
		std::cerr << std::endl;
	}
};
#endif

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
} cuRAND;


