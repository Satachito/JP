//	Written by Satoru Ogura, Tokyo.
//
#pragma once

#include	<cassert>
#include	<iostream>

#include	<random>
namespace JP {
	template	< typename F >	F
	UniformRandomFloat( F l = 0, F h = 1 ) {
		static	std::mt19937_64 sMT( (std::random_device())() );
		return	std::uniform_real_distribution< F >( l, h )( sMT );
	}
	template	< typename F >	F
	NormalRandom( F l = 0, F h = 1 ) {
		static	std::mt19937_64 sMT( (std::random_device())() );
		return	std::normal_distribution< F >( l, h )( sMT );
	}
}

#include	<cmath>

#ifdef	JP_USE_CPU
namespace JP {

	template < typename F >	void	Clr			( F* p, long sP, unsigned long pLength )																{ for ( auto i = 0; i < pLength; i++ ) p[ i * sP ] = 0; }
	template < typename F >	void	Ramp		( F pInit, F pStep, F* p, long sP, unsigned long pLength )												{ for ( auto i = 0; i < pLength; i++ ) p[ i * sP ] = pInit + pStep * i; }
	template < typename F >	void	Sve			( const F* p, long sP, F& v, unsigned long pLength )													{ v = 0; for ( auto i = 0; i < pLength; i++ ) v += p[ i * sP ]; }
	template < typename F >	void	Mean		( const F* p, long sP, F& v, unsigned long pLength )													{ v = 0; for ( auto i = 0; i < pLength; i++ ) v += p[ i * sP ]; v /= pLength; }
	template < typename F >	void	Max			( const F* p, long sP, F& v, unsigned long pLength )													{ v = *p; for ( auto i = 1; i < pLength; i++ ) if ( p[ i * sP ] > v ) v = p[ i * sP ]; }
	template < typename F >	void	Min			( const F* p, long sP, F& v, unsigned long pLength )													{ v = *p; for ( auto i = 1; i < pLength; i++ ) if ( p[ i * sP ] < v ) v = p[ i * sP ]; }
	template < typename F >	void	Svemg		( const F* p, long sP, F& v, unsigned long pLength )													{ v = 0; for ( auto i = 0; i < pLength; i++ ) v += p[ i * sP ] < 0 ? -p[ i * sP ] : p[ i * sP ] ; }
	template < typename F >	void	Svesq		( const F* p, long sP, F& v, unsigned long pLength )													{ v = 0; for ( auto i = 0; i < pLength; i++ ) v += p[ i * sP ] * p[ i * sP ]; }
	template < typename F >	void	Neg			( const F* p, long sP, F* v, long sV, unsigned long pLength )											{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = -p[ i * sP ]; }
	template < typename F >	void	Abs			( const F* p, long sP, F* v, long sV, unsigned long pLength )											{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = p[ i * sP ] < 0 ? -p[ i * sP ] : p[ i * sP ]; }
	template < typename F >	void	Rec			( const F* p, F* v, int pLength )																		{ for ( auto i = 0; i < pLength; i++ ) v[ i ] = 1 / p[ i ]; }
	template < typename F >	void	Exp			( const F* p, F* v, int pLength )																		{ for ( auto i = 0; i < pLength; i++ ) v[ i ] = exp( p[ i ] ); }
	template < typename F >	void	Log			( const F* p, F* v, int pLength )																		{ for ( auto i = 0; i < pLength; i++ ) v[ i ] = log( p[ i ] ); }
	template < typename F >	void	Dot			( const F* l, long sL, const F* r, long sR, F& v, unsigned long pLength )								{ v = 0; for ( auto i = 0; i < pLength; i++ ) v += l[ i * sL ] * r[ i * sR ]; }
	template < typename F >	void	Distancesq	( const F* l, long sL, const F* r, long sR, F& v, unsigned long pLength )								{ v = 0; for ( auto i = 0; i < pLength; i++ ) { auto w = l[ i * sL ] - r[ i * sR ]; v += w * w; } }
	template < typename F >	void	Trans		( const F* p, long sP, F* v, long sV, unsigned long nR, unsigned long nC )								{ for ( auto iR = 0; iR < nR; iR++ ) for ( auto iC = 0; iC < nC; iC++ ) v[ iR * 3 + iC ] = p[ iC * 2 + iR ]; }
	template < typename F >	void	Vma			( const F* l, long sL, const F* r, long sR, const F* p, long sP, F* v, long sV, unsigned long pLength )	{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] * r[ i * sR ] + p[ i * sP ]; }

	template < typename F >	void	Add			( const F* l, long sL, const F* r, long sR, F* v, long sV, unsigned long pLength )						{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] + r[ i * sR ]; }
	template < typename F >	void	Add			( const F* l, long sL, const F& r, F* v, long sV, unsigned long pLength )								{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] + r; }
	template < typename F >	void	Sub			( const F* l, long sL, const F* r, long sR, F* v, long sV, unsigned long pLength )						{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] - r[ i * sR ]; }
	template < typename F >	void	Mul			( const F* l, long sL, const F* r, long sR, F* v, long sV, unsigned long pLength )						{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] * r[ i * sR ]; }
	template < typename F >	void	Mul			( const F* l, long sL, const F& r, F* v, long sV, unsigned long pLength )								{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] * r; }
	template < typename F >	void	Div			( const F* l, long sL, const F* r, long sR, F* v, long sV, unsigned long pLength )						{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] / r[ i * sR ]; }
	template < typename F >	void	Div			( const F* l, long sL, const F& r, F* v, long sV, unsigned long pLength )								{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l[ i * sL ] / r; }
	template < typename F >	void	Div			( const F& l, const F* r, long sR, F* v, long sV, unsigned long pLength )								{ for ( auto i = 0; i < pLength; i++ ) v[ i * sV ] = l / r[ i * sR ]; }
}
#else
#include	<Accelerate/Accelerate.h>
namespace JP {

	inline	void	Clr			( float * p, vDSP_Stride sP, vDSP_Length pLength )																										{ vDSP_vclr			( p, sP, pLength );							}
	inline	void	Clr			( double* p, vDSP_Stride sP, vDSP_Length pLength )																										{ vDSP_vclrD		( p, sP, pLength );							}
	inline	void	Ramp		( float  pInit, float  pStep, float * p, vDSP_Stride sP, vDSP_Length pLength )																			{ vDSP_vramp		( &pInit, &pStep, p, sP, pLength );			}
	inline	void	Ramp		( double pInit, double pStep, double* p, vDSP_Stride sP, vDSP_Length pLength )																			{ vDSP_vrampD		( &pInit, &pStep, p, sP, pLength );			}
	inline	void	Sve			( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )																						{ vDSP_sve			( p, sP, &v, pLength );						}
	inline	void	Sve			( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )																						{ vDSP_sveD			( p, sP, &v, pLength );						}
	inline	void	Mean		( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )																						{ vDSP_meanv		( p, sP, &v, pLength );						}
	inline	void	Mean		( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )																						{ vDSP_meanvD		( p, sP, &v, pLength );						}
	inline	void	Max			( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )																						{ vDSP_maxv			( p, sP, &v, pLength );						}
	inline	void	Max			( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )																						{ vDSP_maxvD		( p, sP, &v, pLength );						}
	inline	void	Min			( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )																						{ vDSP_minv			( p, sP, &v, pLength );						}
	inline	void	Min			( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )																						{ vDSP_minvD		( p, sP, &v, pLength );						}
	inline	void	Svemg		( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )																						{ vDSP_svemg		( p, sP, &v, pLength );						}
	inline	void	Svemg		( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )																						{ vDSP_svemgD		( p, sP, &v, pLength );						}
	inline	void	Svesq		( const float * p, vDSP_Stride sP, float & v, vDSP_Length pLength )																						{ vDSP_svesq		( p, sP, &v, pLength );						}
	inline	void	Svesq		( const double* p, vDSP_Stride sP, double& v, vDSP_Length pLength )																						{ vDSP_svesqD		( p, sP, &v, pLength );						}
	inline	void	Neg			( const float * p, vDSP_Stride sP, float * v, vDSP_Stride sV, vDSP_Length pLength )																		{ vDSP_vneg			( p, sP, v, sV, pLength );					}
	inline	void	Neg			( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length pLength )																		{ vDSP_vnegD		( p, sP, v, sV, pLength );					}
	inline	void	Abs			( const float * p, vDSP_Stride sP, float * v, vDSP_Stride sV, vDSP_Length pLength )																		{ vDSP_vabs			( p, sP, v, sV, pLength );					}
	inline	void	Abs			( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length pLength )																		{ vDSP_vabsD		( p, sP, v, sV, pLength );					}
	inline	void	Rec			( const float * p, float * v, int pLength )																												{ vvrecf			( v, p, &pLength );							}
	inline	void	Rec			( const double* p, double* v, int pLength )																												{ vvrec				( v, p, &pLength );							}
	inline	void	Exp			( const float * p, float * v, int pLength )																												{ vvexpf			( v, p, &pLength );							}
	inline	void	Exp			( const double* p, double* v, int pLength )																												{ vvexp				( v, p, &pLength );							}
	inline	void	Log			( const float * p, float * v, int pLength )																												{ vvlogf			( v, p, &pLength );							}
	inline	void	Log			( const double* p, double* v, int pLength )																												{ vvlog				( v, p, &pLength );							}
	inline	void	Dot			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float & v, vDSP_Length pLength )													{ vDSP_dotpr		( r, sR, l, sL, &v, pLength );				}
	inline	void	Dot			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double& v, vDSP_Length pLength )													{ vDSP_dotprD		( r, sR, l, sL, &v, pLength );				}
	inline	void	Distancesq	( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float & v, vDSP_Length pLength )													{ vDSP_distancesq	( l, sL, r, sR, &v, pLength );				}
	inline	void	Distancesq	( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double& v, vDSP_Length pLength )													{ vDSP_distancesqD	( l, sL, r, sR, &v, pLength );				}
	inline	void	Trans		( const float * p, vDSP_Stride sP, float * v, vDSP_Stride sV, vDSP_Length nR, vDSP_Length nC )															{ vDSP_mtrans		( p, sP, v, sV, nR, nC );					}
	inline	void	Trans		( const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length nR, vDSP_Length nC )															{ vDSP_mtransD		( p, sP, v, sV, nR, nC );					}
	inline	void	Vma			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, const float * p, vDSP_Stride sP, float * v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vma			( l, sL, r, sR, p, sP, v, sV, pLength );	}
	inline	void	Vma			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, const double* p, vDSP_Stride sP, double* v, vDSP_Stride sV, vDSP_Length pLength )	{ vDSP_vmaD			( l, sL, r, sR, p, sP, v, sV, pLength );	}

	inline	void	Add			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vadd			( l, sL, r, sR, v, sV, pLength );			}
	inline	void	Add			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vaddD		( l, sL, r, sR, v, sV, pLength );			}
	inline	void	Add			( const float * l, vDSP_Stride sL, const float & r, float * v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_vsadd		( l, sL, &r, v, sV, pLength );				}
	inline	void	Add			( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_vsaddD		( l, sL, &r, v, sV, pLength );				}
	inline	void	Sub			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vsub			( r, sR, l, sL, v, sV, pLength );			}
	inline	void	Sub			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vsubD		( r, sR, l, sL, v, sV, pLength );			}
	inline	void	Mul			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vmul			( l, sL, r, sR, v, sV, pLength );			}
	inline	void	Mul			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vmulD		( l, sL, r, sR, v, sV, pLength );			}
	inline	void	Mul			( const float * l, vDSP_Stride sL, const float & r, float * v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_vsmul		( l, sL, &r, v, sV, pLength );				}
	inline	void	Mul			( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_vsmulD		( l, sL, &r, v, sV, pLength );				}
	inline	void	Div			( const float * l, vDSP_Stride sL, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vdiv			( r, sR, l, sL, v, sV, pLength );			}
	inline	void	Div			( const double* l, vDSP_Stride sL, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )									{ vDSP_vdivD		( r, sR, l, sL, v, sV, pLength );			}
	inline	void	Div			( const float * l, vDSP_Stride sL, const float & r, float * v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_vsdiv		( l, sL, &r, v, sV, pLength );				}
	inline	void	Div			( const double* l, vDSP_Stride sL, const double& r, double* v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_vsdivD		( l, sL, &r, v, sV, pLength );				}
	inline	void	Div			( const float & l, const float * r, vDSP_Stride sR, float * v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_svdiv		( &l, r, sR, v, sV, pLength );				}
	inline	void	Div			( const double& l, const double* r, vDSP_Stride sR, double* v, vDSP_Stride sV, vDSP_Length pLength )													{ vDSP_svdivD		( &l, r, sR, v, sV, pLength );				}
}
#endif

