#ifndef __MNET_H__
#define __MNET_H__

#include "Type_def.h"

////////////////////////////////////////////////////////////////////////////////
//                                Error Code                                  //
////////////////////////////////////////////////////////////////////////////////
#define ERR_NoError                           0
#define ERR_MNET_Ring_Used                   -1
#define ERR_Invalid_Ring                     -2
#define ERR_Invalid_Slave                    -3
#define ERR_Invalid_Hardware                 -4
#define ERR_Invalid_Sync_Object              -5
#define ERR_RingCommunicationError           -8
#define ERR_RingCommunicationTimeOut         -9
#define ERR_DataFifoFull                    -10
#define ERR_DataCommunicationFailed         -11
#define ERR_InvalidMaxVel                   -16
#define ERR_ObsoleteFunction                -34
#define ERR_G94_RECEIVE_TimeOut             -36
#define ERR_G94_CPURead                     -37
#define ERR_G94_MsgCommunication            -38
#define ERR_G94_MsgSize                     -39
#define ERR_M4_CPLDRead                     -46
#define ERR_M4_RegisterRead                 -47
#define ERR_M4_CPLDWrite                    -48
#define ERR_M4_RegisterWrite                -49
#define ERR_InvalidAxisNo                   -50
#define ERR_FailGetDeviceTable              -74 
#define ERR_NoDeviceFound                   -75 
#define ERR_SlowDownPointError              -101
#define ERR_M1A_InvalidCheckCode            -103
#define ERR_M1A_NotInitializedYet           -105
#define ERR_InvalidParameter_1              -111
#define ERR_InvalidParameter_2              -112
#define ERR_InvalidParameter_3              -113
#define ERR_InvalidParameter_4              -114
#define ERR_InvalidParameter_5              -115
#define ERR_InvalidParameter_6              -116
#define ERR_InvalidParameter_7              -117
#define ERR_InvalidParameter_8              -118
#define ERR_InvalidParameter_9              -119
#define ERR_InvalidParameter_10             -120
#define ERR_InvalidParameter_11             -121
#define ERR_InvalidParameter_12             -122
#define ERR_FunctionNotSupport              -151
#define ERR_InvalidDeviceType               -152
#define ERR_GetDLLPath                      -170
#define ERR_GetDLLVersion                   -171
#define ERR_LoadFileFailed                  -172



////////////////////////////////////////////////////////////////////////////////
//                           Slave Type Definition                            //
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    // Axis
    G9103_M1X1     = 0xA2,
    G9003_M1X1     = 0xA3,
    G9004_M104     = 0xA4,
    G9004_NPM_FUA4 = 0xA5,
    G9004_NPM_M204 = 0xA6,
    G9004_M204     = 0xA7,
	G9004_M304T    = 0xA8,
    // DIO (32 In/Out)
    G9002_Q32      = 0xB0,
    G9002_I16Q16   = 0xB2,
    G9002_I32      = 0xB4,
	// DIO (96 In/Out)
    G9004_I96      = 0xB5,
	G9004_I80Q16   = 0xB6,
	G9004_I64Q32   = 0xB7,
	G9004_I48Q48   = 0xB8,
	G9004_I32Q64   = 0xB9,
	G9004_I16Q80   = 0xBA,
	G9004_Q96      = 0xBB,
    // DIO (16 In/Out)
    G9102_I16      = 0xC0,
    G9102_I8Q8     = 0xC4,
    G9102_Q16      = 0xC7,
    G9205_I16      = 0xC8,
    G9205_I8Q8     = 0xCC,
    G9205_Q16      = 0xCF,
    // AIO
    G9004_A104     = 0xD0,
    G9004_A180     = 0xD1,
    // Message Mode Device
    G9004_MSG      = 0xE0,
    // Others
    G9004_UNKNOWN  = 0xFF, 
} ESlaveType;

////////////////////////////////////////////////////////////////////////////////
//                            Function Declaration                            //
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
//
I16 PASCAL _mnet_open( void );
I16 PASCAL _mnet_close( void );
//
// Ring Operation
I16 PASCAL _mnet_reset_ring( U16 RingNo );
I16 PASCAL _mnet_start_ring( U16 RingNo );
I16 PASCAL _mnet_stop_ring( U16 RingNo );
I16 PASCAL _mnet_get_ring_status( U16 RingNo, U16 *Status );
I16 PASCAL _mnet_get_com_status( U16 RingNo );
I16 PASCAL _mnet_get_ring_error_counter(U16 RingNo, U16 *ErrCount);
I16 PASCAL _mnet_reset_ring_error_counter(U16 RingNo);
I16 PASCAL _mnet_set_ring_config(U16 RingNo, U16 BaudRate);
//
// Slaves
I16 PASCAL _mnet_get_ring_active_table( U16 RingNo, U32 *DevTable );
I16 PASCAL _mnet_get_slave_type( U16 RingNo, U16 SlaveIP, U8 *SlaveType );
I16 PASCAL _mnet_get_msg_slave_type(U16 RingNo, U16 SlaveIP, U16 *Type);
//
// Digital I/O Slave
I16 PASCAL _mnet_io_input( U16 RingNo, U16 SlaveIP, U8 PortNo );
I16 PASCAL _mnet_io_output( U16 RingNo, U16 SlaveIP, U8 PortNo, U8 Val );
I16 PASCAL _mnet_io_input_all(U16 RingNo, U16 SlaveIP, U8 Value[]);
I16 PASCAL _mnet_io_input_all_ex(U16 RingNo, U16 SlaveIP, U8 Value[]);
I16 PASCAL _mnet_io_output_all(U16 RingNo, U16 SlaveIP, U8 Value[]);
I16 PASCAL _mnet_bit_io_input(U16 RingNo, U16 SlaveIP, U8 PortNo, U8 BitNo, U8 *OnOff);
I16 PASCAL _mnet_bit_io_output(U16 RingNo, U16 SlaveIP, U8 PortNo, U8 BitNo, U8 OnOff);
I16 PASCAL _mnet_bit_io_toggle(U16 RingNo, U16 SlaveIP, U8 PortNo, U8 BitNo);
//
// Misc.
#ifndef __INTIME__
I16 PASCAL _mnet_get_DLL_path(TCHAR *lpFilePath, U32 nSize, U32 *nLength);
I16 PASCAL _mnet_get_DLL_version(TCHAR *lpBuf, U32 nSize, U32 *nLength);
#endif //__INTIME__
//
#ifdef __cplusplus
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#endif //__MNET_H__
