#pragma once

#include	"JP.h"

namespace JP {
	inline unsigned char
	HexNum( const char $ ) {
		switch ( $ ) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':	return $ - '0';
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':											return $ - 'A' + 10;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':											return $ - 'a' + 10;
		default:	throw "eh?";
		}
	}
	inline vector< unsigned char >
	DecodeHex( const string& $ ) {
		vector< unsigned char >	$$;
		for ( auto _ = 0; _ < $.size(); _ += 2 ) $$.emplace_back( HexNum( $[ _ ] ) << 4 | HexNum( $[ _ + 1 ] ) );
		return $$;
	}

	inline char
	HexChar( unsigned char $ ) {
		switch ( $ ) {
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
			return $ + '0';
		case 10: case 11: case 12: case 13: case 14: case 15:
			return $ - 10 + 'a';
		default:	throw "eh?";
		}
	}
	inline string
	HexStr( unsigned char $ ) {
		char	_[] = {
			HexChar( ( $ >> 4 ) & 0x0f )
		,	HexChar( $ & 0x0f )
		,	0
		};
		return string( _ );
	}
	inline string
	EncodeHex( unsigned char* _, unsigned char* $ ) {
		string	$$;
		while ( _ < $ ) $$ += HexStr( *_++ );
		return $$;
	}
	inline string
	EncodeHex( unsigned char* _, unsigned long $ ) {
		return EncodeHex( _, _ + $ );
	}
	inline string
	EncodeHex( const vector< unsigned char >& $ ) {
		return EncodeHex( (unsigned char*)&$[ 0 ], $.size() );
	}
	inline string
	EncodeHexLF16( unsigned char* _, unsigned long $ ) {
		string	$$;
		for ( auto i = 0; i < $; i += 16 ) {
			auto nRemain = $ - i;
			$$ += to_string( i ) + ':' + EncodeHex( _ + i, _ + i + ( nRemain < 16 ? nRemain : 16 ) ) + '\n';
		}
		return $$;
	}
	inline string
	EncodeHexLF16( unsigned char* _, unsigned char* $ ) {
		return EncodeHexLF16( _, $ - _ );
	}
	inline string
	EncodeHexLF16( const vector< unsigned char >& _ ) {
		return EncodeHexLF16( (unsigned char*)&_[ 0 ], _.size() );
	}
}
