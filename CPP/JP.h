#pragma once

#include	<unistd.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<fcntl.h>

#include	<iostream>
#include	<string>
#include	<vector>
#include	<map>
#include	<stack>
#include	<numeric>
#include	<ctime>
#include	<sstream>
#include	<random>
#include	<algorithm>

using namespace std;

#include	<filesystem>
using namespace filesystem;

typedef	unsigned char	UI1;
typedef	unsigned short	UI2;
typedef	unsigned int	UI4;
typedef	unsigned long	UI8;

//	UNIX UNDER ZERO ERROR
template	< typename I >	I
_X( I $, const string& _, const string& file, int line ) {
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
	if ( !$ ) {
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
	Unicode( istream& p ) {
		UI1	c;
		p >> c;
		A( !p.eof() );
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
			A( !p.eof() );
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
			return Unicode( cin );	//	UC
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
		while ( ( numRead = X( read( fd, buffer, sizeof( buffer ) ) ) ) ) $.insert( $.end(), buffer, buffer + numRead );
		return $;
	}

	inline vector< UI1 >
	In() {
		return Read( 0 );
	}

	inline void
	Write( int fd, const void* $, UI8 _ ) {
		A( UI8( X( write( fd, $, _ ) ) ) == _ );
	}

	inline void
	Write( int fd, const vector< UI1 >& $ ) {
		Write( fd, $.data(), $.size() );
	}

	inline void
	Out( const void* $, UI8 _ ) {
		Write( 1, $, _ );
	}

	inline void
	Out( const vector< UI1 >& $ ) {
		Write( 1, $.data(), $.size() );
	}

	inline void
	Err( const void* $, UI8 _ ) {
		Write( 2, $, _ );
	}

	inline void
	Err( const vector< UI1 >& $ ) {
		Write( 2, $.data(), $.size() );
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
		A( UI8( X( read( fd, $.data(), $.size() ) ) ) == $.size() );
		close( fd );
		return $;
	}

	inline void
	SetFileContent( const string& path, const UI1* $, UI8 _ ) {
		int fd = creat( path.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
		A( UI8( X( write( fd, $, _ ) ) ) == _ );
		close( fd );
	}

	inline void
	SetFileContent( const string& path, const vector< UI1 >& $ ) {
		SetFileContent( path, $.data(), $.size() );
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

	inline UI1
	HexNum( const char $ ) {
		switch ( $ ) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':	return $ - '0';
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':											return $ - 'A' + 10;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':											return $ - 'a' + 10;
		default:	throw "eh?";
		}
	}
	inline vector< UI1 >
	DecodeHex( const string& $ ) {
		vector< UI1 >	$$;
		for ( auto _ = 0; _ < $.size(); _ += 2 ) $$.emplace_back( HexNum( $[ _ ] ) << 4 | HexNum( $[ _ + 1 ] ) );
		return $$;
	}

	inline char
	HexChar( UI1 $ ) {
		switch ( $ ) {
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
			return $ + '0';
		case 10: case 11: case 12: case 13: case 14: case 15:
			return $ - 10 + 'a';
		default:	throw "eh?";
		}
	}
	inline string
	HexStr( UI1 $ ) {
		char	_[] = {
			HexChar( ( $ >> 4 ) & 0x0f )
		,	HexChar( $ & 0x0f )
		,	0
		};
		return string( _ );
	}
	inline string
	EncodeHex( const UI1* _, const UI1* $ ) {
		string	$$;
		while ( _ < $ ) $$ += HexStr( *_++ );
		return $$;
	}
	inline string
	EncodeHex( const UI1* _, UI8 $ ) {
		return EncodeHex( _, _ + $ );
	}
	inline string
	EncodeHex( const vector< UI1 >& $ ) {
		return EncodeHex( (UI1*)&$[ 0 ], $.size() );
	}
	inline string
	EncodeHexLF16( const UI1* _, UI8 $ ) {
		string	$$;
		for ( auto i = 0; i < $; i += 16 ) {
			auto nRemain = $ - i;
			$$ += to_string( i ) + ':' + EncodeHex( _ + i, _ + i + ( nRemain < 16 ? nRemain : 16 ) ) + '\n';
		}
		return $$;
	}
	inline string
	EncodeHexLF16( const UI1* _, const UI1* $ ) {
		return EncodeHexLF16( _, $ - _ );
	}
	inline string
	EncodeHexLF16( const vector< UI1 >& _ ) {
		return EncodeHexLF16( (UI1*)&_[ 0 ], _.size() );
	}

	inline UI8
	Size1( const UI1* $ ) {
		return $[ 0 ];
	}

	inline UI8
	Size2( const UI1* $ ) {
		return
			UI2( $[ 1 ] ) << 0
		|	UI2( $[ 0 ] ) << 8
		;
	}

	inline UI8
	Size3( const UI1* $ ) {
		return
			UI2( $[ 2 ] ) <<  0
		|	UI2( $[ 1 ] ) <<  8
		|	UI2( $[ 0 ] ) << 16
		;
	}

	inline UI8
	Size4( const UI1* $ ) {
		return
			UI4( $[ 3 ] ) << 0
		|	UI4( $[ 2 ] ) << 8
		|	UI4( $[ 1 ] ) << 16
		|	UI4( $[ 0 ] ) << 24
		;
	}

	inline UI8
	Size8( const UI1* $ ) {
		return
			UI8( $[ 7 ] ) << 0
		|	UI8( $[ 6 ] ) << 8
		|	UI8( $[ 5 ] ) << 16
		|	UI8( $[ 4 ] ) << 24
		|	UI8( $[ 3 ] ) << 32
		|	UI8( $[ 2 ] ) << 40
		|	UI8( $[ 1 ] ) << 48
		|	UI8( $[ 0 ] ) << 56
		;
	}

	struct
	BitReader {
		const	UI1*	$;
				UI8		_;

#define NUM_BYTES	456
UI8 bytes_left;
UI8 bits_left;

		BitReader( const UI1* $ )
		:	$( $ )
		,	_( 0 ) {
bits_left = ( NUM_BYTES * 8 - _ ) % 32;
bytes_left = NUM_BYTES - ( _ / 32 ) * 4;

		}
		bool
		Read() {
			UI8 v = $[ _ / 8 ] & ( 0x80 >> ( _ % 8 ) );
			_++;
bits_left = ( NUM_BYTES * 8 - _ ) % 32;
bytes_left = NUM_BYTES - ( _ / 32 ) * 4;
			return v ? true : false;
		}
		UI8
		Read( UI8 _ ) {
			UI8 v = 0;
			while ( _-- ) v = ( v << 1 ) | ( Read() ? 1 : 0 );
			return v;
		}
		UI8
		ReadGolomb() {
			UI8 lz = 0;
			while ( !Read() ) lz++;
			return lz ? ( 1 << lz ) - 1 + Read( lz ) : 0;
		}
		UI8
		ReadSignedGolomb() {
			auto $ = ReadGolomb();
			if ( $ % 2 ) {
				return ( $ + 1 ) / 2;
			} else {
				return - $ / 2;
			}
		}
		void
		Skip( UI8 $ ) {
			_ += $;
bits_left = ( NUM_BYTES * 8 - _ ) % 32;
bytes_left = NUM_BYTES - ( _ / 32 ) * 4;
		}
	};
}
