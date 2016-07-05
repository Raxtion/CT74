//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SensoLinkF911.h"
#include "dmt.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//CSensoLinkF911 g_F911ModBus(2); //RS485 used. and merge to DeltaPLC.
//---------------------------------------------------------------------------
__fastcall CSensoLinkF911::CSensoLinkF911(int nPort)
{
         /*
        conn_num                �ǦC��s���A�p�GCOM"2"  (�䴩�h���s�u)

        baud_rate                 �j�v�A�p�G 9600

        data_len                   ��ƪ��סA�p�G8

        parity                        �P�줸�A�p�G'E'

        stop_bits                  ����줸�A�p�G1

        modbus_mode        Modbus�Ҧ��A1�GModbus/ASCII�A2�GModbus/RTU
        */

        m_nComPort=nPort;
        int nRes=OpenModbusSerial(m_nComPort, 9600, 8, 'E', 1, 2);         //Modbus/RTU

        if(nRes==-1) m_bInitOK=false;
        else m_bInitOK=true;
        dTmpData = 0;

        m_bTerminated=true;
}
//---------------------------------------------------------------------------
__fastcall CSensoLinkF911::~CSensoLinkF911()
{
        CloseSerial(m_nComPort);
}

//---------------------------------------------------------------------------
bool __fastcall CSensoLinkF911::ReadBitDevice(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength)
{
        //return false;   //debug
        if(!m_bInitOK) return false;
        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nLength & 0xff00)>>8;
       Send[3]=nLength & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 2,Send,4);

       if(nLen==-1) {m_bInitOK=false;return false;}


        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
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
bool __fastcall CSensoLinkF911::ReadWordDevice(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength)
{

        if(!m_bInitOK) return false;
        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nLength & 0xff00)>>8;
       Send[3]=nLength & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 3,Send,4);

       if(nLen==-1) {m_bInitOK=false;return false;}


        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
       */


       int nStation;
       int nFCode;

       //memset(Data,0,nLength/8);
       nLen=ResponseData(0,m_nComPort, &nStation, &nFCode, Data);


        //if(nFCode!=3) {m_bInitOK=false;return false;}

        if(nLen==-1) {m_bInitOK=false;return false;}

      return true;
}
//---------------------------------------------------------------------------
bool __fastcall CSensoLinkF911::ReadWordDeviceNew(int nID,unsigned __int16 nAddr,unsigned char *Data,unsigned int nLength)
{

        if(!m_bInitOK) return false;
        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nLength & 0xff00)>>8;
       Send[3]=nLength & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 4,Send,4);

       if(nLen==-1) {m_bInitOK=false;return false;}


        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
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
bool __fastcall CSensoLinkF911::SetBitDevice(int nID,unsigned __int16 nAddr,bool bOnOff)
{
        if(!m_bInitOK) return false;
        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
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
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
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
bool __fastcall CSensoLinkF911::SetWordDevice(int nID,unsigned __int16 nAddr, __int16 nWord)
{
        if(!m_bInitOK) return false;
        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
       */
       unsigned char Send[100]={0};
       Send[0]=(nAddr & 0xff00)>>8;
       Send[1]=nAddr & 0x00ff;

       Send[2]=(nWord & 0xff00)>>8;
       Send[3]=nWord & 0x00ff;

       int nLen=RequestData(0, m_nComPort, nID, 6,Send,4);

       if(nLen==-1) return false;


        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
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
bool __fastcall CSensoLinkF911::SetDWDDevice(int nID,unsigned __int16 nAddr, __int32 nDWord)
{
        if(!m_bInitOK) return false;
        /*
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
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
       comm_type         �q�T����, 0:�ǦC��, 1:�A�Ӻ���

        conn_num            �ǦC��s���γs�u�ѧO�X

        station_addr         ����

        func_code            �\��X

        data                      Modbus�ƾ�

        datalen                 Modbus�ƾڪ���(Buffer�����ƾڪ���)
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
bool __fastcall CSensoLinkF911::ReadDWDDevice(int nID,unsigned __int16 nAddr,__int32 &Data)
{
        unsigned char byteData[4];

        bool  bRet=ReadWordDevice(nID,nAddr,byteData,4);

        if(!bRet) return bRet;

        Data=(byteData[0]<<8)+byteData[1]+(byteData[2]<<24)+(byteData[3]<<16);
        return true;


}
//---------------------------------------------------------------------------
bool __fastcall CSensoLinkF911::ReadWDDevice(int nID,unsigned __int16 nAddr,__int16 &Data)
{
        unsigned char byteData[4];

        bool  bRet=ReadWordDevice(nID,nAddr,byteData,4);

        if(!bRet) return bRet;

        Data=(byteData[0]<<8)+byteData[1];
        return true;


}
//---------------------------------------------------------------------------
bool __fastcall CSensoLinkF911::ReadWDDeviceNew(int nID,unsigned __int16 nAddr,__int16 &Data)
{
        unsigned char byteData[4];

        bool  bRet=ReadWordDeviceNew(nID,nAddr,byteData,4);

        if(!bRet) return bRet;

        Data=(byteData[0]<<8)+byteData[1];
        return true;


}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall CSensoLinkF911::InitialZero(int nID)
{
    SetBitDevice(nID, 0x0000,true);
}
//---------------------------------------------------------------------------
double __fastcall CSensoLinkF911::GetKg(int nID)
{
    __int32 nData32=0;
    //__int16 Data=0;
    bool ret=ReadDWDDevice(nID, 0x0032, nData32);

    dTmpData=(double)nData32*0.001;
    return (double)nData32*0.001;
}

/*
//usual use address
bit 0x0000 �k�s
bit 0x0002 ��X1���A
bit 0x0003 ��X2���A
bit 0x0004 ��X3���A
bit 0x0005 ��X4���A
word 0x0000 �o�i���]�w
word 0x0010 ��X���A
dword 0x0032 �ثe���o�ƭ�
*/

