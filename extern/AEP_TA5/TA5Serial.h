//---------------------------------------------------------------------------

#ifndef TA5SerialH
#define TA5SerialH
#include <vcl>
#include "SerialPort.h"
//---------------------------------------------------------------------------
class CTA5Serial:private CSerialPort
{
private:
        double m_dX[2];
        double m_dY[2];          //X:div  Y:Kg
public:
        bool m_bInitOK;
        int nTmpData;
        __fastcall CTA5Serial(int nPort);
        __fastcall ~CTA5Serial();

        void __fastcall SetXY(double x1,double x2,double y1,double y2){m_dX[0]=x1;m_dX[1]=x2;m_dY[0]=y1;m_dY[1]=y2;};      

public:
        bool __fastcall OpenPort(int nPort);

        AnsiString __fastcall GetID();
        int __fastcall GetData(int nID);	//50 ~ 200000 div
        bool __fastcall EnableZero(int nID);
        bool __fastcall DisableZero(int nID);
        double __fastcall GetKg(int nID);

};

#endif
