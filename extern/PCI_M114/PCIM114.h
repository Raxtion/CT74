
// PCIM114.h: interface for the CPCIM114 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCIM114_H__A4A5E2F3_B4A2_4488_8253_908A2CA30846__INCLUDED_)
#define AFX_PCIM114_H__A4A5E2F3_B4A2_4488_8253_908A2CA30846__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_LC 2
#define AXIS_FL 4
#define AXIS_RL 5

#define MAX_AXES 8

namespace PCIM114{
enum {RDY=0,ALM,PEL,MEL,ORG,DIR,REVERSE,PCS,ERC,EZ,REVERSE2,LATCH,SD,INP,SVON};
}

class CPCIM114  
{
public :
        double m_dLastTargetPos[8];    //0: Axis 0
public:
	BOOL MyAxisHome(int nAxis,double dStrVel, double dMaxVel, double dTacc);
	void ChangeSpeed(int nAxis, double dSpeed);
	void AxisHome(int nAxis,bool bDir=false);
	int GetMotionStatus(int nAxis);
	bool GetStatus(int nAxis,int nBitNo);
	int GetAllStatus(int nAxis);
	void SetActualPos(int nAxis,double dMM);
	void SetCommandPos(int nAxis,double dMM);
	double GetActualPos(int nAxis);
	double GetCommandPos(int nAxis);
	int WaitMotionDone(int nAxis,DWORD nWaitTime=0);
	void RelMove(int nAxis, double dMM);
	void AbsMove(int nAxis, double dMM);
	void Stop(int nAxis);
	void JogStart(int nAxis, bool bDir);
	void ServoOn(int nAxis, bool bOnOff);
	void SetSpeed(int nAxis, double dAcc, double dDec, double dMM);
	BOOL Initial();
	CPCIM114();
        bool IsMotionDone(int nAxis);
        bool IsPosDone(int nAxis,double dPos);
        bool IsLastPosDone(int nAxis);

	virtual ~CPCIM114();
	double m_dMMtoPLS[MAX_AXES];
	double m_dAcc[MAX_AXES];
	double m_dDec[MAX_AXES];
	double m_dSpeed[MAX_AXES];
	bool m_bInitOK;
        bool m_bAutoMode;
        double dPLimit;
        double dNLimit;

        void InitComparator(int nAxis,double dStart,double dPitch,int nCount);
        void SetTrigger(int nAxis,bool bOnOff);
        void SetSoftLimit(int nAxis,double dPLimit, double dNLimit);
        void SetDisableSoftLimit(int nAxis);
};

#endif // !defined(AFX_PCIM114_H__A4A5E2F3_B4A2_4488_8253_908A2CA30846__INCLUDED_)
