//	Written by Satoru Ogura, Tokyo.
//
#pragma	once

#include	"JPVector.h"

namespace JP {

	template	< typename F >	struct
	iMatrix {
		F*		m;
		size_t	nR;
		size_t	nC;
		iMatrix( F* p, size_t nR, size_t nC )
		:	m( p )
		,	nR( nR )
		,	nC( nC ) {
		}
		F
		operator()( size_t pR, size_t pC ) const {
			return m[ pR * nC + pC ];
		}
		const iVector< F >
		Row( size_t p ) const {
			return iVector< F >( (F*)&m[ p * nC ], nC );
		}
		const iVector< F >
		Col( size_t p ) const {
			return iVector< F >( (F*)&m[ p ], nR, nC );
		}
	};
	
	template	< typename F >	std::ostream&
	operator << ( std::ostream& s, const iMatrix< F >& p ) {
		for ( auto r = 0; r < p.nR; r++ ) {
			for ( auto c = 0; c < p.nC; c++ ) std::cerr << '\t' << p( r, c );
			std::cerr << std::endl;
		}
		return s;
	}

	namespace Accelerate {
		template	< typename F >	struct
		Matrix	: iMatrix< F > {
			~
			Matrix() {
				delete[] iMatrix< F >::m;
			}
			Matrix( size_t nR, size_t nC )
			:	iMatrix< F >( new F[ nR * nC ](), nR, nC ) {
			}
			Matrix( F* p, size_t nR, size_t nC )
			:	iMatrix< F >( new F[ nR * nC ], nR, nC ) {
				for ( auto i = 0; i < nR * nC; i++ ) iMatrix< F >::m[ i ] = p[ i ];
			}
			Matrix( F p, size_t nR, size_t nC )
			:	iMatrix< F >( new F[ nR * nC ], nR, nC ) {
				for ( auto i = 0; i < nR * nC; i++ ) iMatrix< F >::m[ i ] = p;
			}
			Matrix( F( *p )(), size_t nR, size_t nC )
			:	iMatrix< F >( new F[ nR * nC ], nR, nC ) {
				for ( auto i = 0; i < nR * nC; i++ ) iMatrix< F >::m[ i ] = p();
			}
			Matrix( const Matrix< F >& p )
			:	iMatrix< F >( new F[ p.nR * p.nC ], p.nR, p.nC ) {
				for ( auto i = 0; i < p.nR * p.nC; i++ ) iMatrix< F >::m[ i ] = p.m[ i ];
			}
			Matrix( const iMatrix< F >& p )
			:	iMatrix< F >( new F[ p.nR * p.nC ], p.nR, p.nC ) {
				for ( auto i = 0; i < p.nR * p.nC; i++ ) iMatrix< F >::m[ i ] = p.m[ i ];
			}
			const Matrix< F >&
			operator =( const Matrix< F >& p ) {
				delete[] iMatrix< F >::m;
				iMatrix< F >::m = new F[ p.nR * p.nC ];
				iMatrix< F >::nR = p.nR;
				iMatrix< F >::nC = p.nC;
				for ( auto i = 0; i < p.nR * p.nC; i++ ) iMatrix< F >::m[ i ] = p.m[ i ];
				return *this;
			}
			const Matrix< F >&
			operator =( const iMatrix< F >& p ) {
				delete[] iMatrix< F >::m;
				iMatrix< F >::m = new F[ p.nR * p.nC ];
				iMatrix< F >::nR = p.nR;
				iMatrix< F >::nC = p.nC;
				for ( auto i = 0; i < p.nR * p.nC; i++ ) iMatrix< F >::m[ i ] = p.m[ i ];
				return *this;
			}
			F&
			operator()( size_t pR, size_t pC ) {
				return iMatrix< F >::m[ pR * iMatrix< F >::nC + pC ];
			}
			inline	void	AddS( const float* l, vDSP_Stride sL, const float& r, float* v, vDSP_Stride sV, vDSP_Length pLength )		{ vDSP_vsadd		( l, sL, &r, v, sV, pLength ); }
			inline	void	AddS( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vsaddD		( l, sL, &r, v, sV, pLength ); }
			void
			SetR( size_t pR, const iVector< F >& p ) {
				assert( iMatrix< F >::nC == p.n );
//				for ( auto i = 0; i < p.n; i++ ) iMatrix< F >::m[ pR * iMatrix< F >::nC + i ] = p[ i ];
				AddS( p.m, p.s, 0, &iMatrix< F >::m[ pR * iMatrix< F >::nC ], 1, iMatrix< F >::nC );
			}
			void
			SetC( size_t pC, const iVector< F >& p ) {
				assert( iMatrix< F >::nR == p.n );
				AddS( p.m, p.s, 0, &iMatrix< F >::m[ pC ], iMatrix< F >::nC, iMatrix< F >::nR );
			}
			const Matrix< F >&
			operator +=( const iMatrix< F >& p ) {
				assert( iMatrix< F >::nR == p.nR && iMatrix< F >::nC == p.nC );
				Add( iMatrix< F >::m, 1, p.m, 1, iMatrix< F >::m, 1, iMatrix< F >::nR * iMatrix< F >::nC );
				return *this;
			}
			const Matrix< F >&
			operator -=( const iMatrix< F >& p ) {
				assert( iMatrix< F >::nR == p.nR && iMatrix< F >::nC == p.nC );
				Sub( iMatrix< F >::m, 1, p.m, 1, iMatrix< F >::m, 1, iMatrix< F >::nR * iMatrix< F >::nC );
				return *this;
			}
			const Matrix< F >&
			operator *=( const iMatrix< F >& p ) {
				assert( iMatrix< F >::nR == p.nR && iMatrix< F >::nC == p.nC );
				Mul( iMatrix< F >::m, 1, p.m, 1, iMatrix< F >::m, 1, iMatrix< F >::nR * iMatrix< F >::nC );
				return *this;
			}
			const Matrix< F >&
			operator /=( const iMatrix< F >& p ) {
				assert( iMatrix< F >::nR == p.nR && iMatrix< F >::nC == p.nC );
				Div( iMatrix< F >::m, 1, p.m, 1, iMatrix< F >::m, 1, iMatrix< F >::nR * iMatrix< F >::nC );
				return *this;
			}
		};
	
		inline	void
		VMA( const float* l, vDSP_Stride sL, const float* p, vDSP_Stride sP, const float* r, vDSP_Stride sR, float* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vma( l, sL, p, sP, r, sR, v, sV, pLength );
		}
		inline	void
		VMA( const double* l, vDSP_Stride sL, const double* p, vDSP_Stride sP, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength ) {
			vDSP_vmaD( l, sL, p, sP, r, sR, v, sV, pLength );
		}
		template	< typename F >	Vector< F >
		Mul( const iMatrix< F >& l, const iVector< F >& r ) {	//	treat l as vertical
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
		Mul( const iVector< F >& l, const iMatrix< F >& r ) {	//	treat l as horizontal
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
		MulVH( const iVector< F >& l, const iVector< F >& r ) {
			Matrix< F >	v( l.n, r.n );
	//		for ( auto iR = 0; iR < v.nR; iR++ ) {
	//			for ( auto iC = 0; iC < v.nC; iC++ ) {
	//				v( iR, iC ) = l[ iR ] * r[ iC ];
	//			}
	//		}
			for ( auto iC = 0; iC < v.nC; iC++ ) {
				Mul( l.m, l.s, r.m[ iC ], v.m + iC, r.n * r.s, l.n );
			}
			return v;
		}

		template	< typename F >	Vector< F >
		SumH( const iMatrix< F >& p ) {
			Vector< F >	v( p.nR );
			for ( auto iR = 0; iR < p.nR; iR++ ) v[ iR ] = Sum( p[ iR ] );
			return v;
		}

		template	< typename F >	Vector< F >
		SumV( const iMatrix< F >& p ) {
			Vector< F >	v( p.nC );
			for ( auto iC = 0; iC < p.nC; iC++ ) {
				for ( auto iR = 0; iR < p.nR; iR++ ) {
					v[ iC ] += p( iR, iC );
				}
			}
			return v;
		}

		template	< typename F >	Matrix< F >
		operator +( const iMatrix< F >& l, const iMatrix< F >& r ) {
			assert( l.nR == r.nR && l.nC == r.nC );
			Matrix< F >	v( l.nR, l.nC );
			Add( l.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}
		template	< typename F >	Matrix< F >
		operator +( const iMatrix< F >& l, F r ) {
			Matrix< F >	v( l.nR, l.nC, r );
			Add( l.m, 1, v.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}
		template	< typename F >	Matrix< F >
		operator +( F l, const iMatrix< F >& r ) {
			Matrix< F >	v( r.nR, r.nC, l );
			Add( v.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}

		template	< typename F >	Matrix< F >
		operator -( const iMatrix< F >& l, const iMatrix< F >& r ) {
			assert( l.nR == r.nR && l.nC == r.nC );
			Matrix< F >	v( l.nR, l.nC );
			Sub( l.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}
		template	< typename F >	Matrix< F >
		operator -( const iMatrix< F >& l, F r ) {
			Matrix< F >	v( l.nR, l.nC, r );
			Sub( l.m, 1, v.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}
		template	< typename F >	Matrix< F >
		operator -( F l, const iMatrix< F >& r ) {
			Matrix< F >	v( r.nR, r.nC, l );
			Sub( v.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}

		template	< typename F >	Matrix< F >
		operator *( const iMatrix< F >& l, const iMatrix< F >& r ) {
			assert( l.nR == r.nR && l.nC == r.nC );
			Matrix< F >	v( l.nR, l.nC );
			Mul( l.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}
		template	< typename F >	Matrix< F >
		operator *( F l, const iMatrix< F >& r ) {
			Matrix< F >	v( r.nR, r.nC, l );
			Mul( v.m, 1, r.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}
		template	< typename F >	Matrix< F >
		operator *( const iMatrix< F >& l, F r ) {
			Matrix< F >	v( l.nR, l.nC, r );
			Mul( l.m, 1, v.m, 1, v.m, 1, v.nR * v.nC );
			return v;
		}
	}
}
