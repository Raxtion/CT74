//---------------------------------------------------------------------------

#ifndef KeyenceIL030H
#define KeyenceIL030H
#include <vcl>
#include "SerialPort.h"
//---------------------------------------------------------------------------
class CKeyenceIL030:private CSerialPort
{
private:
public:
        bool m_bInitOK;
        int nTmpData;
        __fastcall CKeyenceIL030(int nPort);
        __fastcall ~CKeyenceIL030();

public:
        bool __fastcall OpenPort(int nPort);

        double __fastcall GetLaserValue();
        
};

#endif
