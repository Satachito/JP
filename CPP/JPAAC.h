#pragma once
//	2023- Written by Satoru Ogura. (C)SliP LLC, Tokyo.

//	https://wiki.multimedia.cx/index.php/Understanding_AAC
//	https://developer.apple.com/library/archive/documentation/QuickTime/QTFF
//	https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFChap3/qtff3.html

#define	D( K, V ) cerr << K << ':' << V << endl
//#define	D( K, V )
#define	S( _ ) cerr << _

#include "JPMP4.h"

#include "Huffman.h"

inline void
ADTS( const vector< UI1 >& adts, UI8 offset = 0 ) {
	//	ADTS Header
	while ( offset < adts.size() ) {
		cout << offset << endl;
		auto _ = adts.data() + offset;
		A( _[ 0 ] == 0xff && ( _[ 1 ] & 0xf0 ) == 0xf0 );
		cout << ( ( _[ 1 ] & 0x08 ) ? "MPEG-2" : "MPEG-4" ) << endl;
		A( ( _[ 1 ] & 0x06 ) == 0 );
		cout << ( ( _[ 1 ] & 0x01 ) ? "NO CRC" : "CRC Exists" ) << endl;
		cout << "Profile: ";
		switch ( ( _[ 2 ] & 0xc0 ) >> 6 ) {
		case 0:	cout << "Main" << endl; break;
		case 1:	cout << "LC" << endl; break;
		case 2:	cout << "SSR" << endl; break;
		case 3:	cout << "LTP" << endl; break;
		default: cout << endl; break;
		}
		switch ( ( _[ 2 ] & 0x3c ) >> 2 ) {
		case  0: cout << "96000Hz" << endl; break;
		case  1: cout << "88200Hz" << endl; break;
		case  2: cout << "64000Hz" << endl; break;
		case  3: cout << "48000Hz" << endl; break;
		case  4: cout << "44100Hz" << endl; break;
		case  5: cout << "32000Hz" << endl; break;
		case  6: cout << "24000Hz" << endl; break;
		case  7: cout << "22050Hz" << endl; break;
		case  8: cout << "16000Hz" << endl; break;
		case  9: cout << "12000Hz" << endl; break;
		case 10: cout << "11025Hz" << endl; break;
		case 11: cout << "8000Hz" << endl; break;
		case 12: cout << "7350Hz" << endl; break;
		case 13: cout << "eh? Hz" << endl; break;
		case 14: cout << "eh? Hz" << endl; break;
		case 15: cout << "IMPLICIT Hz" << endl; break;
		}
		cout << ( ( _[ 2 ] & 0x02 ) ? "Decoding" : "Encoding" ) << endl;
		switch ( ( _[ 2 ] & 0x01 ) << 2 | ( _[ 3 ] & 0xc0 ) >> 6 ) {
		case 0: cout << "in PCE" << endl; break;
		case 1: cout << "FC" << endl; break;
		case 2: cout << "FL:FR" << endl; break;
		case 3: cout << "FC:FL:FR" << endl; break;
		case 4: cout << "FC:FL:FR:BC" << endl; break;
		case 5: cout << "FC:FL:FR:BL:BR" << endl; break;
		case 6: cout << "FC:FL:FR:BL:BR:LFE" << endl; break;
		case 7: cout << "FC:FL:FR:SL:SR:BL:BR:LFE" << endl; break;
		default: cout << "eh?" << endl; break;
		}
		auto frameLength = ( _[ 3 ] & 0x03 ) << 11 | _[ 4 ] << 3 | _[ 5 ] >> 5;
		cout << "Frame length: " << frameLength << endl;
		cout << "Buffer length: " << ( ( _[ 5 ] & 0x1f ) << 6 | _[ 6 ] >> 2 ) << endl;
		cout << "num aac frame?: " << ( _[ 6 ] & 0x03 ) << endl;
		if ( ( _[ 1 ] & 0x01 ) == 0 ) cout << hex << (int)_[ 7 ] << ' ' << (int)_[ 8 ] << endl;

		cout << EncodeHex( _ + 7, 16 );
		
		offset += frameLength;
		cout << endl;
	}
}


inline UI8
SevenBitsSize( UI1* _, UI8& $ ) {
	$ = _[ 0 ] & 0x7f;
	if ( ( _[ 0 ] & 0x80 ) == 0 ) return 1;
	$ = ( $ << 7 ) | ( _[ 1 ] & 0x7f );
	if ( ( _[ 1 ] & 0x80 ) == 0 ) return 2;
	$ = ( $ << 7 ) | ( _[ 2 ] & 0x7f );
	if ( ( _[ 2 ] & 0x80 ) == 0 ) return 3;
	$ = ( $ << 7 ) | ( _[ 3 ] & 0x7f );
	return 4;
}

struct
AudioInfo {
	UI8	nChannels				= 0;
	UI8	nBits					= 0;
	UI8	sampleRate				= 0;
	UI8	nSamples				= 0;
	UI8	bufferSize				= 0;
	UI8 bitRateMax				= 0;
	UI8 bitRateAvg				= 0;
	UI8 ascSize					= 0;
	UI8 objectTypeOrg			= 0;
	UI8 objectType				= 0;
	UI8 channelsConfiguration	= 0;
	UI8 frequencyIndexOrg		= 0;
	UI8 frequencyIndex			= 0;
	UI8 frequency				= 0;
	UI8	frameLength				= 0;

	vector< pair< UI8, UI8 > >	frameInfos;
	
	AudioInfo( const Box& trak ) {
		auto mdia = trak.Only( Q_mdia );
		auto stbl = mdia.Only( Q_minf ).Only( Q_stbl );
		for ( auto& _: stbl.Only( Q_stsd ).children ) {
			if ( _.type == Q_mp4a ) {
				{//	faad2:mp4read.c:esdsin
					auto esds	= _.Only( Q_esds );
					auto data	= esds.data.data() + 4;
					
					UI8	dummySize;
				//	TAG_ES = 3, TAG_DC = 4, TAG_DSI = 5, TAG_SLC = 6
				//	ES
					A( *data == 3 );					data += 1;
					data += SevenBitsSize( data, dummySize );
D( "Size: ", dummySize );
D( "ESID: ", Size2( data ) );
														data += 2;
D( "flags: ", HexStr( *data ) );
														data += 1;
				//	DC
					A( *data == 4 );					data += 1;
					data += SevenBitsSize( data, dummySize );
D( "Size: ", dummySize );
					A( *data == 0x40 );					data += 1;	//	MPEG-4 audio
D( "flags: ", HexStr( *data ) );
														data += 1;
					bufferSize = ( Size2( data ) << 8 ) | data[ 2 ];
														data += 3;
					bitRateMax = Size4( data );			data += 4;
					bitRateAvg = Size4( data );			data += 4;
					
				//	DSI
					A( *data == 5 );					data += 1;
					data += SevenBitsSize( data, ascSize );
				//	A( ascSize <= 10 );	//	sizeof(mp4config.asc.buf)
					//	https://wiki.multimedia.cx/index.php/Understanding_AAC
					//	mp4.c::AudioSpecificConfigFromBitfile
D( "mp4config.asc.buf: ",  EncodeHex( data, ascSize ) );
					
					auto asc = data;
					data += ascSize;
					objectType = asc[ 0 ] >> 3;											//	5 bits
D( "_objectType: ",  objectType );
					frequencyIndex = ( ( asc[ 0 ] & 0x07 ) << 1 ) | ( asc[ 1 ] >> 7 );	//	4 bits
D( "_frequencyIndex: ",  frequencyIndex );
					channelsConfiguration = ( asc[ 1 ] & 0x7f ) >> 3;					//	4 bits
D( "_extension: ",  ( asc[ 1 ] & 0x01 ) );

					UI4 frequencyTable[] = {
						96000, 88200, 64000, 48000, 44100, 32000,
						24000, 22050, 16000, 12000, 11025, 8000
					};
					objectTypeOrg = objectType;
					frequencyIndexOrg = frequencyIndex;
					if ( objectType == 5 || objectType == 29 ) {	//	Extension
						auto _ = ( ( asc[ 1 ] & 0x07 ) << 1 ) | ( asc[ 2 ] >> 7 );		//	4 bits
						if ( _ == frequencyIndex ) D( "downSampledSBR", "" );
						frequencyIndex = _;
D( "_frequencyIndex(Extension): ",  frequencyIndex );
						if ( frequencyIndex == 15 ) {
							frequency =	( ( asc[ 2 ] & 0x7f ) << 17 ) | ( asc[ 3 ] << 9 ) | ( asc[ 4 ] << 1 ) | ( asc[ 5 ] >> 7 );
							objectType = ( asc[ 5 ] & 0x7f ) >> 2;
							frameLength = ( asc[ 5 ] & 0x02 ) ? 960 : 1024;
						} else {
							frequency =	frequencyTable[ frequencyIndex ];
							objectType = ( asc[ 2 ] & 0x7f ) >> 2;
							frameLength = ( asc[ 2 ] & 0x02 ) ? 960 : 1024;
						}
D( "_objectType(Extension): ",  objectType );
					} else {
						frequency = frequencyTable[ frequencyIndex ];
						frameLength = ( asc[ 1 ] & 0x04 ) ? 960 : 1024;
					}
					A( objectType == 2 );	//	LC only
				//	SLC
					A( *data == 6 );					data += 1;
					data += SevenBitsSize( data, dummySize );
D( "Size: ",  dummySize );
D( "Predefined: ",  HexStr( *data ) );
														data += 1;
				}

				{	auto data	= _.data.data() + 16;
					nChannels	= Size2( data );		data += 2;
					nBits		= Size2( data );		data += 2;
				}
				
				{	auto mdhd	= mdia.Only( Q_mdhd );
					auto data	= mdhd.data.data() + 12;
					sampleRate	= Size4( data );		data += 4;
					nSamples	= Size4( data );		data += 4;
				}
				{	auto stsz	= stbl.Only( Q_stsz );
					auto data	= stsz.data.data() + 8;
					auto nFrames= Size4( data );		data += 4;
					frameInfos.resize( nFrames );
					for ( UI8 _ = 0; _ < nFrames; _++ ) {
						frameInfos[ _ ].second = Size4( data );	data += 4;
					}
				}
				{	vector< pair< UI8, UI8 > >	sliceInfo;
					{	auto stsc	= stbl.Only( Q_stsc );	//	Slice i.e. Sample to chunk
						auto data	= stsc.data.data() + 4;
						auto nSlices= Size4( data );		data += 4;
						sliceInfo.resize( nSlices );
						for ( UI8 _ = 0; _ < nSlices; _++ ) {
							sliceInfo[ _ ].first = Size4( data );	data += 4;	//	First chunk
							sliceInfo[ _ ].second = Size4( data );	data += 4;	//	Samples per chunk
																	data += 4;	//	Samples description ID
						}
					}
					{	auto stco	= stbl.Only( Q_stco );
						auto data	= stco.data.data() + 4;
						auto nChunks	= Size4( data );	data += 4;	//	101: # of offset data

						for ( UI8 _ = 1; _ < sliceInfo.size(); _++ ) {
							sliceInfo[ _ - 1 ].first = sliceInfo[ _ ].first - sliceInfo[ _ - 1 ].first;
						}
						sliceInfo.back().first = nChunks - sliceInfo.back().first + 1;	//	Index starts with 1, not 0
						
						UI8 iFrame = 0;
						for ( auto& slice: sliceInfo ) {
							for ( UI8 _first = 0; _first < slice.first; _first++ ) {
								auto offset = Size4( data );	data += 4;
								for ( UI8 _second = 0; _second < slice.second; _second++ ) {
									frameInfos[ iFrame ].first = offset;
									offset += frameInfos[ iFrame ].second;
									iFrame++;
								}
							}
						}
						A( iFrame == frameInfos.size() );
					}
				}
			}
		}
	}
	void
	Dump( ostream& s = cerr ) {
		s << "	nChannels				: "	<< nChannels				<< endl;
		s << "	bits					: "	<< nBits					<< endl;
		s << "	sampleRate				: "	<< sampleRate				<< endl;
		s << "	nSamples				: "	<< nSamples					<< endl;
		s << "	# of Frames				: "	<< frameInfos.size()		<< endl;
		s << "	bufferSize				: "	<< bufferSize				<< endl;
		s << "	bitRateMax				: "	<< bitRateMax				<< endl;
		s << "	bitRateAvg				: "	<< bitRateAvg				<< endl;
		s << "	asc.size				: "	<< ascSize					<< endl;
		s << "	objectTypeOrg			: "	<< objectTypeOrg			<< endl;
		s << "	objectType				: "	<< objectType				<< endl;
		s << "	channelsConfiguration	: "	<< channelsConfiguration	<< endl;
		s << "	frequencyIndexOrg		: "	<< frequencyIndexOrg		<< endl;
		s << "	frequencyIndex			: "	<< frequencyIndex			<< endl;
		s << "	frequency				: "	<< frequency				<< endl;
		s << "	frameLength				: "	<< frameLength				<< endl;
	}
	vector< UI1 >
	ADTSHeader( UI8 frameLength ) {
		vector< UI1 >	$( frequencyIndex == 15 ? 10 : 7 );
		$[ 0 ]		|= 0xff;
		$[ 1 ]		|= 0xf1;
		$[ 2 ]		|= ( objectType - 1 ) << 6;				//	01		01
		$[ 2 ]		|= frequencyIndexOrg << 2;				//	0100	010100
															//	0		0101000	Private
		UI8 _ = 2;
		if ( frequencyIndex == 15 ) {	//	24bit
			$[ 2 ]	|= frequency >> 23;
			$[ 3 ]	|= ( frequency & 0x7fffff ) >> 15;
			$[ 4 ]	|= ( frequency & 0x007fff ) >> 7;
			$[ 5 ]	|= ( frequency & 0x00007f ) << 1;
			_ = 5;
		}
		$[ _ ]		|= channelsConfiguration >> 2;			//	010		01010000
		$[ _ + 1 ]	|= ( channelsConfiguration & 3 ) << 6;	//			01010000 10
		//	original
		//	home
		//	copyright_id
		//	copyright_id_start								//			01010000 100000
		$[ _ + 1 ]	|= frameLength >> 11;		//	2/13 bits
		$[ _ + 2 ]	|= ( frameLength & 0x07f8 ) >> 3;
		$[ _ + 3 ]	|= ( frameLength & 0x07 ) << 5;
		$[ _ + 3 ]	|= 0x1f;					//	5/11 bits
		$[ _ + 4 ]	|= 0xfc;					//
		return $;
	}
};


static UI8 swb_offset_1024_96[] = {
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56,
	64, 72, 80, 88, 96, 108, 120, 132, 144, 156, 172, 188, 212, 240,
	276, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1024
};
static UI8 swb_offset_1024_64[] = {
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56,
	64, 72, 80, 88, 100, 112, 124, 140, 156, 172, 192, 216, 240, 268,
	304, 344, 384, 424, 464, 504, 544, 584, 624, 664, 704, 744, 784, 824,
	864, 904, 944, 984, 1024
};
static UI8 swb_offset_1024_48[] = {
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 56, 64, 72,
	80, 88, 96, 108, 120, 132, 144, 160, 176, 196, 216, 240, 264, 292,
	320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672, 704, 736,
	768, 800, 832, 864, 896, 928, 1024
};
static UI8 swb_offset_1024_32[] =
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 56, 64, 72,
	80, 88, 96, 108, 120, 132, 144, 160, 176, 196, 216, 240, 264, 292,
	320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672, 704, 736,
	768, 800, 832, 864, 896, 928, 960, 992, 1024
};
static UI8 swb_offset_1024_24[] =
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 52, 60, 68,
	76, 84, 92, 100, 108, 116, 124, 136, 148, 160, 172, 188, 204, 220,
	240, 260, 284, 308, 336, 364, 396, 432, 468, 508, 552, 600, 652, 704,
	768, 832, 896, 960, 1024
};
static UI8 swb_offset_1024_16[] =
{
	0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 100, 112, 124,
	136, 148, 160, 172, 184, 196, 212, 228, 244, 260, 280, 300, 320, 344,
	368, 396, 424, 456, 492, 532, 572, 616, 664, 716, 772, 832, 896, 960, 1024
};
static UI8 swb_offset_1024_8[] =
{
	0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 172,
	188, 204, 220, 236, 252, 268, 288, 308, 328, 348, 372, 396, 420, 448,
	476, 508, 544, 580, 620, 664, 712, 764, 820, 880, 944, 1024
};
static UI8*swb_offset_1024_window[] = {
	swb_offset_1024_96,      /* 96000 */
	swb_offset_1024_96,      /* 88200 */
	swb_offset_1024_64,      /* 64000 */
	swb_offset_1024_48,      /* 48000 */
	swb_offset_1024_48,      /* 44100 */
	swb_offset_1024_32,      /* 32000 */
	swb_offset_1024_24,      /* 24000 */
	swb_offset_1024_24,      /* 22050 */
	swb_offset_1024_16,      /* 16000 */
	swb_offset_1024_16,      /* 12000 */
	swb_offset_1024_16,      /* 11025 */
	swb_offset_1024_8        /* 8000  */
};

static UI8 swb_offset_128_96[] = { 0, 4, 8, 12, 16, 20, 24, 32, 40, 48, 64, 92, 128 };
static UI8 swb_offset_128_64[] = { 0, 4, 8, 12, 16, 20, 24, 32, 40, 48, 64, 92, 128 };
static UI8 swb_offset_128_48[] = { 0, 4, 8, 12, 16, 20, 28, 36, 44, 56, 68, 80, 96, 112, 128 };
static UI8 swb_offset_128_24[] = { 0, 4, 8, 12, 16, 20, 24, 28, 36, 44, 52, 64, 76, 92, 108, 128 };
static UI8 swb_offset_128_16[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 40, 48, 60, 72, 88, 108, 128 };
static UI8 swb_offset_128_8[] = { 0, 4, 8, 12, 16, 20, 24, 28, 36, 44, 52, 60, 72, 88, 108, 128 };

static UI8* swb_offset_128_window[] = {
    swb_offset_128_96,       /* 96000 */
    swb_offset_128_96,       /* 88200 */
    swb_offset_128_64,       /* 64000 */
    swb_offset_128_48,       /* 48000 */
    swb_offset_128_48,       /* 44100 */
    swb_offset_128_48,       /* 32000 */
    swb_offset_128_24,       /* 24000 */
    swb_offset_128_24,       /* 22050 */
    swb_offset_128_16,       /* 16000 */
    swb_offset_128_16,       /* 12000 */
    swb_offset_128_16,       /* 11025 */
    swb_offset_128_8         /* 8000  */
};

struct
MP4 {
	vector< UI1 >	raw;
	AudioInfo*		audioInfo;
	
	~MP4() {
		if ( audioInfo ) delete audioInfo;
	}
	MP4( const vector< UI1 >& raw )
	:	raw( raw )
	,	audioInfo( 0 ) {
		auto mp4 = Box::List( raw );
		for ( auto& _: mp4 ) {
			switch ( _.type ) {
			case Q_moov:
				for ( auto& trak: _.children ) {
					if ( trak.type != Q_trak ) continue;
					for ( auto& _: trak.Only( Q_mdia ).Only( Q_minf ).Only( Q_stbl ).Only( Q_stsd ).children ) {
						switch ( _.type ) {
						case Q_mp4a:
							audioInfo = new AudioInfo( trak );
							break;
						}
					}
				}
				break;
			}
		}
	}
	struct	//	individual channel stream
	ICS {
		UI8 globalGain;
		UI8	windowSequence;
		UI8	windowShape;
		UI8	maxSFB;
		UI8 scaleFactorGrouping;
		UI8 nWindows;
		UI8 nWindowGroups;
		UI8 windowGroupLength[ 8 ];
		UI8 nSWB;

		UI8 sectSFBOffset[ 8 ][ 15 * 8 ];
		UI8 swbOffset[ 52 ];
		UI8 swbOffsetMax;

		UI8 msUsed[ 8 ][ 51 ];	//	MAX_WINDOW_GROUPS MAX_SFB

		UI8 sectCB[ 8 ][ 15 * 8 ];
		UI8 noiseUsed = 0;
		UI8 isUsed = 0;
		UI8 sectStart[ 8 ][ 15 * 8 ];
		UI8 sectEnd[ 8 ][ 15 * 8 ];
		UI8 sfbCB[ 8 ][ 8 * 15 ];
		UI8 numSec[ 8 ]; /* number of sections in a group */
		
 		UI8 scaleFactors[ 8 ][ 51 ]; /* [0..255], except for noise and intensity */

		short
		spectralData[ 1024 ];
		
		ICS() {
		}

		//	#define ONLY_LONG_SEQUENCE   0x0
		//	#define LONG_START_SEQUENCE  0x1
		//	#define EIGHT_SHORT_SEQUENCE 0x2
		//	#define LONG_STOP_SEQUENCE   0x3

		ICS( BitReader& br, const AudioInfo* audioInfo ) {
			A( !br.Read() );
			windowSequence = br.Read( 2 );
			windowShape = br.Read( 1 );	//	0: Sine / 1:Kaiser Bessel
			nWindowGroups = 1;
			windowGroupLength[ 0 ] = 1;
			switch ( windowSequence ) {
			case 2:	//	EIGHT_SHORT_SEQUENCE
				maxSFB = br.Read( 4 );
				scaleFactorGrouping = br.Read( 7 );
				nWindows = 8;
				static UI8 num_swb_128_window[] = {
					12, 12, 12, 14, 14, 14, 15, 15, 15, 15, 15, 15
				};
				nSWB = num_swb_128_window[ audioInfo->frequencyIndexOrg ];
				for ( UI8 _ = 0; _ < nSWB; _++ ) {
					swbOffset[ _ ] = swb_offset_128_window[ audioInfo->frequencyIndexOrg ][ _ ];
				}
				swbOffset[ nSWB ] = swbOffsetMax = audioInfo->frameLength / 8;
				for ( UI8 _ = 0; _ < nWindows - 1; _++ ) {
					if ( scaleFactorGrouping & ( 1 << ( 6 - _ ) ) ) {
						windowGroupLength[ nWindowGroups - 1 ]++;
					} else {
						nWindowGroups++;
						windowGroupLength[ nWindowGroups - 1 ] = 1;
					}
				}
				for ( UI8 _ = 0; _ < nWindowGroups; _++ ) {
					uint16_t width;
					uint8_t sect_sfb = 0;
					uint16_t offset = 0;

					for ( auto i = 0; i < nSWB; i++ ) {
						if (i+1 == nSWB) {
							width = (audioInfo->frameLength/8) - swb_offset_128_window[ audioInfo->frequencyIndexOrg ][i];
						} else {
							width = swb_offset_128_window[ audioInfo->frequencyIndexOrg ][i+1] -
								swb_offset_128_window[ audioInfo->frequencyIndexOrg ][i];
						}
						width *= windowGroupLength[ _ ];
						sectSFBOffset[ _ ][sect_sfb++] = offset;
						offset += width;
					}
					sectSFBOffset[ _ ][sect_sfb] = offset;
				}
				break;
			default:
				maxSFB = br.Read( 6 );
				nWindows = 1;
				if ( br.Read( 1 ) ) {	//	predictor_data_present
					//	TODO:
				}
//				static UI8 num_swb_512_window[] = { 0, 0, 0, 36, 36, 37, 31, 31, 0, 0, 0, 0 };	//	LD
//				static UI8 num_swb_480_window[] = { 0, 0, 0, 35, 35, 37, 30, 30, 0, 0, 0, 0 };	//	LD
				static UI8 num_swb_960_window[] = { 40, 40, 45, 49, 49, 49, 46, 46, 42, 42, 42, 40 };
				static UI8 num_swb_1024_window[] = { 41, 41, 47, 49, 49, 51, 47, 47, 43, 43, 43, 40 };
				const UI8* window = 0;
				switch ( audioInfo->frameLength ) {
				case 1024	:	window = num_swb_1024_window;	break;
				case 960	:	window = num_swb_960_window;	break;
//				case 512	:	window = num_swb_512_window;	break;	//	LD
//				case 480	:	window = num_swb_480_window;	break;	//	LD
				}
				nSWB = window[ audioInfo->frequencyIndexOrg ];
				for ( UI8 _ = 0; _ < nSWB; _++ ) {
					sectSFBOffset[ 0 ][ _ ] = swb_offset_1024_window[ audioInfo->frequencyIndexOrg ][ _ ];
					swbOffset[ _ ] = swb_offset_1024_window[ audioInfo->frequencyIndexOrg ][ _ ];
				}
				sectSFBOffset[ 0 ][ nSWB ] = audioInfo->frameLength;
				swbOffset[ nSWB ] = audioInfo->frameLength;
				swbOffsetMax = audioInfo->frameLength;
			}
		}

		void
		Supp( BitReader& br, const AudioInfo* audioInfo ) {
//	SECTION DATA
			{	UI8	sect_bits;
				UI8	sect_lim;	 // 51 or 120, anyways less than 127.
				switch ( windowSequence ) {
				case 2:	//	EIGHT_SHORT_SEQUENCE
					sect_bits = 3;
					sect_lim = 8 * 15;
					break;
				default:
					sect_bits = 5;
					sect_lim = maxSFB;
				}
				UI8 sect_esc_val = ( 1 << sect_bits ) - 1;

				for ( UI8 _ = 0; _ < nWindowGroups; _++ ) {
					uint8_t k = 0;
					uint8_t i = 0;
					while ( k < maxSFB ) {
						sectCB[ _ ][ i ] = br.Read( 4 );	//	FAAD2: sect_cb_bits

						switch ( sectCB[ _ ][ i ] ) {
						case 13:	//	NOISE_HCB
							noiseUsed = 1;
							break;
						case 14:	//	INTENSITY_HCB2
						case 15:	//	INTENSITY_HCB
							isUsed = 1;
							break;
						}

						UI8 sect_len_incr = br.Read( sect_bits );
						UI8 sect_len = 0;
						while ( sect_len_incr == sect_esc_val ) {
							sect_len += sect_len_incr;
							sect_len_incr = br.Read( sect_bits );
						}
						sect_len += sect_len_incr;

						sectStart[ _ ][ i ] = k;
						sectEnd[ _ ][ i ] = k + sect_len;

						for ( UI8 sfb = k; sfb < k + sect_len; sfb++ ) sfbCB[ _ ][ sfb ] = sectCB[ _ ][ i ];

						k += sect_len; // k <= sect_lim
						i++;
					}
					numSec[ _ ] = i;
				}
			}
		    
//	SCALE FACTOR DATA
			{	int16_t scale_factor = globalGain;
				int16_t is_position = 0;
				int16_t scale_factor_max = 255;
				int8_t noise_pcm_flag = 1;
				int16_t noise_energy = globalGain - 90;

				for ( UI8 g = 0; g < nWindowGroups; g++ ) {
					for ( UI8 sfb = 0; sfb < maxSFB; sfb++ ) {
//						#define ZERO_HCB       0
//						#define FIRST_PAIR_HCB 5
//						#define ESC_HCB        11
//						#define QUAD_LEN       4
//						#define PAIR_LEN       2
//						#define NOISE_HCB      13
//						#define INTENSITY_HCB2 14
//						#define INTENSITY_HCB  15
						switch ( sfbCB[ g ][ sfb ] ) {
						case  0:	//	ZERO_HCB: /* zero book */
							scaleFactors[g][sfb] = 0;
							break;
						case 14:	//	INTENSITY_HCB: /* intensity books */
						case 15:	//	INTENSITY_HCB2:
							/* decode intensity position */
							{	UI8 t = huffman_scale_factor( br );
								is_position += (t - 60);
								scaleFactors[g][sfb] = is_position;
							}
							break;
						case 13:	//	NOISE_HCB: /* noise books */
							/* decode noise energy */
							{	UI8	t;
								if (noise_pcm_flag) {
									noise_pcm_flag = 0;
									t = br.Read( 9 );
								} else {
									t = huffman_scale_factor( br );
									t -= 60;
								}
								noise_energy += t;
								scaleFactors[g][sfb] = noise_energy;
							}
							break;
						default: // spectral books
							{	scaleFactors[ g ][ sfb ] = 0;	// ics->scale_factors[g][sfb] must be between 0 and 255

								// decode scale factor
								UI8	t = huffman_scale_factor( br );
								scale_factor += (t - 60);
								scaleFactors[g][sfb] = min(scale_factor, scale_factor_max);
							}
							break;
						}
					}
				}
			}

			if ( br.Read() ) {	//	pulse_data_present
				//	TODO
			}
			if ( br.Read() ) {	//	tns_data_present
				typedef struct
				{
					uint8_t n_filt[8];
					uint8_t coef_res[8];
					uint8_t length[8][4];
					uint8_t order[8][4];
					uint8_t direction[8][4];
					uint8_t coef_compress[8][4];
					uint8_t coef[8][4][32];
				} tns_info;
				tns_info _tns;
				auto tns = &_tns;
							
			
				uint8_t w, filt, i, coef_bits;
				uint8_t n_filt_bits = 2;
				uint8_t length_bits = 6;
				uint8_t order_bits = 5;

				if (windowSequence == 2 )	//	EIGHT_SHORT_SEQUENCE
				{
					n_filt_bits = 1;
					length_bits = 4;
					order_bits = 3;
				}

				for (w = 0; w < nWindows; w++)
				{
					uint8_t start_coef_bits = 3;
					tns->n_filt[w] = br.Read( n_filt_bits );

					if (tns->n_filt[w])
					{
						if ((tns->coef_res[w] = br.Read() ) )
							start_coef_bits = 4;
					}

					for (filt = 0; filt < tns->n_filt[w]; filt++)
					{
						tns->length[w][filt] = br.Read( length_bits );
						tns->order[w][filt]  = br.Read( order_bits );
						if (tns->order[w][filt])
						{
							tns->direction[w][filt] = br.Read();
							tns->coef_compress[w][filt] = br.Read();

							coef_bits = start_coef_bits - tns->coef_compress[w][filt];
							for (i = 0; i < tns->order[w][filt]; i++)
							{
								tns->coef[w][filt][i] = br.Read( coef_bits );
							}
						}
					}
				}
			}
			if ( br.Read() ) {	//	gain_control_data_present
				//	TODO
			}

		//	SPECTRAL DATA
			{	int8_t i;
				uint8_t g;
				uint16_t inc, k, p = 0;
				uint8_t groups = 0;
				uint8_t sect_cb;
				uint8_t result;
				uint16_t nshort = audioInfo->frameLength/8;

				for(g = 0; g < nWindowGroups; g++)
				{
					p = groups*nshort;

					for (i = 0; i < numSec[g]; i++)
					{
						sect_cb = sectCB[g][i];

						inc = (sect_cb >= 5) ? 2 : 4;	//	5: FIRST_PAIR_HCB

						switch (sect_cb)
						{
						case  0:	//	ZERO_HCB:
						case 13:	//	NOISE_HCB:
						case 15:	//	INTENSITY_HCB:
						case 14:	//	INTENSITY_HCB2:
							p += (sectSFBOffset[g][sectEnd[g][i]] -
								sectSFBOffset[g][sectStart[g][i]]);
							break;
						default:
							for (k = sectSFBOffset[g][sectStart[g][i]];
								 k < sectSFBOffset[g][sectEnd[g][i]]; k += inc)
							{
								huffman_spectral_data(sect_cb, br, &spectralData[p]);
								p += inc;
							}
							break;
						}
					}
					groups += windowGroupLength[g];
				}
			}
		}
	};
	void
	SkipSingleChannel( BitReader& br ) {
		br.Read( 4 );	//	element ID
		auto globalGain = br.Read( 8 );
		ICS	ics( br, audioInfo );
		ics.globalGain = globalGain;
		ics.Supp( br, audioInfo );
	}
	void
	SkipChannelPair( BitReader& br ) {
		br.Read( 4 );	//	element ID
		ICS ics1;
		ICS ics2;
		auto maxIDX = ics1.nWindowGroups * ics1.maxSFB;
		auto commonWindow = br.Read( 1 );
		if ( commonWindow ) {
			ics1 = ICS( br, audioInfo );
			if ( 1 == br.Read( 2 ) ) {
				for ( UI8 g = 0; g < ics1.nWindowGroups; g++)
				{
					for (UI8  sfb = 0; sfb < ics1.maxSFB; sfb++)
					{
						ics1.msUsed[g][sfb] = br.Read();
					}
				}
			}
			ics2 = ics1;
		} else {
		//	ここほんとにうまくいくの？か確認
		//	ics1->ms_mask_present;
			ics2 = ICS( br, audioInfo );
		}
		ics1.globalGain = br.Read( 8 );
		ics1.Supp( br, audioInfo );
		ics2.globalGain = br.Read( 8 );
		ics2.Supp( br, audioInfo );
	}
	void
	SkipFIL( BitReader& br ) {
		auto count = br.Read( 4 );
		if ( count == 15 ) count += br.Read( 8 ) - 1;
		auto type = br.Read( 4 );
		switch ( type ) {
		case  1:	//	EXT_FILL_DATA:
		case  2:	//	EXT_DATA_ELEMENT:
			throw "eh?";	//	TODO:
		case 11:	//	EXT_DYNAMIC_RANGE:
			{	uint8_t i, n = 1;
				uint8_t band_incr;

				UI8	nBands = 1;

				if ( br.Read() ) {	//	has instance_tag
					br.Read( 8 );
					n++;
				}

				if ( br.Read() ) {	//	excluded_chns_present
					uint8_t i, n = 0;
					uint8_t num_excl_chan = 7;

					for (i = 0; i < 7; i++) br.Read();
					n++;

					while ( br.Read() ) {
						if (i >= 64 - num_excl_chan - 7)	//	64: MAX_CHANNELS
							throw "eh?";
						for (i = num_excl_chan; i < num_excl_chan+7; i++) br.Read();
						n++;
						num_excl_chan += 7;
					}
				}

				if ( br.Read() ) {	//	has bands data
					band_incr = br.Read( 4 );
					br.Skip( 4 );
					n++;
					nBands += band_incr;

					for (i = 0; i < nBands; i++) {
						br.Read( 8 );
						n++;
					}
				}

				if ( br.Read() ) {	//	has prog_ref_level
					br.Skip( 8 );
					n++;
				}

				for (i = 0; i < nBands; i++) {
					br.Skip( 8 );
					n++;
				}
			}
			break;
		default:	//	0:EXT_FIL
			br.Skip( 8 * count - 4 );	//	Align
			break;
		}
	}
	void
	SkipDSE( BitReader& br ) {
	
		br.Skip( 4 );	//	element_instance_tag
		auto byteAligned = br.Read();
		auto nBytes = br.Read( 8 );
		if ( nBytes == 255 ) nBytes += br.Read( 8 );
		if ( byteAligned ) {
			UI8 remainder = br._ % 8;
			if ( remainder ) br.Skip( 8 - remainder );
		}
		br.Skip( 8 * nBytes );
	}
	void
	Skip( const vector< UI1 >& _ ) {
/*
0 SCE single channel element (codes a single audio channel)
1 CPE channel pair element (codes stereo signal)
2 CCE something to do with channel coupling, not implemented in libfaad2
3 LFE low-frequency effects? referenced as "special effects" in RTP doc
4 DSE data stream element (user data)
5 PCE program configuration element (describe bitstream)
6 FIL fill element (pad space/extension data)
7 END marks the end of the frame

This is an example layout for a 5.1 audio stream:
SCE CPE CPE LFE END

*/
		BitReader	br( _.data() );
		while ( br._ < _.size() * 8 ) {
			auto type = br.Read( 3 );
			switch ( type ) {
			case 0:	//	ID_SCE
S( "----	ID_SCE" );
				SkipSingleChannel( br );
				break;
			case 1:	//	channel_pair_element
S( "----	ID_CPE" );
				SkipChannelPair( br );
				break;
			case 2:	//	ID_CCE
S( "----	ID_CCE" );
				break;
			case 3:	//	ID_LFE
S( "----	ID_LFE" );
				SkipSingleChannel( br );
				break;
			case 4:	//	ID_DSE
S( "----	ID_DSE" );
				SkipDSE( br );
				break;
			case 5:	//	ID_PCE
S( "----	ID_PCE" );
				break;
			case 6:	//	ID_FIL
S( "----	ID_FIL" );
				SkipFIL( br );
				break;
			case 7:	//	ID_END
S( "----	ID_END" );
				{	UI8 remainder = br._ % 8;
					if ( remainder ) br.Skip( 8 - remainder );
if ( _.size() * 8 != br._ ) D( _.size() * 8, br._ );
				//	A( _.size() * 8 == br._ );
				}
				break;
			default:
				throw "eh?";
			}
//D( "", br._ % 32 );
cerr << endl;
		}
	}
	void
	Skip() {
int frameCount = 0;
		for ( auto& frameInfo: audioInfo->frameInfos ) {
D( "----------------------------------- frame #", frameCount++ );
			Skip( vector< UI1 >( &raw[ frameInfo.first ], &raw[ frameInfo.first + frameInfo.second ] ) );
		}
	}
	
	vector< UI1 >
	CutAndAppendEnd( const BitReader& br, UI8 startBit ) {
		vector< UI1 >	$;
		auto P = br.$ + startBit / 8;
		auto O = startBit % 8;
		auto nBits = br._ - startBit;
		auto nBytes = ( nBits + 7 ) / 8;
		while ( nBytes-- ) {
			$.emplace_back( ( P[ 0 ] << O ) | ( P[ 1 ] >> ( 8 - O ) ) );
			P++;
		}
		switch ( nBits % 8 ) {
		case 0: 					$.emplace_back( 0xE0 );	break;
		case 1: $.back() |= 0x70;							break;
		case 2: $.back() |= 0x38;							break;
		case 3: $.back() |= 0x1c;							break;
		case 4: $.back() |= 0x0e;							break;
		case 5: $.back() |= 0x07;							break;
		case 6: $.back() |= 0x03;	$.emplace_back( 0x80 );	break;
		case 7: $.back() |= 0x01;	$.emplace_back( 0xc0 );	break;
		}
		return $;
	}

	vector< UI1 >
	SCEFrame( const vector< UI1 >& _ ) {
		BitReader	br( _.data() );
		while ( br._ < _.size() * 8 ) {
			auto type = br.Read( 3 );
			switch ( type ) {
			case 0:	//	ID_SCE
				{	auto startBit = br._ - 3;
					SkipSingleChannel( br );
					return CutAndAppendEnd( br, startBit );
				}
				//	returned
			case 1:	//	channel_pair_element
				SkipChannelPair( br );
				break;
			case 3:	//	ID_LFE
				SkipSingleChannel( br );
				break;
			case 6:	//	ID_FIL
				SkipFIL( br );
				break;
			default:
				throw "eh?";
			}
		}
		throw "eh?";
	}
	vector< UI1 >
	LFEFrame( const vector< UI1 >& _ ) {
		BitReader	br( _.data() );
		while ( br._ < _.size() * 8 ) {
			auto type = br.Read( 3 );
			switch ( type ) {
			case 0:	//	ID_SCE
				SkipSingleChannel( br );
				break;
			case 1:	//	channel_pair_element
				SkipChannelPair( br );
				break;
			case 3:	//	ID_LFE
				{	auto startBit = br._ - 3;
					SkipSingleChannel( br );
					auto $ = CutAndAppendEnd( br, startBit );
					$[ 0 ] &= 0x1f;	//	Make it zero( ID_SCE );
					return $;
				}
				//	returned
			case 6:	//	ID_FIL
				SkipFIL( br );
				break;
			default:
				throw "eh?";
			}
		}
		throw "eh?";
	}
	vector< UI1 >
	CPEFrame( const vector< UI1 >& _, UI8 index ) {
		BitReader	br( _.data() );
		while ( br._ < _.size() * 8 ) {
			auto type = br.Read( 3 );
			switch ( type ) {
			case 0:	//	ID_SCE
				SkipSingleChannel( br );
				break;
			case 1:	//	channel_pair_element
				if ( !(index--) ) {
					auto startBit = br._ - 3;
					SkipChannelPair( br );
					return CutAndAppendEnd( br, startBit );
				} else {
					SkipChannelPair( br );
				}
				break;
			case 3:	//	ID_LFE
				SkipSingleChannel( br );
				break;
			case 6:	//	ID_FIL
				SkipFIL( br );
				break;
			default:
				throw "eh?";
			}
		}
		throw "eh?";
	}
	vector< UI1 >
	SCE_ADTS() {
		vector< UI1 >	$;
		for ( auto& frameInfo: audioInfo->frameInfos ) {
			auto frame = SCEFrame( vector< UI1 >( &raw[ frameInfo.first ], &raw[ frameInfo.first + frameInfo.second ] ) );
			auto savedChannelsConfiguration = audioInfo->channelsConfiguration;
			audioInfo->channelsConfiguration = 1;
			auto _ = audioInfo->ADTSHeader( frame.size() + 7 );
			audioInfo->channelsConfiguration = savedChannelsConfiguration;
			$.insert( $.end(), _.begin(), _.end() );
			$.insert( $.end(), frame.begin(), frame.end() );
		}
		return $;
	}
	vector< UI1 >
	LFE_ADTS() {
		vector< UI1 >	$;
		for ( auto& frameInfo: audioInfo->frameInfos ) {
			auto frame = LFEFrame( vector< UI1 >( &raw[ frameInfo.first ], &raw[ frameInfo.first + frameInfo.second ] ) );
			auto savedChannelsConfiguration = audioInfo->channelsConfiguration;
			audioInfo->channelsConfiguration = 1;
			auto _ = audioInfo->ADTSHeader( frame.size() + 7 );
			audioInfo->channelsConfiguration = savedChannelsConfiguration;
			$.insert( $.end(), _.begin(), _.end() );
			$.insert( $.end(), frame.begin(), frame.end() );
		}
		return $;
	}
	vector< UI1 >
	CPE_ADTS( UI8 index ) {
		vector< UI1 >	$;
		for ( auto& frameInfo: audioInfo->frameInfos ) {
			auto frame = CPEFrame( vector< UI1 >( &raw[ frameInfo.first ], &raw[ frameInfo.first + frameInfo.second ] ), index );
			auto savedChannelsConfiguration = audioInfo->channelsConfiguration;
			audioInfo->channelsConfiguration = 2;
			auto _ = audioInfo->ADTSHeader( frame.size() + 7 );
			audioInfo->channelsConfiguration = savedChannelsConfiguration;
			$.insert( $.end(), _.begin(), _.end() );
			$.insert( $.end(), frame.begin(), frame.end() );
		}
		return $;
	}
	
	vector< UI1 >
	ExtractAAC() {
		vector< UI1 >	$;
		for ( auto& frameInfo: audioInfo->frameInfos ) {
			auto _ = audioInfo->ADTSHeader( frameInfo.second + 7 );
			$.insert( $.end(), _.begin(), _.end() );
			$.insert( $.end(), &raw[ frameInfo.first ], &raw[ frameInfo.first + frameInfo.second ] );
		}
		return $;
	}
	
	UI8
	SingleChannelGGO( BitReader& br ) {
		br.Read( 4 );	//	element ID
		auto $ = br._;
		auto globalGain = br.Read( 8 );
		ICS	ics( br, audioInfo );
		ics.globalGain = globalGain;
		ics.Supp( br, audioInfo );
		return $;
	}
	pair< UI8, UI8 >
	ChannelPairGGO( BitReader& br ) {
		br.Read( 4 );	//	element ID
		ICS ics1;
		ICS ics2;
		auto maxIDX = ics1.nWindowGroups * ics1.maxSFB;
		auto commonWindow = br.Read( 1 );
		if ( commonWindow ) {
			ics1 = ICS( br, audioInfo );
			if ( 1 == br.Read( 2 ) ) {
				for ( UI8 g = 0; g < ics1.nWindowGroups; g++)
				{
					for (UI8  sfb = 0; sfb < ics1.maxSFB; sfb++)
					{
						ics1.msUsed[g][sfb] = br.Read();
					}
				}
			}
			ics2 = ics1;
		} else {
		//	ここほんとにうまくいくの？か確認
		//	ics1->ms_mask_present;
			ics2 = ICS( br, audioInfo );
		}
		pair< UI8, UI8 > $;
		$.first = br._;
		ics1.globalGain = br.Read( 8 );
		ics1.Supp( br, audioInfo );
		$.second = br._;
		ics2.globalGain = br.Read( 8 );
		ics2.Supp( br, audioInfo );
		return $;
	}
	vector< UI8 >
	GlobalGainOffsets( const pair< UI8, UI8 >& frameInfo ) {
 		vector< UI8 > $;
		BitReader	br( raw.data() + frameInfo.first );
		while ( br._ < frameInfo.second * 8 ) {
			auto type = br.Read( 3 );
			switch ( type ) {
			case 0:	//	ID_SCE
				$.emplace_back( SingleChannelGGO( br ) );
				break;
			case 1:	//	channel_pair_element
				{	auto _ = ChannelPairGGO( br );
					$.emplace_back( _.first );
					$.emplace_back( _.second );
				}
				break;
			case 2:	//	ID_CCE
				break;
			case 3:	//	ID_LFE
				$.emplace_back( SingleChannelGGO( br ) );
				break;
			case 4:	//	ID_DSE
				SkipDSE( br );
				break;
			case 5:	//	ID_PCE
				break;
			case 6:	//	ID_FIL
				SkipFIL( br );
				break;
			case 7:	//	ID_END
				{	UI8 remainder = br._ % 8;
					if ( remainder ) br.Skip( 8 - remainder );
				}
				break;
			default:
				throw "eh?";
			}
		}
		return $;
	}
};
