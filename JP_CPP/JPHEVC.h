#include "JPVideo.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

inline vector< vector< UI1 > >
HEVC_GOPTail( const vector< vector< UI1 > >& gop, UI8 nSkips ) {

	HevcVideoParameterSet		vps;
	HevcSequenceParameterSet	sps;
	HevcPictureParameterSet		pps;
	for ( auto& nal: gop ) {
		switch ( ( nal[ 0 ] >> 1 ) & 0x3f ) {
		case 32:
			vps.Parse( &nal[ 0 ], (UI4)nal.size() );
			break;
		case 33:
			sps.Parse( &nal[ 0 ], (UI4)nal.size() );
			break;
		case 34:
			pps.Parse( &nal[ 0 ], (UI4)nal.size() );
			break;
		}
	}
	
	vector< vector< UI1 > >	$;

	auto encoder = avcodec_find_encoder( AV_CODEC_ID_HEVC );
	auto context = avcodec_alloc_context3( encoder );
	context->width = sps.pic_width_in_luma_samples;
	context->height = sps.pic_height_in_luma_samples;
	context->time_base.num = 1;
	context->time_base.den = 60;
	context->framerate.num = 60;
	context->framerate.den = 1;
	context->pix_fmt = AV_PIX_FMT_YUV420P10LE;
	context->gop_size = 60 * 2;
	context->max_b_frames = 0;
	context->refs = 3;
	av_opt_set( context->priv_data, "preset", "ultrafast", 0 );
	av_opt_set( context->priv_data, "crf", "35", 0 );
	av_opt_set( context->priv_data, "tune", "zerolatency", 0 );
	A( avcodec_open2( context, encoder, 0 ) == 0 );

	auto packet = av_packet_alloc();
	auto ReceiveEncoded = [ & ] () {
		while ( true ) {
			auto _ = avcodec_receive_packet( context, packet );
			if ( _ ) {
				if (
					_ == AVERROR( EAGAIN )
				||	_ == AVERROR_EOF
				) break;
				throw _;
			}
			Concat( $, NALs_TS( vector< UI1 >( packet->data, packet->data + packet->size ) ) );
		}
	};
	auto frame		= av_frame_alloc();

	frame->format	= context->pix_fmt;
	frame->height	= context->height;
	frame->width	= context->width;

	av_frame_get_buffer( frame, 0 );
	av_frame_make_writable( frame );
	frame->pts = 1;
	frame->key_frame = 1;
	frame->pict_type = AV_PICTURE_TYPE_I;

	auto decoder = avcodec_find_decoder( AV_CODEC_ID_HEVC );
	auto decoderContext = avcodec_alloc_context3( decoder );
	A( avcodec_open2( decoderContext, decoder, 0 ) == 0 );

	auto decoderFrame = av_frame_alloc();
	auto decoderPacket = av_packet_alloc();

	auto ReceiveDecoded = [ & ] () {
		static auto iFrame = 0;
		while ( true ) {
			auto _ = avcodec_receive_frame( decoderContext, decoderFrame );
			if ( _ ) {
				if (
					_ == AVERROR( EAGAIN )
				||	_ == AVERROR_EOF
				) break;
				throw _;
			}
//cerr << iFrame;
			if ( iFrame < nSkips ) {
//cerr << ": Skipping!" << endl;
			} else {
				frame->data[ 0 ] = decoderFrame->data[ 0 ];
				frame->data[ 1 ] = decoderFrame->data[ 1 ];
				frame->data[ 2 ] = decoderFrame->data[ 2 ];
				A( avcodec_send_frame( context, frame ) == 0 );
				
				ReceiveEncoded();

				frame->pts++;
				frame->key_frame = 0;
				frame->pict_type = AV_PICTURE_TYPE_P;
				
//cerr << endl;
			}
			iFrame++;
		}
	};
	for ( auto& nal: gop ) {
		vector< UI1 > data{ 0, 0, 0, 1 };
		Concat( data, nal );
		decoderPacket->size = (int)data.size();
		decoderPacket->data = (unsigned char*)&data[ 0 ];
		A( avcodec_send_packet( decoderContext, decoderPacket ) == 0 );
		ReceiveDecoded();
		av_packet_unref( decoderPacket );
	}
	A( avcodec_send_packet( decoderContext, 0 ) == 0 );
	ReceiveDecoded();

	avcodec_free_context( &decoderContext );
	avcodec_free_context( &context );
	av_frame_free( &decoderFrame );
	av_frame_free( &frame );
	av_packet_free( &decoderPacket );
	av_packet_free( &packet );
	
	return $;
}


inline bool
HEVC_IsPicture( const vector< UI1 >& _ ) {
	switch ( ( _[ 0 ] >> 1 ) & 0x3f ) {
	case  0: //	return "TRAIL_N - Coded slice segment of a non-TSA, non-STSA trailing picture";
	case  1: //	return "TRAIL_R - Coded slice segment of a non-TSA, non-STSA trailing picture";
	case 16: //	return "BLA_W_LP - Coded slice segment of a BLA picture";
	case 17: //	return "BLA_W_RADL - Coded slice segment of a BLA picture";
	case 18: //	return "BLA_N_LP - Coded slice segment of a BLA picture";
	case 19: //	return "IDR_W_RADL - Coded slice segment of an IDR picture";
	case 20: //	return "IDR_N_LP - Coded slice segment of an IDR picture";
	case 21: //	return "CRA_NUT - Coded slice segment of a CRA picture";
		return true;
	default:
		return false;
	}
}

inline UI8
CountPictures( const vector< vector< UI1 > >& _ ) {
	return accumulate(
		_.begin()
	,	_.end()
	,	(UI8)0
	,	[] ( UI8 $, vector< UI1 >_ ) {
			return HEVC_IsPicture( _ ) ? $ + 1 : $;
		}
	);
}

inline vector< vector< UI1 > >
HEVCTail( const vector< vector< UI1 > >& _, UI8 nSkips ) {
	vector< vector< vector< UI1 > > > gops;
	for ( auto& nal: _ ) {
		if ( ( ( nal[ 0 ] >> 1 ) & 0x3F ) == 39 ) {
			unsigned int payloadType = 0;
			auto _ = &nal[ 2 ];
			while ( *_++ == 0xff ) payloadType += 255;
			payloadType += *( _ - 1 );
			if ( payloadType == 0 ) gops.emplace_back( vector< vector< UI1 > >() );
		}
		gops.back().emplace_back( nal );
	}
	vector< vector< UI1 > >	$;
	auto iGOP = 0;
	while ( iGOP < gops.size() ) {
		auto _ = CountPictures( gops[ iGOP ] );
		if ( nSkips < _ ) break;
		iGOP++;
		nSkips -= _;
	}
	Concat( $, HEVC_GOPTail( gops[ iGOP ], nSkips ) );
	for ( auto _ = iGOP + 1; _ < gops.size(); _++ ) {
		for ( auto& nal: gops[ _ ] ) $.emplace_back( nal );
	}
	return $;
}

inline vector< vector< UI1 > >
HEVCHead( const vector< vector< UI1 > >& _, UI8 nIncludes ) {
	vector< vector< UI1 > >	$;
	UI8	iFrame = 0;
	for ( auto& nal: _ ) {
		if ( iFrame == nIncludes ) break;
		if ( HEVC_IsPicture( nal ) ) iFrame++;
		$.emplace_back( nal );
	}
	return $;
}

