//---------------------------------------------------------------------------

#ifndef KeyboardH
#define KeyboardH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TKeyboardWnd : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TEdit *editSetValue;
  TStaticText *textDefaultValue;
    TPanel *panelString;
    TBitBtn *btnA;
    TBitBtn *btnE;
    TBitBtn *btnD;
    TBitBtn *btnC;
    TBitBtn *btnF;
    TBitBtn *btnG;
    TBitBtn *btnB;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn7;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn6;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TBitBtn *BitBtn21;
    TBitBtn *BitBtn10;
    TBitBtn *BitBtn16;
    TBitBtn *BitBtn11;
    TBitBtn *BitBtn13;
    TBitBtn *BitBtn18;
    TBitBtn *BitBtn14;
    TBitBtn *BitBtn8;
    TBitBtn *BitBtn9;
    TBitBtn *BitBtn20;
    TBitBtn *BitBtn15;
    TBitBtn *BitBtn17;
    TBitBtn *BitBtn19;
    TBitBtn *BitBtn12;
    TBitBtn *BitBtn22;
    TBitBtn *BitBtn23;
    TBitBtn *BitBtn24;
    TBitBtn *BitBtn25;
    TBitBtn *BitBtn26;
    TBitBtn *BitBtn27;
    TBitBtn *BitBtn28;
    TBitBtn *BitBtn29;
    TBitBtn *BitBtn30;
    TBitBtn *BitBtn31;
    TPanel *Panel2;
    TBitBtn *btnOK;
    TBitBtn *btnCancel;
    TPanel *panelNum;
    TBitBtn *btnNo1;
    TBitBtn *btnNo2;
    TBitBtn *btnNo3;
    TBitBtn *btnNo4;
    TBitBtn *btnNo5;
    TBitBtn *btnNo6;
    TBitBtn *btnNo7;
    TBitBtn *btnNo8;
    TBitBtn *btnNo9;
    TBitBtn *btnNo0;
    TBitBtn *btnDot;
    TBitBtn *btnBack;
    TBitBtn *btnPositive;
    TBitBtn *btnNegative;
    TBitBtn *BitBtn32;
    TLabel *labelMax;
    TLabel *labelMin;
    TBitBtn *BitBtn33;
  void __fastcall OnKeyButtonClick(TObject *Sender);
  void __fastcall btnBackClick(TObject *Sender);
  void __fastcall btnPositiveClick(TObject *Sender);
  void __fastcall btnNegativeClick(TObject *Sender);
  void __fastcall btnOKClick(TObject *Sender);
  void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
  void SetBigStyle(){Width=752;panelNum->Visible=false;panelString->Visible=true;};
  bool m_bUseDot;
  bool m_bUseNegative;
  __fastcall TKeyboardWnd(TComponent* Owner);
};

//---------------------------------------------------------------------------
//extern PACKAGE TKeyboardWnd *KeyboardWnd;

//---------------------------------------------------------------------------
#endif
