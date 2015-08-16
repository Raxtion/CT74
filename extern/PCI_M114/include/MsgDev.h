#ifndef __MSGDEV_H__
#define __MSGDEV_H__

#include "Type_def.h"

//
// Message Device Type
#define G9004_P120				0x1620	// Pulse count
#define G9004_T140				0x2040	// Temperature measure
#define G9004_G180				0x0780	// DCON to Motionnet Gateway
#define G9004_A122				0x0122	// Analog input and output
//
// Error Code
#define ERR_MSG_NonsenseParameter1  1
#define ERR_MSG_NonsenseParameter2  2
#define ERR_MSG_NonsenseParameter3  3
#define ERR_MSG_NonsenseParameter4  4
#define ERR_MSG_NonsenseParameter5  5
#define ERR_MSG_NonsenseParameter6  6
#define ERR_MSG_NonsenseParameter7  7
#define ERR_MSG_NonsenseParameter8  8
#define ERR_MSG_NonsenseParameter9  9
#define ERR_MSG_NoError             0
#define ERR_MSG_Base                (-1000)
#define ERR_MSG_InvalidDeviceType   (ERR_MSG_Base-1)
#define ERR_MSG_InvalidParameter1   (ERR_MSG_Base-2)
#define ERR_MSG_InvalidParameter2   (ERR_MSG_Base-3)
#define ERR_MSG_InvalidParameter3   (ERR_MSG_Base-4)
#define ERR_MSG_InvalidParameter4   (ERR_MSG_Base-5)
#define ERR_MSG_InvalidParameter5   (ERR_MSG_Base-6)
#define ERR_MSG_InvalidParameter6   (ERR_MSG_Base-7)
#define ERR_MSG_InvalidParameter7   (ERR_MSG_Base-8)
#define ERR_MSG_InvalidParameter8   (ERR_MSG_Base-9)
#define ERR_MSG_InvalidParameter9   (ERR_MSG_Base-10)
#define ERR_MSG_CommunicationError  (ERR_MSG_Base-11)


#ifndef PASCAL
#	define PASCAL __stdcall
#endif

#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////
// {{ P120
I16 PASCAL _mnet_p120_initial(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_p120_get_hardware_info(U16 RingNo, U16 SlaveIP, U8* DeviceID, U8* MCU_Version);
I16 PASCAL _mnet_p120_start_counter(U16 RingNo, U16 SlaveIP, U16 ChannelNo);
I16 PASCAL _mnet_p120_stop_counter(U16 RingNo, U16 SlaveIP, U16 ChannelNo);
I16 PASCAL _mnet_p120_reset_counter(U16 RingNo, U16 SlaveIP, U16 ChannelNo);
I16 PASCAL _mnet_p120_get_count(U16 RingNo, U16 SlaveIP, U16 ChannelNo, U32* Value);
I16 PASCAL _mnet_p120_get_count_all(U16 RingNo, U16 SlaveIP, U32 Value[4]);
I16 PASCAL _mnet_p120_get_frequency(U16 RingNo, U16 SlaveIP, U16 ChannelNo, F64* Value);
I16 PASCAL _mnet_p120_get_frequency_all(U16 RingNo, U16 SlaveIP, F64 Value[4]);
I16 PASCAL _mnet_p120_get_period(U16 RingNo, U16 SlaveIP, U16 ChannelNo, F64* Value);
I16 PASCAL _mnet_p120_get_period_all(U16 RingNo, U16 SlaveIP, F64 Value[4]);
I16 PASCAL _mnet_p120_get_gate_time(U16 RingNo, U16 SlaveIP, U16 ChannelNo, F64* Value);
I16 PASCAL _mnet_p120_get_gate_time_all(U16 RingNo, U16 SlaveIP, F64 Value[4]);
I16 PASCAL _mnet_p120_get_gate_count(U16 RingNo, U16 SlaveIP, U16 GroupNo, U32* Value);
I16 PASCAL _mnet_p120_get_gate_count_all(U16 RingNo, U16 SlaveIP, U32 Value[2]);
// }} P120
////////////////////////////////////////////////////////////////////////////////
// {{ A122
//
// Input Mode Define
#define MODE_SINGLE_ENDED 0
#define MODE_DIFFERENTIAL 1
// Input Gain Define
#define GAIN_80MV         0 // +/-   80mV
#define GAIN_160MV        1 // +/-  160mV
#define GAIN_320MV        2 // +/-  320mV
#define GAIN_640MV        3 // +/-  640mV
#define GAIN_1280MV       4 // +/- 1280mV
#define GAIN_2_56V        5 // +/-  2.56V
#define GAIN_5_12V        6 // +/-  5.12V
#define GAIN_10_24V       7 // +/- 10.24V
//
I16 PASCAL _mnet_a122_initial(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_a122_get_hardware_info(U16 RingNo, U16 SlaveIP, U8* DeviceID, U8* MCU_Version);
I16 PASCAL _mnet_a122_get_input_channel_value(U16 RingNo, U16 SlaveIP, U16 ChannelNo, I16* Value);
I16 PASCAL _mnet_a122_get_input_value_all(U16 RingNo, U16 SlaveIP, I16* Value);
I16 PASCAL _mnet_a122_get_input_channel_voltage(U16 RingNo, U16 SlaveIP, U16 ChannelNo, F64* Volt);
I16 PASCAL _mnet_a122_get_input_voltage_all(U16 RingNo, U16 SlaveIP, F64* Volt);
I16 PASCAL _mnet_a122_set_output_channel_value(U16 RingNo, U16 SlaveIP, U16 ChannelNo, I16 Value);
I16 PASCAL _mnet_a122_set_output_value_all(U16 RingNo, U16 SlaveIP, I16 Value0, I16 Value1, I16 Value2, I16 Value3);
I16 PASCAL _mnet_a122_set_output_channel_voltage(U16 RingNo, U16 SlaveIP, U16 ChannelNo, F64 Volt);
I16 PASCAL _mnet_a122_set_output_voltage_all(U16 RingNo, U16 SlaveIP, F64 Volt0, F64 Volt1, F64 Volt2, F64 Volt3);
// }} A122
////////////////////////////////////////////////////////////////////////////////
// {{ G180
//
#define ERR_G180_Base               (-1100)
#define ERR_G180_InvalidCommand     (ERR_G180_Base-1)
#define ERR_G180_ChecksumError      (ERR_G180_Base-2)
#define ERR_G180_Timeout            (ERR_G180_Base-3)
//
I16 PASCAL _mnet_g180_initial(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_g180_get_hardware_info(U16 RingNo, U16 SlaveIP, U8* DeviceID, U8* MCU_Version);
I16 PASCAL _mnet_g180_run_mode(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_g180_get_channel_value(U16 RingNo, U16 SlaveIP, U16 AA, U16 ChannelNo, U32* Value, U8* AI_Diagnostic);
I16 PASCAL _mnet_g180_set_channel_value(U16 RingNo, U16 SlaveIP, U16 AA, U16 ChannelNo, U32 Value);
// }} G180
////////////////////////////////////////////////////////////////////////////////
// {{ T140
//
#define ERR_T140_Base            (-1200)
#define ERR_T140_InvalidGain     (ERR_T140_Base-1)

typedef enum
{
    Centigrade = 0, 
    Fahrenheit = 1, 
}
ET140TemperatureType;

I16 PASCAL _mnet_t140_initial(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_t140_get_hardware_info(U16 RingNo, U16 SlaveIP, U8* DeviceID, U8* MCU_Version);
I16 PASCAL _mnet_t140_get_channel_value(U16 RingNo, U16 SlaveIP, U16 ChannelNo, U16* Value);
I16 PASCAL _mnet_t140_get_value_all(U16 RingNo, U16 SlaveIP, U16 Value[8]);
I16 PASCAL _mnet_t140_get_channel_temperature(U16 RingNo, U16 SlaveIP, U16 ChannelNo, U16 Type, F64* Degree);
I16 PASCAL _mnet_t140_get_temperature_all(U16 RingNo, U16 SlaveIP, U16 Type, F64 Degree[8]);
I16 PASCAL _mnet_t140_get_channel_current(U16 RingNo, U16 SlaveIP, U16 ChannelNo, F64* MilliAmpere);
I16 PASCAL _mnet_t140_get_current_all(U16 RingNo, U16 SlaveIP, F64 MilliAmpere[8]);
//
// }} T140
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif //__MSGDEV_H__
