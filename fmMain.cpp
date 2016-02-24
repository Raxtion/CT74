//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmMain.h"
#include "IniFile.h"
#include "PCIM114.h"
#include "fmIOCheck.h"
#include "fmChoiceMotor.h"
#include "fmMotorCheck.h"
#include "fmAD.h"
#include "dir.h"
#include "MyPISODIO.h"
#include "MainThread.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "fmMachinParam.h"
#include "fmManual.h"
#include "FrmProduct.h"
#include "PISODNM100.h"
#include "DeltaPLC.h"
#include "TA5Serial.h";

#include <algorithm>
#include <math.h>
#include "fmPassword.h"
#include "fmOffset.h"
#include "Sqlite3Interface.h"
#include "EQPXML.h"
#include "fmAccountManual.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
SQLITE3IF g_TempLog(0, "C:\\C74 Log\\TempLog");
SQLITE3IF g_ActionLog(1, "C:\\C74 Log\\ActionLog");
extern CIniFile g_IniFile;

extern CPCIM114 g_Motion;
extern CMyPISODIO g_DIO;
extern bool g_bStopMainThread;
extern CMainThread *g_pMainThread;
extern CPISODNM100 g_DNPort0;
extern CPISODNM100 g_DNPort1;
extern CDeltaPLC g_ModBus;
extern CTA5Serial g_Balance;
extern CEQPXML g_eqpXML;

//CGetRealTimeValueThread.h start
//---------------------------------------------------------------------------
class CGetRealTimeValueThread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall CGetRealTimeValueThread(bool CreateSuspended);
};

CGetRealTimeValueThread *g_pGetRealTimeValueThread;
bool g_bStopGetRealTimeValueThread = false;

//CGetRealTimeValueThread.cpp start
//---------------------------------------------------------------------------
__fastcall CGetRealTimeValueThread::CGetRealTimeValueThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall CGetRealTimeValueThread::Execute()
{
    g_bStopGetRealTimeValueThread = false;

    while (true)
	{
		if (g_bStopGetRealTimeValueThread) break;
        //--- renew Temperature and LaserValue
        g_ModBus.SetSV(1, g_IniFile.m_dLamTemp[0]);
        ::Sleep(100);
        g_ModBus.SetSV(2, g_IniFile.m_dLamTemp[1]);
        ::Sleep(100);
        g_pMainThread->m_dRearTempRealTime = g_ModBus.GetPV(1);
        ::Sleep(100);
        g_pMainThread->m_dFrontTempRealTime = g_ModBus.GetPV(2);
        ::Sleep(100);
        g_pMainThread->m_dDownLaserRealTime = g_ModBus.GetAnalogData(3, 0);
        ::Sleep(100);
        g_pMainThread->m_dUpperLaserRealTime = g_ModBus.GetAnalogData(3, 1);
        ::Sleep(100);
        g_pMainThread->m_dForntPressloseRealTime = g_ModBus.GetAnalogData(3, 2);
        ::Sleep(100);
        g_pMainThread->m_dRearPressloseRealTime = g_ModBus.GetAnalogData(3, 3);

        ::Sleep(100);
    }
}
//---------------------------------------------------------------------------

//TfmMain start
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}

//------------------------------------------------------------------------------------------------------------------------------
//按鍵功能


//Control btn
void __fastcall TfrmMain::btnStartPressCal0Click(TObject *Sender)
{

	checkMonitor->Checked = false;
    g_pMainThread->m_bIsManualFinish = false;

	if (btnStartPressCal0->Down || btnStartPressCal1->Down ||
		btnLaserUp1->Down || btnLaserUp0->Down ||
		btnLaserDown1->Down || btnLaserDown0->Down)
	{
		N1->Enabled = false;
		N6->Enabled = false;
		N9->Enabled = false;
		N12->Enabled = false;
	}
	else  SetPrivilege(m_nUserLevel);

	if (g_pMainThread->m_bStartPressCal[0] || g_pMainThread->m_bStartPressCal[1]) SetAllDevice();

	g_DIO.SetDO(DO::LoadCellValve,false);

	TSpeedButton *pBtn = (TSpeedButton *)Sender;

	if (checkRestartCal->Checked && pBtn->Down)
	{
	    g_pMainThread->m_nPressCalMoveIndex[0] = -1;
	    g_pMainThread->m_nLaserCalMoveIndex[0] = -1;

	    g_pMainThread->m_nPressCalMoveIndex[1] = -1;
	    g_pMainThread->m_nLaserCalMoveIndex[1] = -1;
    }
    if (cmbRange->ItemIndex==1)
    {
        g_pMainThread->m_nPressCalMoveIndex[0] = -1;
	    g_pMainThread->m_nLaserCalMoveIndex[0] = -1;

	    g_pMainThread->m_nPressCalMoveIndex[1] = -1;
	    g_pMainThread->m_nLaserCalMoveIndex[1] = -1;
                
        //start the btn also update the value of cmb
        g_pMainThread->m_nManualRange = cmbRange->ItemIndex;
        g_pMainThread->m_nManualFirstLoc = cmbFirstLoc->ItemIndex;
        g_pMainThread->m_nManualTimes = cmbTimes->Text.ToInt();
    }
    // update AutoRetry
    g_pMainThread->m_bAutoRetry = checkAutoRetry->Checked;

            
    g_pMainThread->m_bStartPressCal[0] = btnStartPressCal0->Down;
	g_pMainThread->m_bStartPressCal[1] = btnStartPressCal1->Down;
	g_pMainThread->m_bStartLaserUpCal[1] = btnLaserUp1->Down;
	g_pMainThread->m_bStartLaserUpCal[0] = btnLaserUp0->Down;
	g_pMainThread->m_bStartLaserDownCal[1] = btnLaserDown1->Down;
	g_pMainThread->m_bStartLaserDownCal[0] = btnLaserDown0->Down;

}
//---------------------------------------------------------------------------


//Menu
void __fastcall TfrmMain::N2Click(TObject *Sender)
{
	OpenDialog1->DefaultExt = "ini";
	if (OpenDialog1->Execute())
	{

        //先清除比例閥資料等2秒
        g_DNPort0.ClearOutData();
        g_DNPort0.WriteAllData();
        g_DNPort1.ClearOutData();
        g_DNPort1.WriteAllData();
        ::Sleep(2000);

        //再寫入產品參數
		Caption = OpenDialog1->FileName;
		g_IniFile.m_strLastFileName = Caption;

		g_IniFile.ProductFile(OpenDialog1->FileName.c_str(), true);

		SetAllDevice();

        Label22->Caption = g_IniFile.m_dLamTime[0];
        Label25->Caption = g_IniFile.m_dLamTemp[0];
        Label28->Caption = g_IniFile.m_dLamPress[0];
        Label17->Caption = g_IniFile.m_dLamTime[1];
        Label13->Caption = g_IniFile.m_dLamTemp[1];
        Label14->Caption = g_IniFile.m_dLamPress[1];
        TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
        GroupBox2->Caption = StrList->Strings[StrList->Count-1];
        delete StrList;

	}
    PaintBox1Paint(this);
    PaintBox2Paint(this);

    g_Motion.SetSoftLimit(4, g_IniFile.m_dPLimitF, g_IniFile.m_dNLimitF);
    g_Motion.SetSoftLimit(5, g_IniFile.m_dPLimitR, g_IniFile.m_dNLimitR);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N3Click(TObject *Sender)
{
    if (!FileExists("C:\\Product_Data\\")) _mkdir("C:\\Product_Data\\");
	g_IniFile.MachineFile(false);
	g_IniFile.ProductFile(Caption.c_str(), false);

    g_Motion.SetSoftLimit(4, g_IniFile.m_dPLimitF, g_IniFile.m_dNLimitF);
    g_Motion.SetSoftLimit(5, g_IniFile.m_dPLimitR, g_IniFile.m_dNLimitR);

	if (g_IniFile.m_nLanguageMode>0) ShowMessage("Save Done");
    else ShowMessage("存檔完畢");

    Label22->Caption = g_IniFile.m_dLamTime[0];
    Label25->Caption = g_IniFile.m_dLamTemp[0];
    Label28->Caption = g_IniFile.m_dLamPress[0];
    Label17->Caption = g_IniFile.m_dLamTime[1];
    Label13->Caption = g_IniFile.m_dLamTemp[1];
    Label14->Caption = g_IniFile.m_dLamPress[1];
    TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
    GroupBox2->Caption = StrList->Strings[StrList->Count-1];
    delete StrList;

    PaintBox1Paint(this);
    PaintBox2Paint(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N4Click(TObject *Sender)
{
	SaveDialog1->DefaultExt = "ini";
	if (SaveDialog1->Execute())
	{

        if (!FileExists("C:\\Product_Data\\")) _mkdir("C:\\Product_Data\\");
		g_IniFile.m_strLastFileName = SaveDialog1->FileName;
		Caption = SaveDialog1->FileName;
		g_IniFile.MachineFile(false);
		g_IniFile.ProductFile(SaveDialog1->FileName.c_str(), false);

		SetAllDevice();

        if (g_IniFile.m_nLanguageMode>0) ShowMessage("Save Done");
        else ShowMessage("存檔完畢");
        Label22->Caption = g_IniFile.m_dLamTime[0];
        Label25->Caption = g_IniFile.m_dLamTemp[0];
        Label28->Caption = g_IniFile.m_dLamPress[0];
        Label17->Caption = g_IniFile.m_dLamTime[1];
        Label13->Caption = g_IniFile.m_dLamTemp[1];
        Label14->Caption = g_IniFile.m_dLamPress[1];
        TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
        GroupBox2->Caption = StrList->Strings[StrList->Count-1];
        delete StrList;

	}
    PaintBox1Paint(this);
    PaintBox2Paint(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N5Click(TObject *Sender)
{
    if (g_IniFile.m_nLanguageMode>0)
    {
        if (Application->MessageBox("Ready To Quit?", "Confirm", MB_OKCANCEL) == IDOK)
	    {
		    if (Application->MessageBox("Save File Before Leave?", "Confirm", MB_OKCANCEL) == IDOK)
			N3Click(Sender);
		    exit(0);
	    }
    }
    else
    {
	    if (Application->MessageBox("是否要離開?", "注意", MB_OKCANCEL) == IDOK)
	    {
		    if (Application->MessageBox("離開前是否要存檔?", "注意", MB_OKCANCEL) == IDOK)
			N3Click(Sender);
		    exit(0);
	    }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N7Click(TObject *Sender)
{
	TfrmMachineParam *pMachineDlg = new TfrmMachineParam(this);

    //CreateCaptionFile(pMachineDlg);
    ReadCaptionFile(pMachineDlg, g_IniFile.m_nLanguageMode);

	AnsiString strCtl;
	bool bRead = true;

	//int nAxis[]={AXIS_X,AXIS_Y,AXIS_LC,AXIS_FL,AXIS_RL};

	for (int nIndex = 0; nIndex<8; nIndex++)
	{
		DDX_Float(bRead, g_IniFile.m_dACCSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editAcc%d", nIndex + 1)));
		DDX_Float(bRead, g_IniFile.m_dDECSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editDec%d", nIndex + 1)));
		DDX_Float(bRead, g_IniFile.m_dJogSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editJogSpeed%d", nIndex + 1)));
		DDX_Float(bRead, g_IniFile.m_dWorkSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editWorkSpeed%d", nIndex + 1)));
	}

	DDX_Float(bRead, g_IniFile.m_dSafePos, pMachineDlg->m_dSafePos);
        DDX_Float(bRead, g_IniFile.m_dUpperLaserAlarm, pMachineDlg->m_dUpperLaserAlarm);
        DDX_Float(bRead, g_IniFile.m_dDownLaserAlarm, pMachineDlg->m_dDownLaserAlarm);
    DDX_Check(bRead, g_IniFile.m_bForceEject, pMachineDlg->m_bForceEject);
    DDX_Check(bRead, g_IniFile.m_bIsUseCIM, pMachineDlg->m_bIsUseCIM);
    DDX_ComboBox(bRead, g_IniFile.m_nLanguageMode, pMachineDlg->m_cmbLanguage);

	DDX_Float(bRead, g_IniFile.m_dLCEntryPos, pMachineDlg->m_dLCEntryPos);
	DDX_Float(bRead, g_IniFile.m_dLCFrontPos, pMachineDlg->m_dLCFrontPos);
	DDX_Float(bRead, g_IniFile.m_dLCRearPos, pMachineDlg->m_dLCRearPos);

    DDX_Float(bRead, g_IniFile.m_dPLimitF, pMachineDlg->m_dPLimitF);
    DDX_Float(bRead, g_IniFile.m_dNLimitF, pMachineDlg->m_dNLimitF);
    DDX_Float(bRead, g_IniFile.m_dPLimitR, pMachineDlg->m_dPLimitR);
    DDX_Float(bRead, g_IniFile.m_dNLimitR, pMachineDlg->m_dNLimitR);

	if (pMachineDlg->ShowModal() == mrOk)
	{
		bRead = false;

		for (int nIndex = 0; nIndex<8; nIndex++)
		{
			DDX_Float(bRead, g_IniFile.m_dACCSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editAcc%d", nIndex + 1)));
			DDX_Float(bRead, g_IniFile.m_dDECSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editDec%d", nIndex + 1)));
			DDX_Float(bRead, g_IniFile.m_dJogSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editJogSpeed%d", nIndex + 1)));
			DDX_Float(bRead, g_IniFile.m_dWorkSpeed[nIndex], (TEdit *)pMachineDlg->FindComponent(strCtl.sprintf("editWorkSpeed%d", nIndex + 1)));
		}

		DDX_Float(bRead, g_IniFile.m_dSafePos, pMachineDlg->m_dSafePos);
                DDX_Float(bRead, g_IniFile.m_dUpperLaserAlarm, pMachineDlg->m_dUpperLaserAlarm);
                DDX_Float(bRead, g_IniFile.m_dDownLaserAlarm, pMachineDlg->m_dDownLaserAlarm);
        DDX_Check(bRead, g_IniFile.m_bForceEject, pMachineDlg->m_bForceEject);
        DDX_Check(bRead, g_IniFile.m_bIsUseCIM, pMachineDlg->m_bIsUseCIM);
        DDX_ComboBox(bRead, g_IniFile.m_nLanguageMode, pMachineDlg->m_cmbLanguage);

		DDX_Float(bRead, g_IniFile.m_dLCEntryPos, pMachineDlg->m_dLCEntryPos);
		DDX_Float(bRead, g_IniFile.m_dLCFrontPos, pMachineDlg->m_dLCFrontPos);
		DDX_Float(bRead, g_IniFile.m_dLCRearPos, pMachineDlg->m_dLCRearPos);

        DDX_Float(bRead, g_IniFile.m_dPLimitF, pMachineDlg->m_dPLimitF);
        DDX_Float(bRead, g_IniFile.m_dNLimitF, pMachineDlg->m_dNLimitF);
        DDX_Float(bRead, g_IniFile.m_dPLimitR, pMachineDlg->m_dPLimitR);
        DDX_Float(bRead, g_IniFile.m_dNLimitR, pMachineDlg->m_dNLimitR);

        g_Motion.SetSoftLimit(4, g_IniFile.m_dPLimitF, g_IniFile.m_dNLimitF);
        g_Motion.SetSoftLimit(5, g_IniFile.m_dPLimitR, g_IniFile.m_dNLimitR);

        //CreateCaptionFile(this);
        ReadCaptionFile(this, g_IniFile.m_nLanguageMode);
        //CreateCaptionFileTMainMenu(TfrmMain::MainMenu);
        ReadCaptionFileTMainMenu(MainMenu, g_IniFile.m_nLanguageMode);

        RenewRadioGroup(false);
        this->Repaint();
	}
    delete pMachineDlg;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N8Click(TObject *Sender)
{
	TfmProduct *pWnd = new TfmProduct(this);

    //CreateCaptionFile(pWnd);
    ReadCaptionFile(pWnd, g_IniFile.m_nLanguageMode);

	AnsiString strCtl;
	bool bRead = true;

	DDX_Int(bRead, g_IniFile.m_nCols, pWnd->m_nCols);
	DDX_Int(bRead, g_IniFile.m_nRows, pWnd->m_nRows);
	DDX_Float(bRead, g_IniFile.m_dColPitch, pWnd->m_dColPitch);
	DDX_Float(bRead, g_IniFile.m_dRowPitch, pWnd->m_dRowPitch);

	DDX_Float(bRead, g_IniFile.m_dLamHeight[0], pWnd->m_dLamHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamHeight[1], pWnd->m_dLamHeight1);
	DDX_Float(bRead, g_IniFile.m_dLamStop[0], pWnd->m_dLamStop0);
	DDX_Float(bRead, g_IniFile.m_dLamStop[1], pWnd->m_dLamStop1);
	DDX_Float(bRead, g_IniFile.m_dLamGetPos[0], pWnd->m_dLamGetPos0);
	DDX_Float(bRead, g_IniFile.m_dLamGetPos[1], pWnd->m_dLamGetPos1);
	DDX_Float(bRead, g_IniFile.m_dLamVacHeight[0], pWnd->m_dLamVacHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamVacHeight[1], pWnd->m_dLamVacHeight1);

    DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[0], pWnd->m_dLamSecondHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[1], pWnd->m_dLamSecondHeight1);
	DDX_Float(bRead, g_IniFile.m_dLamSecondTime[0], pWnd->m_dLamSecondTime0);
	DDX_Float(bRead, g_IniFile.m_dLamSecondTime[1], pWnd->m_dLamSecondTime1);
	DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[0], pWnd->m_dLamThirdHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[1], pWnd->m_dLamThirdHeight1);
	DDX_Float(bRead, g_IniFile.m_dLamThirdTime[0], pWnd->m_dLamThirdTime0);
	DDX_Float(bRead, g_IniFile.m_dLamThirdTime[1], pWnd->m_dLamThirdTime1);

    DDX_Int(bRead, g_IniFile.m_nAutoInterval, pWnd->m_nAutoInterval);
	DDX_Float(bRead, g_IniFile.m_dAutoStopRange, pWnd->m_dAutoStopRange);

	DDX_Float(bRead, g_IniFile.m_dLamTime[0], pWnd->m_dLamTime0);
	DDX_Float(bRead, g_IniFile.m_dLamTemp[0], pWnd->m_dLamTemp0);
	DDX_Float(bRead, g_IniFile.m_dLamPress[0], pWnd->m_dLamPress0);
	DDX_Float(bRead, g_IniFile.m_dLamTime[1], pWnd->m_dLamTime1);
	DDX_Float(bRead, g_IniFile.m_dLamTemp[1], pWnd->m_dLamTemp1);
	DDX_Float(bRead, g_IniFile.m_dLamPress[1], pWnd->m_dLamPress1);

	DDX_Float(bRead, g_IniFile.m_dLoadCellPosX[0], pWnd->m_dLoadCellPosX0);
	DDX_Float(bRead, g_IniFile.m_dLoadCellPosY[0], pWnd->m_dLoadCellPosY0);

	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][0], pWnd->m_dLaserUpPosX00);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][0], pWnd->m_dLaserUpPosY00);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][1], pWnd->m_dLaserUpPosX01);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][1], pWnd->m_dLaserUpPosY01);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][2], pWnd->m_dLaserUpPosX02);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][2], pWnd->m_dLaserUpPosY02);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][3], pWnd->m_dLaserUpPosX03);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][3], pWnd->m_dLaserUpPosY03);

	DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[0], pWnd->m_dLaserDownPosX0);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[0], pWnd->m_dLaserDownPosY0);

	DDX_Float(bRead, g_IniFile.m_dLoadCellPosX[1], pWnd->m_dLoadCellPosX1);
	DDX_Float(bRead, g_IniFile.m_dLoadCellPosY[1], pWnd->m_dLoadCellPosY1);

	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][0], pWnd->m_dLaserUpPosX10);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][0], pWnd->m_dLaserUpPosY10);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][1], pWnd->m_dLaserUpPosX11);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][1], pWnd->m_dLaserUpPosY11);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][2], pWnd->m_dLaserUpPosX12);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][2], pWnd->m_dLaserUpPosY12);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][3], pWnd->m_dLaserUpPosX13);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][3], pWnd->m_dLaserUpPosY13);

	DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[1], pWnd->m_dLaserDownPosX1);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[1], pWnd->m_dLaserDownPosY1);

	DDX_Radio(bRead, g_IniFile.m_nRailOption, pWnd->m_nRailOption);
	DDX_Check(bRead, g_IniFile.m_bNotLam, pWnd->m_bNotLam);

	DDX_Float(bRead, g_IniFile.m_dPressCalRange, pWnd->m_dPressCalRange);
	DDX_Float(bRead, g_IniFile.m_dPressCalTime, pWnd->m_dPressCalTime);

    DDX_Float(bRead, g_IniFile.m_dAutoRunTempRange, pWnd->m_dAutoRunTempRange);
    DDX_Float(bRead, g_IniFile.m_dVacDelayTime, pWnd->m_dVacDelayTime);

    DDX_ComboBox(bRead, g_IniFile.m_nHeadType, pWnd->m_cmbHeadType);
    DDX_String(bRead, g_IniFile.m_strHeadScal, pWnd->m_cmbHeadScal, g_IniFile.m_strHeadScals);
    DDX_String(bRead, g_IniFile.m_strModuleScal, pWnd->m_cmbModuleScal, g_IniFile.m_strModuleScals);
    DDX_ComboBox(bRead, g_IniFile.m_nVacummOn, pWnd->m_cmbVacummOn);
    DDX_ComboBox(bRead, g_IniFile.m_nPressCheck, pWnd->m_cmbPressCheck);
    DDX_ComboBox(bRead, g_IniFile.m_nDummyCheck, pWnd->m_cmbDummyCheck);
    DDX_String(bRead, g_IniFile.m_strModuleNum, pWnd->m_strModuleNum);

    DDX_Float(bRead, g_IniFile.m_dBLT[0], pWnd->m_dBLT1);
    DDX_Float(bRead, g_IniFile.m_dBLT[1], pWnd->m_dBLT2);
    DDX_Float(bRead, g_IniFile.m_dBLT[2], pWnd->m_dBLT3);
    DDX_Float(bRead, g_IniFile.m_dBLT[3], pWnd->m_dBLT4);
    DDX_Float(bRead, g_IniFile.m_dBLT[4], pWnd->m_dBLT5);
    DDX_Float(bRead, g_IniFile.m_dTilt[0], pWnd->m_dTilt1);
    DDX_Float(bRead, g_IniFile.m_dTilt[1], pWnd->m_dTilt2);
    DDX_Float(bRead, g_IniFile.m_dTilt[2], pWnd->m_dTilt3);
    DDX_Float(bRead, g_IniFile.m_dTilt[3], pWnd->m_dTilt4);
    DDX_Float(bRead, g_IniFile.m_dTilt[4], pWnd->m_dTilt5);
    DDX_Float(bRead, g_IniFile.m_dGap[0], pWnd->m_dGap1);
    DDX_Float(bRead, g_IniFile.m_dGap[1], pWnd->m_dGap2);
    DDX_Float(bRead, g_IniFile.m_dGap[2], pWnd->m_dGap3);
    DDX_Float(bRead, g_IniFile.m_dGap[3], pWnd->m_dGap4);
    DDX_Float(bRead, g_IniFile.m_dGap[4], pWnd->m_dGap5);

    DDX_Float(bRead, g_IniFile.m_dKeyTemp[0][0], pWnd->m_dKeyTemp01);
    DDX_Float(bRead, g_IniFile.m_dKeyTemp[0][1], pWnd->m_dKeyTemp02);
    DDX_Float(bRead, g_IniFile.m_dKeyTemp[0][2], pWnd->m_dKeyTemp03);
    DDX_Float(bRead, g_IniFile.m_dKeyTemp[1][0], pWnd->m_dKeyTemp11);
    DDX_Float(bRead, g_IniFile.m_dKeyTemp[1][1], pWnd->m_dKeyTemp12);
    DDX_Float(bRead, g_IniFile.m_dKeyTemp[1][2], pWnd->m_dKeyTemp13);

    DDX_Int(bRead, g_IniFile.m_nDownPercent, pWnd->m_nDownPercent);

    pWnd->m_strSetupEENum->Text = g_IniFile.m_strLogInENGAccount;

    if (g_IniFile.m_strLogInENGAccount != "Admin")
    {
        pWnd->PageControl1->Pages[1]->Enabled = false;
    }
    pWnd->PageControl1->ActivePageIndex = 0;

    while (1)
    {
        if (pWnd->ShowModal()== mrOk)
        {
            double SecondSpeed0 = pWnd->m_dLamSecondHeight0->Text.ToDouble()/pWnd->m_dLamSecondTime0->Text.ToDouble();
            double SecondSpeed1 = pWnd->m_dLamSecondHeight1->Text.ToDouble()/pWnd->m_dLamSecondTime1->Text.ToDouble();
            double ThirdSpeed0 =  pWnd->m_dLamThirdHeight0->Text.ToDouble()/pWnd->m_dLamThirdTime0->Text.ToDouble();
            double ThirdSpeed1 =  pWnd->m_dLamThirdHeight1->Text.ToDouble()/pWnd->m_dLamThirdTime1->Text.ToDouble();

            if ( SecondSpeed0 > 50 || SecondSpeed1 > 50)
            {
				Application->MessageBoxA("第二段速度不可超過50mm/s","Confirm",MB_OK);
            }
            else if (ThirdSpeed0 > SecondSpeed0 || ThirdSpeed1 > SecondSpeed1)
            {
                Application->MessageBoxA("第三段速度不可超過第二段速度","Confirm",MB_OK);
            }
            else if (pWnd->m_strSetupEENum->Text == "")
            {
                Application->MessageBoxA("SetupEE 編號不可空白","Confirm",MB_OK);
            }
            else if (pWnd->m_dAutoStopRange->Text.ToDouble() <= pWnd->m_dPressCalRange->Text.ToDouble())
            {
                Application->MessageBoxA("自動停機誤差不可以<=容許誤差","Confirm",MB_OK);
            }
            else
            {
                bRead = false;
                break;
            }
        }
        else
        {
            break;
        }
    }

    //if (pWnd->ShowModal() == mrOk)
    if (bRead == false)
	{
		bRead = false;

		DDX_Int(bRead, g_IniFile.m_nCols, pWnd->m_nCols);
		DDX_Int(bRead, g_IniFile.m_nRows, pWnd->m_nRows);
		DDX_Float(bRead, g_IniFile.m_dColPitch, pWnd->m_dColPitch);
		DDX_Float(bRead, g_IniFile.m_dRowPitch, pWnd->m_dRowPitch);

		DDX_Float(bRead, g_IniFile.m_dLamHeight[0], pWnd->m_dLamHeight0);
		DDX_Float(bRead, g_IniFile.m_dLamHeight[1], pWnd->m_dLamHeight1);
		DDX_Float(bRead, g_IniFile.m_dLamStop[0], pWnd->m_dLamStop0);
		DDX_Float(bRead, g_IniFile.m_dLamStop[1], pWnd->m_dLamStop1);
		DDX_Float(bRead, g_IniFile.m_dLamGetPos[0], pWnd->m_dLamGetPos0);
		DDX_Float(bRead, g_IniFile.m_dLamGetPos[1], pWnd->m_dLamGetPos1);
		DDX_Float(bRead, g_IniFile.m_dLamVacHeight[0], pWnd->m_dLamVacHeight0);
		DDX_Float(bRead, g_IniFile.m_dLamVacHeight[1], pWnd->m_dLamVacHeight1);

        DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[0], pWnd->m_dLamSecondHeight0);
	    DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[1], pWnd->m_dLamSecondHeight1);
	    DDX_Float(bRead, g_IniFile.m_dLamSecondTime[0], pWnd->m_dLamSecondTime0);
	    DDX_Float(bRead, g_IniFile.m_dLamSecondTime[1], pWnd->m_dLamSecondTime1);
	    DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[0], pWnd->m_dLamThirdHeight0);
	    DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[1], pWnd->m_dLamThirdHeight1);
	    DDX_Float(bRead, g_IniFile.m_dLamThirdTime[0], pWnd->m_dLamThirdTime0);
	    DDX_Float(bRead, g_IniFile.m_dLamThirdTime[1], pWnd->m_dLamThirdTime1);

        DDX_Int(bRead, g_IniFile.m_nAutoInterval, pWnd->m_nAutoInterval);
        DDX_Float(bRead, g_IniFile.m_dAutoStopRange, pWnd->m_dAutoStopRange);

		DDX_Float(bRead, g_IniFile.m_dLamTime[0], pWnd->m_dLamTime0);
		DDX_Float(bRead, g_IniFile.m_dLamTemp[0], pWnd->m_dLamTemp0);
		DDX_Float(bRead, g_IniFile.m_dLamPress[0], pWnd->m_dLamPress0);
		DDX_Float(bRead, g_IniFile.m_dLamTime[1], pWnd->m_dLamTime1);
		DDX_Float(bRead, g_IniFile.m_dLamTemp[1], pWnd->m_dLamTemp1);
		DDX_Float(bRead, g_IniFile.m_dLamPress[1], pWnd->m_dLamPress1);

		DDX_Float(bRead, g_IniFile.m_dLoadCellPosX[0], pWnd->m_dLoadCellPosX0);
		DDX_Float(bRead, g_IniFile.m_dLoadCellPosY[0], pWnd->m_dLoadCellPosY0);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][0], pWnd->m_dLaserUpPosX00);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][0], pWnd->m_dLaserUpPosY00);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][1], pWnd->m_dLaserUpPosX01);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][1], pWnd->m_dLaserUpPosY01);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][2], pWnd->m_dLaserUpPosX02);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][2], pWnd->m_dLaserUpPosY02);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[0][3], pWnd->m_dLaserUpPosX03);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[0][3], pWnd->m_dLaserUpPosY03);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[0], pWnd->m_dLaserDownPosX0);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[0], pWnd->m_dLaserDownPosY0);

		DDX_Float(bRead, g_IniFile.m_dLoadCellPosX[1], pWnd->m_dLoadCellPosX1);
		DDX_Float(bRead, g_IniFile.m_dLoadCellPosY[1], pWnd->m_dLoadCellPosY1);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][0], pWnd->m_dLaserUpPosX10);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][0], pWnd->m_dLaserUpPosY10);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][1], pWnd->m_dLaserUpPosX11);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][1], pWnd->m_dLaserUpPosY11);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][2], pWnd->m_dLaserUpPosX12);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][2], pWnd->m_dLaserUpPosY12);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosX[1][3], pWnd->m_dLaserUpPosX13);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosY[1][3], pWnd->m_dLaserUpPosY13);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[1], pWnd->m_dLaserDownPosX1);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[1], pWnd->m_dLaserDownPosY1);

		DDX_Radio(bRead, g_IniFile.m_nRailOption, pWnd->m_nRailOption);
		DDX_Check(bRead, g_IniFile.m_bNotLam, pWnd->m_bNotLam);

		DDX_Float(bRead, g_IniFile.m_dPressCalRange, pWnd->m_dPressCalRange);
		DDX_Float(bRead, g_IniFile.m_dPressCalTime, pWnd->m_dPressCalTime);

        DDX_Float(bRead, g_IniFile.m_dAutoRunTempRange, pWnd->m_dAutoRunTempRange);
        DDX_Float(bRead, g_IniFile.m_dVacDelayTime, pWnd->m_dVacDelayTime);

        DDX_ComboBox(bRead, g_IniFile.m_nHeadType, pWnd->m_cmbHeadType);
        DDX_String(bRead, g_IniFile.m_strHeadScal, pWnd->m_cmbHeadScal, g_IniFile.m_strHeadScals);
        DDX_String(bRead, g_IniFile.m_strModuleScal, pWnd->m_cmbModuleScal, g_IniFile.m_strModuleScals);
        DDX_ComboBox(bRead, g_IniFile.m_nVacummOn, pWnd->m_cmbVacummOn);
        DDX_ComboBox(bRead, g_IniFile.m_nPressCheck, pWnd->m_cmbPressCheck);
        DDX_ComboBox(bRead, g_IniFile.m_nDummyCheck, pWnd->m_cmbDummyCheck);
        DDX_String(bRead, g_IniFile.m_strModuleNum, pWnd->m_strModuleNum);

        DDX_Float(bRead, g_IniFile.m_dBLT[0], pWnd->m_dBLT1);
        DDX_Float(bRead, g_IniFile.m_dBLT[1], pWnd->m_dBLT2);
        DDX_Float(bRead, g_IniFile.m_dBLT[2], pWnd->m_dBLT3);
        DDX_Float(bRead, g_IniFile.m_dBLT[3], pWnd->m_dBLT4);
        DDX_Float(bRead, g_IniFile.m_dBLT[4], pWnd->m_dBLT5);
        DDX_Float(bRead, g_IniFile.m_dTilt[0], pWnd->m_dTilt1);
        DDX_Float(bRead, g_IniFile.m_dTilt[1], pWnd->m_dTilt2);
        DDX_Float(bRead, g_IniFile.m_dTilt[2], pWnd->m_dTilt3);
        DDX_Float(bRead, g_IniFile.m_dTilt[3], pWnd->m_dTilt4);
        DDX_Float(bRead, g_IniFile.m_dTilt[4], pWnd->m_dTilt5);
        DDX_Float(bRead, g_IniFile.m_dGap[0], pWnd->m_dGap1);
        DDX_Float(bRead, g_IniFile.m_dGap[1], pWnd->m_dGap2);
        DDX_Float(bRead, g_IniFile.m_dGap[2], pWnd->m_dGap3);
        DDX_Float(bRead, g_IniFile.m_dGap[3], pWnd->m_dGap4);
        DDX_Float(bRead, g_IniFile.m_dGap[4], pWnd->m_dGap5);

        DDX_Float(bRead, g_IniFile.m_dKeyTemp[0][0], pWnd->m_dKeyTemp01);
        DDX_Float(bRead, g_IniFile.m_dKeyTemp[0][1], pWnd->m_dKeyTemp02);
        DDX_Float(bRead, g_IniFile.m_dKeyTemp[0][2], pWnd->m_dKeyTemp03);
        DDX_Float(bRead, g_IniFile.m_dKeyTemp[1][0], pWnd->m_dKeyTemp11);
        DDX_Float(bRead, g_IniFile.m_dKeyTemp[1][1], pWnd->m_dKeyTemp12);
        DDX_Float(bRead, g_IniFile.m_dKeyTemp[1][2], pWnd->m_dKeyTemp13);

        DDX_Int(bRead, g_IniFile.m_nDownPercent, pWnd->m_nDownPercent);
        DDX_String(bRead, g_IniFile.m_strSetupEENum, pWnd->m_strSetupEENum);

        Label22->Caption = g_IniFile.m_dLamTime[0];
        Label25->Caption = g_IniFile.m_dLamTemp[0];
        Label28->Caption = g_IniFile.m_dLamPress[0];
        Label17->Caption = g_IniFile.m_dLamTime[1];
        Label13->Caption = g_IniFile.m_dLamTemp[1];
        Label14->Caption = g_IniFile.m_dLamPress[1];
        TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
        GroupBox2->Caption = StrList->Strings[StrList->Count-1];
        delete StrList;
	}

	SetAllDevice();
	RefreshImage();
	delete pWnd;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N14Click(TObject *Sender)
{
	TfrmOffser *pWnd = new TfrmOffser(this);

    //CreateCaptionFile(pWnd);
    ReadCaptionFile(pWnd, g_IniFile.m_nLanguageMode);

	AnsiString strCtl;
	bool bRead = true;

	int nAA;

	for (int nIndex = 0; nIndex<50; nIndex++)
	{
		DDX_Float(bRead, g_IniFile.m_dScaleOffsetFront[nIndex], (TEdit *)pWnd->FindComponent(strCtl.sprintf("Edit%d", nIndex + 51)));
		DDX_Float(bRead, g_IniFile.m_dScaleOffsetRear[nIndex], (TEdit *)pWnd->FindComponent(strCtl.sprintf("Edit%d", nIndex + 1)));
	}

	if (pWnd->ShowModal() == mrOk)
	{
		bRead = false;

		for (int nIndex = 0; nIndex<50; nIndex++)
		{
			DDX_Float(bRead, g_IniFile.m_dScaleOffsetFront[nIndex], (TEdit *)pWnd->FindComponent(strCtl.sprintf("Edit%d", nIndex + 51)));
			DDX_Float(bRead, g_IniFile.m_dScaleOffsetRear[nIndex], (TEdit *)pWnd->FindComponent(strCtl.sprintf("Edit%d", nIndex + 1)));
		}
	}

	delete pWnd;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::IOTest1Click(TObject *Sender)
{
	TIOCheckDlg *pWnd = new TIOCheckDlg(this);
	//CreateCaptionFile(pWnd);
	ReadCaptionFile(pWnd, g_IniFile.m_nLanguageMode);
	pWnd->ShowModal();

	delete pWnd;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::MotorTest1Click(TObject *Sender)
{

	TfrmChoiceMotor *pChoiceMotorDlg = new TfrmChoiceMotor(this);
	TfrmMotorCheck *pMotorCheckDlg;

    //CreateCaptionFile(pChoiceMotorDlg);
    ReadCaptionFile(pChoiceMotorDlg, g_IniFile.m_nLanguageMode);

    AnsiString sPath = g_IniFile.m_strApplicationPath;
    sPath = StringReplace(sPath, "\\exe\\", "",TReplaceFlags());

	while (pChoiceMotorDlg->ShowModal() != mrCancel)
	{
		pMotorCheckDlg = new TfrmMotorCheck(this);
                
        //CreateCaptionFile(pMotorCheckDlg);
        ReadCaptionFile(pMotorCheckDlg, g_IniFile.m_nLanguageMode);

        if (pChoiceMotorDlg->m_nSelectAxis == 0)
        {
                pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath+"\\bmp\\right.bmp");
                pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath+"\\bmp\\left.bmp");
        }
        if (pChoiceMotorDlg->m_nSelectAxis == 4 || pChoiceMotorDlg->m_nSelectAxis == 5)
        {
                pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath+"\\bmp\\down.bmp");
                pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath+"\\bmp\\up.bmp");
        }

		pMotorCheckDlg->m_nActiveAxis = pChoiceMotorDlg->m_nSelectAxis;
		pMotorCheckDlg->Caption = pChoiceMotorDlg->m_strCaption;
		pMotorCheckDlg->ShowModal();

		delete pMotorCheckDlg;
	}

	delete pChoiceMotorDlg;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N10Click(TObject *Sender)
{
	if (!g_pMainThread->m_bIsHomeDone)
	{
        if (g_IniFile.m_nLanguageMode>0) ShowMessage("Please Reset Machine");
        else ShowMessage("請先執行機台原點復歸");
		return;             //debug
    }
	SetManualSpeed(1.0);

	TfrmManual *pWnd = new TfrmManual(this);

    //CreateCaptionFile(pWnd);
    ReadCaptionFile(pWnd, g_IniFile.m_nLanguageMode);

	pWnd->ShowModal();

	delete pWnd;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N11Click(TObject *Sender)
{
	TfrmAD *frmAD = new TfrmAD(this);

    //CreateCaptionFile(frmAD);
    ReadCaptionFile(frmAD, g_IniFile.m_nLanguageMode);

	frmAD->ShowModal();

	delete frmAD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::OP1Click(TObject *Sender)
{
	SetPrivilege(0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Engineer1Click(TObject *Sender)
{
	TfrmPassword *pPwdDlg = new TfrmPassword(this);

    //CreateCaptionFile(pPwdDlg);
    ReadCaptionFile(pPwdDlg, g_IniFile.m_nLanguageMode);

    if (g_IniFile.m_nLanguageMode>0)
    {
        pPwdDlg->labelOldPassword->Caption = "Enter Password";
        pPwdDlg->labelAccount->Caption = "Enter Account";
    }
    else
    {
        pPwdDlg->labelOldPassword->Caption = "請輸入密碼";
        pPwdDlg->labelAccount->Caption = "請輸入帳號";
    }

	pPwdDlg->labelNewPassword->Visible = false;
	pPwdDlg->editNewPassword->Visible = false;

    while (1)
    {
        if (pPwdDlg->ShowModal()== mrOk)
        {
            AnsiString result = CheckAccount(pPwdDlg->editAccount->Text, pPwdDlg->editOldPassword->Text);
            if (result == "帳號錯誤") Application->MessageBox("帳號錯誤!!", "認證", MB_OK);
            else if (result == "密碼錯誤") Application->MessageBox("密碼錯誤!!", "認證", MB_OK);
            else
            {
                Application->MessageBox("登入成功!!", "認證", MB_OK);
                g_IniFile.m_strLogInENGAccount = pPwdDlg->editAccount->Text;
                m_nUserLevel = 1;
                break;
            }
        }
        else
        {
            g_IniFile.m_strLogInENGAccount = "OP";
            m_nUserLevel = 0;
            break;
        }
    }

	SetPrivilege(m_nUserLevel);

	delete pPwdDlg;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N13Click(TObject *Sender)
{
	TfrmPassword *pPwdDlg = new TfrmPassword(this);

    //CreateCaptionFile(pPwdDlg);
    ReadCaptionFile(pPwdDlg, g_IniFile.m_nLanguageMode);

    while (1)
    {
        if (pPwdDlg->ShowModal()== mrOk)
        {
            AnsiString result = ChangePWD(pPwdDlg->editAccount->Text, pPwdDlg->editOldPassword->Text, pPwdDlg->editNewPassword->Text);
            if (result == "修改失敗") Application->MessageBox("修改失敗!!", "認證", MB_OK);
            else
            {
                Application->MessageBox("修改成功!!", "認證", MB_OK);
                m_nUserLevel = 0;
                break;
            }
        }
        else
        {
            break;
        }
    }

	delete pPwdDlg;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Admin1Click(TObject *Sender)
{
	TfrmPassword *pPwdDlg = new TfrmPassword(this);

	if (g_IniFile.m_nLanguageMode>0) pPwdDlg->labelOldPassword->Caption = "Enter Password";
    else pPwdDlg->labelOldPassword->Caption = "請輸入密碼";
        
    pPwdDlg->labelAccount->Visible = false;
    pPwdDlg->editAccount->Visible = false;
	pPwdDlg->labelNewPassword->Visible = false;
	pPwdDlg->editNewPassword->Visible = false;
	if (pPwdDlg->ShowModal() == mrOk)
	{
		if (pPwdDlg->editOldPassword->Text == g_IniFile.m_strARTPassword) m_nUserLevel = 2;
		else { Application->MessageBox("密碼錯誤!!", "錯誤", MB_OK); m_nUserLevel = 0; }
	}
	SetPrivilege(m_nUserLevel);

	delete pPwdDlg;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N15Click(TObject *Sender)
{
    TfrmAccountManual *pAccountManual = new TfrmAccountManual(this);

    SQLITE3IF *g_AccountLog = new SQLITE3IF(2, "C:\\C74 Log\\AccountLog");

    AnsiString strQueryResult = g_AccountLog->queryAccountPass();
    pAccountManual->Memo1->Text = strQueryResult;

    if (pAccountManual->ShowModal() == mrOk)
	{
        AnsiString Result = g_AccountLog->updateAccountPass(pAccountManual->Memo1->Text);
        Application->MessageBox(Result.c_str(), "認證", MB_OK);
    }
    delete pAccountManual;
    delete g_AccountLog;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N17Click(TObject *Sender)
{
    if(Application->MessageBoxA("變更系統語系為 中文?","Confirm",MB_ICONQUESTION|MB_OKCANCEL)!=IDOK) return;
    g_IniFile.m_nLanguageMode = 0;
    //CreateCaptionFile(this);
    ReadCaptionFile(this, g_IniFile.m_nLanguageMode);
    //CreateCaptionFileTMainMenu(TfrmMain::MainMenu);
    ReadCaptionFileTMainMenu(MainMenu, g_IniFile.m_nLanguageMode);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N18Click(TObject *Sender)
{
    if(Application->MessageBoxA("Change Language to English?","Confirm",MB_ICONQUESTION|MB_OKCANCEL)!=IDOK) return;
    g_IniFile.m_nLanguageMode = 1;
    //CreateCaptionFile(this);
    ReadCaptionFile(this, g_IniFile.m_nLanguageMode);
    //CreateCaptionFileTMainMenu(TfrmMain::MainMenu);
    ReadCaptionFileTMainMenu(MainMenu, g_IniFile.m_nLanguageMode);
}
//---------------------------------------------------------------------------


//Other
void __fastcall TfrmMain::SpeedButton1Click(TObject *Sender)
{
	g_DIO.SetDO(DO::LoadCellValve, true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SpeedButton2Click(TObject *Sender)
{
	g_DIO.SetDO(DO::LoadCellValve, false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::radioPosOptionClick(TObject *Sender)
{
	if (radioPosOption->ItemIndex == 0)
	{
		SetAllDevice();
	}
}
//---------------------------------------------------------------------------


//Timer
void __fastcall TfrmMain::timerPressureTimer(TObject *Sender)
{
	timerPressure->Enabled = false;

	g_DNPort0.ReadAllData();
	g_DNPort1.ReadAllData();
        
	RefreshImage();

    //Label1->Caption = "目前溫度:" + FormatFloat("0.0", g_ModBus.GetPV(1)) + "度";
    //Label4->Caption = "目前溫度:" + FormatFloat("0.0", g_ModBus.GetPV(2)) + "度";

	if (checkMonitor->Checked)
	{
        Label7->Caption = "Load Cell:" + FormatFloat("0.00kg", g_Balance.GetKg(1));
		Label8->Caption = "Laser(上):" + FormatFloat("0.00", g_pMainThread->m_dUpperLaserRealTime) + "mm";
		Label9->Caption = "Laser(下):" + FormatFloat("0.00", g_pMainThread->m_dDownLaserRealTime) + "mm";
	}

    if (g_IniFile.m_nLanguageMode>0)
    {
        Label2->Caption = "Lam.RemainTime:" + FormatFloat("0", g_pMainThread->m_dLamTimer[1] / 1000.0) + "Sec";
        Label5->Caption = "Lam.RemainTime:" + FormatFloat("0", g_pMainThread->m_dLamTimer[0] / 1000.0) + "Sec";
    }
    else
    {
        Label2->Caption = "壓合剩餘時間:" + FormatFloat("0", g_pMainThread->m_dLamTimer[1] / 1000.0) + "秒";
        Label5->Caption = "壓合剩餘時間:" + FormatFloat("0", g_pMainThread->m_dLamTimer[0] / 1000.0) + "秒";
    }
	timerPressure->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Timer1Timer(TObject *Sender)
{
	static int nErrorCode = 0;

	Timer1->Enabled=false;

	if (tm1MSLogOut.timeDevEnd()>300000) SetPrivilege(0);
	//--Thread Log---
	if (g_pMainThread->m_listLog.size()>0)
	{
		AnsiString strLog = g_pMainThread->m_listLog.front();
		AddList(strLog);
		g_IniFile.AddLog(strLog.c_str(), strLog.Length());
		g_pMainThread->m_listLog.pop_front();
	}
    //--CIMMsg Log---
	if (g_eqpXML.m_strCIMMsgLog.size()>0)
	{
		AnsiString strLog = g_eqpXML.m_strCIMMsgLog.front();
		AddList(strLog);
		g_IniFile.AddLog(strLog.c_str(), strLog.Length());
		g_eqpXML.m_strCIMMsgLog.pop_front();
	}

	//---Error Code
	if (g_IniFile.m_nErrorCode>0 && (nErrorCode != g_IniFile.m_nErrorCode))
	{
		//if(m_pMessageDlg) 
		{
            //process in Machine
			AnsiString strMsg = g_IniFile.GetErrorString("SYSTEM", g_IniFile.m_nErrorCode);
			AddList(strMsg);
			g_IniFile.AddLog(strMsg.c_str(), strMsg.Length());
            //process in CIM
            AnsiString strMsgCIM = g_IniFile.GetErrorStringENG("SYSTEM", g_IniFile.m_nErrorCode);
            AnsiString strCode;
            strCode.sprintf("E%04d",g_IniFile.m_nErrorCode);
            g_eqpXML.SendAlarmMessage(strCode.c_str(), strMsgCIM.c_str());
		}
	}

	if (g_IniFile.m_nErrorCode <= 0 && (nErrorCode != g_IniFile.m_nErrorCode))
	{
		//if(m_pMessageDlg) m_pMessageDlg->ShowWindow(SW_HIDE);
	}

	if (g_DIO.ReadDOBit(DO::Buzzer)) listHistory->Color = clRed;
	else listHistory->Color = clBlack;

	//----Motion
	if (!g_pMainThread->m_bIsAutoMode)
	{
		//textPosX->Caption=FormatFloat("0.000 mm",g_Motion.GetActualPos(AXIS_X));
		//textPosY->Caption=FormatFloat("0.000 mm",g_Motion.GetActualPos(AXIS_Y));
		//textPosZ->Caption=FormatFloat("0.000 mm",g_Motion.GetActualPos(AXIS_Z));
	}

	//---Refresh
	if (g_pMainThread->m_bRefresh)
	{
		g_pMainThread->m_bRefresh = false;
	}

	nErrorCode = g_IniFile.m_nErrorCode;

	if (g_DIO.ReadDIBit(DI::YAxisSafePosA)) Shape1->Brush->Color = clLime;
	else Shape1->Brush->Color = clGreen;

	if (g_DIO.ReadDIBit(DI::YAxisSafePosB)) Shape2->Brush->Color = clLime;
	else Shape2->Brush->Color = clGreen;

    //---RealTime update the m_nPassBoatStart
    if (editBoatFinishStart->Text != "")g_pMainThread->m_nPassBoatStart = editBoatFinishStart->Text.ToInt();

    //---Detect temp. if fail then stop
    if (g_pMainThread->m_bIsAutoMode)
    {
        if (g_IniFile.m_nRailOption == 0)
        {
            if (fabs(g_IniFile.m_dLamTemp[0] - g_pMainThread->m_dRearTempRealTime)>g_IniFile.m_dAutoRunTempRange) g_IniFile.m_nErrorCode = 32;
            if (fabs(g_IniFile.m_dLamTemp[1] - g_pMainThread->m_dFrontTempRealTime)>g_IniFile.m_dAutoRunTempRange) g_IniFile.m_nErrorCode = 31;
        }
        else if (g_IniFile.m_nRailOption == 1)
        {
            if (fabs(g_IniFile.m_dLamTemp[1] - g_pMainThread->m_dFrontTempRealTime)>g_IniFile.m_dAutoRunTempRange) g_IniFile.m_nErrorCode = 31;
        }
        else if (g_IniFile.m_nRailOption == 2)
        {
            if (fabs(g_IniFile.m_dLamTemp[0] - g_pMainThread->m_dRearTempRealTime)>g_IniFile.m_dAutoRunTempRange) g_IniFile.m_nErrorCode = 32;
        }
        editBoatFinishNow->Text = (AnsiString)g_pMainThread->m_nPassBoatCount1;
        editBoatFinishStart->Enabled = false;
    }
    else editBoatFinishStart->Enabled = true;

    //---Detect Manual Mode processing End
    if(g_pMainThread->m_bIsManualFinish == true)
    {
        if (g_pMainThread->m_bStartPressCal[0]==false) btnStartPressCal0->Down = false;
	    if (g_pMainThread->m_bStartPressCal[1]==false) btnStartPressCal1->Down = false;
	    if (g_pMainThread->m_bStartLaserUpCal[1]==false) btnLaserUp1->Down = false;
	    if (g_pMainThread->m_bStartLaserUpCal[0]==false) btnLaserUp0->Down = false;
	    if (g_pMainThread->m_bStartLaserDownCal[1]==false) btnLaserDown1->Down = false;
	    if (g_pMainThread->m_bStartLaserDownCal[0]==false) btnLaserDown0->Down = false;
    }

    //---Renew UPH
    editUnitPerHour->Text = FormatFloat("0.000", float(g_pMainThread->m_dUnitPerHour1)+float(g_pMainThread->m_dUnitPerHour0));

    //---Renew CIM signal
    Shape3->Brush->Color = clLime;

    //---Auto active LC no function
    if (g_IniFile.m_nErrorCode == 54 || g_IniFile.m_nErrorCode == 55) checkStopLC->Checked = true;

    //--Add ActionLog---
	if (g_pMainThread->m_ActionLog.size()>0)
	{
        //g_ActionLog.open(1);
		AnsiString strLog = g_pMainThread->m_ActionLog.front();
        g_ActionLog.insertAction(strLog);
        //g_ActionLog.close();
		g_pMainThread->m_ActionLog.pop_front();
	}

    //--Idle Run Down---
    if (ServerCIM->Active == true && g_pMainThread->m_bIsHomeDone == true)
    {
        if (g_IniFile.m_nErrorCode > 0 && g_IniFile.m_nErrorCode < 1000 && g_eqpXML.m_EqpStatus !='D')
        {
            g_eqpXML.m_EqpStatus='D';
            g_eqpXML.SendEventReport("1");
        }
        else if (g_DIO.ReadDOBit(DO::StartBtnLamp) && g_eqpXML.m_EqpStatus !='R')
        {
            g_eqpXML.m_EqpStatus='R';
            g_eqpXML.SendEventReport("1");
        }
        else if (g_IniFile.m_nErrorCode == 0 && g_eqpXML.m_EqpStatus !='I' && !g_DIO.ReadDOBit(DO::StartBtnLamp))
        {
            g_eqpXML.m_EqpStatus='I';
            g_eqpXML.SendEventReport("1");
        }
    }

    //--CIM Remote Disable MainFromBtn---
    {
        radioPosOption->Enabled = !g_pMainThread->m_bIsAutoMode;
        checkStopLC->Enabled = !g_pMainThread->m_bIsAutoMode;
        SpeedButton1->Enabled = !g_pMainThread->m_bIsAutoMode;
        SpeedButton2->Enabled = !g_pMainThread->m_bIsAutoMode;
        GroupBox1->Enabled = !g_pMainThread->m_bIsAutoMode;
        btnStartPressCal0->Enabled = !g_pMainThread->m_bIsAutoMode;
        btnLaserUp0->Enabled = !g_pMainThread->m_bIsAutoMode;
        btnLaserDown0->Enabled = !g_pMainThread->m_bIsAutoMode;
        btnStartPressCal1->Enabled = !g_pMainThread->m_bIsAutoMode;
        btnLaserUp1->Enabled = !g_pMainThread->m_bIsAutoMode;
        btnLaserDown1->Enabled = !g_pMainThread->m_bIsAutoMode;
        btnHoming->Enabled = !g_pMainThread->m_bIsAutoMode;
        cmbRange->Enabled = !g_pMainThread->m_bIsAutoMode;
        cmbFirstLoc->Enabled = !g_pMainThread->m_bIsAutoMode;
        cmbTimes->Enabled = !g_pMainThread->m_bIsAutoMode;
    }

    //--Keep renew Privilege
    SetPrivilege(m_nUserLevel);

	Timer1->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Timer2Timer(TObject *Sender)
{
    static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);
    static C_GetTime tm2MS(EX_SCALE::TIME_1MS, false);

    Timer2->Enabled = false;

    //--- real time monitor temperature
    if (g_pMainThread->m_bIsManualFinish == true
        && btnLaserUp1->Down == false && btnLaserUp0->Down == false && btnLaserDown1->Down == false && btnLaserDown0->Down == false)
    {
        if (g_IniFile.m_nLanguageMode>0)
        {
            Label1->Caption = "Temperature:" + FormatFloat("0.0", g_pMainThread->m_dRearTempRealTime) + "℃ ";
            Label4->Caption = "Temperature:" + FormatFloat("0.0", g_pMainThread->m_dFrontTempRealTime) + "℃ ";
        }
        else
        {
		    Label1->Caption = "目前溫度:" + FormatFloat("0.0", g_pMainThread->m_dRearTempRealTime) + "度";
		    Label4->Caption = "目前溫度:" + FormatFloat("0.0", g_pMainThread->m_dFrontTempRealTime) + "度";
        }
    }

    //--- real time detect servermoter touch the soft limitation
    if (g_pMainThread->m_bIsHomeDone == true)
    {
        if (g_Motion.GetActualPos(0) < -0.1 || abs(g_Motion.GetActualPos(0) > 292.0)) g_IniFile.m_nErrorCode = 80;
        else if (g_Motion.GetActualPos(1) < -7.0 || abs(g_Motion.GetActualPos(1) > 583.0)) g_IniFile.m_nErrorCode = 81;
        else if (g_Motion.GetActualPos(2) < -2.9 || abs(g_Motion.GetActualPos(2) > 549.0)) g_IniFile.m_nErrorCode = 82;
        else if (g_Motion.GetActualPos(4) < g_IniFile.m_dNLimitF || abs(g_Motion.GetActualPos(4) > g_IniFile.m_dPLimitF)) g_IniFile.m_nErrorCode = 83;
        else if (g_Motion.GetActualPos(5) < g_IniFile.m_dNLimitR || abs(g_Motion.GetActualPos(5) > g_IniFile.m_dPLimitR)) g_IniFile.m_nErrorCode = 84;
    }

    //--- real time detect Front and Rear Gas Pressure leaky
    if (g_pMainThread->m_bIsAutoMode == true && g_pMainThread->m_bIsHomeDone == true)
    {
        //---Detect GassLeaky keep over 5 second then allarm
        if (g_pMainThread->m_dForntPressloseRealTime < 3.5 && !m_bLastGassLeakyFront) tm1MS.timeStart(5000);
        if (m_bLastGassLeakyFront && tm1MS.timeUp())
        {
        	g_pMainThread->m_listLog.push_back(FormatFloat("GassSenser(F) Value= 0.00", g_pMainThread->m_dForntPressloseRealTime));
            g_IniFile.m_nErrorCode = 85;
        }
        if (g_pMainThread->m_dForntPressloseRealTime < 3.5) m_bLastGassLeakyFront = true;
        else m_bLastGassLeakyFront = false;
        //---Detect GassLeaky keep over 5 second then allarm
        if (g_pMainThread->m_dRearPressloseRealTime < 3.5 && !m_bLastGassLeakyRear) tm2MS.timeStart(5000);
        if (m_bLastGassLeakyRear && tm2MS.timeUp())
        {
	        g_pMainThread->m_listLog.push_back(FormatFloat("GassSenser(R) Value= 0.00", g_pMainThread->m_dRearPressloseRealTime));
            g_IniFile.m_nErrorCode = 86;
        }
        if (g_pMainThread->m_dRearPressloseRealTime < 3.5) m_bLastGassLeakyRear = true;
        else m_bLastGassLeakyRear = false;
    }

    Timer2->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::Timer3Timer(TObject *Sender)
{
    Timer3->Enabled = false;

    AnsiString strNow;
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);
    strNow.sprintf("%04d_%02d_%02d",tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday);
    if (strNow != m_strDate)
    {
        g_TempLog.close();
        g_ActionLog.close();
        g_TempLog.open(0);
        g_ActionLog.open(1);
    }

    //--Add tempLog---
    g_TempLog.insertTemp();

    Timer3->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerRenewPainBoxTimer(TObject *Sender)
{
    timerRenewPainBox->Enabled = false;
    PaintBox1Paint(this);
    PaintBox2Paint(this);
    timerRenewPainBox->Enabled = true;
}
//---------------------------------------------------------------------------

//PaintBox
void __fastcall TfrmMain::PaintBox1Paint(TObject *Sender)
{
    static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);

	//CreateMapArray(PaintBox1->ClientRect, g_IniFile.m_nCols, g_IniFile.m_nRows,5,1,1);
	CreateMapArray(PaintBox1->ClientRect, 10, 5, 3, 1, 1);

	PaintBox1->Canvas->Pen->Color = clLime;
	PaintBox1->Canvas->Brush->Color = clGreen;
	for (int nIndex = 0; nIndex<m_vectRect.size(); nIndex++)
	{
		/*
		if( > normal ) PaintBox1->Canvas->Brush->Color=clRed;
		else if( <normal ) PaintBox1->Canvas->Brush->Color=clYellow;
		else PaintBox1->Canvas->Brush->Color=clGreen;
		*/
		int nCol = nIndex % 10;
		int nRow = nIndex / 10;

		if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows)
		{
			if (nIndex == g_pMainThread->m_nPressCalMoveIndex[1] && btnStartPressCal1->Down) PaintBox1->Canvas->Brush->Color = clYellow;
			else if (nIndex == g_pMainThread->m_nLaserCalMoveIndex[1] && btnLaserUp1->Down) PaintBox1->Canvas->Brush->Color = clYellow;
			else if (nIndex == g_pMainThread->m_nLaserCalMoveIndex[1] && btnLaserDown1->Down) PaintBox1->Canvas->Brush->Color = clYellow;
            else if (g_pMainThread->m_dFrontUpperLaserDiff[nIndex][0] > g_IniFile.m_dUpperLaserAlarm || g_pMainThread->m_dFrontDownLaserDiff[nIndex][0] > g_IniFile.m_dDownLaserAlarm) PaintBox1->Canvas->Brush->Color = clRed;
			else PaintBox1->Canvas->Brush->Color = clGreen;
		}
		else PaintBox1->Canvas->Brush->Color = clGray;

		PaintBox1->Canvas->Rectangle(m_vectRect[nIndex]);

        double dGetSetKgValue = g_DNPort0.GetSetKg(nIndex);
        double dGetKgValue = g_DNPort0.GetKg(nIndex);

		int nTextHeight = PaintBox1->Canvas->TextHeight("1");
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1, FormatFloat("(0)", nIndex + 1)); //+ FormatFloat("0.00kg ", dGetSetKgValue) + FormatFloat("0.00kg", dGetKgValue));
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 1, FormatFloat("0.00mm ", g_pMainThread->m_dFrontUpperLaser[nIndex][0]) + FormatFloat("0.00mm", g_pMainThread->m_dFrontUpperLaser[nIndex][1]));
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 2, FormatFloat("0.00mm ", g_pMainThread->m_dFrontUpperLaser[nIndex][2]) + FormatFloat("0.00mm", g_pMainThread->m_dFrontUpperLaser[nIndex][3]));
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 3, FormatFloat("0.00mm ", g_pMainThread->m_dFrontUpperLaserDiff[nIndex][0]) + FormatFloat("0.00kg", g_pMainThread->m_dFrontPressCal[nIndex]));
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 4, FormatFloat("0.00mm ", g_pMainThread->m_dFrontDownLaser[nIndex][0]) + FormatFloat("0.00mm ", g_pMainThread->m_dFrontDownLaserDiff[nIndex][0]));

        if (g_pMainThread->m_bIsHomeDone == true && (g_pMainThread->m_bStartPressCal[0] == false && g_pMainThread->m_bStartPressCal[1] == false))
        {
            //---Detect GassPusherIO are different 5 second then allarm
            if (fabs(dGetSetKgValue-dGetKgValue) > 0.1 && !m_bLastPusherIOErrorFront) tm1MS.timeStart(5000);
            if (m_bLastPusherIOErrorFront && tm1MS.timeUp())
            {
            	g_IniFile.m_nErrorCode = 87;
            	PaintBox1->Canvas->Brush->Color = clRed;
            }
            if (fabs(dGetSetKgValue-dGetKgValue) > 0.1) m_bLastPusherIOErrorFront = true;
            else m_bLastPusherIOErrorFront = false;
        }
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PaintBox2Paint(TObject *Sender)
{
    static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);
	//CreateMapArray(PaintBox1->ClientRect, g_IniFile.m_nCols, g_IniFile.m_nRows,5,1,1);
	CreateMapArray(PaintBox2->ClientRect, 10, 5, 3, 1, 1);

	PaintBox2->Canvas->Pen->Color = clLime;
	PaintBox2->Canvas->Brush->Color = clGreen;
	for (int nIndex = 0; nIndex<m_vectRect.size(); nIndex++)
	{
		/*
		if( > normal ) PaintBox1->Canvas->Brush->Color=clRed;
		else if( <normal ) PaintBox1->Canvas->Brush->Color=clYellow;
		else PaintBox1->Canvas->Brush->Color=clGreen;
		*/
		int nCol = nIndex % 10;
		int nRow = nIndex / 10;

		if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows)
		{

			if (nIndex == g_pMainThread->m_nPressCalMoveIndex[0] && btnStartPressCal0->Down) PaintBox2->Canvas->Brush->Color = clYellow;
			else if (nIndex == g_pMainThread->m_nLaserCalMoveIndex[0] && btnLaserUp0->Down) PaintBox2->Canvas->Brush->Color = clYellow;
			else if (nIndex == g_pMainThread->m_nLaserCalMoveIndex[0] && btnLaserDown0->Down) PaintBox2->Canvas->Brush->Color = clYellow;
            else if (g_pMainThread->m_dRearUpperLaserDiff[nIndex][0] > g_IniFile.m_dUpperLaserAlarm || g_pMainThread->m_dRearDownLaserDiff[nIndex][0] > g_IniFile.m_dDownLaserAlarm) PaintBox2->Canvas->Brush->Color = clRed;
			else PaintBox2->Canvas->Brush->Color = clGreen;
		}
		else PaintBox2->Canvas->Brush->Color = clGray;

		PaintBox2->Canvas->Rectangle(m_vectRect[nIndex]);

        double dGetSetKgValue = g_DNPort1.GetSetKg(nIndex);
        double dGetKgValue = g_DNPort1.GetKg(nIndex);

		int nTextHeight = PaintBox2->Canvas->TextHeight("1");
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1, FormatFloat("(0)", nIndex + 1)); //+ FormatFloat("0.00kg", dGetSetKgValue) + FormatFloat("0.00kg", dGetKgValue));
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 1, FormatFloat("0.00mm ", g_pMainThread->m_dRearUpperLaser[nIndex][0]) + FormatFloat("0.00mm", g_pMainThread->m_dRearUpperLaser[nIndex][1]));
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 2, FormatFloat("0.00mm ", g_pMainThread->m_dRearUpperLaser[nIndex][2]) + FormatFloat("0.00mm", g_pMainThread->m_dRearUpperLaser[nIndex][3]));
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 3, FormatFloat("0.00mm ", g_pMainThread->m_dRearUpperLaserDiff[nIndex][0]) + FormatFloat("0.00Kg", g_pMainThread->m_dRearPressCal[nIndex]));
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 4, FormatFloat("0.00mm ", g_pMainThread->m_dRearDownLaser[nIndex][0]) + FormatFloat("0.00mm ", g_pMainThread->m_dRearDownLaserDiff[nIndex][0]));

        if (g_pMainThread->m_bIsHomeDone == true && (g_pMainThread->m_bStartPressCal[0] == false && g_pMainThread->m_bStartPressCal[1] == false))
        {
            //---Detect GassPusherIO are different 5 second then allarm
            if (fabs(dGetSetKgValue-dGetKgValue) > 0.1 && !m_bLastPusherIOErrorRear) tm1MS.timeStart(5000);
            if (m_bLastPusherIOErrorRear && tm1MS.timeUp())
            {
            	g_IniFile.m_nErrorCode = 87;
            	PaintBox1->Canvas->Brush->Color = clRed;
            }
            if (fabs(dGetSetKgValue-dGetKgValue) > 0.1) m_bLastPusherIOErrorRear = true;
            else m_bLastPusherIOErrorRear = false;
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PaintBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (radioPosOption->ItemIndex == 0) return;

	if (!g_pMainThread->m_bIsHomeDone)
	{
        if (g_IniFile.m_nLanguageMode>0) ShowMessage("Please Reset Machine");
        else ShowMessage("請先執行機台原點復歸");
		return;
	}
	else if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[0] || g_Motion.GetActualPos(AXIS_RL)> g_IniFile.m_dLamStop[1])
	{
		g_IniFile.m_nErrorCode = 69;
		return;
	}
	else if (!g_DIO.ReadDIBit(DI::LoadCellDown))
	{
		g_IniFile.m_nErrorCode = 10;
		return;
	}

	int nFront = ((TPaintBox *)Sender)->Tag;

	double dStartX = 0;
	double dStartY = 0;

	switch (radioPosOption->ItemIndex)
	{
	case 1:
		dStartX = g_IniFile.m_dLoadCellPosX[nFront];
		dStartY = g_IniFile.m_dLoadCellPosY[nFront];
		break;
	case 2:
		dStartX = g_IniFile.m_dLaserUpPosX[nFront][0];
		dStartY = g_IniFile.m_dLaserUpPosY[nFront][0];
		break;
	case 3:
		dStartX = g_IniFile.m_dLaserDownPosX[nFront];
		dStartY = g_IniFile.m_dLaserDownPosY[nFront];
		break;
	}

	for (int nIndex = 0; nIndex<m_vectRect.size(); nIndex++)
	{
		if (PtInRect(m_vectRect[nIndex], TPoint(X, Y)))
		{
			if (radioPosOption->ItemIndex<4)
			{
                AnsiString pString;
                if (g_IniFile.m_nLanguageMode>0) pString = "XY Moving To...";
                else pString = "XY 軸即將 移動至";
				if (MessageDlg(pString + IntToStr(nIndex + 1),
					mtInformation, TMsgDlgButtons() << mbOK << mbCancel, 0) == mrCancel) return;

				int nCol = nIndex % 10;
				int nRow = nIndex / 10;

				if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows)
				{
					g_Motion.AbsMove(AXIS_X, dStartX + nCol*g_IniFile.m_dColPitch);
					g_Motion.AbsMove(AXIS_Y, dStartY - nRow*g_IniFile.m_dRowPitch);
				}

				break;
			}
			else if (radioPosOption->ItemIndex == 4)
			{
				if (nFront)
				{
					g_DNPort0.ClearOutData();
					g_DNPort0.SetKg(nIndex, g_IniFile.m_dLamPress[1]);
					g_DNPort0.WriteAllData();
				}
				else
				{
					g_DNPort1.ClearOutData();
					g_DNPort1.SetKg(nIndex, g_IniFile.m_dLamPress[0]);
					g_DNPort1.WriteAllData();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------
//共用function

void __fastcall TfrmMain::AddList(AnsiString strMessage)
{
	listHistory->AddItem(TimeToStr(Time()) + "-->" + strMessage, NULL);
	listHistory->ItemIndex = listHistory->Count - 1;

	g_IniFile.AddLog(strMessage.c_str(), strMessage.Length());
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SetPrivilege(int nLevel)
{
	m_nUserLevel = nLevel;

	switch (nLevel)
	{
	case 0:
		N1->Enabled = true;
		N5->Enabled = false;
		N6->Enabled = false;
		N9->Enabled = false;
		N12->Enabled = true;
        N15->Enabled = false;
        g_IniFile.m_strLogInENGAccount = "OP";
		break;
	case 1:
		N1->Enabled = true;
		N5->Enabled = true;
		N6->Enabled = true;
		N9->Enabled = true;
		N12->Enabled = true;
        N15->Enabled = false;
        N7->Enabled = false;
        N14->Enabled = false;
        N11->Enabled = false;

		break;
	case 2:
		N1->Enabled = true;
		N5->Enabled = true;
		N6->Enabled = true;
		N9->Enabled = true;
		N12->Enabled = true;
        N15->Enabled = true;
        N7->Enabled = true;
        N14->Enabled = true;
        N11->Enabled = true;
        g_IniFile.m_strLogInENGAccount = "Admin";
		break;
	}

	if (nLevel>0) tm1MSLogOut.timeDevStart();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SetManualSpeed(double dRatio)
{
	for (int nIndex = 0; nIndex<5; nIndex++)
		g_Motion.SetSpeed(nIndex, g_IniFile.m_dACCSpeed[nIndex], g_IniFile.m_dDECSpeed[nIndex], g_IniFile.m_dJogSpeed[nIndex] * dRatio);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ReadCaptionFileTMainMenu(TMainMenu *pMainMenu, int nLanguage)
{
    AnsiString strFile;
	TIniFile *pIniFile;

    if (nLanguage>0) strFile.sprintf("%s%s", "C:\\", "Caption_C74_ENG.ini");
	else strFile.sprintf("%s%s", "C:\\", "Caption_C74.ini");

    const char *pSection = pMainMenu->Name.c_str();

    pIniFile = new TIniFile(strFile);

	TStringList *TempList = new TStringList;

	pIniFile->ReadSection(pSection, TempList);

	AnsiString strClass;
	AnsiString strName;
	AnsiString strValue;

    for (int nIndex = 0; nIndex<TempList->Count; nIndex++)
	{
        strName = TempList->Strings[nIndex];
		strValue = pIniFile->ReadString(pSection, strName, "NA");

        for (int nIndex2 = 0; nIndex2<((TMenuItem *)pMainMenu->Items)->Count; nIndex2++)
        {
            if (((TMenuItem *)pMainMenu->Items)->Items[nIndex2]->Name == strName)
            {
				strClass = ((TMenuItem *)pMainMenu->Items)->Items[nIndex2]->ClassName();
				if (strClass == "TMenuItem") (((TMenuItem *)pMainMenu->Items)->Items[nIndex2])->Caption = strValue;
            }

            TMenuItem *pTopMenu = (TMenuItem *)pMainMenu->Items;

            for (int nIndex3 = 0; nIndex3<((TMenuItem *)pTopMenu->Items[nIndex2])->Count; nIndex3++)
            {
                if (((TMenuItem *)pTopMenu->Items[nIndex2])->Items[nIndex3]->Name == strName)
                {
                    strClass = ((TMenuItem *)pTopMenu->Items[nIndex2])->Items[nIndex3]->ClassName();
                    if (strClass == "TMenuItem") (((TMenuItem *)pTopMenu->Items[nIndex2])->Items[nIndex3])->Caption = strValue;
                }
            }
        }
	}
	delete TempList;
	delete pIniFile;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ReadCaptionFile(TForm *pForm, int nLanguage)
{
	AnsiString strFile;
	TIniFile *pIniFile;

	if (nLanguage>0) strFile.sprintf("%s%s", "C:\\", "Caption_C74_ENG.ini");
	else strFile.sprintf("%s%s", "C:\\", "Caption_C74.ini");

	const char *pSection = pForm->Name.c_str();

	pIniFile = new TIniFile(strFile);

	TStringList *TempList = new TStringList;

	pIniFile->ReadSection(pSection, TempList);

	AnsiString strClass;
	AnsiString strName;
	AnsiString strValue;

	for (int nIndex = 0; nIndex<TempList->Count; nIndex++)
	{
		strName = TempList->Strings[nIndex];
		strValue = pIniFile->ReadString(pSection, strName, "NA");

		if (!pForm->FindComponent(strName)) continue;
		strClass = pForm->FindComponent(strName)->ClassName();

		if (strClass == "TLabel") ((TLabel *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TToolButton") ((TToolButton *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TSpeedButton") ((TSpeedButton *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TBitBtn") ((TBitBtn *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TGroupBox") ((TGroupBox *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TCheckBox") ((TCheckBox *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TStaticText") ((TStaticText *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TButton") ((TButton *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TTabSheet") ((TTabSheet *)pForm->FindComponent(strName))->Caption = strValue;
		else if (strClass == "TPanel") ((TPanel *)pForm->FindComponent(strName))->Caption = strValue;
        else if (strClass == "TRadioGroup") ((TRadioGroup *)pForm->FindComponent(strName))->Caption = strValue;
	}

	delete TempList;
	delete pIniFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::CreateMapArray(TRect &rect, int nPackageX, int nPackageY, int nDeviceGap, int nSectX, int nSectY)
{
	const int nGap = 0;

	if (nSectX <= 0 || nPackageX <= 0 || nSectY <= 0 || nPackageY <= 0) return;

	//m_nLeftColCnt=nPackageX*nSectX;
	//m_nLeftRowCnt=nPackageY*nSectY;
	int nPitchX;
	int nPitchY;

	nPitchX = (rect.Width() - nDeviceGap*(nPackageX - 1)) / (nSectX*nPackageX);
	nPitchY = (rect.Height() - nDeviceGap*(nPackageY - 1)) / (nSectY*nPackageY);

	m_vectRect.clear();

	//for(int nY=0;nY<nSectY*nPackageY;nY++)
	for (int nY = nSectY*nPackageY - 1; nY >= 0; nY--)
		//for(int nX=0;nX<nSectX*nPackageX;nX++)
		for (int nX = nSectX*nPackageX - 1; nX >= 0; nX--)
		{
			m_vectRect.push_back(TRect(rect.right - nPitchX*nX - nPitchX - nGap - (nX / nSectX)*nDeviceGap, rect.top + nPitchY*nY + nGap + (nY / nSectY)*nDeviceGap,
				rect.right - nPitchX*nX - nGap - (nX / nSectX)*nDeviceGap, rect.top + nPitchY*nY + nPitchY - nGap + (nY / nSectY)*nDeviceGap));
		}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::RefreshImage()
{
	RECT tRect;
	tRect.left = PaintBox1->ClientRect.left + PaintBox1->Left;
	tRect.right = PaintBox1->ClientRect.right;
	tRect.top = PaintBox1->ClientRect.top + PaintBox1->Top;
	tRect.bottom = PaintBox1->ClientRect.bottom;

	::InvalidateRect(Handle, &tRect, false);

	tRect.left = PaintBox2->ClientRect.left + PaintBox2->Left;
	tRect.right = PaintBox2->ClientRect.right;
	tRect.top = PaintBox2->ClientRect.top + PaintBox2->Top;
	tRect.bottom = PaintBox2->ClientRect.bottom;

	::InvalidateRect(Handle, &tRect, false);

	//PaintBox1->Invalidate();
	//PaintBox2->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SetAllDevice()
{

	for (int nIndex = 0; nIndex<50; nIndex++)
	{
		int nCol = nIndex % 10;
		int nRow = nIndex / 10;

		if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows &&
			(g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 1))
			g_DNPort0.SetKg(nIndex, g_IniFile.m_dLamPress[1] + g_IniFile.m_dScaleOffsetFront[nIndex]);
		else  g_DNPort0.SetKg(nIndex, 0);
	}
	g_DNPort0.WriteAllData();

	for (int nIndex = 0; nIndex<50; nIndex++)
	{
		int nCol = nIndex % 10;
		int nRow = nIndex / 10;

		if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows &&
			(g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 2))
			g_DNPort1.SetKg(nIndex, g_IniFile.m_dLamPress[0] + g_IniFile.m_dScaleOffsetRear[nIndex]);
		else  g_DNPort1.SetKg(nIndex, 0);
	}
	g_DNPort1.WriteAllData();

	g_Balance.SetXY(g_IniFile.m_dLoadCellX[0], g_IniFile.m_dLoadCellX[1], g_IniFile.m_dLoadCellY[0], g_IniFile.m_dLoadCellY[1]);

}
//---------------------------------------------------------------------------


/*
void __fastcall TfrmMain::CreateCaptionFileTMainMenu(TMainMenu *pMainMenu)
{
    AnsiString strFile;
	TIniFile *pIniFile;

    strFile.sprintf("%s%s", "C:\\", "Caption_C74.ini");
	pIniFile = new TIniFile(strFile);

    const char *Section = pMainMenu->Name.c_str();

    for (int nIndex = 0; nIndex<pMainMenu->Items->Count; nIndex++)
	{
        if (((TMenuItem *)(pMainMenu->Items))->Items[nIndex]->ClassNameIs("TMenuItem"))
		{
            AnsiString strCaption = ((TMenuItem *)(pMainMenu->Items))->Items[nIndex]->Caption;
            AnsiString strName = ((TMenuItem *)(pMainMenu->Items))->Items[nIndex]->Name;
            DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

            for (int nIndex2 = 0; nIndex2<((TMenuItem *)(pMainMenu->Items))->Items[nIndex]->Count; nIndex2++)
	        {
                if (((TMenuItem *)(pMainMenu->Items))->Items[nIndex]->Items[nIndex2]->ClassNameIs("TMenuItem"))
				{
					AnsiString strCaption = ((TMenuItem *)(pMainMenu->Items))->Items[nIndex]->Items[nIndex2]->Caption;
					AnsiString strName = ((TMenuItem *)(pMainMenu->Items))->Items[nIndex]->Items[nIndex2]->Name;
					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
                }
            }
		}
    }
}


void __fastcall TfrmMain::CreateCaptionFile(TForm *pForm)
{
	AnsiString strFile;
	TIniFile *pIniFile;

	strFile.sprintf("%s%s", "C:\\", "Caption_C74.ini");
	pIniFile = new TIniFile(strFile);

	const char *Section = pForm->Name.c_str();

	for (int nIndex = 0; nIndex<pForm->ControlCount; nIndex++)
	{

		if (pForm->Controls[nIndex]->ClassNameIs("TToolBar"))
		{
			for (int nIndex2 = 0; nIndex2<((TToolBar *)pForm->Controls[nIndex])->ControlCount; nIndex2++)
			{
				if (((TToolBar *)pForm->Controls[nIndex])->Controls[nIndex2]->ClassNameIs("TToolButton"))
				{
					AnsiString strCaption = ((TToolButton *)((TToolBar *)pForm->Controls[nIndex])->Controls[nIndex2])->Caption;
					AnsiString strName = ((TToolButton *)((TToolBar *)pForm->Controls[nIndex])->Controls[nIndex2])->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}
			}
		}

		if (pForm->Controls[nIndex]->ClassNameIs("TGroupBox"))
		{
			for (int nIndex2 = 0; nIndex2<((TGroupBox *)pForm->Controls[nIndex])->ControlCount; nIndex2++)
			{
				AnsiString strCaption = ((TGroupBox *)pForm->Controls[nIndex])->Caption;
				AnsiString strName = ((TGroupBox *)pForm->Controls[nIndex])->Name;

				DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

				if (((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2]->ClassNameIs("TSpeedButton"))
				{
					AnsiString strCaption = ((TSpeedButton *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Caption;
					AnsiString strName = ((TSpeedButton *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}

				if (((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2]->ClassNameIs("TBitBtn"))
				{
					AnsiString strCaption = ((TBitBtn *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Caption;
					AnsiString strName = ((TBitBtn *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}

				if (((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2]->ClassNameIs("TCheckBox"))
				{
					AnsiString strCaption = ((TCheckBox *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Caption;
					AnsiString strName = ((TCheckBox *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}

				if (((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2]->ClassNameIs("TLabel"))
				{
					AnsiString strCaption = ((TLabel *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Caption;
					AnsiString strName = ((TLabel *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}

				if (((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2]->ClassNameIs("TStaticText"))
				{
					AnsiString strCaption = ((TStaticText *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Caption;
					AnsiString strName = ((TStaticText *)((TGroupBox *)pForm->Controls[nIndex])->Controls[nIndex2])->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}
			}
		}

		if (pForm->Controls[nIndex]->ClassNameIs("TPageControl"))
		{
			TPageControl *pPage = (TPageControl *)pForm->Controls[nIndex];
			for (int nIndex2 = 0; nIndex2<pPage->ControlCount; nIndex2++)
			{
				if (pPage->Controls[nIndex2]->ClassNameIs("TTabSheet"))
				{
					TTabSheet *pSheet = (TTabSheet *)pPage->Controls[nIndex2];
					AnsiString strCaption = pSheet->Caption;
					AnsiString strName = pSheet->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

					for (int nIndex3 = 0; nIndex3<pSheet->ControlCount; nIndex3++)
					{
						if (pSheet->Controls[nIndex3]->ClassNameIs("TStaticText"))
						{
							TStaticText *pText = (TStaticText *)pSheet->Controls[nIndex3];
							AnsiString strCaption = pText->Caption;
							AnsiString strName = pText->Name;

							DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
						}

						if (pSheet->Controls[nIndex3]->ClassNameIs("TGroupBox"))
						{
							TGroupBox *pGroup = (TGroupBox *)pSheet->Controls[nIndex3];
							AnsiString strCaption = pGroup->Caption;
							AnsiString strName = pGroup->Name;

							DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

							for (int nIndex4 = 0; nIndex4<pGroup->ControlCount; nIndex4++)
							{
								if (pGroup->Controls[nIndex4]->ClassNameIs("TStaticText"))
								{
									TStaticText *pText = (TStaticText *)pGroup->Controls[nIndex4];
									AnsiString strCaption = pText->Caption;
									AnsiString strName = pText->Name;

									DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
								}
							}
						}

						if (pSheet->Controls[nIndex3]->ClassNameIs("TSpeedButton"))
						{
							TSpeedButton *pBtn = (TSpeedButton *)pSheet->Controls[nIndex3];
							AnsiString strCaption = pBtn->Caption;
							AnsiString strName = pBtn->Name;

							DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
						}

						if (pSheet->Controls[nIndex3]->ClassNameIs("TPanel"))
						{
							TPanel *pPanel = (TPanel *)pSheet->Controls[nIndex3];
							AnsiString strCaption = pPanel->Caption;
							AnsiString strName = pPanel->Name;

							DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
						}
					}
				}
			}
		}


		if (pForm->Controls[nIndex]->ClassNameIs("TPanel"))
		{
			TPanel *pPanel = (TPanel *)pForm->Controls[nIndex];
			for (int nIndex2 = 0; nIndex2<pPanel->ControlCount; nIndex2++)
			{
				if (pPanel->Controls[nIndex2]->ClassNameIs("TLabel"))
				{
					TLabel *pLabel = (TLabel *)pPanel->Controls[nIndex2];
					AnsiString strCaption = pLabel->Caption;
					AnsiString strName = pLabel->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}

				if (pPanel->Controls[nIndex2]->ClassNameIs("TBitBtn"))
				{
					TBitBtn *pBtn = (TBitBtn *)pPanel->Controls[nIndex2];
					AnsiString strCaption = pBtn->Caption;
					AnsiString strName = pBtn->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}

				if (pPanel->Controls[nIndex2]->ClassNameIs("TStaticText"))
				{
					TStaticText *pTxt = (TStaticText *)pPanel->Controls[nIndex2];
					AnsiString strCaption = pTxt->Caption;
					AnsiString strName = pTxt->Name;

					DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
				}

				if (pPanel->Controls[nIndex2]->ClassNameIs("TPageControl"))
				{
					TPageControl *pPage = (TPageControl *)pPanel->Controls[nIndex2];
					for (int nIndex3 = 0; nIndex3<pPage->ControlCount; nIndex3++)
					{
						if (pPage->Controls[nIndex3]->ClassNameIs("TTabSheet"))
						{
							TTabSheet *pSheet = (TTabSheet *)pPage->Controls[nIndex3];
							AnsiString strCaption = pSheet->Caption;
							AnsiString strName = pSheet->Name;

							DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

							for (int nIndex4 = 0; nIndex4<pSheet->ControlCount; nIndex4++)
							{
								if (pSheet->Controls[nIndex4]->ClassNameIs("TSpeedButton"))
								{
									TSpeedButton *pBtn = (TSpeedButton *)pSheet->Controls[nIndex4];
									AnsiString strCaption = pBtn->Caption;
									AnsiString strName = pBtn->Name;

									DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
								}

								if (pSheet->Controls[nIndex4]->ClassNameIs("TPanel"))
								{
									TPanel *pPanel = (TPanel *)pSheet->Controls[nIndex4];
									AnsiString strCaption = pPanel->Caption;
									AnsiString strName = pPanel->Name;

									DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
								}
							}
						}
					}
				}
			}
		}

		if (pForm->Controls[nIndex]->ClassNameIs("TBitBtn"))
		{
			AnsiString strCaption = ((TBitBtn *)pForm->Controls[nIndex])->Caption;
			AnsiString strName = ((TBitBtn *)pForm->Controls[nIndex])->Name;

			DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

		}

		if (pForm->Controls[nIndex]->ClassNameIs("TSpeedButton"))
		{
			AnsiString strCaption = ((TSpeedButton *)pForm->Controls[nIndex])->Caption;
			AnsiString strName = ((TSpeedButton *)pForm->Controls[nIndex])->Name;

			DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

		}

		if (pForm->Controls[nIndex]->ClassNameIs("TButton"))
		{
			AnsiString strCaption = ((TButton *)pForm->Controls[nIndex])->Caption;
			AnsiString strName = ((TButton *)pForm->Controls[nIndex])->Name;

			DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

		}

		if (pForm->Controls[nIndex]->ClassNameIs("TLabel"))
		{
			AnsiString strCaption = ((TLabel *)pForm->Controls[nIndex])->Caption;
			AnsiString strName = ((TLabel *)pForm->Controls[nIndex])->Name;

			DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

		}

		if (pForm->Controls[nIndex]->ClassNameIs("TCheckBox"))
		{
			AnsiString strCaption = ((TLabel *)pForm->Controls[nIndex])->Caption;
			AnsiString strName = ((TLabel *)pForm->Controls[nIndex])->Name;

			DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

		}

		if (pForm->Controls[nIndex]->ClassNameIs("TStaticText"))
		{
			AnsiString strCaption = ((TStaticText *)pForm->Controls[nIndex])->Caption;
			AnsiString strName = ((TStaticText *)pForm->Controls[nIndex])->Name;

			DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");

		}

        if (pForm->Controls[nIndex]->ClassNameIs("TRadioGroup"))
        {
            AnsiString strCaption = ((TRadioGroup *)pForm->Controls[nIndex])->Caption;
			AnsiString strName = ((TRadioGroup *)pForm->Controls[nIndex])->Name;

			DDXFile_String(false, pIniFile, Section, strName, strCaption, "NA");
        }
	}

	delete pIniFile;
}
//---------------------------------------------------------------------------
*/






void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
    g_IniFile.MachineFile(true);
	Caption = g_IniFile.m_strLastFileName;
	g_IniFile.ProductFile(g_IniFile.m_strLastFileName.c_str(), true);

    //CreateCaptionFile(this);
    ReadCaptionFile(this, g_IniFile.m_nLanguageMode);
    //CreateCaptionFileTMainMenu(TfrmMain::MainMenu);
    ReadCaptionFileTMainMenu(MainMenu, g_IniFile.m_nLanguageMode);

    Label22->Caption = g_IniFile.m_dLamTime[0];
    Label25->Caption = g_IniFile.m_dLamTemp[0];
    Label28->Caption = g_IniFile.m_dLamPress[0];
    Label17->Caption = g_IniFile.m_dLamTime[1];
    Label13->Caption = g_IniFile.m_dLamTemp[1];
    Label14->Caption = g_IniFile.m_dLamPress[1];
    TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
    GroupBox2->Caption = StrList->Strings[StrList->Count-1];
    delete StrList;

	g_pMainThread = new CMainThread(false);

	SetAllDevice();
    RenewRadioGroup(true);

	Timer1->Enabled = true;
    Timer2->Enabled = true;
	timerPressure->Enabled = true;
    g_pGetRealTimeValueThread = new CGetRealTimeValueThread(false);

    g_TempLog.open(0);
    g_ActionLog.open(1);
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);
    m_strDate.sprintf("%04d_%02d_%02d",tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday);

    g_eqpXML.m_bOnLine = true;
    g_eqpXML.StartProcess = frmMain->StartProcess;
    g_eqpXML.OpenFile = frmMain->OpenFilebyCIM;
    g_eqpXML.m_EqpStatus = 'I';

    // IniFile boot CIM 
    if (g_IniFile.m_bIsUseCIM == true)
    {
        ServerCIM->Active = true;
        AddList("CIM Start!");
    }
    else
    {
        ServerCIM->Active = false;
        AddList("CIM Stop!");
        g_eqpXML.m_CIMStatus = "0";
    }

    m_bLastGassLeakyFront = false;
    m_bLastGassLeakyRear = false;
    m_bLastPusherIOErrorFront = false;
    m_bLastPusherIOErrorRear = false;

    //update the value of cmb
    g_pMainThread->m_nManualRange = cmbRange->ItemIndex;
    g_pMainThread->m_nManualFirstLoc = cmbFirstLoc->ItemIndex;
    g_pMainThread->m_nManualTimes = cmbTimes->Text.ToInt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	g_bStopMainThread = true;
    g_bStopGetRealTimeValueThread = true;
    g_TempLog.close();
    g_ActionLog.close();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmMain::cmbRangeChange(TObject *Sender)
{
	g_pMainThread->m_nManualRange = cmbRange->ItemIndex;
    if (cmbRange->ItemIndex == 1)
    {
        checkAutoRetry->Checked = false;
        checkAutoRetry->Enabled = false;
    }
    else
    {
        checkAutoRetry->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cmbFirstLocChange(TObject *Sender)
{
    g_pMainThread->m_nManualFirstLoc = cmbFirstLoc->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cmbTimesChange(TObject *Sender)
{
    g_pMainThread->m_nManualTimes = cmbTimes->Text.ToInt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::checkAutoRetryClick(TObject *Sender)
{
    g_pMainThread->m_bAutoRetry = checkAutoRetry->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnHomingClick(TObject *Sender)
{
    g_pMainThread->m_bIsHomingFromBtn = true;
    if (g_IniFile.m_nErrorCode == 0)
    {
        AnsiString pString;
        if (g_IniFile.m_nLanguageMode>0) pString = "Homing Reset?";
        else pString = "確實全機復歸";
        switch (Application->MessageBoxA(pString.c_str(), "Confirm", MB_YESNOCANCEL))
        {
            case IDYES: {g_pMainThread->m_nIsFullHoming = 1; break;}
            case IDNO: {g_pMainThread->m_nIsFullHoming = 0; break;}
            case IDCANCEL: {g_pMainThread->m_nIsFullHoming = 2; break;}
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::checkStopLCClick(TObject *Sender)
{
    g_pMainThread->m_bStopLC = checkStopLC->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::RenewRadioGroup(bool bInit)
{
    if (g_IniFile.m_nLanguageMode>0)
    {
        //Create radioPosOption
        radioPosOption->Items->Clear();
        radioPosOption->Items->Add("None");
        radioPosOption->Items->Add("MoveTo Load Cell Loc.");
        radioPosOption->Items->Add("MoveTo Up Laser Loc.");
        radioPosOption->Items->Add("MoveTo Down Laser Loc.");
        radioPosOption->Items->Add("Only On Proportional valve");
        radioPosOption->ItemIndex = 0;
        //Create cmbRange
        cmbRange->Items->Clear();
        cmbRange->Items->Add("ALL");
        cmbRange->Items->Add("One");
        cmbRange->ItemIndex = 0;
        if (bInit)
        {
            //Control Check
            if (g_ModBus.m_bInitOK) AddList("Temp Control Start Success");
	        else AddList("Temp Control Start Failed");
	        if (g_Balance.m_bInitOK) AddList("LoadCell Control Start Success");
	        else AddList("LoadCell Control Start Failed");
        }
    }
    else
    {
        //Create radioPosOption
        radioPosOption->Items->Clear();
        radioPosOption->Items->Add("不動作");
        radioPosOption->Items->Add("移動至Load Cell 位置");
        radioPosOption->Items->Add("移動至Laser 上模 位置");
        radioPosOption->Items->Add("移動至Laser 下模 位置");
        radioPosOption->Items->Add("僅啟動此比例閥");
        radioPosOption->ItemIndex = 0;
        //Create cmbRange
        cmbRange->Items->Clear();
        cmbRange->Items->Add("整盤");
        cmbRange->Items->Add("單顆");
        cmbRange->ItemIndex = 0;
        if (bInit)
        {
			//Control Check
	        if (g_ModBus.m_bInitOK) AddList("溫控器通訊埠開啟成功");
	        else AddList("溫控器通訊埠開啟失敗");
	        if (g_Balance.m_bInitOK) AddList("荷重元通訊埠開啟成功");
	        else AddList("荷重元通訊埠開啟失敗");
        }
    }

}
//---------------------------------------------------------------------------

AnsiString __fastcall TfrmMain::CheckAccount(AnsiString Account, AnsiString Password)
{
    AnsiString result = "";
    SQLITE3IF *g_AccountLog = new SQLITE3IF(2, "C:\\C74 Log\\AccountLog");

    switch (g_AccountLog->checkAccountPass(Account, Password))
    {
    case 0:
        result = "准許通過";
        break;
    case 1:
        result = "帳號錯誤";
        break;
    case 2:
        result = "密碼錯誤";
        break;
    default:
        break;
    }

    delete g_AccountLog;
    return result;
}
//---------------------------------------------------------------------------

AnsiString __fastcall TfrmMain::ChangePWD(AnsiString Account, AnsiString OldPassword, AnsiString NewPassword)
{
    AnsiString result = "";
    SQLITE3IF *g_AccountLog = new SQLITE3IF(2, "C:\\C74 Log\\AccountLog");

    (g_AccountLog->changeAccountPass(Account, OldPassword, NewPassword)) ? result = "修改成功" : result = "修改失敗";

    delete g_AccountLog;
    return result;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnStartMotor0Click(TObject *Sender)
{
    if (btnStartMotor0->Down == true)
    {
        g_DIO.SetDO(DO::LamMotorStart2, true);
        g_DIO.SetDO(DO::LCMotorStart, true);
        g_DIO.SetDO(DO::EjectMotorStart2, true);
    }
    else
    {
        g_DIO.SetDO(DO::LamMotorStart2, false);
        g_DIO.SetDO(DO::LCMotorStart, false);
        g_DIO.SetDO(DO::EjectMotorStart2, false);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnStartMotor1Click(TObject *Sender)
{
    if (btnStartMotor1->Down == true)
    {
        g_DIO.SetDO(DO::LamMotorStart1, true);
        g_DIO.SetDO(DO::LCMotorStart, true);
        g_DIO.SetDO(DO::EjectMotorStart1, true);
    }
    else
    {
        g_DIO.SetDO(DO::LamMotorStart1, false);
        g_DIO.SetDO(DO::LCMotorStart, false);
        g_DIO.SetDO(DO::EjectMotorStart1, false);
    }
}
//---------------------------------------------------------------------------
bool TfrmMain::StartProcess(bool bStart)
{
	if(bStart)
	{
		g_pMainThread->m_bIsStartProcessbyCIM = true;
        g_pMainThread->m_listLog.push_back("收到CIM回應 START，機台開始啟動...");
	}
	else
	{
		g_pMainThread->m_bIsStopProcessbyCIM = true;
        g_pMainThread->m_listLog.push_back("收到CIM回應 STOP，機台禁止啟動...");
        //g_IniFile.m_nErrorCode = 37;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TfrmMain::OpenFilebyCIM(AnsiString strFileName)
{
    if (FileExists(strFileName))
    {
        g_IniFile.MachineFile(true);

        //先清除比例閥資料等2秒
        g_DNPort0.ClearOutData();
        g_DNPort0.WriteAllData();
        g_DNPort1.ClearOutData();
        g_DNPort1.WriteAllData();
        ::Sleep(2000);

        //再寫入產品參數
	    frmMain->Caption = strFileName;
	    g_IniFile.m_strLastFileName = strFileName;

	    g_IniFile.ProductFile(strFileName.c_str(), true);

	    frmMain->SetAllDevice();

	    frmMain->Label22->Caption = g_IniFile.m_dLamTime[0];
	    frmMain->Label25->Caption = g_IniFile.m_dLamTemp[0];
	    frmMain->Label28->Caption = g_IniFile.m_dLamPress[0];
	    frmMain->Label17->Caption = g_IniFile.m_dLamTime[1];
	    frmMain->Label13->Caption = g_IniFile.m_dLamTemp[1];
	    frmMain->Label14->Caption = g_IniFile.m_dLamPress[1];
	    TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
        frmMain->GroupBox2->Caption = StrList->Strings[StrList->Count-1];
        delete StrList;

	    frmMain->PaintBox1Paint(frmMain);
	    frmMain->PaintBox2Paint(frmMain);

	    g_Motion.SetSoftLimit(4, g_IniFile.m_dPLimitF, g_IniFile.m_dNLimitF);
	    g_Motion.SetSoftLimit(5, g_IniFile.m_dPLimitR, g_IniFile.m_dNLimitR);

	    return true;
    }
    else return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ServerCIMClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    g_eqpXML.StartComm(Socket);
    AddList("CIM Connected!!");
    Shape3->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ServerCIMClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    g_eqpXML.EndComm();
    AddList("CIM Disconnected!!");
    Shape3->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ServerCIMClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    AddList("CIM SocketEror");
    Socket->Close();
    Shape3->Brush->Color = clRed;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ServerCIMClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    //AddList("CIM Read");
    g_pMainThread->m_ActionLog.push_back(g_pMainThread->AddTimeString("[ServerCIM]CIM Read"));
    Shape3->Brush->Color = clYellow;
    g_eqpXML.ProcessCIMstr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerDIOStartAgainTimer(TObject *Sender)
{
    g_pMainThread->m_bStartAgain = true;
    g_pMainThread->m_bStopAgain = true;
    g_pMainThread->m_bResetAgain = true;
}
//---------------------------------------------------------------------------



