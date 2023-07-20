#pragma once
//	9/30/2021 Written by Satoru Ogura.

#include	"JP.h"
using namespace JP;

#define	AVC_NAL_UNIT_TYPE_UNSPECIFIED                       0
#define	AVC_NAL_UNIT_TYPE_CODED_SLICE_OF_NON_IDR_PICTURE    1
#define	AVC_NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A      2
#define	AVC_NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_B      3
#define	AVC_NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_C      4
#define	AVC_NAL_UNIT_TYPE_CODED_SLICE_OF_IDR_PICTURE        5
//const unsigned int AVC_NAL_UNIT_TYPE_SEI                               = 6;
//const unsigned int AVC_NAL_UNIT_TYPE_SPS                               = 7;
//const unsigned int AVC_NAL_UNIT_TYPE_PPS                               = 8;
//const unsigned int AVC_NAL_UNIT_TYPE_ACCESS_UNIT_DELIMITER             = 9;
//const unsigned int AVC_NAL_UNIT_TYPE_END_OF_SEQUENCE                   = 10;
//const unsigned int AVC_NAL_UNIT_TYPE_END_OF_STREAM                     = 11;
//const unsigned int AVC_NAL_UNIT_TYPE_FILLER_DATA                       = 12;
//const unsigned int AVC_NAL_UNIT_TYPE_SPS_EXTENSION                     = 13;
//const unsigned int AVC_NAL_UNIT_TYPE_PREFIX                            = 14;
//const unsigned int AVC_NAL_UNIT_TYPE_SUBSET_SPS                        = 15;
//const unsigned int AVC_NAL_UNIT_TYPE_CODED_SLICE_OF_AUXILIARY_PICTURE  = 19;
//const unsigned int AVC_NAL_UNIT_TYPE_CODED_SLICE_IN_SCALABLE_EXTENSION = 20;
//const unsigned int AVC_NAL_UNIT_TYPE_UNSPECIFIED28                     = 28;
//const unsigned int AVC_NAL_UNIT_TYPE_UNSPECIFIED30                     = 30;
//
#define	AVC_SLICE_TYPE_P                                    0
#define	AVC_SLICE_TYPE_B                                    1
#define	AVC_SLICE_TYPE_I                                    2
#define	AVC_SLICE_TYPE_SP                                   3
#define	AVC_SLICE_TYPE_SI                                   4
//
#define	AVC_SPS_MAX_ID                                      255
#define	AVC_SPS_MAX_NUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE	256
#define	AVC_SPS_MAX_SCALING_LIST_COUNT						12
//
#define	AVC_PPS_MAX_ID										255
#define	AVC_PPS_MAX_SLICE_GROUPS							256
#define	AVC_PPS_MAX_PIC_SIZE_IN_MAP_UNITS                   65536
typedef struct {
    int scale[16];
} AvcScalingList4x4;
struct AvcSequenceParameterSet {
    unsigned int profile_idc;
    unsigned int constraint_set0_flag;
    unsigned int constraint_set1_flag;
    unsigned int constraint_set2_flag;
    unsigned int constraint_set3_flag;
    unsigned int constraint_set4_flag;
    unsigned int constraint_set5_flag;
    unsigned int level_idc;
    unsigned int seq_parameter_set_id;
    unsigned int chroma_format_idc;
    unsigned int separate_colour_plane_flag;
    unsigned int bit_depth_luma_minus8;
    unsigned int bit_depth_chroma_minus8;
    unsigned int qpprime_y_zero_transform_bypass_flag;
    unsigned int seq_scaling_matrix_present_flag;
    AvcScalingList4x4 scaling_list_4x4[6];
    bool                  use_default_scaling_matrix_4x4[AVC_SPS_MAX_SCALING_LIST_COUNT];
    AvcScalingList4x4 scaling_list_8x8[6];
    unsigned char         use_default_scaling_matrix_8x8[AVC_SPS_MAX_SCALING_LIST_COUNT];
    unsigned int log2_max_frame_num_minus4;
    unsigned int pic_order_cnt_type;
    unsigned int log2_max_pic_order_cnt_lsb_minus4;
    unsigned int delta_pic_order_always_zero_flags;
    int          offset_for_non_ref_pic;
    int          offset_for_top_to_bottom_field;
    unsigned int num_ref_frames_in_pic_order_cnt_cycle;
    unsigned int offset_for_ref_frame[AVC_SPS_MAX_NUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE];
    unsigned int num_ref_frames;
    unsigned int gaps_in_frame_num_value_allowed_flag;
    unsigned int pic_width_in_mbs_minus1;
    unsigned int pic_height_in_map_units_minus1;
    unsigned int frame_mbs_only_flag;
    unsigned int mb_adaptive_frame_field_flag;
    unsigned int direct_8x8_inference_flag;
    unsigned int frame_cropping_flag;
    unsigned int frame_crop_left_offset;
    unsigned int frame_crop_right_offset;
    unsigned int frame_crop_top_offset;
    unsigned int frame_crop_bottom_offset;
};
struct AvcPictureParameterSet {
    unsigned int pic_parameter_set_id;
    unsigned int seq_parameter_set_id;
    unsigned int entropy_coding_mode_flag;
    unsigned int pic_order_present_flag;
    unsigned int num_slice_groups_minus1;
    unsigned int slice_group_map_type;
    unsigned int run_length_minus1[AVC_PPS_MAX_SLICE_GROUPS];
    unsigned int top_left[AVC_PPS_MAX_SLICE_GROUPS];
    unsigned int bottom_right[AVC_PPS_MAX_SLICE_GROUPS];
    unsigned int slice_group_change_direction_flag;
    unsigned int slice_group_change_rate_minus1;
    unsigned int pic_size_in_map_units_minus1;
    unsigned int num_ref_idx_10_active_minus1;
    unsigned int num_ref_idx_11_active_minus1;
    unsigned int weighted_pred_flag;
    unsigned int weighted_bipred_idc;
    int          pic_init_qp_minus26;
    int          pic_init_qs_minus26;
    int          chroma_qp_index_offset;
    unsigned int deblocking_filter_control_present_flag;
    unsigned int constrained_intra_pred_flag;
    unsigned int redundant_pic_cnt_present_flag;
};
struct AvcSliceHeader {
    unsigned int first_mb_in_slice;
    unsigned int slice_type;
    unsigned int pic_parameter_set_id;
    unsigned int colour_plane_id;
    unsigned int frame_num;
    unsigned int field_pic_flag;
    unsigned int bottom_field_flag;
    unsigned int idr_pic_id;
    unsigned int pic_order_cnt_lsb;
    int          delta_pic_order_cnt[2];
    unsigned int redundant_pic_cnt;
    unsigned int direct_spatial_mv_pred_flag;
    unsigned int num_ref_idx_active_override_flag;
    unsigned int num_ref_idx_l0_active_minus1;
    unsigned int num_ref_idx_l1_active_minus1;
    unsigned int ref_pic_list_reordering_flag_l0;
    unsigned int reordering_of_pic_nums_idc;
    unsigned int abs_diff_pic_num_minus1;
    unsigned int long_term_pic_num;
    unsigned int ref_pic_list_reordering_flag_l1;
    unsigned int luma_log2_weight_denom;
    unsigned int chroma_log2_weight_denom;
    unsigned int cabac_init_idc;
    unsigned int slice_qp_delta;
    unsigned int sp_for_switch_flag;
    int          slice_qs_delta;
    unsigned int disable_deblocking_filter_idc;
    int          slice_alpha_c0_offset_div2;
    int          slice_beta_offset_div2;
    unsigned int slice_group_change_cycle;
    unsigned int no_output_of_prior_pics_flag;
    unsigned int long_term_reference_flag;
    unsigned int difference_of_pic_nums_minus1;
    unsigned int long_term_frame_idx;
    unsigned int max_long_term_frame_idx_plus1;
};
inline	AvcSequenceParameterSet
AvcSPS( const UI1* ptr ) {

	AvcSequenceParameterSet	sps;
	memset( &sps, 0, sizeof( sps ) );
	
	BitReader	bits( ptr );
    bits.Skip( 8 ); // NAL Unit Type

    sps.profile_idc = (unsigned int)bits.Read( 8 );
    sps.constraint_set0_flag = bits.Read();
    sps.constraint_set1_flag = bits.Read();
    sps.constraint_set2_flag = bits.Read();
    sps.constraint_set3_flag = bits.Read();
    sps.constraint_set4_flag = bits.Read();
    sps.constraint_set5_flag = bits.Read();
    bits.Skip( 2 );
    sps.level_idc = (unsigned int)bits.Read( 8 );
    sps.seq_parameter_set_id = (unsigned int)bits.ReadGolomb();
    if (sps.seq_parameter_set_id > AVC_SPS_MAX_ID) throw "ERROR_INVALID_FORMAT";
    if (sps.profile_idc  ==  100
	||	sps.profile_idc  ==  110
    ||	sps.profile_idc  ==  122
    ||	sps.profile_idc  ==  244
    ||	sps.profile_idc  ==  44
    ||	sps.profile_idc  ==  83
	||	sps.profile_idc  ==  86
	||	sps.profile_idc  ==  118
	||	sps.profile_idc  ==  128
	||	sps.profile_idc  ==  138
	||	sps.profile_idc  ==  139
	||	sps.profile_idc  ==  134
	) {
        sps.chroma_format_idc = (unsigned int)bits.ReadGolomb();
        sps.separate_colour_plane_flag = 0;
        if (sps.chroma_format_idc == 3) {
            sps.separate_colour_plane_flag = bits.Read();	//	ORG SKIP
        }
        sps.bit_depth_luma_minus8 = (unsigned int)bits.ReadGolomb();
        sps.bit_depth_chroma_minus8 = (unsigned int)bits.ReadGolomb();
        sps.qpprime_y_zero_transform_bypass_flag = bits.Read();	//	ORG SKIP
        sps.seq_scaling_matrix_present_flag = bits.Read();
        if ( sps.seq_scaling_matrix_present_flag ) {
            for ( int i = 0; i < ( sps.chroma_format_idc != 3 ? 8 : 12 ); i++ ) {
                unsigned int seq_scaling_list_present_flag = bits.Read();	//	ORG SKIP
                if (seq_scaling_list_present_flag) {
					int last_scale = 8;
					int next_scale = 8;
                    if ( i < 6 ) {
                        for ( unsigned int j = 0; j < 16; j++ ) {
                            if ( next_scale ) {
                                auto delta_scale = bits.ReadSignedGolomb();
                                next_scale = ( last_scale + delta_scale + 256 ) % 256;
                                sps.use_default_scaling_matrix_4x4[ i ] = ( j == 0 && next_scale == 0 );
                            }
                            sps.scaling_list_4x4[ i ].scale[ j ] = ( next_scale == 0 ? last_scale : next_scale );
                            last_scale = sps.scaling_list_4x4[ i ].scale[ j ];
                        }
                    } else {
                        for ( unsigned int j = 0; j < 64; j++ ) {
                            if ( next_scale ) {
                                auto delta_scale = bits.ReadSignedGolomb();
                                next_scale = ( last_scale + delta_scale + 256 ) % 256;
                                sps.use_default_scaling_matrix_8x8[ i - 6 ] = ( j == 0 && next_scale == 0 );
                            }
                            sps.scaling_list_8x8[ i - 6 ].scale[ j ] = ( next_scale == 0 ? last_scale : next_scale );
                            last_scale = sps.scaling_list_8x8[ i - 6 ].scale[ j ];
                        }
                    }
                }
            }
        }
    } else {
        sps.chroma_format_idc = 1;
	}
    sps.log2_max_frame_num_minus4 = (unsigned int)bits.ReadGolomb();
    sps.pic_order_cnt_type = (unsigned int)bits.ReadGolomb();
    if (sps.pic_order_cnt_type > 2) throw "ERROR_INVALID_FORMAT";
    if (sps.pic_order_cnt_type == 0) {
        sps.log2_max_pic_order_cnt_lsb_minus4 = (unsigned int)bits.ReadGolomb();
    } else if (sps.pic_order_cnt_type == 1) {
        sps.delta_pic_order_always_zero_flags = bits.Read();
        sps.offset_for_non_ref_pic = (unsigned int)bits.ReadSignedGolomb();
        sps.offset_for_top_to_bottom_field = (unsigned int)bits.ReadSignedGolomb();
        sps.num_ref_frames_in_pic_order_cnt_cycle = (unsigned int)bits.ReadGolomb();
        if (sps.num_ref_frames_in_pic_order_cnt_cycle > AVC_SPS_MAX_NUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE) throw "ERROR_INVALID_FORMAT";
        for (unsigned int i=0; i<sps.num_ref_frames_in_pic_order_cnt_cycle; i++) {
            sps.offset_for_ref_frame[i] = (unsigned int)bits.ReadSignedGolomb();
        }
    }
    sps.num_ref_frames                       = (unsigned int)bits.ReadGolomb();
    sps.gaps_in_frame_num_value_allowed_flag = bits.Read();
    sps.pic_width_in_mbs_minus1              = (unsigned int)bits.ReadGolomb();
    sps.pic_height_in_map_units_minus1       = (unsigned int)bits.ReadGolomb();
    sps.frame_mbs_only_flag                  = bits.Read();
    if (!sps.frame_mbs_only_flag) {
        sps.mb_adaptive_frame_field_flag = bits.Read();
    }
    sps.direct_8x8_inference_flag = bits.Read();
    sps.frame_cropping_flag       = bits.Read();
    if (sps.frame_cropping_flag) {
        sps.frame_crop_left_offset   = (unsigned int)bits.ReadGolomb();
        sps.frame_crop_right_offset  = (unsigned int)bits.ReadGolomb();
        sps.frame_crop_top_offset    = (unsigned int)bits.ReadGolomb();
        sps.frame_crop_bottom_offset = (unsigned int)bits.ReadGolomb();
    }

    return sps;
}
inline	AvcPictureParameterSet
AvcPPS( const UI1* ptr ) {

	AvcPictureParameterSet	pps;
	memset( &pps, 0, sizeof( pps ) );

	BitReader	bits( ptr );
    bits.Skip(8); // NAL Unit Type

    pps.pic_parameter_set_id     = (unsigned int)bits.ReadGolomb();
    if (pps.pic_parameter_set_id > AVC_PPS_MAX_ID) throw "ERROR_INVALID_FORMAT";
    pps.seq_parameter_set_id     = (unsigned int)bits.ReadGolomb();
    if (pps.seq_parameter_set_id > AVC_SPS_MAX_ID) throw "ERROR_INVALID_FORMAT";
    pps.entropy_coding_mode_flag = bits.Read();
    pps.pic_order_present_flag   = bits.Read();
    pps.num_slice_groups_minus1  = (unsigned int)bits.ReadGolomb();
    if (pps.num_slice_groups_minus1 >= AVC_PPS_MAX_SLICE_GROUPS) throw "ERROR_INVALID_FORMAT";
    if (pps.num_slice_groups_minus1 > 0) {
        pps.slice_group_map_type = (unsigned int)bits.ReadGolomb();
        if (pps.slice_group_map_type == 0) {
            for (unsigned int i=0; i<=pps.num_slice_groups_minus1; i++) {
                pps.run_length_minus1[i] = (unsigned int)bits.ReadGolomb();
            }
        } else if (pps.slice_group_map_type == 2) {
            for (unsigned int i=0; i<pps.num_slice_groups_minus1; i++) {
                pps.top_left[i] = (unsigned int)bits.ReadGolomb();
                pps.bottom_right[i] = (unsigned int)bits.ReadGolomb();
            }
        } else if (pps.slice_group_map_type == 3 ||
                   pps.slice_group_map_type == 4 ||
                   pps.slice_group_map_type == 5) {
            pps.slice_group_change_direction_flag = bits.Read();
            pps.slice_group_change_rate_minus1 = (unsigned int)bits.ReadGolomb();
        } else if (pps.slice_group_map_type == 6) {
            pps.pic_size_in_map_units_minus1 = (unsigned int)bits.ReadGolomb();
            if (pps.pic_size_in_map_units_minus1 >= AVC_PPS_MAX_PIC_SIZE_IN_MAP_UNITS) throw "ERROR_INVALID_FORMAT";
            unsigned int num_bits_per_slice_group_id;
            if (pps.num_slice_groups_minus1 + 1 > 4) {
                num_bits_per_slice_group_id = 3;
            } else if (pps.num_slice_groups_minus1 + 1 > 2) {
                num_bits_per_slice_group_id = 2;
            } else {
                num_bits_per_slice_group_id = 1;
            }
            for (unsigned int i=0; i<=pps.pic_size_in_map_units_minus1; i++) {
                /*pps.slice_group_id[i] =*/ bits.Read( num_bits_per_slice_group_id );
            }
        }
    }
    pps.num_ref_idx_10_active_minus1 = (unsigned int)bits.ReadGolomb();
    pps.num_ref_idx_11_active_minus1 = (unsigned int)bits.ReadGolomb();
    pps.weighted_pred_flag           = bits.Read();
    pps.weighted_bipred_idc          = bits.Read( 2 );
    pps.pic_init_qp_minus26          = (unsigned int)bits.ReadSignedGolomb();
    pps.pic_init_qs_minus26          = (unsigned int)bits.ReadSignedGolomb();
    pps.chroma_qp_index_offset       = (unsigned int)bits.ReadSignedGolomb();
    pps.deblocking_filter_control_present_flag = bits.Read();
    pps.constrained_intra_pred_flag            = bits.Read();
    pps.redundant_pic_cnt_present_flag         = bits.Read();
    
    return pps;
}

inline	UI8
AVCNaluHeaderSize( UI1* ptr, int nal_unit_type, int nal_ref_idc, AvcSequenceParameterSet* SPS, AvcPictureParameterSet* PPS ) {

	BitReader	bits( ptr );
	AvcSliceHeader	slice_header;
	memset( &slice_header, 0, sizeof( slice_header ) );
    slice_header.first_mb_in_slice    = (unsigned int)bits.ReadGolomb();
    slice_header.slice_type           = (unsigned int)bits.ReadGolomb();
    slice_header.pic_parameter_set_id = (unsigned int)bits.ReadGolomb();
    if (slice_header.pic_parameter_set_id > AVC_PPS_MAX_ID) throw "ERROR_INVALID_FORMAT";
    auto pps = PPS[slice_header.pic_parameter_set_id];
    auto sps = SPS[pps.seq_parameter_set_id];
    if (sps.separate_colour_plane_flag) slice_header.colour_plane_id = (unsigned int)bits.Read( 2);
    slice_header.frame_num = (unsigned int)bits.Read( sps.log2_max_frame_num_minus4 + 4);
    if (!sps.frame_mbs_only_flag) {
        slice_header.field_pic_flag = bits.Read();
        if (slice_header.field_pic_flag) {
            slice_header.bottom_field_flag = bits.Read();
        }
    }
    if (nal_unit_type == AVC_NAL_UNIT_TYPE_CODED_SLICE_OF_IDR_PICTURE) {
        slice_header.idr_pic_id = (unsigned int)bits.ReadGolomb();
    }
    if (sps.pic_order_cnt_type == 0) {
        slice_header.pic_order_cnt_lsb = (unsigned int)bits.Read( sps.log2_max_pic_order_cnt_lsb_minus4 + 4);
        if (pps.pic_order_present_flag && !slice_header.field_pic_flag) {
            slice_header.delta_pic_order_cnt[0] = (unsigned int)bits.ReadSignedGolomb();
        }
    }
    if (sps.pic_order_cnt_type == 1 && !sps.delta_pic_order_always_zero_flags) {
        slice_header.delta_pic_order_cnt[0] = (unsigned int)bits.ReadSignedGolomb();
        if (pps.pic_order_present_flag && !slice_header.field_pic_flag) {
            slice_header.delta_pic_order_cnt[1] = (unsigned int)bits.ReadSignedGolomb();
        }
    }
    if (pps.redundant_pic_cnt_present_flag) {
        slice_header.redundant_pic_cnt = (unsigned int)bits.ReadGolomb();
    }
    
    unsigned int slice_type = slice_header.slice_type % 5; // this seems to be implicit in the spec
    
    if (slice_type == AVC_SLICE_TYPE_B) {
        slice_header.direct_spatial_mv_pred_flag = bits.Read();
    }
    
    if (slice_type == AVC_SLICE_TYPE_P  ||
        slice_type == AVC_SLICE_TYPE_SP ||
        slice_type == AVC_SLICE_TYPE_B) {
        slice_header.num_ref_idx_active_override_flag = bits.Read();
        
        if (slice_header.num_ref_idx_active_override_flag) {
            slice_header.num_ref_idx_l0_active_minus1 = (unsigned int)bits.ReadGolomb();
            if ((slice_header.slice_type % 5) == AVC_SLICE_TYPE_B) {
                slice_header.num_ref_idx_l1_active_minus1 = (unsigned int)bits.ReadGolomb();
            }
        } else {
            slice_header.num_ref_idx_l0_active_minus1 = pps.num_ref_idx_10_active_minus1;
            slice_header.num_ref_idx_l1_active_minus1 = pps.num_ref_idx_11_active_minus1;
        }
    }
    
    // ref_pic_list_reordering
    if ((slice_header.slice_type % 5) != 2 && (slice_header.slice_type % 5) != 4) {
        slice_header.ref_pic_list_reordering_flag_l0 = bits.Read();
        if (slice_header.ref_pic_list_reordering_flag_l0) {
            do {
                slice_header.reordering_of_pic_nums_idc = (unsigned int)bits.ReadGolomb();
                if (slice_header.reordering_of_pic_nums_idc == 0 ||
					slice_header.reordering_of_pic_nums_idc == 1) {
                    slice_header.abs_diff_pic_num_minus1 = (unsigned int)bits.ReadGolomb();
                } else if (slice_header.reordering_of_pic_nums_idc == 2) {
                    slice_header.long_term_pic_num = (unsigned int)bits.ReadGolomb();
                }
            } while (slice_header.reordering_of_pic_nums_idc != 3);
        }
    }
    if ((slice_header.slice_type % 5) == 1) {
        slice_header.ref_pic_list_reordering_flag_l1 = bits.Read();
        if (slice_header.ref_pic_list_reordering_flag_l1) {
            do {
                slice_header.reordering_of_pic_nums_idc = (unsigned int)bits.ReadGolomb();
                if (slice_header.reordering_of_pic_nums_idc == 0 ||
					slice_header.reordering_of_pic_nums_idc == 1) {
                    slice_header.abs_diff_pic_num_minus1 = (unsigned int)bits.ReadGolomb();
                } else if (slice_header.reordering_of_pic_nums_idc == 2) {
                    slice_header.long_term_pic_num = (unsigned int)bits.ReadGolomb();
                }
            } while (slice_header.reordering_of_pic_nums_idc != 3);
        }
    }
    
    if ((pps.weighted_pred_flag &&
        (slice_type == AVC_SLICE_TYPE_P || slice_type == AVC_SLICE_TYPE_SP)) ||
		(pps.weighted_bipred_idc == 1 && slice_type == AVC_SLICE_TYPE_B)) {
        // pred_weight_table
        slice_header.luma_log2_weight_denom = (unsigned int)bits.ReadGolomb();
        
        if (sps.chroma_format_idc != 0) {
            slice_header.chroma_log2_weight_denom = (unsigned int)bits.ReadGolomb();
        }
        
        for (unsigned int i=0; i<=slice_header.num_ref_idx_l0_active_minus1; i++) {
            unsigned int luma_weight_l0_flag = bits.Read();
            if (luma_weight_l0_flag) {
                /* slice_header.luma_weight_l0[i] = SignedGolomb( */ bits.ReadGolomb();
                /* slice_header.luma_offset_l0[i] = SignedGolomb( */ bits.ReadGolomb();
            }
            if (sps.chroma_format_idc != 0) {
                unsigned int chroma_weight_l0_flag = bits.Read();
                if (chroma_weight_l0_flag) {
                    for (unsigned int j=0; j<2; j++) {
                        /* slice_header.chroma_weight_l0[i][j] = SignedGolomb( */ bits.ReadGolomb();
                        /* slice_header.chroma_offset_l0[i][j] = SignedGolomb( */ bits.ReadGolomb();
                    }
                }
            }
        }
        if ((slice_header.slice_type % 5) == 1) {
            for (unsigned int i=0; i<=slice_header.num_ref_idx_l1_active_minus1; i++) {
                unsigned int luma_weight_l1_flag = bits.Read();
                if (luma_weight_l1_flag) {
                    /* slice_header.luma_weight_l1[i] = SignedGolomb( */ bits.ReadGolomb();
                    /* slice_header.luma_offset_l1[i] = SignedGolomb( */ bits.ReadGolomb();
                }
                if (sps.chroma_format_idc != 0) {
                    unsigned int chroma_weight_l1_flag = bits.Read();
                    if (chroma_weight_l1_flag) {
                        for (unsigned int j=0; j<2; j++) {
                            /* slice_header.chroma_weight_l1[i][j] = SignedGolomb( */ bits.ReadGolomb();
                            /* slice_header.chroma_offset_l1[i][j] = SignedGolomb( */ bits.ReadGolomb();
                        }
                    }
                }
            }
        }
    }

    if (nal_ref_idc != 0) {
        // dec_ref_pic_marking
        if (nal_unit_type == AVC_NAL_UNIT_TYPE_CODED_SLICE_OF_IDR_PICTURE) {
            slice_header.no_output_of_prior_pics_flag = bits.Read();
            slice_header.long_term_reference_flag     = bits.Read();
        } else {
            unsigned int adaptive_ref_pic_marking_mode_flag = bits.Read();
            if (adaptive_ref_pic_marking_mode_flag) {
                unsigned int memory_management_control_operation = 0;
                do {
                    memory_management_control_operation = (unsigned int)bits.ReadGolomb();
                    if (memory_management_control_operation == 1 || memory_management_control_operation == 3) {
                        slice_header.difference_of_pic_nums_minus1 = (unsigned int)bits.ReadGolomb();
                    }
                    if (memory_management_control_operation == 2) {
                        slice_header.long_term_pic_num = (unsigned int)bits.ReadGolomb();
                    }
                    if (memory_management_control_operation == 3 || memory_management_control_operation == 6) {
                        slice_header.long_term_frame_idx = (unsigned int)bits.ReadGolomb();
                    }
                    if (memory_management_control_operation == 4) {
                        slice_header.max_long_term_frame_idx_plus1 = (unsigned int)bits.ReadGolomb();
                    }
                } while (memory_management_control_operation != 0);
            }
        }
    }
    if (pps.entropy_coding_mode_flag && slice_type != AVC_SLICE_TYPE_I && slice_type != AVC_SLICE_TYPE_SI) {
        slice_header.cabac_init_idc = (unsigned int)bits.ReadGolomb();
    }
    slice_header.slice_qp_delta = (unsigned int)bits.ReadGolomb();
    if (slice_type == AVC_SLICE_TYPE_SP || slice_type == AVC_SLICE_TYPE_SI) {
        if (slice_type == AVC_SLICE_TYPE_SP) {
            slice_header.sp_for_switch_flag = bits.Read();
        }
        slice_header.slice_qs_delta = (unsigned int)bits.ReadSignedGolomb();
    }
    if (pps.deblocking_filter_control_present_flag) {
        slice_header.disable_deblocking_filter_idc = (unsigned int)bits.ReadGolomb();
        if (slice_header.disable_deblocking_filter_idc != 1) {
            slice_header.slice_alpha_c0_offset_div2 = (unsigned int)bits.ReadSignedGolomb();
            slice_header.slice_beta_offset_div2     = (unsigned int)bits.ReadSignedGolomb();
        }
    }
    if (pps.num_slice_groups_minus1 > 0 &&
        pps.slice_group_map_type >= 3   &&
        pps.slice_group_map_type <= 5) {
        slice_header.slice_group_change_cycle = (unsigned int)bits.ReadGolomb();
    }
    return ( bits._ + 7 ) / 8;
}

typedef int            AP4_Result;
typedef unsigned int   AP4_Flags;
typedef unsigned int   AP4_Mask;
typedef unsigned int   AP4_Cardinal;
typedef unsigned int   AP4_Ordinal;
typedef unsigned int   AP4_UI32;
typedef signed   int   AP4_SI32;
typedef unsigned short AP4_UI16;
typedef signed   short AP4_SI16;
typedef unsigned char  AP4_UI08;
typedef AP4_UI08       AP4_Byte;
typedef AP4_UI32       AP4_Size;

typedef unsigned long long	AP4_UI64;

struct
HevcProfileTierLevel {
    unsigned int general_profile_space;
    unsigned int general_tier_flag;
    unsigned int general_profile_idc;
    AP4_UI32     general_profile_compatibility_flags;
    
    // this is a synthetic field that includes 48 bits starting with:
    // general_progressive_source_flag    (1 bit)
    // general_interlaced_source_flag     (1 bit)
    // general_non_packed_constraint_flag (1 bit)
    // general_frame_only_constraint_flag (1 bit)
    // general_reserved_zero_44bits       (44 bits)
    AP4_UI64     general_constraint_indicator_flags;

    unsigned int general_level_idc;
    struct {
        unsigned char sub_layer_profile_present_flag;
        unsigned char sub_layer_level_present_flag;
        unsigned char sub_layer_profile_space;
        unsigned char sub_layer_tier_flag;
        unsigned char sub_layer_profile_idc;
        AP4_UI32      sub_layer_profile_compatibility_flags;
        unsigned char sub_layer_progressive_source_flag;
        unsigned char sub_layer_interlaced_source_flag;
        unsigned char sub_layer_non_packed_constraint_flag;
        unsigned char sub_layer_frame_only_constraint_flag;
        unsigned char sub_layer_level_idc;
    } sub_layer_info[8];

	void
	Parse( BitReader& bits, unsigned int max_num_sub_layers_minus_1)
	{
		// profile_tier_level
		general_profile_space               = bits.Read( 2);
		general_tier_flag                   = bits.Read();
		general_profile_idc                 = bits.Read( 5);
		general_profile_compatibility_flags = bits.Read( 32);
		
		general_constraint_indicator_flags  = ((AP4_UI64)bits.Read( 16)) << 32;
		general_constraint_indicator_flags |= bits.Read( 32);
		
		general_level_idc                   = bits.Read( 8);
		for (unsigned int i = 0; i < max_num_sub_layers_minus_1; i++) {
			sub_layer_info[i].sub_layer_profile_present_flag = bits.Read();
			sub_layer_info[i].sub_layer_level_present_flag   = bits.Read();
		}
		if (max_num_sub_layers_minus_1) {
			for (unsigned int i = max_num_sub_layers_minus_1; i < 8; i++) {
				bits.Read( 2); // reserved_zero_2bits[i]
			}
		}
		for (unsigned int i = 0; i < max_num_sub_layers_minus_1; i++) {
			if (sub_layer_info[i].sub_layer_profile_present_flag) {
				sub_layer_info[i].sub_layer_profile_space               = bits.Read( 2);
				sub_layer_info[i].sub_layer_tier_flag                   = bits.Read();
				sub_layer_info[i].sub_layer_profile_idc                 = bits.Read( 5);
				sub_layer_info[i].sub_layer_profile_compatibility_flags = bits.Read( 32);
				sub_layer_info[i].sub_layer_progressive_source_flag     = bits.Read();
				sub_layer_info[i].sub_layer_interlaced_source_flag      = bits.Read();
				sub_layer_info[i].sub_layer_non_packed_constraint_flag  = bits.Read();
				sub_layer_info[i].sub_layer_frame_only_constraint_flag  = bits.Read();
				bits.Read( 32); bits.Read( 12); // sub_layer_reserved_zero_44bits
			}
			if (sub_layer_info[i].sub_layer_level_present_flag) {
				sub_layer_info[i].sub_layer_level_idc = bits.Read( 8);
			}
		}
	}
};

inline	void
Unescape( const unsigned char* in, unsigned int in_size, unsigned char* out ) {
    unsigned int zero_count = 0;
    unsigned int bytes_removed = 0;
    for (unsigned int i=0; i<in_size; i++) {
        if (zero_count == 2 && in[i] == 3 && i+1 < in_size && in[i+1] <= 3) {
            ++bytes_removed;
            zero_count = 0;
        } else {
            out[i-bytes_removed] = in[i];
            if (in[i] == 0) {
                ++zero_count;
            } else {
                zero_count = 0;
            }
        }
    }
}
struct
HevcVideoParameterSet {
    unsigned int             vps_video_parameter_set_id;
    unsigned int             vps_max_layers_minus1;
    unsigned int             vps_max_sub_layers_minus1;
    unsigned int             vps_temporal_id_nesting_flag;
    HevcProfileTierLevel     profile_tier_level;
    unsigned int             vps_sub_layer_ordering_info_present_flag;
    unsigned int             vps_max_dec_pic_buffering_minus1[8];
    unsigned int             vps_max_num_reorder_pics[8];
    unsigned int             vps_max_latency_increase_plus1[8];
    unsigned int             vps_max_layer_id;
    unsigned int             vps_num_layer_sets_minus1;
    unsigned int             vps_timing_info_present_flag;
    unsigned int             vps_num_units_in_tick;
    unsigned int             vps_time_scale;
    unsigned int             vps_poc_proportional_to_timing_flag;
    unsigned int             vps_num_ticks_poc_diff_one_minus1;
    
    void
    Parse( const unsigned char* data, unsigned int data_size ) {

		UI1 $[ data_size ];
		Unescape( data, data_size, $ );
		BitReader bits( $ );

		bits.Skip( 16 ); // NAL Unit Header

		vps_video_parameter_set_id     = bits.Read( 4);
		/* vps_reserved_three_2bits */   bits.Read( 2);
		vps_max_layers_minus1          = bits.Read( 6);
		vps_max_sub_layers_minus1      = bits.Read( 3);
		vps_temporal_id_nesting_flag   = bits.Read();
		/* vps_reserved_0xffff_16bits */ bits.Read( 16);
		profile_tier_level.Parse(bits, vps_max_sub_layers_minus1);
		vps_sub_layer_ordering_info_present_flag = bits.Read();
		for (unsigned int i = (vps_sub_layer_ordering_info_present_flag ? 0 : vps_max_sub_layers_minus1);
						  i <= vps_max_sub_layers_minus1;
						  i++) {
			vps_max_dec_pic_buffering_minus1[i] = bits.ReadGolomb();
			vps_max_num_reorder_pics[i]         = bits.ReadGolomb();
			vps_max_latency_increase_plus1[i]   = bits.ReadGolomb();
		}
		vps_max_layer_id          = bits.Read( 6);
		vps_num_layer_sets_minus1 = bits.ReadGolomb();
		for (unsigned int i = 1; i <= vps_num_layer_sets_minus1; i++) {
			for (unsigned int j = 0; j <= vps_max_layer_id; j++) {
				bits.Read();
			}
		}
		vps_timing_info_present_flag = bits.Read();
		if (vps_timing_info_present_flag) {
			vps_num_units_in_tick               = bits.Read( 32);
			vps_time_scale                      = bits.Read( 32);
			vps_poc_proportional_to_timing_flag = bits.Read();
			if (vps_poc_proportional_to_timing_flag) {
				vps_num_ticks_poc_diff_one_minus1 = bits.ReadGolomb();
			}
		}
	}
};

struct
HevcVuiParameters {
  unsigned int aspect_ratio_info_present_flag;
  unsigned int aspect_ratio_idc;
  unsigned int sar_width;
  unsigned int sar_height;
  unsigned int overscan_info_present_flag;
  unsigned int overscan_appropriate_flag;
  unsigned int video_signal_type_present_flag;
  unsigned int video_format;
  unsigned int video_full_range_flag;
  unsigned int colour_description_present_flag;
  unsigned int colour_primaries;
  unsigned int transfer_characteristics;
  unsigned int matrix_coeffs;
  //unsigned int chroma_loc_info_present_flag;
  //unsigned int chroma_sample_loc_type_top_field;
  //unsigned int chroma_sample_loc_type_bottom_field;
  //unsigned int neutral_chroma_indication_flag;
  //unsigned int field_seq_flag;
  //unsigned int frame_field_info_present_flag;
  //unsigned int default_display_window_flag;
  //unsigned int def_disp_win_left_offset;
  //unsigned int def_disp_win_right_offset;
  //unsigned int def_disp_win_top_offset;
  //unsigned int def_disp_win_bottom_offset;
  //unsigned int vui_timing_info_present_flag;
  //unsigned int vui_num_units_in_tick;
  //unsigned int vui_time_scale;
  //unsigned int vui_poc_proportional_to_timing_flag;
  //unsigned int vui_num_ticks_poc_diff_one_minus1;
  //unsigned int vui_hrd_parameters_present_flag;

  //// skip hrd_parameters
  //unsigned int bitstream_restriction_flag;
  //unsigned int tiles_fixed_structure_flag;
  //unsigned int motion_vectors_over_pic_boundaries_flag;
  //unsigned int restricted_ref_pic_lists_flag;
  //unsigned int min_spatial_segmentation_idc;
  //unsigned int max_bytes_per_pic_denom;
  //unsigned int max_bits_per_min_cu_denom;
  //unsigned int log2_max_mv_length_horizontal;
  //unsigned int log2_max_mv_length_vertical;

	void
	Parse(BitReader& bits, unsigned int& transfer_characteristics)
	{
		// vui_parameters
		aspect_ratio_info_present_flag = bits.Read();
		if (aspect_ratio_info_present_flag) {
			aspect_ratio_idc = bits.Read( 8);
			if (aspect_ratio_idc == 255) {
				sar_width  = bits.Read( 16);
				sar_height = bits.Read( 16);
			}
		}
		overscan_info_present_flag = bits.Read() ;
		if (overscan_info_present_flag) overscan_appropriate_flag = bits.Read() ;
		video_signal_type_present_flag = bits.Read() ;
		if (video_signal_type_present_flag) {
			video_format                    = bits.Read( 3);
			video_full_range_flag           = bits.Read() ;
			colour_description_present_flag = bits.Read() ;
			if (colour_description_present_flag) {
				colour_primaries         = bits.Read( 8);
				transfer_characteristics = bits.Read( 8);
				matrix_coeffs            = bits.Read( 8);
			}
		}
	}
};

struct
HevcShortTermRefPicSet {
    unsigned int delta_poc_s0_minus1[16];
    unsigned int delta_poc_s1_minus1[16];
    unsigned int used_by_curr_pic_s0_flag[16];
    unsigned int used_by_curr_pic_s1_flag[16];
    unsigned int num_negative_pics;
    unsigned int num_positive_pics;
    unsigned int num_delta_pocs;
};

inline void
scaling_list_data(BitReader& bits)
{
    for (unsigned int sizeId = 0; sizeId < 4; sizeId++) {
        for (unsigned int matrixId = 0; matrixId < (unsigned int)((sizeId == 3)?2:6); matrixId++) {
            unsigned int flag = bits.Read(); // scaling_list_pred_mode_flag[ sizeId ][ matrixId ]
            if (!flag) {
                bits.ReadGolomb(); // scaling_list_pred_matrix_id_delta[ sizeId ][ matrixId ]
            } else {
                // nextCoef = 8;
                unsigned int coefNum = (1 << (4+(sizeId << 1)));
                if (coefNum > 64) coefNum = 64;
                if (sizeId > 1) {
                    bits.ReadGolomb(); // scaling_list_dc_coef_minus8[ sizeId − 2 ][ matrixId ]
                    // nextCoef = scaling_list_dc_coef_minus8[ sizeId − 2 ][ matrixId ] + 8
                }
                for (unsigned i = 0; i < coefNum; i++) {
                    bits.ReadGolomb(); // scaling_list_delta_coef
                    // nextCoef = ( nextCoef + scaling_list_delta_coef + 256 ) % 256
                    // ScalingList[ sizeId ][ matrixId ][ i ] = nextCoef
                }
            }
        }
    }
}

struct
HevcSequenceParameterSet {
    unsigned int             sps_video_parameter_set_id;
    unsigned int             sps_max_sub_layers_minus1;
    unsigned int             sps_temporal_id_nesting_flag;
    HevcProfileTierLevel     profile_tier_level;
    unsigned int             sps_seq_parameter_set_id;
    unsigned int             chroma_format_idc;
    unsigned int             separate_colour_plane_flag;
    unsigned int             pic_width_in_luma_samples;
    unsigned int             pic_height_in_luma_samples;
    unsigned int             conformance_window_flag;
    unsigned int             conf_win_left_offset;
    unsigned int             conf_win_right_offset;
    unsigned int             conf_win_top_offset;
    unsigned int             conf_win_bottom_offset;
    unsigned int             bit_depth_luma_minus8;
    unsigned int             bit_depth_chroma_minus8;
    unsigned int             sps_max_dec_pic_buffering_minus1[8];
    unsigned int             sps_max_num_reorder_pics[8];
    unsigned int             sps_max_latency_increase_plus1[8];
    unsigned int             log2_max_pic_order_cnt_lsb_minus4;
    unsigned int             sps_sub_layer_ordering_info_present_flag;
    unsigned int             log2_min_luma_coding_block_size_minus3;
    unsigned int             log2_diff_max_min_luma_coding_block_size;
    unsigned int             log2_min_transform_block_size_minus2;
    unsigned int             log2_diff_max_min_transform_block_size;
    unsigned int             max_transform_hierarchy_depth_inter;
    unsigned int             max_transform_hierarchy_depth_intra;
    unsigned int             scaling_list_enabled_flag;
    unsigned int             sps_scaling_list_data_present_flag;
                             // skipped scaling list data
    unsigned int             amp_enabled_flag;
    unsigned int             sample_adaptive_offset_enabled_flag;
    unsigned int             pcm_enabled_flag;
    unsigned int             pcm_sample_bit_depth_luma_minus1;
    unsigned int             pcm_sample_bit_depth_chroma_minus1;
    unsigned int             log2_min_pcm_luma_coding_block_size_minus3;
    unsigned int             log2_diff_max_min_pcm_luma_coding_block_size;
    unsigned int             pcm_loop_filter_disabled_flag;
    unsigned int             num_short_term_ref_pic_sets;
    unsigned int             long_term_ref_pics_present_flag;
    unsigned int             num_long_term_ref_pics_sps;
    unsigned int             sps_temporal_mvp_enabled_flag;
    unsigned int             strong_intra_smoothing_enabled_flag;
    unsigned int             vui_parameters_present_flag;
    HevcVuiParameters        vui_parameters;
    //	64	AP4_HEVC_SPS_MAX_RPS
    HevcShortTermRefPicSet   short_term_ref_pic_sets[ 64 ];

	void
	Parse(const unsigned char* data, unsigned int data_size)
	{
		UI1 $[ data_size ];
		Unescape( data, data_size, $ );
		BitReader bits( $ );

		bits.Skip(16); // NAL Unit Header

		sps_video_parameter_set_id   = bits.Read( 4);
		sps_max_sub_layers_minus1    = bits.Read( 3);
		sps_temporal_id_nesting_flag = bits.Read();
		
		profile_tier_level.Parse(bits, sps_max_sub_layers_minus1);
		
		sps_seq_parameter_set_id = bits.ReadGolomb();
		A (sps_seq_parameter_set_id <= 15 );	//	AP4_HEVC_SPS_MAX_ID

		chroma_format_idc = bits.ReadGolomb();
		if (chroma_format_idc == 3) {
			separate_colour_plane_flag = bits.Read();
		}
		pic_width_in_luma_samples  = bits.ReadGolomb();
		pic_height_in_luma_samples = bits.ReadGolomb();
		conformance_window_flag    = bits.Read();
		
		if (conformance_window_flag) {
			conf_win_left_offset    = bits.ReadGolomb();
			conf_win_right_offset   = bits.ReadGolomb();
			conf_win_top_offset     = bits.ReadGolomb();
			conf_win_bottom_offset  = bits.ReadGolomb();
		}
		bit_depth_luma_minus8                    = bits.ReadGolomb();
		bit_depth_chroma_minus8                  = bits.ReadGolomb();
		log2_max_pic_order_cnt_lsb_minus4        = bits.ReadGolomb();
		A(log2_max_pic_order_cnt_lsb_minus4 <= 16);
		
		sps_sub_layer_ordering_info_present_flag = bits.Read();
		for (unsigned int i = (sps_sub_layer_ordering_info_present_flag ? 0 : sps_max_sub_layers_minus1);
						  i <= sps_max_sub_layers_minus1;
						  i++) {
			sps_max_dec_pic_buffering_minus1[i] = bits.ReadGolomb();
			sps_max_num_reorder_pics[i]         = bits.ReadGolomb();
			sps_max_latency_increase_plus1[i]   = bits.ReadGolomb();
		}
		log2_min_luma_coding_block_size_minus3   = bits.ReadGolomb();
		log2_diff_max_min_luma_coding_block_size = bits.ReadGolomb();
		log2_min_transform_block_size_minus2     = bits.ReadGolomb();
		log2_diff_max_min_transform_block_size   = bits.ReadGolomb();
		max_transform_hierarchy_depth_inter      = bits.ReadGolomb();
		max_transform_hierarchy_depth_intra      = bits.ReadGolomb();
		scaling_list_enabled_flag                = bits.Read();
		if (scaling_list_enabled_flag) {
			sps_scaling_list_data_present_flag = bits.Read();
			if (sps_scaling_list_data_present_flag) {
				scaling_list_data(bits);
			}
		}
		amp_enabled_flag = bits.Read();
		sample_adaptive_offset_enabled_flag = bits.Read();
		pcm_enabled_flag = bits.Read();
		if (pcm_enabled_flag) {
			pcm_sample_bit_depth_luma_minus1 = bits.Read( 4);
			pcm_sample_bit_depth_chroma_minus1 = bits.Read( 4);
			log2_min_pcm_luma_coding_block_size_minus3 = bits.ReadGolomb();
			log2_diff_max_min_pcm_luma_coding_block_size = bits.ReadGolomb();
			pcm_loop_filter_disabled_flag = bits.Read();
		}
		num_short_term_ref_pic_sets = bits.ReadGolomb();
		A(num_short_term_ref_pic_sets <= 64 );	//	AP4_HEVC_SPS_MAX_RPS
		for (unsigned int i=0; i<num_short_term_ref_pic_sets; i++) {
//			parse_st_ref_pic_set(&short_term_ref_pic_sets[i], this, i, num_short_term_ref_pic_sets, bits);
			auto rps = &short_term_ref_pic_sets[i];
			auto sps = this;
			auto stRpsIdx = i;
			memset(rps, 0, sizeof(*rps));
			
			unsigned int inter_ref_pic_set_prediction_flag = 0;
			if (stRpsIdx != 0) {
				inter_ref_pic_set_prediction_flag = bits.Read();
			}
			if (inter_ref_pic_set_prediction_flag) {
				unsigned int delta_idx_minus1 = 0;
				if (stRpsIdx == num_short_term_ref_pic_sets) {
					delta_idx_minus1 = bits.ReadGolomb();
				}
				/* delta_rps_sign = */ bits.Read();
				/* abs_delta_rps_minus1 = */ bits.ReadGolomb();
				A (delta_idx_minus1+1 <= stRpsIdx);
				unsigned int RefRpsIdx = stRpsIdx - (delta_idx_minus1 + 1);
				unsigned int NumDeltaPocs = sps->short_term_ref_pic_sets[RefRpsIdx].num_delta_pocs;
				for (unsigned j=0; j<=NumDeltaPocs; j++) {
					unsigned int used_by_curr_pic_flag /*[j]*/ = bits.Read();
					unsigned int use_delta_flag /*[j]*/ = 1;
					if (!used_by_curr_pic_flag /*[j]*/) {
						use_delta_flag /*[j]*/ = bits.Read();
					}
					if (used_by_curr_pic_flag /*[j]*/ || use_delta_flag /*[j]*/) {
						rps->num_delta_pocs++;
					}
				}
			} else {
				rps->num_negative_pics = bits.ReadGolomb();
				rps->num_positive_pics = bits.ReadGolomb();
				A (rps->num_negative_pics <= 16 && rps->num_positive_pics <= 16);
				rps->num_delta_pocs = rps->num_negative_pics + rps->num_positive_pics;
				for (unsigned int i=0; i<rps->num_negative_pics; i++) {
					rps->delta_poc_s0_minus1[i] = bits.ReadGolomb();
					rps->used_by_curr_pic_s0_flag[i] = bits.Read();
				}
				for (unsigned i=0; i<rps->num_positive_pics; i++) {
					rps->delta_poc_s1_minus1[i] = bits.ReadGolomb();
					rps->used_by_curr_pic_s1_flag[i] = bits.Read();
				}
			}
		}
		long_term_ref_pics_present_flag = bits.Read();
		if (long_term_ref_pics_present_flag) {
			num_long_term_ref_pics_sps = bits.ReadGolomb();
			for (unsigned int i=0; i<num_long_term_ref_pics_sps; i++) {
				/* lt_ref_pic_poc_lsb_sps[i] = */ bits.Read( log2_max_pic_order_cnt_lsb_minus4 + 4);
				/* used_by_curr_pic_lt_sps_flag[i] = */ bits.Read();
			}
		} else {
			num_long_term_ref_pics_sps = 0;
		}
		sps_temporal_mvp_enabled_flag  = bits.Read();
		strong_intra_smoothing_enabled_flag = bits.Read();
		vui_parameters_present_flag = bits.Read();
		if (vui_parameters_present_flag) {
		  vui_parameters.Parse(bits, vui_parameters.transfer_characteristics);
		}
	}
};

struct
HevcPictureParameterSet {
    unsigned int   pps_pic_parameter_set_id;
    unsigned int   pps_seq_parameter_set_id;
    unsigned int   dependent_slice_segments_enabled_flag;
    unsigned int   output_flag_present_flag;
    unsigned int   num_extra_slice_header_bits;
    unsigned int   sign_data_hiding_enabled_flag;
    unsigned int   cabac_init_present_flag;
    unsigned int   num_ref_idx_l0_default_active_minus1;
    unsigned int   num_ref_idx_l1_default_active_minus1;
    int            init_qp_minus26;
    unsigned int   constrained_intra_pred_flag;
    unsigned int   transform_skip_enabled_flag;
    unsigned int   cu_qp_delta_enabled_flag;
    unsigned int   diff_cu_qp_delta_depth;
    int            pps_cb_qp_offset;
    int            pps_cr_qp_offset;
    unsigned int   pps_slice_chroma_qp_offsets_present_flag;
    unsigned int   weighted_pred_flag;
    unsigned int   weighted_bipred_flag;
    unsigned int   transquant_bypass_enabled_flag;
    unsigned int   tiles_enabled_flag;
    unsigned int   entropy_coding_sync_enabled_flag;
    unsigned int   num_tile_columns_minus1;
    unsigned int   num_tile_rows_minus1;
    unsigned int   uniform_spacing_flag;
    unsigned int   loop_filter_across_tiles_enabled_flag;
    unsigned int   pps_loop_filter_across_slices_enabled_flag;
    unsigned int   deblocking_filter_control_present_flag;
    unsigned int   deblocking_filter_override_enabled_flag;
    unsigned int   pps_deblocking_filter_disabled_flag;
    int            pps_beta_offset_div2;
    int            pps_tc_offset_div2;
    unsigned int   pps_scaling_list_data_present_flag;
    unsigned int   lists_modification_present_flag;
    unsigned int   log2_parallel_merge_level_minus2;
    unsigned int   slice_segment_header_extension_present_flag;

	void
	Parse(const unsigned char* data, unsigned int data_size)
	{
		UI1 $[ data_size ];
		Unescape( data, data_size, $ );
		BitReader bits( $ );

		bits.Skip(16); // NAL Unit Header

		pps_pic_parameter_set_id = bits.ReadGolomb();
		A(pps_pic_parameter_set_id <= 63);	// AP4_HEVC_PPS_MAX_ID) {
		pps_seq_parameter_set_id = bits.ReadGolomb();
		A(pps_seq_parameter_set_id <= 15);	// AP4_HEVC_SPS_MAX_ID) {
		dependent_slice_segments_enabled_flag    = bits.Read() ;
		output_flag_present_flag                 = bits.Read() ;
		num_extra_slice_header_bits              = bits.Read(  3);
		sign_data_hiding_enabled_flag            = bits.Read() ;
		cabac_init_present_flag                  = bits.Read() ;
		num_ref_idx_l0_default_active_minus1     = bits.ReadGolomb();
		num_ref_idx_l1_default_active_minus1     = bits.ReadGolomb();
		init_qp_minus26                          = bits.ReadSignedGolomb();
		constrained_intra_pred_flag              = bits.Read() ;
		transform_skip_enabled_flag              = bits.Read() ;
		cu_qp_delta_enabled_flag                 = bits.Read() ;
		if (cu_qp_delta_enabled_flag) {
			diff_cu_qp_delta_depth = bits.ReadGolomb();
		}
		pps_cb_qp_offset                         = bits.ReadSignedGolomb();
		pps_cr_qp_offset                         = bits.ReadSignedGolomb();
		pps_slice_chroma_qp_offsets_present_flag = bits.Read() ;
		weighted_pred_flag                       = bits.Read() ;
		weighted_bipred_flag                     = bits.Read() ;
		transquant_bypass_enabled_flag           = bits.Read() ;
		tiles_enabled_flag                       = bits.Read() ;
		entropy_coding_sync_enabled_flag         = bits.Read() ;
		if (tiles_enabled_flag) {
			num_tile_columns_minus1 = bits.ReadGolomb();
			num_tile_rows_minus1    = bits.ReadGolomb();
			uniform_spacing_flag    = bits.Read() ;
			if (!uniform_spacing_flag) {
				for (unsigned int i=0; i<num_tile_columns_minus1; i++) {
					bits.ReadGolomb(); // column_width_minus1[i]
				}
				for (unsigned int i = 0; i < num_tile_rows_minus1; i++) {
					bits.ReadGolomb(); // row_height_minus1[i]
				}
			}
			loop_filter_across_tiles_enabled_flag = bits.Read() ;
		}
		pps_loop_filter_across_slices_enabled_flag = bits.Read() ;
		deblocking_filter_control_present_flag     = bits.Read() ;
		if (deblocking_filter_control_present_flag) {
			deblocking_filter_override_enabled_flag = bits.Read() ;
			pps_deblocking_filter_disabled_flag     = bits.Read() ;
			if (!pps_deblocking_filter_disabled_flag) {
				pps_beta_offset_div2 = bits.ReadSignedGolomb();
				pps_tc_offset_div2   = bits.ReadSignedGolomb();
			}
		}
		pps_scaling_list_data_present_flag = bits.Read() ;
		if (pps_scaling_list_data_present_flag) {
			scaling_list_data(bits);
		}
		lists_modification_present_flag = bits.Read() ;
		log2_parallel_merge_level_minus2 = bits.ReadGolomb();
		slice_segment_header_extension_present_flag = bits.Read() ;
    }
};

inline unsigned int
BitsNeeded(unsigned int num_values)
{
    unsigned int bits_needed = 1;
    while (num_values > (unsigned int)(1 << bits_needed)) {
        ++bits_needed;
    }
    
    return bits_needed;
}

inline void
parse_st_ref_pic_set(
	HevcShortTermRefPicSet*         rps
,	const HevcSequenceParameterSet*	sps
,	unsigned int                    stRpsIdx
,	unsigned int                    num_short_term_ref_pic_sets
,	BitReader&						bits
) {
    memset( rps, 0, sizeof(*rps) );
    
    unsigned int inter_ref_pic_set_prediction_flag = 0;
    if (stRpsIdx != 0) {
        inter_ref_pic_set_prediction_flag = bits.Read();
    }
    if (inter_ref_pic_set_prediction_flag) {
        unsigned int delta_idx_minus1 = 0;
        if (stRpsIdx == num_short_term_ref_pic_sets) {
            delta_idx_minus1 = bits.ReadGolomb();
        }
        /* delta_rps_sign = */ bits.Read();
        /* abs_delta_rps_minus1 = */ bits.ReadGolomb();
        if (delta_idx_minus1+1 > stRpsIdx) __builtin_trap(); // should not happen
        unsigned int RefRpsIdx = stRpsIdx - (delta_idx_minus1 + 1);
        unsigned int NumDeltaPocs = sps->short_term_ref_pic_sets[RefRpsIdx].num_delta_pocs;
        for (unsigned j=0; j<=NumDeltaPocs; j++) {
            unsigned int used_by_curr_pic_flag /*[j]*/ = bits.Read();
            unsigned int use_delta_flag /*[j]*/ = 1;
            if (!used_by_curr_pic_flag /*[j]*/) {
                use_delta_flag /*[j]*/ = bits.Read();
            }
            if (used_by_curr_pic_flag /*[j]*/ || use_delta_flag /*[j]*/) {
                rps->num_delta_pocs++;
            }
        }
    } else {
        rps->num_negative_pics = bits.ReadGolomb();
        rps->num_positive_pics = bits.ReadGolomb();
        if (rps->num_negative_pics > 16 || rps->num_positive_pics > 16) {
            __builtin_trap();
        }
        rps->num_delta_pocs = rps->num_negative_pics + rps->num_positive_pics;
        for (unsigned int i=0; i<rps->num_negative_pics; i++) {
            rps->delta_poc_s0_minus1[i] = bits.ReadGolomb();
            rps->used_by_curr_pic_s0_flag[i] = bits.Read();
        }
        for (unsigned i=0; i<rps->num_positive_pics; i++) {
            rps->delta_poc_s1_minus1[i] = bits.ReadGolomb();
            rps->used_by_curr_pic_s1_flag[i] = bits.Read();
        }
    }
}

inline	UI8
HEVCNaluHeaderSize( UI1* ptr, int nal_unit_type, HevcSequenceParameterSet* SPSs, HevcPictureParameterSet* PPSs ) {

    unsigned int first_slice_segment_in_pic_flag;
    unsigned int no_output_of_prior_pics_flag;
    unsigned int slice_pic_parameter_set_id;
    unsigned int dependent_slice_segment_flag = 0;
    unsigned int slice_segment_address;
    unsigned int slice_type;
    unsigned int pic_output_flag;
    unsigned int colour_plane_id;
    unsigned int slice_pic_order_cnt_lsb;
    unsigned int short_term_ref_pic_set_sps_flag;
    unsigned int short_term_ref_pic_set_idx;
    unsigned int num_entry_point_offsets;
    unsigned int offset_len_minus1;
    unsigned int num_long_term_sps = 0;
    unsigned int num_long_term_pics = 0;

    HevcShortTermRefPicSet short_term_ref_pic_set;
    //	32	AP4_HEVC_MAX_LT_REFS
    unsigned int used_by_curr_pic_lt_flag[ 32 ];

	BitReader bits( ptr );
	#define	AP4_HEVC_SLICE_TYPE_B			0
	#define	AP4_HEVC_SLICE_TYPE_P			1
	#define	AP4_HEVC_SLICE_TYPE_I			2
	#define	AP4_HEVC_NALU_TYPE_BLA_W_LP		16
	#define	AP4_HEVC_NALU_TYPE_RSV_IRAP_VCL	23
	#define	AP4_HEVC_PPS_MAX_ID				63
	#define	AP4_HEVC_NALU_TYPE_IDR_W_RADL	19
	#define	AP4_HEVC_NALU_TYPE_IDR_N_LP		20
	#define	AP4_HEVC_MAX_LT_REFS			32

    first_slice_segment_in_pic_flag = bits.Read();
    if (nal_unit_type >= AP4_HEVC_NALU_TYPE_BLA_W_LP && nal_unit_type <= AP4_HEVC_NALU_TYPE_RSV_IRAP_VCL) {
        no_output_of_prior_pics_flag = bits.Read();
    }
    slice_pic_parameter_set_id = bits.ReadGolomb();
    A (slice_pic_parameter_set_id <= AP4_HEVC_PPS_MAX_ID);
    HevcPictureParameterSet* pps = &PPSs[slice_pic_parameter_set_id];
    A(pps);
    HevcSequenceParameterSet* sps = &SPSs[pps->pps_seq_parameter_set_id];
    A(sps);

    // PicSizeInCtbsY = PicWidthInCtbsY * PicHeightInCtbsY
    // PicWidthInCtbsY = Ceil( pic_width_in_luma_samples / CtbSizeY )
    // PicHeightInCtbsY = Ceil( pic_height_in_luma_samples / CtbSizeY )
    // CtbSizeY = 1 << CtbLog2SizeY
    // CtbLog2SizeY = MinCbLog2SizeY + log2_diff_max_min_luma_coding_block_size
    // MinCbLog2SizeY = log2_min_luma_coding_block_size_minus3 + 3
    unsigned int MinCbLog2SizeY   = sps->log2_min_luma_coding_block_size_minus3 + 3;
    unsigned int CtbLog2SizeY     = MinCbLog2SizeY + sps->log2_diff_max_min_luma_coding_block_size;
    unsigned int CtbSizeY         = 1 << CtbLog2SizeY;
    unsigned int PicWidthInCtbsY  = (sps->pic_width_in_luma_samples + CtbSizeY - 1) / CtbSizeY;
    unsigned int PicHeightInCtbsY = (sps->pic_height_in_luma_samples + CtbSizeY - 1) / CtbSizeY;
    unsigned int PicSizeInCtbsY   = PicWidthInCtbsY * PicHeightInCtbsY;

    if (!first_slice_segment_in_pic_flag) {
        if (pps->dependent_slice_segments_enabled_flag) {
            dependent_slice_segment_flag = bits.Read();
        }
        
        unsigned int bits_needed = BitsNeeded(PicSizeInCtbsY);
        if (bits_needed) {
            slice_segment_address = bits.Read( bits_needed);
        }
    }
    
    if (!dependent_slice_segment_flag) {
        if (pps->num_extra_slice_header_bits) {
            bits.Read( pps->num_extra_slice_header_bits); // slice_reserved_flag[...]
        }
    
        slice_type = bits.ReadGolomb();
        if (slice_type != AP4_HEVC_SLICE_TYPE_B && slice_type != AP4_HEVC_SLICE_TYPE_P && slice_type != AP4_HEVC_SLICE_TYPE_I) {
			__builtin_trap();
        }
        if (pps->output_flag_present_flag) {
            pic_output_flag = bits.Read();
        }
        if (sps->separate_colour_plane_flag) {
            colour_plane_id = bits.Read( 2);
        }
        unsigned int slice_sao_luma_flag = 0;
        unsigned int slice_sao_chroma_flag = 0;
        unsigned int slice_deblocking_filter_disabled_flag = 0;
        unsigned int slice_temporal_mvp_enabled_flag = 0;
        const HevcShortTermRefPicSet* rps = NULL;
        if (nal_unit_type != AP4_HEVC_NALU_TYPE_IDR_W_RADL && nal_unit_type != AP4_HEVC_NALU_TYPE_IDR_N_LP) {
            slice_pic_order_cnt_lsb = bits.Read( sps->log2_max_pic_order_cnt_lsb_minus4+4);
            short_term_ref_pic_set_sps_flag = bits.Read();
            if (!short_term_ref_pic_set_sps_flag) {
                parse_st_ref_pic_set(&short_term_ref_pic_set,
                                                         sps,
                                                         sps->num_short_term_ref_pic_sets,
                                                         sps->num_short_term_ref_pic_sets,
                                                         bits);
                rps = &short_term_ref_pic_set;
            } else if (sps->num_short_term_ref_pic_sets > 1) {
                short_term_ref_pic_set_idx = bits.Read( BitsNeeded(sps->num_short_term_ref_pic_sets));
                rps = &sps->short_term_ref_pic_sets[short_term_ref_pic_set_idx];
            }
            
            if (sps->long_term_ref_pics_present_flag) {
                if (sps->num_long_term_ref_pics_sps > 0) {
                    num_long_term_sps = bits.ReadGolomb();
                }
                num_long_term_pics = bits.ReadGolomb();
                
                if (num_long_term_sps > sps->num_long_term_ref_pics_sps) {
					__builtin_trap();
                }
                if (num_long_term_sps + num_long_term_pics > AP4_HEVC_MAX_LT_REFS) {
					__builtin_trap();
                }
                for (unsigned int i=0; i<num_long_term_sps + num_long_term_pics; i++) {
                    if (i < num_long_term_sps) {
                        if (sps->num_long_term_ref_pics_sps > 1) {
                            /* lt_idx_sps[i] = */ bits.Read( BitsNeeded(sps->num_long_term_ref_pics_sps));
                        }
                    } else {
                        /* poc_lsb_lt[i] = */ bits.Read( sps->log2_max_pic_order_cnt_lsb_minus4+4);
                        used_by_curr_pic_lt_flag[i] = bits.Read();
                    }
                    unsigned int delta_poc_msb_present_flag /*[i]*/ = bits.Read();
                    if (delta_poc_msb_present_flag /*[i]*/) {
                        /* delta_poc_msb_cycle_lt[i] = */ bits.ReadGolomb();
                    }
                }
            }
            if (sps->sps_temporal_mvp_enabled_flag) {
                slice_temporal_mvp_enabled_flag = bits.Read();
            }
        }
        if (sps->sample_adaptive_offset_enabled_flag) {
            slice_sao_luma_flag   = bits.Read();
            unsigned int ChromaArrayType = sps->separate_colour_plane_flag ? 0 : sps->chroma_format_idc;
            if (ChromaArrayType) {
                slice_sao_chroma_flag = bits.Read();
            }
        }
        if (slice_type == AP4_HEVC_SLICE_TYPE_P || slice_type == AP4_HEVC_SLICE_TYPE_B) {
            unsigned int num_ref_idx_l0_active_minus1 = pps->num_ref_idx_l0_default_active_minus1;
            unsigned int num_ref_idx_l1_active_minus1 = pps->num_ref_idx_l1_default_active_minus1;
            unsigned int num_ref_idx_active_override_flag = bits.Read();
            if (num_ref_idx_active_override_flag) {
                num_ref_idx_l0_active_minus1 = bits.ReadGolomb();
                if (slice_type == AP4_HEVC_SLICE_TYPE_B) {
                    num_ref_idx_l1_active_minus1 = bits.ReadGolomb();
                }
            }
            if (num_ref_idx_l0_active_minus1 > 14 || num_ref_idx_l1_active_minus1 > 14) {
				__builtin_trap();
            }
//			unsigned int nptc = NumPicTotalCurr(rps, this);
			unsigned int nptc = 0;
			if (rps) {
				for (unsigned int i=0; i<rps->num_negative_pics; i++) {
					if (rps->used_by_curr_pic_s0_flag[i]) {
						++nptc;
					}
				}
				for (unsigned int i=0; i<rps->num_positive_pics; i++) {
					if (rps->used_by_curr_pic_s1_flag[i]) {
						++nptc;
					}
				}
			}
			for (unsigned int i=0; i<num_long_term_sps + num_long_term_pics; i++) {
				if (used_by_curr_pic_lt_flag[i]) {
					++nptc;
				}
			}
            if (pps->lists_modification_present_flag && nptc > 1) {
                // ref_pic_lists_modification
                unsigned int ref_pic_list_modification_flag_l0 = bits.Read();
                if (ref_pic_list_modification_flag_l0) {
                    for (unsigned int i=0; i<=num_ref_idx_l0_active_minus1; i++) {
                        /* list_entry_l0[i]; */ bits.Read( BitsNeeded(nptc));
                    }
                }
                if (slice_type == AP4_HEVC_SLICE_TYPE_B) {
                    unsigned int ref_pic_list_modification_flag_l1 = bits.Read();
                    if (ref_pic_list_modification_flag_l1) {
                        for (unsigned int i=0; i<=num_ref_idx_l1_active_minus1; i++) {
                            /* list_entry_l1[i]; */ bits.Read( BitsNeeded(nptc));
                        }
                    }
                }
            }
            if (slice_type == AP4_HEVC_SLICE_TYPE_B) {
                /* mvd_l1_zero_flag = */ bits.Read();
            }
            if (pps->cabac_init_present_flag) {
                /* cabac_init_flag = */ bits.Read();
            }
            if (slice_temporal_mvp_enabled_flag) {
                unsigned int collocated_from_l0_flag = 1;
                if (slice_type == AP4_HEVC_SLICE_TYPE_B) {
                    collocated_from_l0_flag = bits.Read();
                }
                if (( collocated_from_l0_flag && num_ref_idx_l0_active_minus1 > 0) ||
                    (!collocated_from_l0_flag && num_ref_idx_l1_active_minus1 > 0)) {
                    /* collocated_ref_idx = */ bits.ReadGolomb();
                }
            }
            if ((pps->weighted_pred_flag   && slice_type == AP4_HEVC_SLICE_TYPE_P) ||
                (pps->weighted_bipred_flag && slice_type == AP4_HEVC_SLICE_TYPE_B)) {
                // +++ pred_weight_table()
                /* luma_log2_weight_denom = */ bits.ReadGolomb();
                if (sps->chroma_format_idc != 0) {
                    /* delta_chroma_log2_weight_denom = */ /* SignedGolomb( */ bits.ReadGolomb() /*)*/;
                }
                unsigned int luma_weight_l0_flag[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                for (unsigned int i=0; i<=num_ref_idx_l0_active_minus1; i++) {
                    luma_weight_l0_flag[i] = bits.Read();
                }
                unsigned int chroma_weight_l0_flag[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                if (sps->chroma_format_idc != 0) {
                    for (unsigned int i=0; i<=num_ref_idx_l0_active_minus1; i++) {
                        chroma_weight_l0_flag[i] = bits.Read();
                    }
                }
                for (unsigned int i=0; i<=num_ref_idx_l0_active_minus1; i++) {
                    if (luma_weight_l0_flag[i]) {
                        /* delta_luma_weight_l0[i] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                        /* luma_offset_l0[i] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                    }
                    if (chroma_weight_l0_flag[i]) {
                        for (unsigned int j=0; j<2; j++) {
                            /* delta_chroma_weight_l0[i][j] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                            /* delta_chroma_offset_l0[i][j] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                        }
                    }
                }
                if (slice_type == AP4_HEVC_SLICE_TYPE_B) {
                    unsigned int luma_weight_l1_flag[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    for (unsigned int i=0; i<=num_ref_idx_l1_active_minus1; i++) {
                        luma_weight_l1_flag[i] = bits.Read();
                    }
                    unsigned int chroma_weight_l1_flag[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    if (sps->chroma_format_idc != 0) {
                        for (unsigned int i=0; i<=num_ref_idx_l1_active_minus1; i++) {
                            chroma_weight_l1_flag[i] = bits.Read();
                        }
                    }
                    for (unsigned int i=0; i<=num_ref_idx_l1_active_minus1; i++) {
                        if (luma_weight_l1_flag[i]) {
                            /* delta_luma_weight_l1[i] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                            /* luma_offset_l1[i] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                        }
                        if (chroma_weight_l1_flag[i]) {
                            for (unsigned int j=0; j<2; j++) {
                                /* delta_chroma_weight_l1[i][j] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                                /* delta_chroma_offset_l1[i][j] = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                            }
                        }
                    }
                }
                // --- pred_weight_table()
            }
            /* five_minus_max_num_merge_cand = */ bits.ReadGolomb();
        }
        /* slice_qp_delta = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
        if (pps->pps_slice_chroma_qp_offsets_present_flag) {
            /* slice_cb_qp_offset = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
            /* slice_cr_qp_offset = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
        }
        unsigned int deblocking_filter_override_flag = 0;
        if (pps->deblocking_filter_override_enabled_flag) {
            deblocking_filter_override_flag = bits.Read();
        }
        if (deblocking_filter_override_flag) {
            slice_deblocking_filter_disabled_flag = bits.Read();
            if (!slice_deblocking_filter_disabled_flag) {
                /* slice_beta_offset_div2 = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
                /* slice_tc_offset_div2   = */ /*SignedGolomb(*/ bits.ReadGolomb() /*)*/;
            }
        }
        if (pps->pps_loop_filter_across_slices_enabled_flag &&
            (slice_sao_luma_flag || slice_sao_chroma_flag || !slice_deblocking_filter_disabled_flag)) {
            /* slice_loop_filter_across_slices_enabled_flag = */ bits.Read();
        }
    }

    if (pps->tiles_enabled_flag || pps->entropy_coding_sync_enabled_flag) {
        num_entry_point_offsets = bits.ReadGolomb();
        if (num_entry_point_offsets > 0 ) {
            offset_len_minus1 = bits.ReadGolomb();
            if (offset_len_minus1 > 31) {
				__builtin_trap();
            }
            for (unsigned int i=0; i<num_entry_point_offsets; i++) {
                bits.Read( offset_len_minus1+1);
            }
        }
    }

    if (pps->slice_segment_header_extension_present_flag) {
        unsigned int slice_segment_header_extension_length = bits.ReadGolomb();
        for (unsigned int i=0; i<slice_segment_header_extension_length; i++) {
            bits.Read( 8); // slice_segment_header_extension_data_byte[i]
        }
    }

    // byte_alignment()
	bits.Read(); // alignment_bit_equal_to_one
	auto bits_read = bits._;
	if (bits_read % 8) {
		bits.Read( 8-(bits_read%8));
	}
 
    return bits._ / 8;
}


inline vector< vector< UI1 > >
NALs_TS( const vector< UI1 >& _ ) {
	vector< vector< UI1 > > $;
	auto s = &_[ 0 ];
	auto e = s + _.size();
	auto zeroCount = 0;
	auto nalStart = (const UI1*)0;
	while ( s < e ) {
		if ( *s == 1 && zeroCount > 1 ) {
			if ( nalStart ) $.emplace_back( vector< UI1 >( nalStart, s - zeroCount ) );
			nalStart = s + 1;
		}
		zeroCount = *s ? 0 : zeroCount + 1;
		s++;
	}
	if ( nalStart ) $.emplace_back( vector< UI1 >( nalStart, s - zeroCount ) );
	return $;
}

inline vector< vector< UI1 > >
NALs_MDAT( const vector< UI1 >& _ ) {
	auto nalus = vector< vector< UI1 > >();
	auto $ = &_[ 0 ];
	auto e = $ + _.size();
	while ( $ < e ) {
		auto size = Size4( $ );
		if ( size == 0 ) break;
		$ += 4;
		nalus.emplace_back( vector< UI1 >( $, $ + size ) );
		$ += size;
	}
	return nalus;
}

inline vector< UI1 >
TS_NALs( const vector< vector< UI1 > >& _ ) {

	static vector< UI1 > startCode{ 0, 0, 0, 1 };
 
	vector< UI1 > $;
	for ( auto& nal: _ ) {
		$.insert( $.end(), startCode.begin(), startCode.end() );
		$.insert( $.end(), nal.begin(), nal.end() );
	};
	return $;
}

inline vector< UI1 >
MDAT_NALs( const vector< vector< UI1 > >& _ ) {
	vector< UI1 > $;
	for ( auto& nal: _ ) {
		auto size = Swap4( (UI4)nal.size() );
		$.insert( $.end(), (UI1*)&size, (UI1*)&size + 4 );
		$.insert( $.end(), nal.begin(), nal.end() );
	};
	return $;
}

template < typename T > void
Concat( vector< T >& $, const vector< T >& _ ) {
	$.insert( $.end(), _.begin(), _.end() );
}
template < typename T > void
Concat( vector< T >& $, T* _, UI8 size ) {
	$.insert( $.end(), _, _ + size );
}

inline const char*
HEVCNaluTypeName( unsigned int nalu_type ) {
	switch ( nalu_type ) {
	case  0: return "TRAIL_N - Coded slice segment of a non-TSA, non-STSA trailing picture";
	case  1: return "TRAIL_R - Coded slice segment of a non-TSA, non-STSA trailing picture";
	case  2: return "TSA_N - Coded slice segment of a TSA picture";
	case  3: return "TSA_R - Coded slice segment of a TSA picture";
	case  4: return "STSA_N - Coded slice segment of an STSA picture";
	case  5: return "STSA_R - Coded slice segment of an STSA picture";
	case  6: return "RADL_N - Coded slice segment of a RADL picture";
	case  7: return "RADL_R - Coded slice segment of a RADL picture";
	case  8: return "RASL_N - Coded slice segment of a RASL picture";
	case  9: return "RASL_R - Coded slice segment of a RASL picture";
	case 10: return "RSV_VCL_N10 - Reserved non-IRAP sub-layer non-reference";
	case 12: return "RSV_VCL_N12 - Reserved non-IRAP sub-layer non-reference";
	case 14: return "RSV_VCL_N14 - Reserved non-IRAP sub-layer non-reference";
	case 11: return "RSV_VCL_R11 - Reserved non-IRAP sub-layer reference";
	case 13: return "RSV_VCL_R13 - Reserved non-IRAP sub-layer reference";
	case 15: return "RSV_VCL_R15 - Reserved non-IRAP sub-layer reference";
	case 16: return "BLA_W_LP - Coded slice segment of a BLA picture";
	case 17: return "BLA_W_RADL - Coded slice segment of a BLA picture";
	case 18: return "BLA_N_LP - Coded slice segment of a BLA picture";
	case 19: return "IDR_W_RADL - Coded slice segment of an IDR picture";
	case 20: return "IDR_N_LP - Coded slice segment of an IDR picture";
	case 21: return "CRA_NUT - Coded slice segment of a CRA picture";
	case 22: return "RSV_IRAP_VCL22 - Reserved IRAP";
	case 23: return "RSV_IRAP_VCL23 - Reserved IRAP";
	case 24: return "RSV_VCL24 - Reserved non-IRAP";
	case 25: return "RSV_VCL25 - Reserved non-IRAP";
	case 26: return "RSV_VCL26 - Reserved non-IRAP";
	case 27: return "RSV_VCL27 - Reserved non-IRAP";
	case 28: return "RSV_VCL28 - Reserved non-IRAP";
	case 29: return "RSV_VCL29 - Reserved non-IRAP";
	case 30: return "RSV_VCL30 - Reserved non-IRAP";
	case 31: return "RSV_VCL31 - Reserved non-IRAP";
	case 32: return "VPS_NUT - Video parameter set";
	case 33: return "SPS_NUT - Sequence parameter set";
	case 34: return "PPS_NUT - Picture parameter set";
	case 35: return "AUD_NUT - Access unit delimiter";
	case 36: return "EOS_NUT - End of sequence";
	case 37: return "EOB_NUT - End of bitstream";
	case 38: return "FD_NUT - Filler data";
	case 39: return "PREFIX_SEI_NUT - Supplemental enhancement information";
	case 40: return "SUFFIX_SEI_NUT - Supplemental enhancement information";
	case 62: return "Dolby Vision RPU NAL units";
	case 63: return "Dolby Vision EL NAL units";
	default: return NULL;
	}
}

inline void
PrefixSEIUnit( const vector< UI1 >& _, ostream& s ) {
	auto $ = &_[ 0 ];
	unsigned int payloadType = 0;
	while ( *$++ == 0xff ) payloadType += 255;
	payloadType += *( $ - 1 );
	unsigned int payloadSize = 0;
	while ( *$++ == 0xff ) payloadSize += 255;
	payloadSize += *( $ - 1 );
//	printf( "payload: %04d %04d ", payloadType, payloadSize );
	s << '\t';
	switch ( payloadType ) {
	case 0:
		s << "buffering_period";
		break;
	case 1:
		s << "pic_timing";
		break;
	case 5:
		//	16 byte uuid
		s << "UUID:" << EncodeHex( $, 16 ) << endl;
		$ += 16;
		s << string( $, &_[ 0 ] + _.size() ) << endl;
		break;
	case 129:
		s << "active_parameter_sets";
		break;
	default:
		s << endl;
		break;
	}
}

inline void
DumpHEVCNAL( const vector< UI1 >& nal, ostream& s ) {
	static auto counter = 0;
	auto nal_unit_type	= ( nal[ 0 ] >> 1 ) & 0x3F;
//	auto nuh_layer_id	 = ( ( ( nal[ 0 ] & 1 ) << 5 ) | ( nal[ 1 ] >> 3 ) );
	auto nuh_temporal_id	= nal[ 1 ] & 0x7;
	A( nuh_temporal_id );
	nuh_temporal_id--;
	auto nal_unit_type_name = HEVCNaluTypeName( nal_unit_type );
	if ( nal_unit_type_name == 0 ) nal_unit_type_name = "UNKNOWN";
	s << counter++ << ':' << nal.size() << ':' << nal_unit_type_name;
	auto $ = &nal[ 0 ];
	switch ( nal_unit_type ) {
	case 39:
		PrefixSEIUnit( vector< UI1 >( $ + 2, $ + nal.size() ), s );
		break;
	}
	s << endl;
}
inline void
DumpHEVCNALs( const vector< vector< UI1 > >& nals, ostream& s = cerr ) {
	for ( auto& nal: nals ) DumpHEVCNAL( nal, s );
	s << "--------" << endl;
}
