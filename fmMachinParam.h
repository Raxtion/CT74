//---------------------------------------------------------------------------

#ifndef fmMachinParamH
#define fmMachinParamH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmMachineParam : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TGroupBox *GroupBox1;
        TEdit *editAcc1;
        TEdit *editDec1;
        TEdit *editJogSpeed1;
        TEdit *editWorkSpeed1;
        TEdit *editAcc2;
        TEdit *editDec2;
        TEdit *editJogSpeed2;
        TEdit *editWorkSpeed2;
        TEdit *editAcc3;
        TEdit *editDec3;
        TEdit *editJogSpeed3;
        TEdit *editWorkSpeed3;
        TEdit *editAcc4;
        TEdit *editDec4;
        TEdit *editJogSpeed4;
        TEdit *editWorkSpeed4;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        TStaticText *StaticText4;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TStaticText *StaticText7;
        TStaticText *StaticText8;
        TStaticText *StaticText9;
        TStaticText *StaticText10;
        TStaticText *StaticText11;
        TStaticText *StaticText12;
        TStaticText *StaticText13;
        TStaticText *StaticText14;
        TEdit *editAcc5;
        TEdit *editDec5;
        TEdit *editJogSpeed5;
        TEdit *editWorkSpeed5;
        TStaticText *StaticText15;
        TStaticText *StaticText16;
        TEdit *m_dSafePos;
        TStaticText *StaticText17;
        TGroupBox *GroupBox2;
        TStaticText *StaticText18;
        TEdit *m_dLCEntryPos;
        TStaticText *StaticText19;
        TStaticText *StaticText20;
        TEdit *m_dLCFrontPos;
        TStaticText *StaticText21;
        TStaticText *StaticText22;
        TEdit *m_dLCRearPos;
        TStaticText *StaticText23;
        TEdit *editAcc6;
        TEdit *editDec6;
        TEdit *editJogSpeed6;
        TEdit *editWorkSpeed6;
        TEdit *editAcc7;
        TEdit *editDec7;
        TEdit *editJogSpeed7;
        TEdit *editWorkSpeed7;
        TEdit *editAcc8;
        TEdit *editDec8;
        TEdit *editJogSpeed8;
        TEdit *editWorkSpeed8;
        TCheckBox *m_bForceEject;
        TGroupBox *GroupBox3;
        TStaticText *StaticText24;
        TStaticText *StaticText25;
        TEdit *m_dPLimitF;
        TEdit *m_dNLimitF;
        TStaticText *StaticText28;
        TStaticText *StaticText26;
        TStaticText *StaticText27;
        TStaticText *StaticText29;
        TEdit *m_dPLimitR;
        TEdit *m_dNLimitR;
        TStaticText *StaticText30;
        TStaticText *StaticText31;
        TComboBox *m_cmbLanguage;
        TLabel *Label1;
private:	// User declarations
public:		// User declarations
        __fastcall TfrmMachineParam(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMachineParam *frmMachineParam;
//---------------------------------------------------------------------------
#endif
