object frmPassword: TfrmPassword
  Left = 492
  Top = 309
  Width = 355
  Height = 215
  Caption = #23494#30908
  Color = 13430215
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = #26032#32048#26126#39636
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 16
  object labelOldPassword: TLabel
    Left = 40
    Top = 32
    Width = 96
    Height = 16
    Caption = #35531#36664#20837#33290#23494#30908
  end
  object labelNewPassword: TLabel
    Left = 40
    Top = 72
    Width = 96
    Height = 16
    Caption = #35531#36664#20837#26032#23494#30908
  end
  object editOldPassword: TMaskEdit
    Left = 144
    Top = 24
    Width = 153
    Height = 24
    PasswordChar = '*'
    TabOrder = 0
    OnClick = editOldPasswordClick
  end
  object editNewPassword: TMaskEdit
    Left = 144
    Top = 64
    Width = 153
    Height = 24
    PasswordChar = '*'
    TabOrder = 1
    OnClick = editOldPasswordClick
  end
  object BitBtn1: TBitBtn
    Left = 40
    Top = 112
    Width = 105
    Height = 41
    Caption = #30906#23450
    TabOrder = 2
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 200
    Top = 112
    Width = 97
    Height = 41
    Caption = #21462#28040
    TabOrder = 3
    Kind = bkCancel
  end
end
