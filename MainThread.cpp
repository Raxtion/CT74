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
#include "time.h"
#include "EQPXML.h"
#include <algorithm>

extern CMyPISODIO g_DIO;
extern CPCIM114 g_Motion;
extern CIniFile g_IniFile;
extern CPISODNM100 g_DNPort0;
extern CPISODNM100 g_DNPort1;
extern CTA5Serial g_Balance;
extern CDeltaPLC g_ModBus;
extern CEQPXML g_eqpXML;

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
    m_bPrassNeedReCal = false;

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
    m_nPressCalPassCount = 0;

	m_nPassBoatCount0 = 0;
	m_nPassBoatCount1 = 0;
	m_bStopLC = false;
    m_bStartAgain = false;
    m_bStopAgain = false;
    m_bResetAgain = false;

    m_dUpperLaserRealTime = 0.0;
    m_dDownLaserRealTime = 0.0;
    m_dFrontTempRealTime = 0.0;
    m_dRearTempRealTime = 0.0;

    m_dFirstLaserValueUp = 0.0;
    m_dFirstLaserValueDown = 0.0;

	m_bIsStartProcessbyDIO = false;
	m_bIsStartProcessbyCIM = false;
	m_bIsStopProcessbyCIM = false;

    m_dFirstNewValue = 0.5;
}
//---------------------------------------------------------------------------
void __fastcall CMainThread::Execute()
{
	//---- Place thread code here ----
	C_GetTime tmReset, tmAlarm, tmResetLamp;

	g_bStopMainThread = false;
	m_bIsDoAutoCal[0] = false;
    m_bIsDoAutoCal[1] = false;

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
		g_Motion.m_bAutoMode = m_bIsAutoMode;


		//---Start Homing
		if (g_DIO.ReadDIBit(DI::ResetBtn) && !bLastReset) tmReset.timeStart(3000);
		if (m_bIsAutoMode==false && bLastReset && tmReset.timeUp())
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

                    if (!g_DIO.ReadDIBit(DI::LCStopUp) && g_DIO.ReadDIBit(DI::LCExist) == false) {nThreadIndex[1] = 0; g_DIO.SetDO(DO::LCStop, true);}
					//nThreadIndex[1] = 0;               //LC動作不被清除(Boat不好拿出來)
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

                    m_bStartLamSub[1] = false;
                    m_bStartLamSub[0] = false;
					bStartMachineInit = false;

					g_IniFile.m_nErrorCode = 1003;
					m_bIsHomingFromBtn = false;
					m_nIsFullHoming = -1;
                    m_nPassBoatCount1 == 0;
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
			//m_bStartLamSub[0] = false;
			//m_bStartLamSub[1] = false;
			m_bStartAutoCal[0] = false;
			m_bStartAutoCal[1] = false;
			m_bIsManualFinish = true;

			g_Motion.Stop(0);
			g_Motion.Stop(1);
			g_Motion.Stop(2);
			g_Motion.Stop(3);
			//g_Motion.Stop(4);
			//g_Motion.Stop(5);
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
		if (g_DIO.ReadDIBit(DI::ResetBtn) && m_bResetAgain)
		{
			g_IniFile.m_nErrorCode = 0;
            if (m_bResetAgain == true && bHomeDone) m_listLog.push_back("機台重置");
			g_DIO.SetDO(DO::RedLamp, false);
			g_DIO.SetDO(DO::Buzzer, false);
            m_bResetAgain = false;
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
		if ((g_eqpXML.m_CIMStatus.ToInt()!=2 && (g_DIO.ReadDIBit(DI::StopBtn) && m_bStopAgain)) || m_bIsStopProcessbyCIM)
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
            g_DIO.SetDO(DO::ReadyIn1, false);
            if (m_bStopAgain == true)
            {
                m_listLog.push_back("機台停止");
                m_ActionLog.push_back(AddTimeString("[Execute]由DIO停止"));
            }
            if (m_bIsStopProcessbyCIM == true)
            {
                m_listLog.push_back("機台停止byCIM");
                m_ActionLog.push_back(AddTimeString("[Execute]由CIM停止"));
            }
			m_bIsStopProcessbyCIM = false;
            m_bStopAgain = false;
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

                g_Motion.SetSoftLimit(0, 293.0, -1.99);
                g_Motion.SetSoftLimit(1, 584.0, -8.0);
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

				if (nRemainder == 0 && m_bIsDoAutoCal[1] == true && m_bIsDoAutoCal[0] == false && nThreadIndex[2] == 2 && nThreadIndex[4] == 1)
				{
					//Do DoPressCal+detect and DoPressCal when get per Boat
					DoAutoCal(true, nThreadIndex[14]);
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

				if (nRemainder == 0 && m_bIsDoAutoCal[0] == true && m_bIsDoAutoCal[1] == false && nThreadIndex[3] == 2 && nThreadIndex[5] == 1)
				{
					//Do DoPressCal+detect and DoPressCal when get per Boat
					DoAutoCal(false, nThreadIndex[15]);
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

			//當start綠燈被壓下時，或者CIM觸發StartProcess function
			switch (g_eqpXML.m_CIMStatus.ToInt())
			{
			case 0:                                                                              //Offline
				if (g_DIO.ReadDIBit(DI::StartBtn) && m_bStartAgain)
				{
					m_listLog.push_back("機台啟動");
					m_bIsStartProcessbyDIO = true;
					m_bStartAgain = false;
				}
				break;
			case 1:                                                                              //online/local
				if (g_DIO.ReadDIBit(DI::StartBtn) && m_bStartAgain)
                {
					g_eqpXML.SendEventReport("115");
					m_listLog.push_back("發送啟動請求，等待CIM回應...");
					m_bStartAgain = false;
                }
				break;
			case 2:                                                                             //online/remote
				if (g_DIO.ReadDIBit(DI::StartBtn) && m_bStartAgain)
                {
					m_listLog.push_back("CIM online/remote啟動無效...");
					m_bStartAgain = false;
                }
				break;
			default:
				break;
            }
			if (m_bIsStartProcessbyDIO || m_bIsStartProcessbyCIM)
			{
                if (m_bIsStartProcessbyDIO == true) m_ActionLog.push_back(AddTimeString("[Execute]由DIO啟動"));
                else if (m_bIsStartProcessbyCIM == true) m_ActionLog.push_back(AddTimeString("[Execute]由CIM啟動"));

                if (m_bStopLC == true && g_IniFile.m_nRailOption == 0
                    && !g_DIO.ReadDIBit(DI::LamInp1) && !g_DIO.ReadDIBit(DI::LamEntry1) && !g_DIO.ReadDIBit(DI::LamWarp1)
                    && !g_DIO.ReadDIBit(DI::EjectInp1) && !g_DIO.ReadDIBit(DI::EjectEntry1) && !g_DIO.ReadDIBit(DI::EjectExit1)
                    && !g_DIO.ReadDIBit(DI::LamInp2) && !g_DIO.ReadDIBit(DI::LamEntry2) && !g_DIO.ReadDIBit(DI::LamWarp2)
                    && !g_DIO.ReadDIBit(DI::EjectInp2) && !g_DIO.ReadDIBit(DI::EjectEntry2) && !g_DIO.ReadDIBit(DI::EjectExit2))
                {
                    g_IniFile.m_nErrorCode = 33;
                }
                else if (m_bStopLC == true && g_IniFile.m_nRailOption == 1
                        && !g_DIO.ReadDIBit(DI::LamInp1) && !g_DIO.ReadDIBit(DI::LamEntry1) && !g_DIO.ReadDIBit(DI::LamWarp1)
                        && !g_DIO.ReadDIBit(DI::EjectInp1) && !g_DIO.ReadDIBit(DI::EjectEntry1) && !g_DIO.ReadDIBit(DI::EjectExit1))
                {
                    g_IniFile.m_nErrorCode = 33;
                }
                else if (m_bStopLC == true && g_IniFile.m_nRailOption == 2
                        && !g_DIO.ReadDIBit(DI::LamInp2) && !g_DIO.ReadDIBit(DI::LamEntry2) && !g_DIO.ReadDIBit(DI::LamWarp2)
                        && !g_DIO.ReadDIBit(DI::EjectInp2) && !g_DIO.ReadDIBit(DI::EjectEntry2) && !g_DIO.ReadDIBit(DI::EjectExit2))
                {
                    g_IniFile.m_nErrorCode = 33;
                }

                //nThreadIndex[1]=0;       //some thread need to start from zero
                bAutoMode = true;

                //m_nPassBoatCount0 = m_nPassBoatStart;                           //If auto 從0開始
                //m_nPassBoatCount1 = m_nPassBoatStart;
                m_nPassBoatCount0 = 0;
                m_nPassBoatCount1 = 0;

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

				m_bIsStartProcessbyCIM = false;
				m_bIsStartProcessbyDIO = false;
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
        m_ActionLog.push_back(AddTimeString("[InitialMachine][0]復歸開始"));
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
            m_ActionLog.push_back(AddTimeString("[InitialMachine][2]確認IO"));
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
            m_ActionLog.push_back(AddTimeString("[InitialMachine][3]開啟伺服馬達"));
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
            m_ActionLog.push_back(AddTimeString("[InitialMachine][8]伺服馬達設定參數"));
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
            m_ActionLog.push_back(AddTimeString("[InitialMachine][10]機台變數初始化"));
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
AnsiString CMainThread::AddTimeString(AnsiString Input)
{
    AnsiString strDateTime;
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);
    strDateTime.sprintf("[%2d:%02d:%02d]",tblock->tm_hour,tblock->tm_min, tblock->tm_sec);
    strDateTime = strDateTime+Input;
    return strDateTime;
}
//---------------------------------------------------------------------------
AnsiString CMainThread::AddTimeString(bool bFront,AnsiString Input)
{
    AnsiString strFront;
    if (bFront) strFront = "1";
    else strFront = "0";
    AnsiString strDateTime;
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);
    strDateTime.sprintf("[%2d:%02d:%02d]",tblock->tm_hour,tblock->tm_min, tblock->tm_sec);
    strDateTime = strDateTime+"["+strFront+"]"+Input;
    return strDateTime;
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
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][0]LC 移動至接料區"));
			nThreadIndex++;
		}
		break;
	case 1:
		if (g_Motion.IsLastPosDone(AXIS_LC))
		{
			g_DIO.SetDO(DO::ReadyIn1, true);
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][1]LC 移動接料區到位"));
			nThreadIndex++;
		}
		break;
	case 2:
		if (g_DIO.ReadDIBit(DI::LCEntry))
		{
			g_DIO.SetDO(DO::ReadyIn1, false);
			g_DIO.SetDO(DO::LCMotorStart, true);
			tm1MS.timeStart(10000);
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][2]LC 接料區進料開始"));
			nThreadIndex++;
		}
		break;
	case 3:
		if (g_DIO.ReadDIBit(DI::LCInp) && !g_DIO.ReadDIBit(DI::LCEntry))    //g_DIO.ReadDIBit(DI::LCExist)
		{
			tm1MS.timeStart(300);
			nThreadIndex++;
		}
        else if (!g_DIO.ReadDOBit(DO::LCMotorStart))
        {
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][3]LC 接料區進料失敗重試"));
            g_DIO.SetDO(DO::LCMotorStart, true);
			tm1MS.timeStart(10000);
            break;
		}
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 43;
		break;
	case 4:
		if (tm1MS.timeUp())
		{
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][4]LC 接料區進料到位"));
			g_DIO.SetDO(DO::LCMotorStart, false);
			nThreadIndex++;
		}
		break;
	case 5:
		if (m_bLamReady[0] || m_bLamReady[1])
		{
			if (m_bLamReady[1])
			{
                m_ActionLog.push_back(AddTimeString("[DoLaneChanger][5]LC 前出料區Ready"));
				g_Motion.AbsMove(AXIS_LC, g_IniFile.m_dLCFrontPos);
				nLamInp = DI::LamInp1;
				nLamEntry = DI::LamEntry1;
			}
			else if (m_bLamReady[0])
			{
                m_ActionLog.push_back(AddTimeString("[DoLaneChanger][5]LC 後出料區Ready"));
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
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][6]LC 出料區到位"));
			g_DIO.SetDO(DO::LCStop, false);
			tm1MS.timeStart(3000);
			nThreadIndex++;
		}
		break;
	case 7:
		if (g_DIO.ReadDIBit(DI::LCStopDown))
		{
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][7]LC 出料區Stop汽缸下降"));
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
        else if (!g_DIO.ReadDOBit(DO::LCMotorStart))
        {
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][8]LC 出料區出料失敗重試"));
            g_DIO.SetDO(DO::LCMotorStart, true);
			tm1MS.timeStart(5000);
			//nThreadIndex++;
            break;
        }
		else if (tm1MS.timeUp()) g_IniFile.m_nErrorCode = 44;
		break;
	case 9:
		if (tm1MS.timeUp())
		{
            m_ActionLog.push_back(AddTimeString("[DoLaneChanger][9]LC 出料區出料到位"));
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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][0]Lane Lifter上升到接片高度"));
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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][2]Lane 進料區Ready"));
			g_DIO.SetDO(nLamMotorStart, true);
			m_bLamReady[bFront] = false;
			tm1MS->timeStart(5000);
			nThreadIndex++;
		}
		break;
	case 3:
		//g_DIO.SetDO(nLamMotorStart, true);
		if (g_DIO.ReadDIBit(nLamInp))
		{
			tm1MS->timeStart(300);
			nThreadIndex++;
		}
        else if (!g_DIO.ReadDOBit(nLamMotorStart))
        {
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][3]Lane 進料區進料失敗重試"));
            g_DIO.SetDO(nLamMotorStart, true);
            tm1MS->timeStart(5000);
			//nThreadIndex++;
            break;
        }
		else if (g_DIO.ReadDIBit(nLamWarp)) bFront ? g_IniFile.m_nErrorCode = 47 : g_IniFile.m_nErrorCode = 48;
		else if (tm1MS->timeUp())  bFront ? g_IniFile.m_nErrorCode = 52 : g_IniFile.m_nErrorCode = 53;
		break;
	case 4:
		if (tm1MS->timeUp())
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][4]Lane 進料區成功開始壓合"));
			g_DIO.SetDO(nLamMotorStart, false);
			m_bStartLamSub[bFront] = !g_IniFile.m_bNotLam;

			nThreadIndex++;
		}
		break;
	case 5:                                                     //stop here and wait LamSub finished
		if (!m_bStartLamSub[bFront])
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][5]Lane 壓合流程結束"));
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
			if (bFront) { m_nPassBoatCount1++; m_bIsDoAutoCal[1] = true; }
			else { m_nPassBoatCount0++; m_bIsDoAutoCal[0] = true; }
			nThreadIndex++;
		}
		break;
	case 8:
		if (m_bEjectReady[bFront])
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][8]Lane 出料區Ready"));
			g_DIO.SetDO(nLamMotorStart, true);
			tm1MS->timeStart(5000);
			nThreadIndex++;
		}
		break;
	case 9:
		if (g_DIO.ReadDIBit(nEjectInp))
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][9]Lane 出料區出料成功"));
			g_DIO.SetDO(nLamMotorStart, false);
			nThreadIndex++;
		}
        else if (!g_DIO.ReadDOBit(nLamMotorStart))
        {
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamination][9]Lane 出料區出料失敗重試"));
            g_DIO.SetDO(nLamMotorStart, true);
			tm1MS->timeStart(5000);
			//nThreadIndex++;
            break;
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
	CPISODNM100 *pDNPort;

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
        pDNPort = &g_DNPort0;
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
        pDNPort = &g_DNPort1;
	}

	switch (nThreadIndex)
	{
	case 0:
		//if(tm1MS->timeUp())
	{
        m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][0]LamSub 壓合開始移動至LamHeight"));
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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][2]LamSub lifter接觸boat"));
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dWorkSpeed[nAxisLifter]);
			nThreadIndex++;
		}
		break;
	case 3:
		if (g_Motion.GetActualPos(nAxisLifter)>(g_IniFile.m_dLamHeight[bFront] - g_IniFile.m_dLamSecondHeight[bFront]))
		{
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dLamSecondHeight[bFront] / g_IniFile.m_dLamSecondTime[bFront]);
            if (g_IniFile.m_nVacummOn == 0)
                m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][3]LamSub lifter進入第二段真空不開啟"));
            else
            {
                m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][3]LamSub lifter進入第二段真空開啟"));
			    g_DIO.SetDO(nLifterVac, true);
            }

            // add PressDownPercent  (slowdown)
            /*for (int nIndex = 0; nIndex<50; nIndex++)
            {
        	    if (bFront) pDNPort->SetKg(nIndex, (g_IniFile.m_dLamPress[1] + g_IniFile.m_dScaleOffsetFront[nIndex])*(100-g_IniFile.m_nDownPercent)/100);
                else pDNPort->SetKg(nIndex, (g_IniFile.m_dLamPress[0] + g_IniFile.m_dScaleOffsetRear[nIndex])*(100-g_IniFile.m_nDownPercent)/100);
            }
            pDNPort->WriteAllData();
            */

			tm1MS->timeStart(2000);
			nThreadIndex++;
		}
		break;
	case 4:   //20150803 加入第一次真空判別 有成功不反應 失敗時有警訊不停機
        if (g_IniFile.m_nVacummOn == 0)
        {
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][4]LamSub 第一次真空不偵測"));
            m_bFirstVacSuccessed = true;
            nThreadIndex++;
        }
        else
        {
		    if (g_DIO.ReadDIBit(nLifterVacGauge))
		    {
                m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][4]LamSub 第一次真空偵測成功"));
                m_bFirstVacSuccessed = true;
			    nThreadIndex++;
		    }
		    else if (tm1MS->timeUp())
		    {
                m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][4]LamSub 第一次真空偵測失敗"));
                if (bFront) m_listLog.push_back("前壓合流道真空失敗(第一次)");
                else m_listLog.push_back("後壓合流道真空失敗(第一次)");
			    //bFront ? g_IniFile.m_nErrorCode = 2054 : g_IniFile.m_nErrorCode = 2055;
                m_bFirstVacSuccessed = false;
                g_DIO.SetDO(DO::Buzzer, true);
			    nThreadIndex++;
		    }
        }
		break;
	case 5: //20150721 加入上升第三段變速  小於1mm就降速為1mm跑完需要60秒
		//20150803 加入進入第三段 開始倒數壓合、Buzzer OFF、VacDelayTimeStart
		if (g_Motion.GetActualPos(nAxisLifter)>(g_IniFile.m_dLamHeight[bFront] - g_IniFile.m_dLamThirdHeight[bFront]))
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][5]LamSub lifter進入第三段"));
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dLamThirdHeight[bFront] / g_IniFile.m_dLamThirdTime[bFront]);

			tm1MS->timeStart(g_IniFile.m_dLamTime[bFront] * 1000);

            if (m_bFirstVacSuccessed == false) g_DIO.SetDO(DO::Buzzer, false);
			tm2MS->timeStart(g_IniFile.m_dVacDelayTime * 1000);

            // add Reload PressDownPercent (slowdown)
            /*for (int nIndex = 0; nIndex<50; nIndex++)
            {
        	    if (bFront) pDNPort->SetKg(nIndex, (g_IniFile.m_dLamPress[1] + g_IniFile.m_dScaleOffsetFront[nIndex]));
                else pDNPort->SetKg(nIndex, (g_IniFile.m_dLamPress[0] + g_IniFile.m_dScaleOffsetRear[nIndex]));
            }
            pDNPort->WriteAllData();
            */
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
        if (g_IniFile.m_nVacummOn == 0)
        {
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][7]LamSub 第二次真空不偵測"));
            bVacError = false;
            nThreadIndex++;
        }
        else
        {
		    if (g_DIO.ReadDIBit(nLifterVacGauge))
		    {
                m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][7]LamSub 第二次真空偵測成功"));
			    bVacError = false;
			    nThreadIndex++;
		    }
		    else if (tm2MS->timeUp())
		    {
                m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][7]LamSub 第二次真空偵測失敗"));
			    bVacError = true;
			    nThreadIndex++;
		    }
        }
		m_dLamTimer[bFront] = g_IniFile.m_dLamTime[bFront] * 1000 - tm1MS->timeStartTime();
		break;
	case 8:	//壓合時間到
		if (tm1MS->timeUp())
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][8]LamSub 壓合時間到移動至m_dLamGetPos"));
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
			m_dLamTimer[bFront] = 0.0;
			nThreadIndex++;
		}
		break;
	case 10:
		if (g_Motion.GetActualPos(nAxisLifter)<(g_IniFile.m_dLamHeight[bFront] - g_IniFile.m_dLamSecondHeight[bFront]))
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][10]LamSub lifter進入第一段真空關閉"));
			g_Motion.ChangeSpeed(nAxisLifter, g_IniFile.m_dWorkSpeed[nAxisLifter]);
			g_DIO.SetDO(nLifterVac, false);
			tm1MS->timeStart(1000);
			g_Motion.Stop(nAxisLifter);
			nThreadIndex++;
		}
		break;
	case 11:
		if (tm1MS->timeUp())
		{
			g_Motion.AbsMove(nAxisLifter, g_IniFile.m_dLamGetPos[bFront]);
			m_dLamTimer[bFront] = 0.0;
			nThreadIndex++;
		}
        break;
	case 12:
		if (g_Motion.GetActualPos(nAxisLifter)<(g_IniFile.m_dLamGetPos[bFront] + 5.0))
		{
			g_Motion.ChangeSpeed(nAxisLifter, 10.0);
			nThreadIndex++;
		}
		break;
	case 13:
		if (g_Motion.IsPosDone(nAxisLifter, g_IniFile.m_dLamGetPos[bFront]))
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLamSub][13]LamSub lifter到達m_dLamGetPos"));
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

	int nEjectInp, nEjectEntry, nEjectExit, nEjectExist, nEjectStopUp, nEjectStopDown, nReadyOut, nReadyOutY;
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
        nReadyOutY = DO::ReadyOutF;

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
        nReadyOutY = DO::ReadyOutR;

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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoEject][0]EjectLane 進料區Ready"));
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
            m_bEjectReady[bFront] = false;
			tm1MS->timeStart(500);
			nThreadIndex++;
		}
        else if (!g_DIO.ReadDOBit(nEjectMotorStart))
        {
            m_ActionLog.push_back(AddTimeString(bFront, "[DoEject][2]EjectLane 進料區進料失敗重試"));
            m_bEjectReady[bFront] = true;
            g_DIO.SetDO(nEjectMotorStart, true);
			tm1MS->timeStart(5000);
            break;
        }
		else if (tm1MS->timeUp()) bFront ? g_IniFile.m_nErrorCode = 56 : g_IniFile.m_nErrorCode = 57;
		break;
	case 3:
		if (tm1MS->timeUp())
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoEject][3]EjectLane 進料區進料成功"));
            g_DIO.SetDO(nReadyOutY, true);
			g_DIO.SetDO(nEjectMotorStart, false);
			nThreadIndex++;
		}
		break;
	case 4:
		if (g_DIO.ReadDIBit(nReadyOut) || g_IniFile.m_bForceEject)        //bypass
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoEject][3]EjectLane 出料區Ready"));
			if (bFront)
			{
					dPassTimeFront = CMainThread::tmUPHFront.timeDevEnd();
					CMainThread::tmUPHFront.timeDevStart();
					if (dPassTimeFront != 0.0) m_dUnitPerHour1 = ( (g_IniFile.m_nCols * g_IniFile.m_nRows)/ dPassTimeFront) * 3600 * 1000;
					else m_dUnitPerHour1 = 0.0;
			}
			else
			{
					dPassTimeRear = CMainThread::tmUPHRear.timeDevEnd();
					CMainThread::tmUPHRear.timeDevStart();
					if (dPassTimeRear != 0.0) m_dUnitPerHour0 = ( (g_IniFile.m_nCols * g_IniFile.m_nRows)/ dPassTimeRear) * 3600 * 1000;
					else m_dUnitPerHour0 = 0.0;
			}

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
			tm1MS->timeStart(3000);
			nThreadIndex++;
		}
        else if (!g_DIO.ReadDOBit(nEjectMotorStart))
        {
            g_DIO.SetDO(nEjectMotorStart, true);
			tm1MS->timeStart(10000);
        }
		else if (tm1MS->timeUp()) bFront ? g_IniFile.m_nErrorCode = 67 : g_IniFile.m_nErrorCode = 66;
		break;
    case 7:
        if (tm1MS->timeUp())
        {
            if (!g_DIO.ReadDIBit(nEjectInp) && !g_DIO.ReadDIBit(nEjectEntry) && !g_DIO.ReadDIBit(nEjectExit) && !g_DIO.ReadDIBit(nEjectExist))
            {
                m_ActionLog.push_back(AddTimeString(bFront, "[DoEject][6]EjectLane 出料區出料完成"));
                g_DIO.SetDO(nReadyOutY, false);
			    g_DIO.SetDO(nEjectMotorStart, false);
			    g_DIO.SetDO(nEjectStop, true);
                nThreadIndex++;
            }
            else
            {
                g_DIO.SetDO(nEjectMotorStart, true);
			    tm1MS->timeStart(10000);
                nThreadIndex--;
            }
        }
        break;
	case 8:
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

	//nManualTimes = nManualTimes + 1;                                //跳過第一次測量數據    need debug

	const int nTagA = 1000;

	double dStartX, dStartY;
	CPISODNM100 *pDNPort;

	static int nMoveIndex = 0;
	static int nTryTimes = 0;                                        
	static double dNewValue = 0;
	static int nTimes = 0;
	static bool bPassCal = false;

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
		if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[0] || g_Motion.GetActualPos(AXIS_RL)>g_IniFile.m_dLamStop[1]) g_IniFile.m_nErrorCode = 69;
		else if (!g_DIO.ReadDIBit(DI::LoadCellDown)) g_IniFile.m_nErrorCode = 10;
		//else if(g_DIO.ReadDIBit(DI::LaserCheck)) g_IniFile.m_nErrorCode=9;    //don't need
		else
		{
			m_nCalCol = nMoveIndex % 10;
			m_nCalRow = nMoveIndex / 10;

			if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
			{
                m_ActionLog.push_back(AddTimeString(bFront, "[DoPressCal][0]LoadCell 開始重量校正程序"));
				if (m_bAutoRetry == true) m_listLog.push_back("開始重量 校正程序-->" + FormatFloat("0", nMoveIndex + 1));
				else m_listLog.push_back("開始重量 量測程序-->" + FormatFloat("0", nMoveIndex + 1));

				if (nTryTimes == 0)
				{
					if (m_bAutoRetry == true)
					{
						//dNewValue = g_IniFile.m_dLamPress[bFront];
                        //dNewValue = (((g_IniFile.m_dLamPress[bFront]-0.2)/4)); use previous PressCal value is more efficient then pre-function
                        dNewValue = m_dFirstNewValue;
					}
					else
					{
						dNewValue = pOffset[nMoveIndex] + g_IniFile.m_dLamPress[bFront];
					}
					//m_listLog.push_back("Set Data=" + FormatFloat("0.0000 Kg/cm2", dNewValue));
					//pDNPort->SetKg(nMoveIndex, dNewValue);
					//pDNPort->WriteAllData();
				}

				g_Motion.AbsMove(AXIS_X, dStartX + m_nCalCol*g_IniFile.m_dColPitch);
				g_Motion.AbsMove(AXIS_Y, dStartY - m_nCalRow*g_IniFile.m_dRowPitch);

				nThreadIndex++;
			}
			else
			{
				if (nManualRange == 0) {nMoveIndex++; nThreadIndex = nTagA;}                                //0=整盤,1=單顆
				else nThreadIndex = nTagA;
			}
		}
		break;
	case 1:
		if (g_Motion.IsPosDone(AXIS_X, dStartX + m_nCalCol*g_IniFile.m_dColPitch) && g_Motion.IsPosDone(AXIS_Y, dStartY - m_nCalRow*g_IniFile.m_dRowPitch))
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoPressCal][1]LoadCell XY移動就位"));

            m_listLog.push_back("Set Data=" + FormatFloat("0.0000 Kg/cm2 * ", dNewValue) + FormatFloat("0.00 %", (100-g_IniFile.m_nDownPercent)));
            pDNPort->SetKg(nMoveIndex, dNewValue*(100-g_IniFile.m_nDownPercent)/100);
            pDNPort->WriteAllData();

            g_DIO.SetDO(DO::LoadCellValve, true);
            tm1MS.timeStart(3000);
            nThreadIndex++;
		}
		break;
	case 2:
		if (g_DIO.ReadDIBit(DI::LoadCellUp))
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoPressCal][2]LoadCell 上推到位讀值"));

            m_listLog.push_back("Set Data=" + FormatFloat("ReLoad 0.00 %", g_IniFile.m_nDownPercent));
			pDNPort->SetKg(nMoveIndex, dNewValue);
			pDNPort->WriteAllData();

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
				//--自動情況與手動皆判斷誤差容許值------------------------------------------------------------------------------------
				//if (nTimes != 0)//--------------------------------------------------------------------------------------------------------
				//{
					if (dLoadCellValue > g_IniFile.m_dLamPress[bFront] * (1.0 + (g_IniFile.m_dAutoStopRange / 100.0) * 1.0) ||
						dLoadCellValue < g_IniFile.m_dLamPress[bFront] * (1.0 - (g_IniFile.m_dAutoStopRange / 100.0) * 1.0))
					{
						m_listLog.push_back("重量超過容許誤差" + FormatFloat("0.00 %", g_IniFile.m_dAutoStopRange) + "，必須停機");
						g_IniFile.m_nErrorCode = 74;
                        m_bPrassNeedReCal = false;
					}
					else if (dLoadCellValue > g_IniFile.m_dLamPress[bFront] * (1.0 + (g_IniFile.m_dPressCalRange / 100.0) * 1.0) ||
								dLoadCellValue < g_IniFile.m_dLamPress[bFront] * (1.0 - (g_IniFile.m_dPressCalRange / 100.0) * 1.0))
					{
						m_listLog.push_back("重量超過容許誤差，未超過" + FormatFloat("0.00 %", g_IniFile.m_dAutoStopRange) + "，必須重新校正");
                        m_bPrassNeedReCal = true;
					}
				//}

				if (nManualRange == 0) nMoveIndex++;                                                                 //0=整盤,1=單顆
				nTryTimes = 0;
				nThreadIndex = nTagA;
			}
			else
			{
                //--加入壓力補償---------------------------------------------------------------------------------------------------
                /*修改重量校正流程:先以(((g_IniFile.m_dLamPress[bFront]-0.2)/4))公式計算出接近值,在多退少補*/
				/*新設定值增加值採用曲線增加，當開始校正時bPassCal為F，代表該Unit尚未通過第一次PressCal，
				當第一次進入容許值之後，bPassCal為T，就進入微調設定值，而不重新由0.1逼近，一直微調到連續通過3次;或動作25次還過不去，就出現Error並跳出.*/
                /*將校正標準提高2倍以防止自動走位校正會一直在校正(機構問題:第一次量測下數值偏高,容易超出容許範圍*/
				//if (dLoadCellValue > g_IniFile.m_dLamPress[bFront] * (1.0 + g_IniFile.m_dPressCalRange / 100.0) ||
				//	dLoadCellValue < g_IniFile.m_dLamPress[bFront] * (1.0 - g_IniFile.m_dPressCalRange / 100.0))
                if (dLoadCellValue > g_IniFile.m_dLamPress[bFront] * (1.0 + g_IniFile.m_dPressCalRange / 200.0) ||
					dLoadCellValue < g_IniFile.m_dLamPress[bFront] * (1.0 - g_IniFile.m_dPressCalRange / 200.0))
				{
					m_listLog.push_back("NG-->Try Again");
					nTryTimes++;
					if (bPassCal == false)
					{
						//校正補償公式
						//dNewValue=dNewValue*dNewValue/dLoadCellValue;
						//dNewValue = dNewValue*(1 + (g_IniFile.m_dLamPress[bFront] - dLoadCellValue) / g_IniFile.m_dLamPress[bFront]);
						//dNewValue = dNewValue*(1 + (g_IniFile.m_dLamPress[bFront] - dLoadCellValue) / (g_IniFile.m_dLamPress[bFront] * 2));
                        //if (dNewValue <= 0)  dNewValue = 0.1;
                        //if (((g_IniFile.m_dLamPress[bFront]* (1.0 + g_IniFile.m_dPressCalRange / 100.0)) - dLoadCellValue) < 0) dNewValue = 0.1;
                        //if ((g_IniFile.m_dLamPress[bFront] - dLoadCellValue) < 0) dNewValue = 0.1;
                        //if ((g_IniFile.m_dLamPress[bFront] - dLoadCellValue) < 0) dNewValue = (((g_IniFile.m_dLamPress[bFront]-0.9)/3)*0.9);
                        //else dNewValue = dNewValue*(1 + pow(((g_IniFile.m_dLamPress[bFront] - dLoadCellValue) / g_IniFile.m_dLamPress[bFront]), 0.9));

                        if ((g_IniFile.m_dLamPress[bFront] - dLoadCellValue) > 0)
                            dNewValue = dNewValue*(1 + pow(((g_IniFile.m_dLamPress[bFront] - dLoadCellValue) / (g_IniFile.m_dLamPress[bFront] * 2)), 0.9));
                        else
                            dNewValue = dNewValue*(1 - pow(((dLoadCellValue - g_IniFile.m_dLamPress[bFront]) / (g_IniFile.m_dLamPress[bFront] * 2)), 0.9));

						m_listLog.push_back("Set Data=" + FormatFloat("0.0000 Kg/cm2", dNewValue));
						pDNPort->SetKg(nMoveIndex, dNewValue);
						pDNPort->WriteAllData();

						m_nPressCalPassCount = 0;
					}
					else                                                                                                           //設定值加減微調
					{
                        if ((g_IniFile.m_dLamPress[bFront] - dLoadCellValue) > 0)
                            dNewValue = dNewValue*(1 + pow(((g_IniFile.m_dLamPress[bFront] - dLoadCellValue) / (g_IniFile.m_dLamPress[bFront] * 4)), 0.9));
                        else
						    dNewValue = dNewValue*(1 - pow(((dLoadCellValue - g_IniFile.m_dLamPress[bFront]) / (g_IniFile.m_dLamPress[bFront] * 4)), 0.9));

						m_listLog.push_back("Set Data=" + FormatFloat("0.0000 Kg/cm2", dNewValue));
						pDNPort->SetKg(nMoveIndex, dNewValue);
						pDNPort->WriteAllData();

						m_listLog.push_back("數值微調,再測一次");
						m_nPressCalPassCount = 0;                                                                                  //一直微調到超過25次 停機
					}

					if (nTryTimes<25) nThreadIndex = nTagA;
					else g_IniFile.m_nErrorCode = 71;
				}
				else
				{
					bPassCal = true;
					if (m_nPressCalPassCount < 3)
                    {
                        m_listLog.push_back("數值合格,再測一次");
                        m_nPressCalPassCount++;
                        nThreadIndex = nTagA;
                    }
                    else
                    {
					    pOffset[nMoveIndex] = dNewValue - g_IniFile.m_dLamPress[bFront];

					    pLoadCellValue[nMoveIndex] = dLoadCellValue;
					    m_listLog.push_back("數值合格,OK");

						if (nManualRange == 0) nMoveIndex++;                                           //0=整盤,1=單顆
					    nTryTimes = 0;
                        m_nPressCalPassCount = 0;
					    nThreadIndex = nTagA;
                        m_dFirstNewValue = dNewValue;
						bPassCal = false;
                    }
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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoPressCal][5]LoadCell 下降到位"));
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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoPressCal][7]LoadCell 回到安全位置"));
			nMoveIndex = 0;
			nTryTimes = 0;
			if (m_bIsAutoMode && m_bIsHomeDone)
            {
                if (m_bPrassNeedReCal != true) m_bPrassNeedReCal = false;
            }
			else g_IniFile.m_nErrorCode = 72;
			if (m_bAutoRetry == true)                                                                       //若完成校正則紀錄已校正的數據
			{
				g_IniFile.m_dLastLamPress[bFront] = g_IniFile.m_dLamPress[bFront];
				g_IniFile.MachineFile(false);
			}
			nThreadIndex++;
		}
		break;
	default:
		m_bStartAutoCal[bFront] = false;
		nThreadIndex = 0;
		nTryTimes = 0;
	}

	if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)
	{
		//nMoveIndex = 0;
        bPassCal = false;
		nThreadIndex = 0;
		nTryTimes = 0;
		m_bStartAutoCal[bFront] = false;
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
    double *p_dLaserValueDiff;
    //typedef vector<double>::iterator iterator;

	static int nMoveIndex = 0;
	static int nMoveIndexSub = 0;

	static int nTimes = 0;                                           

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
            p_dLaserValueDiff = (double *)m_dFrontUpperLaserDiff;
		}
		else
		{
			dStartX[0] = g_IniFile.m_dLaserDownPosX[bFront];
			dStartY[0] = g_IniFile.m_dLaserDownPosY[bFront];
			nLaserChannel = 1;

			p_dLaserValue = (double *)m_dFrontDownLaser;
            p_dLaserValueDiff = (double *)m_dFrontDownLaserDiff;
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
            p_dLaserValueDiff = (double *)m_dRearUpperLaserDiff;
		}
		else
		{
			dStartX[0] = g_IniFile.m_dLaserDownPosX[bFront];
			dStartY[0] = g_IniFile.m_dLaserDownPosY[bFront];
			nLaserChannel = 1;

			p_dLaserValue = (double *)m_dRearDownLaser;
            p_dLaserValueDiff = (double *)m_dRearDownLaserDiff;
		}


	}

	switch (nThreadIndex)
	{
	case 0:
        if (g_Motion.GetActualPos(AXIS_FL)>g_IniFile.m_dLamStop[0] || g_Motion.GetActualPos(AXIS_RL)>g_IniFile.m_dLamStop[1]) g_IniFile.m_nErrorCode = 69;
		else if (!g_DIO.ReadDIBit(DI::LoadCellDown)) g_IniFile.m_nErrorCode = 10;
		//else if(g_DIO.ReadDIBit(DI::LaserCheck)) g_IniFile.m_nErrorCode=9;      //don't need
		{

			m_nCalCol = nMoveIndex % 10;
			m_nCalRow = nMoveIndex / 10;

			if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
			{
                m_ActionLog.push_back(AddTimeString(bFront, "[DoLaserCal][0]Laser 開始Laser量測程序"));
				if (nMoveIndex == 0 && nMoveIndexSub == 0)
                {
                    memset(p_dLaserValue, 0, sizeof(double) * 50);
                    memset(p_dLaserValueDiff, 0, sizeof(double) * 50);
                }
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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLaserCal][1]Laser XY移動就位"));
			if (/*g_DIO.ReadDIBit(DI::LaserCheck)*/false) g_IniFile.m_nErrorCode = 9;         //bypass
			else
			{
				tm1MS.timeStart(2000);
				nThreadIndex++;
			}
		}
		break;
	case 2:
		if (tm1MS.timeUp())
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLaserCal][2]Laser 高度讀值"));
			double dLaserData = 0.0;
            if (bUp) dLaserData = m_dUpperLaserRealTime;
            else dLaserData = m_dDownLaserRealTime;
			//dLaserData = g_ModBus.GetAnalogData(3, bUp);

            /*
            //do first laser value Cal
            if (nMoveIndex == 0)
            {
                (bUp) ? m_dFirstLaserValueUp = dLaserData : m_dFirstLaserValueDown = dLaserData ;
            }

			if (g_ModBus.m_bInitOK)
			{
                if (bUp)
                {
                    p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = m_dFirstLaserValueUp-dLaserData;
                    m_listLog.push_back("高度=" + FormatFloat("0.0000 mm", m_dFirstLaserValueUp-dLaserData));
                }
                else
                {
                    p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = m_dFirstLaserValueDown-dLaserData;
                    m_listLog.push_back("高度=" + FormatFloat("0.0000 mm", m_dFirstLaserValueDown-dLaserData));
                }

				//p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = dLaserData;
				//m_listLog.push_back("高度=" + FormatFloat("0.00 mm", dLaserData));

			}
			else
			{
				g_ModBus.m_bInitOK = true;
				p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = 999;
				m_listLog.push_back("高度=N/A");
			}
            */

            //Not do first laser value Cal
            if (g_ModBus.m_bInitOK)
			{
				p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = dLaserData;
				m_listLog.push_back("高度=" + FormatFloat("0.0000 mm", dLaserData));
			}
			else
			{
				g_ModBus.m_bInitOK = true;
				p_dLaserValue[nMoveIndex * 4 + nMoveIndexSub] = 999;
				m_listLog.push_back("高度=N/A");
			}


            if (bUp)
            {
                if (nMoveIndexSub == 4-1)
                {
                    std::vector<double> vecTempUp;
                    for (int i=0;i<4;i++)
                    {
                        if (p_dLaserValue[i] != 999) vecTempUp.push_back(p_dLaserValue[(nMoveIndex * 4 + nMoveIndexSub) - 3 + i]);
                    }
                    if (vecTempUp.size() != 0)
                    {
                        double *maxValue = std::max_element(vecTempUp.begin(), vecTempUp.end());
                        double *minValue = std::min_element(vecTempUp.begin(), vecTempUp.end());
                        p_dLaserValueDiff[(nMoveIndex * 4 + nMoveIndexSub) - 3] = (*maxValue - *minValue);
                        vecTempUp.clear();
                        m_listLog.push_back("上模高度誤差= " + FormatFloat("0.0000 mm", (*maxValue - *minValue)));
                    }
                    else
                    {
                        p_dLaserValueDiff[nMoveIndex] = 999;
                        m_listLog.push_back("上模高度誤差 錯誤!!");
                    }
                }
            }
            else
            {
                if (nMoveIndex == 10*(g_IniFile.m_nRows-1)+(g_IniFile.m_nCols-1))
                {
                    std::vector<double> vecTempDown;
                    for (int i=0;i<50;i++)
                    {
                        if ((i % 10)<g_IniFile.m_nCols && (i / 10)< g_IniFile.m_nRows)
                        {
                            if (p_dLaserValue[i] != 999) vecTempDown.push_back(p_dLaserValue[i * 4]);
                        }
                    }
                    if (vecTempDown.size() != 0)
                    {
                        double *maxValue = std::max_element(vecTempDown.begin(), vecTempDown.end());
                        double *minValue = std::min_element(vecTempDown.begin(), vecTempDown.end());
                        //p_dLaserValueDiff[(nMoveIndex * 4 + nMoveIndexSub)] = (*maxValue - *minValue);
                        for (int i=0;i<50;i++)
                        {
                            p_dLaserValueDiff[i*4] = (*maxValue - *minValue);
                        }
                        vecTempDown.clear();
                        m_listLog.push_back("下模高度誤差= " + FormatFloat("0.0000 mm", (*maxValue - *minValue)));
                    }
                    else
                    {
                        p_dLaserValueDiff[nMoveIndex] = 999;
                        m_listLog.push_back("下模高度誤差 錯誤!!");
                    }
                }
            }
			nThreadIndex++;
		}
		break;
	case 3:
	case nTagA:nThreadIndex = 3;
		{
			//4 case below.
			if (nManualRange == 0)                                                           //0=整盤,1=單顆
			{                                                                                //上模與下模 整盤
                if (!bUp)                                                                    //deside nMoveIndex, nMoveIndexSub
			    {
				    if (nManualRange == 0) nMoveIndex++;
				    nMoveIndexSub = 0;
			    }
    			else
    			{
    				nMoveIndexSub++;
    				if (nMoveIndexSub>3)
    				{
    					if (nManualRange == 0) nMoveIndex++;
    					nMoveIndexSub = 0;
    				}
    			}

				if (nMoveIndex<50)                                                           //deside nTimes, nThreadIndex
				{
					nThreadIndex = 0;
				}
				else if (nTimes < nManualTimes)
				{
					nTimes++;
					nThreadIndex = 0;
					nMoveIndex = 0;
                    nMoveIndexSub = 0;
				}
				else
				{
					nThreadIndex++;
					nTimes = 0;
				}
			}
			else
			{
                if (!bUp)                                                                    //下模單顆
			    {                                                                            //deside nMoveIndex, nMoveIndexSub
                    if (nManualRange == 0) nMoveIndex++;
                    nMoveIndexSub = 0;

                    if (nTimes < nManualTimes)                                               //deside nTimes, nThreadIndex
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
                else                                                                         //上模單顆
                {
                                                                                             //deside nMoveIndex, nMoveIndexSub
                    if (nMoveIndexSub < 3)
					{
						nMoveIndexSub++;
						nThreadIndex = 0;
					}
					else if (nTimes < nManualTimes)                                          //deside nTimes, nThreadIndex
					{
						nTimes++;
						nThreadIndex = 0;
						nMoveIndexSub = 0;
					}
					else
					{
						nThreadIndex++;
						nTimes = 0;
					}

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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoLaserCal][5]Laser LoadCell回到安全位置"));
			nMoveIndex = 0;
			g_IniFile.m_nErrorCode = 73;
		}
		break;
	default:
		nThreadIndex = 0;
	}

	if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)
	{
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
            m_ActionLog.push_back(AddTimeString(bFront, "[DoAutoCal][1]Lifter 下降至0.0"));
			m_bLamReady[bFront] = false;
            //tm1MS->timeStart(3000);
			nThreadIndex++;
		}
		else if (tm1MS->timeUp())  bFront ? g_IniFile.m_nErrorCode = 21 : g_IniFile.m_nErrorCode = 21;
		break;
	case 2:
        m_ActionLog.push_back(AddTimeString(bFront, "[DoAutoCal][2]LoadCell 自動模式,測量開始(第一次)"));
        m_listLog.push_back("自動模式,測量開始(第一次)");
		m_bStartAutoCal[bFront] = true;
		m_bAutoRetry = false;
		nThreadIndex++;
		break;
	case 3:
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
	case 4:
		if (m_bStartAutoCal[bFront] == false)
		{
			if (m_bPrassNeedReCal == false)
			{
				nThreadIndex = nTagA;
			}
			else if (m_bPrassNeedReCal == true)
			{
                m_ActionLog.push_back(AddTimeString(bFront, "[DoAutoCal][4]LoadCell 自動模式,校正開始..."));
                m_listLog.push_back("自動模式,校正開始...");
				m_bStartAutoCal[bFront] = true;
				m_bAutoRetry = true;
				nThreadIndex++;
			}
		}
		break;
	case 5:
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
	case 6:
		if (m_bStartAutoCal[bFront] == false)
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoAutoCal][6]LoadCell 自動模式,測量開始(第二次)"));
            m_listLog.push_back("自動模式,測量開始(第二次)");
			g_IniFile.m_nErrorCode = 0;
			m_bStartAutoCal[bFront] = true;
			m_bAutoRetry = false;
			nThreadIndex++;
		}
		break;
	case 7:
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
	case 8:
		if (m_bStartAutoCal[bFront] == false)
		{
			if (m_bPrassNeedReCal == false)
			{
				nThreadIndex++;
			}
			else if (m_bPrassNeedReCal == true)
			{
				g_IniFile.m_nErrorCode == 74;
                m_bPrassNeedReCal = false;
				nThreadIndex++;
			}
		}
		break;
	case 9:
	case nTagA: nThreadIndex = 9;
		g_Motion.AbsMove(AXIS_Y, g_IniFile.m_dSafePos);
		nThreadIndex++;
		break;
	case 10:
		if (g_Motion.IsPosDone(AXIS_Y, g_IniFile.m_dSafePos))
		{
            m_ActionLog.push_back(AddTimeString(bFront, "[DoAutoCal][10]LoadCell 回到安全位置"));
			if (m_bPrassNeedReCal == false)
			{
				m_listLog.push_back("自動模式,測量與校正完成,繼續動作");
			}
			nThreadIndex++;
		}
		break;

	default:
		m_bIsDoAutoCal[bFront] = false;
        CMainThread::nThreadIndex[2] = 0;
        CMainThread::nThreadIndex[4] = 0;
		nThreadIndex = 0;
	}

	if (g_IniFile.m_nErrorCode>0 && g_IniFile.m_nErrorCode<1000)
	{	
		m_bIsDoAutoCal[bFront] = false;
        CMainThread::nThreadIndex[2] = 0;
        CMainThread::nThreadIndex[4] = 0;
		nThreadIndex = 0;
	}
}














