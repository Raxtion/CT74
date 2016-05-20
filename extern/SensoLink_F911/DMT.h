
/////////////////////////////////////////////////////////////////////////
//
//	Function:
//		DMT.h for Visual C++ 6.0
//
//	Description:
//		DMT Library VC application declare function header
//
//	History:
//		Date		Author			Comment
//		5/01/2009	Anderson/Taoyuan I	Version 2.0
//		10/01/2007	Anderson/Taoyuan I	Version 1.1
//		8/01/2007	Anderson/Taoyuan I	Version 1.0
//	
/////////////////////////////////////////////////////////////////////////

// Export functions
#ifdef	__cplusplus
extern "C"
{
#endif	/*  __cplusplus */

// Data Access
// Modbus Data Request Function
int WINAPI RequestData(int comm_type, int conn_num, int slave_addr, int func_code, const unsigned char *data, int datalen);
// Modbus Data Response Function
int WINAPI ResponseData(int comm_type, int conn_num, int *slave_addr, int *func_code, unsigned char *data);

// Serial Communication
// Modbus Serial Communication Function
int WINAPI OpenModbusSerial(int conn_num, int baud_rate, int data_len, char parity, int stop_bits, int modbus_mode);
// Modbus Serial Communication Extra Function
//int WINAPI OpenModbusSerialEx(int comport_num, int baud_rate, int data_len, char parity, int stop_bits, int modbus_mode);
// Close Serial
void WINAPI CloseSerial(int conn_num);

// Get Serial Error Message Function
int WINAPI GetLastSerialErr();
// Clear Serial Error Message Function
void WINAPI ResetSerialErr();

// Socket Communication
// Modbus Socket Communication Function
int WINAPI OpenModbusTCPSocket(int conn_num, int ipaddr);
// Close Socket Function
void WINAPI CloseSocket(int conn_num);

// Get Socket Error Message Function
int WINAPI GetLastSocketErr();
// Clear Socket Error Message Function
void WINAPI ResetSocketErr();

// Socket Select Function
int WINAPI ReadSelect(int conn_num, int SelectTime_msec);

#ifdef	__cplusplus
}
#endif	/*  __cplusplus */