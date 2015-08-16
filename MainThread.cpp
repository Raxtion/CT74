//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainThread.h"
#pragma package(smart_init)

#include "MyPISODIO.h""
#include "C_GetTime.h"
#include "PCIM114.h"
#include "IniFile.h"
#include "math.h"
#include "PISODNM100.h"
#include "DeltaPLC.h"
#include "TA5Serial.h";
#include "DeltaPLC.h"


#include <algorithm>

extern CMyPISODIO g_DIO;
extern CPCIM114 g_Motion;
extern CIniFile g_IniFile;
extern CPISODNM100 g_DNPort0;
extern CPISODNM100 g_DNPort1;
extern CTA5Serial g_Balance;
extern CDeltaPLC g_ModBus;

bool g_bStopMainThread = false;

CMainThread *g_pMainThread;
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall CMainThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall CMainThread::CMainThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{

	m_bRefresh = false;
	m_bIsHomeDone = false;

	m_nPressCalMoveIndex[0] = 0;
	m_nPressCalMoveIndex[1] = 0;
	m_nLaserCalMoveIndex[0] = 0;
	m_nLaserCalMoveIndex[1] = 0;
	m_nManualRange = 0;
	m_nManualFirstLoc = 0;
	m_nManualTimes = 0;
	m_bAutoRetry = false;
	m_bIsHomingFromBtn = false;
	m_bIsManualFinish = true;
	m_nIsFullHoming = -1;
        m_dUnitPerHour1 = 0.0;
        m_dUnitPerHour0 = 0.0;

	m_nPassBoatCount0 = 0;
	m_nPassBoatCount1 = 0;
	m_bStopLC = false;
}
//---------------------------------------------------------------------------
void __fastcall CMainThread::Execute()
{
	//---- Place thread code here ----
	C_GetTime tmReset, tmAlarm, tmResetLamp;

	g_bStopMainThread = false;
	m_bIsDoAutoCal = true;

	bool bHomeDone = false;
	bool bAutoMode = false;

	bool bLastStart = false;
	bool bLastReset = false;

	bool bAlarmLamp = false;
	bool bResetLamp = false;

	bool bStartMachineInit = false;

	//int nThreadIndex[MAX_PROCESS]={0};		//0:Inti 1:Start Measure

	while (1)
	{
		if (g_bStopMainThread) break;

		//Status
		m_bIsHomeDone = bHomeDone;
		m_bIsAutoMode = bAutoMode;
		//

		g_Motion.m_bAutoMode = m_bIsAutoMode;


		//---Start Homing
		if (g_DIO.ReadDIBit(DI::ResetBtn) && !bLastReset) tmReset.timeStart(3000);
		if (bLastReset && tmReset.timeUp())
		{
			bStartMachineInit = true;
			bHomeDone = false;
		}
		bLastReset = g_DIO.ReadDIBit(DI::ResetBtn);
		//---Start Homing from Button
		if (m_bIsHomingFromBtn == true)
		{
			if (g_IniFile.m_nErrorCode != 0)
			{
				g_IniFile.m_nErrorCode = 0;
				g_DIO.SetDO(DO::RedLamp, false);
				g_DIO.SetDO(DO::Buzzer, false);
				m_bIsHomingFromBtn = false;
			}
			else
			{
				if (m_nIsFullHoming == 1)
				{
					bStartMachineInit = true;
					bHomeDone = false;
				}
				else if (m_nIsFullHoming == 0)
				{
					if (g_DIO.ReadDIBit(DI::LifterVac1) || g_DIO.ReadDIBit(DI::LifterVac2)) g_IniFile.m_nErrorCode = 8;
					//else if(g_DIO.ReadDIBit(DI::LaserCheck)) g_IniFile.m_nErrorCode=9;           //Don't Need
					else if (!g_DIO.ReadDIBit(DI::LoadCellDown)) g_IniFile.m_nErrorCode = 10;
					else if (g_DIO.ReadDIBit(DI::LamInp1) || g_DIO.ReadDIBit(DI::LamEntry1) || g_DIO.ReadDIBit(DI::LamWarp1)) g_IniFile.m_nErrorCode = 11;     //bypass
					else if (g_DIO.ReadDIBit(DI::LamInp2) || g_DIO.ReadDIBit(DI::LamEntry2) || g_DIO.ReadDIBit(DI::LamWarp2)) g_IniFile.m_nErrorCode = 12;
					else if (!g_DIO.ReadDIBit(DI::EjectStopUp1)) g_IniFile.m_nErrorCode = 13;
					else if (!g_DIO.ReadDIBit(DI::EjectStopUp2)) g_IniFile.m_nErrorCode = 14;
					else if (g_DIO.ReadDIBit(DI::EjectEntry1) || g_DIO.ReadDIBit(DI::EjectExit1) || g_DIO.ReadDIBit(DI::EjectInp1) || g_DIO.ReadDIBit(DI::EjectExist1)) g_IniFile.m_nErrorCode = 15;
					else if (g_DIO.ReadDIBit(DI::EjectEntry2) || g_DIO.ReadDIBit(DI::EjectExit2) || g_DIO.ReadDIBit(DI::EjectInp2) || g_DIO.ReadDIBit(DI::EjectExist2)) g_IniFile.m_nErrorCode = 16;
					//else if (!g_DIO.ReadDIBit(DI::LCStopUp)) g_IniFile.m_nErrorCode = 17;
					//else if (g_DIO.ReadDIBit(DI::LCInp) || g_DIO.ReadDIBit(DI::LCExist) || g_DIO.ReadDIBit(DI::LCEntry)) g_IniFile.m_nErrorCode = 18;
					else if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB)) g_IniFile.m_nErrorCode = 51;
					else CheckAlarm();
					g_IniFile.m_nErrorCode = 0;

					//nThreadIndex[1] = 0;
					nThreadIndex[2] = 0;
					nThreadIndex[3] = 0;
					nThreadIndex[4] = 0;
					nThreadIndex[5] = 0;
					nThreadIndex[6] = 0;
					nThreadIndex[7] = 0;
					nThreadIndex[8] = 0;
					nThreadIndex[9] = 0;
					nThreadIndex[10] = 0;
					nThreadIndex[11] = 0;
					nThreadIndex[12] = 0;
					nThreadIndex[13] = 0;
					nThreadIndex[14] = 0;
					nThreadIndex[15] = 0;

					bStartMachineInit = false;

					g_IniFile.m_nErrorCode = 1003;
					m_bIsHomingFromBtn = false;
					m_nIsFullHoming = -1;
				}
                                else if (m_nIsFullHoming == 2)
                                {
                                        m_bIsHomingFromBtn = false;
                                        m_nIsFullHoming = -1;
                                }

			}
		}

		//----Alarm Occured
		if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)
		{
			bStartMachineInit = false;
			bAutoMode = false;

			m_bStartPressCal[0] = false;
			m_bStartPressCal[1] = false;
			m_bStartLaserUpCal[0] = false;
			m_bStartLaserUpCal[1] = false;
			m_bStartLaserDownCal[0] = false;
			m_bStartLaserDownCal[1] = false;
			m_bStartLamSub[0] = false;
			m_bStartLamSub[1] = false;
			m_bStartAutoCal[0] = false;
			m_bStartAutoCal[1] = false;
			m_bIsManualFinish = true;

			g_Motion.Stop(0);
			g_Motion.Stop(1);
			g_Motion.Stop(2);
			g_Motion.Stop(3);
			g_Motion.Stop(4);
			g_Motion.Stop(5);
			g_Motion.Stop(6);
			g_Motion.Stop(7);

			g_DIO.SetDO(DO::LCMotorStart, false);
			g_DIO.SetDO(DO::LamMotorStart1, false);
			g_DIO.SetDO(DO::LamMotorStart2, false);
			g_DIO.SetDO(DO::EjectMotorStart1, false);
			g_DIO.SetDO(DO::EjectMotorStart2, false);

			SetManualSpeed();

		}

		//---Reset Alarm
		if (g_DIO.ReadDIBit(DI::ResetBtn))
		{
			g_IniFile.m_nErrorCode = 0;

			g_DIO.SetDO(DO::RedLamp, false);
			g_DIO.SetDO(DO::Buzzer, false);
		}

		//---Buzzer
		if (tmAlarm.timeUp() && ((g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000) || (g_IniFile.m_nErrorCode>2000)))
		{
			g_DIO.SetDO(DO::RedLamp, bAlarmLamp);
			g_DIO.SetDO(DO::Buzzer, bAlarmLamp);
			bAlarmLamp = !bAlarmLamp;
			tmAlarm.timeStart(500);
		}

		//--Stop Auto
		if (g_DIO.ReadDIBit(DI::StopBtn))
		{
			bAutoMode = false;
			SetManualSpeed();

			//g_Motion.Stop(AXIS_FL);
			//g_Motion.Stop(AXIS_RL);
			g_DIO.SetDO(DO::LCMotorStart, false);
			g_DIO.SetDO(DO::LamMotorStart1, false);
			g_DIO.SetDO(DO::LamMotorStart2, false);
			g_DIO.SetDO(DO::EjectMotorStart1, false);
			g_DIO.SetDO(DO::EjectMotorStart2, false);

		}

		//---Homing Process
		if (!bHomeDone && !bAutoMode  && bStartMachineInit)
		{

			bHomeDone = InitialMachine(nThreadIndex[0]);
			if (bHomeDone)
			{
				nThreadIndex[1] = 0;
				nThreadIndex[2] = 0;
				nThreadIndex[3] = 0;
				nThreadIndex[4] = 0;
				nThreadIndex[5] = 0;
				nThreadIndex[6] = 0;
				nThreadIndex[7] = 0;
				nThreadIndex[8] = 0;
				nThreadIndex[9] = 0;
				nThreadIndex[10] = 0;
				nThreadIndex[11] = 0;
				nThreadIndex[12] = 0;
				nThreadIndex[13] = 0;
                                nThreadIndex[14] = 0;
                                nThreadIndex[15] = 0;

				bStartMachineInit = false;
                                m_nIsFullHoming = -1;

                                g_Motion.SetSoftLimit(2, 550.0, -3.0);
                                g_Motion.SetSoftLimit(4, g_IniFile.m_dPLimitF, g_IniFile.m_dNLimitF);
                                g_Motion.SetSoftLimit(5, g_IniFile.m_dPLimitR, g_IniFile.m_dNLimitR);
			}

			CheckAlarm();
			m_bIsHomingFromBtn = false;

		}
		//---AutoMode
		else if (bAutoMode && bHomeDone)
		{
			g_DIO.SetDO(DO::StopBtnLamp, false);
			g_DIO.SetDO(DO::StartBtnLamp, true);
			g_DIO.SetDO(DO::GreenLamp, true);
			g_DIO.SetDO(DO::YellowLamp, false);
			g_DIO.SetDO(DO::RedLamp, false);

			CheckAlarm();
			SetWorkSpeed();

			//do Auto process
			if (m_bStopLC == false)
			{
				DoLaneChanger(nThreadIndex[1]);
			}
			if (g_IniFile.m_nRailOption == 1 || g_IniFile.m_nRailOption == 0)
			{
				int nRemainder;
				if (g_IniFile.m_nAutoInterval == 0 || m_nPassBoatCount1 == 0) nRemainder = -1;
				else nRemainder = m_nPassBoatCount1%g_IniFile.m_nAutoInterval;

				if (nRemainder == 0 && m_bIsDoAutoCal == true && nThreadIndex[2] == 2 && nThreadIndex[4] == 1)
				{

					//Do DoPressCal+detect and DoPressCal when get per Boat
					//g_IniFile.m_nErrorCode = 1002;
                                        DoAutoCal(true, nThreadIndex[14]);
					//m_bIsDoAutoCal = false;
				}
				else
				{
					DoLamination(true, nThreadIndex[2]);
					DoEject(true, nThreadIndex[4]);
				}
			}
			if (g_IniFile.m_nRailOption == 2 || g_IniFile.m_nRailOption == 0)
			{
				int nRemainder;
				if (g_IniFile.m_nAutoInterval == 0 || m_nPassBoatCount0 == 0) nRemainder = -1;
				else nRemainder = m_nPassBoatCount0%g_IniFile.m_nAutoInterval;

				if (nRemainder == 0 && m_bIsDoAutoCal == true && nThreadIndex[3] == 2 && nThreadIndex[5] == 1)
				{
					//Do DoPressCal+detect and DoPressCal when get per Boat
					//g_IniFile.m_nErrorCode = 1002;
                                        DoAutoCal(false, nThreadIndex[15]);
					//m_bIsDoAutoCal = false;
				}
				else
				{
					DoLamination(false, nThreadIndex[3]);
					DoEject(false, nThreadIndex[5]);
				}
			}

		}
		//---Manual Mode
		else if (bHomeDone)
		{
			g_DIO.SetDO(DO::StopBtnLamp, true);
			g_DIO.SetDO(DO::StartBtnLamp, false);
			g_DIO.SetDO(DO::ResetBtnLamp, false);

			SetManualSpeed();

			g_DIO.SetDO(DO::GreenLamp, false);
			g_DIO.SetDO(DO::YellowLamp, true);
			//g_DIO.SetDO(DO::RedLamp,false);

			if (g_DIO.ReadDIBit(DI::StartBtn))                                          //當start綠燈被壓下時
			{
				//nThreadIndex[1]=0;       //some thread need to start from zero

				bAutoMode = true;
				m_nPassBoatCount0 = m_nPassBoatStart;
				m_nPassBoatCount1 = m_nPassBoatStart;
                                CMainThread::tmUPHFront.timeDevStart();                                //start UPH timer
				CMainThread::tmUPHRear.timeDevStart();                                //start UPH timer

				//g_Motion.AbsMove(AXIS_X,g_Motion.m_dLastTargetPos[AXIS_X]);
				//g_Motion.AbsMove(AXIS_Y,g_Motion.m_dLastTargetPos[AXIS_Y]);
				g_Motion.AbsMove(AXIS_LC, g_Motion.m_dLastTargetPos[AXIS_LC]);
				//g_Motion.AbsMove(AXIS_FL, g_Motion.m_dLastTargetPos[AXIS_FL]);   //由手動轉自動過程禁止FL,RL軸回到之前的位置 (這樣會再壓一次)
				//g_Motion.AbsMove(AXIS_RL, g_Motion.m_dLastTargetPos[AXIS_RL]);
				::Sleep(300);
				g_Motion.WaitMotionDone(AXIS_X, 30000);
				g_Motion.WaitMotionDone(AXIS_Y, 30000);
				g_Motion.WaitMotionDone(AXIS_LC, 30000);
				g_Motion.WaitMotionDone(AXIS_FL, 30000);
				g_Motion.WaitMotionDone(AXIS_RL, 30000);

				SetWorkSpeed();

				nThreadIndex[12] = 0;
				nThreadIndex[13] = 0;
			}

			if (m_bStartPressCal[1]) DoPressCal(true, nThreadIndex[6], m_nManualRange, m_nManualFirstLoc, m_nManualTimes);
			else if (m_bStartPressCal[0]) DoPressCal(false, nThreadIndex[7], m_nManualRange, m_nManualFirstLoc, m_nManualTimes);
			else if (m_bStartLaserUpCal[1]) DoLaserCal(true, true, nThreadIndex[8], m_nManualRange, m_nManualFirstLoc, m_nManualTimes);
			else if (m_bStartLaserUpCal[0]) DoLaserCal(false, true, nThreadIndex[9], m_nManualRange, m_nManualFirstLoc, m_nManualTimes);
			else if (m_bStartLaserDownCal[1]) DoLaserCal(true, false, nThreadIndex[10], m_nManualRange, m_nManualFirstLoc, m_nManualTimes);
			else if (m_bStartLaserDownCal[0]) DoLaserCal(false, false, nThreadIndex[11], m_nManualRange, m_nManualFirstLoc, m_nManualTimes);

		}
		else
		{
			if (!bHomeDone && g_DIO.ReadDIBit(DI::StartBtn)) g_IniFile.m_nErrorCode = 29;
			//Announce to Homing 
			if (tmResetLamp.timeUp())
			{
				g_DIO.SetDO(DO::ResetBtnLamp, bResetLamp);
				g_DIO.SetDO(DO::YellowLamp, bResetLamp);
				bResetLamp = !bResetLamp;
				tmResetLamp.timeStart(500);
			}
		}

		//Non Stop Process when Stop
		if (m_bStartLamSub[1] && bHomeDone) DoLamSub(true, nThreadIndex[13]);
		if (m_bStartLamSub[0] && bHomeDone) DoLamSub(false, nThreadIndex[12]);


		::Sleep(10);
	}
}
//---------------------------------------------------------------------------
//復歸使用
bool __fastcall CMainThread::InitialMachine(int &nThreadIndex)
{
	static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);


	switch (nThreadIndex)
	{
	case 0:
                g_Motion.SetDisableSoftLimit(2);
                g_Motion.SetDisableSoftLimit(4);
                g_Motion.SetDisableSoftLimit(5);
		g_DIO.WriteDOPort(0, 0x00);
		g_DIO.WriteDOPort(1, 0x00);
		tm1MS.timeStart(300);
		nThreadIndex++;
		break;
	case 1:
		if (tm1MS.timeUp())
		{
			g_DIO.SetDO(DO::LCStop, true);
			g_DIO.SetDO(DO::EjectStop1, true);
			g_DIO.SetDO(DO::EjectStop2, true);
			g_DIO.SetDO(DO::BigValve1, true);
			g_DIO.SetDO(DO::BigValve2, true);
			tm1MS.timeStart(3000);
			nThreadIndex++;
		}
		break;
	case 2:
		if (tm1MS.timeUp())
		{
			if (g_DIO.ReadDIBit(DI::LifterVac1) || g_DIO.ReadDIBit(DI::LifterVac2)) g_IniFile.m_nErrorCode = 8;
			//else if(g_DIO.ReadDIBit(DI::LaserCheck)) g_IniFile.m_nErrorCode=9;           //Don't Need
			else if (!g_DIO.ReadDIBit(DI::LoadCellDown)) g_IniFile.m_nErrorCode = 10;
			else if (g_DIO.ReadDIBit(DI::LamInp1) || g_DIO.ReadDIBit(DI::LamEntry1) || g_DIO.ReadDIBit(DI::LamWarp1)) g_IniFile.m_nErrorCode = 11;     //bypass
			else if (g_DIO.ReadDIBit(DI::LamInp2) || g_DIO.ReadDIBit(DI::LamEntry2) || g_DIO.ReadDIBit(DI::LamWarp2)) g_IniFile.m_nErrorCode = 12;
			else if (!g_DIO.ReadDIBit(DI::EjectStopUp1)) g_IniFile.m_nErrorCode = 13;
			else if (!g_DIO.ReadDIBit(DI::EjectStopUp2)) g_IniFile.m_nErrorCode = 14;
			else if (g_DIO.ReadDIBit(DI::EjectEntry1) || g_DIO.ReadDIBit(DI::EjectExit1) || g_DIO.ReadDIBit(DI::EjectInp1) || g_DIO.ReadDIBit(DI::EjectExist1)) g_IniFile.m_nErrorCode = 15;
			else if (g_DIO.ReadDIBit(DI::EjectEntry2) || g_DIO.ReadDIBit(DI::EjectExit2) || g_DIO.ReadDIBit(DI::EjectInp2) || g_DIO.ReadDIBit(DI::EjectExist2)) g_IniFile.m_nErrorCode = 16;
			else if (!g_DIO.ReadDIBit(DI::LCStopUp)) g_IniFile.m_nErrorCode = 17;
			else if (g_DIO.ReadDIBit(DI::LCInp) || g_DIO.ReadDIBit(DI::LCExist) || g_DIO.ReadDIBit(DI::LCEntry)) g_IniFile.m_nErrorCode = 18;
			else if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB)) g_IniFile.m_nErrorCode = 51;
			else nThreadIndex++;
		}
		break;
	case 3:
		if (!g_DIO.ReadDIBit(DI::MainAir)) g_IniFile.m_nErrorCode = 6;     //debug
		else
		{
			g_Motion.ServoOn(AXIS_X, true);
			g_Motion.ServoOn(AXIS_Y, true);
			g_Motion.ServoOn(AXIS_LC, true);
			g_Motion.ServoOn(AXIS_FL, true);
			g_Motion.ServoOn(AXIS_RL, true);
			nThreadIndex++;
		}
		break;
	case 4:
		g_IniFile.m_nErrorCode = 1000;    //Start Homing
		g_Motion.SetSpeed(AXIS_X, 0.01, 0.01, 20);
		g_Motion.SetSpeed(AXIS_Y, 0.01, 0.01, 20);
		g_Motion.SetSpeed(AXIS_LC, 0.01, 0.01, 20);
		g_Motion.SetSpeed(AXIS_FL, 0.01, 0.01, 5);
		g_Motion.SetSpeed(AXIS_RL, 0.01, 0.01, 5);

		if (g_Motion.GetStatus(AXIS_X, PCIM114::MEL) || g_Motion.GetStatus(AXIS_X, PCIM114::ORG)) g_Motion.RelMove(AXIS_X, 10.0);
		if (g_Motion.GetStatus(AXIS_Y, PCIM114::MEL) || g_Motion.GetStatus(AXIS_Y, PCIM114::ORG))g_Motion.RelMove(AXIS_Y, 10.0);
		if (g_Motion.GetStatus(AXIS_LC, PCIM114::MEL) || g_Motion.GetStatus(AXIS_LC, PCIM114::ORG))g_Motion.RelMove(AXIS_LC, 10.0);
		if (g_Motion.GetStatus(AXIS_FL, PCIM114::MEL) || g_Motion.GetStatus(AXIS_FL, PCIM114::ORG))g_Motion.RelMove(AXIS_FL, 10.0);
		if (g_Motion.GetStatus(AXIS_RL, PCIM114::MEL) || g_Motion.GetStatus(AXIS_RL, PCIM114::ORG))g_Motion.RelMove(AXIS_RL, 10.0);
		tm1MS.timeStart(30000);
		nThreadIndex++;
		break;
	case 5:
		if (g_Motion.IsMotionDone(AXIS_X) && g_Motion.IsMotionDone(AXIS_Y) && g_Motion.IsMotionDone(AXIS_LC) && g_Motion.IsMotionDone(AXIS_FL) && g_Motion.IsMotionDone(AXIS_RL))
		{
			g_Motion.AxisHome(AXIS_LC);
			g_Motion.AxisHome(AXIS_FL);
			g_Motion.AxisHome(AXIS_RL);

			tm1MS.timeStart(1000 * 30);               //30sec
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 20;
		break;
	case 6:
		if (/*g_Motion.IsMotionDone(AXIS_LC)&&*/ g_Motion.IsMotionDone(AXIS_FL) && g_Motion.IsMotionDone(AXIS_RL))
		{
			g_Motion.AxisHome(AXIS_X);
			g_Motion.AxisHome(AXIS_Y);

			tm1MS.timeStart(1000 * 30);               //30sec
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 21;
		break;
	case 7:
		if (g_Motion.IsMotionDone(AXIS_X) && g_Motion.IsMotionDone(AXIS_Y) && g_Motion.IsMotionDone(AXIS_LC))
		{
			tm1MS.timeStart(1000);
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 22;
		break;


	case 8:
		if (tm1MS.timeUp())
		{
			g_Motion.SetSpeed(AXIS_X, g_IniFile.m_dACCSpeed[AXIS_X], g_IniFile.m_dDECSpeed[AXIS_X], g_IniFile.m_dJogSpeed[AXIS_X]);
			g_Motion.SetSpeed(AXIS_Y, g_IniFile.m_dACCSpeed[AXIS_Y], g_IniFile.m_dDECSpeed[AXIS_Y], g_IniFile.m_dJogSpeed[AXIS_Y]);
			g_Motion.SetSpeed(AXIS_LC, g_IniFile.m_dACCSpeed[AXIS_LC], g_IniFile.m_dDECSpeed[AXIS_LC], g_IniFile.m_dJogSpeed[AXIS_LC]);
			g_Motion.SetSpeed(AXIS_FL, g_IniFile.m_dACCSpeed[AXIS_FL], g_IniFile.m_dDECSpeed[AXIS_FL], g_IniFile.m_dJogSpeed[AXIS_FL]);
			g_Motion.SetSpeed(AXIS_RL, g_IniFile.m_dACCSpeed[AXIS_RL], g_IniFile.m_dDECSpeed[AXIS_RL], g_IniFile.m_dJogSpeed[AXIS_RL]);

			g_Motion.SetActualPos(AXIS_X, 0.0);
			g_Motion.SetCommandPos(AXIS_X, 0.0);
			g_Motion.SetActualPos(AXIS_Y, 0.0);
			g_Motion.SetCommandPos(AXIS_Y, 0.0);
			g_Motion.SetActualPos(AXIS_LC, 0.0);
			g_Motion.SetCommandPos(AXIS_LC, 0.0);
			g_Motion.SetActualPos(AXIS_FL, 0.0);
			g_Motion.SetCommandPos(AXIS_FL, 0.0);
			g_Motion.SetActualPos(AXIS_RL, 0.0);
			g_Motion.SetCommandPos(AXIS_RL, 0.0);

			g_Motion.m_dLastTargetPos[0] = 0.0;
			g_Motion.m_dLastTargetPos[1] = 0.0;
			g_Motion.m_dLastTargetPos[2] = 0.0;
			g_Motion.m_dLastTargetPos[3] = 0.0;
			g_Motion.m_dLastTargetPos[4] = 0.0;
			g_Motion.m_dLastTargetPos[5] = 0.0;
			g_Motion.m_dLastTargetPos[6] = 0.0;
			g_Motion.m_dLastTargetPos[7] = 0.0;

			// to default position
			g_Motion.AbsMove(AXIS_Y, g_IniFile.m_dSafePos);

			nThreadIndex++;
		}
		break;
	case 9:
		if (g_Motion.IsMotionDone(AXIS_Y))
		{
			nThreadIndex++;
		}
		break;
	case 10:
		if (g_Motion.IsMotionDone(AXIS_X) && g_Motion.IsMotionDone(AXIS_Y) && g_Motion.IsMotionDone(AXIS_LC) && g_Motion.IsMotionDone(AXIS_FL) && g_Motion.IsMotionDone(AXIS_RL))
		{
			g_IniFile.m_nErrorCode = 2001;

			//HANDSHAKE INIT
			m_bLamReady[0] = false;
			m_bLamReady[1] = false;
			m_bEjectReady[0] = false;
			m_bEjectReady[1] = false;

			m_bStartPressCal[0] = false;
			m_bStartPressCal[1] = false;
			m_bStartLaserUpCal[0] = false;
			m_bStartLaserUpCal[1] = false;
			m_bStartLaserDownCal[0] = false;
			m_bStartLaserDownCal[1] = false;

			m_bStartLamSub[0] = false;
			m_bStartLamSub[1] = false;

			m_dLamTimer[0] = 0;
			m_dLamTimer[1] = 0;

			nThreadIndex++;
		}
		break;
	default:
		nThreadIndex = 0;
		return true;

	}

	if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)  nThreadIndex = 0;

	return false;

}
//---------------------------------------------------------------------------
//Homing+AutoMode
void __fastcall CMainThread::CheckAlarm()
{
	if (g_Motion.GetStatus(AXIS_X, PCIM114::ALM)) g_IniFile.m_nErrorCode = 1;
	if (g_Motion.GetStatus(AXIS_Y, PCIM114::ALM)) g_IniFile.m_nErrorCode = 2;
	if (g_Motion.GetStatus(AXIS_LC, PCIM114::ALM)) g_IniFile.m_nErrorCode = 3;
	if (g_Motion.GetStatus(AXIS_FL, PCIM114::ALM)) g_IniFile.m_nErrorCode = 4;
	if (g_Motion.GetStatus(AXIS_RL, PCIM114::ALM)) g_IniFile.m_nErrorCode = 7;

	//if(!g_DIO.ReadDIBit(DI::EmgBtn)) g_IniFile.m_nErrorCode=5;     //debug
	if (!g_DIO.ReadDIBit(DI::MainAir)) g_IniFile.m_nErrorCode = 6;  //debug
	if (!g_DIO.ReadDIBit(DI::SafetyDoor) && !g_DIO.ReadDOBit(DO::SafetyDoorByPass)) g_IniFile.m_nErrorCode = 35; //debug
}
//---------------------------------------------------------------------------
//AutoMode+Manual
void __fastcall CMainThread::SetWorkSpeed()
{
	for (int nIndex = 0; nIndex<8; nIndex++)
		g_Motion.SetSpeed(nIndex, g_IniFile.m_dACCSpeed[nIndex], g_IniFile.m_dDECSpeed[nIndex], g_IniFile.m_dWorkSpeed[nIndex]);

}
//---------------------------------------------------------------------------
//Alarm Occured+Stop Auto+Manual Mode
void __fastcall CMainThread::SetManualSpeed()
{
	for (int nIndex = 0; nIndex<8; nIndex++)
		g_Motion.SetSpeed(nIndex, g_IniFile.m_dACCSpeed[nIndex], g_IniFile.m_dDECSpeed[nIndex], g_IniFile.m_dJogSpeed[nIndex]);
}
//---------------------------------------------------------------------------
//上端進料
void __fastcall CMainThread::DoLaneChanger(int &nThreadIndex)
{
	static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);

	static int nLamInp, nLamEntry;

	switch (nThreadIndex)
	{
	case 0:
		if (g_DIO.ReadDIBit(DI::LCEntry)) g_IniFile.m_nErrorCode = 40;
		else if (g_DIO.ReadDIBit(DI::LamEntry1)) g_IniFile.m_nErrorCode = 41;
		else if (g_DIO.ReadDIBit(DI::LamEntry2)) g_IniFile.m_nErrorCode = 42;
		else if (!g_DIO.ReadDIBit(DI::LCStopUp)) g_IniFile.m_nErrorCode = 17;
		else
		{
			g_Motion.AbsMove(AXIS_LC, g_IniFile.m_dLCEntryPos);

			nThreadIndex++;
		}
		break;
	case 1:
		if (g_Motion.IsLastPosDone(AXIS_LC))
		{
			g_DIO.SetDO(DO::ReadyIn1, true);
			nThreadIndex++;
		}
		break;
	case 2:
		if (g_DIO.ReadDIBit(DI::LCEntry))
		{
			g_DIO.SetDO(DO::ReadyIn1, false);
			g_DIO.SetDO(DO::LCMotorStart, true);
			tm1MS.timeStart(5000);
			nThreadIndex++;
		}
		break;
	case 3:
		g_DIO.SetDO(DO::LCMotorStart, true);     //for alarm->restart
		if (g_DIO.ReadDIBit(DI::LCInp) && !g_DIO.ReadDIBit(DI::LCEntry))    //g_DIO.ReadDIBit(DI::LCExist)
		{
			tm1MS.timeStart(300);
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 43;
		break;
	case 4:
		if (tm1MS.timeUp())
		{
			g_DIO.SetDO(DO::LCMotorStart, false);
			nThreadIndex++;
		}
		break;
	case 5:
		if (m_bLamReady[0] || m_bLamReady[1])
		{
			if (m_bLamReady[1])
			{
				g_Motion.AbsMove(AXIS_LC, g_IniFile.m_dLCFrontPos);
				nLamInp = DI::LamInp1;
				nLamEntry = DI::LamEntry1;
			}
			else if (m_bLamReady[0])
			{
				g_Motion.AbsMove(AXIS_LC, g_IniFile.m_dLCRearPos);
				nLamInp = DI::LamInp2;
				nLamEntry = DI::LamEntry2;
			}
			nThreadIndex++;
		}
		break;
	case 6:
		if (g_Motion.IsLastPosDone(AXIS_LC))
		{
			g_DIO.SetDO(DO::LCStop, false);
			tm1MS.timeStart(3000);
			nThreadIndex++;
		}
		break;
	case 7:
		if (g_DIO.ReadDIBit(DI::LCStopDown))
		{
			g_DIO.SetDO(DO::LCMotorStart, true);
			tm1MS.timeStart(5000);
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 68;
		break;
	case 8:
		if (!g_DIO.ReadDIBit(DI::LCInp) && !g_DIO.ReadDIBit(nLamEntry) && g_DIO.ReadDIBit(nLamInp))
		{
			tm1MS.timeStart(1000);
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 44;
		break;
	case 9:
		if (tm1MS.timeUp())
		{
			g_DIO.SetDO(DO::LCMotorStart, false);
			g_DIO.SetDO(DO::LCStop, true);
			tm1MS.timeStart(1000);
			nThreadIndex++;
		}
		break;
	case 10:
		if (g_DIO.ReadDIBit(DI::LCStopUp))
		{
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 17;
		break;
	default:
		nThreadIndex = 0;
	}

	//if(g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)  nThreadIndex=0;
}
//---------------------------------------------------------------------------
//AutoMode壓合前動
void __fastcall CMainThread::DoLamination(bool bFront, int &nThreadIndex)
{
	static C_GetTime tm1MSFront(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm1MSRear(EX_SCALE::TIME_1MS, false);

	C_GetTime *tm1MS;

	int nLamInp, nLamEntry, nLamWarp, nEjectEntry, nLifterVacGauge, nEjectInp;
	int nLamMotorStart, nLifterVac;
	int nAxisLifter;

	if (bFront)
	{
		nLamInp = DI::LamInp1;
		nLamEntry = DI::LamEntry1;
		nLamWarp = DI::LamWarp1;
		nEjectEntry = DI::EjectEntry1;
		nAxisLifter = AXIS_FL;
		nLifterVacGauge = DI::LifterVac1;
		nEjectInp = DI::EjectInp1;

		nLamMotorStart = DO::LamMotorStart1;
		nLifterVac = DO::LifterVac1;

		tm1MS = &tm1MSFront;
	}
	else
	{
		nLamInp = DI::LamInp2;
		nLamEntry = DI::LamEntry2;
		nLamWarp = DI::LamWarp2;
		nEjectEntry = DI::EjectEntry2;
		nAxisLifter = AXIS_RL;
		nLifterVacGauge = DI::LifterVac2;
		nEjectInp = DI::EjectInp2;

		nLamMotorStart = DO::LamMotorStart2;
		nLifterVac = DO::LifterVac2;

		tm1MS = &tm1MSRear;
	}



	switch (nThreadIndex)
	{
	case 0:
		if (g_DIO.ReadDIBit(nLamEntry)) bFront ? g_IniFile.m_nErrorCode = 41 : g_IniFile.m_nErrorCode = 42;
		else if (g_DIO.ReadDIBit(nLamInp)) bFront ? g_IniFile.m_nErrorCode = 45 : g_IniFile.m_nErrorCode = 46;
		else if (g_DIO.ReadDIBit(nLamWarp)) bFront ? g_IniFile.m_nErrorCode = 47 : g_IniFile.m_nErrorCode = 48;
		else if (g_DIO.ReadDIBit(nEjectEntry)) bFront ? g_IniFile.m_nErrorCode = 49 : g_IniFile.m_nErrorCode = 50;
		else if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB)) g_IniFile.m_nErrorCode = 51;
		else
		{
			g_Motion.AbsMove(nAxisLifter, g_IniFile.m_dLamStop[bFront]);
			nThreadIndex++;
		}
		break;
	case 1:
		if (g_Motion.IsLastPosDone(nAxisLifter))
		{
			m_bLamReady[bFront] = true;
			nThreadIndex++;
		}
		break;
	case 2:
		if (g_DIO.ReadDIBit(nLamEntry))                                   //wait the for boat login
		{
                        g_DIO.SetDO(nLamMotorStart, true);
			m_bLamReady[bFront] = false;
			tm1MS->timeStart(3000);
			nThreadIndex++;
		}
		break;
	case 3:
		g_DIO.SetDO(nLamMotorStart, true);
		if (g_DIO.ReadDIBit(nLamInp))
		{
			tm1MS->timeStart(300);
			nThreadIndex++;
		}
		else if (g_DIO.ReadDIBit(nLamWarp)) bFront ? g_IniFile.m_nErrorCode = 47 : g_IniFile.m_nErrorCode = 48;
		else if (tm1MS->timeUp())  bFront ? g_IniFile.m_nErrorCode = 52 : g_IniFile.m_nErrorCode = 53;
		break;
	case 4:
		if (tm1MS->timeUp())
		{
			g_DIO.SetDO(nLamMotorStart, false);
			m_bStartLamSub[bFront] = !g_IniFile.m_bNotLam;

			nThreadIndex++;
		}
		break;
	case 5:                                                     //stop here and wait LamSub finished
		if (!m_bStartLamSub[bFront])
		{
			tm1MS->timeStart(300);
			nThreadIndex++;
		}
		break;
	case 6:
		if (tm1MS->timeUp())
		{
			g_Motion.AbsMove(nAxisLifter, 0.0);
			nThreadIndex++;
		}
		break;

	case 7:
		if (g_Motion.IsLastPosDone(nAxisLifter))
		{
			m_bIsDoAutoCal = true;
                        if (bFront) m_nPassBoatCount1++;
                        else m_nPassBoatCount0++;
			nThreadIndex++;
		}
		break;
	case 8:
		if (m_bEjectReady[bFront])
		{
			g_DIO.SetDO(nLamMotorStart, true);
			tm1MS->timeStart(5000);
			nThreadIndex++;
		}
		break;
	case 9:
		if (g_DIO.ReadDIBit(nEjectInp))
		{
			g_DIO.SetDO(nLamMotorStart, false);
			nThreadIndex++;
		}
		else if (tm1MS->timeUp())  bFront ? g_IniFile.m_nErrorCode = 56 : g_IniFile.m_nErrorCode = 57;
		break;
	default:
		nThreadIndex = 0;
	}
}
//---------------------------------------------------------------------------
//AutoMode壓合後動(為不被STOP命令終止，寫外面)
void __fastcall CMainThread::DoLamSub(bool bFront, int &nThreadIndex)
{
	static C_GetTime tm1MSFront(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm1MSRear(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm2MSFront(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm2MSRear(EX_SCALE::TIME_1MS, false);
	static bool bVacError = false;

	C_GetTime *tm1MS;
	C_GetTime *tm2MS;

	int nLamInp, nLamEntry, nLamWarp, nEjectEntry, nLifterVacGauge, nEjectInp;
	int nLamMotorStart, nLifterVac;
	int nAxisLifter;

	if (bFront)
	{
		nLamInp = DI::LamInp1;
		nLamEntry = DI::LamEntry1;
		nLamWarp = DI::LamWarp1;
		nEjectEntry = DI::EjectEntry1;
		nAxisLifter = AXIS_FL;
		nLifterVacGauge = DI::LifterVac1;
		nEjectInp = DI::EjectInp1;

		nLamMotorStart = DO::LamMotorStart1;
		nLifterVac = DO::LifterVac1;

		tm1MS = &tm1MSFront;
		tm2MS = &tm2MSFront;

	}
	else
	{
		nLamInp = DI::LamInp2;
		nLamEntry = DI::LamEntry2;
		nLamWarp = DI::LamWarp2;
		nEjectEntry = DI::EjectEntry2;
		nAxisLifter = AXIS_RL;
		nLifterVacGauge = DI::LifterVac2;
		nEjectInp = DI::EjectInp2;

		nLamMotorStart = DO::LamMotorStart2;
		nLifterVac = DO::LifterVac2;

		tm1MS = &tm1MSRear;
		tm2MS = &tm2MSRear;
	}
	//及時更新畫面時間
	//if ((g_IniFile.m_dLamTime[bFront] * 1000 - tm1MS->timeStartTime())<0.0) m_dLamTimer[bFront] = 0.0;
	//else m_dLamTimer[bFront] = g_IniFile.m_dLamTime[bFront] * 1000 - tm1MS->timeStartTime();

	switch (nThreadIndex)
	{
	case 0:
		//if(tm1MS->timeUp())
	{
		g_DIO.SetDO(nLamMotorStart, false);
		g_Motion.AbsMove(nAxisLifter, g_IniFile.m_dLamHeight[bFront]);

		nThreadIndex++;
	}
	break;
	case 1:
		if (g_Motion.GetActualPos(nAxisLifter)>(g_IniFile.m_dLamStop[bFront] - 5.0))
		{
			g_Motion.ChangeSpeed(nAxisLifter, 10.0);
			nThreadIndex++;
		}
		break;
	case 2:
		if (g_Motion.GetActualPos(nAxisLifter)>(g_IniFile.m_dLamStop[bFront] + 5.0))
		{
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dWorkSpeed[nAxisLifter]);
			nThreadIndex++;
		}
		break;
	case 3:
		if (g_Motion.GetActualPos(nAxisLifter)>(g_IniFile.m_dLamHeight[bFront] - g_IniFile.m_dLamSecondHeight[bFront]))
		{
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dLamSecondHeight[bFront] / g_IniFile.m_dLamSecondTime[bFront]);
			g_DIO.SetDO(nLifterVac, true);
			tm1MS->timeStart(2000);
			nThreadIndex++;
		}
		break;
	case 4:   //20150803 加入第一次真空判別 有成功不反應 失敗時有警訊不停機
		if (g_DIO.ReadDIBit(nLifterVacGauge))
		{
			nThreadIndex++;
		}
		else if (tm1MS->timeUp())
		{
			bFront ? g_IniFile.m_nErrorCode = 2054 : g_IniFile.m_nErrorCode = 2055;
			nThreadIndex++;
		}
		break;
	case 5: //20150721 加入上升第三段變速  小於1mm就降速為1mm跑完需要60秒
		//20150803 加入進入第三段 開始倒數壓合、Buzzer OFF、VacDelayTimeStart
		if (g_Motion.GetActualPos(nAxisLifter)>(g_IniFile.m_dLamHeight[bFront] - g_IniFile.m_dLamThirdHeight[bFront]))
		{
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dLamThirdHeight[bFront] / g_IniFile.m_dLamThirdTime[bFront]);

			tm1MS->timeStart(g_IniFile.m_dLamTime[bFront] * 1000);

			if (g_IniFile.m_nErrorCode > 2000)
			{
				g_IniFile.m_nErrorCode = 0;
				g_DIO.SetDO(DO::Buzzer, false);

				g_DIO.SetDO(DO::StopBtnLamp, false);
				g_DIO.SetDO(DO::StartBtnLamp, true);
				g_DIO.SetDO(DO::GreenLamp, true);
				g_DIO.SetDO(DO::YellowLamp, false);
				g_DIO.SetDO(DO::RedLamp, false);
			}

			tm2MS->timeStart(g_IniFile.m_dVacDelayTime * 1000);

			nThreadIndex++;
		}
		break;
	case 6:
		if (tm2MS->timeUp() || tm1MS->timeUp())
		{
			tm2MS->timeStart(3000);
			nThreadIndex++;
		}
		m_dLamTimer[bFront] = g_IniFile.m_dLamTime[bFront] * 1000 - tm1MS->timeStartTime();
		break;
	case 7:
		if (g_DIO.ReadDIBit(nLifterVacGauge)) //bypass
		{
			bVacError = false;
			nThreadIndex++;
		}
		else if (tm2MS->timeUp())
		{
			bVacError = true;
			nThreadIndex++;
		}
		m_dLamTimer[bFront] = g_IniFile.m_dLamTime[bFront] * 1000 - tm1MS->timeStartTime();
		break;
	case 8:	//壓合時間到
		if (tm1MS->timeUp())
		{
			g_Motion.Stop(nAxisLifter);
			nThreadIndex++;
		}
		m_dLamTimer[bFront] = g_IniFile.m_dLamTime[bFront] * 1000 - tm1MS->timeStartTime();
		break;
	case 9: //是否停下
		if (g_Motion.IsMotionDone(nAxisLifter))
		{
			g_Motion.AbsMove(nAxisLifter, g_IniFile.m_dLamGetPos[bFront]);
			g_Motion.ChangeSpeed(nAxisLifter, 10.0);
			m_dLamTimer[bFront] = 0;
			nThreadIndex++;
		}
		break;
	case 10:
		if (g_Motion.GetActualPos(nAxisLifter)<(g_IniFile.m_dLamHeight[bFront] - g_IniFile.m_dLamSecondHeight[bFront]))
		{
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dWorkSpeed[nAxisLifter]);
			g_DIO.SetDO(nLifterVac, false);
			tm1MS->timeStart(1000);
			g_Motion.Stop(nAxisLifter);
			nThreadIndex++;
		}
		else g_Motion.AbsMove(nAxisLifter, g_IniFile.m_dLamGetPos[bFront]);
		break;
	case 11:
		if (tm1MS->timeUp())
		{
			g_Motion.AbsMove(nAxisLifter, g_IniFile.m_dLamGetPos[bFront]);
			m_dLamTimer[bFront] = 0.0;
			nThreadIndex++;
		}
	case 12:
		if (g_Motion.GetActualPos(nAxisLifter)<(g_IniFile.m_dLamGetPos[bFront] + 5.0))
		{
			g_Motion.ChangeSpeed(nAxisLifter, 10.0);
			//g_DIO.SetDO(nLifterVac, false);
			nThreadIndex++;
		}
		break;

	case 13:
		if (g_Motion.IsPosDone(nAxisLifter, g_IniFile.m_dLamGetPos[bFront]))
		{
			nThreadIndex++;
		}
		break;
	case 14:
		if (bVacError)
		{
			bFront ? g_IniFile.m_nErrorCode = 54 : g_IniFile.m_nErrorCode = 55;
                        m_bStartLamSub[bFront] = false;
                        nThreadIndex = 0;
		}
		nThreadIndex++;
		break;
	default:
		m_bStartLamSub[bFront] = false;
		nThreadIndex = 0;
	}
}
//---------------------------------------------------------------------------
//下端退料
void __fastcall CMainThread::DoEject(bool bFront, int &nThreadIndex)
{
	static C_GetTime tm1MSFront(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm1MSRear(EX_SCALE::TIME_1MS, false);

	C_GetTime *tm1MS;

	int nEjectInp, nEjectEntry, nEjectExit, nEjectExist, nEjectStopUp, nEjectStopDown, nReadyOut;
	int nEjectMotorStart, nEjectStop;

        double dPassTimeFront;
        double dPassTimeRear;

	if (bFront)
	{
		nEjectInp = DI::EjectInp1;
		nEjectEntry = DI::EjectEntry1;
		nEjectExit = DI::EjectExit1;
		nEjectExist = DI::EjectExist1;
		nEjectStopUp = DI::EjectStopUp1;
		nEjectStopDown = DI::EjectStopDown1;
		nReadyOut = DI::ReadyOut1;

		nEjectMotorStart = DO::EjectMotorStart1;
		nEjectStop = DO::EjectStop1;

		tm1MS = &tm1MSFront;
	}
	else
	{
		nEjectInp = DI::EjectInp2;
		nEjectEntry = DI::EjectEntry2;
		nEjectExit = DI::EjectExit2;
		nEjectExist = DI::EjectExist2;
		nEjectStopUp = DI::EjectStopUp2;
		nEjectStopDown = DI::EjectStopDown2;
		nReadyOut = DI::ReadyOut2;

		nEjectMotorStart = DO::EjectMotorStart2;
		nEjectStop = DO::EjectStop2;

		tm1MS = &tm1MSRear;
	}


	switch (nThreadIndex)
	{
	case 0:
		if (g_DIO.ReadDIBit(nEjectInp)) bFront ? g_IniFile.m_nErrorCode = 59 : g_IniFile.m_nErrorCode = 58;
		else if (g_DIO.ReadDIBit(nEjectEntry)) bFront ? g_IniFile.m_nErrorCode = 61 : g_IniFile.m_nErrorCode = 60;
		else if (g_DIO.ReadDIBit(nEjectExit)) bFront ? g_IniFile.m_nErrorCode = 63 : g_IniFile.m_nErrorCode = 62;
		//else if(g_DIO.ReadDIBit(nEjectExist)) bFront ? g_IniFile.m_nErrorCode=65: g_IniFile.m_nErrorCode=64;
		else if (!g_DIO.ReadDIBit(nEjectStopUp)) bFront ? g_IniFile.m_nErrorCode = 13 : g_IniFile.m_nErrorCode = 14;
		else
		{
			m_bEjectReady[bFront] = true;
			nThreadIndex++;
		}
		break;
	case 1:
		if (g_DIO.ReadDIBit(nEjectEntry))
		{
			m_bEjectReady[bFront] = false;
			g_DIO.SetDO(nEjectMotorStart, true);
			tm1MS->timeStart(5000);
			nThreadIndex++;
		}
		break;
	case 2:
		if (g_DIO.ReadDIBit(nEjectInp) && !g_DIO.ReadDIBit(nEjectEntry))               //g_DIO.ReadDIBit(nEjectExist)
		{
			tm1MS->timeStart(500);
			nThreadIndex++;
		}
		else if (tm1MS->timeUp()) bFront ? g_IniFile.m_nErrorCode = 56 : g_IniFile.m_nErrorCode = 57;
		break;
	case 3:
		if (tm1MS->timeUp())
		{
			g_DIO.SetDO(nEjectMotorStart, false);
			nThreadIndex++;
		}
		break;
	case 4:
		if (g_DIO.ReadDIBit(nReadyOut) || g_IniFile.m_bForceEject)        //bypass
		{
                        if (bFront)
                        {
                                dPassTimeFront = CMainThread::tmUPHFront.timeDevEnd();
                                CMainThread::tmUPHFront.timeDevStart();
                        }
                        else
                        {
                                dPassTimeRear = CMainThread::tmUPHRear.timeDevEnd();
                                CMainThread::tmUPHRear.timeDevStart();
                        }
                        m_dUnitPerHour1 = ( (g_IniFile.m_nCols * g_IniFile.m_nRows)/ dPassTimeFront) * 3600 * 1000;
                        m_dUnitPerHour0 = ( (g_IniFile.m_nCols * g_IniFile.m_nRows)/ dPassTimeRear) * 3600 * 1000;

			g_DIO.SetDO(nEjectStop, false);
			tm1MS->timeStart(3000);
			nThreadIndex++;
		}
		break;
	case 5:
		if (g_DIO.ReadDIBit(nEjectStopDown))
		{
			g_DIO.SetDO(nEjectMotorStart, true);
			tm1MS->timeStart(10000);
			nThreadIndex++;
		}
		else if (tm1MS->timeUp()) bFront ? g_IniFile.m_nErrorCode = 67 : g_IniFile.m_nErrorCode = 66;
		break;
	case 6:
		if (!g_DIO.ReadDIBit(nEjectInp) && !g_DIO.ReadDIBit(nEjectEntry) && !g_DIO.ReadDIBit(nEjectExit) && !g_DIO.ReadDIBit(nEjectExist))
		{
			g_DIO.SetDO(nEjectMotorStart, false);
			g_DIO.SetDO(nEjectStop, true);
			tm1MS->timeStart(3000);
			nThreadIndex++;

		}
		else if (tm1MS->timeUp()) bFront ? g_IniFile.m_nErrorCode = 67 : g_IniFile.m_nErrorCode = 66;
		break;
	case 7:
		if (g_DIO.ReadDIBit(nEjectStopUp))
		{
			nThreadIndex++;
		}
		else if (tm1MS->timeUp()) bFront ? g_IniFile.m_nErrorCode = 13 : g_IniFile.m_nErrorCode = 14;
		break;
	default:
		nThreadIndex = 0;
	}
}
//---------------------------------------------------------------------------
//手動量測壓力
void __fastcall CMainThread::DoPressCal(bool bFront, int &nThreadIndex,
	int nManualRange, int nManualFirstLoc, int nManualTimes)
{
	static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);

	const int nTagA = 1000;

	double dStartX, dStartY;
	CPISODNM100 *pDNPort;

	static int nMoveIndex = 0;
	static int nTryTimes = 0;                                        //
	static double dNewValue = 0;
	static int nTimes = 0;                                           //次數

	if (m_nPressCalMoveIndex[bFront] == -1)                          // 當打勾從頭開始  就傳入起始位置
	{
		//nMoveIndex = 0;
		nMoveIndex = nManualFirstLoc;
		nThreadIndex = 0;
		nTimes = 0;
	}

	m_nPressCalMoveIndex[bFront] = nMoveIndex;

	double *pOffset;
	double *pLoadCellValue;

	if (bFront)
	{
		dStartX = g_IniFile.m_dLoadCellPosX[bFront];
		dStartY = g_IniFile.m_dLoadCellPosY[bFront];

		pDNPort = &g_DNPort0;

		pOffset = g_IniFile.m_dScaleOffsetFront;
		pLoadCellValue = m_dFrontPressCal;
	}
	else
	{
		dStartX = g_IniFile.m_dLoadCellPosX[bFront];
		dStartY = g_IniFile.m_dLoadCellPosY[bFront];

		pDNPort = &g_DNPort1;

		pOffset = g_IniFile.m_dScaleOffsetRear;
		pLoadCellValue = m_dRearPressCal;
	}

	switch (nThreadIndex)
	{
	case 0:
		if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[0] || g_Motion.GetActualPos(AXIS_RL)> g_IniFile.m_dLamStop[1]) g_IniFile.m_nErrorCode = 69;
		else if (!g_DIO.ReadDIBit(DI::LoadCellDown)) g_IniFile.m_nErrorCode = 10;
		//else if(g_DIO.ReadDIBit(DI::LaserCheck)) g_IniFile.m_nErrorCode=9;    //don't need
		else
		{
			m_nCalCol = nMoveIndex % 10;
			m_nCalRow = nMoveIndex / 10;


			if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
			{
				//nTryTimes=0;
				m_listLog.push_back("開始重量校正程序-->" + FormatFloat("0", nMoveIndex + 1));

				if (nTryTimes == 0)
				{
					//dNewValue = g_IniFile.m_dLamPress[bFront];
					if (m_bAutoRetry == true)
					{
						dNewValue = g_IniFile.m_dLamPress[bFront];
					}
					else
					{
						dNewValue = pOffset[nMoveIndex] + g_IniFile.m_dLamPress[bFront];
					}
					m_listLog.push_back("Set Data=" + FormatFloat("0.00 Kg/cm2", dNewValue));
					pDNPort->SetKg(nMoveIndex, dNewValue);
					pDNPort->WriteAllData();
				}

				g_Motion.AbsMove(AXIS_X, dStartX + m_nCalCol*g_IniFile.m_dColPitch);
				g_Motion.AbsMove(AXIS_Y, dStartY - m_nCalRow*g_IniFile.m_dRowPitch);
				nThreadIndex++;
			}
			else
			{
				if (nManualRange == 0)                                                                  //0=整盤,1=單顆
				{                                                                                    //整盤時不引用cmbFirstLoc
					nMoveIndex++;
					nThreadIndex = nTagA;
				}
				else
				{
					nThreadIndex = nTagA;
				}
			}

		}
		break;
	case 1:
		if (g_Motion.IsPosDone(AXIS_X, dStartX + m_nCalCol*g_IniFile.m_dColPitch) && g_Motion.IsPosDone(AXIS_Y, dStartY - m_nCalRow*g_IniFile.m_dRowPitch))
		{
			if (/*g_DIO.ReadDIBit(DI::LaserCheck)*/false) g_IniFile.m_nErrorCode = 9;      //bypass
			else
			{
				g_DIO.SetDO(DO::LoadCellValve, true);
				tm1MS.timeStart(3000);
				nThreadIndex++;
			}
		}
		break;
	case 2:
		if (g_DIO.ReadDIBit(DI::LoadCellUp))
		{
			tm1MS.timeStart(g_IniFile.m_dPressCalTime * 1000);          //Stable Time
			nThreadIndex++;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 70;
		break;
	case 3:
		if (tm1MS.timeUp())
		{
			//Get Load Cell Value

			double dLoadCellValue = g_Balance.GetKg(1);       //Kg
			m_listLog.push_back("重量=" + FormatFloat("0.00 Kg", dLoadCellValue));
			
			
			if (m_bAutoRetry == false)
			{
				//--自動情況------------------------------------------------------------------------------------------------------
				if (m_bIsAutoMode && m_bIsHomeDone)
				{
					if (dLoadCellValue > g_IniFile.m_dLamPress[bFront] * (1.0 + (g_IniFile.m_dPressCalRange / 100.0) * g_IniFile.m_dAutoStopTimes) ||
						dLoadCellValue < g_IniFile.m_dLamPress[bFront] * (1.0 - (g_IniFile.m_dPressCalRange / 100.0) * g_IniFile.m_dAutoStopTimes))
					{
						m_listLog.push_back("重量超過容許誤差" + FormatFloat("0.00 倍", g_IniFile.m_dAutoStopTimes) + "，必須停機");
						g_IniFile.m_nErrorCode = 74;
					}
					else if (dLoadCellValue > g_IniFile.m_dLamPress[bFront] * (1.0 + (g_IniFile.m_dPressCalRange / 100.0) * 1.0) ||
							 dLoadCellValue < g_IniFile.m_dLamPress[bFront] * (1.0 - (g_IniFile.m_dPressCalRange / 100.0) * 1.0))
					{
						m_listLog.push_back("重量超過容許誤差，未超過" + FormatFloat("0.00 倍", g_IniFile.m_dAutoStopTimes) + "，必須重新校正");
						g_IniFile.m_nErrorCode = 1005;
					}
				}

				if (nManualRange == 0)                                                                  //0=整盤,1=單顆
				{
					nMoveIndex++;
				}
				nTryTimes = 0;
				nThreadIndex = nTagA;
			}
			else
			{
				if (dLoadCellValue > g_IniFile.m_dLamPress[bFront] * (1.0 + g_IniFile.m_dPressCalRange / 100.0) ||
					dLoadCellValue < g_IniFile.m_dLamPress[bFront] * (1.0 - g_IniFile.m_dPressCalRange / 100.0))
				{
					m_listLog.push_back("NG-->Try Again");
					//dNewValue=dNewValue*dNewValue/dLoadCellValue;            //
					dNewValue = dNewValue*(1 + (g_IniFile.m_dLamPress[bFront] - dLoadCellValue) / g_IniFile.m_dLamPress[bFront]);
					if (dNewValue <= 0)  dNewValue = 0.1;
					m_listLog.push_back("Set Data=" + FormatFloat("0.00 Kg/cm2", dNewValue));
					pDNPort->SetKg(nMoveIndex, dNewValue);
					pDNPort->WriteAllData();


					nTryTimes++;
					if (nTryTimes<10)
					{
						//tm1MS.timeStart(g_IniFile.m_dPressCalTime*1000);     //valve stable time
						nThreadIndex = nTagA;
					}
					else (m_bIsAutoMode && m_bIsHomeDone) ? g_IniFile.m_nErrorCode = 1004 : g_IniFile.m_nErrorCode = 71;
				}
				else
				{

					pOffset[nMoveIndex] = dNewValue - g_IniFile.m_dLamPress[bFront];

					pLoadCellValue[nMoveIndex] = dLoadCellValue;
					//dNewValue=g_IniFile.m_dLamPress[bFront];
					m_listLog.push_back("OK");

					if (nManualRange == 0)                                                                  //0=整盤,1=單顆
					{                                                                                    //整盤時不引用cmbFirstLoc
						nMoveIndex++;
					}
					nTryTimes = 0;
					nThreadIndex = nTagA;

				}
			}
		}
		break;
	case 4:
	case nTagA: nThreadIndex = 4;
	{
		g_DIO.SetDO(DO::LoadCellValve, false);
		tm1MS.timeStart(10000);
		nThreadIndex++;
	}
	break;
	case 5:
		if (g_DIO.ReadDIBit(DI::LoadCellDown))
		{
			//nMoveIndex++;
			//if (nMoveIndex<50) nThreadIndex = 0;
			//else nThreadIndex++;
			if (nManualRange == 0)
			{
				if (nMoveIndex<50)
				{
					nThreadIndex = 0;
				}
				else if (nTimes < nManualTimes)
				{
					nTimes++;
					nThreadIndex = 0;
					nMoveIndex = 0;
				}
				else
				{
					nThreadIndex++;
					nTimes = 0;
				}
			}
			else
			{
				if (nTimes < nManualTimes)
				{
					nTimes++;
					nThreadIndex = 0;
				}
				else
				{
					nThreadIndex++;
					nTimes = 0;
				}
			}

		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 10;
		break;
	case 6:
		g_Motion.AbsMove(AXIS_Y, g_IniFile.m_dSafePos);
		nThreadIndex++;
		break;
	case 7:
		if (g_Motion.IsPosDone(AXIS_Y, g_IniFile.m_dSafePos))
		{
			nMoveIndex = 0;
			nTryTimes = 0;
			g_IniFile.m_nErrorCode = 72;
		}
		break;
	default:
		m_bStartAutoCal[bFront] = false;
		nThreadIndex = 0;
	}

	if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)
	{
		//nMoveIndex=0;
		nThreadIndex = 0;
		nTryTimes = 0;
		g_DIO.SetDO(DO::LoadCellValve, false);
	}
}
//---------------------------------------------------------------------------
//手動量測高度
void __fastcall CMainThread::DoLaserCal(bool bFront, bool bUp, int &nThreadIndex,
	int nManualRange, int nManualFirstLoc, int nManualTimes)
{
	static C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);

	const int nTagA = 1000;


	double dStartX[4] = { 0 };
	double dStartY[4] = { 0 };
	int nLaserChannel;
	double *p_dLaserValue;

	static int nMoveIndex = 0;
	static int nMoveIndexSub = 0;
	bool bIsUpLaser = false;

	static int nTimes = 0;                                           //次數

	if (m_nLaserCalMoveIndex[bFront] == -1)                          // 當不打勾從頭開始  就傳入起始位置
	{
		//nMoveIndex = 0;
		nMoveIndex = nManualFirstLoc;
		nMoveIndexSub = 0;
		nThreadIndex = 0;
		nTimes = 0;
	}

	m_nLaserCalMoveIndex[bFront] = nMoveIndex;

	if (bFront)
	{

		if (bUp)
		{
			dStartX[0] = g_IniFile.m_dLaserUpPosX[bFront][0];
			dStartY[0] = g_IniFile.m_dLaserUpPosY[bFront][0];
			dStartX[1] = g_IniFile.m_dLaserUpPosX[bFront][1];
			dStartY[1] = g_IniFile.m_dLaserUpPosY[bFront][1];
			dStartX[2] = g_IniFile.m_dLaserUpPosX[bFront][2];
			dStartY[2] = g_IniFile.m_dLaserUpPosY[bFront][2];
			dStartX[3] = g_IniFile.m_dLaserUpPosX[bFront][3];
			dStartY[3] = g_IniFile.m_dLaserUpPosY[bFront][3];
			nLaserChannel = 0;

			p_dLaserValue = (double *)m_dFrontUpperLaser;

			bIsUpLaser = true;

		}
		else
		{
			dStartX[0] = g_IniFile.m_dLaserDownPosX[bFront];
			dStartY[0] = g_IniFile.m_dLaserDownPosY[bFront];
			nLaserChannel = 1;

			p_dLaserValue = (double *)m_dFrontDownLaser;
		}

	}
	else
	{
		if (bUp)
		{
			dStartX[0] = g_IniFile.m_dLaserUpPosX[bFront][0];
			dStartY[0] = g_IniFile.m_dLaserUpPosY[bFront][0];
			dStartX[1] = g_IniFile.m_dLaserUpPosX[bFront][1];
			dStartY[1] = g_IniFile.m_dLaserUpPosY[bFront][1];
			dStartX[2] = g_IniFile.m_dLaserUpPosX[bFront][2];
			dStartY[2] = g_IniFile.m_dLaserUpPosY[bFront][2];
			dStartX[3] = g_IniFile.m_dLaserUpPosX[bFront][3];
			dStartY[3] = g_IniFile.m_dLaserUpPosY[bFront][3];
			nLaserChannel = 0;

			p_dLaserValue = (double *)m_dRearUpperLaser;

			bIsUpLaser = true;
		}
		else
		{
			dStartX[0] = g_IniFile.m_dLaserDownPosX[bFront];
			dStartY[0] = g_IniFile.m_dLaserDownPosY[bFront];
			nLaserChannel = 1;

			p_dLaserValue = (double *)m_dRearDownLaser;
		}


	}

	switch (nThreadIndex)
	{
	case 0:
		if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[0] || g_Motion.GetActualPos(AXIS_RL)> g_IniFile.m_dLamStop[1]) g_IniFile.m_nErrorCode = 69;
		else if (!g_DIO.ReadDIBit(DI::LoadCellDown)) g_IniFile.m_nErrorCode = 10;
		//else if(g_DIO.ReadDIBit(DI::LaserCheck)) g_IniFile.m_nErrorCode=9;      //don't need
		{
			m_nCalCol = nMoveIndex % 10;
			m_nCalRow = nMoveIndex / 10;

			if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
			{
				if (nMoveIndex == 0 && nMoveIndexSub == 0) memset(p_dLaserValue, 0, sizeof(double) * 50);
				m_listLog.push_back("開始Laser 量測程序-->" + FormatFloat("0", nMoveIndex + 1));

				g_Motion.AbsMove(AXIS_X, dStartX[nMoveIndexSub] + m_nCalCol*g_IniFile.m_dColPitch);
				g_Motion.AbsMove(AXIS_Y, dStartY[nMoveIndexSub] - m_nCalRow*g_IniFile.m_dRowPitch);
				nThreadIndex++;
			}
			else nThreadIndex = nTagA;

		}
		break;
	case 1:
		if (g_Motion.IsPosDone(AXIS_X, dStartX[nMoveIndexSub] + m_nCalCol*g_IniFile.m_dColPitch) && g_Motion.IsPosDone(AXIS_Y, dStartY[nMoveIndexSub] - m_nCalRow*g_IniFile.m_dRowPitch))
		{
			if (/*g_DIO.ReadDIBit(DI::LaserCheck)*/false) g_IniFile.m_nErrorCode = 9;         //bypass
			else
			{
				tm1MS.timeStart(1000);
				nThreadIndex++;
			}
		}
		break;
	case 2:
		if (tm1MS.timeUp())
		{
			double dLaserData = 0.0;
			dLaserData = g_ModBus.GetAnalogData(3, bUp);
			if (g_ModBus.m_bInitOK)
			{
				p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = dLaserData;

				m_listLog.push_back("高度=" + FormatFloat("0.00 mm", dLaserData));
			}
			else
			{
				g_ModBus.m_bInitOK = true;
				p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = 0.0;
				m_listLog.push_back("高度=N/A");
			}

			nThreadIndex++;
		}
		break;
	case 3:
	case nTagA:nThreadIndex = 3;
		{
			if (!bIsUpLaser)
			{
				if (nManualRange == 0) nMoveIndex++;                                              //0=整盤,1=單顆
				nMoveIndexSub = 0;
			}
			else
			{
				nMoveIndexSub++;
				if (nMoveIndexSub>3)
				{
					if (nManualRange == 0) nMoveIndex++;                                      //0=整盤,1=單顆
					nMoveIndexSub = 0;
				}
			}

			//if (nMoveIndex<50) nThreadIndex = 0;
			//else nThreadIndex++;
			if (nManualRange == 0)                                                           //0=整盤,1=單顆  
			{
				if (nMoveIndex<50)
				{
					nThreadIndex = 0;
				}
				else if (nTimes < nManualTimes)
				{
					nTimes++;
					nThreadIndex = 0;
					nMoveIndex = 0;
				}
				else
				{
					nThreadIndex++;
					nTimes = 0;
				}
			}
			else
			{
				if (nTimes < nManualTimes)
				{
					nTimes++;
					nThreadIndex = 0;
				}
				else
				{
					nThreadIndex++;
					nTimes = 0;
				}
			}
		}
		break;
	case 4:
		g_Motion.AbsMove(AXIS_Y, g_IniFile.m_dSafePos);
		nThreadIndex++;
		break;
	case 5:
		if (g_Motion.IsPosDone(AXIS_Y, g_IniFile.m_dSafePos))
		{
			nMoveIndex = 0;
			g_IniFile.m_nErrorCode = 73;
		}
		break;
	default:
		nThreadIndex = 0;
	}

	if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)
	{
		//nMoveIndex=0;
		nThreadIndex = 0;
	}
}
//---------------------------------------------------------------------------
//自動校正
void __fastcall CMainThread::DoAutoCal(bool bFront, int &nThreadIndex)
{
	static C_GetTime tm1MSFront(EX_SCALE::TIME_1MS, false);
	static C_GetTime tm1MSRear(EX_SCALE::TIME_1MS, false);

	C_GetTime *tm1MS;

	int nLamInp, nLamEntry, nLamWarp, nEjectEntry, nEjectInp, nEjectExit, nEjectExist;
	int nAxisLifter;
        int nPassBoatCount;

        const int nTagA = 1000;

	if (bFront)
	{
		nLamInp = DI::LamInp1;
		nLamEntry = DI::LamEntry1;
		nLamWarp = DI::LamWarp1;

		nAxisLifter = AXIS_FL;

		nEjectInp = DI::EjectInp1;
		nEjectEntry = DI::EjectEntry1;
		nEjectExit = DI::EjectExit1;
		nEjectExist = DI::EjectExist1;


		tm1MS = &tm1MSFront;
		nPassBoatCount = m_nPassBoatCount1;
	}
	else
	{
		nLamInp = DI::LamInp2;
		nLamEntry = DI::LamEntry2;
		nLamWarp = DI::LamWarp2;

		nAxisLifter = AXIS_RL;
		
		nEjectInp = DI::EjectInp2;
		nEjectEntry = DI::EjectEntry2;
		nEjectExit = DI::EjectExit2;
		nEjectExist = DI::EjectExist2;

		tm1MS = &tm1MSRear;
		nPassBoatCount = m_nPassBoatCount0;
	}

	switch (nThreadIndex)
	{
	case 0:
		if (g_DIO.ReadDIBit(nLamEntry)) bFront ? g_IniFile.m_nErrorCode = 41 : g_IniFile.m_nErrorCode = 42;
		else if (g_DIO.ReadDIBit(nLamInp)) bFront ? g_IniFile.m_nErrorCode = 45 : g_IniFile.m_nErrorCode = 46;
		else if (g_DIO.ReadDIBit(nLamWarp)) bFront ? g_IniFile.m_nErrorCode = 47 : g_IniFile.m_nErrorCode = 48;
		else if (g_DIO.ReadDIBit(nEjectEntry)) bFront ? g_IniFile.m_nErrorCode = 49 : g_IniFile.m_nErrorCode = 50;
		else if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB)) g_IniFile.m_nErrorCode = 51;
		else
		{
			g_Motion.AbsMove(nAxisLifter, 0.0);
			tm1MS->timeStart(3000);
			nThreadIndex++;
		}
		break;
	case 1:
		if (g_Motion.IsLastPosDone(nAxisLifter))
		{
			m_bLamReady[bFront] = false;
                        //tm1MS->timeStart(3000);
			nThreadIndex++;
		}
		else if (tm1MS->timeUp())  bFront ? g_IniFile.m_nErrorCode = 21 : g_IniFile.m_nErrorCode = 21;
		break;
	case 2:
		m_bStartAutoCal[bFront] = true;
		m_bAutoRetry = false;
		if (bFront == true)
		{
			//m_nManualRange = 0; m_nManualFirstLoc = 0; m_nManualTimes = 0;
			if (m_bStartAutoCal[bFront]) DoPressCal(true, CMainThread::nThreadIndex[6], 0, 0, 0);
			else nThreadIndex++;
		}
		else
		{
			//m_nManualRange = 0; m_nManualFirstLoc = 0; m_nManualTimes = 0;
			if (m_bStartAutoCal[bFront]) DoPressCal(false, CMainThread::nThreadIndex[7], 0, 0, 0);
			else nThreadIndex++;
		}
		break;
	case 3:
		if (m_bStartAutoCal[bFront] == false)
		{
			if (g_IniFile.m_nErrorCode == 0)
			{
				nThreadIndex = nTagA;
			}
			else if (g_IniFile.m_nErrorCode == 1005)
			{
				nThreadIndex++;
			}
		}
		break;
	case 4:
		m_bStartAutoCal[bFront] = true;
		m_bAutoRetry = true;
		if (bFront == true)
		{
			//m_nManualRange = 0; m_nManualFirstLoc = 0; m_nManualTimes = 0;
			if (m_bStartAutoCal[bFront]) DoPressCal(true, CMainThread::nThreadIndex[6], 0, 0, 0);
			else nThreadIndex++;
		}
		else
		{
			//m_nManualRange = 0; m_nManualFirstLoc = 0; m_nManualTimes = 0;
			if (m_bStartAutoCal[bFront]) DoPressCal(false, CMainThread::nThreadIndex[7], 0, 0, 0);
			else nThreadIndex++;
		}
		break;
	case 5:
		if (m_bStartAutoCal[bFront] == false)
		{
			g_IniFile.m_nErrorCode = 0;
			nThreadIndex++;
		}
		break;
	case 6:
		m_bStartAutoCal[bFront] = true;
		m_bAutoRetry = false;
		if (bFront == true)
		{
			//m_nManualRange = 0; m_nManualFirstLoc = 0; m_nManualTimes = 0;
			if (m_bStartAutoCal[bFront]) DoPressCal(true, CMainThread::nThreadIndex[6], 0, 0, 0);
			else nThreadIndex++;
		}
		else
		{
			//m_nManualRange = 0; m_nManualFirstLoc = 0; m_nManualTimes = 0;
			if (m_bStartAutoCal[bFront]) DoPressCal(false, CMainThread::nThreadIndex[7], 0, 0, 0);
			else nThreadIndex++;
		}
		break;
	case 7:
		if (m_bStartAutoCal[bFront] == false)
		{
			if (g_IniFile.m_nErrorCode == 0)
			{
				nThreadIndex++;
			}
			else if (g_IniFile.m_nErrorCode == 1005)
			{
				g_IniFile.m_nErrorCode == 74;
				nThreadIndex++;
			}
		}
		break;
	case 8:
	case nTagA: nThreadIndex = 8;
		g_Motion.AbsMove(AXIS_Y, g_IniFile.m_dSafePos);
		nThreadIndex++;
		break;
	case 9:
		if (g_Motion.IsPosDone(AXIS_Y, g_IniFile.m_dSafePos))
		{
			g_IniFile.m_nErrorCode = 1006;
		}
		break;

	default:
                m_bIsDoAutoCal = false;
		nThreadIndex = 0;
	}

	if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)
	{	
		nThreadIndex = 0;
	}
}






