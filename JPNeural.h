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
				Vector< F >	output;
				Vector< F >	theta;
				Matrix< F >	weight;
				Vector< F >	deltaT;
				Matrix< F >	deltaW;
				virtual	~
				vLayer() {
				}
				vLayer( size_t nN, size_t nI )
				:	output( nN )
				,	theta( Random< F >, nN )
				,	weight( Random< F >, nN, nI )
				,	deltaT( nN )
				,	deltaW( nN, nI ) {
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
				virtual	const Vector< F >
				Activate( const vVector< F >& ) = 0;
				virtual	const Vector< F >&
				Forward( const vVector< F >& p ) {
					return output = Activate( theta + Mul( weight, p ) );
				}
				virtual	const Vector< F >
				Gradient( const vVector< F >& ) = 0;
				virtual	const Vector< F >
				Backward( const vVector< F >& d, const vVector< F >& p ) {
					auto w = d * Gradient( p );
					deltaT += w;
					deltaW += MulVH( w, p );
					return Mul( w, weight );
				}
			};
			struct
			SigmoidLayer: vLayer {
				SigmoidLayer( size_t nN, size_t nI )
				:	vLayer( nN, nI ) {
				}
				const Vector< F >
				Activate( const vVector< F >& p ) {
					auto w = Exp( p );
					return w / ( F( 1 ) + w );
			//		return Rec( Exp( - p ) + F( 1 ) );
				}
				const Vector< F >
				Gradient( const vVector< F >& p ) {
					return vLayer::output * ( F( 1 ) - vLayer::output );
				}
			};
			struct
			SoftmaxLayer: vLayer {
				SoftmaxLayer( size_t nN, size_t nI )
				:	vLayer( nN, nI ) {
				}
				const Vector< F >
				Activate( const vVector< F >& p ) {
					auto v = Exp( p - Max( p ) );
					v /= Sum( v );
					return v;
				}
				const Vector< F >
				Gradient( const vVector< F >& p ) {
					return vLayer::output * ( F( 1 ) - vLayer::output );
				}
			};
			struct
			ReLULayer: vLayer {
				ReLULayer( size_t nN, size_t nI )
				:	vLayer( nN, nI ) {
				}
				const Vector< F >
				Activate( const vVector< F >& p ) {
					Vector< F >	v( p.n );
					for ( auto i = 0; i < v.n; i++ ) v[ i ] = p[ i ] < 0 ? 0 : p[ i ];
					return v;
				}
				const Vector< F >
				Gradient( const vVector< F >& p ) {
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
				layers.emplace_back( new SigmoidLayer( p, layers.size() ? layers.back()->output.n : nInput ) );
			}
			void
			NewSoftmaxLayer( size_t p ) {
				layers.emplace_back( new SoftmaxLayer( p, layers.size() ? layers.back()->output.n : nInput ) );
			}
			void
			NewReLULayer( size_t p ) {
				layers.emplace_back( new ReLULayer( p, layers.size() ? layers.back()->output.n : nInput ) );
			}
			
			const vVector< F >&
			Predict( const vVector< F >& p ) {
				const vVector< F >*	v = &p;
				for ( auto w: layers ) v = &w->Forward( *v );
				return *v;
			}

			void
			TrainMain(
				const vVector< F >&	X
			,	const vVector< F >&	A
			,	F					η									//	Learning rate
			) {
				auto	V = X;
				for ( auto w: layers ) V = w->Forward( V );
//static	int	sCounter = 0;
//if ( sCounter % 100 == 0 ) std::cerr << "Train " << sCounter << ": " << ( A - V ) << std::endl;
//sCounter++;
				auto	D = η * ( A - V );
				for ( auto w = layers.rbegin(); w != layers.rend(); w++ ) {
					auto wPrev = w + 1;
					D = (*w)->Backward( D, wPrev == layers.rend() ? X : (*wPrev)->output );
				}
			}
			
			void
			TrainPartialBatch(
				const vMatrix< F >&	Xs
			,	const vMatrix< F >&	As
			,	F					η									//	Learning rate
			,	size_t				nSample
			) {
				assert( Xs.nR == As.nR );
				Clear();
				for ( auto i = 0; i < nSample; i++ ) {
					auto wIndex = UniformRandomInt< size_t >( 0, Xs.nR );
					TrainMain( Xs.Row( wIndex ), As.Row( wIndex ), η );
				}
				Update();
			}
			void
			TrainBatch(
				const vMatrix< F >&	Xs
			,	const vMatrix< F >&	As
			,	F					η									//	Learning rate
			) {
				assert( Xs.nR == As.nR );
				Clear();
				for ( auto i = 0; i < Xs.nR; i++ ) TrainMain( Xs.Row( i ), As.Row( i ), η );
				Update();
			}
			void
			TrainPartial(
				const vMatrix< F >&	Xs
			,	const vMatrix< F >&	As
			,	F					η									//	Learning rate
			,	size_t				nSample
			) {
				assert( Xs.nR == As.nR );
				for ( auto i = 0; i < nSample; i++ ) {
					auto wIndex = UniformRandomInt< size_t >( 0, Xs.nR );
					Clear();
					TrainMain( Xs.Row( wIndex ), As.Row( wIndex ), η );
					Update();
				}
			}
			void
			Train(
				const vMatrix< F >&	Xs
			,	const vMatrix< F >&	As
			,	F					η									//	Learning rate
			) {
				assert( Xs.nR == As.nR );
				for ( auto i = 0; i < Xs.nR; i++ ) {
					Clear();
					TrainMain( Xs.Row( i ), As.Row( i ), η );
					Update();
				}
			}

			template < typename E >	void
			ForAll(
				const vMatrix< F >& Xs
			,	E	p
			) const {
				for ( auto iR = 0; iR < Xs.nR; iR++ ) {
					auto	V = Xs.Row( iR );
					for ( auto w: layers ) V = w->Forward( V );
					p( iR, V );
				}
			}
/*
			Matrix< F >
			Diff(
				const vMatrix< F >& Xs
			,	const vMatrix< F >& As
			) {
				assert( Xs.nR == As.nR );
				Matrix< F >	v( As.nR, As.nC );
				for ( auto iR = 0; iR < v.nR; iR++ ) {
					auto	V = Xs.Row( iR );
					for ( auto w: layers ) V = w->Forward( V );
					v.SetR( iR, As.Row( iR ) - V );
				}
				return v;
			}
*/
			bool
			Eval(
				const vMatrix< F >& Xs
			,	const vMatrix< F >& As
			,	F					pThreshold
			) {
				assert( Xs.nR == As.nR );
				pThreshold *= pThreshold;
				for ( auto iR = 0; iR < Xs.nR; iR++ ) {
					auto	V = Xs.Row( iR );
					for ( auto w: layers ) V = w->Forward( V );
					V = As.Row( iR ) - V;
					for ( auto iC = 0; iC < V.n; iC++ ) {
						auto w = V[ iC ];
						if ( w * w > pThreshold ) return false;
					}
				}
				return true;
			}
		};
	}
}
