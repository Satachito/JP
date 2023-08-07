#pragma once
//	2016- Written by Satoru Ogura. (C)SliP LLC, Tokyo.

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

//	ASSERTION
#ifdef	DEBUG
inline void
_A( bool $, string const& _, string const& file, int line ) {
	if ( !$ ) {
		cerr << file + ':' + to_string( line ) + ':' + _ << endl;
		__builtin_trap();
	}
}
#define A( $ ) _A( $, #$, __FILE__, __LINE__ )
#else
#define A( $ )
#endif

//	UNIX UNDER ZERO ERROR
template	< typename I >	I
_X( I $, string const& _, string const& file, int line ) {
	if ( $ < 0 ) {
		cerr << file + ':' + to_string( line ) + ':' + strerror( errno ) + ':' + _ << endl;
		throw file + ':' + to_string( line ) + ':' + strerror( errno ) + ':' + _;
	}
	return $;
}
#define X( $ ) _X( $, #$, __FILE__, __LINE__ )

//	NULL EXCEPTION
template	< typename T >	T*
_N( T* $, string const& _, string const& file, int line ) {
	if ( !$ ) {
		cerr << file + ':' + to_string( line ) + ':' + _ << endl;
		throw file + ':' + to_string( line ) + ':' + _;
	}
	return $;
}
#define	N( $ ) _N( $, #$, __FILE__, __LINE__ )

template < typename T, typename F > auto
Apply( vector< T > const& _, F f ) {
	vector< decltype( f( *_.begin() ) ) > $;
	for ( auto& _: _ ) $.emplace_back( f( _ ) );
	return $;
}

typedef	unsigned char	UI1;
typedef	unsigned short	UI2;
typedef	unsigned int	UI4;
typedef	unsigned long	UI8;

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

template	< typename I >	I
UniformRandomInt( I l = 0, I h = 1 ) {
	static	mt19937_64 $( (std::random_device())() );
	return	uniform_int_distribution< I >( l, h - 1 )( $ );
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
Write( int fd, vector< UI1 > const& $ ) {
	auto	head = $.data();
	auto	tail = head + $.size();
	while ( head < tail ) {
		head += X( write( fd, head, tail - head ) );
		A( head == tail );
	}
}
inline void
Out( vector< UI1 > const& $ ) {
	Write( 1, $ );
}
inline void
Err( vector< UI1 > const& $ ) {
	Write( 2, $ );
}
inline UI8
FileSize( string const& path ) {
	struct stat $;
	X( stat( path.c_str(), &$ ) );
	return $.st_size;
}
struct
FileCloser {
	int	$;
	FileCloser( int $ )
	:	$( $ ) {
	}
	~
	FileCloser() {
		X( close( $ ) );
	}
};
inline vector< UI1 >
GetFileContent( string const& path ) {
	FileCloser _( open( path.c_str(), O_RDONLY ) );
	return Read( _.$ );
}
inline void
SetFileContent( string const& path, vector< UI1 > const& $ ) {
	FileCloser _( creat( path.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP ) );
	Write( _.$, $ );
}

inline vector< string >
GetLines( istream& stream = cin ) {
	vector< string >	$;
	string				s;
	while ( !stream.eof() ) {
		getline( stream, s );
		if ( s.size() ) $.emplace_back( s );
	}
	return $;
}

inline UI8
Size1( UI1 const* $ ) {
	return $[ 0 ];
}

inline UI8
Size2( UI1 const* $ ) {
	return
		UI2( $[ 1 ] ) << 0
	|	UI2( $[ 0 ] ) << 8
	;
}

inline UI8
Size3( UI1 const* $ ) {
	return
		UI2( $[ 2 ] ) <<  0
	|	UI2( $[ 1 ] ) <<  8
	|	UI2( $[ 0 ] ) << 16
	;
}

inline UI8
Size4( UI1 const* $ ) {
	return
		UI4( $[ 3 ] ) << 0
	|	UI4( $[ 2 ] ) << 8
	|	UI4( $[ 1 ] ) << 16
	|	UI4( $[ 0 ] ) << 24
	;
}

inline UI8
Size8( UI1 const* $ ) {
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
	UI1	const*	$;
	UI8			_;

	BitReader( UI1 const* $ )
	:	$( $ )
	,	_( 0 ) {
	}
	bool
	Read() {
		UI8 v = $[ _ / 8 ] & ( 0x80 >> ( _ % 8 ) );
		_++;
		return v ? true : false;
	}
	UI8
	Read( UI8 $ ) {
		UI8 v = 0;
		while ( $-- ) v = ( v << 1 ) | ( Read() ? 1 : 0 );
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
	}
	void
	ByteAlign() {
		UI8 $ = _ % 8;
		if ( $ ) _ += 8 - $;
	}
};

typedef	UI4	UTF32;

inline	string
UTF8( UTF32 _ ) {
	vector< UI1 >	$;
	if ( _ <= 0x7F) {
		$.emplace_back( _ );
	} else if ( _ <= 0x7FF ) {
		$.emplace_back( ( 0xC0 | (( _ >> 6) & 0x1F)));
		$.emplace_back( ( 0x80 | ( _ & 0x3F)));
	} else if ( _ <= 0xFFFF) {
		$.emplace_back( ( 0xE0 | (( _ >> 12) & 0x0F)));
		$.emplace_back( ( 0x80 | (( _ >> 6) & 0x3F)));
		$.emplace_back( ( 0x80 | ( _ & 0x3F)));
	} else if ( _ <= 0x10FFFF) {
		$.emplace_back( ( 0xF0 | (( _ >> 18) & 0x07)));
		$.emplace_back( ( 0x80 | (( _ >> 12) & 0x3F)));
		$.emplace_back( ( 0x80 | (( _ >> 6) & 0x3F)));
		$.emplace_back( ( 0x80 | ( _ & 0x3F)));
	} else {
		throw "eh?";
	}
	return string( $.begin(), $.end() );
}

struct
UI1Stream {
	vector< UI1 >	$;
	UI8				_;
	UI1Stream( string const& _ )
	:	$( _.begin(), _.end() )
	,	_( 0 ) {
	}
	UI1
	Avail() { return _ < $.size(); }
	UI1
	Get() { return $[ _++ ]; }
};

inline	UTF32
Unicode( UI1Stream& stream ) {
	A( stream.Avail() );

	UI1	_ = stream.Get();
	if ( _ < 0x80 ) return _;

	_ <<= 1;
	auto	wNumCont = 0;
	while ( _ & 0x80 ) {
		wNumCont++;
		_ <<= 1;
	}
	UTF32	$ = _ >> ( wNumCont + 1 );

	while ( wNumCont-- ) {
		A( stream.Avail() );
		UI1	_ = stream.Get();
		$ = ( $ << 6 ) | ( _ & 0x3f );
	}
	return $;
}
struct
UnicodeReader {
	UTF32		buffer = 0;
	bool		unread = false;
	UI1Stream	$;
	UnicodeReader( string const& _ )
	:	$( _ ) {
	}
	bool
	Avail() {
		if ( unread ) return true;
		try {
			auto _ = Unicode( $ );
			Unread( _ );
		} catch( ... ) {
			return false;
		}
		return true;
	}
	void
	Unread( UTF32 p ) {
		buffer = p;
		unread = true;
	}
	UTF32
	_Read() {
		if ( unread ) {
			unread = false;
			return buffer;
		}
		return Unicode( $ );
	}
	UTF32
	Read() {
		auto $ = _Read();
		return $;
	}
};
struct
PreProcessor {
	UnicodeReader&	$;
	PreProcessor( UnicodeReader& $ )
	:	$( $ ) {
	}
	UTF32
	Read() {
	RESET:
		auto v = $.Read();
		if ( v == '/' ) {
			auto w = $.Read();
			if ( w == '/' ) {
				while ( $.Read() != '\n' ) {}
				goto RESET;	//	return Read();
			} else {
				$.Unread( w );
				return v;
			}
		}
		return v;
	}
};

inline UI1
HexNum( char $ ) {
	switch ( $ ) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':	return $ - '0';
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':											return $ - 'A' + 10;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':											return $ - 'a' + 10;
	default:	throw "eh?";
	}
}
inline vector< UI1 >
DecodeHex( string const& string ) {
	vector< UI1 >	$;
	for ( auto _ = 0; _ < string.size(); _ += 2 ) $.emplace_back( HexNum( string[ _ ] ) << 4 | HexNum( string[ _ + 1 ] ) );
	return $;
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
EncodeHex( vector< UI1 > const& _ ) {
	string	$;
	for ( auto& _: _ ) $ += HexStr( _ );
	return $;
}
