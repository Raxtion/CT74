//---------------------------------------------------------------------------

#ifndef fmPasswordH
#define fmPasswordH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmPassword : public TForm
{
__published:	// IDE-managed Components
    TMaskEdit *editOldPassword;
    TLabel *labelOldPassword;
    TLabel *labelNewPassword;
    TMaskEdit *editNewPassword;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    void __fastcall editOldPasswordClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmPassword(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPassword *frmPassword;
//---------------------------------------------------------------------------
#endif
