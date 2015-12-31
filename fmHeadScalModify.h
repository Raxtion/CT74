//---------------------------------------------------------------------------

#ifndef fmHeadScalModifyH
#define fmHeadScalModifyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmHeadScalModify : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TComboBox *cmbSelectData;
    TBitBtn *BitBtn3;
    TLabel *Label1;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmHeadScalModify(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHeadScalModify *frmHeadScalModify;
//---------------------------------------------------------------------------
#endif
