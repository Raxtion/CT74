//---------------------------------------------------------------------------

#ifndef fmMainH
#define fmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "C_GetTime.h"
#include "FrmProduct.h"
#include "Sqlite3Interface.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ScktComp.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *IOTest1;
        TMenuItem *MotorTest1;
        TMenuItem *N11;
        TMenuItem *N10;
        TMenuItem *N12;
        TMenuItem *OP1;
        TMenuItem *Engineer1;
        TMenuItem *N13;
        TMenuItem *Admin1;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
        TTimer *Timer1;
        TListBox *listHistory;
        TSpeedButton *btnStartPressCal0;
        TTimer *timerPressure;
        TLabel *Label1;
        TLabel *Label2;
        TPaintBox *PaintBox1;
        TLabel *Label4;
        TLabel *Label5;
        TPaintBox *PaintBox2;
        TSpeedButton *btnStartPressCal1;
        TSpeedButton *btnLaserUp0;
        TSpeedButton *btnLaserDown0;
        TSpeedButton *btnLaserUp1;
        TSpeedButton *btnLaserDown1;
        TShape *Shape1;
        TShape *Shape2;
        TMenuItem *N14;
        TGroupBox *GroupBox1;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TCheckBox *checkMonitor;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TLabel *Label3;
        TTimer *Timer2;
        TCheckBox *checkRestartCal;
        TLabel *Label6;
        TLabel *Label10;
        TLabel *Label11;
        TComboBox *cmbRange;
        TComboBox *cmbFirstLoc;
        TComboBox *cmbTimes;
        TCheckBox *checkAutoRetry;
        TGroupBox *GroupBox2;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label31;
        TPanel *Panel1;
        TButton *btnHoming;
        TCheckBox *checkStopLC;
        TGroupBox *GroupBox3;
        TEdit *editBoatFinishStart;
        TLabel *Label32;
        TLabel *Label33;
        TEdit *editBoatFinishNow;
        TGroupBox *GroupBox4;
        TLabel *Label34;
        TEdit *editUnitPerHour;
    TSpeedButton *btnStartMotor1;
    TSpeedButton *btnStartMotor0;
    TTimer *Timer3;
    TServerSocket *ServerCIM;
    TShape *Shape3;
    TMenuItem *N15;
    TTimer *timerRenewPainBox;
    TMenuItem *N16;
    TMenuItem *N17;
    TMenuItem *N18;
    TRadioGroup *radioPosOption;
    TLabel *Label35;
    TLabel *Label36;
    TSpeedButton *btnClearEject1;
    TSpeedButton *btnStartOneStepPressCal;
    TSpeedButton *btnClearLC;
    TSpeedButton *btnClearEject0;
        void __fastcall N2Click(TObject *Sender);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);
        void __fastcall N5Click(TObject *Sender);
        void __fastcall N7Click(TObject *Sender);
        void __fastcall N8Click(TObject *Sender);
        void __fastcall IOTest1Click(TObject *Sender);
        void __fastcall MotorTest1Click(TObject *Sender);
        void __fastcall N10Click(TObject *Sender);
        void __fastcall N11Click(TObject *Sender);
        void __fastcall OP1Click(TObject *Sender);
        void __fastcall Engineer1Click(TObject *Sender);
        void __fastcall N13Click(TObject *Sender);
        void __fastcall Admin1Click(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall PaintBox1Paint(TObject *Sender);
        void __fastcall PaintBox2Paint(TObject *Sender);
        void __fastcall timerPressureTimer(TObject *Sender);
        void __fastcall btnStartPressCal0Click(TObject *Sender);
        void __fastcall PaintBox1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall N14Click(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall radioPosOptionClick(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall cmbRangeChange(TObject *Sender);
        void __fastcall cmbFirstLocChange(TObject *Sender);
        void __fastcall cmbTimesChange(TObject *Sender);
        void __fastcall checkAutoRetryClick(TObject *Sender);
        void __fastcall btnHomingClick(TObject *Sender);
        void __fastcall checkStopLCClick(TObject *Sender);
    void __fastcall btnStartMotor0Click(TObject *Sender);
    void __fastcall btnStartMotor1Click(TObject *Sender);
    void __fastcall Timer3Timer(TObject *Sender);
    void __fastcall ServerCIMClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerCIMClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerCIMClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ServerCIMClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall N15Click(TObject *Sender);
    void __fastcall timerRenewPainBoxTimer(TObject *Sender);
    void __fastcall N17Click(TObject *Sender);
    void __fastcall N18Click(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall btnClearEject0Click(TObject *Sender);
    void __fastcall btnClearEject1Click(TObject *Sender);
    void __fastcall btnStartOneStepPressCalClick(TObject *Sender);
    void __fastcall btnClearLCClick(TObject *Sender);
private:	// User declarations
        C_GetTime tm1MSLogOut;
        int m_nROIIndex;
        int m_nUserLevel;
        AnsiString m_strDate;
        bool m_bIsAutoRun;    //AutoRun = true, Manual = false
        bool m_bLastGassLeakyFront;
        bool m_bLastGassLeakyRear;
        bool m_bLastPusherIOErrorFront;
        bool m_bLastPusherIOErrorRear;

        std::vector<TRect> m_vectRect;
        std::vector<TRect> m_vectRect2;
public:		// User declarations
        __fastcall TfrmMain(TComponent* Owner);
        void __fastcall SetPrivilege(int nLevel);
        void __fastcall SetManualSpeed(double dRatio);
        void __fastcall AddList(AnsiString strMessage);

        void __fastcall CreateCaptionFile(TForm *pForm);
        void __fastcall CreateCaptionFileTMainMenu(TMainMenu *pMainMenu);
        void __fastcall ReadCaptionFile(TForm *pForm,int nLanguage=0);
        void __fastcall ReadCaptionFileTMainMenu(TMainMenu *pMainMenu, int nLanguage=0);

        void __fastcall CreateMapArray(TRect &rect,int nPackageX,int nPackageY,int nDeviceGap,int nSectX,int nSectY);
        void __fastcall CreateMapArray2(TRect &rect,int nPackageX,int nPackageY,int nDeviceGap,int nSectX,int nSectY);

        void __fastcall RefreshImage();
        void __fastcall SetAllDevice();
        void __fastcall RenewRadioGroup(bool bInit);
        AnsiString __fastcall CheckAccount(AnsiString Account, AnsiString Password);
        AnsiString __fastcall ChangePWD(AnsiString Account, AnsiString OldPassword, AnsiString NewPassword);

        static bool StartProcess(bool bStart);
        static bool OpenFilebyCIM(AnsiString strFileName);
        void __fastcall CheckParamChange(TfmProduct *pWnd, TfmProduct *pWndRecord, SQLITE3IF *g_ChangeLog, AnsiString SetupEE);
        void __fastcall ImportOffsetFromDB();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif

