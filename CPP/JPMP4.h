#pragma once
//	Written by Satoru Ogura. (C)SliP LLC, Tokyo.

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
#define Q_prft	QUARTET( 'p', 'r', 'f', 't' )
#define Q_esds	QUARTET( 'e', 's', 'd', 's' )

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

inline	UI8
AtomBufferSize( const unsigned char* $ ) {
	auto _ = Size4( $ );
	return _ == 1 ? Size8( $ + 8 ) : _;
}

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

inline UI4		Type( Atom* $, UI8 start ) { return Type( &$->$[ start ] ); }
inline UI8		Size8( Atom* $, UI8 start ) { return Size8( &$->$[ start ] ); }
inline UI8		Size4( Atom* $, UI8 start ) { return Size4( &$->$[ start ] ); }
inline UI8		Size3( Atom* $, UI8 start ) { return Size3( &$->$[ start ] ); }
inline UI8		Size2( Atom* $, UI8 start ) { return Size2( &$->$[ start ] ); }
inline UI8		Size1( Atom* $, UI8 start ) { return Size1( &$->$[ start ] ); }

inline Atom*
NonTextFirstTrak( const Atom* moov ) {
	vector< Atom* > $;
	for ( auto trak: moov->Children( Q_trak ) ) if ( Type( &trak->Child( Q_mdia )->Child( Q_hdlr )->$[ 8 ] ) != 0x74786574 ) $.emplace_back( trak );
	return $[ 0 ];
}

inline UI8
BaseMediaDecodeTime( const Atom* moof ) {
	auto tfdt = moof->Child( Q_traf )->Child( Q_tfdt );
	return tfdt->$[ 0 ]
	?	Size8( &tfdt->$[ 4 ] )
	:	Size4( &tfdt->$[ 4 ] )
	;
}

inline UI8
TimeScale_trak( const Atom* trak ) {
    auto mdhd = trak->Child( Q_mdia )->Child( Q_mdhd );
    return Size4( &mdhd->$[ mdhd->$[ 0 ] ? 20 : 12 ] );
}

inline UI8
TimeScale_moov( const Atom* moov ) {
    return TimeScale_trak( NonTextFirstTrak( moov ) );
}

inline UI8
TimeScale( const Atom* moov ) {
	return TimeScale_trak( moov->Children( Q_trak )[ 0 ] );
}


inline int
TrunHeaderSize( const Atom* trun ) {
	auto $ = 8;
	auto vf = trun->$[ 3 ];
	if ( vf &    1 ) $ += 4;
	if ( vf &    2 ) $ += 4;
	if ( vf &    4 ) $ += 4;
	if ( vf &    8 ) $ += 4;
	if ( vf & 0x10 ) $ += 4;
	if ( vf & 0x20 ) $ += 4;
	if ( vf & 0x40 ) $ += 4;
	if ( vf & 0x80 ) $ += 4;
	return $;
}
inline int
TrunEntrySize( const Atom* trun ) {
	auto $ = 0;
	auto vf = trun->$[ 2 ];
	if ( vf &    1 ) $ += 4;
	if ( vf &    2 ) $ += 4;
	if ( vf &    4 ) $ += 4;
	if ( vf &    8 ) $ += 4;
	if ( vf & 0x10 ) $ += 4;
	if ( vf & 0x20 ) $ += 4;
	if ( vf & 0x40 ) $ += 4;
	if ( vf & 0x80 ) $ += 4;
	return $;
}
inline int
TrunSizeIndex( const Atom* trun ) {
	auto vf = trun->$[ 2 ];
	return vf & 1 ? 4 : 0;
}

inline UI8
NumSamples( const Atom* moof ) {
	return Size4( &moof->Child( Q_traf )->Child( Q_trun )->$[ 4 ] );
}

inline UI8
DefaultSampleDuration( const Atom* moof ) {
	auto tfhd = moof->Child( Q_traf )->Child( Q_tfhd );
	auto tfhdFlags = tfhd->$[ 3 ];
	if ( tfhdFlags & 0x08 ) {
		auto dsdIndex = 8;
		if ( tfhdFlags & 0x01 ) dsdIndex += 8;		// BaseDataOffset
		if ( tfhdFlags & 0x02 ) dsdIndex += 4;		// SampleDescriptionIndex
		return Size4( tfhd, dsdIndex );
	}
	throw "eh?";
}

inline UI8
Duration_trun( const Atom* moof ) {

	auto trun = moof->Child( Q_traf )->Child( Q_trun );
	auto trunFlags = Size4( trun, 0 );
	if ( trunFlags & 0x100 ) {
		auto nEntries = Size4( trun, 4 );
		auto trunHeaderSize = TrunHeaderSize( trun );
		auto trunEntrySize = TrunEntrySize( trun );
		auto $ = UI8( 0 );
		for ( auto _ = 0; _ < nEntries; _++ ) $ += Size4( trun, trunHeaderSize + trunEntrySize * _ );
		return $;
	}
	throw "eh?";
}

inline double
Duration( const Atom* moov, const Atom* moof ) {
	try {
		return Duration_trun( moof ) / (double)TimeScale_moov( moov );
	} catch (...) {
		return DefaultSampleDuration( moof ) / (double)TimeScale_moov( moov ) * NumSamples( moof );
	}
}

inline UI8
NumSamplesInDurationHead( double* duration, const Atom* moov, const Atom* moof ) {

	auto timeScale = (double)TimeScale_moov( moov );

	auto trun = moof->Child( Q_traf )->Child( Q_trun );
	auto trunFlags = Size4( trun, 0 );
	if ( trunFlags & 0x100 ) {
		auto nEntries = Size4( trun, 4 );
		auto trunHeaderSize = TrunHeaderSize( trun );
		auto trunEntrySize = TrunEntrySize( trun );
		auto D = UI8( 0 );
		auto $ = 0;
		while ( $ < nEntries ) {
			if ( D >= *duration * timeScale ) break;
			D += Size4( trun, trunHeaderSize + trunEntrySize * $++ );
		}
		*duration = D / timeScale;
		return $;
	} else {
		auto dsd = DefaultSampleDuration( moof ) / timeScale;
		auto $ = (UI8)ceil( *duration / dsd );
		*duration = $ * dsd;
		return $;
	}
}

inline UI8
NumSamplesInDurationTail( double* duration, const Atom* moov, const Atom* moof ) {

	auto timeScale = (double)TimeScale_moov( moov );

	auto trun = moof->Child( Q_traf )->Child( Q_trun );
	auto trunFlags = Size4( trun, 0 );
	if ( trunFlags & 0x100 ) {
		auto nEntries = Size4( trun, 4 );
		auto trunHeaderSize = TrunHeaderSize( trun );
		auto trunEntrySize = TrunEntrySize( trun );
		auto D = UI8( 0 );
		auto $ = nEntries;
		while ( $ ) {
			if ( D >= *duration * timeScale ) break;
			D += Size4( trun, trunHeaderSize + trunEntrySize * --$ );
		}
		*duration = D / timeScale;
		return nEntries - $;
	} else {
		auto dsd = DefaultSampleDuration( moof ) / timeScale;
		auto $ = (UI8)ceil( *duration / dsd );
		*duration = $ * dsd;
		return $;
	}
}

inline UI8
NumSamplesInDuration( double* duration, const Atom* moov, const Atom* moof ) {
	return NumSamplesInDurationHead( duration, moov, moof );
}
inline void
DumpSkelton( Atom* $, ostream& s, UI8 level );
inline void
DumpSkelton( const vector< Atom* >& $, ostream& s, UI8 level = 0 ) {
	for ( auto $ : $ ) DumpSkelton( $, s, level );
}
inline void
DumpSkelton( Atom* $, ostream& s, UI8 level ) {
	s << level;
	for ( auto i = 0; i <= level; i++ ) s << '\t';
	s << TypeString( $->type ) << ':' << $->Size() << endl;
	switch ( $->type ) {
	case Q_stsd	:
	case Q_meta	:
	case Q_dref	:
	case Q_mp4a	:
	SIMPLE_NODES
		DumpSkelton( $->children, s, level + 1 );
		break;
	}
}

inline void		Dump( Atom* $, ostream& s, unsigned long& IV_Size, UI8 level );
inline void		Dump( const vector< Atom* >& $, ostream& s, unsigned long& IV_Size, UI8 level = 0 ) { for ( auto $ : $ ) Dump( $, s, IV_Size, level ); }

inline string	EncodeHex( Atom* $, UI8 start, UI8 size ) { return EncodeHex( &$->$[ start ], size ); }
inline string	EncodeHexLF16( Atom* $, UI8 start, UI8 size ) { return EncodeHexLF16( &$->$[ start ], size ); }

inline void
Dump( Atom* $, ostream& s, unsigned long& IV_Size, UI8 level ) {
	static int mdatType = 0;

	s << level;
	for ( auto i = 0; i <= level; i++ ) s << '\t';
	s << TypeString( $->type ) << ':' << $->Size() << endl;
	switch ( $->type ) {
	case Q_stsd	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		s << "Number of entries( Sample description ):" << Size4( $, 4 ) << endl;
		Dump( $->children, s, IV_Size, level + 1 );
		break;
	case Q_meta	:
		Dump( $->children, s, IV_Size, level + 1 );
		break;
	case Q_dref	:
		Dump( $->children, s, IV_Size, level + 1 );
		break;
	case Q_mp4a	:
	case Q_enca	:
		s << EncodeHexLF16( $, 0, $->$.size() ) << endl;
		Dump( $->children, s, IV_Size, level + 1 );
		break;
	SIMPLE_NODES
		Dump( $->children, s, IV_Size, level + 1 );
		break;
	case Q_ftyp	:
	case Q_styp	:
		for ( auto _ = 8; _ < $->$.size(); _ += 4 ) s << ':' << TypeString( Type( $, _ ) );
		s << endl;
		break;
	case Q_pssh	:
		s << "Version + Flags:"	<< EncodeHex( $, 0, 4 ) << endl;
		s << "SystemID:"		<< EncodeHex( $, 4, 16 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)20;
			if ( vf >> 24 ) {
				auto nKID = Size4( $, pos );										pos += 4;
				for ( auto _ = 0; _ < nKID; _++ ) {
					s << "KID #" << _ << ':' << EncodeHex( $, pos, 16 ) << endl;	pos += 16;
				}
			}
			auto size = Size4( $, pos );											pos += 4;
			s << "Data:"		<< EncodeHex( $, pos, size ) << endl;				pos += size;
			s << "Extra:"		<< EncodeHex( $, pos, $->$.size() - pos ) << endl;
		}
		break;
	case Q_trex	:
		s << "Version + Flags:"					<< EncodeHex( $, 0, 4 ) << endl;
		s << "TrackId:"							<< Size4( $,  4 ) << endl;
		s << "DefaultSampleDescriptionIndex:"	<< Size4( $,  8 ) << endl;
		s << "DefaultSampleDuration:"			<< Size4( $, 12 ) << endl;
		s << "DefaultSampleSize:"				<< Size4( $, 16 ) << endl;
		s << "DefaultSampleFlags:"				<< Size4( $, 20 ) << endl;
		break;
	case Q_senc	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)4;

//const UI32 CENC_SAMPLE_ENCRYPTION_FLAG_OVERRIDE_TRACK_ENCRYPTION_DEFAULTS = 1;
//const UI32 CENC_SAMPLE_ENCRYPTION_FLAG_USE_SUB_SAMPLE_ENCRYPTION          = 2;
			if ( vf & 1 ) {
				s << " AlgorithmId(3) + PerSampleIvSize(1):" << Size4( $, 4 ) << endl;					pos +=  4;
				s << " Kid:" << EncodeHex( $, 8, 16 ) << endl;											pos += 16;
			}
			auto sampleCount = Size4( $, pos );															pos += 4;
			s << " SampleCount:" << sampleCount << endl;
			for ( auto i = 0; i < sampleCount; i++ ) {
				if ( IV_Size ) {
					s << " InitializationVector:" << i << ':' << EncodeHex( $, pos, IV_Size ) << endl;	pos += IV_Size;
				}
				if ( vf & 2 ) {
					auto subSampleCount = Size2( $, pos );												pos += 2;
					for ( auto j = 0; j < subSampleCount; j++ ) {
						auto nClear = Size2( $, pos );													pos += 2;
						auto nCrypt = Size4( $, pos );													pos += 4;
						s << i << ':' << j << ": Clear:" << nClear << " Enc:" << nCrypt<< endl;
					}
				}
			}
		}
		break;
	case Q_tkhd	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto version = Size4( $, 0 ) >> 24;
			auto pos = (UI8)4;
			pos += version ? 16 : 8;
			s << "TrackId:"	<< Size4( $, pos ) << endl; pos += 4;
//			pos += version ? 12 : 8;
//			pos += 16 + 9 * 4;
//			s << "Width:"	<< Size4( $, pos ) << endl; pos += 4;
//			s << "Height:"	<< Size4( $, pos ) << endl; pos += 4;
		}
		break;
	case Q_tfhd	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
//const UI32 TFHD_FLAG_BASE_DATA_OFFSET_PRESENT         = 0x00001;
//const UI32 TFHD_FLAG_SAMPLE_DESCRIPTION_INDEX_PRESENT = 0x00002;
//const UI32 TFHD_FLAG_DEFAULT_SAMPLE_DURATION_PRESENT  = 0x00008;
//const UI32 TFHD_FLAG_DEFAULT_SAMPLE_SIZE_PRESENT      = 0x00010;
//const UI32 TFHD_FLAG_DEFAULT_SAMPLE_FLAGS_PRESENT     = 0x00020;
//const UI32 TFHD_FLAG_DURATION_IS_EMPTY                = 0x10000;
//const UI32 TFHD_FLAG_DEFAULT_BASE_IS_MOOF             = 0x20000;
			auto pos = (UI8)4;
			s << "TrackId:"											<< Size4( $, pos )			<< endl; pos += 4;
			if ( vf & 0x00001 ) { s << "BaseDataOffset:"			<< Size8( $, pos )			<< endl; pos += 8; }
			if ( vf & 0x00002 ) { s << "SampleDescriptionIndex:"	<< Size4( $, pos )			<< endl; pos += 4; }
			if ( vf & 0x00008 ) { s << "DefaultSampleDuration:"		<< Size4( $, pos )			<< endl; pos += 4; }
			if ( vf & 0x00010 ) { s << "DefaultSampleSize:"			<< Size4( $, pos )			<< endl; pos += 4; }
			if ( vf & 0x00020 ) { s << "DefaultSampleFlags:"		<< EncodeHex( $, pos, 4 )	<< endl; pos += 4; }
		}
		break;
	case Q_tfdt :
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		s << "BaseMediaDecodeTime:" << ( Size4( $, 0 ) >> 24 ? Size8( $, 4 ) : Size4( $, 4 ) ) << endl;
		break;
	case Q_hvc1	:
	case Q_hev1	:
	case Q_avc1	:
		{	auto pos = (UI8)0;
			s << "  Reserved:"				<< EncodeHex( $, pos, 6 )	<< endl;	pos +=  6;
			s << "  Data reference index:"	<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  Predefined1:"			<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  Reserved2:"				<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  Predefined2:"			<< EncodeHex( $, pos, 12 )	<< endl;	pos += 12;
			s << "  Width:"					<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  Height:"				<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  HorizResolution:"		<< Size4( $, pos )			<< endl;	pos +=  4;
			s << "  VertResolution:"		<< Size4( $, pos )			<< endl;	pos +=  4;
			s << "  Reserved3:"				<< Size4( $, pos )			<< endl;	pos +=  4;
			s << "  FrameCount:"			<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  CompressorName:"		<< string(
				&$->$[ pos + 1 ]
			,	&$->$[ pos + 1 ] + $->$[ pos ]
			)															<< endl;	pos += 32;
			s << "  Depth:"					<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  Predefined3:"			<< Size2( $, pos )			<< endl;	pos +=  2;
			s << "  INDEX:		"			<< pos						<< endl;
			Dump( $->children, s, IV_Size, level + 1 );
		}
		mdatType = $->type;
		break;
	case Q_hvcC	:
		s << "ConfigurationVersion:"				<< Size1( $, 0 )							<< endl;
		s << "GeneralProfileSpace:"					<< ( ( Size1( $, 1 ) >> 6 ) & 0x03 )		<< endl;
		s << "GeneralTierFlag:"						<< ( ( Size1( $, 1 ) >> 5 ) & 0x01 )		<< endl;
		s << "GeneralProfile:"						<< ( Size1( $, 1 ) & 0x1f )					<< endl;
		s << "GeneralProfileCompatibilityFlags:"	<< Size4( $, 2 )							<< endl;
		s << "GeneralConstraintIndicatorFlags:"		<< ( Size4( $, 6 ) << 16 | Size2( $, 10 ) )	<< endl;
		s << "GeneralLevel:"						<< Size1( $, 12 )							<< endl;
		s << "Reserved1:"							<< ( Size1( $, 13 ) >> 4 & 0x0f )			<< endl;
		s << "MinSpatialSegmentation:"				<< ( Size2( $, 13 ) & 0x0fff )				<< endl;
		s << "Reserved2:"							<< ( Size1( $, 15 ) >> 2 & 0x3f )			<< endl;
		s << "ParallelismType:"						<< ( Size1( $, 15 ) & 0x03 )				<< endl;
		s << "Reserved3:"							<< ( Size1( $, 16 ) >> 2 & 0x3f )			<< endl;
		s << "ChromaFormat:"						<< ( Size1( $, 16 ) & 0x03 )				<< endl;
		s << "Reserved4:"							<< ( Size1( $, 17 ) >> 3 & 0x1f )			<< endl;
		s << "LumaBitDepth:"						<< 8 + ( Size1( $, 17 ) & 0x07 )			<< endl;
		s << "Reserved5:"							<< ( Size1( $, 18 ) >> 3 & 0x1f )			<< endl;
		s << "ChromaBitDepth:"						<< 8 + ( Size1( $, 18 ) & 0x07 )			<< endl;
		s << "AverageFrameRate:"					<< Size2( $, 19 )							<< endl;
		s << "ConstantFrameRate:"					<< ( Size1( $, 21 ) >> 6 & 0x03 )			<< endl;
		s << "NumTemporalLayers:"					<< ( Size1( $, 21 ) >> 3 & 0x07 )			<< endl;
		s << "TemporalIdNested:"					<< ( Size1( $, 21 ) >> 2 & 0x01 )			<< endl;
		s << "NaluLengthSize:"						<< 1 + ( Size1( $, 21 ) & 0x03 )			<< endl;
		break;
	case Q_avcC	:
		s << "ConfigurationVersion:"	<< Size1( $, 0 )				<< endl;
		s << "Profile:"					<< Size1( $, 1 )				<< endl;
		s << "ProfileCompatibility:"	<< Size1( $, 2 )				<< endl;
		s << "Level:"					<< Size1( $, 3 )				<< endl;
		s << "NaluLengthSize:"			<< 1 + ( Size1( $, 4 ) & 3 )	<< endl;

		{	auto nSeqParams = Size1( $, 5 ) & 0x1f;
			auto spekePos = (UI8)6;
			AvcSequenceParameterSet SPSs[ nSeqParams ];
			for ( auto _ = 0; _ < nSeqParams; _++ ) {
				auto paramLength = Size2( $, spekePos );
				spekePos += 2;
				auto sps = AvcSPS( &$->$[ 0 ] + spekePos );
s << "	profile_idc	:" <<	sps.profile_idc << endl;
s << "	constraint_set0_flag	:" <<	sps.constraint_set0_flag << endl;
s << "	constraint_set1_flag	:" <<	sps.constraint_set1_flag << endl;
s << "	constraint_set2_flag	:" <<	sps.constraint_set2_flag << endl;
s << "	constraint_set3_flag	:" <<	sps.constraint_set3_flag << endl;
s << "	constraint_set4_flag	:" <<	sps.constraint_set4_flag << endl;
s << "	constraint_set5_flag	:" <<	sps.constraint_set5_flag << endl;
s << "	level_idc	:" <<	sps.level_idc << endl;
s << "	seq_parameter_set_id	:" <<	sps.seq_parameter_set_id << endl;
s << "	chroma_format_idc	:" <<	sps.chroma_format_idc << endl;
s << "	separate_colour_plane_flag	:" <<	sps.separate_colour_plane_flag << endl;
s << "	bit_depth_luma_minus8	:" <<	sps.bit_depth_luma_minus8 << endl;
s << "	bit_depth_chroma_minus8	:" <<	sps.bit_depth_chroma_minus8 << endl;
s << "	qpprime_y_zero_transform_bypass_flag	:" <<	sps.qpprime_y_zero_transform_bypass_flag << endl;
s << "	seq_scaling_matrix_present_flag	:" <<	sps.seq_scaling_matrix_present_flag << endl;
//s << "	AvcScalingList4x4scaling_list_4x4[6]	:" <<	sps.AvcScalingList4x4scaling_list_4x4[6] << endl;
//s << "	use_default_scaling_matrix_4x4[AVC_SPS_MAX_SCALING_LIST_COUNT]	:" <<	sps.use_default_scaling_matrix_4x4[AVC_SPS_MAX_SCALING_LIST_COUNT] << endl;
//s << "	AvcScalingList4x4scaling_list_8x8[6]	:" <<	sps.AvcScalingList4x4scaling_list_8x8[6] << endl;
//s << "	use_default_scaling_matrix_8x8[AVC_SPS_MAX_SCALING_LIST_COUNT]	:" <<	sps.use_default_scaling_matrix_8x8[AVC_SPS_MAX_SCALING_LIST_COUNT] << endl;
s << "	log2_max_frame_num_minus4	:" <<	sps.log2_max_frame_num_minus4 << endl;
s << "	pic_order_cnt_type	:" <<	sps.pic_order_cnt_type << endl;
s << "	log2_max_pic_order_cnt_lsb_minus4	:" <<	sps.log2_max_pic_order_cnt_lsb_minus4 << endl;
s << "	delta_pic_order_always_zero_flags	:" <<	sps.delta_pic_order_always_zero_flags << endl;
s << "	offset_for_non_ref_pic	:" <<	sps.offset_for_non_ref_pic << endl;
s << "	offset_for_top_to_bottom_field	:" <<	sps.offset_for_top_to_bottom_field << endl;
s << "	num_ref_frames_in_pic_order_cnt_cycle	:" <<	sps.num_ref_frames_in_pic_order_cnt_cycle << endl;
//s << "	offset_for_ref_frame[AVC_SPS_MAX_NUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE]	:" <<	sps.offset_for_ref_frame[AVC_SPS_MAX_NUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE] << endl;
s << "	num_ref_frames	:" <<	sps.num_ref_frames << endl;
s << "	gaps_in_frame_num_value_allowed_flag	:" <<	sps.gaps_in_frame_num_value_allowed_flag << endl;
s << "	pic_width_in_mbs_minus1	:" <<	sps.pic_width_in_mbs_minus1 << endl;
s << "	pic_height_in_map_units_minus1	:" <<	sps.pic_height_in_map_units_minus1 << endl;
s << "	frame_mbs_only_flag	:" <<	sps.frame_mbs_only_flag << endl;
s << "	mb_adaptive_frame_field_flag	:" <<	sps.mb_adaptive_frame_field_flag << endl;
s << "	direct_8x8_inference_flag	:" <<	sps.direct_8x8_inference_flag << endl;
s << "	frame_cropping_flag	:" <<	sps.frame_cropping_flag << endl;
s << "	frame_crop_left_offset	:" <<	sps.frame_crop_left_offset << endl;
s << "	frame_crop_right_offset	:" <<	sps.frame_crop_right_offset << endl;
s << "	frame_crop_top_offset	:" <<	sps.frame_crop_top_offset << endl;
s << "	frame_crop_bottom_offset	:" <<	sps.frame_crop_bottom_offset << endl;
				SPSs[ _ ] = sps;
				spekePos += paramLength;
			}
s << endl;
			auto nPicParams = Size1( $, spekePos );
			spekePos += 1;
			AvcPictureParameterSet PPSs[ nPicParams ];
			for ( auto _ = 0; _ < nPicParams; _++ ) {
				auto paramLength = Size2( $, spekePos );
				spekePos += 2;
				auto pps = AvcPPS( &$->$[ 0 ] + spekePos );
s << "	pic_parameter_set_id	:" <<	pps.pic_parameter_set_id << endl;
s << "	seq_parameter_set_id	:" <<	pps.seq_parameter_set_id << endl;
s << "	entropy_coding_mode_flag	:" <<	pps.entropy_coding_mode_flag << endl;
s << "	pic_order_present_flag	:" <<	pps.pic_order_present_flag << endl;
s << "	num_slice_groups_minus1	:" <<	pps.num_slice_groups_minus1 << endl;
s << "	slice_group_map_type	:" <<	pps.slice_group_map_type << endl;
//s << "	run_length_minus1[AVC_PPS_MAX_SLICE_GROUPS]	:" <<	pps.run_length_minus1[AVC_PPS_MAX_SLICE_GROUPS] << endl;
//s << "	top_left[AVC_PPS_MAX_SLICE_GROUPS]	:" <<	pps.top_left[AVC_PPS_MAX_SLICE_GROUPS] << endl;
//s << "	bottom_right[AVC_PPS_MAX_SLICE_GROUPS]	:" <<	pps.bottom_right[AVC_PPS_MAX_SLICE_GROUPS] << endl;
s << "	slice_group_change_direction_flag	:" <<	pps.slice_group_change_direction_flag << endl;
s << "	slice_group_change_rate_minus1	:" <<	pps.slice_group_change_rate_minus1 << endl;
s << "	pic_size_in_map_units_minus1	:" <<	pps.pic_size_in_map_units_minus1 << endl;
s << "	num_ref_idx_10_active_minus1	:" <<	pps.num_ref_idx_10_active_minus1 << endl;
s << "	num_ref_idx_11_active_minus1	:" <<	pps.num_ref_idx_11_active_minus1 << endl;
s << "	weighted_pred_flag	:" <<	pps.weighted_pred_flag << endl;
s << "	weighted_bipred_idc	:" <<	pps.weighted_bipred_idc << endl;
s << "	pic_init_qp_minus26	:" <<	pps.pic_init_qp_minus26 << endl;
s << "	pic_init_qs_minus26	:" <<	pps.pic_init_qs_minus26 << endl;
s << "	chroma_qp_index_offset	:" <<	pps.chroma_qp_index_offset << endl;
s << "	deblocking_filter_control_present_flag	:" <<	pps.deblocking_filter_control_present_flag << endl;
s << "	constrained_intra_pred_flag	:" <<	pps.constrained_intra_pred_flag << endl;
s << "	redundant_pic_cnt_present_flag	:" <<	pps.redundant_pic_cnt_present_flag << endl;
				PPSs[ _ ] = pps;
				spekePos += paramLength;
			}
		}
/*		{	auto nSeqParams = Size1( $, 5 ) & 0x1f;
			auto pos = (UI8)6;
			for ( auto _ = 0; _ < nSeqParams; _++ ) {
				auto paramLength = Size2( $, pos );
				pos += 2;
				s << "SequenceParameter:" << EncodeHex( $, pos, paramLength ) << endl;
				pos += paramLength;
			}
			auto nPicParams = Size1( $, pos );
			pos += 1;
			for ( auto _ = 0; _ < nPicParams; _++ ) {
				auto paramLength = Size2( $, pos );
				pos += 2;
				s << "PictureParameter:" << EncodeHex( $, pos, paramLength ) << endl;
				pos += paramLength;
			}
		}
*/
		break;
	case Q_schm	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		s << " SchemeType:" << TypeString( Type( $, 4 ) ) << endl;
		s << " SchemeVersion:" << EncodeHex( $, 8, $->$.size() < 12 ? 2 : 4 ) << endl;
		break;
	case Q_tenc	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)4;
			UI2 nBlocks = Size2( $, pos );
			pos += 2;
			if ( vf >> 24 ) {
				s << " DefaultCryptByteBlock:"	<< ( nBlocks >> 8 ) << endl;
				s << " DefaultSkipByteBlock:"	<< ( nBlocks & 0xff ) << endl;
			}
			s << " DefaultIsProtected:"			<< Size1( $, pos ) << endl;
			pos += 1;
			auto defaultPerSampleIvSize = Size1( $, pos );
			IV_Size = defaultPerSampleIvSize;
			pos += 1;
			s << " DefaultPerSampleIvSize:"		<< (int)defaultPerSampleIvSize << endl;
			s << " DefaultKID:"					<< EncodeHex( $, pos, 16 ) << endl;
			pos += 16;
			if ( !defaultPerSampleIvSize ) {
				auto defaultConstantIVSize = Size1( $, pos );
				s << " DefaultConstantIVSize:"	<< (int)defaultConstantIVSize << endl;
				pos += 1;
				s << " DefaultConstantIV:"		<< EncodeHex( $, pos, defaultConstantIVSize ) << endl;
				pos += defaultConstantIVSize;
			}
		}
		break;
	case Q_frma:
		s << TypeString( Type( $, 0 ) ) << endl;
		break;
	case Q_stts	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto nEntries = Size4( $, 4 );
			s << "Number of entries( Time-to-Sample ):" << nEntries << endl;
			auto pos = (UI8)8;
			for ( auto _ = 0; _ < nEntries; _++ ) {
				s << " Sample count:" << Size4( $, pos ) << endl;
				s << " Sample duration:" << Size4( $, pos + 4 ) << endl;
				pos += 8;
			}
		}
		break;
	case Q_stsc	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto nEntries = Size4( $, 4 );
			s << "Number of entries( Sample-to-Chunk ):" << nEntries << endl;
			auto pos = (UI8)8;
			for ( auto _ = 0; _ < nEntries; _++ ) {
				s << " First chunk:" << Size4( $, pos ) << endl;
				s << " Samples per chunk:" << Size4( $, pos + 4 ) << endl;
				s << " Samples description ID:" << Size4( $, pos + 8 ) << endl;
				pos += 12;
			}
		}
		break;
	case Q_stsz	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto sampleSize = Size4( $, 4 );
			s << "Sample size:" << sampleSize << endl;
			auto nEntries = Size4( $, 8 );
			s << "Number of entries( Sample Size ):" << nEntries << endl;
			auto pos = (UI8)12;
			for ( auto _ = 0; _ < nEntries; _++ ) {
				s << " Size:" << Size4( $, pos ) << endl;
				pos += 4;
			}
		}
		break;
	case Q_stco	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto nEntries = Size4( $, 4 );
			s << "Number of entries( Chunk Offset ):" << nEntries << endl;
			auto pos = (UI8)8;
			for ( auto _ = 0; _ < nEntries; _++ ) {
				s << " Offset:" << Size4( $, pos ) << endl;
				pos += 4;
			}
		}
		break;
	case Q_saiz	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)4;
			if ( vf & 1 ) {
				s << " AuxInfoType:" << EncodeHex( $, 4, 4 ) << endl;				pos += 4;
				s << " AuxInfoTypeParameter:" << EncodeHex( $, 8, 4 ) << endl;		pos += 4;
				
			}
			auto defaultSampleInfoSize = Size1( $, pos );							pos += 1;
			s << " DefaultSampleInfoSize:" << (UI4)defaultSampleInfoSize << endl;
			if ( ! defaultSampleInfoSize ) {
				auto sampleCount = Size4( $, pos );										pos += 4;
				s << " SampleCount:" << sampleCount << endl;
				for ( auto _ = 0; _ < sampleCount; _++ ) s << " Entry:" << _ << ':' << Size1( $, pos + _ ) << endl;
			}
		}
		break;
	case Q_saio	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)4;
			if ( vf & 1 ) {
				s << " AuxInfoType:" << EncodeHex( $, 4, 4 ) << endl;
				s << " AuxInfoTypeParameter:" << EncodeHex( $, 8, 4 ) << endl;
				pos += 8;
			}
			auto nEntries = Size4( $, pos );
			pos += 4;
			s << " nEntries:" << nEntries << endl;
			if ( vf >> 24 ) {
				for ( auto _ = 0; _ < nEntries; _++ ) s << " Entry:" << _ << ':' << Size8( $, pos + _ * 8 ) << endl;
			} else {
				for ( auto _ = 0; _ < nEntries; _++ ) s << " Entry:" << _ << ':' << Size4( $, pos + _ * 4 ) << endl;
			}
		}
		break;
	case Q_trun	:
//	const UI32 TRUN_FLAG_DATA_OFFSET_PRESENT                    = 0x0001;
//	const UI32 TRUN_FLAG_FIRST_SAMPLE_FLAGS_PRESENT             = 0x0004;
//	const UI32 TRUN_FLAG_SAMPLE_DURATION_PRESENT                = 0x0100;
//	const UI32 TRUN_FLAG_SAMPLE_SIZE_PRESENT                    = 0x0200;
//	const UI32 TRUN_FLAG_SAMPLE_FLAGS_PRESENT                   = 0x0400;
//	const UI32 TRUN_FLAG_SAMPLE_COMPOSITION_TIME_OFFSET_PRESENT = 0x0800;
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto nEntries = Size4( $, 4 );
			s << " nEntries:" << nEntries << endl;
			auto pos = (UI8)8;
//s << EncodeHexLF16( $, 0, 64 ) << endl;
			if ( vf &    1 ) {
				s << " DataOffset:(must be moof size + 8)" << Size4( $, pos ) << endl;			pos += 4;
			}
			if ( vf &    2 ) 																	pos += 4;
			if ( vf &    4 ) {
				s << " FirstSampleFlags:" << EncodeHex( $, pos, 4 ) << endl;					pos += 4;
			}
			if ( vf &    8 ) pos += 4;
			if ( vf & 0x10 ) pos += 4;
			if ( vf & 0x20 ) pos += 4;
			if ( vf & 0x40 ) pos += 4;
			if ( vf & 0x80 ) pos += 4;
			for ( auto _ = 0; _ < nEntries; _++ ) {
				s << " Entry:" << _ << endl;
				if ( vf & 0x0100 ) {
					s << "  sample_duration: "	<< Size4( $, pos ) << endl;						pos += 4;
				}
				if ( vf & 0x0200 ) {
					s << "  sample_size: "		<< Size4( $, pos ) << endl;						pos += 4;
				}
				if ( vf & 0x0400 ) {
					s << "  sample_flags: "		<< EncodeHex( $, pos, 4 ) << endl;				pos += 4;
				}
				if ( vf & 0x0800 ) {
					s << "  sample_composition_time_offset: " << Size4( $, pos ) << endl;		pos += 4;
				}
				if ( vf & 0x1000 ) pos += 4;
				if ( vf & 0x2000 ) pos += 4;
				if ( vf & 0x4000 ) pos += 4;
				if ( vf & 0x8000 ) pos += 4;
			}
		}
		break;
	case Q_mvhd	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)4;
			if ( vf >> 24 ) {
				s << "CreationTime:"		<< LocalTimeString_MP4TimeStamp( Size8( $, pos ) );	pos += 8;
				s << "ModificationTime:"	<< LocalTimeString_MP4TimeStamp( Size8( $, pos ) );	pos += 8;
				s << "TimeScale:"			<< Size4( $, pos ) << endl;							pos += 4;
				s << "Duration:"			<< Size8( $, pos ) << endl;							pos += 8;
			} else {
				s << "CreationTime:"		<< LocalTimeString_MP4TimeStamp( Size4( $, pos ) );	pos += 4;
				s << "ModificationTime:"	<< LocalTimeString_MP4TimeStamp( Size4( $, pos ) );	pos += 4;
				s << "TimeScale:"			<< Size4( $, pos ) << endl;							pos += 4;
				s << "Duration:"			<< Size4( $, pos ) << endl;							pos += 4;
			}
			s << "Rate:" << Size4( $, pos ) << endl;		pos += 4;
			s << "Volume:" << Size2( $, pos ) << endl;		pos += 2;
			pos += 2;	//	Reserved1
			pos += 8;	//	Reserved2
			s << "Matrix:";
			for ( auto _ = 0; _ < 9; _++ ) {
				s << ' ' << Size4( $, pos );
				pos += 4;
			}
			s << endl;
			s << "Predefined:" << EncodeHex( $, pos, 24 ) << endl;	pos += 24;
			s << "NextTrackID:" << (int)Size4( $, pos ) << endl;	pos += 4;
		}
		break;

//	case Q_iods	:
//		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
//		{	auto vf = Size4( $, 0 );
//		}
//		break;
	case Q_mdhd	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)4;
			if ( vf >> 24 ) {
				s << "CreationTime:"		<< LocalTimeString_MP4TimeStamp( Size8( $, pos ) );	pos += 8;
				s << "ModificationTime:"	<< LocalTimeString_MP4TimeStamp( Size8( $, pos ) );	pos += 8;
				s << "TimeScale:"			<< Size4( $, pos ) << endl;							pos += 4;
				s << "Duration:"			<< Size8( $, pos ) << endl;							pos += 8;
			} else {
				s << "CreationTime:"		<< LocalTimeString_MP4TimeStamp( Size4( $, pos ) );	pos += 4;
				s << "ModificationTime:"	<< LocalTimeString_MP4TimeStamp( Size4( $, pos ) );	pos += 4;
				s << "TimeScale:"			<< Size4( $, pos ) << endl;							pos += 4;
				s << "Duration:"			<< Size4( $, pos ) << endl;							pos += 4;
			}
			s << "Lang:" << EncodeHex( $, pos, 2 ) << endl;		pos += 4;
		}
		break;
	case Q_mehd	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			if ( vf >> 24 ) {
				s << "Duration:" << Size8( $, 4 ) << endl;
			} else {
				s << "Duration:" << Size4( $, 4 ) << endl;
			}
		}
		break;
	case Q_elst	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto entryCount = Size4( $, 4 );
			s << "EntryCount:" << entryCount << endl;
			if ( vf >> 24 ) {
				s << "MaxEntries:" << ( $->$.size() / 20 ) << endl;
				for ( auto _ = 0; _ < entryCount; _++ ) {
					s << '\t' << _ << '\t' << "Segment Duration:" << Size8( $, 8 + _ * 20 ) << endl;
					s << '\t' << '\t' << "Media Time:" << Size8( $, 8 + _ * 20 + 8 ) << endl;
					s << '\t' << '\t' << "Media Rate:" << Size2( $, 8 + _ * 20 + 16 ) << endl;
				}
			} else {
				s << "MaxEntries:" << ( $->$.size() / 12 ) << endl;
				for ( auto _ = 0; _ < entryCount; _++ ) {
					s << '\t' << _ << '\t' << "Segment Duration:" << Size4( $, 8 + _ * 12 ) << endl;
					s << '\t' << '\t' << "Media Time:" << Size4( $, 8 + _ * 12 + 4 ) << endl;
					s << '\t' << '\t' << "Media Rate:" << Size2( $, 8 + _ * 12 + 8 ) << endl;
				}
			}
		}
		break;
	case Q_hdlr	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		s << "HandlerType:" << TypeString( Type( $, 8 ) ) << endl;
		if ( $->$.size() > 24 ) s << "Name:" << &$->$[ 24 ] << endl;
		break;
	case Q_vmhd	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		s << "GraphicsMode:" << Size2( $, 4 ) << endl;
		s << "R:" << Size2( $,  6 ) << endl;
		s << "G:" << Size2( $,  8 ) << endl;
		s << "B:" << Size2( $, 10 ) << endl;
		break;
	case Q_sidx	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		s << "RefernceID:" << Size4( $, 4 ) << endl;
		s << "TimeScale:" << Size4( $, 8 ) << endl;
		{	auto vf = Size4( $, 0 );
			auto pos = (UI8)12;
			if ( vf >> 24 ) {
				s << "EarliestPresentationTime:" << Size8( $, pos ) << endl;	pos += 8;
				s << "FirstOffset:" << Size8( $, pos ) << endl;					pos += 8;
			} else {
				s << "EarliestPresentationTime:" << Size4( $, pos ) << endl;	pos += 4;
				s << "FirstOffset:" << Size4( $, pos ) << endl;					pos += 4;
			}
			pos += 2;
			auto referenceCount = Size2( $, pos );	pos += 2;
			s << "ReferenceCount:" << referenceCount << endl;
			for ( auto _ = 0; _ < referenceCount; _++ ) {
//				stream.ReadUI32(value);
//				m_References[i].m_ReferenceType = (value&(1<<31))?1:0;
//				m_References[i].m_ReferencedSize = value & 0x7FFFFFFF;
//				stream.ReadUI32(m_References[i].m_SubsegmentDuration);
//				stream.ReadUI32(value);
//				m_References[i].m_StartsWithSap = ((value&(1<<31)) != 0);
//				m_References[i].m_SapType       = (UI08)((value >>28)&0x07);
//				m_References[i].m_SapDeltaTime  = value & 0x0FFFFFFF;
				s << "ReferenceType + ReferencedSize :" << Size4( $, pos ) << endl;	pos += 4;
				s << "SubsegmentDuration:" << Size4( $, pos ) << endl;				pos += 4;
				s << "Sap:" << Size4( $, pos ) << endl;								pos += 4;
			}
		}
		break;

	case Q_tfra	:
		s << "Version + Flags:" << EncodeHex( $, 0, 4 ) << endl;
		{	auto vf = Size4( $, 0 );
			s << "TrackID:" << Size4( $, 4 ) << endl;
			auto fields = Size4( $, 8 );
			s << "LengthSize:" << ( ( fields >> 4 ) & 3 ) << ':' << ( ( fields >> 2 ) & 3 ) << ':' << ( fields & 3 ) << endl;
			auto nEntries = Size4( $, 12 );
			auto pos = (UI8)16;
			for ( auto _ = 0; _ < nEntries; _++ ) {
				if ( vf >> 24 ) {
					s << "Time:"		<< Size8( $, pos ) << endl;	pos += 8;
					s << "MoofOffset:"	<< Size8( $, pos ) << endl;	pos += 8;
				} else {
					s << "Time:"		<< Size4( $, pos ) << endl;	pos += 4;
					s << "MoofOffset:"	<< Size4( $, pos ) << endl;	pos += 4;
				}
				switch ( ( fields >> 4 ) & 3 ) {
				case 0:	 s << "TrafNumber:" << Size1( $, pos ) << endl; pos += 1; break;
				case 1:	 s << "TrafNumber:" << Size2( $, pos ) << endl; pos += 2; break;
				case 2:	 s << "TrafNumber:" << Size3( $, pos ) << endl; pos += 3; break;
				case 3:	 s << "TrafNumber:" << Size4( $, pos ) << endl; pos += 4; break;
				}
				switch ( ( fields >> 2 ) & 3 ) {
				case 0:	 s << "TrunNumber:" << Size1( $, pos ) << endl; pos += 1; break;
				case 1:	 s << "TrunNumber:" << Size2( $, pos ) << endl; pos += 2; break;
				case 2:	 s << "TrunNumber:" << Size3( $, pos ) << endl; pos += 3; break;
				case 3:	 s << "TrunNumber:" << Size4( $, pos ) << endl; pos += 4; break;
				}
				switch ( fields & 3 ) {
				case 0:	 s << "SampleNumber:" << Size1( $, pos ) << endl; pos += 1; break;
				case 1:	 s << "SampleNumber:" << Size2( $, pos ) << endl; pos += 2; break;
				case 2:	 s << "SampleNumber:" << Size3( $, pos ) << endl; pos += 3; break;
				case 3:	 s << "SampleNumber:" << Size4( $, pos ) << endl; pos += 4; break;
				}
			}
		}
		break;

	case Q_mfro	:
		s << "MfraSize:" << Size4( $, 4 ) << endl;
		break;

	case Q_free	:
		s << string( (char*)&$->$[ 0 ], $->$.size() ) << endl;
		break;

	case Q_mdat	:
		switch ( mdatType ) {
		case Q_hev1:
		case Q_hvc1:
			DumpHEVCNALs( NALs_MDAT( $->$ ), s );
			break;
		default:
			s << EncodeHexLF16( $, 0, $->$.size() ) << endl;
			break;
		}
		break;

	default		:
		s << "eh?" << endl;
//		s << EncodeHexLF16( $, 0, $->$.size() ) << endl;
		break;
	}
}

struct
Box {
	UI4				type;
	vector< UI1 >	data;
	vector< Box >	children;

	static vector< Box >
	List( const UI1* _, const UI1* e ) {
		vector< Box > $;
		while ( _ < e ) {
			$.emplace_back( Box( _ ) );
			_ += $.back().Size();
		}
		return $;
	}

	static vector< Box >
	List( const vector< UI1 >& _ ) {
		return List( _.data(), _.data() + _.size() );
	}

	virtual ~
	Box() {
//cerr << '-' << --nGen << ':' << TypeString( type ) << endl;
	}

    static vector< UI8 >    //  Data, Children and End
    Offsets( const UI1* _ ) {
        vector< UI8 > ${ 8, 8, Size4( _ ) };
        if ( $[ 2 ] == 1 ) {
            $[ 0 ] = 16;
            $[ 2 ] = Size8( _ + 8 );
        }
        switch ( Type( _ + 4 ) ) {
        case Q_mp4a :
        case Q_enca :
            $[ 1 ] = $[ 0 ] + 28;
            break;
        case Q_meta :
            $[ 1 ] = $[ 0 ] +  4;
            break;
        case Q_dref :
        case Q_stsd :
            $[ 1 ] = $[ 0 ] +  8;
            break;
        case Q_hvc1 :
        case Q_hev1 :
        case Q_avc1 :
            $[ 1 ] = $[ 0 ] + 78;
            break;
        SIMPLE_NODES
            $[ 1 ] = $[ 0 ];
            break;
        default     :
            $[ 1 ] = $[ 2 ];
            break;
        }
        return $;
    }
        
    static vector< UI1 >
    Data( const UI1* _ ) {
        auto offsets = Offsets( _ );
        return vector< UI1 >( _ + offsets[ 0 ], _ + offsets[ 1 ] );
    }

    static vector< Box >
    Children( const UI1* _ ) {
        auto offsets = Offsets( _ );
        return List( _ + offsets[ 1 ], _ + offsets[ 2 ] );
    }

	Box( const UI1* _ )
	:	type( Type( _ + 4 ) )
	,	data( Data( _ ) )
	,	children( Children( _ ) ) {
//cerr << '+' << ++nGen << ':' << TypeString( type ) << endl;
	}

	Box( const Box& _ )
	:	type( _.type )
	,	data( _.data )
	,	children( _.children ) {
//cerr << '&' << ++nGen << ':' << TypeString( type ) << endl;
	}

	UI8
	Size() const {
		auto $ = RawSize() + 8;
		return $ < 4294967296 ? $ : $ + 8;
	}

	UI8
	RawSize() const {
		return data.size() + accumulate(
			children.begin()
		,	children.end()
		,	0
		,	[]( UI8 $, const Box& child ) { return $ + child.Size(); }
		);
	}

	UI8
	Write( UI1* ptr ) const {
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
		memcpy( _, &data[ 0 ], data.size() );	_ += data.size();
		for ( auto& $: children ) _ += $.Write( _ );
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
		::Write( fd, data );
		for ( auto $: children ) $.Write( fd );
	}

	vector< UI1 >
	Serialize() {
		vector< UI1 >$( Size() );
		Write( $.data() );
		return $;
	}

	vector< Box >
	Children( UI4 type ) const {
		vector< Box >	$;
		for ( auto& _: children ) if ( _.type == type ) $.emplace_back( _ );
		return $;
	}

	Box
	Only( UI4 type ) const {
		auto $ = Children( type );
		A( $.size() == 1 );
		return $[ 0 ];
	}
};

void
DumpSkelton( const Box& _, ostream& s = cout, int level = 0 ) {
	for ( auto _ = 0; _ < level; _++ ) s << '\t';
	s << TypeString( _.type ) << endl;
	level++;
	for ( auto& _: _.children ) DumpSkelton( _, s, level );
}
