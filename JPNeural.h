//	Written by Satoru Ogura, Fokyo.
//
#pragma once

#include	"JPMatrix.h"

namespace JP {

	template	< typename F >	F
	Random() {
		return UniformRandomFloat< F >( -1, 1 );
	}
	namespace Accelerate {

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
				,	theta( Random< F >, nN )
				,	weight( Random< F >, nI, nN )
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
				virtual	const Matrix< F >&
				Forward( const vMatrix< F >& p ) {
					return output = Activate( MulAdd( p, weight, theta ) );
				}
				virtual	const Matrix< F >
				Gradient( const vMatrix< F >& ) = 0;
				virtual	const Matrix< F >
				Backward( const vMatrix< F >& d, const vMatrix< F >& p ) {
					auto w = d * Gradient( p );
					for ( auto iR = 0; iR < d.nR; iR++ ) {
						auto wRow = w.Row( iR );
						deltaT += wRow;
						deltaW += MulVH( p.Row( iR ), wRow );
					}
					return Transpose( Mul( weight, Transpose( w ) ) );
				}
			};
			struct
			SigmoidLayer: vLayer {
				SigmoidLayer( size_t nI, size_t nN )
				:	vLayer( nI, nN ) {
				}
				const Matrix< F >
				Activate( const vMatrix< F >& p ) {
					Matrix< F >	v( p.nR, p.nC );
					for ( auto iR = 0; iR < v.nR; iR++ ) {
						auto wRow = p.Row( iR );
						auto w = Exp( wRow );
						v.SetRow( iR, w / ( F( 1 ) + w ) );
					}
					return v;
			//		return Rec( Exp( - p ) + F( 1 ) );
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
						w /= Sum( wRow );
						v.SetRow( iR, w );
					}
					return v;
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
					Vector< F >	v( p.n );
					for ( auto i = 0; i < v.n; i++ ) v[ i ] = p[ i ] < 0 ? 0 : p[ i ];
					return v;
				}
				const Matrix< F >
				Gradient( const vMatrix< F >& p ) {
//					auto v = vLayer::theta + Mul( vLayer::weight, p );
//					for ( auto i = 0; i < v.n; i++ ) v[ i ] = v[ i ] < 0 ? 0 : 1;
//					return v;
					Vector< F >	v( vLayer::output.n );
					for ( auto i = 0; i < v.n; i++ ) v[ i ] = vLayer::output[ i ] == 0 ? 0 : 1;
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
			
//			const vVector< F >&
//			Predict( const vVector< F >& p ) const {
//				const vVector< F >*	v = &p;
//				for ( auto w: layers ) v = &w->Forward( *v );
//				return *v;
//			}
//
			const vMatrix< F >&
			Predict( const vMatrix< F >& p ) const {
				const vMatrix< F >*	v = &p;
				for ( auto w: layers ) v = &w->Forward( *v );
				return *v;
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
			
			void
			TrainPartialBatch(
				const vMatrix< F >&	Xs
			,	const vMatrix< F >&	As
			,	F					η									//	Learning rate
			,	size_t				nSample
			) {
				assert( Xs.nR == As.nR );
				Matrix< F >	SubXs( nSample, Xs.nC );
				Matrix< F >	SubAs( nSample, As.nC );
				for ( auto i = 0; i < nSample; i++ ) {
					auto wIndex = UniformRandomInt< size_t >( 0, Xs.nR );
					SubXs.SetRow( i, Xs.Row( wIndex ) );
					SubAs.SetRow( i, As.Row( wIndex ) );
				}
				Train( SubXs, SubAs, η );
			}
			
			const Matrix< F >
			RowMatrix( const vMatrix< F >& p, size_t i ) {
				return Matrix< F >( p.Row( i ).m, 1, p.nC );
			}
			void
			TrainEachPartial(
				const vMatrix< F >&	Xs
			,	const vMatrix< F >&	As
			,	F					η									//	Learning rate
			,	size_t				nSample
			) {
				assert( Xs.nR == As.nR );
				for ( auto i = 0; i < nSample; i++ ) {
					auto wIndex = UniformRandomInt< size_t >( 0, Xs.nR );
					Train( RowMatrix( Xs, wIndex ), RowMatrix( As, wIndex ), η );
				}
			}
			void
			TrainEach(
				const vMatrix< F >&	Xs
			,	const vMatrix< F >&	As
			,	F					η									//	Learning rate
			) {
				assert( Xs.nR == As.nR );
				for ( auto i = 0; i < Xs.nR; i++ ) {
					Train( RowMatrix( Xs, i ), RowMatrix( As, i ), η );
				}
			}
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
		};
	}
}
