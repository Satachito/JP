//	Written by Satoru Ogura, Fokyo.
//
#pragma once

#include	"JPMatrix.h"
#include	<iostream>
#include	<fstream>

using namespace std;

namespace JP {

	template	< typename F >	F
	Random() {
		return NormalRandom< F >();
	}
	template	< typename F >	struct
	Network {

		struct
		vLayer {
			Matrix< F >	output;
			Vector< F >	theta;
			Matrix< F >	weight;
			Vector< F >	deltaT;
			Matrix< F >	deltaW;
			virtual	~
			vLayer() {
			}
			vLayer( size_t nI, size_t nN )
			:	output( 0, 0 )
			,	theta( nN, Random )
			,	weight( nI, nN )
			,	deltaT( nN )
			,	deltaW( nI, nN ) {
			}
			const void
			Clear() {
				deltaT.Clear();
				deltaW.Clear();
			}
			const void
			Update() {
				theta += deltaT;
				weight += deltaW;
			}
			virtual	const Matrix< F >
			Activate( const vMatrix< F >& ) = 0;
			virtual	const Matrix< F >
			Forward( const vMatrix< F >& p ) {
				auto v = Dot( p, weight );
				for ( auto iR = 0; iR < v.nR; iR++ ) v.Row( iR ) += theta;
//printf( "%zu:%zu:%f\n", v.nR, v.nC, v( v.nR - 1, v.nC - 1 ) );
				v = Activate( v );
				output = v;
//std::cerr << std::endl << "w: " << weight << std::endl;
//std::cerr << "t: " << theta << std::endl;
//std::cerr << "o: " << output << std::endl;
				return v;
			}
			virtual	const Matrix< F >
			Gradient( const vMatrix< F >& ) = 0;
			virtual	const Matrix< F >
			Backward( const vMatrix< F >& d, const vMatrix< F >& p ) {
//std::cerr << "d: " << d << std::endl;
//std::cerr << "p: " << p << std::endl;
				auto wG = Gradient( p );
//std::cerr << "g: " << wG << std::endl;
				auto w = d * wG;
//std::cerr << "w: " << w << std::endl;
				for ( auto iR = 0; iR < w.nR; iR++ ) {
					auto wRow = w.Row( iR );
					deltaT += wRow;
					deltaW += Spread( p.Row( iR ), wRow );
				}
//std::cerr << "weight: " << weight << std::endl;
//				std::cerr << T( Dot( weight, T( w ) ) ) << std::endl;
				assert( weight.nC == w.nC );
				Matrix< F >	v( w.nR, weight.nR );
				for ( auto iR = 0; iR < v.nR; iR++ ) {
					auto wR = w.Row( iR );
					for ( auto iC = 0; iC < v.nC; iC++ ) v( iR, iC ) = Dot( weight.Row( iC ), wR );
				}
				return v;
			}
		};
/*
weight:
	w0	w1
	w2	w3
	w4	w5

d * wG:
	d0	d1
	d2	d3
	d4	d5
	d6	d7

T( d * wG ):
	d0	d2	d4	d6
	d1	d3	d5	d7

Dot( weight, T( d * wG ) )
	0011	0213	0415	0617
	2031	2233	2435	2637
	4051	4253	4455	4657

T( Dot( weight, T( d * wG ) ) )
	0011	2031	4051
	0213	2233	4253
	0415	2435	4455
	0617	2637	4657
*/
		struct
		SigmoidLayer: vLayer {
			SigmoidLayer( size_t nI, size_t nN )
			:	vLayer( nI, nN ) {
			}
			const Matrix< F >
			Activate( const vMatrix< F >& p ) {
        		auto v = Exp( p );
				return v / ( F( 1 ) + v );
			}
			const Matrix< F >
			Gradient( const vMatrix< F >& p ) {
				return vLayer::output * ( F( 1 ) - vLayer::output );
			}
		};
		struct
		SoftmaxLayer: vLayer {
			SoftmaxLayer( size_t nI, size_t nN )
			:	vLayer( nI, nN ) {
			}
			const Matrix< F >
			Activate( const vMatrix< F >& p ) {
				Matrix< F >	v( p.nR, p.nC );
				for ( auto iR = 0; iR < v.nR; iR++ ) {
					auto wRow = p.Row( iR );
					auto w = Exp( wRow - Max( wRow ) );
					w /= Sum( w );
					v.SetRow( iR, w );
				}
				return v;
			}
			void
			CrossEntropy( const vMatrix< F >& t ) {
				vLayer::output = -Log( vLayer::output + 1e-7 ) * t;
			}
			const Matrix< F >
			Gradient( const vMatrix< F >& p ) {
				return vLayer::output * ( F( 1 ) - vLayer::output );
			}
		};
		struct
		ReLULayer: vLayer {
			ReLULayer( size_t nI, size_t nN )
			:	vLayer( nI, nN ) {
			}
			const Matrix< F >
			Activate( const vMatrix< F >& p ) {
				Matrix< F >	v( p.nR, p.nC );
				for ( auto iR = 0; iR < v.nR; iR++ ) {
					for ( auto iC = 0; iC < v.nC; iC++ ) v( iR, iC ) = p( iR, iC ) == 0 ? 0 : p( iR, iC );
				}
				return v;
			}
			const Matrix< F >
			Gradient( const vMatrix< F >& p ) {
//					auto v = vLayer::theta + Mul( vLayer::weight, p );
//					for ( auto i = 0; i < v.n; i++ ) v[ i ] = v[ i ] < 0 ? 0 : 1;
//					return v;
				Matrix< F >	v( p.nR, p.nC );
				for ( auto iR = 0; iR < v.nR; iR++ ) {
					for ( auto iC = 0; iC < v.nC; iC++ ) v( iR, iC ) = vLayer::output( iR, iC ) == 0 ? 0 : 1;
				}
				return v;
			}
		};
		std::vector< vLayer* >	layers;
		size_t					nInput;
		size_t					nBatch;
		~
		Network() {
			for ( auto w: layers ) delete w;
		}
		Network( size_t nInput ) : nInput( nInput ) {
		}
		void
		Clear() {
			for ( auto w: layers ) w->Clear();
		}
		void
		Update() {
			for ( auto w: layers ) w->Update();
		}
		void
		NewSigmoidLayer( size_t p ) {
			layers.emplace_back( new SigmoidLayer( layers.size() ? layers.back()->theta.n : nInput, p ) );
		}
		void
		NewSoftmaxLayer( size_t p ) {
			layers.emplace_back( new SoftmaxLayer( layers.size() ? layers.back()->theta.n : nInput, p ) );
		}
		void
		NewReLULayer( size_t p ) {
			layers.emplace_back( new ReLULayer( layers.size() ? layers.back()->theta.n : nInput, p ) );
		}
		
		const Matrix< F >
		Predict( const vMatrix< F >& p ) const {
			Matrix< F >	v = p;
			for ( auto w: layers ) v = w->Forward( v );
			return v;
		}

		void
		Train(
			const vMatrix< F >&	X
		,	const vMatrix< F >&	A
		,	F					η									//	Learning rate
		) {
//static	int	sCounter = 0;
//if ( sCounter % 100 == 0 ) std::cerr << "Train " << sCounter << ": " << ( A - V ) << std::endl;
//sCounter++;
			Clear();
			auto	D = η * ( A - Predict( X ) );
			for ( auto w = layers.rbegin(); w != layers.rend(); w++ ) {
				auto wPrev = w + 1;
				D = (*w)->Backward( D, wPrev == layers.rend() ? X : (*wPrev)->output );
			}
			Update();
		}
		/*
		bool
		Eval(
			const vMatrix< F >& Xs
		,	const vMatrix< F >& As
		,	F					pThreshold
		) {
			assert( Xs.nR == As.nR );
			pThreshold *= pThreshold;
			Matrix< F > w = Xs;
			for ( auto wLayers: layers ) w = wLayers->Forward( w );
			w -= As;
			for ( auto iR = 0; iR < w.nR; iR++ ) {
				for ( auto iC = 0; iC < w.nC; iC++ ) {
					if ( pow( w( iR, iC ), 2 ) > pThreshold ) return false;
				}
			}
			return true;
		}
		*/
	};
}
