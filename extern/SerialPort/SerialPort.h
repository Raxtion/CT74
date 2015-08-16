//---------------------------------------------------------------------------

#ifndef SerialPortH
#define SerialPortH

#include "PComm.h"
//---------------------------------------------------------------------------
class CSerialPort 
{
private:
	
        int m_nPortNo;

	//DWORD m_nReadByte;
public:
        __fastcall CSerialPort();
        __fastcall ~CSerialPort();

        bool __fastcall Open(int nPort,int nBaudRate,int nMode);

        bool __fastcall WriteData(char *pBuffer);
        bool __fastcall ReadData(char *pBuffer,char EndByte);


};


//---------------------------------------------------------------------------
#endif
