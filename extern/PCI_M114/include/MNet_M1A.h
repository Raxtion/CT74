#ifndef __MNET_M1A_H__
#define __MNET_M1A_H__

#include "Type_def.h"


#ifdef __cplusplus
extern "C" {
#endif
//
// Initial
I16 PASCAL _mnet_m1a_initial( U16 RingNo, U16 SlaveIP );
//
// Pulse I/O Configuration
I16 PASCAL _mnet_m1a_set_pls_outmode( U16 RingNo, U16 SlaveIP, U16 pls_outmode );
I16 PASCAL _mnet_m1a_set_pls_iptmode( U16 RingNo, U16 SlaveIP, U16 pls_iptmode, U16 pls_iptdir );
I16 PASCAL _mnet_m1a_set_feedback_src( U16 RingNo, U16 SlaveIP, U16 FbkSrc );
I16 PASCAL _mnet_m1a_set_abs_reference( U16 RingNo, U16 SlaveIP, U16 Ref );
I16 PASCAL _mnet_m1a_set_direction_change_delay( U16 RingNo, U16 SlaveIP, U8 Enabled );
//
// Interface I/O Configuration
I16 PASCAL _mnet_m1a_set_alm( U16 RingNo, U16 SlaveIP, U16 alm_logic, U16 alm_mode );
I16 PASCAL _mnet_m1a_set_inp( U16 RingNo, U16 SlaveIP, U16 inp_enable, U16 inp_logic );
I16 PASCAL _mnet_m1a_set_sd( U16 RingNo, U16 SlaveIP, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode );
I16 PASCAL _mnet_m1a_set_erc( U16 RingNo, U16 SlaveIP, U16 erc_logic, U16 erc_on_time, U16 erc_off_time );
I16 PASCAL _mnet_m1a_set_erc_on( U16 RingNo, U16 SlaveIP, I16 on_off );
I16 PASCAL _mnet_m1a_set_auto_erc( U16 RingNo, U16 SlaveIP, I16 Enable );
I16 PASCAL _mnet_m1a_set_svon( U16 RingNo, U16 SlaveIP, U16 on_off );
I16 PASCAL _mnet_m1a_set_ralm( U16 RingNo, U16 SlaveIP, U16 on_off );
I16 PASCAL _mnet_m1a_set_ltc_logic( U16 RingNo, U16 SlaveIP,  U16 ltc_logic );
I16 PASCAL _mnet_m1a_set_mechanical_input_filter( U16 RingNo, U16 SlaveIP, U16 on_off );
I16 PASCAL _mnet_m1a_set_backlash( U16 RingNo, U16 SlaveIP, U16 Value, U16 Enable, U16 CntSrc );
I16 PASCAL _mnet_m1a_dio_output( U16 RingNo, U16 SlaveIP, U16 DoNo, U16 on_off );
I16 PASCAL _mnet_m1a_dio_input( U16 RingNo, U16 SlaveIP, U16 DiNo );
I16 PASCAL _mnet_m1a_get_dio( U16 RingNo, U16 SlaveIP, U8 *Val );
I16 PASCAL _mnet_m1a_get_io_status( U16 RingNo, U16 SlaveIP, U32 *IO_status );
//
// Counter Operating
I16 PASCAL _mnet_m1a_get_command( U16 RingNo, U16 SlaveIP, I32 *Cmd );
I16 PASCAL _mnet_m1a_set_command( U16 RingNo, U16 SlaveIP, I32 Cmd );
I16 PASCAL _mnet_m1a_reset_command( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_get_position( U16 RingNo, U16 SlaveIP, I32 *Pos );
I16 PASCAL _mnet_m1a_set_position( U16 RingNo, U16 SlaveIP, I32 Pos );
I16 PASCAL _mnet_m1a_reset_position( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_get_error_counter( U16 RingNo, U16 SlaveIP, I32 *ErrCnt );
I16 PASCAL _mnet_m1a_set_error_counter( U16 RingNo, U16 SlaveIP, I32 ErrCnt );
I16 PASCAL _mnet_m1a_reset_error_counter( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_enable_command_ring_counter(U16 RingNo, U16 SlaveIP, U32 RingCounter);
I16 PASCAL _mnet_m1a_disable_command_ring_counter(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_m1a_enable_position_ring_counter(U16 RingNo, U16 SlaveIP, U32 RingCounter);
I16 PASCAL _mnet_m1a_disable_position_ring_counter(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_m1a_get_current_speed( U16 RingNo, U16 SlaveIP, U32 *Speed );
//
// Motion
I16 PASCAL _mnet_m1a_set_tmove_speed( U16 RingNo, U16 SlaveIP, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_set_smove_speed( U16 RingNo, U16 SlaveIP, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_set_smove_speed_ex( U16 RingNo, U16 SlaveIP, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec, U32 SVacc, U32 SVdec );
I16 PASCAL _mnet_m1a_start_r_move( U16 RingNo, U16 SlaveIP, I32 Dist );
I16 PASCAL _mnet_m1a_start_a_move( U16 RingNo, U16 SlaveIP, I32 Pos );
I16 PASCAL _mnet_m1a_start_tr_move( U16 RingNo, U16 SlaveIP, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_start_ta_move( U16 RingNo, U16 SlaveIP, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_start_sr_move( U16 RingNo, U16 SlaveIP, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_start_sa_move( U16 RingNo, U16 SlaveIP, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_v_move( U16 RingNo, U16 SlaveIP, U8 Dir );
I16 PASCAL _mnet_m1a_tv_move( U16 RingNo, U16 SlaveIP, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc );
I16 PASCAL _mnet_m1a_sv_move( U16 RingNo, U16 SlaveIP, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc );
I16 PASCAL _mnet_m1a_sd_stop( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_sd_stop_ex( U16 RingNo, U16 SlaveIP, F32 Tdec );
I16 PASCAL _mnet_m1a_imd_stop( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_emg_stop( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_motion_done( U16 RingNo, U16 SlaveIP, U16 *MoSt );
//
// Home
I16 PASCAL _mnet_m1a_set_home_config( U16 RingNo, U16 SlaveIP, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out );
I16 PASCAL _mnet_m1a_start_home_move( U16 RingNo, U16 SlaveIP, U8 Dir );
I16 PASCAL _mnet_m1a_start_home_search( U16 RingNo, U16 SlaveIP, U8 Dir, I32 ORGOffset );
I16 PASCAL _mnet_m1a_start_home_escape( U16 RingNo, U16 SlaveIP, U8 Dir );
I16 PASCAL _mnet_m1a_start_search_ez( U16 RingNo, U16 SlaveIP, U8 Dir, U16 EzCount );
I16 PASCAL _mnet_m1a_home_move( U16 RingNo, U16 SlaveIP, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc );
I16 PASCAL _mnet_m1a_home_search( U16 RingNo, U16 SlaveIP, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, I32 ORGOffset );
I16 PASCAL _mnet_m1a_home_escape( U16 RingNo, U16 SlaveIP, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc );
I16 PASCAL _mnet_m1a_search_ez( U16 RingNo, U16 SlaveIP, U8 Dir, U16 EzCount, U32 StrVel, U32 MaxVel, F32 Tacc );
//
// Position Compare and Latch
I16 PASCAL _mnet_m1a_set_comparator_mode( U16 RingNo, U16 SlaveIP, I16 CompNo, I16 CmpSrc, I16 CmpMethod, I16 CmpAction );
I16 PASCAL _mnet_m1a_set_comparator_data( U16 RingNo, U16 SlaveIP, I16 CompNo, I32 Pos );
I16 PASCAL _mnet_m1a_set_trigger_comparator( U16 RingNo, U16 SlaveIP, U16 CmpSrc, U16 CmpMethod );
I16 PASCAL _mnet_m1a_set_trigger_comparator_data( U16 RingNo, U16 SlaveIP,  I32 Data );
I16 PASCAL _mnet_m1a_set_auto_trigger( U16 RingNo, U16 SlaveIP, U16 CmpSrc, U16 CmpMethod, U16 Interval, U16 on_off );
I16 PASCAL _mnet_m1a_get_comparator_data( U16 RingNo, U16 SlaveIP, I16 CompNo, I32 *Pos );
I16 PASCAL _mnet_m1a_set_soft_limit( U16 RingNo, U16 SlaveIP, I32 PLimit, I32 MLimit );
I16 PASCAL _mnet_m1a_enable_soft_limit( U16 RingNo, U16 SlaveIP, U8 Action );
I16 PASCAL _mnet_m1a_disable_soft_limit( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_get_latch_data( U16 RingNo, U16 SlaveIP, I16 LatchNo, I32 *Pos );
//
// Velocity Change
I16 PASCAL _mnet_m1a_v_change( U16 RingNo, U16 SlaveIP, U32 NewVel, F32 Time );
I16 PASCAL _mnet_m1a_fix_speed_range( U16 RingNo, U16 SlaveIP, U32 MaxVel );
I16 PASCAL _mnet_m1a_unfix_speed_range( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_get_speed_range( U16 RingNo, U16 SlaveIP, U32 *MinVel, U32 *MaxVel );
//
// Position Override
I16 PASCAL _mnet_m1a_enable_pos_override( U16 RingNo, U16 SlaveIP, U16 OnOff );
I16 PASCAL _mnet_m1a_start_p_change( U16 RingNo, U16 SlaveIP, I32 NewPos );
I16 PASCAL _mnet_m1a_start_d_change( U16 RingNo, U16 SlaveIP, I32 NewDist );
//
// Pre-registers
I16 PASCAL _mnet_m1a_get_preregister_status( U16 RingNo, U16 SlaveIP, U8* Full );
I16 PASCAL _mnet_m1a_get_preregister_depth( U16 RingNo, U16 SlaveIP, U8* Depth );
I16 PASCAL _mnet_m1a_cancel_preregister( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_shift_preregister( U16 RingNo, U16 SlaveIP );
//
// Interpolation
I16 PASCAL _mnet_m1a_set_group( U16 RingNo, U16 GroupNo, U16* AxisArray, U16 AxisCount );
I16 PASCAL _mnet_m1a_group_reset_command( U16 RingNo, U16 GroupNo );
I16 PASCAL _mnet_m1a_group_reset_position( U16 RingNo, U16 GroupNo );
I16 PASCAL _mnet_m1a_group_continuous_move( U16 RingNo, U16 GroupNo, U16 Enable );
I16 PASCAL _mnet_m1a_group_tr_lineN( U16 RingNo, U16 GroupNo, I32 *DistArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_ta_lineN( U16 RingNo, U16 GroupNo, I32 *PosArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sr_lineN( U16 RingNo, U16 GroupNo, I32 *DistArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sa_lineN( U16 RingNo, U16 GroupNo, I32 *PosArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_tr_lineN_continuous( U16 RingNo, U16 GroupNo, I32 *DistArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_ta_lineN_continuous( U16 RingNo, U16 GroupNo, I32 *PosArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sr_lineN_continuous( U16 RingNo, U16 GroupNo, I32 *DistArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sa_lineN_continuous( U16 RingNo, U16 GroupNo, I32 *PosArray, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_tr_arc( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_ta_arc( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 Cx, I32 Cy, I32 Ex, I32 Ey, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sr_arc( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sa_arc( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 Cx, I32 Cy, I32 Ex, I32 Ey, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_tr_arc_continuous( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_ta_arc_continuous( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 Cx, I32 Cy, I32 Ex, I32 Ey, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sr_arc_continuous( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_sa_arc_continuous( U16 RingNo, U16 GroupNo, U16 AxIP, U16 AyIP, I32 Cx, I32 Cy, I32 Ex, I32 Ey, I16 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec );
I16 PASCAL _mnet_m1a_group_emg_stop( U16 RingNo, U16 GroupNo );
I16 PASCAL _mnet_m1a_group_imd_stop( U16 RingNo, U16 GroupNo );
I16 PASCAL _mnet_m1a_group_sd_stop( U16 RingNo, U16 GroupNo );
//
// EEPROM
I16 PASCAL _mnet_m1a_get_hardware_info( U16 RingNo, U16 SlaveIP, U32* VendorID, U16* DeviceID );
I16 PASCAL _mnet_m1a_recovery_from_EEPROM( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1a_backup_to_EEPROM( U16 RingNo, U16 SlaveIP );
//
#ifdef __cplusplus
}
#endif


#endif //__MNET_M1A_H__
