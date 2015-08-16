//---------------------------------------------------------------------------

#ifndef fmChoiceMotorH
#define fmChoiceMotorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmChoiceMotor : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TBitBtn *BitBtn4;
        TBitBtn *BitBtn5;
        TBitBtn *BitBtn6;
    void __fastcall btnPickAxisClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmChoiceMotor(TComponent* Owner);
    UINT m_nSelectAxis;
    AnsiString m_strCaption;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmChoiceMotor *frmChoiceMotor;
//---------------------------------------------------------------------------
#endif
