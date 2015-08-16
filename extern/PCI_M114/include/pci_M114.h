#ifndef __PCI_M114_H__
#define __PCI_M114_H__

#include "type_def.h"


// Error Table
enum {
    ERR_NoError                   =  0, 
    ERR_OpenCardFailed            =  -14801, 
    ERR_MapMemoryFailed           =  -14802, 
    ERR_CardNumberRepeated        =  -14803, 
    ERR_CardNotExist              =  -14804, 
    ERR_CardNotInitYet            =  -14805, 
    ERR_LoadLibraryFailed         =  -14806, 
    ERR_OpenMNetFailed            =  -14807, 
    ERR_InvalidCardNumber         =  -14808, 
    ERR_InvalidAxisNumber         =  -14809, 
    ERR_InvalidParameter1         =  -14810, 
    ERR_InvalidParameter2         =  -14811, 
    ERR_InvalidParameter3         =  -14812, 
    ERR_InvalidParameter4         =  -14813, 
    ERR_InvalidParameter5         =  -14814, 
    ERR_InvalidParameter6         =  -14815, 
    ERR_InvalidParameter7         =  -14816, 
    ERR_InvalidParameter8         =  -14817, 
    ERR_InvalidParameter9         =  -14818, 
    ERR_InvalidParameter10        =  -14819, 
    ERR_InvalidParameter11        =  -14820, 
    ERR_InvalidParameter12        =  -14821, 
    ERR_SlowDownPointError        =  -14822, 
    ERR_Err3PointsInput           =  -14823, 
    ERR_GetCenterFailed           =  -14824, 
    ERR_CompareBufferFull         =  -14825, 
    ERR_AxisNotStoppedYet         =  -14826, 
    ERR_ObsoleteFunction          =  -14827, 
    ERR_GetSecureIdFailed         =  -14828, 
    ERR_GenAesKeyFailed           =  -14829, 
	ERR_NotSupported              =  -14830, 
};

// Card Type
enum {
	CARD_UNKNOWN    = 0, 
	CARD_PCI_M114   = 1, 
	CARD_PCI_M114GH = 2
};


#ifndef PASCAL
#   define PASCAL __stdcall
#endif

#ifdef __cplusplus
extern "C" {
#endif


// Initialization
I16 PASCAL _m114_open(U16 *existcards);
I16 PASCAL _m114_close();
I16 PASCAL _m114_initial(U16 SwitchCardNo);
I16 PASCAL _m114_get_switch_card_num(U16 CardIndex, U16 *SwitchCardNo);
I16 PASCAL _m114_check_switch_card_num(U16 SwitchCardNo, U8 *IsExist);
I16 PASCAL _m114_get_card_type(U16 SwitchCardNo, U8 *CardType);

// Motionnet
I16 PASCAL _m114_set_ring_config(U16 SwitchCardNo, U16 RingOfCard, U8 BaudRate);
I16 PASCAL _m114_open_mnet(U16 SwitchCardNo);
I16 PASCAL _m114_get_start_ring_num(U16 SwitchCardNo, U16 *RingNo);

// GPIO
I16 PASCAL _m114_get_axis_input(U16 SwitchCardNo, U16 AxisNo, U16 *OnOff);
I16 PASCAL _m114_get_axis_output(U16 SwitchCardNo, U16 AxisNo, U16 *OnOff);
I16 PASCAL _m114_set_axis_output(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
I16 PASCAL _m114_toggle_axis_output(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_start_axis_output(U16 SwitchCardNo, U16 AxisNo, U16 OnTime, U16 OffTime, U16 Count);
I16 PASCAL _m114_stop_axis_output(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_get_axis_output_count(U16 SwitchCardNo, U16 AxisNo, U16 *Count);

// Security
I16 PASCAL _m114_get_secure_id(U16 SwitchCardNo, U8 SecureID[8]);
I16 PASCAL _m114_gen_aes_key(U16 SwitchCardNo, U8 SI_Key[16], U8 SecureID[8], U8 AES_Key[16]);
I16 PASCAL _m114_check_aes_key(U16 SwitchCardNo, U8 SI_Key[16], U8 AES_Key[16], U8* Validity);

// Interface I/O Configuration
I16 PASCAL _m114_set_alm(U16 SwitchCardNo, U16 AxisNo, I16 alm_logic, I16 alm_mode);
I16 PASCAL _m114_set_inp(U16 SwitchCardNo, U16 AxisNo, I16 inp_enable, I16 inp_logic);
I16 PASCAL _m114_set_erc(U16 SwitchCardNo, U16 AxisNo, I16 erc_logic, I16 erc_on_time);
I16 PASCAL _m114_set_auto_erc(U16 SwitchCardNo, U16 AxisNo, I16 enable);
I16 PASCAL _m114_set_erc_on(U16 SwitchCardNo, U16 AxisNo, I16 on_off);
I16 PASCAL _m114_set_servo(U16 SwitchCardNo, U16 AxisNo, I16 on_off);
I16 PASCAL _m114_set_ralm(U16 SwitchCardNo, U16 AxisNo, I16 on_off);  
I16 PASCAL _m114_set_ell(U16 SwitchCardNo, U16 AxisNo, I16 ell_logic, I16 ell_mode);
I16 PASCAL _m114_set_sd(U16 SwitchCardNo, U16 AxisNo, I16 enable, I16 sd_logic, I16 sd_latch, I16 sd_mode);
I16 PASCAL _m114_set_mechanical_input_filter(U16 SwitchCardNo, U16 AxisNo, U16 ON_OFF);

// Pulse I/O Configuration
I16 PASCAL _m114_set_pls_outmode(U16 SwitchCardNo, U16 AxisNo, I16 pls_outmode);
I16 PASCAL _m114_set_pls_iptmode(U16 SwitchCardNo, U16 AxisNo, I16 pls_iptmode, I16 pls_logic);
I16 PASCAL _m114_set_feedback_src(U16 SwitchCardNo, U16 AxisNo, I16 Src);
I16 PASCAL _m114_set_abs_reference(U16 SwitchCardNo, U16 AxisNo, I16 Ref);

// Status
I16 PASCAL _m114_motion_done(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_get_io_status(U16 SwitchCardNo, U16 AxisNo, U16 *io_sts);
I16 PASCAL _m114_check_error(U16 SwitchCardNo, U16 AxisNo, U32 *error);

// Stop
I16 PASCAL _m114_sd_stop(U16 SwitchCardNo, U16 AxisNo, F32 Tdec);
I16 PASCAL _m114_imd_stop(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_emg_stop(U16 SwitchCardNo, U16 AxisNo);

// Homing 
I16 PASCAL _m114_set_home_config(U16 SwitchCardNo, U16 AxisNo, I16 home_mode, I16 org_logic, I16 ez_logic, I16 ez_count, I16 erc_out);
I16 PASCAL _m114_home_move(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc);
I16 PASCAL _m114_home_search(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, I32 ORGOffset);
I16 PASCAL _m114_escape_home(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc);
I16 PASCAL _m114_search_ez(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U8 EzCount, U32 StrVel, U32 MaxVel, F32 Tacc);

// Jog
enum JogStep { Jog_Step_1 = 1, Jog_Step_10 = 10, Jog_Step_100 = 100 };
I16 PASCAL _m114_jog_switch_continue(U16 SwitchCardNo, U16 AxisNo, U8 switch_logic, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_jog_switch_step(U16 SwitchCardNo, U16 AxisNo, U8 switch_logic, I32 Step, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_jog_pulser_step(U16 SwitchCardNo, U16 AxisNo, U8 pulser_Dir, U8 Step, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_jog_stop(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_get_jogio_status(U16 SwitchCardNo, U16 AxisNo, U16 *jogio_sts);

// Velocity Move
I16 PASCAL _m114_tv_move(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc);
I16 PASCAL _m114_sv_move(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc);
I16 PASCAL _m114_fix_speed_range(U16 SwitchCardNo, U16 AxisNo, U32 MaxVel);
I16 PASCAL _m114_unfix_speed_range(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_get_current_speed(U16 SwitchCardNo, U16 AxisNo, U32 *speed);
I16 PASCAL _m114_v_change (U16 SwitchCardNo, U16 AxisNo, U32 NewVel, F32 Time);

// Single-Axis Move
I16 PASCAL _m114_start_tr_move(U16 SwitchCardNo, U16 AxisNo, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_move(U16 SwitchCardNo, U16 AxisNo, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_move(U16 SwitchCardNo, U16 AxisNo, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_move(U16 SwitchCardNo, U16 AxisNo, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_tr_move_after_axis(U16 SwitchCardNo, U16 AxisNo, U16 LeadingAxes, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_ta_move_after_axis(U16 SwitchCardNo, U16 AxisNo, U16 LeadingAxes, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_sr_move_after_axis(U16 SwitchCardNo, U16 AxisNo, U16 LeadingAxes, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_sa_move_after_axis(U16 SwitchCardNo, U16 AxisNo, U16 LeadingAxes, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_p_change(U16 SwitchCardNo, U16 AxisNo, I32 NewPos); //abs

// 2-Axis Linear Move
I16 PASCAL _m114_start_tr_move_xy(U16 SwitchCardNo, I32 DistX, I32 DistY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_move_xy(U16 SwitchCardNo, I32 PosX, I32 PosY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_move_xy(U16 SwitchCardNo, I32 DistX, I32 DistY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_move_xy(U16 SwitchCardNo, I32 PosX, I32 PosY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_tr_move_zu(U16 SwitchCardNo, I32 DistZ, I32 DistU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_move_zu(U16 SwitchCardNo, I32 PosZ, I32 PosU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_move_zu(U16 SwitchCardNo, I32 DistZ, I32 DistU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_move_zu(U16 SwitchCardNo, I32 PosZ, I32 PosU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_tr_line2(U16 SwitchCardNo, U16 *AxisArray, I32 DistX, I32 DistY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_line2(U16 SwitchCardNo, U16 *AxisArray, I32 PosX, I32 PosY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_line2(U16 SwitchCardNo, U16 *AxisArray, I32 DistX, I32 DistY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_line2(U16 SwitchCardNo, U16 *AxisArray, I32 PosX, I32 PosY, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);

// 3-Axis Linear Move
I16 PASCAL _m114_start_tr_line3(U16 SwitchCardNo, U16 *AxisArray, I32 DistX, I32 DistY, I32 DistZ, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_line3(U16 SwitchCardNo, U16 *AxisArray, I32 PosX, I32 PosY, I32 PosZ, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_line3(U16 SwitchCardNo, U16 *AxisArray, I32 DistX, I32 DistY, I32 DistZ, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_line3(U16 SwitchCardNo, U16 *AxisArray, I32 PosX, I32 PosY, I32 PosZ, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);

// 4-Axis Linear Move
I16 PASCAL _m114_start_tr_line4(U16 SwitchCardNo, I32 DistX, I32 DistY, I32 DistZ, I32 DistU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_line4(U16 SwitchCardNo, I32 PosX, I32 PosY, I32 PosZ, I32 PosU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_line4(U16 SwitchCardNo, I32 DistX, I32 DistY, I32 DistZ, I32 DistU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_line4(U16 SwitchCardNo, I32 PosX, I32 PosY, I32 PosZ, I32 PosU, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);

// Circular Move
I16 PASCAL _m114_get_center(U16 SwitchCardNo, I32 Sx, I32 Sy, I32 Ex, I32 Ey, I32 Radius, U8 Dir, I32 *Cx, I32 *Cy);
I16 PASCAL _m114_start_tr_arc2(U16 SwitchCardNo, U16 *AxisArray, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_arc2(U16 SwitchCardNo, U16 *AxisArray, I32 Cx, I32 Cy, I32 Ex, I32 Ey, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_arc2(U16 SwitchCardNo, U16 *AxisArray, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_arc2(U16 SwitchCardNo, U16 *AxisArray, I32 Cx, I32 Cy, I32 Ex, I32 Ey, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_tr_arc_xy(U16 SwitchCardNo, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_arc_xy(U16 SwitchCardNo, I32 Cx, I32 Cy, I32 Ex, I32 Ey, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_tr_arc_zu(U16 SwitchCardNo, I32 OffsetCz, I32 OffsetCu, I32 OffsetEz, I32 OffsetEu, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_arc_zu(U16 SwitchCardNo, I32 Cz, I32 Cu, I32 Ez, I32 Eu, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_arc_xy(U16 SwitchCardNo, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_arc_xy(U16 SwitchCardNo, I32 Cx, I32 Cy, I32 Ex, I32 Ey, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sr_arc_zu(U16 SwitchCardNo, I32 OffsetCz, I32 OffsetCu, I32 OffsetEz, I32 OffsetEu, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_sa_arc_zu(U16 SwitchCardNo, I32 Cz, I32 Cu, I32 Ez, I32 Eu, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);

// Cylinder Surface Move
I16 PASCAL _m114_start_tr_arc_xyz(U16 SwitchCardNo, I32 OffsetCx, I32 OffsetCy, I32 OffsetEx, I32 OffsetEy, I32 DistZ, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
I16 PASCAL _m114_start_ta_arc_xyz(U16 SwitchCardNo, I32 Cx, I32 Cy, I32 Ex, I32 Ey, I32 PosZ, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);

// Counter Control
I16 PASCAL _m114_get_position(U16 SwitchCardNo, U16 AxisNo, I32 *pos);
I16 PASCAL _m114_set_position(U16 SwitchCardNo, U16 AxisNo, I32 pos);
I16 PASCAL _m114_get_command(U16 SwitchCardNo, U16 AxisNo, I32 *cmd);
I16 PASCAL _m114_set_command(U16 SwitchCardNo, U16 AxisNo, I32 cmd);
I16 PASCAL _m114_get_error_counter(U16 SwitchCardNo, U16 AxisNo, I16 *error);
I16 PASCAL _m114_reset_error_counter(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_get_general_counter(U16 SwitchCardNo, U16 AxisNo, I32 *CntValue);
I16 PASCAL _m114_set_general_counter(U16 SwitchCardNo, U16 AxisNo, I16 CntSrc, I32 CntValue);
I16 PASCAL _m114_get_target_pos(U16 SwitchCardNo, U16 AxisNo, I32 *pos);
I16 PASCAL _m114_set_target_pos(U16 SwitchCardNo, U16 AxisNo, I32 pos);

// Ring Counter
I16 PASCAL _m114_enable_command_ring_counter(U16 SwitchCardNo, U16 AxisNo, U32 RingCounter);
I16 PASCAL _m114_disable_command_ring_counter(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_enable_position_ring_counter(U16 SwitchCardNo, U16 AxisNo, U32 RingCounter);
I16 PASCAL _m114_disable_position_ring_counter(U16 SwitchCardNo, U16 AxisNo);

// Soft Limit
I16 PASCAL _m114_set_soft_limit(U16 SwitchCardNo, U16 AxisNo, I32 PLimit, I32 NLimit);
I16 PASCAL _m114_enable_soft_limit(U16 SwitchCardNo, U16 AxisNo, I16 Action);
I16 PASCAL _m114_disable_soft_limit(U16 SwitchCardNo, U16 AxisNo);

// Counter Latch
I16 PASCAL _m114_set_ltc_logic(U16 SwitchCardNo, U16 AxisNo, I16 ltc_logic);
I16 PASCAL _m114_get_latch_data(U16 SwitchCardNo, U16 AxisNo, I16 LatchNo, I32 *Pos);

// Counter Comparator
I16 PASCAL _m114_set_general_comparator(U16 SwitchCardNo, U16 AxisNo, I16 CmpSrc, I16 CmpMethod, I16 CmpAction, I32 Data);
I16 PASCAL _m114_check_compare_data(U16 SwitchCardNo, U16 AxisNo, I16 CompType, I32 *Pos);
I16 PASCAL _m114_check_compare_status(U16 SwitchCardNo, U16 AxisNo, U16 *cmp_sts);

// Counter Comparator Trigger Out (Axis 2 & 3)
I16 PASCAL _m114_enable_trigger_comparator(U16 SwitchCardNo, U16 AxisNo, U8 CmpSrc, U8 CmpMethod , I32 CmpData);
I16 PASCAL _m114_disable_trigger_comparator(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_get_trigger_available_buffer(U16 SwitchCardNo, U16 AxisNo, U8 *buffer);
I16 PASCAL _m114_add_trigger_comparator(U16 SwitchCardNo, U16 AxisNo, I32 CmpData);
I16 PASCAL _m114_cancel_trigger_buffer(U16 SwitchCardNo, U16 AxisNo);

// Auto Compare (Axis 0 & 1)
I16 PASCAL _m114_get_auto_compare_encoder(U16 SwitchCardNo, U16 AxisNo, I32 *EncPos);
I16 PASCAL _m114_set_auto_compare_encoder(U16 SwitchCardNo, U16 AxisNo, I32 EncPos);
I16 PASCAL _m114_get_auto_compare_count(U16 SwitchCardNo, U16 AxisNo, U16 *Count);
I16 PASCAL _m114_get_auto_compare_status(U16 SwitchCardNo, U16 AxisNo, U16 *OnOff);
I16 PASCAL _m114_set_auto_compare_trigger(U16 SwitchCardNo, U16 AxisNo, U16 Level, U16 Width);
I16 PASCAL _m114_set_auto_compare_function(U16 SwitchCardNo, U16 AxisNo, U8 Dir, I32 StrPos, I32 Interval, U16 TrgCnt);
I16 PASCAL _m114_set_auto_compare_table(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U16 Size, I32 *Table);
I16 PASCAL _m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
I16 PASCAL _m114_force_trigger_output(U16 SwitchCardNo, U16 AxisNo);

// Continuous Move
I16 PASCAL _m114_check_continuous_buffer(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_get_continuous_buffer_status(U16 SwitchCardNo, U16 AxisNo);
I16 PASCAL _m114_cancel_continuous_buffer(U16 SwitchCardNo, U16 AxisNo);


#ifdef __cplusplus
}
#endif


#endif //__PCI_M114_H__
