#pragma once

#include	<unistd.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<fcntl.h>

#include	<iostream>
#include	<string>
#include	<vector>
#include	<stack>
#include	<numeric>
#include	<ctime>
#include	<sstream>
#include	<random>

using namespace std;

#include	<filesystem>
using namespace filesystem;

typedef	unsigned char	UI1;
typedef	unsigned short	UI2;
typedef	unsigned int	UI4;
typedef	unsigned long	UI8;

//	UNIX UNDER ZERO ERROR
inline int
_X( int $, const string& _, const string& file, int line ) {
	if ( $ < 0 ) {
		cerr << file + ':' + to_string( line ) + ':' + strerror( errno ) + ':' + _ << endl;
#ifdef DEBUG
		__builtin_trap();
#endif
		throw file + ':' + to_string( line ) + ':' + strerror( errno ) + ':' + _;
	}
	return $;
}
#define X( $ ) _X( $, #$, __FILE__, __LINE__ )

//	ASSERTION
inline void
_A( bool $, const string& _, const string& file, int line ) {
	if ( !$ ) {
		cerr << file + ':' + to_string( line ) + ':' + _ << endl;
#ifdef DEBUG
		__builtin_trap();
#endif
		throw file + ':' + to_string( line ) + ':' + _;
	}
}
#define A( $ ) _A( $, #$, __FILE__, __LINE__ )

//	NULL EXCEPTION
template	< typename T >	T*
_N( T* $, const string& _, const string& file, int line ) {
	if ( ! $ ) {
		cerr << file + ':' + to_string( line ) + ':' + _ << endl;
#ifdef DEBUG
		__builtin_trap();
#endif
		throw file + ':' + to_string( line ) + ':' + _;
	}
	return $;
}
#define	N( $ ) _N( $, #$, __FILE__, __LINE__ )

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

	inline	UI2
	Swap2( UI2 $ ) {
		return $ >> 8 | $ << 8;
	}
	inline	UI4
	Swap4( UI4 $ ) {
		return $ >> 24 | ( $ >> 8 & 0x0000ff00 ) | ( $ << 8 & 0x00ff0000 ) | $ << 24;
	}
	inline	UI8
	Swap8( UI8 $ ) {
		return
			$ >> 56
		|	( $ >> 40 & 0x000000000000ff00L )
		|	( $ >> 24 & 0x0000000000ff0000L )
		|	( $ >>  8 & 0x00000000ff000000L )
		|	( $ <<  8 & 0x000000ff00000000L )
		|	( $ << 24 & 0x0000ff0000000000L )
		|	( $ << 40 & 0x00ff000000000000L )
		|	$ << 56
		;
	}

	inline vector< UI1 >
	Read( int fd ) {
		vector< UI1 >	$;
		char	buffer[ 1024 * 1024 ];
		ssize_t	numRead;
		while ( ( numRead = read( fd, buffer, sizeof( buffer ) ) ) ) {
			X( numRead < 0 ? -1 : 0 );
			$.insert( $.end(), buffer, buffer + numRead );
		}
		return $;
	}

	inline vector< UI1 >
	In() {
		return Read( 0 );
	}

	inline void
	Write( int fd, const void* $, UI8 _ ) {
		A( write( fd, $, _ ) == _ );
	}

	inline void
	Write( int fd, const vector< UI1 >& $ ) {
		Write( fd, &$[ 0 ], $.size() );
	}

	inline void
	Out( const void* $, UI8 _ ) {
		Write( 1, $, _ );
	}

	inline void
	Out( const vector< UI1 >& $ ) {
		Write( 1, &$[ 0 ], $.size() );
	}

	inline void
	Err( const void* $, UI8 _ ) {
		Write( 2, $, _ );
	}

	inline void
	Err( const vector< UI1 >& $ ) {
		Write( 2, &$[ 0 ], $.size() );
	}

	inline UI8
	FileSize( const string& path ) {
		struct stat $;
		X( stat( path.c_str(), &$ ) );
		return $.st_size;
	}

	inline vector< UI1 >
	GetFileContent( const string& path ) {
		vector< UI1 >	$( FileSize( path ) );
		int fd = open( path.c_str(), O_RDONLY );
		A( fd > 2 );
		A( read( fd, &$[ 0 ], $.size() ) == $.size() );
		close( fd );
		return $;
	}

	inline void
	SetFileContent( const string& path, const UI1* $, UI8 _ ) {
		int fd = creat( path.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
		A( write( fd, $, _ ) == _ );
		close( fd );
	}

	inline void
	SetFileContent( const string& path, const vector< UI1 >& $ ) {
		SetFileContent( path, &$[ 0 ], $.size() );
	}

	inline vector< string >
	GetLines( istream& $ = cin ) {
		vector< string >	_;
		string				s;
		while ( !$.eof() ) {
			getline( $, s );
			if ( s.size() ) _.emplace_back( s );
		}
		return _;
	}
}
