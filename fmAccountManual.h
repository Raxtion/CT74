//---------------------------------------------------------------------------

#ifndef fmAccountManualH
#define fmAccountManualH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmAccountManual : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn1;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmAccountManual(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAccountManual *frmAccountManual;
//---------------------------------------------------------------------------
#endif
