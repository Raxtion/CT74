#ifdef __DNM100APIFUNCTION
     #define EXP extern "C" __declspec (dllexport)
#else
     #define EXP extern "C" __declspec (dllimport)
#endif





//Board Functions
EXP DWORD CALLBACK DNM100_GetBoardInf(BYTE BoardNo, DWORD *dwVID,DWORD *dwDID,
						    DWORD *dwSVID,DWORD *dwSDID,DWORD *dwSAuxID,DWORD *dwIrqNo);
EXP DWORD CALLBACK DNM100_TotalDNM100Board(BYTE *TotalBoards ,BYTE *BoardIDList);
EXP DWORD CALLBACK DNM100_ActiveBoard(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_CloseBoard(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_GetDLLVersion(void);

//Firmware Functions
EXP DWORD CALLBACK DNM100_GetFirmwareVersion(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_ResetFirmware(BYTE BoardNo);

//Master Operating Functions
EXP DWORD CALLBACK DNM100_GetMasterMACID(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_SetMasterMACID(BYTE BoardNo,BYTE MasterMACID);
EXP DWORD CALLBACK DNM100_GetBaudRate(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_SetBaudRate(BYTE BoardNo,BYTE BaudRate);
EXP DWORD CALLBACK DNM100_GetMasterStatus(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_GetSlaveStatus(BYTE BoardNo,BYTE DesMACID);

EXP DWORD CALLBACK DNM100_StartDevice(BYTE BoardNo,BYTE DesMACID);
EXP DWORD CALLBACK DNM100_StopDevice(BYTE BoardNo,BYTE DesMACID);
EXP DWORD CALLBACK DNM100_StartAllDevice(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_StopAllDevice(BYTE BoardNo);

EXP DWORD CALLBACK DNM100_AddDevice(BYTE BoardNo,BYTE DesMACID,WORD Explicit_EPR);
EXP DWORD CALLBACK DNM100_RemoveDevice(BYTE BoardNo,BYTE DesMACID);
EXP DWORD CALLBACK DNM100_AddIOConnection(BYTE BoardNo,BYTE DesMACID,BYTE ConType,WORD DeviceInputLen,WORD DeviceOutputLen,WORD EPR);
EXP DWORD CALLBACK DNM100_RemoveIOConnection(BYTE BoardNo,BYTE DesMACID,BYTE ConType);

EXP DWORD CALLBACK DNM100_GetAttribute(BYTE BoardNo,BYTE DesMACID,BYTE ClassID,BYTE InstanceID,BYTE AttributeID);
EXP DWORD CALLBACK DNM100_GetAttributeW(BYTE BoardNo,BYTE DesMACID,WORD ClassID,WORD InstanceID,BYTE AttributeID);

EXP DWORD CALLBACK DNM100_IsGetAttributeOK(BYTE BoardNo,BYTE DesMACID);
EXP DWORD CALLBACK DNM100_GetAttributeValue(BYTE BoardNo,BYTE DesMACID,WORD *DataLen,BYTE *DATA);

EXP DWORD CALLBACK DNM100_SetAttribute(BYTE BoardNo,BYTE DesMACID,BYTE ClassID,BYTE InstanceID,BYTE AttributeID,WORD DataLen,BYTE *DATA);
EXP DWORD CALLBACK DNM100_SetAttributeW(BYTE BoardNo,BYTE DesMACID,WORD ClassID,WORD InstanceID,BYTE AttributeID,WORD DataLen,BYTE *DATA);

EXP DWORD CALLBACK DNM100_IsSetAttributeOK(BYTE BoardNo,BYTE DesMACID);
EXP DWORD CALLBACK DNM100_ClearAllConfig(BYTE BoardNo);

EXP DWORD CALLBACK DNM100_SearchAllDevices(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_SearchSpecificDevice(BYTE BoardNo,WORD ListCount,BYTE *DesMACIDList);
EXP DWORD CALLBACK DNM100_IsSearchOK(BYTE BoardNo);
EXP DWORD CALLBACK DNM100_GetSearchedDevices(BYTE BoardNo,WORD *TotalDevices,BYTE *DesMACID,BYTE *Type,WORD *DeviceInputLen,WORD *DeviceOutputLen);
EXP DWORD CALLBACK DNM100_GetDeviceInfoFromScanList(BYTE BoardNo,BYTE DesMACID,WORD *ListCount,BYTE *ConnectionTypeList,WORD *InputDataLenList,WORD *OutputDataLenList,WORD *EPRList);
EXP DWORD CALLBACK DNM100_GetScanList(BYTE BoardNo,WORD *TotalDevices,BYTE *DesMACIDList,BYTE *ConnectionTypeList,WORD *InputDataLenList,WORD *OutputDataLenList,WORD *EPR_List);
EXP DWORD CALLBACK DNM100_ImportEEPROM(BYTE BoardNo,WORD ListCount,BYTE *DesMACIDList,BYTE *ConnectionTypeList,WORD *InputDataLenList,WORD *OutputDataLenList,WORD *EPR_List);

// I/O Configuring Functions
EXP DWORD CALLBACK DNM100_ReadInputData(BYTE BoardNo,BYTE DesMACID,BYTE ConType,WORD *IOLen,BYTE *IODATA);
EXP DWORD CALLBACK DNM100_WriteOutputData(BYTE BoardNo,BYTE DesMACID,BYTE ConType,WORD IOLen,BYTE *IODATA);

EXP DWORD CALLBACK DNM100_SendExplicitMSG(BYTE BoardNo,BYTE DesMACID,BYTE ServiceID,BYTE ClassID,BYTE InstanceID,WORD DataLen,BYTE *DATA);
EXP DWORD CALLBACK DNM100_SendExplicitMSG_W(BYTE BoardNo,BYTE DesMACID,BYTE ServiceID,WORD ClassID,WORD InstanceID,WORD DataLen,BYTE *DATA);

EXP DWORD CALLBACK DNM100_IsExplicitMSGRespOK(BYTE BoardNo,BYTE DesMACID);
EXP DWORD CALLBACK DNM100_GetExplicitMSGRespValue(BYTE BoardNo,BYTE DesMACID,WORD *DataLen,BYTE *DATA);

EXP DWORD CALLBACK DNM100_ReadbackOutputData(BYTE BoradNo,BYTE DesMACID,BYTE ConType,WORD *IOLen,BYTE *IODATA);

//for Firmware V2.3 or newer
EXP DWORD CALLBACK DNM100_ReadInputArea(BYTE BoradNo,WORD Offset,WORD DataLen,BYTE *DataArray);
EXP DWORD CALLBACK DNM100_WriteOutputArea(BYTE BoradNo,WORD Offset,WORD DataLen,BYTE *DataArray);
EXP DWORD CALLBACK DNM100_ReadbackOutputArea(BYTE BoradNo,WORD Offset,WORD DataLen,BYTE *DataArray);



//ConnectionType
#define ConType_Explicit  0
#define ConType_Poll      1
#define ConType_BitStrobe 2
#define ConType_COS       3
#define ConType_Cyclic    4
#define ConType_MultiPoll 5



//Board Error Code
#define DNM100_NoError                    0
#define DNM100_DriverError                10001
#define DNM100_ActiveBoardError           10002
#define DNM100_BoardNumberError           10003
#define DNM100_PortNumberError            10004 
#define DNM100_InitError                  10007       
#define DNM100_SoftBufferIsEmpty          10021
#define DNM100_SoftBufferIsFull           10022
#define DNM100_TimeOut                    10023
#define DNM100_SetCyclicMsgFailure        10024
#define DNM100_DpramOverRange             10025
#define DNM100_NoDpramCmd                 10026
#define DNM100_ModeError                  10027
#define DNM100_NoFileInside               10030
#define DNM100_DownloadFailure            10031
#define DNM100_EEPROMDamage               10032
#define DNM100_NotEnoughSpace             10033
#define DNM100_StillDownloading           10034
#define DNM100_BoardModeError             10035
#define DNM100_CardTypeError              10036



//Firmware Error Code
#define DNMXS_NoError               0
#define DNMXS_UnKnowError           5000

//Master Status Error Code
#define DNMXS_BoardNotActive        1000
#define DNMXS_OnlineError           1001
#define DNMXS_CANBusError           1002
#define DNMXS_Booting               1003

//General Error Code
#define DNMXS_MACIDError            1050
#define DNMXS_BaudRateError         1051
#define DNMXS_ConnectionTypeError   1052
#define DNMXS_DuplicMasterMACID     1053 //DesMACID»PMaster MACID¬Û¦P 
#define DNMXS_EEPROMError           1054
#define DNMXS_NowScanning           1055
#define DNMXS_ScanListError         1056
#define DNMXS_DeviceExist           1057
#define DNMXS_DeviceNotExist        1058 
#define DNMXS_MapTableError         1059

//IOConnection Error
#define DNMXS_ExplicitNotAllocate   1100
#define DNMXS_PollNotAllocate       1101
#define DNMXS_BitStrobeNotAllocate  1102
#define DNMXS_COSNotAllocate        1103
#define DNMXS_CyclicNotAllocate     1104
#define DNMXS_PollAlreadyExist      1105
#define DNMXS_BitStrobeAlreadyExist 1106
#define DNMXS_COSAlreadyExist       1107
#define DNMXS_CyclicAlreadyExist    1108
#define DNMXS_CommunicationPause    1109

//Slave Error Code
#define DNMXS_SlaveNoResp           1150
#define DNMXS_WaitForSlaveResp      1151
#define DNMXS_SlaveRespError        1152
#define DNMXS_OutputDataLenError    1153
#define DNMXS_InputDataLenError     1154

//Input / Output Area
#define DNMXS_OutofRange            1200

