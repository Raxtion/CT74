//---------------------------------------------------------------------------

#ifndef DeltaPLCH
#define DeltaPLCH
#include <vcl>


#define MAX_X_POINT 128
#define MAX_Y_POINT 128
#define MAX_M_POINT 256
#define MAX_S_POINT 128
#define MAX_T_POINT 128
#define MAX_C_POINT 128
#define MAX_D_WORD 64         //WORD

#define S_ADDR 0x0000
#define X_ADDR 0x0400
#define Y_ADDR 0x0500
#define T_ADDR 0x0600
#define M0_ADDR 0x0800          //M0~M1535
#define M1_ADDR 0xB000-1536          //M1536~M4095
#define C_ADDR 0x0E00
#define D0_ADDR 0x1000          //D0~D4095
#define D1_ADDR 0x9000-4096          //D4096~D44300

#define PLC_UNIT 10.0
#define PLC_UNIT_100 100.0
//---------------------------------------------------------------------------
namespace PLC{
enum PLC_AXIS{Press_Y=0,PnP_X,CCD_X,PnP_Z,Lane_Y,UnLoad_Z,CCD_Y,BOAT_Z};
}
//---------------------------------------------------------------------------
class CDeltaPLC
{
private:
        int m_nComPort;
public:
        bool m_bInitOK; 
        double dF911Data;
public:
        unsigned char m_byteX[MAX_X_POINT/8];
        unsigned char m_byteY[MAX_Y_POINT/8];
        unsigned char m_byteM[5][MAX_M_POINT/8];
        unsigned char m_byteS[MAX_S_POINT/8];
        unsigned char m_byteT[MAX_T_POINT/8];
        unsigned char m_byteC[MAX_C_POINT/8];
        unsigned char m_byteD[6][MAX_D_WORD*2];
        

        bool m_bTerminated;
        __fastcall CDeltaPLC(int nPort);
        __fastcall ~CDeltaPLC();

        bool __fastcall ReadBitDevice(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength);         //Func Code 02,S X Y M T C
        bool __fastcall ReadWordDevice(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength);        //Func Code 03,T C D
        bool __fastcall ReadWordDeviceNew(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength);                               //Func Code 04 //for icpdas m-7017z
        bool __fastcall SetBitDevice(int nID,unsigned __int16 nAddr,bool bOnOff);                                       //Func Code 05 S,Y,M,T,C
        bool __fastcall SetWordDevice(int nID,unsigned __int16 nAddr, __int16 nWord);                                   //Func Code 06 T,C,D
        bool __fastcall SetDWDDevice(int nID,unsigned __int16 nAddr,__int32 nDWord);                                    //Func Code 16 T,C,D
        bool __fastcall ReadDWDDevice(int nID,unsigned __int16 nAddr,__int32 &Data);
        bool __fastcall ReadWDDevice(int nID,unsigned __int16 nAddr,__int16 &Data);                                    //Func Code 03,T C D
        bool __fastcall ReadWDDeviceNew(int nID,unsigned __int16 nAddr,__int16 &Data);                                    //Func Code 04

        bool __fastcall GetBitOnOff(int nID,unsigned __int16 nAddr);

        //-----For Delta PLC-------------------
        void __fastcall StartJog(int nAxis,bool bDir);
        void __fastcall StopJog(int nAxis);
        void __fastcall AxisHome(int nAxis);
        double __fastcall GetAxisPos(int nAxis);
        void __fastcall SetAxisWorkSpeed(int nAxis,__int32 nSpeed);
        void __fastcall SetAxisJogSpeed(int nAxis,__int32 nSpeed);

        void __fastcall RunMode(bool bAuto);
        bool __fastcall SetHeartBit();

        //----For Delta DTA4848 thermal controller
        double __fastcall GetPV(int nID);                //Get Current Temperature
        void __fastcall SetSV(int nID,double dValue);    //Set Target Temperature
        void __fastcall SetTempOffset(int nID,double dValue);    //Set Offset Target Temperature

        //---For ICPDAS M-7017Z
        double __fastcall GetAnalogData(int nID,int nChannel);

        //---For F911
        double __fastcall GetKg(int nID);
        void __fastcall InitialZero(int nID);
};                      

//---------------------------------------------------------------------------
#endif
