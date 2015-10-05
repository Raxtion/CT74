//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmManual.h"
#include "MyPISODIO.h""
#include "C_GetTime.h" 
#include "PCIM114.h"
#include "IniFile.h"
#include "CPIC9114.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmManual *frmManual;

extern CMyPISODIO g_DIO;
extern CPCIM114 g_Motion;
extern CIniFile g_IniFile;

//---------------------------------------------------------------------------
__fastcall TfrmManual::TfrmManual(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmManual::Timer1Timer(TObject *Sender)
{


        for(int nIndex=0;nIndex<ControlCount;nIndex++)
        {
                if(Controls[nIndex]->ClassNameIs("TGroupBox"))
                {
                        for(int nIndex2=0;nIndex2<((TGroupBox *)Controls[nIndex])->ControlCount;nIndex2++)
                        {
                                if(((TGroupBox *)Controls[nIndex])->Controls[nIndex2]->ClassNameIs("TSpeedButton"))
                                {
                                        ((TGroupBox *)Controls[nIndex])->Controls[nIndex2]->Enabled=g_DIO.ReadDIBit(DI::SafetyDoor);
                                }
                        }
                }
        }

        Label2->Visible=!g_DIO.ReadDIBit(DI::SafetyDoor);
}
//---------------------------------------------------------------------------

void __fastcall TfrmManual::SpeedButton1Click(TObject *Sender)
{
        TSpeedButton *pBtn=(TSpeedButton *)Sender;
        pBtn->Enabled=false;

        double dPos[30]={0.0};

        dPos[0]=g_IniFile.m_dSafePos;

        dPos[1]=g_IniFile.m_dLCFrontPos;
        dPos[2]=g_IniFile.m_dLCRearPos;
        dPos[3]=g_IniFile.m_dLCEntryPos;

        dPos[4]=g_IniFile.m_dLamStop[0];
        dPos[5]=g_IniFile.m_dLamHeight[0]-g_IniFile.m_dLamThirdHeight[0]-g_IniFile.m_dLamSecondHeight[0];
        dPos[6]=g_IniFile.m_dLamHeight[0];
        dPos[7]=0.0;

        dPos[8]=g_IniFile.m_dLamStop[1];
        dPos[9]=g_IniFile.m_dLamVacHeight[1]-g_IniFile.m_dLamThirdHeight[1]-g_IniFile.m_dLamSecondHeight[1];
        dPos[10]=g_IniFile.m_dLamHeight[1];
        dPos[11]=0.0;

        dPos[12]=g_IniFile.m_dLoadCellPosX[1];
        dPos[13]=g_IniFile.m_dLoadCellPosY[1];
        dPos[14]=g_IniFile.m_dLaserUpPosX[1][0];
        dPos[15]=g_IniFile.m_dLaserUpPosY[1][0];
        dPos[16]=g_IniFile.m_dLaserDownPosX[1];
        dPos[17]=g_IniFile.m_dLaserDownPosY[1];

        dPos[18]=g_IniFile.m_dLoadCellPosX[0];
        dPos[19]=g_IniFile.m_dLoadCellPosY[0];
        dPos[20]=g_IniFile.m_dLaserUpPosX[0][0];
        dPos[21]=g_IniFile.m_dLaserUpPosY[0][0];
        dPos[22]=g_IniFile.m_dLaserDownPosX[0];
        dPos[23]=g_IniFile.m_dLaserDownPosY[0];

        if((!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB)) && pBtn->Tag!=0) g_IniFile.m_nErrorCode = 51;
        else if(g_DIO.ReadDIBit(DI::LamEntry1) || g_DIO.ReadDIBit(DI::LamEntry2)) g_IniFile.m_nErrorCode = 12;
        else if(!g_DIO.ReadDIBit(DI::LoadCellDown)) g_IniFile.m_nErrorCode=10;
        else
        switch(pBtn->Tag)
        {
                case 0:
                        g_Motion.AbsMove(AXIS_Y,dPos[pBtn->Tag]);
                        break;
                case 1:
                case 2:
                case 3:
                        g_Motion.AbsMove(AXIS_LC,dPos[pBtn->Tag]);
                        break;
                case 4:
                case 5:
                case 6:
                case 7:
                        g_Motion.AbsMove(AXIS_RL,dPos[pBtn->Tag]);
                        break;
                case 8:
                case 9:
                case 10:
                case 11:
                        g_Motion.AbsMove(AXIS_FL,dPos[pBtn->Tag]);
                        break;
                case 12:
                case 14:
                case 16:
                case 18:
                case 20:
                case 22:
                        g_Motion.AbsMove(AXIS_X,dPos[pBtn->Tag]);
                        g_Motion.AbsMove(AXIS_Y,dPos[pBtn->Tag+1]);
                        break;
                case 24:
                        break;
                default:
                        break;
        }

        //
        while(1)
        {
                Application->ProcessMessages();
                 if(g_Motion.IsMotionDone(AXIS_X) && g_Motion.IsMotionDone(AXIS_Y) && g_Motion.IsMotionDone(AXIS_LC)&& g_Motion.IsMotionDone(AXIS_FL)&& g_Motion.IsMotionDone(AXIS_RL)) break;
        }

        pBtn->Enabled=true;
}
//---------------------------------------------------------------------------


