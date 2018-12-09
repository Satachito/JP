//	Written by Satoru Ogura, Tokyo.
//
#pragma	once

#include	<iostream>

inline	int	//	UNIX ERROR EXCEPTION
_U( int p, const char* file, int line ) {
	if ( p == -1 ) {
		fprintf( stderr, "%s:%d:%s\n", file, line, strerror( errno ) );
		throw -1;
	}
	return p;
}
#define	U( p ) _U( p, __FILE__, __LINE__ )

template	< typename T >	T*	//	NULL EXCEPTION
_N( T* p, const char* file, int line ) {
	if ( ! p ) {
		fprintf( stderr, "%s:%d:NULL\n", file, line );
		throw 0;
	}
	return p;
}
#define	N( p ) _N( p, __FILE__, __LINE__ )

#include	<random>

namespace JP {

	typedef	int	UNICODE;

	inline	UNICODE
	Unicode( std::istream& p ) {
		unsigned char	c;
		p >> c;
		if ( p.eof() ) throw "EOF";
		if ( c < 0x80 ) return c;

		c <<= 1;
		auto	wNumCont = 0;
		while ( c & 0x80 ) {
			wNumCont++;
			c <<= 1;
		}
		UNICODE	v = c >> ( wNumCont + 1 );

		while ( wNumCont-- ) {
			p >> c;
			if ( p.eof() ) throw "EOF";
			v = ( v << 6 ) | ( c & 0x3f );
		}
		
		return v;
	}
	struct
	UnicodeStream {
		UNICODE	buffer = 0;
		bool	Unreaded = false;
		void
		Unread( UNICODE p ) {
			buffer = p;
			Unreaded = true;
		}
		UNICODE
		Read() {
			if ( Unreaded ) {
				Unreaded = false;
				return buffer;
			}
			return Unicode( std::cin );	//	UC
		}
	};
	struct
	PreProcessor: UnicodeStream {
		UNICODE
		Read() {
		RESET:
			auto v = UnicodeStream::Read();
			if ( v == '/' ) {
				auto w = UnicodeStream::Read();
				if ( w == '/' ) {
					while ( UnicodeStream::Read() != '\n' ) {}
					goto RESET;	//	return Read();
				} else {
					Unread( w );
					return v;
				}
			}
			return v;
		}
	};
	
	template	< typename I >	I
	UniformRandomInt( I l = 0, I h = 1 ) {
		static	std::mt19937_64 sMT( (std::random_device())() );
		return	std::uniform_int_distribution< I >( l, h - 1 )( sMT );
	}
	
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
};
