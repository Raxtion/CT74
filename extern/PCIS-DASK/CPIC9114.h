//---------------------------------------------------------------------------
#include <vector>

#ifndef CPIC9114H
#define CPIC9114H
//---------------------------------------------------------------------------
//PCI-9114 HG AD_B_10_V, AD_B_1_V, AD_B_0_1_V, AD_B_0_01_V
//PCI-9114 DG AD_B_10_V, AD_B_5_V, AD_B_2_5_V, AD_B_1_25_V
class CPCI9114
{
private:
        bool m_bInitOK;
        short m_nCard;
        int m_nRange[32];
        double m_dRatio[32];    // mm/v

public:
        std::vector<double> m_vectData[32];
public:
        __fastcall CPCI9114();
        __fastcall ~CPCI9114();

        void __fastcall SetRangeDG(int nChannel,int nRange,double dMMperVolt);
        double __fastcall VRead(int nChannel,int nAvgCount=1);
        double __fastcall ReadReal(int nChannel,int nAvgCount=1);

        unsigned long __fastcall ReadDIPort();
        unsigned long __fastcall ReadDOPort();
        void __fastcall WriteDOPort(unsigned long nData);

        void __fastcall SetDO(int nBitNo,bool bOnOff);
        BOOL ReadDOBit(WORD nBitNo);
        void WriteDOBitON(WORD nBitNo);
        void WriteDOBitOFF(WORD nBitNo);
        BOOL ReadDIBit(WORD nBitNo);

        

};
#endif
