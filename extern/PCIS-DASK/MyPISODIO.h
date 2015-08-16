// MyPISODIO.h: interface for the CMyPISODIO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPISODIO_H__9653BDBD_4C90_4191_AC7E_F64E8723AA02__INCLUDED_)
#define AFX_MYPISODIO_H__9653BDBD_4C90_4191_AC7E_F64E8723AA02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vcl>

#define DI_TIME_OUT -1

#define INFINITE_TIME 60000

#include "DIODefine.h"
//-------------------------------------------------------------------------//

class CMyPISODIO
{
private:
    //WORD m_wTotalBoard, m_wInitialCode;
    //DWORD m_wGetAddrBase;   // the base address assigned for board
    //WORD m_wGetIrqNo;      // the IRQ Level assigned for board
    //WORD m_wGetSubVendor, m_wGetSubDevice, m_wGetSubAux, m_wGetSlotBus, m_wGetSlotDevice;
    WORD m_wOutput[8], m_wInput[8];
    BOOL m_bIOCardReady;
    
    int m_nCardID;

public:
        void __fastcall SetDO(int nBitNo,bool bOnOff);
        unsigned long __fastcall ReadDIPort(int nPort);
        unsigned long __fastcall ReadDOPort(int nPort);
        void __fastcall WriteDOPort(int nPort,unsigned long nData);

    BOOL WaitDIBitOFF(WORD nBitNo, UINT nTimeOut = 1000);
    BOOL WaitDIBitON(WORD nBitNo, UINT nTimeOut = 1000);
    BOOL ReadDOBit(WORD nBitNo);
    void WriteDOBitON(WORD nBitNo);
    void WriteDOBitOFF(WORD nBitNo);
    BOOL ReadDIBit(WORD nBitNo);
    BOOL InitCard();
    CMyPISODIO();
    virtual ~CMyPISODIO();


    void __fastcall StartLamp();
    void __fastcall StopLamp();
    void __fastcall ResetLamp();
    void __fastcall AlarmLamp();
};

#endif // !defined(AFX_MYPISODIO_H__9653BDBD_4C90_4191_AC7E_F64E8723AA02__INCLUDED_)
