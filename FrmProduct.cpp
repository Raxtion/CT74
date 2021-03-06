//---------------------------------------------------------------------------

#include <vcl.h>
#include "IniFile.h"
#pragma hdrstop

#include "FrmProduct.h"
#include "MyPISODIO.h"
#include "fmHeadScalModify.h"
#include "fmMachinParam.h"
#include "fmMotorCheck.h"
#include "fmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern CIniFile g_IniFile;
extern CMyPISODIO g_DIO;
extern TfrmMain *frmMain;

//TfmProduct *fmProduct;
//---------------------------------------------------------------------------
__fastcall TfmProduct::TfmProduct(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfmProduct::FormCreate(TObject *Sender)
{
    RenewRadioGroup();
    RenewCmbScal("HeadScal");
    RenewCmbScal("ModuleScal");

    //Let the image1 do not show background color
    Image1->Transparent=true;
    Graphics::TBitmap *Bmp = new Graphics::TBitmap();
    Bmp->Assign(Image1->Picture->Bitmap);
    Bmp->Transparent = true;
    Bmp->TransparentColor = Bmp->Canvas->Pixels[1][1];
    Image1->Picture->Assign(Bmp);

    Bmp->Assign(Image2->Picture->Bitmap);
    Bmp->Transparent = true;
    Bmp->TransparentColor = Bmp->Canvas->Pixels[1][1];
    Image2->Picture->Assign(Bmp);

    Bmp->Assign(Image3->Picture->Bitmap);
    Bmp->Transparent = true;
    Bmp->TransparentColor = Bmp->Canvas->Pixels[1][1];
    Image3->Picture->Assign(Bmp);

    Bmp->Assign(Image4->Picture->Bitmap);
    Bmp->Transparent = true;
    Bmp->TransparentColor = Bmp->Canvas->Pixels[1][1];
    Image4->Picture->Assign(Bmp);
    delete Bmp;
}
//---------------------------------------------------------------------------
void __fastcall TfmProduct::RenewRadioGroup()
{
        if (g_IniFile.m_nLanguageMode>0)
        {
                //Create radioPosOption
                m_nRailOption->Items->Clear();
                m_nRailOption->Items->Add("All Lane");
                m_nRailOption->Items->Add("Only Front Lane");
                m_nRailOption->Items->Add("Only Rear Lane");
                btnHeadScalModify->Caption = "HeadScal Modify";
        }
        else
        {
                //Create radioPosOption
                m_nRailOption->Items->Clear();
                m_nRailOption->Items->Add("全部都開啟");
                m_nRailOption->Items->Add("前流道");
                m_nRailOption->Items->Add("後流道");
                btnHeadScalModify->Caption = "壓頭尺寸修改";
        }
}

//---------------------------------------------------------------------------
void __fastcall TfmProduct::RenewCmbScal(AnsiString Hint)
{
    TComboBox* pCmb;
    if (Hint == "HeadScal") pCmb = this->m_cmbHeadScal;
    else if (Hint == "ModuleScal") pCmb = this->m_cmbModuleScal;
    else return;

    pCmb->Clear();

    TStringList* strList;
    if (Hint == "HeadScal") strList = SplitString(g_IniFile.m_strHeadScals, "/");
    else if (Hint == "ModuleScal") strList = SplitString(g_IniFile.m_strModuleScals, "/");
    else return;

    for (int i=0;i<strList->Count;i++)
    {
        pCmb->AddItem(strList->Strings[i], NULL);
    }
    pCmb->ItemIndex = 0;
    delete strList;
}

//---------------------------------------------------------------------------
void __fastcall TfmProduct::btnScalModifyClick(TObject *Sender)
{
    TButton* pbtn = (TButton*)Sender;
    TfrmHeadScalModify *pHeadScalModifyDlg = new TfrmHeadScalModify(this);

    TStringList* strList;
    if (pbtn->Hint == "HeadScal") strList = SplitString(g_IniFile.m_strHeadScals, "/");
    else if (pbtn->Hint == "ModuleScal") strList = SplitString(g_IniFile.m_strModuleScals, "/");
    else return;

    pHeadScalModifyDlg->cmbSelectData->AddItem("", NULL);
    for (int i=0;i<strList->Count;i++)
    {
        pHeadScalModifyDlg->cmbSelectData->AddItem(strList->Strings[i], NULL);
    }
    pHeadScalModifyDlg->cmbSelectData->ItemIndex = 0;
    Shortint result = pHeadScalModifyDlg->ShowModal();
	if (result == mrOk)                                                 //新增
	{
        bool bIsInputINstrList = false;
        for (int i=0;i<strList->Count;i++)
        {
            if (pHeadScalModifyDlg->cmbSelectData->Text == strList->Strings[i])
            {
                bIsInputINstrList = true;
                break;
            }
            else bIsInputINstrList = false;
        }
        if (bIsInputINstrList == false && pHeadScalModifyDlg->cmbSelectData->Text != "")
        {
            if (pbtn->Hint == "HeadScal") g_IniFile.m_strHeadScals += (pHeadScalModifyDlg->cmbSelectData->Text+"/");
            else if (pbtn->Hint == "ModuleScal") g_IniFile.m_strModuleScals += (pHeadScalModifyDlg->cmbSelectData->Text+"/");
            g_IniFile.MachineFile(false);
        }
	}
    else if (result == mrYes)                                           //刪除
    {
        bool bIsInputINstrList = false;
        for (int i=0;i<strList->Count;i++)
        {
            if (pHeadScalModifyDlg->cmbSelectData->Text == strList->Strings[i])
            {
                bIsInputINstrList = true;
                break;
            }
            else bIsInputINstrList = false;
        }
        if (bIsInputINstrList == true && pHeadScalModifyDlg->cmbSelectData->Text != "")
        {
            if (pbtn->Hint == "HeadScal") ReplaceString(&g_IniFile.m_strHeadScals, pHeadScalModifyDlg->cmbSelectData->Items->Strings[pHeadScalModifyDlg->cmbSelectData->ItemIndex]+"/", "");
            else if (pbtn->Hint == "ModuleScal") ReplaceString(&g_IniFile.m_strModuleScals, pHeadScalModifyDlg->cmbSelectData->Items->Strings[pHeadScalModifyDlg->cmbSelectData->ItemIndex]+"/", "");
            g_IniFile.MachineFile(false);
        }
    }
    RenewCmbScal(pbtn->Hint);
    delete strList;
    delete pHeadScalModifyDlg;
}
//---------------------------------------------------------------------------


void __fastcall TfmProduct::btnGenerateEndLocClick(TObject *Sender)
{
    TBitBtn* pbtn = (TBitBtn*)Sender;
    AnsiString strHint = pbtn->Hint;
    if (strHint == "00")        //Rear Down
    {
        m_dLaserDownPosEndX0->Text = FormatFloat("0.000", m_dLaserDownPosX0->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserDownPosEndY0->Text = FormatFloat("0.000", m_dLaserDownPosY0->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
    }
    else if (strHint == "01")   //Rear Up
    {
        m_dLaserUpPosEndX00->Text = FormatFloat("0.000", m_dLaserUpPosX00->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserUpPosEndX01->Text = FormatFloat("0.000", m_dLaserUpPosX01->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserUpPosEndX02->Text = FormatFloat("0.000", m_dLaserUpPosX02->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserUpPosEndX03->Text = FormatFloat("0.000", m_dLaserUpPosX03->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());

        m_dLaserUpPosEndY00->Text = FormatFloat("0.000", m_dLaserUpPosY00->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
        m_dLaserUpPosEndY01->Text = FormatFloat("0.000", m_dLaserUpPosY01->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
        m_dLaserUpPosEndY02->Text = FormatFloat("0.000", m_dLaserUpPosY02->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
        m_dLaserUpPosEndY03->Text = FormatFloat("0.000", m_dLaserUpPosY03->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
    }
    else if (strHint == "10")   //Front Down
    {
        m_dLaserDownPosEndX1->Text = FormatFloat("0.000", m_dLaserDownPosX1->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserDownPosEndY1->Text = FormatFloat("0.000", m_dLaserDownPosY1->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
    }
    else if (strHint == "11")   //Front Up
    {
        m_dLaserUpPosEndX10->Text = FormatFloat("0.000", m_dLaserUpPosX10->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserUpPosEndX11->Text = FormatFloat("0.000", m_dLaserUpPosX11->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserUpPosEndX12->Text = FormatFloat("0.000", m_dLaserUpPosX12->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());
        m_dLaserUpPosEndX13->Text = FormatFloat("0.000", m_dLaserUpPosX13->Text.ToDouble() + (m_nCols->Text.ToDouble()-1)*m_dColPitch->Text.ToDouble());

        m_dLaserUpPosEndY10->Text = FormatFloat("0.000", m_dLaserUpPosY10->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
        m_dLaserUpPosEndY11->Text = FormatFloat("0.000", m_dLaserUpPosY11->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
        m_dLaserUpPosEndY12->Text = FormatFloat("0.000", m_dLaserUpPosY12->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
        m_dLaserUpPosEndY13->Text = FormatFloat("0.000", m_dLaserUpPosY13->Text.ToDouble() - (m_nRows->Text.ToDouble()-1)*m_dRowPitch->Text.ToDouble());
    }
}
//---------------------------------------------------------------------------


void __fastcall TfmProduct::m_dLaserPosDblClick(TObject *Sender)
{
    //pEdit->Tag X=0, Y=1, Lane Changer=2, Front Lifter=3, Rear Lifter=4

    TEdit *pEdit=(TEdit *)Sender;
    TfrmMotorCheck *pMotorCheckDlg;

    AnsiString strCap = "";
    if (pEdit->Tag == 0)
    {
        strCap = "X";
        if (!g_DIO.ReadDIBit(DI::LoadCellDown))
        {
            Application->MessageBoxA("請確認LoadCell是否在上位!!", "Confirm", MB_OK);
            return;
        }
    }
    else if (pEdit->Tag == 1)
    {
        strCap = "Y";
        if (!g_DIO.ReadDIBit(DI::LoadCellDown))
        {
            Application->MessageBoxA("請確認LoadCell是否在上位!!", "Confirm", MB_OK);
            return;
        }
    }
    else if (pEdit->Tag == 2) strCap = "Lane Changer";
    else if (pEdit->Tag == 4)
    {
        strCap = "Front Lifter";
        if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB))
        {
            Application->MessageBoxA("請確認LoadCell是否在安全位置!!", "Confirm", MB_OK);
            return;
        }
    }
    else if (pEdit->Tag == 5)
    {
        strCap = "Rear Lifter";
        if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB))
        {
            Application->MessageBoxA("請確認LoadCell是否在安全位置!!", "Confirm", MB_OK);
            return;
        }
    }
    else strCap = "Error!";

    pMotorCheckDlg=new TfrmMotorCheck(this);
    pMotorCheckDlg->m_nActiveAxis=pEdit->Tag;
    pMotorCheckDlg->Caption=strCap;

    AnsiString sPath = g_IniFile.m_strApplicationPath;
	sPath = StringReplace(sPath, "\\exe\\", "", TReplaceFlags());

    if (pMotorCheckDlg->m_nActiveAxis == 0)
    {
        pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath + "\\bmp\\right.bmp");
        pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath + "\\bmp\\left.bmp");
    }
    else if (pMotorCheckDlg->m_nActiveAxis == 4 || pMotorCheckDlg->m_nActiveAxis == 5)
    {
        pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath + "\\bmp\\down.bmp");
        pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath + "\\bmp\\up.bmp");
    }

    if(pMotorCheckDlg->ShowModal()==mrOk)
    {
        TEdit *pEdit2=(TEdit *)FindComponent(pEdit->Hint);
        if(pEdit2) pEdit2->Text=pMotorCheckDlg->m_dCurrPos;
        else ShowMessage("寫入失敗");
    }

    delete pMotorCheckDlg;
}
//---------------------------------------------------------------------------


void __fastcall TfmProduct::btnRead1DFrontClick(TObject *Sender)
{
    C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);

    TSpeedButton *pBtn = (TSpeedButton *)Sender;
	pBtn->Enabled = false;

    frmMain->str1DReaderData[1] = "";
    frmMain->client1DF->Socket->SendText("LON\r\n");

    tm1MS.timeStart(5000);
	while (1)
	{
		if (frmMain->str1DReaderData[1] != "") break;
		if (tm1MS.timeUp()) break;
		Application->ProcessMessages();
	}

    if(frmMain->str1DReaderData[1] == "")
    {
        ShowMessage("1D Reader Time Out!");
        frmMain->client1DF->Socket->SendText("LOFF\r\n");
        pBtn->Enabled = true;
        return;
    }
    else m_strModuleNum1->Text = frmMain->str1DReaderData[1];

    pBtn->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfmProduct::btnRead1DRearClick(TObject *Sender)
{
    C_GetTime tm1MS(EX_SCALE::TIME_1MS, false);

    TSpeedButton *pBtn = (TSpeedButton *)Sender;
	pBtn->Enabled = false;

    frmMain->str1DReaderData[0] = "";
    frmMain->client1DR->Socket->SendText("LON\r\n");

    tm1MS.timeStart(5000);
	while (1)
	{
		if (frmMain->str1DReaderData[0] != "") break;
		if (tm1MS.timeUp()) break;
		Application->ProcessMessages();
	}

    if(frmMain->str1DReaderData[0] == "")
    {
        ShowMessage("1D Reader Time Out!");
        frmMain->client1DR->Socket->SendText("LOFF\r\n");
        pBtn->Enabled = true;
        return;
    }
    else m_strModuleNum0->Text = frmMain->str1DReaderData[0];

    pBtn->Enabled = true;    
}
//---------------------------------------------------------------------------

void __fastcall TfmProduct::btn1DReaderReconnectClick(TObject *Sender)
{
    frmMain->client1DF->Active = false;
    frmMain->client1DR->Active = false;

    ::Sleep(1000);

    frmMain->client1DF->Active = true;
    frmMain->client1DR->Active = true;
}
//---------------------------------------------------------------------------

void __fastcall TfmProduct::m_cmbUseLamCorrectBoardClick(TObject *Sender)
{
    TComboBox *pBtn = (TComboBox *)Sender;
    if (pBtn->ItemIndex == 1)
    {
        m_dLamCorrBoardHeight->Enabled = false;
        m_dLamCorrBoardHeight->Color = clGray;
    }
    else
    {
        m_dLamCorrBoardHeight->Enabled = true;
        m_dLamCorrBoardHeight->Color = clWindow;
    }
}
//---------------------------------------------------------------------------

