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

    bool m_bPrassNeedReCal;                 //Record for NeedReCal in DoPressCal.
	bool m_bFirstVacSuccessed;              //Record for First Vac Success.
	bool m_bIsHomingFromBtn;                //Homing from button
	int m_nPassBoatStart;                   //Input counted Boats start number.
	int m_nPassBoatCount0;                  //Counted how many Boats are finished (rare).
	int m_nPassBoatCount1;                  //Counted how many Boats are finished (front).
	bool m_bIsDoAutoCal[2];                 //Record for get into AutoCal. ([1]=fornt, [0]=rear)
	bool m_bIsManualFinish;                 //Record for Manual is finished then UP the btn.
	int m_nIsFullHoming;                    //Record for Homing button should go fullhoming. (-1=normal, 1=true, 0=false, 2=cancel)
	double m_dUnitPerHour1;                 //The result of UPH Front.
	double m_dUnitPerHour0;                 //The result of UPH Rear.
	int m_nPressCalPassCount;               //Record for succeed PressCal times. (default 2 times)

	int m_nTrayRowIndex;

	int m_nPressCalMoveIndex[2];
	int m_nLaserCalMoveIndex[2];

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

	double m_dFrontPressCal[50];
	double m_dRearPressCal[50];

	int m_nCalCol;
	int m_nCalRow;

	// g_ModBus Real Time data //
	double m_dUpperLaserRealTime;           //The result of g_ModBus get UpperLaser in UI
	double m_dDownLaserRealTime;            //The result of g_ModBus get DownLaser in UI
	double m_dFrontTempRealTime;            //The result of g_ModBus get FrontTemp in UI
	double m_dRearTempRealTime;             //The result of g_ModBus get RearTemp in UI

	//Thread HandShake
	bool m_bLamReady[2];            //1: front 0:Rear
	bool m_bEjectReady[2];

	__fastcall CMainThread(bool CreateSuspended);
	void __fastcall SetWorkSpeed();
	void __fastcall SetManualSpeed();

	//Manual Mode
	bool m_bStartPressCal[2];
	bool m_bStartLaserUpCal[2];
	bool m_bStartLaserDownCal[2];
	int m_nManualRange;
	int m_nManualFirstLoc;
	int m_nManualTimes;
	bool m_bAutoRetry;

	//Non Stop
	bool m_bStartLamSub[2];
	bool m_bStartAutoCal[2];

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

	void __fastcall DoAutoCal(bool bFront, int &nThreadIndex);

	void __fastcall CheckAlarm();
	AnsiString AddTimeString(AnsiString Input);
	AnsiString AddTimeString(bool bFront, AnsiString Input);
};
//---------------------------------------------------------------------------
#endif

