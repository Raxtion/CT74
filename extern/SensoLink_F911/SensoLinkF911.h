//---------------------------------------------------------------------------

#ifndef SensoLinkF911H
#define SensoLinkF911H
#include <vcl>


#define MAX_X_POINT 128
#define MAX_Y_POINT 128
#define MAX_M_POINT 256
#define MAX_S_POINT 128
#define MAX_T_POINT 128
#define MAX_C_POINT 128
#define MAX_D_WORD 64         //WORD



//---------------------------------------------------------------------------

class CSensoLinkF911
{
private:
        int m_nComPort;

public:
        bool m_bInitOK;
        double dTmpData;
public:
        unsigned char m_byteX[MAX_X_POINT/8];
        unsigned char m_byteY[MAX_Y_POINT/8];
        unsigned char m_byteM[5][MAX_M_POINT/8];
        unsigned char m_byteS[MAX_S_POINT/8];
        unsigned char m_byteT[MAX_T_POINT/8];
        unsigned char m_byteC[MAX_C_POINT/8];
        unsigned char m_byteD[6][MAX_D_WORD*2];
        

        bool m_bTerminated;
        __fastcall CSensoLinkF911(int nPort);
        __fastcall ~CSensoLinkF911();

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

        //-----For SensoLink F911--------------
        void __fastcall InitialZero(int nID);
        double __fastcall GetKg(int nID);

};

//---------------------------------------------------------------------------
#endif
