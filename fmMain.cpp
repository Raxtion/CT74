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
#include "PISODNM100.h"
#include "DeltaPLC.h"
#include "TA5Serial.h";
#include "SensoLinkF911.h"
#include "KeyenceIL030.h"

#include <algorithm>
#include <math.h>
#include "fmPassword.h"
#include "fmOffset.h"
#include "EQPXML.h"
#include "fmAccountManual.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
SQLITE3IF g_TempLog(0, "C:\\C74 Log\\TempLog");
SQLITE3IF g_ActionLog(1, "C:\\C74 Log\\ActionLog");
//SQLITE3IF g_ChangeLog(3, "C:\\C74 Log\\ChangeLog");
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
//extern CSensoLinkF911 g_F911ModBus; //RS485 used. and merge to DeltaPLC.
extern CKeyenceIL030 g_IL030SerialUp;
extern CKeyenceIL030 g_IL030SerialDown;

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

	double dTempOffsetF = 0.0;
	double dTempOffsetR = 0.0;

	while (true)
	{
		if (g_bStopGetRealTimeValueThread) break;
		//--- renew Temperature and LaserValue
		if (g_IniFile.m_bNotLam)
		{
			//If not Lam then set temp to 20 degree
			g_ModBus.SetSV(1, 20.0);
			::Sleep(10);
			g_ModBus.SetSV(2, 20.0);
			::Sleep(10);
		}
		else
		{
			g_ModBus.SetSV(1, g_IniFile.m_dLamTemp[0]);
			::Sleep(10);
			g_ModBus.SetSV(2, g_IniFile.m_dLamTemp[1]);
			::Sleep(10);
		}
		g_pMainThread->m_dRearTempRealTime = g_ModBus.GetPV(1);
		::Sleep(10);
		g_pMainThread->m_dFrontTempRealTime = g_ModBus.GetPV(2);
		::Sleep(10);
		if (dTempOffsetF != g_IniFile.m_dTempOffsetF)
		{
			g_ModBus.SetTempOffset(2, g_IniFile.m_dTempOffsetF);
			::Sleep(10);
		}
		dTempOffsetF = g_IniFile.m_dTempOffsetF;
		if (dTempOffsetR != g_IniFile.m_dTempOffsetR)
		{
			g_ModBus.SetTempOffset(1, g_IniFile.m_dTempOffsetR);
			::Sleep(10);
		}
		dTempOffsetR = g_IniFile.m_dTempOffsetR;

		if (g_IniFile.m_bIsUseIL030)
		{
			g_pMainThread->m_dDownLaserRealTime = g_IL030SerialDown.GetLaserValue();
			::Sleep(10);
			g_pMainThread->m_dUpperLaserRealTime = g_IL030SerialUp.GetLaserValue();
			::Sleep(10);
		}
		else
		{
			g_pMainThread->m_dDownLaserRealTime = g_ModBus.GetAnalogData(3, 0);
			::Sleep(10);
			g_pMainThread->m_dUpperLaserRealTime = g_ModBus.GetAnalogData(3, 1);
			::Sleep(10);
		}
		g_pMainThread->m_dForntPressloseRealTime = g_ModBus.GetAnalogData(3, 2);
		::Sleep(10);
		g_pMainThread->m_dRearPressloseRealTime = g_ModBus.GetAnalogData(3, 3);
		::Sleep(10);
		if (g_IniFile.m_bIsUseF911) g_pMainThread->m_dSensoLinkF911RealTime = g_ModBus.GetKg(4);

		::Sleep(10);
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

#pragma region MainCommonFunction
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	g_pMainThread = new CMainThread(false);
	g_pGetRealTimeValueThread = new CGetRealTimeValueThread(false);

	g_IniFile.MachineFile(true);
	Caption = g_IniFile.m_strLastFileName;
	g_IniFile.ProductFile(g_IniFile.m_strLastFileName.c_str(), true);
	if (g_IniFile.m_bIsUseDBOffset) ImportOffsetFromDB();

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
	GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
	delete StrList;


	SetAllDevice();
	RenewRadioGroup(true);

	Timer1->Enabled = true;
	Timer2->Enabled = true;
	timerPressure->Enabled = true;

	g_TempLog.open(0);
	g_ActionLog.open(1);
	//g_ChangeLog.open(3);
	time_t timer = time(NULL);
	struct tm *tblock = localtime(&timer);
	m_strDate.sprintf("%04d_%02d_%02d", tblock->tm_year + 1900, tblock->tm_mon + 1, tblock->tm_mday);

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

	m_bIsAutoRun = false;
	m_bLastGassLeakyFront = false;
	m_bLastGassLeakyRear = false;
	m_bLastPusherIOErrorFront = false;
	m_bLastPusherIOErrorRear = false;

	//update the value of cmb
	g_pMainThread->m_nManualRange = cmbRange->ItemIndex;
	g_pMainThread->m_nManualFirstLoc = cmbFirstLoc->ItemIndex;
	g_pMainThread->m_nManualTimes = cmbTimes->Text.ToInt();

	//Count MoveLocX, MoveLocY
	int nColum = 0;
	int nRow = 0;
	double dPitchFrontUpX = 0;
	double dPitchFrontUpY = 0;
	double dPitchRearUpX = 0;
	double dPitchRearUpY = 0;
	double dPitchFrontDownX = 0;
	double dPitchFrontDownY = 0;
	double dPitchRearDownX = 0;
	double dPitchRearDownY = 0;
	if (g_IniFile.m_nCols>1) dPitchFrontUpX = fabs(g_IniFile.m_dLaserUpPosX[1][0] - g_IniFile.m_dLaserUpPosEndX[1][0]) / (g_IniFile.m_nCols - 1);
	if (g_IniFile.m_nCols>1) dPitchFrontUpY = fabs(g_IniFile.m_dLaserUpPosY[1][0] - g_IniFile.m_dLaserUpPosEndY[1][0]) / (g_IniFile.m_nRows - 1);
	if (g_IniFile.m_nCols>1) dPitchRearUpX = fabs(g_IniFile.m_dLaserUpPosX[0][0] - g_IniFile.m_dLaserUpPosEndX[0][0]) / (g_IniFile.m_nCols - 1);
	if (g_IniFile.m_nCols>1) dPitchRearUpY = fabs(g_IniFile.m_dLaserUpPosY[0][0] - g_IniFile.m_dLaserUpPosEndY[0][0]) / (g_IniFile.m_nRows - 1);
	if (g_IniFile.m_nCols>1) dPitchFrontDownX = fabs(g_IniFile.m_dLaserDownPosX[1] - g_IniFile.m_dLaserDownPosEndX[1]) / (g_IniFile.m_nCols - 1);
	if (g_IniFile.m_nCols>1) dPitchFrontDownY = fabs(g_IniFile.m_dLaserDownPosY[1] - g_IniFile.m_dLaserDownPosEndY[1]) / (g_IniFile.m_nRows - 1);
	if (g_IniFile.m_nCols>1) dPitchRearDownX = fabs(g_IniFile.m_dLaserDownPosX[0] - g_IniFile.m_dLaserDownPosEndX[0]) / (g_IniFile.m_nCols - 1);
	if (g_IniFile.m_nCols>1) dPitchRearDownY = fabs(g_IniFile.m_dLaserDownPosY[0] - g_IniFile.m_dLaserDownPosEndY[0]) / (g_IniFile.m_nRows - 1);
	for (int nX = 0; nX<50; nX++)
	{
		nColum = nX % 10;
		nRow = nX / 10;

		if (nColum < g_IniFile.m_nCols && nRow < g_IniFile.m_nRows)
		{
			for (int nY = 0; nY < 4; nY++)
			{
				g_pMainThread->m_dFrontUpperMoveLocX[nX][nY] = g_IniFile.m_dLaserUpPosX[1][nY] + nColum*dPitchFrontUpX;
				g_pMainThread->m_dFrontUpperMoveLocY[nX][nY] = g_IniFile.m_dLaserUpPosY[1][nY] - nRow*dPitchFrontUpY;
				g_pMainThread->m_dRearUpperMoveLocX[nX][nY] = g_IniFile.m_dLaserUpPosX[0][nY] + nColum*dPitchRearUpX;
				g_pMainThread->m_dRearUpperMoveLocY[nX][nY] = g_IniFile.m_dLaserUpPosY[0][nY] - nRow*dPitchRearUpY;
			}

			g_pMainThread->m_dFrontDownMoveLocX[nX][0] = g_IniFile.m_dLaserDownPosX[1] + nColum*dPitchFrontDownX;
			g_pMainThread->m_dFrontDownMoveLocY[nX][0] = g_IniFile.m_dLaserDownPosY[1] - nRow*dPitchFrontDownY;
			g_pMainThread->m_dRearDownMoveLocX[nX][0] = g_IniFile.m_dLaserDownPosX[0] + nColum*dPitchRearDownX;
			g_pMainThread->m_dRearDownMoveLocY[nX][0] = g_IniFile.m_dLaserDownPosY[0] - nRow*dPitchRearDownY;
		}
	}

	str1DReaderData[0] = "";
	str1DReaderData[1] = "";
	client1DF->Active = true;
	client1DR->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	g_bStopMainThread = true;
	g_bStopGetRealTimeValueThread = true;
	::Sleep(50);
	g_TempLog.free();
	g_ActionLog.free();
	//g_ChangeLog.free();
	g_TempLog.close();
	::Sleep(50);
	g_ActionLog.close();
	::Sleep(50);
	//g_ChangeLog.close();
	::Sleep(50);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	CanClose = false;
	if (m_nUserLevel == 0)
	{
		bool bCheckAccountPass = false;
		TfrmPassword *pPwdDlg = new TfrmPassword(this);

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
			if (pPwdDlg->ShowModal() == mrOk)
			{
				AnsiString result = CheckAccount(pPwdDlg->editAccount->Text, pPwdDlg->editOldPassword->Text);
				if (result == "帳號錯誤") Application->MessageBox("帳號錯誤!!", "認證", MB_OK);
				else if (result == "密碼錯誤") Application->MessageBox("密碼錯誤!!", "認證", MB_OK);
				else
				{
					bCheckAccountPass = true;
					break;
				}
			}
			else
			{
				bCheckAccountPass = false;
				break;
			}
		}

		delete pPwdDlg;


		if (!bCheckAccountPass) return;
	}
	else if (m_nUserLevel == 1 || m_nUserLevel == 2) {}


	if (g_IniFile.m_nLanguageMode>0)
	{
		if (Application->MessageBox("Ready To Quit?", "Confirm", MB_OKCANCEL) == IDOK)
		{
			if (Application->MessageBox("Save File Before Leave?", "Confirm", MB_OKCANCEL) == IDOK)
				N3Click(Sender);
			CanClose = true;
		}
	}
	else
	{
		if (Application->MessageBox("是否要離開?", "注意", MB_OKCANCEL) == IDOK)
		{
			if (Application->MessageBox("離開前是否要存檔?", "注意", MB_OKCANCEL) == IDOK)
				N3Click(Sender);
			CanClose = true;
		}
	}
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
		//radioPosOption->Items->Add("Only On Proportional valve");
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
			if (!g_IniFile.m_bIsUseF911)
			{
				if (g_Balance.m_bInitOK) AddList("LoadCell Control Start Success");
				else AddList("LoadCell Control Start Failed");
			}
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
		//radioPosOption->Items->Add("僅啟動此比例閥");
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
			if (!g_IniFile.m_bIsUseF911)
			{
				if (g_Balance.m_bInitOK) AddList("荷重元通訊埠開啟成功");
				else AddList("荷重元通訊埠開啟失敗");
			}
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
bool TfrmMain::StartProcess(bool bStart)
{
	if (bStart)
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
	if (FileExists("C:\\Product_Data\\" + strFileName + ".ini"))
	{
		//g_IniFile.MachineFile(true);  //not reopen the MachineFile, because may off the bIsUseCIM. 

		//先清除比例閥資料等2秒
		g_DNPort0.ClearOutData();
		g_DNPort0.WriteAllData();
		g_DNPort1.ClearOutData();
		g_DNPort1.WriteAllData();
		::Sleep(2000);

		//再寫入產品參數
		frmMain->Caption = "C:\\Product_Data\\" + strFileName + ".ini";
		g_IniFile.m_strLastFileName = "C:\\Product_Data\\" + strFileName + ".ini";

		g_IniFile.ProductFile(g_IniFile.m_strLastFileName.c_str(), true);
		if (g_IniFile.m_bIsUseDBOffset) frmMain->ImportOffsetFromDB();

		frmMain->SetAllDevice();

		frmMain->Label22->Caption = g_IniFile.m_dLamTime[0];
		frmMain->Label25->Caption = g_IniFile.m_dLamTemp[0];
		frmMain->Label28->Caption = g_IniFile.m_dLamPress[0];
		frmMain->Label17->Caption = g_IniFile.m_dLamTime[1];
		frmMain->Label13->Caption = g_IniFile.m_dLamTemp[1];
		frmMain->Label14->Caption = g_IniFile.m_dLamPress[1];
		TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
		frmMain->GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
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
		radioPosOption->Enabled = false;
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
		MotorTest1->Enabled = true;
		radioPosOption->Enabled = false;
		N10->Enabled = true;

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
		MotorTest1->Enabled = true;
		N10->Enabled = true;
		radioPosOption->Enabled = true;
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
			(g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 1) && !g_IniFile.m_bNotLam)
			g_DNPort0.SetKg(nIndex, g_IniFile.m_dLamPress[1] + g_IniFile.m_dScaleOffsetFront[nIndex]);
		else  g_DNPort0.SetKg(nIndex, 0);
	}
	g_DNPort0.WriteAllData();

	for (int nIndex = 0; nIndex<50; nIndex++)
	{
		int nCol = nIndex % 10;
		int nRow = nIndex / 10;

		if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows &&
			(g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 2) && !g_IniFile.m_bNotLam)
			g_DNPort1.SetKg(nIndex, g_IniFile.m_dLamPress[0] + g_IniFile.m_dScaleOffsetRear[nIndex]);
		else  g_DNPort1.SetKg(nIndex, 0);
	}
	g_DNPort1.WriteAllData();

	if (!g_IniFile.m_bIsUseF911) g_Balance.SetXY(g_IniFile.m_dLoadCellX[0], g_IniFile.m_dLoadCellX[1], g_IniFile.m_dLoadCellY[0], g_IniFile.m_dLoadCellY[1]);

	if (g_IniFile.m_dLamPress[0] == 0)
	{
		g_DNPort1.ClearOutData();
		g_DNPort1.WriteAllData();
	}
	if (g_IniFile.m_dLamPress[1] == 0)
	{
		g_DNPort0.ClearOutData();
		g_DNPort0.WriteAllData();
	}
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
void __fastcall TfrmMain::CheckParamChange(TfmProduct *pWnd, TfmProduct *pWndRecord, SQLITE3IF *g_ChangeLog, AnsiString SetupEE)
{
	//TfmProduct Check Param Change and Write to Log
	TRadioGroup *m_nRailOption = (TRadioGroup *)pWnd->FindComponent("m_nRailOption");
	TRadioGroup *m_nRailOptionRecord = (TRadioGroup *)pWndRecord->FindComponent("m_nRailOption");
	if (m_nRailOption->ItemIndex != m_nRailOptionRecord->ItemIndex)
	{
		AnsiString strLog = "";
		strLog.sprintf("流道數據修改: From %s To %s (%s)", FormatFloat("0", m_nRailOptionRecord->ItemIndex), FormatFloat("0", m_nRailOption->ItemIndex), SetupEE);
		//g_ChangeLog->insertChange(strLog);
		AddList(strLog);      //if compare more param,this can delete. And Log it in database only.
	}

	//TfmProduct Check Param Change and Turn off Tag
	TEdit *m_dLamPress1 = (TEdit *)pWnd->FindComponent("m_dLamPress1");
	TEdit *m_dLamPress1Record = (TEdit *)pWndRecord->FindComponent("m_dLamPress1");
	if (m_dLamPress1->Text.ToDouble() != m_dLamPress1Record->Text.ToDouble())
	{
		g_pMainThread->m_bIsAutoCalPressOverAllowF = true;
	}

	//TfmProduct Check Param Change and Turn off Tag
	TEdit *m_dLamPress0 = (TEdit *)pWnd->FindComponent("m_dLamPress0");
	TEdit *m_dLamPress0Record = (TEdit *)pWndRecord->FindComponent("m_dLamPress0");
	if (m_dLamPress0->Text.ToDouble() != m_dLamPress0Record->Text.ToDouble())
	{
		g_pMainThread->m_bIsAutoCalPressOverAllowR = true;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ImportOffsetFromDB()
{
	AnsiString strHeadType; (g_IniFile.m_nHeadType) ? strHeadType = "H_" : strHeadType = "S_";
	AnsiString strLayout = IntToStr(g_IniFile.m_nRows) + "X" + IntToStr(g_IniFile.m_nCols) + "_";
	AnsiString strHeadScal = g_IniFile.m_strHeadScal;


	AnsiString strModuleNumF = g_IniFile.m_strModuleNum[1] + "_";
	AnsiString strTableNameF = strHeadType + strModuleNumF + strLayout + strHeadScal;
	SQLITE3IF *g_OffsetTableF = new SQLITE3IF(4, "C:\\C74 Log\\OffsetTable", strTableNameF);
	AnsiString strResultF = g_OffsetTableF->queryOffsetTable(strTableNameF, true, g_IniFile.m_dLamPress[1]);
	if (strResultF == "") g_pMainThread->m_listLog.push_back("Table: " + strTableNameF + ". No Front Offset Data in DB.");
	delete g_OffsetTableF;


	AnsiString strModuleNumR = g_IniFile.m_strModuleNum[0] + "_";
	AnsiString strTableNameR = strHeadType + strModuleNumR + strLayout + strHeadScal;
	SQLITE3IF *g_OffsetTableR = new SQLITE3IF(4, "C:\\C74 Log\\OffsetTable", strTableNameR);
	AnsiString strResultR = g_OffsetTableR->queryOffsetTable(strTableNameR, false, g_IniFile.m_dLamPress[0]);
	if (strResultR == "") g_pMainThread->m_listLog.push_back("Table: " + strTableNameR + ". No Rear Offset Data in DB.");
	delete g_OffsetTableR;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMain::ExportDBtoCSV(bool bFront)
{
	bool bIsProcessOK = false;

	if (!FileExists("C:\\C74 Log\\OffsetTable\\Backup")) _mkdir("C:\\C74 Log\\OffsetTable\\Backup");

	AnsiString strFileName = "C:\\C74 Log\\OffsetTable\\Backup\\";
	AnsiString strDatetime = FormatDateTime("yyyymmddhhnnss", Now());
	strFileName = strFileName + strDatetime + "_" + g_IniFile.m_strModuleNum[bFront] + ".csv";

	double *pOffset;
	if (bFront) pOffset = g_IniFile.m_dScaleOffsetFront;
	else pOffset = g_IniFile.m_dScaleOffsetRear;

	try
	{
		FILE *pFile;

		pFile = fopen(strFileName.c_str(), "a+");

		if (pFile != NULL)
		{
			fprintf(pFile, "idx,isfront,location,setKg,offsetKg,statistic,Note\n");

			int j = 0;
			for (int i = 0; i < 50; i++)
			{
				if ((i % 10) < g_IniFile.m_nCols && (i / 10) < g_IniFile.m_nRows)
				{
					AnsiString strOup = IntToStr(j)
						+ "," + IntToStr(bFront)
						+ "," + IntToStr(i)
						+ "," + FormatFloat("0.000", g_IniFile.m_dLamPress[bFront])
						+ "," + FormatFloat("0.000", pOffset[i])
						+ "," + "NULL"
						+ "," + "NULL" + "\n";
					fprintf(pFile, strOup.c_str());
					j++;
				}
			}
		}
		bIsProcessOK = true;

	}
	catch (const EAccessViolation &e)
	{

	}

	return bIsProcessOK;
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
*/
#pragma endregion

#pragma region Socket
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
void __fastcall TfrmMain::client1DFConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AddList("Front 1D Reader連線 IP:" + Socket->RemoteAddress);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::client1DFDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AddList("Front 1D Reader 讀取器斷線!");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::client1DFError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    AddList("[clientVenderID Error]連線Front 1D Reader讀取器發生異常");
	ErrorCode = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::client1DFRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString strR = Socket->ReceiveText();

	if (strR.Length()>6 && strR != "ERROR")
	{
		AddList("Front 1D Reader:" + strR);
		str1DReaderData[1] = strR;
        if (str1DReaderData[1] == g_IniFile.m_strModuleNum[1]) g_pMainThread->m_bIsCheckParamModuleNumF_OK = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::client1DRConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AddList("Rear 1D Reader連線 IP:" + Socket->RemoteAddress);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::client1DRDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AddList("Rear 1D Reader 讀取器斷線!");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::client1DRError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    AddList("[clientVenderID Error]連線Rear 1D Reader讀取器發生異常");
	ErrorCode = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::client1DRRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString strR = Socket->ReceiveText();

	if (strR.Length()>6 && strR != "ERROR")
	{
		AddList("Rear 1D Reader:" + strR);
		str1DReaderData[0] = strR;
        if (str1DReaderData[0] == g_IniFile.m_strModuleNum[0]) g_pMainThread->m_bIsCheckParamModuleNumR_OK = true;
	}
}
//---------------------------------------------------------------------------

#pragma endregion

#pragma region MainTimer
void __fastcall TfrmMain::timerPressureTimer(TObject *Sender)
{
	timerPressure->Enabled = false;

	g_DNPort0.ReadAllData();
	g_DNPort1.ReadAllData();

	RefreshImage();

	//--- real time monitor temperature
	if (g_IniFile.m_nLanguageMode>0)
	{
		Label1->Caption = "Temperature:" + FormatFloat("0.0", g_pMainThread->m_dRearTempRealTime) + "℃ ";
		Label4->Caption = "Temperature:" + FormatFloat("0.0", g_pMainThread->m_dFrontTempRealTime) + "℃ ";

		Label2->Caption = "Lam.RemainTime:" + FormatFloat("0", g_pMainThread->m_dLamTimer[1] / 1000.0) + "Sec";
		Label5->Caption = "Lam.RemainTime:" + FormatFloat("0", g_pMainThread->m_dLamTimer[0] / 1000.0) + "Sec";
	}
	else
	{
		Label1->Caption = "目前溫度:" + FormatFloat("0.0", g_pMainThread->m_dRearTempRealTime) + "度";
		Label4->Caption = "目前溫度:" + FormatFloat("0.0", g_pMainThread->m_dFrontTempRealTime) + "度";

		Label2->Caption = "壓合剩餘時間:" + FormatFloat("0", g_pMainThread->m_dLamTimer[1] / 1000.0) + "秒";
		Label5->Caption = "壓合剩餘時間:" + FormatFloat("0", g_pMainThread->m_dLamTimer[0] / 1000.0) + "秒";
	}

	if (checkMonitor->Checked)
	{
		if (!g_IniFile.m_bIsUseF911) Label7->Caption = "Load Cell:" + FormatFloat("0.00kg", g_Balance.GetKg(1));
		else Label7->Caption = "Load Cell:" + FormatFloat("0.00kg", g_pMainThread->m_dSensoLinkF911RealTime);
		Label8->Caption = "Laser(上):" + FormatFloat("0.00", g_pMainThread->m_dUpperLaserRealTime) + "mm";
		Label9->Caption = "Laser(下):" + FormatFloat("0.00", g_pMainThread->m_dDownLaserRealTime) + "mm";
		Label35->Caption = "Gass R" + FormatFloat("0.00", g_pMainThread->m_dRearPressloseRealTime);
		Label36->Caption = "Gass F" + FormatFloat("0.00", g_pMainThread->m_dForntPressloseRealTime);
	}

	timerPressure->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::Timer1Timer(TObject *Sender)
{
	static int nErrorCode = 0;

	Timer1->Enabled = false;

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
			strCode.sprintf("%04d", g_IniFile.m_nErrorCode);
			g_eqpXML.SendAlarmMessage(strCode.c_str(), strMsgCIM.c_str());

			//Check DNport connection Error
			if (!g_IniFile.m_bNotLam && g_IniFile.m_nErrorCode == 2001)
			{
				bool bDNPortError = false;
				for (int nX = 0; nX < 50; nX++)
				{
					if ((g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 1) && (g_pMainThread->m_bDNPortConnectError1[nX] == true))
					{
						AddList("Front: " + IntToStr(nX + 1) + " DNport Connection Error.");
						bDNPortError = true;
					}
					if ((g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 2) && (g_pMainThread->m_bDNPortConnectError0[nX] == true))
					{
						AddList("Rear: " + IntToStr(nX + 1) + " DNport Connection Error");
						bDNPortError = true;
					}
					g_pMainThread->m_bDNPortConnectError0[nX] = false;
					g_pMainThread->m_bDNPortConnectError1[nX] = false;
				}
				if (!bDNPortError) AddList("比例閥通訊測試...OK");
				SetAllDevice();
			}
		}
		//lock RestetButton
		g_pMainThread->m_bIsResetAlarmLocked = false;
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
		if (!g_IniFile.m_bNotLam)
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
	}
	else editBoatFinishStart->Enabled = true;

	//---Detect Manual Mode processing End
	if (g_pMainThread->m_bIsManualFinish == true)
	{
		if (g_pMainThread->m_bStartPressCal[0] == false) btnStartPressCal0->Down = false;
		if (g_pMainThread->m_bStartPressCal[1] == false) btnStartPressCal1->Down = false;
		if (g_pMainThread->m_bStartLaserUpCal[1] == false) btnLaserUp1->Down = false;
		if (g_pMainThread->m_bStartLaserUpCal[0] == false) btnLaserUp0->Down = false;
		if (g_pMainThread->m_bStartLaserDownCal[1] == false) btnLaserDown1->Down = false;
		if (g_pMainThread->m_bStartLaserDownCal[0] == false) btnLaserDown0->Down = false;
		if (g_pMainThread->m_bStartOneStepCal == false) btnStartOneStepPressCal->Down = false;
	}

	//---Renew UPH
	editUnitPerHour->Text = FormatFloat("0.000", float(g_pMainThread->m_dUnitPerHour1) + float(g_pMainThread->m_dUnitPerHour0));

	//---Renew CIM signal
	Shape3->Brush->Color = clLime;

	//---Auto active LC no function
	if (g_IniFile.m_nErrorCode == 54 || g_IniFile.m_nErrorCode == 55
		|| g_IniFile.m_nErrorCode == 92 || g_IniFile.m_nErrorCode == 93
		|| (!g_IniFile.m_bNotLam && (g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 1) && g_pMainThread->m_bIsAutoCalPressOverAllowF)
		|| (!g_IniFile.m_bNotLam && (g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 2) && g_pMainThread->m_bIsAutoCalPressOverAllowR)
		|| (!g_IniFile.m_bNotLam && (g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 1) && g_pMainThread->m_bIsAutoCalTimesOver25F)
		|| (!g_IniFile.m_bNotLam && (g_IniFile.m_nRailOption == 0 || g_IniFile.m_nRailOption == 2) && g_pMainThread->m_bIsAutoCalTimesOver25R))
	{
		g_pMainThread->m_bStopLC = true;
	}

	//---Syn the checkStopLC
	checkStopLC->Checked = g_pMainThread->m_bStopLC;

	//---When StopLC, Off the ReadyIn1
	if (g_pMainThread->m_bStopLC) g_DIO.SetDO(DO::ReadyIn1, false);

	//---Auto ReLoad ProductParam
	if (g_IniFile.m_nErrorCode == 999 && g_pMainThread->m_bIsNeedReLoadProductParam)
	{
		g_pMainThread->m_bIsNeedReLoadProductParam = false;
		g_IniFile.ProductFile(Caption.c_str(), true);
		if (g_IniFile.m_bIsUseDBOffset) ImportOffsetFromDB();
	}

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
		if (g_IniFile.m_nErrorCode > 0 && g_IniFile.m_nErrorCode < 1000 && g_eqpXML.m_EqpStatus != 'D')
		{
			g_eqpXML.m_EqpStatus = 'D';
			g_eqpXML.SendEventReport("1");
		}
		else if (g_DIO.ReadDOBit(DO::StartBtnLamp) && g_eqpXML.m_EqpStatus != 'R' && !g_DIO.ReadDOBit(DO::StopBtnLamp))
		{
			g_eqpXML.m_EqpStatus = 'R';
			g_eqpXML.SendEventReport("1");
		}
		else if (g_IniFile.m_nErrorCode == 0 && g_eqpXML.m_EqpStatus != 'I' && !g_DIO.ReadDOBit(DO::StartBtnLamp))
		{
			g_eqpXML.m_EqpStatus = 'I';
			g_eqpXML.SendEventReport("1");
		}
	}

	//--CIM Remote Disable MainFromBtn---
	if (btnStartPressCal0->Down == false && btnLaserUp0->Down == false && btnLaserDown0->Down == false
		&& btnStartPressCal1->Down == false && btnLaserUp1->Down == false && btnLaserDown1->Down == false
		&& btnStartOneStepPressCal->Down == false)
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
		btnClearEject0->Enabled = !g_pMainThread->m_bIsAutoMode;
		btnClearEject1->Enabled = !g_pMainThread->m_bIsAutoMode;
		btnClearLC->Enabled = !g_pMainThread->m_bIsAutoMode;
		btnStartOneStepPressCal->Enabled = !g_pMainThread->m_bIsAutoMode;
	}

	//--Keep renew Privilege When Idel
	if (!btnStartPressCal1->Down && !btnStartPressCal0->Down
		&& !btnLaserUp1->Down && !btnLaserUp0->Down
		&& !btnLaserDown1->Down && !btnLaserDown0->Down
		&& !btnStartOneStepPressCal->Down)
		SetPrivilege(m_nUserLevel);
	else { MotorTest1->Enabled = false; N10->Enabled = false; }

	//--ShowNonLam
	if (g_IniFile.m_bNotLam)
	{
		if (g_IniFile.m_nLanguageMode>0) Caption = g_IniFile.m_strLastFileName + " ( Non Lamination )";
		else
		{
			Caption = g_IniFile.m_strLastFileName + " ( 不壓合 )";
			TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
			GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
			delete StrList;
		}
	}
	else
	{
		Caption = g_IniFile.m_strLastFileName;
		TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
		GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
		delete StrList;
	}

	//--Renew Mainfrom Product Info. Force!!
	Label22->Caption = g_IniFile.m_dLamTime[0];
	Label25->Caption = g_IniFile.m_dLamTemp[0];
	Label28->Caption = g_IniFile.m_dLamPress[0];
	Label17->Caption = g_IniFile.m_dLamTime[1];
	Label13->Caption = g_IniFile.m_dLamTemp[1];
	Label14->Caption = g_IniFile.m_dLamPress[1];

	Timer1->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::Timer2Timer(TObject *Sender)
{
	static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm2MS(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm3MS(EX_SCALE::TIME_1MS, false);

	Timer2->Enabled = false;

	//--- real time detect servermoter touch the soft limitation
	if (g_pMainThread->m_bIsHomeDone == true)
	{
		if (g_Motion.GetActualPos(0) < -0.1 || abs(g_Motion.GetActualPos(0) > 292.0)) g_IniFile.m_nErrorCode = 80;
		else if (g_Motion.GetActualPos(1) < -7.0 || abs(g_Motion.GetActualPos(1) > 583.0)) g_IniFile.m_nErrorCode = 81;
		else if (g_Motion.GetActualPos(2) < -2.9 || abs(g_Motion.GetActualPos(2) > 549.0)) g_IniFile.m_nErrorCode = 82;
		else if (g_Motion.GetActualPos(4) < g_IniFile.m_dNLimitF || abs(g_Motion.GetActualPos(4) > g_IniFile.m_dPLimitF)) g_IniFile.m_nErrorCode = 83;
		else if (g_Motion.GetActualPos(5) < g_IniFile.m_dNLimitR || abs(g_Motion.GetActualPos(5) > g_IniFile.m_dPLimitR)) g_IniFile.m_nErrorCode = 84;
	}

	//--- If g_pMainThread->m_bIsAutoMode change status, Recode on m_bIsAutoRun and Reset tm1MS and tm2MS
	if (g_pMainThread->m_bIsAutoMode == true && m_bIsAutoRun == false)
	{
		tm1MS.timeStart(5000);
		tm2MS.timeStart(5000);
		tm3MS.timeStart(1000);
	}
	m_bIsAutoRun = g_pMainThread->m_bIsAutoMode;

	//--- real time detect Front and Rear Gas Pressure leaky
	if (g_pMainThread->m_bIsAutoMode == true && g_pMainThread->m_bIsHomeDone == true && tm3MS.timeUp())
	{
		//---Detect GassLeaky keep over 5 second then allarm
		if (g_pMainThread->m_dForntPressloseRealTime < g_IniFile.m_dRightGassLeakylimit && !m_bLastGassLeakyFront) tm1MS.timeStart(5000);
		if (m_bLastGassLeakyFront && tm1MS.timeUp())
		{
			g_pMainThread->m_listLog.push_back(FormatFloat("GassSenser(F) Value= 0.00", g_pMainThread->m_dForntPressloseRealTime));
			g_IniFile.m_nErrorCode = 85;
		}
		if (g_pMainThread->m_dForntPressloseRealTime < g_IniFile.m_dRightGassLeakylimit) m_bLastGassLeakyFront = true;
		else m_bLastGassLeakyFront = false;
		//---Detect GassLeaky keep over 5 second then allarm
		if (g_pMainThread->m_dRearPressloseRealTime < g_IniFile.m_dLeftGassLeakylimit && !m_bLastGassLeakyRear) tm2MS.timeStart(5000);
		if (m_bLastGassLeakyRear && tm2MS.timeUp())
		{
			g_pMainThread->m_listLog.push_back(FormatFloat("GassSenser(R) Value= 0.00", g_pMainThread->m_dRearPressloseRealTime));
			g_IniFile.m_nErrorCode = 86;
		}
		if (g_pMainThread->m_dRearPressloseRealTime < g_IniFile.m_dLeftGassLeakylimit) m_bLastGassLeakyRear = true;
		else m_bLastGassLeakyRear = false;
	}

	//---If Finish PressCal write Offset in to OffsetTable Database
	if (g_pMainThread->m_bIsWriteOffsetToDB_F)
	{
		g_pMainThread->m_bIsWriteOffsetToDB_F = false;

		if (g_IniFile.m_strModuleNum[1] != "")
		{
			AnsiString strHeadType; (g_IniFile.m_nHeadType) ? strHeadType = "H_" : strHeadType = "S_";
			AnsiString strModuleNum = g_IniFile.m_strModuleNum[1] + "_";
			AnsiString strLayout = IntToStr(g_IniFile.m_nRows) + "X" + IntToStr(g_IniFile.m_nCols) + "_";
			AnsiString strHeadScal = g_IniFile.m_strHeadScal;
			AnsiString strTableName = strHeadType + strModuleNum + strLayout + strHeadScal;

			g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". OffsetTable Synchnizing, Please Waite.");
			//Write to DB
			SQLITE3IF *g_OffsetTable = new SQLITE3IF(4, "C:\\C74 Log\\OffsetTable", strTableName);
			AnsiString strResult = g_OffsetTable->updateOffsetTable(strTableName, true, g_IniFile.m_dLamPress[1]);
			g_pMainThread->m_listLog.push_back(strResult);
			delete g_OffsetTable;

			//Write to .csv
			if (ExportDBtoCSV(true)) g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". Backup finished.");
			else g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". Backup failed.");
			g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". OffsetTable Synchnizing, Process End.");
		}
	}
	if (g_pMainThread->m_bIsWriteOffsetToDB_R)
	{
		g_pMainThread->m_bIsWriteOffsetToDB_R = false;

		if (g_IniFile.m_strModuleNum[0] != "")
		{
			AnsiString strHeadType; (g_IniFile.m_nHeadType) ? strHeadType = "H_" : strHeadType = "S_";
			AnsiString strModuleNum = g_IniFile.m_strModuleNum[0] + "_";
			AnsiString strLayout = IntToStr(g_IniFile.m_nRows) + "X" + IntToStr(g_IniFile.m_nCols) + "_";
			AnsiString strHeadScal = g_IniFile.m_strHeadScal;
			AnsiString strTableName = strHeadType + strModuleNum + strLayout + strHeadScal;

			g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". OffsetTable Synchnizing, Please Waite.");
			//Write to DB
			SQLITE3IF *g_OffsetTable = new SQLITE3IF(4, "C:\\C74 Log\\OffsetTable", strTableName);
			AnsiString strResult = g_OffsetTable->updateOffsetTable(strTableName, false, g_IniFile.m_dLamPress[0]);
			g_pMainThread->m_listLog.push_back(strResult);
			delete g_OffsetTable;

			//Write to .csv
			if (ExportDBtoCSV(false)) g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". Backup finished.");
			else g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". Backup failed.");
			g_pMainThread->m_listLog.push_back("Table: " + strTableName + ". OffsetTable Synchnizing, Process End.");
		}
	}

    //---Make sure Param ModuleNum Correct
    if (g_pMainThread->m_bIsCheckParamModuleNum)
    {
        g_pMainThread->m_bIsCheckParamModuleNum = false;
        str1DReaderData[1] = "";
        str1DReaderData[0] = "";
        client1DF->Socket->SendText("LON\r\n");
        client1DR->Socket->SendText("LON\r\n");
        g_pMainThread->m_bIsCheckParamModuleNumF_OK = false;
        g_pMainThread->m_bIsCheckParamModuleNumR_OK = false;
    }
    if (g_pMainThread->m_bIsStopCheckParamModuleNum)
    {
        g_pMainThread->m_bIsStopCheckParamModuleNum = false;
        client1DF->Socket->SendText("LOFF\r\n");
        client1DR->Socket->SendText("LOFF\r\n");
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
	strNow.sprintf("%04d_%02d_%02d", tblock->tm_year + 1900, tblock->tm_mon + 1, tblock->tm_mday);
	if (strNow != m_strDate)
	{
		g_TempLog.free();
		g_ActionLog.free();
		//g_ChangeLog.free();
		g_TempLog.close();
		g_ActionLog.close();
		//g_ChangeLog.close();
		g_TempLog.open(0);
		g_ActionLog.open(1);
		//g_ChangeLog.open(3);
		m_strDate = "";
		time_t timer = time(NULL);
		struct tm *tblock = localtime(&timer);
		m_strDate.sprintf("%04d_%02d_%02d", tblock->tm_year + 1900, tblock->tm_mon + 1, tblock->tm_mday);
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
#pragma endregion

#pragma region MainMenu
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
		if (g_IniFile.m_bIsUseDBOffset) ImportOffsetFromDB();
		SetAllDevice();

		//Count MoveLocX, MoveLocY
		int nColum = 0;
		int nRow = 0;
		double dPitchFrontUpX = 0;
		double dPitchFrontUpY = 0;
		double dPitchRearUpX = 0;
		double dPitchRearUpY = 0;
		double dPitchFrontDownX = 0;
		double dPitchFrontDownY = 0;
		double dPitchRearDownX = 0;
		double dPitchRearDownY = 0;
		if (g_IniFile.m_nCols>1) dPitchFrontUpX = fabs(g_IniFile.m_dLaserUpPosX[1][0] - g_IniFile.m_dLaserUpPosEndX[1][0]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchFrontUpY = fabs(g_IniFile.m_dLaserUpPosY[1][0] - g_IniFile.m_dLaserUpPosEndY[1][0]) / (g_IniFile.m_nRows - 1);
		if (g_IniFile.m_nCols>1) dPitchRearUpX = fabs(g_IniFile.m_dLaserUpPosX[0][0] - g_IniFile.m_dLaserUpPosEndX[0][0]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchRearUpY = fabs(g_IniFile.m_dLaserUpPosY[0][0] - g_IniFile.m_dLaserUpPosEndY[0][0]) / (g_IniFile.m_nRows - 1);
		if (g_IniFile.m_nCols>1) dPitchFrontDownX = fabs(g_IniFile.m_dLaserDownPosX[1] - g_IniFile.m_dLaserDownPosEndX[1]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchFrontDownY = fabs(g_IniFile.m_dLaserDownPosY[1] - g_IniFile.m_dLaserDownPosEndY[1]) / (g_IniFile.m_nRows - 1);
		if (g_IniFile.m_nCols>1) dPitchRearDownX = fabs(g_IniFile.m_dLaserDownPosX[0] - g_IniFile.m_dLaserDownPosEndX[0]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchRearDownY = fabs(g_IniFile.m_dLaserDownPosY[0] - g_IniFile.m_dLaserDownPosEndY[0]) / (g_IniFile.m_nRows - 1);
		for (int nX = 0; nX<50; nX++)
		{
			nColum = nX % 10;
			nRow = nX / 10;

			if (nColum < g_IniFile.m_nCols && nRow < g_IniFile.m_nRows)
			{
				for (int nY = 0; nY < 4; nY++)
				{
					g_pMainThread->m_dFrontUpperMoveLocX[nX][nY] = g_IniFile.m_dLaserUpPosX[1][nY] + nColum*dPitchFrontUpX;
					g_pMainThread->m_dFrontUpperMoveLocY[nX][nY] = g_IniFile.m_dLaserUpPosY[1][nY] - nRow*dPitchFrontUpY;
					g_pMainThread->m_dRearUpperMoveLocX[nX][nY] = g_IniFile.m_dLaserUpPosX[0][nY] + nColum*dPitchRearUpX;
					g_pMainThread->m_dRearUpperMoveLocY[nX][nY] = g_IniFile.m_dLaserUpPosY[0][nY] - nRow*dPitchRearUpY;
				}

				g_pMainThread->m_dFrontDownMoveLocX[nX][0] = g_IniFile.m_dLaserDownPosX[1] + nColum*dPitchFrontDownX;
				g_pMainThread->m_dFrontDownMoveLocY[nX][0] = g_IniFile.m_dLaserDownPosY[1] - nRow*dPitchFrontDownY;
				g_pMainThread->m_dRearDownMoveLocX[nX][0] = g_IniFile.m_dLaserDownPosX[0] + nColum*dPitchRearDownX;
				g_pMainThread->m_dRearDownMoveLocY[nX][0] = g_IniFile.m_dLaserDownPosY[0] - nRow*dPitchRearDownY;
			}
		}

		Label22->Caption = g_IniFile.m_dLamTime[0];
		Label25->Caption = g_IniFile.m_dLamTemp[0];
		Label28->Caption = g_IniFile.m_dLamPress[0];
		Label17->Caption = g_IniFile.m_dLamTime[1];
		Label13->Caption = g_IniFile.m_dLamTemp[1];
		Label14->Caption = g_IniFile.m_dLamPress[1];
		TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
		GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
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
	g_IniFile.ProductFile(g_IniFile.m_strLastFileName.c_str(), false);

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
	GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
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
		GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
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
			//exit(0);
			FormClose(this, true);
			Application->Terminate();
		}
	}
	else
	{
		if (Application->MessageBox("是否要離開?", "注意", MB_OKCANCEL) == IDOK)
		{
			if (Application->MessageBox("離開前是否要存檔?", "注意", MB_OKCANCEL) == IDOK)
				N3Click(Sender);
			//exit(0);
			FormClose(this, true);
			Application->Terminate();
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
	DDX_Float(bRead, g_IniFile.m_dUpperTotalLaserAlarm, pMachineDlg->m_dUpperTotalLaserAlarm);
	DDX_Float(bRead, g_IniFile.m_dLeftGassLeakylimit, pMachineDlg->m_dLeftGassLeakylimit);
	DDX_Float(bRead, g_IniFile.m_dRightGassLeakylimit, pMachineDlg->m_dRightGassLeakylimit);
	DDX_Float(bRead, g_IniFile.m_dLaneTransportDelayTime, pMachineDlg->m_dLaneTransportDelayTime);
	DDX_Float(bRead, g_IniFile.m_dLamSubDelayTime, pMachineDlg->m_dLamSubDelayTime);
	DDX_Check(bRead, g_IniFile.m_bForceEject, pMachineDlg->m_bForceEject);
	DDX_Check(bRead, g_IniFile.m_bIsUseF911, pMachineDlg->m_bIsUseF911);
	DDX_Check(bRead, g_IniFile.m_bIsUseIL030, pMachineDlg->m_bIsUseIL030);
	DDX_Check(bRead, g_IniFile.m_bIsUseCIM, pMachineDlg->m_bIsUseCIM);
	DDX_ComboBox(bRead, g_IniFile.m_nLanguageMode, pMachineDlg->m_cmbLanguage);
	DDX_Check(bRead, g_IniFile.m_bIsMochineTestMode, pMachineDlg->m_bIsMochineTestMode);
	DDX_Check(bRead, g_IniFile.m_bIsFullLaserMode, pMachineDlg->m_bIsFullLaserMode);
	DDX_Check(bRead, g_IniFile.m_bIsUseDBOffset, pMachineDlg->m_bIsUseDBOffset);
	DDX_Check(bRead, g_IniFile.m_bIsUpdateOffsetTable, pMachineDlg->m_bIsUpdateOffsetTable);

	DDX_Float(bRead, g_IniFile.m_dLCEntryPos, pMachineDlg->m_dLCEntryPos);
	DDX_Float(bRead, g_IniFile.m_dLCFrontPos, pMachineDlg->m_dLCFrontPos);
	DDX_Float(bRead, g_IniFile.m_dLCRearPos, pMachineDlg->m_dLCRearPos);

	DDX_Float(bRead, g_IniFile.m_dPLimitF, pMachineDlg->m_dPLimitF);
	DDX_Float(bRead, g_IniFile.m_dNLimitF, pMachineDlg->m_dNLimitF);
	DDX_Float(bRead, g_IniFile.m_dPLimitR, pMachineDlg->m_dPLimitR);
	DDX_Float(bRead, g_IniFile.m_dNLimitR, pMachineDlg->m_dNLimitR);

	DDX_Float(bRead, g_IniFile.m_dLamHeight[0], pMachineDlg->m_dLamHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamHeight[1], pMachineDlg->m_dLamHeight1);
	DDX_Float(bRead, g_IniFile.m_dLamStop[0], pMachineDlg->m_dLamStop0);
	DDX_Float(bRead, g_IniFile.m_dLamStop[1], pMachineDlg->m_dLamStop1);
	DDX_Float(bRead, g_IniFile.m_dLamGetPos[0], pMachineDlg->m_dLamGetPos0);
	DDX_Float(bRead, g_IniFile.m_dLamGetPos[1], pMachineDlg->m_dLamGetPos1);
	DDX_Float(bRead, g_IniFile.m_dLamVacHeight[0], pMachineDlg->m_dLamVacHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamVacHeight[1], pMachineDlg->m_dLamVacHeight1);

	DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[0], pMachineDlg->m_dLamSecondHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[1], pMachineDlg->m_dLamSecondHeight1);
	DDX_Float(bRead, g_IniFile.m_dLamSecondTime[0], pMachineDlg->m_dLamSecondTime0);
	DDX_Float(bRead, g_IniFile.m_dLamSecondTime[1], pMachineDlg->m_dLamSecondTime1);
	DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[0], pMachineDlg->m_dLamThirdHeight0);
	DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[1], pMachineDlg->m_dLamThirdHeight1);
	DDX_Float(bRead, g_IniFile.m_dLamThirdTime[0], pMachineDlg->m_dLamThirdTime0);
	DDX_Float(bRead, g_IniFile.m_dLamThirdTime[1], pMachineDlg->m_dLamThirdTime1);

	DDX_Float(bRead, g_IniFile.m_dPressCalRange, pMachineDlg->m_dPressCalRange);
	DDX_Float(bRead, g_IniFile.m_dPressCalTime, pMachineDlg->m_dPressCalTime);

	DDX_Int(bRead, g_IniFile.m_nAutoInterval, pMachineDlg->m_nAutoInterval);
	DDX_Float(bRead, g_IniFile.m_dAutoStopRange, pMachineDlg->m_dAutoStopRange);

	DDX_Float(bRead, g_IniFile.m_dAutoRunTempRange, pMachineDlg->m_dAutoRunTempRange);
	DDX_Float(bRead, g_IniFile.m_dVacDelayTime, pMachineDlg->m_dVacDelayTime);
	DDX_Float(bRead, g_IniFile.m_dDeVacDelayTime, pMachineDlg->m_dDeVacDelayTime);

	DDX_Int(bRead, g_IniFile.m_nDownPercent, pMachineDlg->m_nDownPercent);
	DDX_Int(bRead, g_IniFile.m_nPressCalRecheckTimes, pMachineDlg->m_nPressCalRecheckTimes);

	while (1)
	{
		if (pMachineDlg->ShowModal() == mrOk)
		{
			double SecondSpeed0 = pMachineDlg->m_dLamSecondHeight0->Text.ToDouble() / pMachineDlg->m_dLamSecondTime0->Text.ToDouble();
			double SecondSpeed1 = pMachineDlg->m_dLamSecondHeight1->Text.ToDouble() / pMachineDlg->m_dLamSecondTime1->Text.ToDouble();
			double ThirdSpeed0 = pMachineDlg->m_dLamThirdHeight0->Text.ToDouble() / pMachineDlg->m_dLamThirdTime0->Text.ToDouble();
			double ThirdSpeed1 = pMachineDlg->m_dLamThirdHeight1->Text.ToDouble() / pMachineDlg->m_dLamThirdTime1->Text.ToDouble();

			if (SecondSpeed0 > 50 || SecondSpeed1 > 50)
			{
				Application->MessageBoxA("第二段速度不可超過50mm/s", "Confirm", MB_OK);
			}
			else if (ThirdSpeed0 > SecondSpeed0 || ThirdSpeed1 > SecondSpeed1)
			{
				Application->MessageBoxA("第三段速度不可超過第二段速度", "Confirm", MB_OK);
			}
			else if (pMachineDlg->m_dAutoStopRange->Text.ToDouble() <= pMachineDlg->m_dPressCalRange->Text.ToDouble())
			{
				Application->MessageBoxA("自動停機誤差不可以<=容許誤差", "Confirm", MB_OK);
			}
			else if (pMachineDlg->m_dLamStop0->Text.ToDouble() > 10 || pMachineDlg->m_dLamStop1->Text.ToDouble() > 10)
			{
				Application->MessageBoxA("前後 接片高度 不可以超過10", "Confirm", MB_OK);
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

	//if (pMachineDlg->ShowModal() == mrOk)
	if (bRead == false)
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
		DDX_Float(bRead, g_IniFile.m_dUpperTotalLaserAlarm, pMachineDlg->m_dUpperTotalLaserAlarm);
		DDX_Float(bRead, g_IniFile.m_dLeftGassLeakylimit, pMachineDlg->m_dLeftGassLeakylimit);
		DDX_Float(bRead, g_IniFile.m_dRightGassLeakylimit, pMachineDlg->m_dRightGassLeakylimit);
		DDX_Float(bRead, g_IniFile.m_dLaneTransportDelayTime, pMachineDlg->m_dLaneTransportDelayTime);
		DDX_Float(bRead, g_IniFile.m_dLamSubDelayTime, pMachineDlg->m_dLamSubDelayTime);
		DDX_Check(bRead, g_IniFile.m_bForceEject, pMachineDlg->m_bForceEject);
		DDX_Check(bRead, g_IniFile.m_bIsUseF911, pMachineDlg->m_bIsUseF911);
		DDX_Check(bRead, g_IniFile.m_bIsUseIL030, pMachineDlg->m_bIsUseIL030);
		DDX_Check(bRead, g_IniFile.m_bIsUseCIM, pMachineDlg->m_bIsUseCIM);
		DDX_ComboBox(bRead, g_IniFile.m_nLanguageMode, pMachineDlg->m_cmbLanguage);
		DDX_Check(bRead, g_IniFile.m_bIsMochineTestMode, pMachineDlg->m_bIsMochineTestMode);
		DDX_Check(bRead, g_IniFile.m_bIsFullLaserMode, pMachineDlg->m_bIsFullLaserMode);
		DDX_Check(bRead, g_IniFile.m_bIsUseDBOffset, pMachineDlg->m_bIsUseDBOffset);
		DDX_Check(bRead, g_IniFile.m_bIsUpdateOffsetTable, pMachineDlg->m_bIsUpdateOffsetTable);

		DDX_Float(bRead, g_IniFile.m_dLCEntryPos, pMachineDlg->m_dLCEntryPos);
		DDX_Float(bRead, g_IniFile.m_dLCFrontPos, pMachineDlg->m_dLCFrontPos);
		DDX_Float(bRead, g_IniFile.m_dLCRearPos, pMachineDlg->m_dLCRearPos);

		DDX_Float(bRead, g_IniFile.m_dPLimitF, pMachineDlg->m_dPLimitF);
		DDX_Float(bRead, g_IniFile.m_dNLimitF, pMachineDlg->m_dNLimitF);
		DDX_Float(bRead, g_IniFile.m_dPLimitR, pMachineDlg->m_dPLimitR);
		DDX_Float(bRead, g_IniFile.m_dNLimitR, pMachineDlg->m_dNLimitR);

		DDX_Float(bRead, g_IniFile.m_dLamHeight[0], pMachineDlg->m_dLamHeight0);
		DDX_Float(bRead, g_IniFile.m_dLamHeight[1], pMachineDlg->m_dLamHeight1);
		DDX_Float(bRead, g_IniFile.m_dLamStop[0], pMachineDlg->m_dLamStop0);
		DDX_Float(bRead, g_IniFile.m_dLamStop[1], pMachineDlg->m_dLamStop1);
		DDX_Float(bRead, g_IniFile.m_dLamGetPos[0], pMachineDlg->m_dLamGetPos0);
		DDX_Float(bRead, g_IniFile.m_dLamGetPos[1], pMachineDlg->m_dLamGetPos1);
		DDX_Float(bRead, g_IniFile.m_dLamVacHeight[0], pMachineDlg->m_dLamVacHeight0);
		DDX_Float(bRead, g_IniFile.m_dLamVacHeight[1], pMachineDlg->m_dLamVacHeight1);

		DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[0], pMachineDlg->m_dLamSecondHeight0);
		DDX_Float(bRead, g_IniFile.m_dLamSecondHeight[1], pMachineDlg->m_dLamSecondHeight1);
		DDX_Float(bRead, g_IniFile.m_dLamSecondTime[0], pMachineDlg->m_dLamSecondTime0);
		DDX_Float(bRead, g_IniFile.m_dLamSecondTime[1], pMachineDlg->m_dLamSecondTime1);
		DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[0], pMachineDlg->m_dLamThirdHeight0);
		DDX_Float(bRead, g_IniFile.m_dLamThirdHeight[1], pMachineDlg->m_dLamThirdHeight1);
		DDX_Float(bRead, g_IniFile.m_dLamThirdTime[0], pMachineDlg->m_dLamThirdTime0);
		DDX_Float(bRead, g_IniFile.m_dLamThirdTime[1], pMachineDlg->m_dLamThirdTime1);

		DDX_Float(bRead, g_IniFile.m_dPressCalRange, pMachineDlg->m_dPressCalRange);
		DDX_Float(bRead, g_IniFile.m_dPressCalTime, pMachineDlg->m_dPressCalTime);

		DDX_Int(bRead, g_IniFile.m_nAutoInterval, pMachineDlg->m_nAutoInterval);
		DDX_Float(bRead, g_IniFile.m_dAutoStopRange, pMachineDlg->m_dAutoStopRange);

		DDX_Float(bRead, g_IniFile.m_dAutoRunTempRange, pMachineDlg->m_dAutoRunTempRange);
		DDX_Float(bRead, g_IniFile.m_dVacDelayTime, pMachineDlg->m_dVacDelayTime);
		DDX_Float(bRead, g_IniFile.m_dDeVacDelayTime, pMachineDlg->m_dDeVacDelayTime);

		DDX_Int(bRead, g_IniFile.m_nDownPercent, pMachineDlg->m_nDownPercent);
		DDX_Int(bRead, g_IniFile.m_nPressCalRecheckTimes, pMachineDlg->m_nPressCalRecheckTimes);

		g_Motion.SetSoftLimit(4, g_IniFile.m_dPLimitF, g_IniFile.m_dNLimitF);
		g_Motion.SetSoftLimit(5, g_IniFile.m_dPLimitR, g_IniFile.m_dNLimitR);

		//CreateCaptionFile(this);
		ReadCaptionFile(this, g_IniFile.m_nLanguageMode);
		//CreateCaptionFileTMainMenu(TfrmMain::MainMenu);
		ReadCaptionFileTMainMenu(MainMenu, g_IniFile.m_nLanguageMode);

		//Manual mode Bypass m_bIsAutoCalPressOverAllow
		if (g_IniFile.m_bIsMochineTestMode)
		{
			g_pMainThread->m_bIsAutoCalPressOverAllowF = false;
			g_pMainThread->m_bIsAutoCalPressOverAllowR = false;
			g_pMainThread->m_bIsAutoCalTimesOver25F = false;
			g_pMainThread->m_bIsAutoCalTimesOver25R = false;
		}

		RenewRadioGroup(false);
		this->Repaint();
	}
	delete pMachineDlg;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::N8Click(TObject *Sender)
{
	TfmProduct *pWnd = new TfmProduct(this);
	//TfmProduct *pWndRecord = new TfmProduct(this);

	//CreateCaptionFile(pWnd);
	ReadCaptionFile(pWnd, g_IniFile.m_nLanguageMode);

	AnsiString strCtl;
	bool bRead = true;

    /*
	//---------------------------------------------------------------------------
	//pWndRecord ready for CheckParamChange
	DDX_Int(bRead, g_IniFile.m_nCols, pWndRecord->m_nCols);
	DDX_Int(bRead, g_IniFile.m_nRows, pWndRecord->m_nRows);
	DDX_Float(bRead, g_IniFile.m_dColPitch, pWndRecord->m_dColPitch);
	DDX_Float(bRead, g_IniFile.m_dRowPitch, pWndRecord->m_dRowPitch);
    ...
    ...
    ...

    */
	//---------------------------------------------------------------------------
	//pWnd ready to show for user
	DDX_Int(bRead, g_IniFile.m_nCols, pWnd->m_nCols);
	DDX_Int(bRead, g_IniFile.m_nRows, pWnd->m_nRows);
	DDX_Float(bRead, g_IniFile.m_dColPitch, pWnd->m_dColPitch);
	DDX_Float(bRead, g_IniFile.m_dRowPitch, pWnd->m_dRowPitch);

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
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][0], pWnd->m_dLaserUpPosEndX00);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][0], pWnd->m_dLaserUpPosEndY00);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][1], pWnd->m_dLaserUpPosEndX01);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][1], pWnd->m_dLaserUpPosEndY01);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][2], pWnd->m_dLaserUpPosEndX02);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][2], pWnd->m_dLaserUpPosEndY02);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][3], pWnd->m_dLaserUpPosEndX03);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][3], pWnd->m_dLaserUpPosEndY03);

	DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[0], pWnd->m_dLaserDownPosX0);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[0], pWnd->m_dLaserDownPosY0);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndX[0], pWnd->m_dLaserDownPosEndX0);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndY[0], pWnd->m_dLaserDownPosEndY0);

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
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][0], pWnd->m_dLaserUpPosEndX10);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][0], pWnd->m_dLaserUpPosEndY10);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][1], pWnd->m_dLaserUpPosEndX11);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][1], pWnd->m_dLaserUpPosEndY11);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][2], pWnd->m_dLaserUpPosEndX12);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][2], pWnd->m_dLaserUpPosEndY12);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][3], pWnd->m_dLaserUpPosEndX13);
	DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][3], pWnd->m_dLaserUpPosEndY13);

	DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[1], pWnd->m_dLaserDownPosX1);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[1], pWnd->m_dLaserDownPosY1);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndX[1], pWnd->m_dLaserDownPosEndX1);
	DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndY[1], pWnd->m_dLaserDownPosEndY1);

	DDX_Radio(bRead, g_IniFile.m_nRailOption, pWnd->m_nRailOption);
	DDX_Check(bRead, g_IniFile.m_bNotLam, pWnd->m_bNotLam);

	DDX_ComboBox(bRead, g_IniFile.m_nBoatType, pWnd->m_cmbBoatType);
	DDX_ComboBox(bRead, g_IniFile.m_nHeadType, pWnd->m_cmbHeadType);
	DDX_String(bRead, g_IniFile.m_strHeadScal, pWnd->m_cmbHeadScal, g_IniFile.m_strHeadScals);
	DDX_String(bRead, g_IniFile.m_strModuleScal, pWnd->m_cmbModuleScal, g_IniFile.m_strModuleScals);
	DDX_ComboBox(bRead, g_IniFile.m_nUseLamCorrectBoard, pWnd->m_cmbUseLamCorrectBoard);
	DDX_ComboBox(bRead, g_IniFile.m_nVacummOn, pWnd->m_cmbVacummOn);
	DDX_ComboBox(bRead, g_IniFile.m_nPressCheck, pWnd->m_cmbPressCheck);
	DDX_ComboBox(bRead, g_IniFile.m_nDummyCheck, pWnd->m_cmbDummyCheck);
	g_IniFile.m_strModuleNum[0] = g_IniFile.m_strModuleNum[0].UpperCase();
	g_IniFile.m_strModuleNum[1] = g_IniFile.m_strModuleNum[1].UpperCase();
	DDX_String(bRead, g_IniFile.m_strModuleNum[0], pWnd->m_strModuleNum0);
	DDX_String(bRead, g_IniFile.m_strModuleNum[1], pWnd->m_strModuleNum1);

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

	DDX_Float(bRead, g_IniFile.m_dCoolDownTimeF, pWnd->m_dCoolDownTimeF);
	DDX_Float(bRead, g_IniFile.m_dCoolDownTimeR, pWnd->m_dCoolDownTimeR);

	DDX_Float(bRead, g_IniFile.m_dLamSecondKeepTime, pWnd->m_dLamSecondKeepTime);
	DDX_Check(bRead, g_IniFile.m_bIsLamSecondStop, pWnd->m_bIsLamSecondStop);
	DDX_Float(bRead, g_IniFile.m_dLamSecondCorrectTimes, pWnd->m_dLamSecondCorrectTimes);
	DDX_Check(bRead, g_IniFile.m_bIsLamUpDownCorrect, pWnd->m_bIsLamUpDownCorrect);

	DDX_Float(bRead, g_IniFile.m_dTempOffsetF, pWnd->m_dTempOffsetF);
	DDX_Float(bRead, g_IniFile.m_dTempOffsetR, pWnd->m_dTempOffsetR);

    DDX_Float(bRead, g_IniFile.m_dLamCorrBoardHeight, pWnd->m_dLamCorrBoardHeight);
    DDX_Float(bRead, g_IniFile.m_dLamProductHeight, pWnd->m_dLamProductHeight);

	pWnd->m_strSetupEENum->Text = g_IniFile.m_strLogInENGAccount;

	//if (g_IniFile.m_strLogInENGAccount != "Admin")
	if (g_IniFile.m_strLogInENGAccount == "OP")
	{
		pWnd->PageControl1->Pages[1]->Enabled = false;
	}
	pWnd->PageControl1->ActivePageIndex = 0;
	pWnd->m_cmbHeadType->Enabled = false;
	pWnd->m_cmbHeadScal->Enabled = false;
	pWnd->m_cmbModuleScal->Enabled = false;

    if (g_IniFile.m_nUseLamCorrectBoard)
    {
        pWnd->m_dLamCorrBoardHeight->Enabled = false;
        pWnd->m_dLamCorrBoardHeight->Color = clGray;
    }

	while (1)
	{
		if (pWnd->ShowModal() == mrOk)
		{
			if (pWnd->m_strSetupEENum->Text == "")
			{
				Application->MessageBoxA("SetupEE 編號不可空白", "Confirm", MB_OK);
			}
			else if (pWnd->m_nCols->Text.ToDouble() < 1 || pWnd->m_nRows->Text.ToDouble() < 1)
			{
				Application->MessageBoxA("Row與Colum不可以 < 1", "Confirm", MB_OK);
			}
			else if (pWnd->m_dLamTemp0->Text.ToDouble() >= 250 || (pWnd->m_dLamTemp0->Text.ToDouble() < 0))
			{
				Application->MessageBoxA("壓合溫度不可以超過0~250", "Confirm", MB_OK);
			}
			else if (pWnd->m_dLamTemp1->Text.ToDouble() >= 250 || (pWnd->m_dLamTemp1->Text.ToDouble() < 0))
			{
				Application->MessageBoxA("壓合溫度不可以超過0~250", "Confirm", MB_OK);
			}
			else if (!g_IniFile.m_bIsMochineTestMode && (pWnd->m_dLamTime0->Text.ToDouble() <= 60 || pWnd->m_dLamTime1->Text.ToDouble() <= 60))
			{
				Application->MessageBoxA("壓合時間不可以小於60秒", "Confirm", MB_OK);
			}
			else if (pWnd->m_dLamSecondCorrectTimes->Text.ToDouble() < 1)
			{
				Application->MessageBoxA("上下修正次數 不可以 < 1", "Confirm", MB_OK);
			}
            else if (!pWnd->m_bNotLam->Checked
                    && ((pWnd->m_nRailOption->ItemIndex == 0 && (pWnd->m_strModuleNum1->Text == "" || pWnd->m_strModuleNum0->Text == ""))
                        || (pWnd->m_nRailOption->ItemIndex == 1 && pWnd->m_strModuleNum1->Text == "")
                        || (pWnd->m_nRailOption->ItemIndex == 2 && pWnd->m_strModuleNum0->Text == "")))
            {
                Application->MessageBoxA("請輸入 模具編號", "Confirm", MB_OK);
            }
            else if (pWnd->m_dLamCorrBoardHeight->Text.ToDouble() < 0 || pWnd->m_dLamCorrBoardHeight->Text.ToDouble() < 0)
            {
                Application->MessageBoxA("退縮厚度 不可以 < 0", "Confirm", MB_OK);
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
		//CheckParamChange(pWnd, pWndRecord, &g_ChangeLog, g_IniFile.m_strLogInENGAccount);

		bRead = false;

		DDX_Int(bRead, g_IniFile.m_nCols, pWnd->m_nCols);
		DDX_Int(bRead, g_IniFile.m_nRows, pWnd->m_nRows);
		DDX_Float(bRead, g_IniFile.m_dColPitch, pWnd->m_dColPitch);
		DDX_Float(bRead, g_IniFile.m_dRowPitch, pWnd->m_dRowPitch);

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
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][0], pWnd->m_dLaserUpPosEndX00);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][0], pWnd->m_dLaserUpPosEndY00);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][1], pWnd->m_dLaserUpPosEndX01);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][1], pWnd->m_dLaserUpPosEndY01);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][2], pWnd->m_dLaserUpPosEndX02);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][2], pWnd->m_dLaserUpPosEndY02);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[0][3], pWnd->m_dLaserUpPosEndX03);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[0][3], pWnd->m_dLaserUpPosEndY03);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[0], pWnd->m_dLaserDownPosX0);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[0], pWnd->m_dLaserDownPosY0);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndX[0], pWnd->m_dLaserDownPosEndX0);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndY[0], pWnd->m_dLaserDownPosEndY0);

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
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][0], pWnd->m_dLaserUpPosEndX10);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][0], pWnd->m_dLaserUpPosEndY10);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][1], pWnd->m_dLaserUpPosEndX11);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][1], pWnd->m_dLaserUpPosEndY11);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][2], pWnd->m_dLaserUpPosEndX12);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][2], pWnd->m_dLaserUpPosEndY12);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndX[1][3], pWnd->m_dLaserUpPosEndX13);
		DDX_Float(bRead, g_IniFile.m_dLaserUpPosEndY[1][3], pWnd->m_dLaserUpPosEndY13);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosX[1], pWnd->m_dLaserDownPosX1);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosY[1], pWnd->m_dLaserDownPosY1);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndX[1], pWnd->m_dLaserDownPosEndX1);
		DDX_Float(bRead, g_IniFile.m_dLaserDownPosEndY[1], pWnd->m_dLaserDownPosEndY1);

		DDX_Radio(bRead, g_IniFile.m_nRailOption, pWnd->m_nRailOption);
		DDX_Check(bRead, g_IniFile.m_bNotLam, pWnd->m_bNotLam);

		DDX_ComboBox(bRead, g_IniFile.m_nBoatType, pWnd->m_cmbBoatType);
		DDX_ComboBox(bRead, g_IniFile.m_nHeadType, pWnd->m_cmbHeadType);
		DDX_String(bRead, g_IniFile.m_strHeadScal, pWnd->m_cmbHeadScal, g_IniFile.m_strHeadScals);
		DDX_String(bRead, g_IniFile.m_strModuleScal, pWnd->m_cmbModuleScal, g_IniFile.m_strModuleScals);
		DDX_ComboBox(bRead, g_IniFile.m_nUseLamCorrectBoard, pWnd->m_cmbUseLamCorrectBoard);
		DDX_ComboBox(bRead, g_IniFile.m_nVacummOn, pWnd->m_cmbVacummOn);
		DDX_ComboBox(bRead, g_IniFile.m_nPressCheck, pWnd->m_cmbPressCheck);
		DDX_ComboBox(bRead, g_IniFile.m_nDummyCheck, pWnd->m_cmbDummyCheck);
		DDX_String(bRead, g_IniFile.m_strModuleNum[0], pWnd->m_strModuleNum0);
		DDX_String(bRead, g_IniFile.m_strModuleNum[1], pWnd->m_strModuleNum1);
		g_IniFile.m_strModuleNum[0] = g_IniFile.m_strModuleNum[0].UpperCase();
		g_IniFile.m_strModuleNum[1] = g_IniFile.m_strModuleNum[1].UpperCase();

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

		DDX_String(bRead, g_IniFile.m_strSetupEENum, pWnd->m_strSetupEENum);

		DDX_Float(bRead, g_IniFile.m_dCoolDownTimeF, pWnd->m_dCoolDownTimeF);
		DDX_Float(bRead, g_IniFile.m_dCoolDownTimeR, pWnd->m_dCoolDownTimeR);

		DDX_Float(bRead, g_IniFile.m_dLamSecondKeepTime, pWnd->m_dLamSecondKeepTime);
		DDX_Check(bRead, g_IniFile.m_bIsLamSecondStop, pWnd->m_bIsLamSecondStop);
		DDX_Float(bRead, g_IniFile.m_dLamSecondCorrectTimes, pWnd->m_dLamSecondCorrectTimes);
		DDX_Check(bRead, g_IniFile.m_bIsLamUpDownCorrect, pWnd->m_bIsLamUpDownCorrect);

		DDX_Float(bRead, g_IniFile.m_dTempOffsetF, pWnd->m_dTempOffsetF);
		DDX_Float(bRead, g_IniFile.m_dTempOffsetR, pWnd->m_dTempOffsetR);

        DDX_Float(bRead, g_IniFile.m_dLamCorrBoardHeight, pWnd->m_dLamCorrBoardHeight);
        DDX_Float(bRead, g_IniFile.m_dLamProductHeight, pWnd->m_dLamProductHeight);

		//Count MoveLocX, MoveLocY
		int nColum = 0;
		int nRow = 0;
		double dPitchFrontUpX = 0;
		double dPitchFrontUpY = 0;
		double dPitchRearUpX = 0;
		double dPitchRearUpY = 0;
		double dPitchFrontDownX = 0;
		double dPitchFrontDownY = 0;
		double dPitchRearDownX = 0;
		double dPitchRearDownY = 0;
		if (g_IniFile.m_nCols>1) dPitchFrontUpX = fabs(g_IniFile.m_dLaserUpPosX[1][0] - g_IniFile.m_dLaserUpPosEndX[1][0]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchFrontUpY = fabs(g_IniFile.m_dLaserUpPosY[1][0] - g_IniFile.m_dLaserUpPosEndY[1][0]) / (g_IniFile.m_nRows - 1);
		if (g_IniFile.m_nCols>1) dPitchRearUpX = fabs(g_IniFile.m_dLaserUpPosX[0][0] - g_IniFile.m_dLaserUpPosEndX[0][0]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchRearUpY = fabs(g_IniFile.m_dLaserUpPosY[0][0] - g_IniFile.m_dLaserUpPosEndY[0][0]) / (g_IniFile.m_nRows - 1);
		if (g_IniFile.m_nCols>1) dPitchFrontDownX = fabs(g_IniFile.m_dLaserDownPosX[1] - g_IniFile.m_dLaserDownPosEndX[1]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchFrontDownY = fabs(g_IniFile.m_dLaserDownPosY[1] - g_IniFile.m_dLaserDownPosEndY[1]) / (g_IniFile.m_nRows - 1);
		if (g_IniFile.m_nCols>1) dPitchRearDownX = fabs(g_IniFile.m_dLaserDownPosX[0] - g_IniFile.m_dLaserDownPosEndX[0]) / (g_IniFile.m_nCols - 1);
		if (g_IniFile.m_nCols>1) dPitchRearDownY = fabs(g_IniFile.m_dLaserDownPosY[0] - g_IniFile.m_dLaserDownPosEndY[0]) / (g_IniFile.m_nRows - 1);
		for (int nX = 0; nX<50; nX++)
		{
			nColum = nX % 10;
			nRow = nX / 10;

			if (nColum < g_IniFile.m_nCols && nRow < g_IniFile.m_nRows)
			{
				for (int nY = 0; nY < 4; nY++)
				{
					g_pMainThread->m_dFrontUpperMoveLocX[nX][nY] = g_IniFile.m_dLaserUpPosX[1][nY] + nColum*dPitchFrontUpX;
					g_pMainThread->m_dFrontUpperMoveLocY[nX][nY] = g_IniFile.m_dLaserUpPosY[1][nY] - nRow*dPitchFrontUpY;
					g_pMainThread->m_dRearUpperMoveLocX[nX][nY] = g_IniFile.m_dLaserUpPosX[0][nY] + nColum*dPitchRearUpX;
					g_pMainThread->m_dRearUpperMoveLocY[nX][nY] = g_IniFile.m_dLaserUpPosY[0][nY] - nRow*dPitchRearUpY;
				}

				g_pMainThread->m_dFrontDownMoveLocX[nX][0] = g_IniFile.m_dLaserDownPosX[1] + nColum*dPitchFrontDownX;
				g_pMainThread->m_dFrontDownMoveLocY[nX][0] = g_IniFile.m_dLaserDownPosY[1] - nRow*dPitchFrontDownY;
				g_pMainThread->m_dRearDownMoveLocX[nX][0] = g_IniFile.m_dLaserDownPosX[0] + nColum*dPitchRearDownX;
				g_pMainThread->m_dRearDownMoveLocY[nX][0] = g_IniFile.m_dLaserDownPosY[0] - nRow*dPitchRearDownY;
			}
		}

		//Read OffsetTable
		if (g_IniFile.m_bIsUseDBOffset) ImportOffsetFromDB();

		Label22->Caption = g_IniFile.m_dLamTime[0];
		Label25->Caption = g_IniFile.m_dLamTemp[0];
		Label28->Caption = g_IniFile.m_dLamPress[0];
		Label17->Caption = g_IniFile.m_dLamTime[1];
		Label13->Caption = g_IniFile.m_dLamTemp[1];
		Label14->Caption = g_IniFile.m_dLamPress[1];
		TStringList *StrList = SplitString(g_IniFile.m_strLastFileName, "\\");
		GroupBox2->Caption = StrList->Strings[StrList->Count - 1];
		delete StrList;
	}

	SetAllDevice();
	RefreshImage();
	//delete pWndRecord;
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
	sPath = StringReplace(sPath, "\\exe\\", "", TReplaceFlags());

	while (pChoiceMotorDlg->ShowModal() != mrCancel)
	{
		pMotorCheckDlg = new TfrmMotorCheck(this);

		//CreateCaptionFile(pMotorCheckDlg);
		ReadCaptionFile(pMotorCheckDlg, g_IniFile.m_nLanguageMode);

		if (pChoiceMotorDlg->m_nSelectAxis == 0)
		{
			pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath + "\\bmp\\right.bmp");
			pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath + "\\bmp\\left.bmp");
			if (!g_DIO.ReadDIBit(DI::LoadCellDown))
			{
				Application->MessageBoxA("請確認LoadCell是否在上位!!", "Confirm", MB_OK);
				delete pMotorCheckDlg;
				delete pChoiceMotorDlg;
				return;
			}
			if ((!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassF) && !g_DIO.ReadDIBit(DI::UpperMoldCheck1))
				|| (!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassR) && !g_DIO.ReadDIBit(DI::UpperMoldCheck2)))
			{
				Application->MessageBoxA("請確認 前後上模具護蓋已經卸除!!", "Confirm", MB_OK);
				delete pMotorCheckDlg;
				delete pChoiceMotorDlg;
				return;
			}
		}
		if (pChoiceMotorDlg->m_nSelectAxis == 1)
		{
			if (!g_DIO.ReadDIBit(DI::LoadCellDown))
			{
				Application->MessageBoxA("請確認LoadCell是否在上位!!", "Confirm", MB_OK);
				delete pMotorCheckDlg;
				delete pChoiceMotorDlg;
				return;
			}
		}
		if (pChoiceMotorDlg->m_nSelectAxis == 4 || pChoiceMotorDlg->m_nSelectAxis == 5)
		{
			pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath + "\\bmp\\down.bmp");
			pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath + "\\bmp\\up.bmp");
			if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB))
			{
				Application->MessageBoxA("請確認LoadCell是否在安全位置!!", "Confirm", MB_OK);
				delete pMotorCheckDlg;
				delete pChoiceMotorDlg;
				return;
			}
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
	g_IniFile.m_strLogInENGAccount = "OP";
    AnsiString strShow = g_IniFile.m_strLogInENGAccount + " 登入.";
	g_pMainThread->m_listLog.push_back(strShow);
    AddChangeLog(strShow.c_str(),strShow.Length());
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
		if (pPwdDlg->ShowModal() == mrOk)
		{
			AnsiString result = CheckAccount(pPwdDlg->editAccount->Text, pPwdDlg->editOldPassword->Text);
			if (result == "帳號錯誤") Application->MessageBox("帳號錯誤!!", "認證", MB_OK);
			else if (result == "密碼錯誤") Application->MessageBox("密碼錯誤!!", "認證", MB_OK);
			else
			{
				Application->MessageBox("登入成功!!", "認證", MB_OK);
				g_IniFile.m_strLogInENGAccount = pPwdDlg->editAccount->Text;
				m_nUserLevel = 1;
                AnsiString strShow = g_IniFile.m_strLogInENGAccount + " 登入.";
				g_pMainThread->m_listLog.push_back(strShow);
                AddChangeLog(strShow.c_str(),strShow.Length());
				//unlock RestetButton
				g_pMainThread->m_bIsResetAlarmLocked = false;
				break;
			}
		}
		else
		{
			g_IniFile.m_strLogInENGAccount = "OP";
            AnsiString strShow = g_IniFile.m_strLogInENGAccount + " 登入.";
			g_pMainThread->m_listLog.push_back(strShow);
            AddChangeLog(strShow.c_str(),strShow.Length());
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
		if (pPwdDlg->ShowModal() == mrOk)
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
		if (pPwdDlg->editOldPassword->Text == g_IniFile.m_strARTPassword)
		{
			g_IniFile.m_strLogInENGAccount = "Admin";
            AnsiString strShow = g_IniFile.m_strLogInENGAccount + " 登入.";
			g_pMainThread->m_listLog.push_back(strShow);
            AddChangeLog(strShow.c_str(),strShow.Length());
			//unlock RestetButton
			g_pMainThread->m_bIsResetAlarmLocked = false;
			m_nUserLevel = 2;
		}
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
    
    //For Debug
    if (g_pMainThread->m_bIsHomeDone)
    {
        AnsiString strShowThreadIndex = "";
        for (int nIndex = 0; nIndex<MAX_PROCESS; nIndex++)
        {
            strShowThreadIndex += IntToStr(g_pMainThread->nThreadIndex[nIndex]) + ',';
        }
        for (int nAxis = 0; nAxis<6; nAxis++)
        {
            strShowThreadIndex += FormatFloat("0.000", g_Motion.GetActualPos(nAxis)) + ',';
        }

        AddList(strShowThreadIndex);
    }

	delete pAccountManual;
	delete g_AccountLog;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::N17Click(TObject *Sender)
{
	if (Application->MessageBoxA("變更系統語系為 中文?", "Confirm", MB_ICONQUESTION | MB_OKCANCEL) != IDOK) return;
	g_IniFile.m_nLanguageMode = 0;
	//CreateCaptionFile(this);
	ReadCaptionFile(this, g_IniFile.m_nLanguageMode);
	//CreateCaptionFileTMainMenu(TfrmMain::MainMenu);
	ReadCaptionFileTMainMenu(MainMenu, g_IniFile.m_nLanguageMode);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::N18Click(TObject *Sender)
{
	if (Application->MessageBoxA("Change Language to English?", "Confirm", MB_ICONQUESTION | MB_OKCANCEL) != IDOK) return;
	g_IniFile.m_nLanguageMode = 1;
	//CreateCaptionFile(this);
	ReadCaptionFile(this, g_IniFile.m_nLanguageMode);
	//CreateCaptionFileTMainMenu(TfrmMain::MainMenu);
	ReadCaptionFileTMainMenu(MainMenu, g_IniFile.m_nLanguageMode);
}
//---------------------------------------------------------------------------
#pragma endregion

#pragma region MainImage
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
			else if ((nIndex == g_pMainThread->m_nLaserCalMoveIndex[1] && !g_pMainThread->m_bIsDoLaserCalFinished) || (nIndex == g_pMainThread->m_nPressCalMoveIndex[1] && !g_pMainThread->m_bIsDoPressCalFinished)
				&& btnStartOneStepPressCal->Down) PaintBox1->Canvas->Brush->Color = clYellow;
			else if (g_pMainThread->m_dFrontUpperLaserDiff[nIndex][0] > g_IniFile.m_dUpperLaserAlarm
				|| g_pMainThread->m_dFrontDownLaserDiff[nIndex][0] > g_IniFile.m_dDownLaserAlarm
				|| g_pMainThread->m_dFrontUpperTotalLaserDiff[nIndex][0] > g_IniFile.m_dUpperTotalLaserAlarm) PaintBox1->Canvas->Brush->Color = clRed;
			else if (fabs(g_pMainThread->m_dFrontPressCal[nIndex] - g_IniFile.m_dLamPress[1]) > g_IniFile.m_dAutoStopRange*0.001) PaintBox1->Canvas->Brush->Color = clRed;
			else if (g_pMainThread->m_arrybDoPressCal[1][nIndex]) PaintBox1->Canvas->Brush->Color = clRed;
			else PaintBox1->Canvas->Brush->Color = clGreen;
		}
		else PaintBox1->Canvas->Brush->Color = clGray;

		PaintBox1->Canvas->Rectangle(m_vectRect[nIndex]);

		double dGetSetKgValue = g_DNPort0.GetSetKg(nIndex);
		double dGetKgValue = g_DNPort0.GetKg(nIndex);

		int nTextHeight = PaintBox1->Canvas->TextHeight("1");
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1, FormatFloat("(0)", nIndex + 1) + FormatFloat("0.00kg", g_pMainThread->m_dFrontPressCal[nIndex])); //+ FormatFloat("0.00kg ", dGetSetKgValue) + FormatFloat("0.00kg", dGetKgValue));
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 1, FormatFloat("0.00mm ", g_pMainThread->m_dFrontUpperLaser[nIndex][0]) + FormatFloat("0.00mm", g_pMainThread->m_dFrontUpperLaser[nIndex][1]));
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 2, FormatFloat("0.00mm ", g_pMainThread->m_dFrontUpperLaser[nIndex][2]) + FormatFloat("0.00mm", g_pMainThread->m_dFrontUpperLaser[nIndex][3]));
		if (nIndex == 0 || nIndex == 10 * (g_IniFile.m_nRows - 1) || nIndex == 10 * (g_IniFile.m_nRows - 1) + (g_IniFile.m_nCols - 1) || nIndex == g_IniFile.m_nCols - 1)
		{
			PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 3, FormatFloat("0.00mm ", g_pMainThread->m_dFrontUpperLaserDiff[nIndex][0]) + FormatFloat("0.00mm", g_pMainThread->m_dFrontUpperTotalLaserDiff[nIndex][0]));
		}
		else
		{
			PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 3, FormatFloat("0.00mm ", g_pMainThread->m_dFrontUpperLaserDiff[nIndex][0]));
		}
		PaintBox1->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 4, FormatFloat("0.00mm ", g_pMainThread->m_dFrontDownLaser[nIndex][0]) + FormatFloat("0.00mm ", g_pMainThread->m_dFrontDownLaserDiff[nIndex][0]));

		if (g_pMainThread->m_bIsHomeDone == true && (g_pMainThread->m_bStartPressCal[0] == false && g_pMainThread->m_bStartPressCal[1] == false))
		{
			//---Detect GassPusherIO are different 5 second then allarm
			if (fabs(dGetSetKgValue - dGetKgValue) > 0.05 && !m_bLastPusherIOErrorFront) tm1MS.timeStart(5000);
			if (m_bLastPusherIOErrorFront && tm1MS.timeUp())
			{
				g_IniFile.m_nErrorCode = 87;
				PaintBox1->Canvas->Brush->Color = clRed;
			}
			if (fabs(dGetSetKgValue - dGetKgValue) > 0.05) m_bLastPusherIOErrorFront = true;
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
			else if ((nIndex == g_pMainThread->m_nLaserCalMoveIndex[0] && !g_pMainThread->m_bIsDoLaserCalFinished) || (nIndex == g_pMainThread->m_nPressCalMoveIndex[0] && !g_pMainThread->m_bIsDoPressCalFinished)
				&& btnStartOneStepPressCal->Down) PaintBox2->Canvas->Brush->Color = clYellow;
			else if (g_pMainThread->m_dRearUpperLaserDiff[nIndex][0] > g_IniFile.m_dUpperLaserAlarm
				|| g_pMainThread->m_dRearDownLaserDiff[nIndex][0] > g_IniFile.m_dDownLaserAlarm
				|| g_pMainThread->m_dRearUpperTotalLaserDiff[nIndex][0] > g_IniFile.m_dUpperTotalLaserAlarm) PaintBox2->Canvas->Brush->Color = clRed;
			else if (fabs(g_pMainThread->m_dRearPressCal[nIndex] - g_IniFile.m_dLamPress[0]) > g_IniFile.m_dAutoStopRange*0.001) PaintBox2->Canvas->Brush->Color = clRed;
			else if (g_pMainThread->m_arrybDoPressCal[0][nIndex]) PaintBox2->Canvas->Brush->Color = clRed;
			else PaintBox2->Canvas->Brush->Color = clGreen;
		}
		else PaintBox2->Canvas->Brush->Color = clGray;

		PaintBox2->Canvas->Rectangle(m_vectRect[nIndex]);

		double dGetSetKgValue = g_DNPort1.GetSetKg(nIndex);
		double dGetKgValue = g_DNPort1.GetKg(nIndex);

		int nTextHeight = PaintBox2->Canvas->TextHeight("1");
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1, FormatFloat("(0)", nIndex + 1) + FormatFloat("0.00Kg", g_pMainThread->m_dRearPressCal[nIndex])); //+ FormatFloat("0.00kg", dGetSetKgValue) + FormatFloat("0.00kg", dGetKgValue));
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 1, FormatFloat("0.00mm ", g_pMainThread->m_dRearUpperLaser[nIndex][0]) + FormatFloat("0.00mm", g_pMainThread->m_dRearUpperLaser[nIndex][1]));
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 2, FormatFloat("0.00mm ", g_pMainThread->m_dRearUpperLaser[nIndex][2]) + FormatFloat("0.00mm", g_pMainThread->m_dRearUpperLaser[nIndex][3]));
		if (nIndex == 0 || nIndex == 10 * (g_IniFile.m_nRows - 1) || nIndex == 10 * (g_IniFile.m_nRows - 1) + (g_IniFile.m_nCols - 1) || nIndex == g_IniFile.m_nCols - 1)
		{
			PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 3, FormatFloat("0.00mm ", g_pMainThread->m_dRearUpperLaserDiff[nIndex][0]) + FormatFloat("0.00mm", g_pMainThread->m_dRearUpperTotalLaserDiff[nIndex][0]));
		}
		else
		{
			PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 3, FormatFloat("0.00mm ", g_pMainThread->m_dRearUpperLaserDiff[nIndex][0]));
		}
		PaintBox2->Canvas->TextOutA(m_vectRect[nIndex].Left + 3, m_vectRect[nIndex].top + 1 + nTextHeight * 4, FormatFloat("0.00mm ", g_pMainThread->m_dRearDownLaser[nIndex][0]) + FormatFloat("0.00mm ", g_pMainThread->m_dRearDownLaserDiff[nIndex][0]));

		if (g_pMainThread->m_bIsHomeDone == true && (g_pMainThread->m_bStartPressCal[0] == false && g_pMainThread->m_bStartPressCal[1] == false))
		{
			//---Detect GassPusherIO are different 5 second then allarm
			if (fabs(dGetSetKgValue - dGetKgValue) > 0.05 && !m_bLastPusherIOErrorRear) tm1MS.timeStart(5000);
			if (m_bLastPusherIOErrorRear && tm1MS.timeUp())
			{
				g_IniFile.m_nErrorCode = 87;
				PaintBox2->Canvas->Brush->Color = clRed;
			}
			if (fabs(dGetSetKgValue - dGetKgValue) > 0.05) m_bLastPusherIOErrorRear = true;
			else m_bLastPusherIOErrorRear = false;
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::PaintBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (radioPosOption->ItemIndex == 0 || g_pMainThread->m_bIsAutoMode) return;

	if (!g_pMainThread->m_bIsHomeDone)
	{
		if (g_IniFile.m_nLanguageMode>0) ShowMessage("Please Reset Machine");
		else ShowMessage("請先執行機台原點復歸");
		return;
	}
	else if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[1] + 1)
	{
		g_IniFile.m_nErrorCode = 98;
		return;
	}
	else if (g_Motion.GetActualPos(AXIS_RL)> g_IniFile.m_dLamStop[0] + 1)
	{
		g_IniFile.m_nErrorCode = 99;
		return;
	}
	else if (!g_DIO.ReadDIBit(DI::LoadCellDown))
	{
		g_IniFile.m_nErrorCode = 10;
		return;
	}
	else if ((!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassF) && !g_DIO.ReadDIBit(DI::UpperMoldCheck1))
		|| (!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassR) && !g_DIO.ReadDIBit(DI::UpperMoldCheck2)))
	{
		Application->MessageBoxA("請確認 前後上模具護蓋已卸除!!", "Confirm", MB_OK);
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

                //Manual Logging
                AnsiString strShow = "手動控制: 控制項=" + IntToStr(radioPosOption->ItemIndex) + " Index=" + IntToStr(nIndex);
                AddChangeLog(strShow.c_str(), strShow.Length());

				int nCol = nIndex % 10;
				int nRow = nIndex / 10;

				switch (radioPosOption->ItemIndex)
				{
				case 1:
					if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows)
					{
                        if (g_pMainThread->CheckSafeAbsMove(AXIS_X, dStartX + nCol*g_IniFile.m_dColPitch) && g_pMainThread->CheckSafeAbsMove(AXIS_Y, dStartY - nRow*g_IniFile.m_dRowPitch))
                        {
						    g_Motion.AbsMove(AXIS_X, dStartX + nCol*g_IniFile.m_dColPitch);
						    g_Motion.AbsMove(AXIS_Y, dStartY - nRow*g_IniFile.m_dRowPitch);
                        }
					}
					break;
				case 2:
					if (nFront)
					{
                        if (g_pMainThread->CheckSafeAbsMove(AXIS_X, g_pMainThread->m_dFrontUpperMoveLocX[nIndex][0]) && g_pMainThread->CheckSafeAbsMove(AXIS_Y, g_pMainThread->m_dFrontUpperMoveLocY[nIndex][0]))
                        {
						    g_Motion.AbsMove(AXIS_X, g_pMainThread->m_dFrontUpperMoveLocX[nIndex][0]);
						    g_Motion.AbsMove(AXIS_Y, g_pMainThread->m_dFrontUpperMoveLocY[nIndex][0]);
                        }
					}
					else
					{
                        if (g_pMainThread->CheckSafeAbsMove(AXIS_X, g_pMainThread->m_dRearUpperMoveLocX[nIndex][0]) && g_pMainThread->CheckSafeAbsMove(AXIS_Y, g_pMainThread->m_dRearUpperMoveLocY[nIndex][0]))
                        {
						    g_Motion.AbsMove(AXIS_X, g_pMainThread->m_dRearUpperMoveLocX[nIndex][0]);
						    g_Motion.AbsMove(AXIS_Y, g_pMainThread->m_dRearUpperMoveLocY[nIndex][0]);
                        }
					}
					break;
				case 3:
					if (nFront)
					{
                        if (g_pMainThread->CheckSafeAbsMove(AXIS_X, g_pMainThread->m_dFrontDownMoveLocX[nIndex][0]) && g_pMainThread->CheckSafeAbsMove(AXIS_Y, g_pMainThread->m_dFrontDownMoveLocY[nIndex][0]))
                        {
                            g_Motion.AbsMove(AXIS_X, g_pMainThread->m_dFrontDownMoveLocX[nIndex][0]);
						    g_Motion.AbsMove(AXIS_Y, g_pMainThread->m_dFrontDownMoveLocY[nIndex][0]);
                        }
					}
					else
					{
                        if (g_pMainThread->CheckSafeAbsMove(AXIS_X, g_pMainThread->m_dFrontDownMoveLocX[nIndex][0]) && g_pMainThread->CheckSafeAbsMove(AXIS_Y, g_pMainThread->m_dFrontDownMoveLocY[nIndex][0]))
                        {
						    g_Motion.AbsMove(AXIS_X, g_pMainThread->m_dRearDownMoveLocX[nIndex][0]);
						    g_Motion.AbsMove(AXIS_Y, g_pMainThread->m_dRearDownMoveLocY[nIndex][0]);
                        }
                    }
					break;
				}

				/*
				if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows)
				{
				g_Motion.AbsMove(AXIS_X, dStartX + nCol*g_IniFile.m_dColPitch);
				g_Motion.AbsMove(AXIS_Y, dStartY - nRow*g_IniFile.m_dRowPitch);
				}
				*/
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
#pragma endregion

#pragma region MainManual
void __fastcall TfrmMain::btnStartPressCal0Click(TObject *Sender)
{
    TSpeedButton *pBtn = (TSpeedButton *)Sender;

	if (g_pMainThread->m_bIsAutoMode) return;
    if (!g_pMainThread->m_bIsHomeDone)
	{
		if (g_IniFile.m_nLanguageMode>0) ShowMessage("Please Reset Machine");
		else ShowMessage("請先執行機台原點復歸");
		return;
	}
	if (pBtn->Down == true
        && ((!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassF) && !g_DIO.ReadDIBit(DI::UpperMoldCheck1))
		    || (!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassR) && !g_DIO.ReadDIBit(DI::UpperMoldCheck2))))
	{
		Application->MessageBoxA("請確認 前後上模具護蓋已卸除!!", "Confirm", MB_OK);
        pBtn->Down = false;
		return;
	}

    //Manual Logging
    AnsiString strShow = "手動控制: 按下 主畫面六個控制項 (" + pBtn->HelpKeyword + ")";
    AddChangeLog(strShow.c_str(), strShow.Length());

	//Select if PressCal from 1st location
	bool bFrom1st = false;
	AnsiString pString;
	if (g_IniFile.m_nLanguageMode>0) pString = "PressCal From 1st Location?";
	else pString = "是否重頭開始校正?";
	if (g_pMainThread->m_bAutoRetry && pBtn->Down == true)
	{
		switch (Application->MessageBoxA(pString.c_str(), "Confirm", MB_YESNOCANCEL))
		{
		case IDYES:
            {
                bFrom1st = true;
                pBtn->Down = true;
            }
            break;
		case IDNO:
            {
                bFrom1st = false;
                pBtn->Down = true;
            }
            break;
		case IDCANCEL:
            {
                return;
            }
            break;
		}
	}

	checkMonitor->Checked = false;
	g_pMainThread->m_bIsManualFinish = false;

	//Privilege Set
	if (btnStartPressCal0->Down || btnStartPressCal1->Down ||
		btnLaserUp1->Down || btnLaserUp0->Down ||
		btnLaserDown1->Down || btnLaserDown0->Down)
	{
		MotorTest1->Enabled = false;
		N10->Enabled = false;
	}
	else SetPrivilege(m_nUserLevel);

	//block function switch within one is running
	if (pBtn->Down == true)
	{
		btnStartPressCal0->Enabled = false;
		btnLaserUp0->Enabled = false;
		btnLaserDown0->Enabled = false;
		btnStartPressCal1->Enabled = false;
		btnLaserUp1->Enabled = false;
		btnLaserDown1->Enabled = false;
	}
	else
	{
		btnStartPressCal0->Enabled = true;
		btnLaserUp0->Enabled = true;
		btnLaserDown0->Enabled = true;
		btnStartPressCal1->Enabled = true;
		btnLaserUp1->Enabled = true;
		btnLaserDown1->Enabled = true;
	}
	pBtn->Enabled = true;

	//if PressCal SetAllDevice
	if (g_pMainThread->m_bStartPressCal[0] || g_pMainThread->m_bStartPressCal[1]) SetAllDevice();

	//Protect LoadCell
	g_DIO.SetDO(DO::LoadCellValve, false);

	//if btn down
	if (checkRestartCal->Checked && pBtn->Down)
	{
		g_pMainThread->m_nPressCalMoveIndex[0] = -1;
		g_pMainThread->m_nLaserCalMoveIndex[0] = -1;

		g_pMainThread->m_nPressCalMoveIndex[1] = -1;
		g_pMainThread->m_nLaserCalMoveIndex[1] = -1;

		if (checkAutoRetry->Checked && cmbRange->ItemIndex == 0 && cmbFirstLoc->ItemIndex == 0 && (pBtn->Tag == 0 || pBtn->Tag == 3))
		{
			//Not Reset
			if (bFrom1st)
			{
				if (btnStartPressCal1->Down) { for (int nI = 0; nI<50; nI++) { g_pMainThread->m_arrybDoPressCal[1][nI] = true; } }
				if (btnStartPressCal0->Down) { for (int nI = 0; nI<50; nI++) { g_pMainThread->m_arrybDoPressCal[0][nI] = true; } }
			}
		}
	}
	if (cmbRange->ItemIndex == 1)
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
void __fastcall TfrmMain::SpeedButton1Click(TObject *Sender)
{
	if (g_pMainThread->m_bIsAutoMode) return;
    AnsiString pString;
    if (g_IniFile.m_nLanguageMode>0) pString = "Make sure LoadCell UP??";
    else pString = "是否 上升LoadCell??";
    if (Application->MessageBoxA(pString.c_str(), "Confirm", MB_ICONQUESTION | MB_OKCANCEL) != IDOK) return;
    //Manual Logging
    AnsiString strShow = "手動控制: LoadCell Up.";
    AddChangeLog(strShow.c_str(), strShow.Length());
	g_DIO.SetDO(DO::LoadCellValve, true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SpeedButton2Click(TObject *Sender)
{
	if (g_pMainThread->m_bIsAutoMode) return;
    //Manual Logging
    AnsiString strShow = "手動控制: LoadCell Down.";
    AddChangeLog(strShow.c_str(), strShow.Length());
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
		case IDYES:
            {
            g_pMainThread->m_nIsFullHoming = 1;
            AnsiString strShow = "手動控制: 按下 主畫面 是 全機復歸.";
            AddChangeLog(strShow.c_str(), strShow.Length());

            }
            break;
		case IDNO:
            {
            g_pMainThread->m_nIsFullHoming = 0;
            AnsiString strShow = "手動控制: 按下 主畫面 否 全機復歸.";
            AddChangeLog(strShow.c_str(), strShow.Length());

            }
            break;
		case IDCANCEL:
            {
            g_pMainThread->m_nIsFullHoming = 2;
            AnsiString strShow = "手動控制: 按下 主畫面 取消 全機復歸.";
            AddChangeLog(strShow.c_str(), strShow.Length());

            }
            break;
		}
	}

	//------------------------------------------------------------------------------------------------------------------------
	//For Test


	//g_pMainThread->m_bIsWriteOffsetToDB_F = true;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::checkStopLCClick(TObject *Sender)
{
	g_pMainThread->m_bStopLC = checkStopLC->Checked;
}
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
void __fastcall TfrmMain::btnClearEject0Click(TObject *Sender)
{
	if (Application->MessageBoxA("確定手動排除 後排除流道?", "Confirm", MB_ICONQUESTION | MB_OKCANCEL) != IDOK) return;
	if (g_DIO.ReadDIBit(DI::EjectEntry2) || g_DIO.ReadDIBit(DI::EjectInp2) || g_DIO.ReadDIBit(DI::EjectExit2))
	{
		g_pMainThread->m_listLog.push_back("請確實排除流道上的 Boat");
		return;
	}
	g_pMainThread->nThreadIndex[5] = 0;
	g_DIO.SetDO(DO::ReadyOutR, false);
	g_DIO.SetDO(DO::EjectMotorStart2, false);
	g_DIO.SetDO(DO::EjectStop2, true);
    //Manual Logging
    AnsiString strShow = "手動控制: 按下 主畫面 後排除流道 步驟清除";
    AddChangeLog(strShow.c_str(), strShow.Length());
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnClearEject1Click(TObject *Sender)
{
	if (Application->MessageBoxA("確定手動排除 前排除流道?", "Confirm", MB_ICONQUESTION | MB_OKCANCEL) != IDOK) return;
	if (g_DIO.ReadDIBit(DI::EjectEntry1) || g_DIO.ReadDIBit(DI::EjectInp1) || g_DIO.ReadDIBit(DI::EjectExit1))
	{
		g_pMainThread->m_listLog.push_back("請確實排除流道上的 Boat");
		return;
	}
	g_pMainThread->nThreadIndex[4] = 0;
	g_DIO.SetDO(DO::ReadyOutF, false);
	g_DIO.SetDO(DO::EjectMotorStart1, false);
	g_DIO.SetDO(DO::EjectStop1, true);
    //Manual Logging
    AnsiString strShow = "手動控制: 按下 主畫面前 排除流道 步驟清除";
    AddChangeLog(strShow.c_str(), strShow.Length());
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnClearLCClick(TObject *Sender)
{
	if (Application->MessageBoxA("確定手動排除 LC流道?", "Confirm", MB_ICONQUESTION | MB_OKCANCEL) != IDOK) return;
	if (g_DIO.ReadDIBit(DI::LCInp) || g_DIO.ReadDIBit(DI::LamEntry2) || g_DIO.ReadDIBit(DI::LamEntry1))
	{
		g_pMainThread->m_listLog.push_back("請確實排除流道上的 Boat");
		return;
	}
	g_pMainThread->nThreadIndex[1] = 0;
	g_DIO.SetDO(DO::LCMotorStart, false);
	g_DIO.SetDO(DO::LCStop, true);
    //Manual Logging
    AnsiString strShow = "手動控制: 按下 主畫面LC 步驟清除";
    AddChangeLog(strShow.c_str(), strShow.Length());
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnStartOneStepPressCalClick(TObject *Sender)
{
    C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);
    TSpeedButton *pBtn = (TSpeedButton *)Sender;

    //Manual Logging
    AnsiString strShow = "手動控制: 按下 主畫面一鍵量測校正";
    AddChangeLog(strShow.c_str(), strShow.Length());

	if (g_pMainThread->m_bIsAutoMode) return;
    if (!g_pMainThread->m_bIsHomeDone)
	{
		if (g_IniFile.m_nLanguageMode>0) ShowMessage("Please Reset Machine");
		else ShowMessage("請先執行機台原點復歸");
		return;
	}
    if (pBtn->Down == true
	    && ((!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassF) && !g_DIO.ReadDIBit(DI::UpperMoldCheck1))
		    || (!g_DIO.ReadDOBit(DO::UpperMoldCheckByPassR) && !g_DIO.ReadDIBit(DI::UpperMoldCheck2))))
	{
		Application->MessageBoxA("請確認 前後上模具護蓋已卸除!!", "Confirm", MB_OK);
        pBtn->Down = false;
		return;
	}

	bool bIsStart = pBtn->Down;

	bool bIsContinue = false;
	if (bIsStart)                   //Strat
    {
        //Make sure Param ModuleNum Correct
        g_pMainThread->m_bIsCheckParamModuleNum = true;

        tm1MS.timeStart(10000);
        while (1)
        {
            if (g_IniFile.m_nRailOption == 0 && g_pMainThread->m_bIsCheckParamModuleNumF_OK && g_pMainThread->m_bIsCheckParamModuleNumR_OK) break;
            else if (g_IniFile.m_nRailOption == 1 && g_pMainThread->m_bIsCheckParamModuleNumF_OK) break;
            else if (g_IniFile.m_nRailOption == 2 && g_pMainThread->m_bIsCheckParamModuleNumR_OK) break;
            else if (tm1MS.timeUp())
            {
                g_pMainThread->m_bIsStopCheckParamModuleNum = true;
                //Application->MessageBoxA("請確認 Reader TimeOut!!", "Confirm", MB_OK);
                break;
            }
            Application->ProcessMessages();
        }
        if (!g_pMainThread->m_bIsCheckParamModuleNumF_OK)
        {
            Application->MessageBoxA("請確認 前 上模具編號與產品參數設定相同!!", "Confirm", MB_OK);
		    return;
        }
        if (!g_pMainThread->m_bIsCheckParamModuleNumR_OK)
        {
            Application->MessageBoxA("請確認 後 上模具編號與產品參數設定相同!!", "Confirm", MB_OK);
		    return;
        }


		AnsiString pString;
		if (g_IniFile.m_nLanguageMode>0) pString = "OneStepCal Restrat?";
		else pString = "一鍵量測校正 是否重新開始?";
		switch (Application->MessageBoxA(pString.c_str(), "Confirm", MB_YESNOCANCEL))
		{
		case IDYES:
            {
                bIsContinue = false;
            }
            break;
		case IDNO:
            {
                bIsContinue = true;
            }
            break;
		case IDCANCEL: { return; }
		}
	}
	else						//Stop
	{
		g_pMainThread->m_bStartPressCal[0] = false;
		g_pMainThread->m_bStartPressCal[1] = false;
		g_pMainThread->m_bStartLaserUpCal[1] = false;
		g_pMainThread->m_bStartLaserUpCal[0] = false;
		g_pMainThread->m_bStartLaserDownCal[1] = false;
		g_pMainThread->m_bStartLaserDownCal[0] = false;

		g_pMainThread->m_bStartOneStepCal = false;
	}


	if (g_pMainThread->m_bStartOneStepCal) SetAllDevice();

	g_DIO.SetDO(DO::LoadCellValve, false);

	if (bIsStart && bIsContinue)
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
	else if (bIsStart && !bIsContinue)
	{
		g_pMainThread->nThreadIndex[6] = 0;
		g_pMainThread->nThreadIndex[7] = 0;
		g_pMainThread->nThreadIndex[8] = 0;
		g_pMainThread->nThreadIndex[9] = 0;
		g_pMainThread->nThreadIndex[10] = 0;
		g_pMainThread->nThreadIndex[11] = 0;
		g_pMainThread->nThreadIndex[16] = 0;

		g_pMainThread->m_nPressCalMoveIndex[0] = -1;
		g_pMainThread->m_nLaserCalMoveIndex[0] = -1;

		g_pMainThread->m_nPressCalMoveIndex[1] = -1;
		g_pMainThread->m_nLaserCalMoveIndex[1] = -1;

		//start the btn also update the value of cmb
		g_pMainThread->m_nManualRange = cmbRange->ItemIndex;
		g_pMainThread->m_nManualFirstLoc = cmbFirstLoc->ItemIndex;
		g_pMainThread->m_nManualTimes = cmbTimes->Text.ToInt();

		g_DIO.SetDO(DO::LoadCellValve, false);
		::Sleep(500);
        if (g_pMainThread->CheckSafeAbsMove(AXIS_X, g_IniFile.m_dSafePos))
        {
		    g_Motion.AbsMove(AXIS_Y, g_IniFile.m_dSafePos);
        }

		tm1MS.timeStart(10000);
		while (true)
		{
			if (g_Motion.IsMotionDone(AXIS_Y)) break;
			if (tm1MS.timeUp()) break;
			Application->ProcessMessages();
		}

	}

	g_pMainThread->m_bStartOneStepCal = bIsStart;
	btnStartOneStepPressCal->Down = bIsStart;


	if (btnStartOneStepPressCal->Down)
	{
		MotorTest1->Enabled = false;
		N10->Enabled = false;
	}
	else  SetPrivilege(m_nUserLevel);

	//block function switch within one is running
	if (bIsStart)
	{
		btnStartPressCal0->Enabled = false;
		btnLaserUp0->Enabled = false;
		btnLaserDown0->Enabled = false;
		btnStartPressCal1->Enabled = false;
		btnLaserUp1->Enabled = false;
		btnLaserDown1->Enabled = false;
		btnStartOneStepPressCal->Enabled = false;
	}
	else
	{
		btnStartPressCal0->Enabled = true;
		btnLaserUp0->Enabled = true;
		btnLaserDown0->Enabled = true;
		btnStartPressCal1->Enabled = true;
		btnLaserUp1->Enabled = true;
		btnLaserDown1->Enabled = true;
		btnStartOneStepPressCal->Enabled = true;
	}
	pBtn->Enabled = true;

}
//---------------------------------------------------------------------------

#pragma endregion



