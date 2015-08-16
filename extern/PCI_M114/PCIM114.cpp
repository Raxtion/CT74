// PCIM114.cpp: implementation of the CPCIM114 class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <vcl>

#include "PCIM114.h"
#include "pci_M114.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _m114_Servo_On 1
#define _m114_Servo_Off 0

#define _m114_DIR_PLS_0 0
#define _m114_DIR_PLS_1 1
#define _m114_DIR_PLS_2 2
#define _m114_DIR_PLS_3 3
#define _m114_CW_CCW_F 4
#define _m114_CW_CCW_R 5
#define _m114_AB_PHASE 6
#define _m114_BA_PHASE 7

#define _m114_CNT_SRC_EXT 0
#define _m114_CNT_SRC_CMD 1
#define _m114_CNT_SRC_CMD_CMD 2
#define _m114_CNT_SRC_EXT_CMD 3

#define _m114_IN_1XAB 0
#define _m114_IN_2XAB 1
#define _m114_IN_4XAB 2
#define _m114_IN_CW_CCW 3
#define _m114_IN_PLS_NOT_INVERSE 0
#define _m114_IN_PLS_INVERSE 1

#define _m114_Home_Mode_Org 0
#define _m114_Home_Mode_OrgEZ 1
#define _m114_Home_Mode_OrgEZOrg 2

#define _m114_Org_Active_Low 0
#define _m114_Org_Active_High 1
#define _m114_Org_NoLatch 0
#define _m114_Org_Latch 1
#define _m114_EZ_Active_Low 0
#define _m114_EZ_Active_High 1
#define _m114_Inp_Active_Low 0
#define _m114_Inp_Active_High 1
#define _m114_Inp_Disable 0
#define _m114_Inp_Enable 1
#define _m114_Alarm_Active_Low 0
#define _m114_Alarm_Active_High 1
#define _m114_Alarm_Mode_Stop 0
#define _m114_Alarm_Mode_DecStop 1
#define _m114_SD_Active_Low 0
#define _m114_SD_Active_High 1
#define _m114_SD_NoLatch 0
#define _m114_SD_Latch 1
#define _m114_SD_Disable 0
#define _m114_SD_Enable 1
#define _m114_SD_Mode_Slow 0
#define _m114_SD_Mode_SlowStop 1
#define _m114_ERC_Diasble 0
#define _m114_ERC_Enable 1

/* ------------ I/O Status Definition ------------------------------------- */
#define   _m114_rdy_sts         0x0001
#define   _m114_alm_switch      0x0002
#define   _m114_pos_limit       0x0004
#define   _m114_neg_limit       0x0008
#define   _m114_org_limit       0x0010
#define	  _m114_dir_sts	        0x0020
#define	  _m114_reserve1_sts    0x0040
#define	  _m114_pcs_sts	        0x0080
#define   _m114_erc_sts         0x0100
#define   _m114_idx_switch      0x0200
#define	  _m114_reserve2_sts    0x0400
#define	  _m114_latch_sts       0x0800
#define   _m114_sd_switch       0x1000
#define   _m114_inp_sts         0x2000
#define   _m114_svon_sts        0x4000
#define   _m114_ralm_sts        0x8000

/* ------------ Motion Done Reurn Value Definition ------------------------ */
#define   _m114_Stop            0
#define   _m114_Res1            1
#define   _m114_Res2            2
#define   _m114_Res3            3
#define   _m114_WaitOtherAxis   4
#define   _m114_WaitErc         5
#define   _m114_DirChange       6
#define   _m114_BacklashComp    7
#define   _m114_WaitPAPB        8
#define   _m114_InHomeMotion    9
#define   _m114_InStartMotion   10
#define   _m114_InAcc           11
#define   _m114_InMaxVel        12
#define   _m114_InDec           13
#define   _m114_WaitInp         14
#define   _m114_Res4            15

/*--------------------------------------------*/

CPCIM114 g_Motion;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCIM114::CPCIM114()
{

	m_bInitOK=false;
	if(!Initial()) ShowMessage("���F�d PCIM114 �Ұʥ���,���ˬd!");

	m_dMMtoPLS[AXIS_X]= 10000.0/11.0;
	m_dMMtoPLS[AXIS_Y]= 10000.0/14.0;
	m_dMMtoPLS[AXIS_LC]= 10000.0/10.0;
	m_dMMtoPLS[AXIS_FL]= 10000.0/5.0;
        m_dMMtoPLS[AXIS_RL]= 10000.0/5.0;

	for(int nIndex=0;nIndex<MAX_AXES;nIndex++)
	        _m114_set_servo(nIndex / 4,nIndex % 4,_m114_Servo_On);

	for(int nIndex=0;nIndex<MAX_AXES;nIndex++)
		SetSpeed(nIndex,0.1,0.1,10.0);
        m_bAutoMode=false;
}

CPCIM114::~CPCIM114()
{
        for(int nIndex=0;nIndex<MAX_AXES;nIndex++)
	        _m114_set_servo(nIndex / 4,nIndex % 4,_m114_Servo_Off);


	_m114_close();
}

BOOL CPCIM114::Initial()
{
	U16 TotalCard;
	_m114_open(&TotalCard);
	if(TotalCard<1) return FALSE;

        for(int nCard=0;nCard<MAX_AXES/4;nCard++)
        {
                _m114_initial(nCard);
        }

        for(int nIndex=0;nIndex<MAX_AXES;nIndex++)
        {

	        _m114_set_pls_outmode(nIndex/4,nIndex%4,_m114_CW_CCW_R);
	        _m114_set_feedback_src(nIndex/4,nIndex%4,_m114_CNT_SRC_EXT);
	        _m114_set_pls_iptmode(nIndex/4,nIndex%4,_m114_IN_4XAB,_m114_IN_PLS_NOT_INVERSE);
	        _m114_set_home_config(nIndex/4,nIndex%4,_m114_Home_Mode_OrgEZ,_m114_Org_Active_High,_m114_EZ_Active_Low,1,1);
	        _m114_set_inp(nIndex/4,nIndex%4,_m114_Inp_Enable,_m114_Inp_Active_High);
	        _m114_set_alm(nIndex/4,nIndex%4,_m114_Alarm_Active_High,_m114_Alarm_Mode_Stop);
	        _m114_set_sd(nIndex/4,nIndex%4,_m114_SD_Disable,_m114_SD_Active_Low,_m114_SD_NoLatch,_m114_SD_Mode_Slow);
                _m114_set_ell(nIndex/4, nIndex%4,0,0);       //0: Low Active 0:Stop Immediate
	        //_m114_set_move_ratio(nIndex/4,nIndex%4,1);

                m_dLastTargetPos[nIndex]=0.0;
        }


	m_bInitOK=true;
	return TRUE;
}

void CPCIM114::SetSpeed(int nAxis, double dAcc, double dDec, double dMM)
{
	m_dAcc[nAxis]=dAcc;
	m_dDec[nAxis]=dDec;
	m_dSpeed[nAxis]=dMM*m_dMMtoPLS[nAxis];
}

void CPCIM114::ServoOn(int nAxis, bool bOnOff)
{
	if(!m_bInitOK) return;

	if(bOnOff) _m114_set_servo(nAxis/4,nAxis%4,_m114_Servo_On);
	else _m114_set_servo(nAxis/4,nAxis%4,_m114_Servo_Off);
}

void CPCIM114::JogStart(int nAxis, bool bDir)
{
	if(!m_bInitOK) return;

        I16 ret;

	ret=_m114_tv_move(nAxis/4,nAxis%4,bDir,m_dSpeed[nAxis]/2.0,m_dSpeed[nAxis],m_dAcc[nAxis]);

}

void CPCIM114::Stop(int nAxis)
{
	if(!m_bInitOK) return;

	_m114_sd_stop(nAxis/4,nAxis%4,m_dDec[nAxis]);
}

void CPCIM114::AbsMove(int nAxis, double dMM)
{
	if(!m_bInitOK) return;
        if(m_bAutoMode) m_dLastTargetPos[nAxis]=dMM;
	_m114_start_ta_move(nAxis/4,nAxis%4,dMM*m_dMMtoPLS[nAxis],m_dSpeed[nAxis]/2.0,m_dSpeed[nAxis],
		m_dAcc[nAxis],m_dDec[nAxis]);
}

void CPCIM114::RelMove(int nAxis, double dMM)
{
	if(!m_bInitOK) return;
        if(m_bAutoMode)  m_dLastTargetPos[nAxis]=GetActualPos(nAxis)+dMM;
	_m114_start_tr_move(nAxis/4,nAxis%4,dMM*m_dMMtoPLS[nAxis],m_dSpeed[nAxis]/2.0,m_dSpeed[nAxis],
		m_dAcc[nAxis],m_dDec[nAxis]);
}

int CPCIM114::WaitMotionDone(int nAxis, DWORD nWaitTime)
{
	if(!m_bInitOK) return 0;
	DWORD dwStartTime=GetTickCount();
	while(1)
	{
		int nStatus=_m114_motion_done(nAxis/4,nAxis%4);
		if((nStatus==_m114_Stop) && GetStatus(nAxis,PCIM114::INP)) break;
		if(GetStatus(nAxis,PCIM114::ALM)) return -1;
		if(GetStatus(nAxis,PCIM114::PEL)) return -2;
		if(GetStatus(nAxis,PCIM114::ORG)) return -4;
		if(GetStatus(nAxis,PCIM114::MEL)) return -3;
		if(nWaitTime>0) if(GetTickCount()-dwStartTime>nWaitTime) return false;
		
		//::Sleep(10);
	}
	return 1;
}

bool CPCIM114::IsMotionDone(int nAxis)
{
        int nStatus=_m114_motion_done(nAxis/4,nAxis%4);
        if((nStatus==_m114_Stop) && GetStatus(nAxis,PCIM114::INP)) return true;
        else return false;
}

bool CPCIM114::IsPosDone(int nAxis,double dPos)
{
        double dP=GetActualPos(nAxis);
        if(dP>dPos-0.1 && dP<dPos+0.1) return true;
        else return false;
}

bool CPCIM114::IsLastPosDone(int nAxis)
{
        double dPos=m_dLastTargetPos[nAxis];

        double dP=GetActualPos(nAxis);
        if(dP>dPos-0.1 && dP<dPos+0.1) return true;
        else return false;
}

double CPCIM114::GetCommandPos(int nAxis)
{
	if(!m_bInitOK) return 0;

	long nPLS;
	_m114_get_command(nAxis/4,nAxis%4,&nPLS);
	double dPLS=nPLS/m_dMMtoPLS[nAxis];
	return dPLS;
}

double CPCIM114::GetActualPos(int nAxis)
{
	if(!m_bInitOK) return 0;

	I32 nPLS;
        double dPLS;
	_m114_get_position(nAxis/4,nAxis%4,&nPLS);
        dPLS=nPLS;
	dPLS=dPLS/m_dMMtoPLS[nAxis];
	return dPLS;
}

void CPCIM114::SetCommandPos(int nAxis, double dMM)
{
	if(!m_bInitOK) return;

	dMM=dMM*m_dMMtoPLS[nAxis];
	_m114_set_command(nAxis/4,nAxis%4,dMM);
}

void CPCIM114::SetActualPos(int nAxis, double dMM)
{
	if(!m_bInitOK) return;
	dMM=dMM*m_dMMtoPLS[nAxis];
	_m114_set_position(nAxis/4,nAxis%4,dMM);
}

int CPCIM114::GetAllStatus(int nAxis)
{
	if(!m_bInitOK) return 0;
  unsigned short nStatus;
  _m114_get_io_status(nAxis/4,nAxis%4,&nStatus);
  return nStatus;
}

bool CPCIM114::GetStatus(int nAxis, int nBitNo)
{
	if(!m_bInitOK) return 0;
  int nStatus=GetAllStatus(nAxis);
  if(nStatus & (int)pow(2,nBitNo)) return true;
  else return false;
}

int CPCIM114::GetMotionStatus(int nAxis)
{
	if(!m_bInitOK) return 0;
  return _m114_motion_done(nAxis/4,nAxis%4);
}

void CPCIM114::AxisHome(int nAxis,bool bDir)
{
	if(!m_bInitOK) return;
	_m114_home_move(nAxis/4,nAxis%4,bDir,m_dSpeed[nAxis]/2.0,m_dSpeed[nAxis],m_dAcc[nAxis]);

	//if(nAxis!=CCD_Axis_Y) _m114_home_move(nAxis,-m_dSpeed[nAxis]/2.0,-m_dSpeed[nAxis],m_dAcc[nAxis]);
	//else MyAxisHome(nAxis,-m_dSpeed[nAxis]/2.0,-m_dSpeed[nAxis],m_dAcc[nAxis]);

}

void CPCIM114::ChangeSpeed(int nAxis, double dSpeed)
{
	if(!m_bInitOK) return;

	_m114_v_change(nAxis/4,nAxis%4,dSpeed*m_dMMtoPLS[nAxis],0.01);

}

BOOL CPCIM114::MyAxisHome(int nAxis,double dStrVel, double dMaxVel, double dTacc)
{
	unsigned short io_sts;
	AnsiString szError;

	_m114_get_io_status(nAxis/4,nAxis%4,&io_sts);

	if(io_sts& _m114_alm_switch)			//Alarm
	{
		szError.sprintf("Axis %d Table Motion Homing Stoped,Servo Driver Alarm Occured.",nAxis);
		ShowMessage(szError);
		return FALSE;
	}   


	if((io_sts & _m114_neg_limit) ||(io_sts & _m114_org_limit))
	{
		_m114_start_tr_move(nAxis/4,nAxis%4,50.0*m_dMMtoPLS[nAxis],dStrVel, dMaxVel,dTacc,dTacc);
     
		while(1)		//Wait Motion Done
		{
			_m114_get_io_status(nAxis/4,nAxis%4,&io_sts);
   			if ((_m114_motion_done(nAxis/4,nAxis%4)==0)&&( io_sts & _m114_inp_sts))  //In Position
				break;
			
			::Sleep(1);
		}
	}

	::Sleep(100);

	_m114_tv_move(nAxis/4,nAxis%4,false,dStrVel,dMaxVel,dTacc);

	while(1)		//Wait Original Sensor
	{
		_m114_get_io_status(nAxis/4,nAxis%4,&io_sts);
    
		if(io_sts & _m114_org_limit)		//Original Limit
		{
			_m114_sd_stop(nAxis/4,nAxis%4,0.02);
			break;	
		}

		if(io_sts & _m114_neg_limit)
		{
			_m114_sd_stop(nAxis/4,nAxis%4,0.02);
			::Sleep(20);
			_m114_start_tr_move(nAxis/4,nAxis%4,50.0*m_dMMtoPLS[nAxis],dStrVel, dMaxVel,dTacc,dTacc);
     
			while(1)		//Wait Motion Done
			{
				_m114_get_io_status(nAxis/4,nAxis%4,&io_sts);
   				if ((_m114_motion_done(nAxis/4,nAxis%4)==0)&&( io_sts & _m114_inp_sts))  //In Position
					break;
			
				::Sleep(1);
			}
			_m114_sd_stop(nAxis/4,nAxis%4,0.02);
		}	
	}     // end of  Wait Original Sensor      
	
	
	::Sleep(100);
	_m114_tv_move(nAxis/4,nAxis%4,false,dStrVel,dStrVel,dTacc);
	while( 1 )		//Wait EZ Signal
	{
		_m114_get_io_status(nAxis/4,nAxis%4,&io_sts);
		
		if(io_sts&_m114_idx_switch)		//EZ Signal
		{
			_m114_sd_stop(nAxis/4,nAxis%4,0.02);
			break;	
		}
		::Sleep(1); 
	}   //Wait EZ Signal
	
	
	::Sleep(100);
	_m114_set_position(nAxis/4,nAxis%4,0.0);

 return TRUE;	
}

void CPCIM114::InitComparator(int nAxis,double dStart,double dPitch,int nCount)
{

        double dStartPos=dStart*m_dMMtoPLS[nAxis];
        double dPitchA=dPitch*m_dMMtoPLS[nAxis];

        _m114_start_auto_compare(nAxis/4, nAxis % 4, 1);

        U8 bDir = (U8)((dPitchA >= 0) ? 1 : 0); // 0:-, 1:+

        _m114_set_auto_compare_trigger(nAxis/4, nAxis % 4, 1, 100);       //level=1 ,   width=100 us
        _m114_set_auto_compare_function(nAxis/4, nAxis % 4, bDir, dStartPos, abs(dPitchA), nCount);
        
        _m114_start_auto_compare(nAxis/4, nAxis % 4, 1);

}

void CPCIM114::SetTrigger(int nAxis,bool bOnOff)
{
        _m114_set_auto_compare_trigger(nAxis/4, nAxis % 4, bOnOff, 100);
        _m114_force_trigger_output(nAxis/4, nAxis % 4);
}

void CPCIM114::SetSoftLimit(int nAxis,double dPLimit, double dNLimit)
{
       _m114_disable_soft_limit(nAxis/4, nAxis % 4);
       _m114_set_soft_limit(nAxis/4, nAxis % 4, dPLimit*m_dMMtoPLS[nAxis], dNLimit*m_dMMtoPLS[nAxis]);
       _m114_enable_soft_limit(nAxis/4, nAxis % 4, 1);  //Action: 0=INT only, 1=Immediately stop, 2=Slow down then stop, 3=Reserved
}

void CPCIM114::SetDisableSoftLimit(int nAxis)
{
        _m114_disable_soft_limit(nAxis/4, nAxis % 4);
}
