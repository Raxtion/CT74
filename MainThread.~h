//---------------------------------------------------------------------------

#ifndef MainThreadH
#define MainThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <list>

#define MAX_PROCESS 20

#define InitialMachine_Index 0
#define DoLaneChanger_Index 1
#include "C_GetTime.h"

//---------------------------------------------------------------------------
class CMainThread : public TThread
{
private:
	C_GetTime tmUPHFront;                        //tm for count UPH font.
	C_GetTime tmUPHRear;                         //tm for count UPH rear.
	int m_nPassUnit;                         //Record for Pass Units (2x4 = 8 unit = 1 plate)
protected:
    void __fastcall Execute();	
public:
	std::list<AnsiString> m_listLog;
	std::list<AnsiString> m_ActionLog;
	int nThreadIndex[MAX_PROCESS];		//0:Inti 1:Start Measure
	bool m_bRefresh;

	bool m_bIsHomeDone;
	bool m_bIsAutoMode;
    bool m_bIsStartProcessbyDIO;            //Record for Strat Process by DI::StartBtn
	bool m_bIsStartProcessbyCIM;            //Record for Strat Process by CIM in CMainThread::Execute() not by DI::StartBtn
	bool m_bIsStopProcessbyCIM;             //Record for Stop Process by CIM in CMainThread::Execute() not by DI::StopBtn

	int m_nCalCol;
	int m_nCalRow;
    bool m_bPrassNeedReCal;                 //Record for NeedReCal in DoPressCal.
	bool m_bFirstVacSuccessed;              //Record for First Vac Success.
	bool m_bIsHomingFromBtn;                //Homing from button
	int m_nPassBoatStart;                   //Input counted Boats start number.
	int m_nPassBoatCount0;                  //Counted how many Boats are finished (rare).
	int m_nPassBoatCount1;                  //Counted how many Boats are finished (front).
	bool m_bIsManualFinish;                 //Record for Manual is finished then UP the btn.
	int m_nIsFullHoming;                    //Record for Homing button should go fullhoming. (-1=normal, 1=true, 0=false, 2=cancel)
	double m_dUnitPerHour1;                 //The result of UPH Front.
	double m_dUnitPerHour0;                 //The result of UPH Rear.
	int m_nPressCalPassCount;               //Record for succeed PressCal times. (default 2 times) ·L½Õ®Õ¥¿
    double m_dFirstLaserValueUp;            //Record for use to Cal. up laser
    double m_dFirstLaserValueDown;          //Record for use to Cal. down laser
    double m_dFirstNewValue;                //Record for first Press Cal. input value that used to Cal. next.
    bool m_bIsTempMonitorFail;				//Record for get NULL value from Sqlite3Interface.
	bool m_bIsNeedReLoadProductParam;		//Machine Test Mode Change, Need To ReLoad ProductParam
    bool m_bIsWriteOffsetToDB_F;            //When PressCal Finished turn true, to Synchronize the OffsetTable.db3 by UI. (front)
    bool m_bIsWriteOffsetToDB_R;            //When PressCal Finished turn true, to Synchronize the OffsetTable.db3 by UI. (rear)

	//Error Tag
    bool m_bIsAutoCalPressOverAllowF;		//Record for Need Shutdown in AutoCal Processing, when Front Lane Press detection over ErrorAllow.
	bool m_bIsAutoCalPressOverAllowR;		//Record for Need Shutdown in AutoCal Processing, when Rear Lane Press detection over ErrorAllow.
    bool m_bIsAutoCalTimesOver25F;			//Record for Need Shutdown in AutoCal Processing, when Front Lane Press calibration times over 25.
	bool m_bIsAutoCalTimesOver25R;			//Record for Need Shutdown in AutoCal Processing, when Rear Lane Press calibration times over 25.
	bool m_bIsbVacErrorF;					//Record for Need Shutdown in LamSub, when g_IniFile.m_nErrorCode = 54;
	bool m_bIsbVacErrorR;					//Record for Need Shutdown in LamSub, when g_IniFile.m_nErrorCode = 55;
	bool m_bIsLamSecondTimeErrorF;			//Record for Need Shutdown in LamSub, when g_IniFile.m_nErrorCode = 92;
	bool m_bIsLamSecondTimeErrorR;			//Record for Need Shutdown in LamSub, when g_IniFile.m_nErrorCode = 93;
    bool m_bIsLamCorrCheckErrorF;			//Record for Need Shutdown in LamSub, when g_IniFile.m_nErrorCode = 100;
    bool m_bIsLamCorrCheckErrorR;           //Record for Need Shutdown in LamSub, when g_IniFile.m_nErrorCode = 101;

	//Lock
	bool m_bIsAutoCalLocked;                //Record for someone use AutoCal
    bool m_bIsResetAlarmLocked;				//Record for Lock ResetButton when Alarm..

	//MoveIndex
	int m_nPressCalMoveIndex[2];
	int m_nLaserCalMoveIndex[2];
	double m_dFrontUpperMoveLocX[50][4];
	double m_dFrontUpperMoveLocY[50][4];
	double m_dFrontDownMoveLocX[50][4];
	double m_dFrontDownMoveLocY[50][4];
	double m_dRearUpperMoveLocX[50][4];
	double m_dRearUpperMoveLocY[50][4];
	double m_dRearDownMoveLocX[50][4];
	double m_dRearDownMoveLocY[50][4];

	//Button Again
    bool m_bStartAgain;
    bool m_bStopAgain;
    bool m_bResetAgain;
    bool m_bInitalAgain;

	//Monitor Data//
	double m_dLamTimer[2];   //Lamination timer count down
	double m_dFrontUpperLaser[50][4];
	double m_dFrontDownLaser[50][4];
	double m_dRearUpperLaser[50][4];
	double m_dRearDownLaser[50][4];

    double m_dFrontUpperLaserDiff[50][4];
    double m_dFrontDownLaserDiff[50][4];
    double m_dRearUpperLaserDiff[50][4];
    double m_dRearDownLaserDiff[50][4];
    double m_dFrontUpperTotalLaserDiff[50][4];
    double m_dRearUpperTotalLaserDiff[50][4];

	double m_dFrontPressCal[50];
	double m_dRearPressCal[50];

	//Record for DNPort Connecttion Error when Machine Init
	double m_bDNPortConnectError0[50];
	double m_bDNPortConnectError1[50];

	// g_ModBus Real Time data //
	double m_dUpperLaserRealTime;           //The result of g_ModBus get UpperLaser in UI
	double m_dDownLaserRealTime;            //The result of g_ModBus get DownLaser in UI
	double m_dFrontTempRealTime;            //The result of g_ModBus get FrontTemp in UI
	double m_dRearTempRealTime;             //The result of g_ModBus get RearTemp in UI
    double m_dForntPressloseRealTime;       //The result of g_ModBus get FrontPresslose in UI
    double m_dRearPressloseRealTime;        //The result of g_ModBus get RearPresslose in UI
    double m_dSensoLinkF911RealTime;        //The result of g_ModBus get F911 LoadCell data in UI

    //Laser Keep Data for Deal with Error Value
    double m_dLaserKeepValue;

	//Thread HandShake
	bool m_bLamReady[2];					//1: front 0:Rear
	bool m_bEjectReady[2];
	bool m_bStartLamSub[2];					//Non Stop
	bool m_bStartAutoCal[2];				//Non Stop
	bool m_bIsDoAutoCal[2];                 //Record for get into AutoCal. ([1]=fornt, [0]=rear)
	bool m_bIsAutoCalFinished[2];			//Record for finished AutoCal.
    bool m_bIsDoPressCalFinished[2];		//Record for finished DoPressCal.
	bool m_bIsDoLaserCalFinished[2];		//Record for finished DoLaserCal.
    bool m_bIsDoLamSubFinished[2];          //Record for finished DoLamSub.
    bool m_bIsLamCorrCheckLock;             //Record for Use LamCorrectionCheck. (Maybe Co-control).

	//Cal Job container for DoOneStepCal
	bool m_arrybDoUpperMoldLaser[50];
	bool m_arrybDoDownMoldLaser[50];
	bool m_arrybDoPressCal[2][50];

	__fastcall CMainThread(bool CreateSuspended);
	void __fastcall SetWorkSpeed();
	void __fastcall SetManualSpeed();

	//Manual Mode
    bool m_bStartOneStepCal;
	bool m_bStartPressCal[2];
	bool m_bStartLaserUpCal[2];
	bool m_bStartLaserDownCal[2];
	int m_nManualRange;
	int m_nManualFirstLoc;
	int m_nManualTimes;
	bool m_bAutoRetry;

	//Stop LaneChange
	bool m_bStopLC;

	//Homing
	bool __fastcall InitialMachine(int &nThreadIndex);
	//Auto Mode
	void __fastcall DoLaneChanger(int &nThreadIndex);
	void __fastcall DoLamination(bool bFront,int &nThreadIndex);
	void __fastcall DoLamSub(bool bFront,int &nThreadIndex);
	void __fastcall DoEject(bool bFront,int &nThreadIndex);

	//Manual Mode
	void __fastcall DoPressCal(bool bFront,int &nThreadIndex, int m_nManualRange, int m_nManualFirstLoc, int m_nManualTimes);
	void __fastcall DoLaserCal(bool bFront,bool bUp,int &nThreadIndex, int m_nManualRange, int m_nManualFirstLoc, int m_nManualTimes);
	void __fastcall DoOneStepCal(int &nThreadIndex);

	void __fastcall DoAutoCal(bool bFront, int &nThreadIndex);

	void __fastcall CheckAlarm();
	AnsiString AddTimeString(AnsiString Input);
	AnsiString AddTimeString(bool bFront, AnsiString Input);
};
//---------------------------------------------------------------------------
#endif







