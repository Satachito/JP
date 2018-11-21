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

		template	< typename F >	Vector< F >			ReLU( const iVector< F >& p ) {
			Vector< F >	v( p.n );
			for ( auto i = 0; i < v.n; i++ ) v[ i ] = p[ i ] < 0 ? 0 : p[ i ];
			return v;
		}

		template	< typename F >	struct
		Network {

			struct
			Layer {
				Vector< F >	output;
				Vector< F >	theta;
				Matrix< F >	weight;
				Vector< F >	deltaT;
				Matrix< F >	deltaW;
				virtual	~
				Layer() {
				}
				Layer( size_t nN, size_t nI )
				:	output( nN )
				,	theta( Random< F >, nN )
				,	weight( Random< F >, nN, nI )
				,	deltaT( nN )
				,	deltaW( nN, nI ) {
				}
				virtual	const Vector< F >&
				Forward( const iVector< F >& ) = 0;
				virtual	const Vector< F >
				Backward( const iVector< F >&, const iVector< F >& ) = 0;
			};
			struct
			SigmoidLayer: Layer {
				SigmoidLayer( size_t nN, size_t nI )
				:	Layer( nN, nI ) {
				}
				Vector< F >
				Sigmoid( const iVector< F >& p ) {
					auto w = Exp( p );
					return w / ( F( 1 ) + w );
			//		return Rec( Exp( - p ) + F( 1 ) );
				}
				const Vector< F >&
				Forward( const iVector< F >& p ) {
					Layer::output = Sigmoid( Layer::theta + Mul( Layer::weight, p ) );
					return Layer::output;
				}
				const Vector< F >
				Backward( const iVector< F >& p, const iVector< F >& i ) {
					Layer::deltaT = p * Layer::output * ( F( 1 ) - Layer::output );
					Layer::deltaW = MulVH( Layer::deltaT, i );
					return Mul( Layer::deltaT, Layer::weight );
				}
			};
			struct
			ReLULayer: Layer {
				ReLULayer( size_t nN, size_t nI )
				:	Layer( nN, nI ) {
				}
				const Vector< F >&
				Forward( const iVector< F >& p ) {
					for ( auto i = 0; i < Layer::output.n; i++ ) Layer::output[ i ] = p[ i ] < 0 ? 0 : p[ i ];
					return Layer::output;
				}
				const Vector< F >
				Backward( const iVector< F >& p, const iVector< F >& i ) {
					for ( auto i = 0; i < Layer::deltaT.n; i++ ) Layer::deltaT[ i ] = p[ i ] < 0 ? 0 : 1;
					Layer::deltaW = MulVH( Layer::deltaT, i );
					return Mul( Layer::deltaT, Layer::weight );
				}
			};
			std::vector< Layer* >	layers;
			size_t					nInput;
			~
			Network() {
				for ( auto w: layers ) delete w;
			}
			Network( size_t nInput ) : nInput( nInput ) {
			}
			void
			NewSigmoidLayer( size_t p ) {
				layers.emplace_back( new SigmoidLayer( p, layers.size() ? layers.back()->output.n : nInput ) );
			}
			void
			NewReLULayer( size_t p ) {
				layers.emplace_back( new ReLULayer( p, layers.size() ? layers.back()->output.n : nInput ) );
			}

			void
			TrainMain(
				const iVector< F >&	X
			,	const iVector< F >&	A
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
				for ( auto w: layers ) {
					w->theta += w->deltaT;
					w->weight += w->deltaW;
				}
			}
			void
			Train(
				const iMatrix< F >&	Xs
			,	const iMatrix< F >&	As
			,	F					η									//	Learning rate
			,	size_t				nMiniBatch = 0
			) {
				assert( Xs.nR == As.nR );
				if ( nMiniBatch ) {
					for ( auto i = 0; i < nMiniBatch; i++ ) {
						auto wIndex = UniformRandomInt< size_t >( 0, Xs.nR );
						TrainMain( Xs.Row( wIndex ), As.Row( wIndex ), η );
					}
				} else {
					for ( auto i = 0; i < Xs.nR; i++ ) {
						TrainMain( Xs.Row( i ), As.Row( i ), η );
					}
				}
			}
			
			template < typename E >	void
			ForAll(
				const iMatrix< F >& Xs
			,	E	p
			) {
				for ( auto iR = 0; iR < Xs.nR; iR++ ) {
					auto	V = Xs.Row( iR );
					for ( auto w: layers ) V = w->Forward( V );
					p( iR, V );
				}
			}
/*
			Matrix< F >
			Diff(
				const iMatrix< F >& Xs
			,	const iMatrix< F >& As
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
				const iMatrix< F >& Xs
			,	const iMatrix< F >& As
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
