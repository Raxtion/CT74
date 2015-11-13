//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("fmMain.cpp", frmMain);
USEFORM("fmIOCheck.cpp", IOCheckDlg);
USEFORM("fmPassword.cpp", frmPassword);
USEFORM("FrmProduct.cpp", fmProduct);
USEFORM("fmChoiceMotor.cpp", frmChoiceMotor);
USEFORM("fmMachinParam.cpp", frmMachineParam);
USEFORM("fmMotorCheck.cpp", frmMotorCheck);
USEFORM("fmAD.cpp", frmAD);
USEFORM("fmManual.cpp", frmManual);
USEFORM("fmOffset.cpp", frmOffser);
USEFORM("fmHeadScalModify.cpp", frmHeadScalModify);
USEFORM("fmAccountManual.cpp", frmAccountManual);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TfrmMain), &frmMain);
         Application->CreateForm(__classid(TfrmHeadScalModify), &frmHeadScalModify);
         Application->CreateForm(__classid(TfrmAccountManual), &frmAccountManual);
         Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------


