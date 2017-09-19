//	Written by Satoru Ogura, Tokyo.
//
#include	<iostream>

inline	int
_U( int p, const char* file, int line ) {
	if ( p == -1 ) {
		fprintf( stderr, "%s:%d:%s\n", file, line, strerror( errno ) );
		throw 0;
	}
	return p;
}
#define	U( p ) _U( p, __FILE__, __LINE__ )

template	< typename T >	T*
_N( T* p, const char* file, int line ) {
	if ( ! p ) {
		fprintf( stderr, "%s:%d:NULL\n", file, line );
		throw 0;
	}
	return p;
}
#define	N( p ) _N( p, __FILE__, __LINE__ )


#include	<vector>

namespace
JP {
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
					v = UnicodeStream::Read();
					while ( v != '\n' ) v = UnicodeStream::Read();
					goto RESET;	//	return Read();
				} else {
					Unread( w );
					return v;
				}
			}
			return v;
		}
	};

	template	< typename T >	std::vector<T>
	operator+ ( std::vector<T> const& l, std::vector<T> const& r ) {
		std::vector<T> v( l.size() + r.size() );
		v.insert( v.end(), l.begin(), l.end() );
		v.insert( v.end(), r.begin(), r.end() );
		return v;
	}
}
