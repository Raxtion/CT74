//---------------------------------------------------------------------------

#ifndef fmMotorCheckH
#define fmMotorCheckH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMotorCheck : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TSpeedButton *btnJog;
    TSpeedButton *btnPitch1;
    TSpeedButton *btnPitch01;
    TSpeedButton *btnPitch001;
    TPanel *Panel1;
    TSpeedButton *btnFastSpeed;
    TSpeedButton *btnMidSpeed;
    TSpeedButton *btnSlowSpeed;
    TSpeedButton *bntVerySlowSpeed;
    TPanel *Panel3;
    TLabel *Label1;
    TShape *shapePL;
    TShape *shapeML;
    TLabel *Label3;
    TLabel *Label2;
    TShape *shapeOrg;
    TShape *shapeInp;
    TLabel *Label4;
    TShape *shapeAlarm;
    TLabel *Label5;
    TLabel *labelPosition;
    TPanel *Panel4;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TPanel *Panel5;
    TSpeedButton *btnFWD;
    TSpeedButton *btnStop;
    TSpeedButton *btnRWD;
    TSpeedButton *btnHome;
    TTimer *Timer1;
    TSpeedButton *btnServoOnOff;
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btnStopClick(TObject *Sender);
    void __fastcall btnJogClick(TObject *Sender);
    void __fastcall btnFastSpeedClick(TObject *Sender);
    void __fastcall btnHomeClick(TObject *Sender);
    void __fastcall btnFWDMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnRWDMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnFWDMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnRWDMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnServoOnOffClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
private:
    void __fastcall SetActiveAxis(unsigned int value);	// User declarations
public:		// User declarations
    __fastcall TfrmMotorCheck(TComponent* Owner);
    __property unsigned int ActiveAxis  = { write=SetActiveAxis };

public:
    unsigned int m_nActiveAxis;
    double m_dCurrPos;
private:
    UINT m_nMoveMode;
    UINT m_nSpeedMode;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMotorCheck *frmMotorCheck;
//---------------------------------------------------------------------------
#endif
