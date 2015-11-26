//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DeltaPLC.h"
#include "dmt.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
CDeltaPLC g_ModBus(1);
//---------------------------------------------------------------------------
__fastcall CDeltaPLC::CDeltaPLC(int nPort)
{
         /*
        conn_num                序列埠編號，如：COM"1"  (支援多條連線)

        baud_rate                 鮑率，如： 9600

        data_len                   資料長度，如：7

        parity                        同位元，如：'E'

        stop_bits                  停止位元，如：1

        modbus_mode        Modbus模式，1：Modbus/ASCII，2：Modbus/RTU 
        */
        
        m_nComPort=nPort;
        int nRes=OpenModbusSerial(m_nComPort, 9600, 8, 'E', 1, 2);         //RTU

        if(nRes==-1) m_bInitOK=false;
        else m_bInitOK=true;

        m_bTerminated=true;        
}
//---------------------------------------------------------------------------
__fastcall CDeltaPLC::~CDeltaPLC()
{
        CloseSerial(m_nComPort);
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::ReadBitDevice(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength)
{
        //return false;   //debug
        if(!m_bInitOK) return false;
        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nLength & 0xff00)>>8;
       Send[3]=nLength & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 2,Send,4);

       if(nLen==-1) {m_bInitOK=false;return false;}


        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */


       int nStation;
       int nFCode;

       //memset(Data,0,nLength/8);
       nLen=ResponseData(0,m_nComPort, &nStation, &nFCode, Data);

       if(nFCode!=2) {m_bInitOK=false;return false;}
      if(nLen==-1) {m_bInitOK=false;return false;}

      return true;
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::ReadWordDevice(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength)
{

        if(!m_bInitOK) return false;
        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nLength & 0xff00)>>8;
       Send[3]=nLength & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 3,Send,4);

       if(nLen==-1) {m_bInitOK=false;return false;}


        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */


       int nStation;
       int nFCode;

       //memset(Data,0,nLength/8);
       nLen=ResponseData(0,m_nComPort, &nStation, &nFCode, Data);

        if(nFCode!=3) {m_bInitOK=false;return false;}
      if(nLen==-1) {m_bInitOK=false;return false;}

      return true;
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::ReadWordDeviceNew(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength)
{

        if(!m_bInitOK) return false;
        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nLength & 0xff00)>>8;
       Send[3]=nLength & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 4,Send,4);

       if(nLen==-1) {m_bInitOK=false;return false;}


        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */


       int nStation;
       int nFCode;

       //memset(Data,0,nLength/8);
       nLen=ResponseData(0,m_nComPort, &nStation, &nFCode, Data);

        if(nFCode!=4) {m_bInitOK=false;return false;}
      if(nLen==-1) {m_bInitOK=false;return false;}

      return true;
}

//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::SetBitDevice(int nID,unsigned __int16 nAddr,bool bOnOff)
{
        if(!m_bInitOK) return false;
        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       if(bOnOff) Send[2]=0xff;
       else Send[2]=0x00;
       Send[3]=0x00;

       int nLen=RequestData(0, m_nComPort, nID, 5,Send,4);

       if(nLen==-1) {m_bInitOK=false;return false;}


        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */


       int nStation;
       int nFCode;
       unsigned char Data[4]={0};
       //memset(Data,0,nLength/8);
       nLen=ResponseData(0,m_nComPort, &nStation, &nFCode, Data);

        if(nFCode!=5) {m_bInitOK=false;return false;}
      if(nLen==-1) {m_bInitOK=false;return false;}

      return true;
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::SetWordDevice(int nID,unsigned __int16 nAddr, __int16 nWord)
{
        if(!m_bInitOK) return false;
        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nWord & 0xff00)>>8;
       Send[3]=nWord & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 6,Send,4);

       if(nLen==-1) return false;


        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */


       int nStation;
       int nFCode;
       unsigned char Data[4]={0};
       //memset(Data,0,nLength/8);
       nLen=ResponseData(0,m_nComPort, &nStation, &nFCode, Data);

        if(nFCode!=6) {m_bInitOK=false;return false;}
      if(nLen==-1) {m_bInitOK=false;return false;}

      return true;
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::SetDWDDevice(int nID,unsigned __int16 nAddr, __int32 nDWord)
{
        if(!m_bInitOK) return false;
        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;
       Send[2]=0x00;
       Send[3]=0x02;
       Send[4]=0x04;

       Send[5]=(nDWord & 0x0000ff00)>>8;
       Send[6]= nDWord & 0x000000ff;
       Send[7]=(nDWord & 0xff000000)>>24;
       Send[8]=(nDWord & 0x00ff0000)>>16;

       int nLen=RequestData(0, m_nComPort, nID, 16,Send,9);

       if(nLen==-1) {m_bInitOK=false;return false;}

              
        /*
       comm_type         通訊種類, 0:序列埠, 1:乙太網路

        conn_num            序列埠編號或連線識別碼

        station_addr         站號

        func_code            功能碼

        data                      Modbus數據

        datalen                 Modbus數據長度(Buffer內的數據長度)
       */


       int nStation;
       int nFCode;
       unsigned char Data[4]={0};
       //memset(Data,0,nLength/8);
       nLen=ResponseData(0,m_nComPort, &nStation, &nFCode, Data);

        if(nFCode!=16) {m_bInitOK=false;return false;}
      if(nLen==-1) {m_bInitOK=false;return false;}

      return true;
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::ReadDWDDevice(int nID,unsigned __int16 nAddr,__int32 &Data)
{
        unsigned char byteData[4];

        bool  bRet=ReadWordDevice(nID,nAddr,byteData,4);

        if(!bRet) return bRet;

        Data=(byteData[0]<<8)+byteData[1]+(byteData[2]<<24)+(byteData[3]<<16);
        return true;


}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::ReadWDDevice(int nID,unsigned __int16 nAddr,__int16 &Data)
{
        unsigned char byteData[4];

        bool  bRet=ReadWordDevice(nID,nAddr,byteData,4);

        if(!bRet) return bRet;

        Data=(byteData[0]<<8)+byteData[1];
        return true;


}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::ReadWDDeviceNew(int nID,unsigned __int16 nAddr,__int16 &Data)
{
        unsigned char byteData[4];

        bool  bRet=ReadWordDeviceNew(nID,nAddr,byteData,4);

        if(!bRet) return bRet;

        Data=(byteData[0]<<8)+byteData[1];
        return true;


}
//---------------------------------------------------------------------------
void __fastcall CDeltaPLC::StartJog(int nAxis,bool bDir)
{
        if(nAxis>6) return;
        unsigned __int16 nAddr[7][2];

        nAddr[0][0]=2040;
        nAddr[0][1]=2041;
        nAddr[1][0]=2042;
        nAddr[1][1]=2043;
        nAddr[2][0]=2044;
        nAddr[2][1]=2045;
        nAddr[3][0]=2046;
        nAddr[3][1]=2047;
        nAddr[4][0]=2048;
        nAddr[4][1]=2049;
        nAddr[5][0]=2050;
        nAddr[5][1]=2051;
        nAddr[6][0]=2044;
        nAddr[6][1]=2045;


        if(bDir) SetBitDevice(1,M1_ADDR+nAddr[nAxis][0],true);
        else SetBitDevice(1,M1_ADDR+nAddr[nAxis][1],true);
}
//---------------------------------------------------------------------------
void __fastcall CDeltaPLC::StopJog(int nAxis)
{
        if(nAxis>6) return;
        unsigned __int16 nAddr[7][2];

        nAddr[0][0]=2040;
        nAddr[0][1]=2041;
        nAddr[1][0]=2042;
        nAddr[1][1]=2043;
        nAddr[2][0]=2044;
        nAddr[2][1]=2045;
        nAddr[3][0]=2046;
        nAddr[3][1]=2047;
        nAddr[4][0]=2048;
        nAddr[4][1]=2049;
        nAddr[5][0]=2050;
        nAddr[5][1]=2051;
        nAddr[6][0]=2044;
        nAddr[6][1]=2045;


        SetBitDevice(1,M1_ADDR+nAddr[nAxis][0],false);
        SetBitDevice(1,M1_ADDR+nAddr[nAxis][1],false);
}
//---------------------------------------------------------------------------
void __fastcall CDeltaPLC::AxisHome(int nAxis)
{
        SetBitDevice(1,M1_ADDR+2020+nAxis,true);
}
//---------------------------------------------------------------------------
double __fastcall CDeltaPLC::GetAxisPos(int nAxis)
{
        if(nAxis>=8) return 0.0;
        __int16 nPos16=0;
        __int32 nPos32=0;
        unsigned __int16 nAddr[8];

        nAddr[PLC::Press_Y]=400;
        nAddr[PLC::PnP_X]=402;
        nAddr[PLC::CCD_X]=404;
        nAddr[PLC::PnP_Z]=408;
        nAddr[PLC::Lane_Y]=406;
        nAddr[PLC::UnLoad_Z]=410;
        nAddr[PLC::BOAT_Z]=404;

        if(nAxis==PLC::CCD_X || nAxis ==PLC::UnLoad_Z)
        {
                ReadDWDDevice(1,D0_ADDR+nAddr[nAxis],nPos32);
                return nPos32;
        }
        else
        {
                ReadWDDevice(1,D0_ADDR+nAddr[nAxis],nPos16);
                return nPos16;
        }

        //return nPos;
}
//---------------------------------------------------------------------------
void __fastcall CDeltaPLC::SetAxisWorkSpeed(int nAxis,__int32 nSpeed)
{
        if(nAxis>=8) return;
        unsigned __int16 nAddr[8];

        nAddr[PLC::Press_Y]=2000;
        nAddr[PLC::PnP_X]=2020;
        nAddr[PLC::CCD_X]=2040;
        nAddr[PLC::PnP_Z]=2060;
        nAddr[PLC::Lane_Y]=2080;
        nAddr[PLC::UnLoad_Z]=2100;
        nAddr[PLC::CCD_Y]=2042;         //16 bits
        nAddr[PLC::CCD_Y]=2042;
        nAddr[PLC::BOAT_Z]=2302;

         if(nAxis==PLC::CCD_X || nAxis ==PLC::UnLoad_Z )
                SetDWDDevice(1,D0_ADDR+nAddr[nAxis],nSpeed);
         else SetWordDevice(1,D0_ADDR+nAddr[nAxis],(__int16)nSpeed);
}
//---------------------------------------------------------------------------
void __fastcall CDeltaPLC::SetAxisJogSpeed(int nAxis,__int32 nSpeed)
{
        if(nAxis>=8) return;
        unsigned __int16 nAddr[8];

        nAddr[PLC::Press_Y]=2150;
        nAddr[PLC::PnP_X]=2152;
        nAddr[PLC::CCD_X]=2154;
        nAddr[PLC::PnP_Z]=2158;
        nAddr[PLC::Lane_Y]=2156;
        nAddr[PLC::UnLoad_Z]=2160;
        nAddr[PLC::BOAT_Z]=2154;

        if(nAxis==PLC::CCD_X || nAxis ==PLC::UnLoad_Z)
                SetDWDDevice(1,D0_ADDR+nAddr[nAxis],nSpeed);
         else SetWordDevice(1,D0_ADDR+nAddr[nAxis],(__int16)nSpeed);
}
//---------------------------------------------------------------------------
void __fastcall CDeltaPLC::RunMode(bool bAuto)
{
        SetBitDevice(1,M1_ADDR+2063,bAuto);
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::SetHeartBit()
{
        return SetBitDevice(1,M1_ADDR+2010,true);
}
//---------------------------------------------------------------------------
bool __fastcall CDeltaPLC::GetBitOnOff(int nID,unsigned __int16 nAddr)
{
        unsigned char Data;
        if(!ReadBitDevice(nID,nAddr,&Data,1)) return false;
        else
        {
                return (Data & 0x01) ? true : false;
        }
}
//---------------------------------------------------------------------------
double __fastcall CDeltaPLC::GetPV(int nID)                //Get Current Temperature
{
        __int16 Data=0;
        bool ret=ReadWDDevice(nID,0x4700,Data);

        return (double)Data*0.1;
}
//---------------------------------------------------------------------------
void __fastcall CDeltaPLC::SetSV(int nID,double dValue)    //Set Target Temperature
{
        SetWordDevice(nID,0x4701,dValue*10);
}
//---------------------------------------------------------------------------
double __fastcall CDeltaPLC::GetAnalogData(int nID,int nChannel)
{
         __int16 nData=0;
         double dData;
        bool ret=ReadWDDeviceNew(nID,nChannel,nData);

        dData=22.55-nData*3.06/1331.0;

        return dData;
}

