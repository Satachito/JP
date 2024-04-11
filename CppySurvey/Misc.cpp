#include	"JPVideo.h"

#define QUARTET( a, b, c, d ) ( a | b << 8 | c << 16 | d << 24 )

#define Q_ftyp	QUARTET( 'f', 't', 'y', 'p' )
#define Q_styp	QUARTET( 's', 't', 'y', 'p' )
#define Q_mvhd	QUARTET( 'm', 'v', 'h', 'd' )
#define Q_coin	QUARTET( 'c', 'o', 'i', 'n' )
#define Q_hdlr	QUARTET( 'h', 'd', 'l', 'r' )
#define Q_xml	QUARTET( 'x', 'm', 'l', ' ' )
#define Q_iloc	QUARTET( 'i', 'l', 'o', 'c' )
#define Q_idat	QUARTET( 'i', 'd', 'a', 't' )
#define Q_tkhd	QUARTET( 't', 'k', 'h', 'd' )
#define Q_elst	QUARTET( 'e', 'l', 's', 't' )
#define Q_mdhd	QUARTET( 'm', 'd', 'h', 'd' )
#define Q_hdlr	QUARTET( 'h', 'd', 'l', 'r' )
#define Q_vmhd	QUARTET( 'v', 'm', 'h', 'd' )
#define Q_smhd	QUARTET( 's', 'm', 'h', 'd' )
#define Q_sthd	QUARTET( 's', 't', 'h', 'd' )
#define Q_dref	QUARTET( 'd', 'r', 'e', 'f' )
#define Q_frma	QUARTET( 'f', 'r', 'm', 'a' )
#define Q_schm	QUARTET( 's', 'c', 'h', 'm' )
#define Q_tenc	QUARTET( 't', 'e', 'n', 'c' )
#define Q_stts	QUARTET( 's', 't', 't', 's' )
#define Q_stsc	QUARTET( 's', 't', 's', 'c' )
#define Q_stsz	QUARTET( 's', 't', 's', 'z' )
#define Q_stz2	QUARTET( 's', 't', 'z', '2' )
#define Q_stco	QUARTET( 's', 't', 'c', 'o' )
#define Q_mehd	QUARTET( 'm', 'e', 'h', 'd' )
#define Q_trex	QUARTET( 't', 'r', 'e', 'x' )
#define Q_pssh	QUARTET( 'p', 's', 's', 'h' )
#define Q_free	QUARTET( 'f', 'r', 'e', 'e' )
#define Q_sidx	QUARTET( 's', 'i', 'd', 'x' )
#define Q_emsg	QUARTET( 'e', 'm', 's', 'g' )
#define Q_mfhd	QUARTET( 'm', 'f', 'h', 'd' )
#define Q_tfhd	QUARTET( 't', 'f', 'h', 'd' )
#define Q_tfdt	QUARTET( 't', 'f', 'd', 't' )
#define Q_trun	QUARTET( 't', 'r', 'u', 'n' )
#define Q_avcn	QUARTET( 'a', 'v', 'c', 'n' )
#define Q_senc	QUARTET( 's', 'e', 'n', 'c' )
#define Q_saio	QUARTET( 's', 'a', 'i', 'o' )
#define Q_saiz	QUARTET( 's', 'a', 'i', 'z' )
#define Q_sbgp	QUARTET( 's', 'b', 'g', 'p' )
#define Q_sgpd	QUARTET( 's', 'g', 'p', 'd' )
#define Q_mdat	QUARTET( 'm', 'd', 'a', 't' )
#define Q_tfra	QUARTET( 't', 'f', 'r', 'a' )
#define Q_mfro	QUARTET( 'm', 'f', 'r', 'o' )

#define Q_avc1	QUARTET( 'a', 'v', 'c', '1' )
#define Q_hvc1	QUARTET( 'h', 'v', 'c', '1' )
#define Q_hev1	QUARTET( 'h', 'e', 'v', '1' )

#define Q_encv	QUARTET( 'e', 'n', 'c', 'v' )
#define Q_avcC	QUARTET( 'a', 'v', 'c', 'C' )
#define Q_hvcC	QUARTET( 'h', 'v', 'c', 'C' )
#define Q_enca	QUARTET( 'e', 'n', 'c', 'a' )
#define Q_mp4a	QUARTET( 'm', 'p', '4', 'a' )

#define Q_moov	QUARTET( 'm', 'o', 'o', 'v' )
#define Q_trak	QUARTET( 't', 'r', 'a', 'k' )
#define Q_edts	QUARTET( 'e', 'd', 't', 's' )
#define Q_mdia	QUARTET( 'm', 'd', 'i', 'a' )
#define Q_minf	QUARTET( 'm', 'i', 'n', 'f' )
#define Q_dinf	QUARTET( 'd', 'i', 'n', 'f' )
#define Q_stbl	QUARTET( 's', 't', 'b', 'l' )
#define Q_sinf	QUARTET( 's', 'i', 'n', 'f' )
#define Q_schi	QUARTET( 's', 'c', 'h', 'i' )
#define Q_mvex	QUARTET( 'm', 'v', 'e', 'x' )
#define Q_moof	QUARTET( 'm', 'o', 'o', 'f' )
#define Q_traf	QUARTET( 't', 'r', 'a', 'f' )
#define Q_mfra	QUARTET( 'm', 'f', 'r', 'a' )
#define Q_udta	QUARTET( 'u', 'd', 't', 'a' )
#define Q_ilst	QUARTET( 'i', 'l', 's', 't' )
#define Q_too	QUARTET( 0xa9, 't', 'o', 'o' )

#define Q_meta	QUARTET( 'm', 'e', 't', 'a' )
#define Q_stsd	QUARTET( 's', 't', 's', 'd' )
#define Q_dref	QUARTET( 'd', 'r', 'e', 'f' )
#define Q_prft	QUARTET( 'p', 'r', 'f', 't' )

#define	SIMPLE_NODES \
		case Q_moov	: \
		case Q_trak	: \
		case Q_edts	: \
		case Q_mdia	: \
		case Q_minf	: \
		case Q_dinf	: \
		case Q_stbl	: \
		case Q_sinf	: \
		case Q_schi	: \
		case Q_mvex	: \
		case Q_moof	: \
		case Q_traf	: \
		case Q_mfra	: \
		case Q_udta	: \
		case Q_ilst	: \
		case Q_too	: \


inline UI4
Type( const UI1* $ ) {
	return *(UI4*)$;
}

inline string
TypeString( const UI4 $ ) {
	return $ == Q_too ? ".too" : string( (char*)&$, 4 );
}

inline char*	//	IC
LocalTimeString_MP4TimeStamp( UI8 $ ) {
	time_t _ = $ - 3600 * 24 * ( 365 * 66 + 17 );	//	Jan. 1, 1904
	return asctime( localtime( &_ ) );
}
inline char*
GMTimeString_MP4TimeStamp( UI8 $ ) {
	time_t _ = $ - 3600 * 24 * ( 365 * 66 + 17 );	//	Jan. 1, 1904
	return asctime( gmtime( &_ ) );
}

struct
Atom {
	UI4				type;
	vector< UI1 >	$;
	vector< Atom* >	children;

	Atom( UI4 type, const UI1* $, UI8 _, const vector< Atom* >& children )
	:	type( type )
	,	$( $, $ + _ )
	,	children( children ) {
	}

	Atom( UI4 type, const vector< Atom* >& children )
	:	type( type )
	,	children( children ) {
	}

	Atom( UI4 type, const UI1* $, UI8 _ )
	:	type( type )
	,	$( $, $ + _ ) {
	}

	Atom( UI4 type )
	:	type( type ) {
	}

	virtual ~
	Atom() {
		for ( auto $: children ) delete $;
	}

	UI8
	Size() const {
		auto $ = RawSize() + 8;
		return $ < 4294967296 ? $ : $ + 8;
	}

	UI8
	RawSize() const {
		return $.size() + accumulate( children.begin(), children.end(), 0, []( UI8 $, Atom* child ) { return $ + child->Size(); } );
	}
	
	UI8
	Write( UI1* ptr ) const {
//cerr << TypeString( type ) << endl;
		auto	_		= ptr;
		UI8		size	= Size();
		if ( size < 4294967296 ) {
			size = Swap4( (UI4)size );
			memcpy( _, &size, 4 );		_ += 4;
			memcpy( _, &type, 4 );		_ += 4;
		} else {
			UI4	one = Swap4( 1 );
			memcpy( _, &one , 4 );		_ += 4;
			memcpy( _, &type, 4 );		_ += 4;
			size = Swap8( size );
			memcpy( _, &size, 8 );		_ += 8;
		}
		memcpy( _, &$[ 0 ], $.size() );	_ += $.size();
		for ( auto $: children ) _ += $->Write( _ );
		return _ - ptr;
	}

	void
	Write( int fd ) const {
		UI8	size = Size();
		if ( size < 4294967296 ) {
			size = Swap4( (UI4)size );
			::Write( fd, &size, 4 );
			::Write( fd, &type, 4 );
		} else {
			UI4	one = Swap4( 1 );
			::Write( fd, &one, 4 );
			::Write( fd, &type, 4 );
			size = Swap8( size );
			::Write( fd, &size, 8 );
		}
		::Write( fd, &$[ 0 ], $.size() );
		for ( auto $: children ) $->Write( fd );
	}

	static vector< Atom* >
	Filter( const vector< Atom* >& $, UI4 _ ) {
		vector< Atom* >	$$;
		for ( auto $: $ ) if ( $->type == _ ) $$.emplace_back( $ );
		return $$;
	}

	vector< Atom* >
	Children( UI4 _ ) const {
		return Filter( children, _ );
	}

	Atom*
	Child( UI4 _ ) const {
		auto $ = this->Children( _ );
		A( $.size() == 1 );
		return $[ 0 ];
	}
	
	UI8
	Offset( Atom* _ ) const {
		auto offset = ( RawSize() + 8 < 4294967296 ? 8 : 16 ) + $.size();
		for ( auto child: children ) {
			if ( child == _ ) return offset;
			try {
				return offset + child->Offset( _ );
			} catch (...) {
				offset += child->Size();
			}
		}
		throw 0;
	}
};

namespace
MP4 {
	inline vector< Atom* >
	Build( const UI1* $, UI8 _ ) {

		vector< Atom* > $$;
		const UI1*	end = $ + _;
		while ( $ < end ) {
			auto	size = Size4( $ );
			auto	type = Type( $ + 4 );

			$ += 8;
			UI8		headSize = 8;
			if ( size == 1 ) {
				size = Size8( $ );
				$ += 8;
				headSize += 8;
			}
			size -= headSize;

			switch ( type ) {
			case Q_mp4a	:
			case Q_enca	:
				$$.emplace_back( new Atom( type, $, 28, Build( $ + 28, size - 28 ) ) );
				break;
			case Q_meta	:
				$$.emplace_back( new Atom( type, $,  4, Build( $ +  4, size -  4 ) ) );
				break;
			case Q_dref	:
				$$.emplace_back( new Atom( type, $,  8, Build( $ +  8, size -  8 ) ) );
				break;
			case Q_stsd	:
				$$.emplace_back( new Atom( type, $,  8, Build( $ +  8, size -  8 ) ) );
				break;
			case Q_hvc1	:
			case Q_hev1	:
				$$.emplace_back( new Atom( type, $, 78, Build( $ + 78, size - 78 ) ) );
				break;
			case Q_avc1	:
				$$.emplace_back( new Atom( type, $, 78, Build( $ + 78, size - 78 ) ) );
				break;
			SIMPLE_NODES
				$$.emplace_back( new Atom( type, Build( $, size ) ) );
				break;
			default		:
				$$.emplace_back( new Atom( type, $, size ) );
				break;
			}

			$ += size;
		}
		return $$;
	}

	inline vector< Atom* >
	Build( const vector< UI1 >& _ ) {
		return Build( _.data(), _.size() );
	}

	inline vector< UI1 >
	Serialize( const vector< Atom* >& _ ) {
		auto size = UI8( 0 );
		for ( auto& _: _ ) size += _->Size();
		vector< UI1 >$( size );
		auto pointer = $.data();
		for ( auto& _: _ ) {
			_->Write( pointer );
			pointer += _->Size();
		}
	A( pointer - $.data() == size );
		return $;
	}

	template < typename F > void
	Crawl( const vector< Atom* >& $, F _ ) {
		for ( auto $: $ ) {
			_( $ );
			Crawl( $->children, _ );
		}
	}

	inline Atom*
	Find( const vector< Atom* >& $, UI4 _ ) {
		Atom* $$ = 0;
		Crawl( $, [ & ]( Atom* $ ) { if ( $->type == _ ) $$ = $; } );
		return $$;
	}

}


void
Main() {
	AtomVector
}

int
main( int argc, char* argv[] ) {
	Main();
	return 0;
}
