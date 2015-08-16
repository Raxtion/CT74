//---------------------------------------------------------------------------

#ifndef fmManualH
#define fmManualH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <AppEvnts.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmManual : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TTimer *Timer1;
        TLabel *Label2;
        TSpeedButton *SpeedButton1;
        TGroupBox *GroupBox1;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton6;
        TSpeedButton *SpeedButton7;
        TSpeedButton *SpeedButton8;
        TGroupBox *GroupBox2;
        TSpeedButton *SpeedButton9;
        TSpeedButton *SpeedButton10;
        TSpeedButton *SpeedButton11;
        TSpeedButton *SpeedButton12;
        TGroupBox *GroupBox3;
        TSpeedButton *SpeedButton13;
        TSpeedButton *SpeedButton14;
        TSpeedButton *SpeedButton15;
        TGroupBox *GroupBox4;
        TSpeedButton *SpeedButton16;
        TSpeedButton *SpeedButton17;
        TSpeedButton *SpeedButton18;
        TGroupBox *GroupBox5;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        TSpeedButton *SpeedButton2;
        
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmManual(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmManual *frmManual;
//---------------------------------------------------------------------------
#endif
