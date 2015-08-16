//---------------------------------------------------------------------------


#pragma hdrstop
#include <vcl>

#include "SerialPort.h"
#include <stdio.h>



#define MAX_BUFFER 300

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
__fastcall CSerialPort::CSerialPort()
{

}
//---------------------------------------------------------------------------
__fastcall CSerialPort::~CSerialPort()
{
        sio_close(m_nPortNo);
}
//---------------------------------------------------------------------------
bool __fastcall CSerialPort::Open(int nPort,int nBaudRate,int nMode)
{
        m_nPortNo=nPort;
  if(sio_open(nPort)!=SIO_OK)
    return false;

  //if(sio_ioctl(nPort,B9600,BIT_8|STOP_1|P_NONE)!=SIO_OK)
        if( sio_ioctl(nPort,nBaudRate,nMode) != SIO_OK )
                return false;

  //sio_SetReadTimeouts(nPort,MAXDWORD,2000);

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall CSerialPort::WriteData(char *pBuffer)
{
        if(sio_write(m_nPortNo,pBuffer,strlen(pBuffer))<0)
        return false;

        return true;
}
//---------------------------------------------------------------------------
bool __fastcall CSerialPort::ReadData(char *pBuffer,char EndByte)
{
        DWORD dwStart=GetTickCount();
        char Rx[MAX_BUFFER]={0};
        int nIndex=0;
        int nRet;
        while(1)
        {
                nRet=sio_read(m_nPortNo,pBuffer,MAX_BUFFER);
                if(nRet>0)
                {
                        strncpy(Rx+nIndex,pBuffer,nRet);
                        nIndex+=nRet;
                         if(pBuffer[nRet-1]==EndByte)
                         {
                                strcpy(pBuffer,Rx);
                                return true;
                         }
                }
                if(nRet<0) return false;

                if((GetTickCount()-dwStart)>2000) break;
                ::Sleep(1);
                
        }

    return false;

}

