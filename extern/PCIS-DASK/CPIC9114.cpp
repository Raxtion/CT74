//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CPIC9114.h"
#include "dask.h"
#include "math.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
CPCI9114 g_AD;
//---------------------------------------------------------------------------
__fastcall CPCI9114::CPCI9114()
{

        m_bInitOK=true;
        m_nCard=Register_Card(PCI_9114DG, 0);

        if(m_nCard<0)
        {
                ShowMessage("Initial PCI-9114 Card Error!!");
                m_bInitOK=false;
        }

        for(int nIndex=0;nIndex<32;nIndex++)
        {
                m_nRange[nIndex]=AD_B_10_V;
                m_dRatio[nIndex]=5.0;           //5mm/v
        }


}
//---------------------------------------------------------------------------
__fastcall CPCI9114::~CPCI9114()
{
        if(m_nCard>0) Release_Card(m_nCard);
}
//---------------------------------------------------------------------------
void __fastcall CPCI9114::SetRangeDG(int nChannel,int nRange,double dMMperVolt)
{
        if( nChannel>31) return;
        m_dRatio[nChannel]=dMMperVolt;

        switch(nRange)
        {
                case 0:
                        m_nRange[nChannel]=AD_B_10_V;
                        break;
                case 1:
                         m_nRange[nChannel]=AD_B_5_V;
                        break;
                case 2:
                        m_nRange[nChannel]=AD_B_2_5_V;
                        break;
                case 3:
                        m_nRange[nChannel]=AD_B_1_25_V;
                        break;
                default:
                        m_nRange[nChannel]=AD_B_10_V;
        }

}
//---------------------------------------------------------------------------
double __fastcall CPCI9114::VRead(int nChannel,int nAvgCount)
{
        if(!m_bInitOK) return 0.0;

        m_vectData[nChannel].clear();
        double dAvgData=0;
        double Data;


        for(int nIndex=0;nIndex<nAvgCount;nIndex++)
        {
                if(AI_VReadChannel(m_nCard, nChannel, m_nRange[nChannel], &Data)==0)
                {
                        m_vectData[nChannel].push_back(Data);
                        dAvgData+=Data;
                }
        }
        if(m_vectData[nChannel].size()>0) dAvgData=dAvgData/m_vectData[nChannel].size();

        return dAvgData;
}
//---------------------------------------------------------------------------
BOOL CPCI9114::ReadDIBit(WORD  nBitNo)
{
    if (!m_bInitOK)
	return FALSE;
	    
    U32 nData=0;
    if(nBitNo<32) 
    {    	
    	DI_ReadPort(m_nCard,  0, &nData);
    }
    
    return (nData & (U32) pow(2, nBitNo));
}
//---------------------------------------------------------------------------
void CPCI9114::WriteDOBitOFF(WORD nBitNo)
{
    if (!m_bInitOK)
	return;   
    
    U32 nDataA;    
    DO_ReadPort(m_nCard, 0, &nDataA);

    
    if(nBitNo<32)
    {
    	nDataA &= ~(U32) pow(2, nBitNo);
        DO_WritePort(m_nCard, 0, nDataA);
    } 

}
//---------------------------------------------------------------------------
void CPCI9114::WriteDOBitON(WORD nBitNo)
{
    if (!m_bInitOK)
	return;
    
    U32 nDataA;
    DO_ReadPort(m_nCard, 0, &nDataA);

    
    if(nBitNo<32)
    {
    	nDataA |= (U32) pow(2, nBitNo);
        DO_WritePort(m_nCard, 0, nDataA);
    }
}
//---------------------------------------------------------------------------
BOOL CPCI9114::ReadDOBit(WORD	nBitNo)
{
    if (!m_bInitOK)
	return false;
	    
    U32 nDataA;
    DO_ReadPort(m_nCard, P7442_CH0, &nDataA);

    return ( nDataA & (U32) pow(2, nBitNo));
}
//---------------------------------------------------------------------------
void __fastcall CPCI9114::SetDO(int nBitNo,bool bOnOff)
{
        if(bOnOff) WriteDOBitON(nBitNo);
        else WriteDOBitOFF(nBitNo);
}
//---------------------------------------------------------------------------
unsigned long __fastcall CPCI9114::ReadDIPort()
{
        if (!m_bInitOK)
	return 0;

        U32 nData=0;
        DI_ReadPort(m_nCard, 0, &nData);
        return nData;
}
//---------------------------------------------------------------------------
unsigned long __fastcall CPCI9114::ReadDOPort()
{
         if (!m_bInitOK)
	return 0;

        U32 nData=0;
        DO_ReadPort(m_nCard, 0, &nData);
        return nData;
}
//---------------------------------------------------------------------------
void __fastcall CPCI9114::WriteDOPort(unsigned long nData)
{
        if (!m_bInitOK)
	return ;

        DO_WritePort(m_nCard, 0, nData);

        return ;
}
//---------------------------------------------------------------------------
double __fastcall CPCI9114::ReadReal(int nChannel,int nAvgCount)
{
        //return (VRead(nChannel,nAvgCount)-2.5) * m_dRatio[nChannel];    //10mA * 250 ohm=2.5v
        //return VRead(nChannel,nAvgCount)*5.0-15.0;            //+/- 10mm
        //return VRead(nChannel,nAvgCount)*2.5-7.5;               // +/- 5mm
        return VRead(nChannel,nAvgCount)*7.5+12.5;               // 35 +/- 15mm
}
