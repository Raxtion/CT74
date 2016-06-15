//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmMotorCheck.h"
#include "PCIM114.h"
#include "IniFile.h"
#include "MyPISODIO.h"
#include "MainThread.h"
#include "math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmMotorCheck *frmMotorCheck;
extern CMyPISODIO g_DIO;
extern CPCIM114 g_Motion;
extern CIniFile g_IniFile;
extern CMainThread *g_pMainThread;
//---------------------------------------------------------------------------
__fastcall TfrmMotorCheck::TfrmMotorCheck(TComponent* Owner)
    : TForm(Owner)
{
    m_nActiveAxis=0;
    m_nMoveMode=0;
    m_nSpeedMode=1;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMotorCheck::SetActiveAxis(unsigned int value)
{
    if(m_nActiveAxis != value) {
        m_nActiveAxis = value;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::Timer1Timer(TObject *Sender)
{
    AnsiString strPos,strUnit;

    switch(m_nActiveAxis)
    {
        //case Table_T:strUnit="°";break;
        case AXIS_X:
        case AXIS_Y:
        case AXIS_LC:
        case AXIS_FL:
        case AXIS_RL:
                strUnit="mm";
                break;
    }
    m_dCurrPos=g_Motion.GetActualPos(m_nActiveAxis);
    strPos.sprintf("%4.3f %s",m_dCurrPos,strUnit);
    labelPosition->Caption=strPos;

    //Axis Status
    if(g_Motion.GetStatus(m_nActiveAxis,PCIM114::PEL)) shapePL->Brush->Color=clLime;else shapePL->Brush->Color=clGreen;
    if(g_Motion.GetStatus(m_nActiveAxis,PCIM114::MEL)) shapeML->Brush->Color=clLime;else shapeML->Brush->Color=clGreen;
    if(g_Motion.GetStatus(m_nActiveAxis,PCIM114::ORG)) shapeOrg->Brush->Color=clLime;else shapeOrg->Brush->Color=clGreen;
    if(g_Motion.GetStatus(m_nActiveAxis,PCIM114::INP)) shapeInp->Brush->Color=clLime;else shapeInp->Brush->Color=clGreen;
    if(!g_Motion.GetStatus(m_nActiveAxis,PCIM114::ALM)) shapeAlarm->Brush->Color=clMaroon;else shapeAlarm->Brush->Color=clRed;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::btnStopClick(TObject *Sender)
{
    g_Motion.Stop(m_nActiveAxis);    
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::btnJogClick(TObject *Sender)
{
    m_nMoveMode=((TSpeedButton *)Sender)->Tag;
}
//--------------------------------------------------------------------------- 
void __fastcall TfrmMotorCheck::btnFastSpeedClick(TObject *Sender)
{
     m_nSpeedMode=((TSpeedButton *)Sender)->Tag;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::btnHomeClick(TObject *Sender)
{
    if (g_pMainThread->m_bIsHomeDone != true)
    {
        g_Motion.SetSpeed(m_nActiveAxis,0.1,0.1,10);
        g_Motion.AxisHome(m_nActiveAxis);
        g_Motion.WaitMotionDone(m_nActiveAxis,10000);
        g_Motion.SetActualPos(m_nActiveAxis,0);
        g_Motion.SetCommandPos(m_nActiveAxis,0);
    }
    else if (g_pMainThread->m_bIsHomeDone == true)
    {
        g_Motion.AbsMove(m_nActiveAxis, 0);
    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmMotorCheck::btnFWDMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    double dMovePitch,dMoveSpeed;

    switch(m_nMoveMode)
    {
    case 0:dMovePitch=0;break;
    case 1:dMovePitch=1;break;
    case 2:dMovePitch=0.1;break;
    case 3:dMovePitch=0.01;break;
    }

    switch(m_nSpeedMode)
    {
    case 0:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*2;break;
    case 1:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*1;break;
    case 2:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*0.5;break;
    case 3:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*0.1;break;
    }

    g_Motion.SetSpeed(m_nActiveAxis,g_IniFile.m_dACCSpeed[m_nActiveAxis],g_IniFile.m_dDECSpeed[m_nActiveAxis],dMoveSpeed);

    if (m_nActiveAxis == 4 || m_nActiveAxis == 5)
    {
        if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB))g_IniFile.m_nErrorCode = 51;
        else
        {
            if(dMovePitch==0) g_Motion.JogStart(m_nActiveAxis,true);
            else g_Motion.RelMove(m_nActiveAxis,dMovePitch);
        }
    }
    else if (m_nActiveAxis == 1)
    {
        if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[1]+1 || g_Motion.GetActualPos(AXIS_RL)>g_IniFile.m_dLamStop[0]+1) g_IniFile.m_nErrorCode = 69;
        else
        {
            if(dMovePitch==0) g_Motion.JogStart(m_nActiveAxis,true);
            else g_Motion.RelMove(m_nActiveAxis,dMovePitch);
        }
    }
    else
    {
        if(dMovePitch==0) g_Motion.JogStart(m_nActiveAxis,true);
        else g_Motion.RelMove(m_nActiveAxis,dMovePitch);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::btnRWDMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    double dMovePitch,dMoveSpeed;

    switch(m_nMoveMode)
    {
    case 0:dMovePitch=0;break;
    case 1:dMovePitch=1;break;
    case 2:dMovePitch=0.1;break;
    case 3:dMovePitch=0.01;break;
    }

    switch(m_nSpeedMode)
    {
    case 0:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*2;break;
    case 1:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*1;break;
    case 2:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*0.5;break;
    case 3:dMoveSpeed=g_IniFile.m_dJogSpeed[m_nActiveAxis]*0.1;break;
    }

    g_Motion.SetSpeed(m_nActiveAxis,g_IniFile.m_dACCSpeed[m_nActiveAxis],g_IniFile.m_dDECSpeed[m_nActiveAxis],dMoveSpeed);
    
    if (m_nActiveAxis == 4 || m_nActiveAxis == 5)
    {
        if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB))g_IniFile.m_nErrorCode = 51;
        else
        {
            if(dMovePitch==0) g_Motion.JogStart(m_nActiveAxis,false);
            else g_Motion.RelMove(m_nActiveAxis,-dMovePitch);
        }
    }
    else if (m_nActiveAxis == 1)
    {
        if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[1]+1 || g_Motion.GetActualPos(AXIS_RL)>g_IniFile.m_dLamStop[0]+1) g_IniFile.m_nErrorCode = 69;
        else
        {
            if(dMovePitch==0) g_Motion.JogStart(m_nActiveAxis,false);
            else g_Motion.RelMove(m_nActiveAxis,-dMovePitch);
        }
    }
    else
    {
        if(dMovePitch==0) g_Motion.JogStart(m_nActiveAxis,false);
        else g_Motion.RelMove(m_nActiveAxis,-dMovePitch);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::btnFWDMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    g_Motion.Stop(m_nActiveAxis);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::btnRWDMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    g_Motion.Stop(m_nActiveAxis);    
}
//---------------------------------------------------------------------------
void __fastcall TfrmMotorCheck::FormShow(TObject *Sender)
{
     switch(m_nActiveAxis)
    {
        /*
        case Table_T:
            Caption="馬達測試----平台角度";
            btnPitch1->Caption="1°";
            btnPitch01->Caption="0.1°";
            btnPitch001->Caption="0.01°";
                break;
        */
        
        case AXIS_X:
            Caption="馬達測試----Axis X";
            break;
        case AXIS_Y:
            Caption="馬達測試----Axis Y";
            break;
        case AXIS_LC:
            Caption="馬達測試----Axis LC";
            break;
        case AXIS_FL:
            Caption="馬達測試----Axis FL";
            break;
        case AXIS_RL:
            Caption="馬達測試----Axis RL";
            break;


    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::btnServoOnOffClick(TObject *Sender)
{
    g_Motion.ServoOn(m_nActiveAxis,btnServoOnOff->Down);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMotorCheck::BitBtn1Click(TObject *Sender)
{
    g_Motion.ServoOn(m_nActiveAxis,true);
}
//---------------------------------------------------------------------------




