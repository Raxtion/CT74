#define SENSOR_ON_DELAY 100
#define SENSOR_ON_TIMEOUT 3000
#define AXIS_TIMEOUT 10000
//-------------------------------DO----------------------------------------//
namespace DO
{
        enum DOList{
        StartBtnLamp	,
StopBtnLamp	,
ResetBtnLamp	,
RedLamp	,
GreenLamp	,
YellowLamp	,
Buzzer	,
Y007	,
BigValve1	,
BigValve2	,
LoadCellValve	,
LamMotorStart1	,
LamMotorStart2	,
EjectMotorStart1	,
EjectStop1	,
EjectMotorStart2	,
EjectStop2	,
LCMotorStart	,
LCStop	,
LifterVac1	,
LifterVac2	,
Y021	,
Y022	,
Y023	,
Y024	,
Y025	,
Y026	,
Y027	,
Y028	,
Y029	,
Y030	,
Y031	,
Y032	,
Y033	,
Y034	,
Y035	,
Y036	,
Y037	,
Y038	,
Y039	,
Y040	,
Y041	,
Y042	,
Y043	,
Y044	,
Y045	,
Y046	,
Y047	,
Y048	,
Y049	,
Y050	,
Y051	,
Y052	,
Y053	,
Y054	,
Y055	,
Y056	,
Y057	,
Y058	,
SafetyDoorByPass	,
ReadyOutF	,
ReadyOutR	,
ReadyIn1	,
ReadyIn2 };
}

//-----------------------------------DI------------------------------------//
namespace DI
{
        enum DIList{
        StartBtn	,
StopBtn	,
ResetBtn	,
EmgBtn	,
MainAir	,
SafetyDoor	,
X006	,
X007	,
LifterVac1	,
LifterVac2	,
LaserCheck	,
LoadCellUp	,
LoadCellDown	,
LamInp1	,
LamInp2	,
LamEntry1	,
LamEntry2	,
EjectStopUp1	,
EjectStopDown1	,
EjectEntry1	,
EjectExit1	,
EjectInp1	,
EjectExist1	,
EjectStopUp2	,
EjectStopDown2	,
EjectEntry2	,
EjectExit2	,
EjectInp2	,
EjectExist2	,
LamWarp1	,
LamWarp2	,
LCStopUp	,
LCStopDown	,
LCInp	,
LCExist	,
LCEntry	,
YAxisSafePosA,
YAxisSafePosB,
X038	,
X039	,
X040	,
X041	,
X042	,
X043	,
X044	,
X045	,
X046	,
X047	,
X048	,
X049	,
X050	,
X051	,
X052	,
X053	,
X054	,
X055	,
X056	,
X057	,
X058	,
X059	,
X060	,
X061	,
ReadyOut1	,
ReadyOut2  };
}
