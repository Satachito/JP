#pragma once
//	2016- Written by Satoru Ogura. (C)SliP LLC, Tokyo.

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
#include	<algorithm>
#include	<ranges>

#include	<unordered_set>
#include	<unordered_map>

#include	<mutex>
#include	<functional>
////////////////////////////////////////////////////////////////
#include	<numbers>
////////////////////////////////////////////////////////////////
#ifdef __APPLE__
#include <type_traits> // for std::is_integral
#include <limits>      // for std::numeric_limits
#include <iostream>    // for example usage
#include <string>      // for REPR in example

template <typename T>
bool ckd_add( T* res ,T a, T b) {
    static_assert(std::is_integral<T>::value, "ckd_add only supports integral types.");
    // __builtin_add_overflow returns true if overflow occurs
    return __builtin_add_overflow(a, b, res);
}

template <typename T>
bool ckd_sub( T* res ,T a, T b) {
    static_assert(std::is_integral<T>::value, "ckd_sub only supports integral types.");
    // __builtin_sub_overflow returns true if overflow occurs
    return __builtin_sub_overflow(a, b, res);
}

template <typename T>
bool ckd_mul( T* res ,T a, T b) {
    static_assert(std::is_integral<T>::value, "ckd_mul only supports integral types.");
    // __builtin_mul_overflow returns true if overflow occurs
    return __builtin_mul_overflow(a, b, res);
}

#else
#include <stdckdint.h>
#endif
////////////////////////////////////////////////////////////////

using namespace std;
using namespace std::views;
using namespace std::ranges;

#include	<filesystem>
using namespace filesystem;

#define	_Z( $ )	throw runtime_error( $ )
template< typename T > auto
Z( string const& _, T $ ) {
	if ( !$ ) _Z( _ );
	return $;
}


//	ASSERTION
#ifdef	DEBUG
inline auto
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


//	POSIX SYSTEM CALL (UNDER ZERO)
template < typename T > auto	//	T: int, ssize_t, off_t
_X( T $, string const& _, string const& file, int line ) {
	if ( $ < 0 ) {
		cerr << file + ':' + to_string( line ) + ':' + strerror( errno ) + ':' + _ << endl;
		throw file + ':' + to_string( line ) + ':' + strerror( errno ) + ':' + _;
	}
	return $;
}
#define X( $ ) _X( $, #$, __FILE__, __LINE__ )

//	NULL EXCEPTION	map,
template < typename T > auto
_N( T $, string const& _, string const& file, int line ) {
	if ( !$ ) {
		cerr << file + ':' + to_string( line ) + ':' + _ << endl;
		throw file + ':' + to_string( line ) + ':' + _;
	}
	return $;
}
#define	N( $ ) _N( $, #$, __FILE__, __LINE__ )

#define	THROW	throw string( __FILE__ ) + ":" + to_string( __LINE__ )

template < typename T >	auto
operator+( vector< T > const& l, vector< T > const& r ) {
	vector<T> $;
	$.reserve( l.size() + r.size() );
	$.insert( $.end(), l.begin(), l.end() );
	$.insert( $.end(), r.begin(), r.end() );
	return $;
}

template < range R, typename T > auto
has( R&& _, T const& t ) {
	return contains( _, t );
}

template < range R > auto
zipIndex( R&& _ ) {
    return zip( _, views::iota( static_cast< size_t >( 0 ), ranges::size( _ ) ) );
}

template < range R, invocable< range_reference_t< R >, size_t > F > auto
project( R&& _, F f ) {
    size_t index = 0;
    return _ | views::transform(
		[ & ]( auto&& $ ) mutable {
			return f( $, index++ ); // Call the user-provided function 'f'
	   }
	);
}

template < range R, invocable< range_reference_t< R > > F > auto
project( R&& _, F f ) { 
    return _ | views::transform( f );
}

template< range R, typename U, invocable< U, range_reference_t< R >, size_t > F > auto
reduce( R&& _, F f, U $ ) {
	size_t index = 0;
    for ( auto&& _ : _ ) $ = f( std::move( $ ), _, index++ );
	return $;
}

template< range R, typename U, invocable< U, range_reference_t< R > > F > auto
reduce( R&& _, F f, U $ ) {
	for( auto&& _ : _ ) $ = f( std::move( $ ), _ );
	return $;
}

template < range R, typename F > auto
some( R&& _, F f ) {
	return ranges::any_of( _, f );
}

template < range R, typename F > auto
every( R&& _, F f ) {
	return ranges::all_of( _, f );
}

template < range R, typename F > auto
apply( R&& _, F f ) {
	for( auto const& _: _ ) f( _ );
}

template < range R > auto
drop( R&& _, size_t from ) {
    return _ | views::drop( from );
}

template < range R > auto
take( R&& _, size_t to ) {
    return _ | views::take( to );
}

////////////////////////////////////////////////////////////////

inline auto
string_U( char32_t _ ) {

	string $;

	if ( 0xd800 <= _ && _ <= 0xdfff ) {		//	Surrogate area
		$ += '?';
	} else if ( _ <= 0x7F ) {
		$ += static_cast<char>( _ );
	} else if ( _ <= 0x7FF ) {
		$ += static_cast<char>( 0b11000000 | ( _ >> 6) );
		$ += static_cast<char>( 0b10000000 | ( _ & 0b00111111 ) );
	} else if ( _ <= 0xFFFF ) {
		$ += static_cast<char>( 0b11100000 | ( _ >> 12 ) );
		$ += static_cast<char>( 0b10000000 | ( ( _ >> 6 ) & 0b00111111 ) );
		$ += static_cast<char>( 0b10000000 | ( _ & 0b00111111 ) );
	} else if ( _ <= 0x10FFFF ) {
		$ += static_cast<char>( 0b11110000 | ( _ >> 18 ) );
		$ += static_cast<char>( 0b10000000 | ( ( _ >> 12 ) & 0b00111111 ) );
		$ += static_cast<char>( 0b10000000 | ( ( _ >> 6 ) & 0b00111111 ) );
		$ += static_cast<char>( 0b10000000 | ( _ & 0b00111111 ) );
	} else {
		$ += '?';
	}
	return $;
}

inline auto
string_Us( vector< char32_t > const& char32s ) {
	string $;
	for( char32_t _ : char32s ) $ += string_U( _ );
	return $;
}

template< typename IT > char32_t
U_string( IT& it, IT end ) {

	A( it < end );
	char32_t _0 = static_cast< unsigned char >( *it++ );
	if( ( _0 & 0x80 ) == 0 ) return _0;

	A( it < end );
	char32_t _1 = static_cast< unsigned char >( *it++ );
	if(	( _0 & 0xE0 ) == 0xC0 ) return ( ( _0 & 0x1F ) << 6 )
	|	( _1 & 0x3F )
	;

	A( it < end );
	char32_t _2 = static_cast< unsigned char >( *it++ );
	if(	( _0 & 0xF0 ) == 0xE0 ) return ( ( _0 & 0x0F ) << 12 )
	|	( ( _1 & 0x3F ) << 6 )
	|	( _2 & 0x3F )
	;

	A( it < end );
	char32_t _3 = static_cast< unsigned char >( *it++ );
	if( ( _0 & 0xF8 ) == 0xF0 ) return ( ( _0 & 0x07 ) << 18 )
	|	( ( _1 & 0x3F ) << 12 )
	|	( ( _2 & 0x3F ) << 6 )
	|	( _3 & 0x3F )
	;

	throw string( __FILE__ ) + ":" + to_string( __LINE__ );
}

template < typename IT > vector<char32_t>
Us_string( IT begin, IT end ) {
	vector< char32_t > $;
	while ( begin < end ) {
		$.push_back( U_string( begin, end ) );
	}
	return $;
}

inline vector< char32_t >
Us_string( string_view _ ) {
	auto begin = _.begin();
	return Us_string( begin, _.end() );
}

inline vector< string >
Split( string const& _ ) {

	size_t start = 0;
	size_t end = _.find( '\n' );

	vector< string >	$;
	while ( end != string::npos ) {
		auto line = _.substr( start, end - start );
		if ( !line.empty() ) $.push_back( line );
		start = end + 1;
		end = _.find( '\n', start );
	}
	auto line = _.substr( start );
	if ( !line.empty() ) $.push_back( line );
	return $;
}

inline bool	//	Excluding NO-BREAK SPACE i.e. a0, feff
IsBreakingWhite( char32_t _ ) {
	if( _ <= 0x20 ) return true;
	if( 0x7f <= _ && _ < 0xA0 ) return true;
	switch ( _ ) {
	case 0x1680:
	case 0x2000:
	case 0x2001:
	case 0x2002:
	case 0x2003:
	case 0x2004:
	case 0x2005:
	case 0x2006:
	case 0x2007:
	case 0x2008:
	case 0x2009:
	case 0x200A:
	case 0x2028:
	case 0x2029:
	case 0x202F:
	case 0x205F:
	case 0x3000:
		return true;
	default:
		return false;
	}
}

inline bool
IsAllBreakingWhite( string const& _ ) {
	return every(
		_
	,	[]( auto c ) {
			return IsBreakingWhite( c );
		}
	);
}

inline auto
IsDigit( char32_t _ ) {
	return U'0' <= _ && _ <= U'9';
}


typedef	uint8_t		UI1;
typedef	uint16_t	UI2;
typedef	uint32_t	UI4;
typedef	uint64_t	UI8;

inline	auto
Swap2( UI2 $ ) {
	return UI2( $ >> 8 | $ << 8 );
}
static_assert(
	is_same_v<
		decltype( Swap2( declval< UI2 >() ) )
	,	UI2
	>
,	"The return type should be UI2"
);

inline	auto
Swap4( UI4 $ ) {
	return $ >> 24 | ( $ >> 8 & 0x0000ff00 ) | ( $ << 8 & 0x00ff0000 ) | $ << 24;
}
static_assert(
	is_same_v<
		decltype( Swap4( declval< UI4 >() ) )
	,	UI4
	>
,	"The return type should be UI4"
);

inline	auto
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
static_assert(
	is_same_v<
		decltype( Swap8( declval< UI8 >() ) )
	,	UI8
	>
,	"The return type should be UI8"
);

template < typename I > auto
UniformRandomInt( I l = 0, I h = 1 ) {
	static	mt19937_64 $( (random_device())() );
	return	uniform_int_distribution< I >( l, h - 1 )( $ );
}

inline auto
Read( int fd ) {
	vector< UI1 >	$;
	char	buffer[ 1024 * 1024 ];
	ssize_t	numRead;
	while ( ( numRead = X( read( fd, buffer, sizeof( buffer ) ) ) ) ) $.insert( $.end(), buffer, buffer + numRead );
	return $;
}
inline auto
In() {
	return Read( 0 );
}

inline auto
Write( int fd, UI1 const* $, UI1 const* _ ) {
	while ( $ < _ ) $ += X( write( fd, $, _ - $ ) );
}
inline auto
Write( int fd, UI1 const* $, UI8 _ ) {
	Write( fd, $, $ + _ );
}
inline auto
Write( int fd, vector< UI1 > const& $ ) {
	Write( fd, $.data(), $.size() );
}
inline auto
Out( vector< UI1 > const& $ ) {
	Write( 1, $ );
}
inline auto
Err( vector< UI1 > const& $ ) {
	Write( 2, $ );
}
inline auto
FileSize( string const& path ) {
	struct stat $;
	X( stat( path.c_str(), &$ ) );
	return UI8( $.st_size );
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
inline auto
GetFileContent( string const& path ) {
	FileCloser _( open( path.c_str(), O_RDONLY ) );
	return Read( _.$ );
}
inline auto
SetFileContent( string const& path, vector< UI1 > const& $ ) {
	FileCloser _( creat( path.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP ) );
	Write( _.$, $ );
}

inline auto
GetLines( std::istream& stream = cin ) {
	vector< string >	$;
	string				s;
	while ( !stream.eof() ) {
		getline( stream, s );
		if ( s.size() ) $.emplace_back( s );
	}
	return $;
}

inline auto
Size1( UI1 const* $ ) {
	return UI8( $[ 0 ] );
}

inline auto
Size2( UI1 const* $ ) {
	return
		UI8( $[ 1 ] ) << 0
	|	UI8( $[ 0 ] ) << 8
	;
}

inline auto
Size3( UI1 const* $ ) {
	return
		UI8( $[ 2 ] ) <<  0
	|	UI8( $[ 1 ] ) <<  8
	|	UI8( $[ 0 ] ) << 16
	;
}

inline auto
Size4( UI1 const* $ ) {
	return
		UI8( $[ 3 ] ) << 0
	|	UI8( $[ 2 ] ) << 8
	|	UI8( $[ 1 ] ) << 16
	|	UI8( $[ 0 ] ) << 24
	;
}

inline auto
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
	auto
	Read() {
		UI8 v = $[ _ / 8 ] & ( 0x80 >> ( _ % 8 ) );
		_++;
		return v ? true : false;
	}
	auto
	Read( UI8 $ ) {
		UI8 v = 0;
		while ( $-- ) v = ( v << 1 ) | ( Read() ? 1 : 0 );
		return v;
	}
	auto
	ReadGolomb() {
		UI8 lz = 0;
		while ( !Read() ) lz++;
		return lz ? ( 1 << lz ) - 1 + Read( lz ) : 0;
	}
	auto
	ReadSignedGolomb() {
		auto $ = ReadGolomb();
		if ( $ % 2 ) {
			return ( $ + 1 ) / 2;
		} else {
			return - $ / 2;
		}
	}
	auto
	Skip( UI8 $ ) {
		_ += $;
	}
	auto
	ByteAlign() {
		UI8 $ = _ % 8;
		if ( $ ) _ += 8 - $;
	}
};

inline auto
IsSpace( char32_t _ ) {
	if ( _ <= ' ' ) return true;
	switch ( _ ) {
	case 0x1680: case 0x180e: case 0x2000:
	case 0x2001: case 0x2002: case 0x2003: case 0x2004: case 0x2005: case 0x2006: case 0x2007: case 0x2008: case 0x2009: case 0x200a:
	case 0x202f: case 0x205f: case 0x3000:
	case 0x2028: case 0x2029: case 0xfeff:
		return true;
	}
	return false;
}

inline	auto
UTF8( char32_t _ ) {
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
		A( false );
		throw "eh?";
	}
	return string( $.begin(), $.end() );
}

inline	auto
UTF8( vector< char32_t > const& _ ) {
	string $;
	for ( auto& _: _ ) $ += UTF8( _ );
	return $;
}

struct
UI1Stream {
	vector< UI1 >	$;
	UI8				_;
	UI1Stream( string const& _ )
	:	$( _.begin(), _.end() )
	,	_( 0 ) {
	}
	auto
	Avail() { return _ < $.size(); }
	auto
	Get() { return $[ _++ ]; }
};

inline	auto
Unicode( UI1Stream& stream ) {
	if ( !stream.Avail() ) THROW;

	UI1	_ = stream.Get();
	if ( _ < 0x80 ) return (char32_t)_;

	_ <<= 1;
	auto	wNumCont = 0;
	while ( _ & 0x80 ) {
		wNumCont++;
		_ <<= 1;
	}
	char32_t	$ = _ >> ( wNumCont + 1 );

	while ( wNumCont-- ) {
		if ( !stream.Avail() ) THROW;
		UI1	_ = stream.Get();
		$ = ( $ << 6 ) | ( _ & 0x3f );
	}
	return $;
}
struct
UnicodeReader {

	UI8	index = 0;
	vector< char32_t >	_;

	char32_t		ungot = 0;
	bool		unget = false;
	UI1Stream	$;
	UnicodeReader( string const& _ )
	:	$( _ ) {
	}
	auto
	UnGet( char32_t _ ) {
		this->_.pop_back();
		index--;
		unget = true;
		ungot = _;
	}
	auto
	_Get() {
		if ( unget ) {
			unget = false;
			return ungot;
		}
		return Unicode( $ );
	}
	auto
	Get() {
		auto $ = _Get();
		_.emplace_back( $ );
		index++;
		return $;
	}
	auto
	Peek() {
		if ( !unget ) {
			ungot = Unicode( $ );
			unget = true;
		}
		return ungot;
	}
	auto
	Avail() {
		if ( unget ) return true;
		try {
			UnGet( Get() );
		} catch( ... ) {
			return false;
		}
		return true;
	}
};
struct
PreProcessor : UnicodeReader {
	PreProcessor( string const& _ )
	:	UnicodeReader( _ ) {
	}
	auto
	Get() {
	RESET:
		auto v = UnicodeReader::Get();
		if ( v == '/' ) {
			auto w = UnicodeReader::Get();
			if ( w == '/' ) {
				while ( UnicodeReader::Get() != '\n' ) {}
				goto RESET;	//	return Read();
			} else {
				UnicodeReader::UnGet( w );
				return v;
			}
		}
		return v;
	}
};

inline auto
Unescape( string const& _ ) {
	auto escaping = false;
	string $;
	for ( auto& _: _ ) {
		if ( escaping ) {
			escaping = false;
			switch ( _ ) {
			case '0': $ += '\0'	; break;
			case 'f': $ += '\f'	; break;
			case 'n': $ += '\n'	; break;
			case 'r': $ += '\r'	; break;
			case 't': $ += '\t'	; break;
			case 'v': $ += '\v'	; break;
			default	: $ += _	; break;
			}
		} else {
			_ == '\\'
			?	void( escaping = true )
			:	void( $ += _ )
			;
		}
	}
	return $;
};
inline auto
Shrink( string const& _ ) {
	string $;
	for ( auto& _: _ ) if ( !IsSpace( _ ) ) $ += _;
	return $;
};

inline auto
Digit( char32_t _ ) {
	switch ( _ ) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		return UI1( _ - '0' );
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
	case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		return UI1( _ - 'a' + 10 );
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
	case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
	case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		return UI1( _ - 'A' + 10 );
	}
	THROW;
};

inline auto
DecodeHex( string const& string ) {
	vector< UI1 >	$;
	for ( auto _ = 0; _ < string.size(); _ += 2 ) $.emplace_back( Digit( string[ _ ] ) << 4 | Digit( string[ _ + 1 ] ) );
	return $;
}

inline auto
DigitCharacter( UI1 $ ) {
	switch ( $ ) {
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
		return char( $ + '0' );
	case 10: case 11: case 12: case 13: case 14: case 15:
	case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25:
	case 26: case 27: case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35:
		return char( $ - 10 + 'a' );
	}
	THROW;
}
inline auto
HexStr( UI1 $ ) {
	char	_[] = {
		DigitCharacter( ( $ >> 4 ) & 0x0f )
	,	DigitCharacter( $ & 0x0f )
	,	0
	};
	return string( _ );
}
inline auto
EncodeHex( UI1 const* $, UI8 _ ) {
	return accumulate(
		$
	,	$ + _
	,	string()
	,	[]( string const& $, UI1 _ ) { return $ + HexStr( _ ); }
	);
}
inline auto
EncodeHex( UI1 _ ) {
	return EncodeHex( &_, 1 );
}
inline auto
EncodeHex( UI2 _ ) {
	auto $ = Swap2( _ );
	return EncodeHex( (UI1*)&$, 2 );
}
inline auto
EncodeHex( UI4 _ ) {
	auto $ = Swap4( _ );
	return EncodeHex( (UI1*)&$, 4 );
}
inline auto
EncodeHex( UI8 _ ) {
	auto $ = Swap8( _ );
	return EncodeHex( (UI1*)&$, 8 );
}

inline auto
EncodeDouble( double _ ) {
	string $ = to_string( _ );
	while ( $.size() > 3 && $[ $.size() - 1 ] == '0' ) $.pop_back();
	return $;
}

template < typename I > auto
Sign( I _ ) {
	return _ == 0
	?	0
	:	_ < 0 ? -1 : 1
	;
}

template < typename I > auto
GEL( I p, I q ) {
	return p == q
	?	0
	:	p < q ? -1 : 1
	;
}

template < typename I > auto
NumLeadingZeroBits( I _ ) {
	static_assert( is_unsigned< I >::value, "eh?");
	constexpr UI8 N_BITS_PER_I = sizeof( I ) * 8;

//#if defined(__GNUC__) || defined(__clang__)
//	if constexpr ( sizeof( I ) == sizeof( UI4 ) ) return _ ? __builtin_clz( _ )		: N_BITS_PER_I;
//	if constexpr ( sizeof( I ) == sizeof( UI8 ) ) return _ ? __builtin_clzll( _ )	: N_BITS_PER_I;
//#endif

	UI8 $ = 0;
	while ( $ < N_BITS_PER_I ) {
		if ( _ & ( I( 1 ) << ( N_BITS_PER_I - $ - 1 ) ) ) break;
		$++;
	}
	return $;
}


template < typename T > struct
Chain {
	T		_;
	Chain*	$;
	Chain( T const& _, Chain* $ ) : _( _ ), $( $ ) {}
	T&
	Last() {
		return $
		?	$->Last()
		:	_
		;
	}
};

template < typename UI, typename F > void
Combination( UI N, UI R, F f, UI S = 0, Chain< UI >* $ = 0 ) {
	switch ( R ) {
	case 0	:
		throw "eh?";
	case 1	:
		for ( UI _ = S; _ < N; _++ ) {
			Chain< UI > chain( _, $ );
			f( &chain );
		}
		break;
	default	:
		for ( UI _ = S; _ < N - R + 1; _++ ) {
			Chain< UI > chain( _, $ );
			Combination( N, R - 1, f, _ + 1, &chain );
		}
		break;
	}
}

#include	<chrono>
using namespace chrono;
struct
StopWatch {

	using Clock		= high_resolution_clock;
	using TimePoint	= time_point< Clock >;
	using Duration	= duration< double >;

	string		title;
	TimePoint	_;
	StopWatch( string title ) : title( title ), _( Clock::now() ) {}
	~
	StopWatch() {
		cerr << duration_cast< Duration >( Clock::now() - _ ).count() << endl;
	}
};
