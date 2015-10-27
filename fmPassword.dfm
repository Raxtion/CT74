object frmPassword: TfrmPassword
  Left = 701
  Top = 201
  Width = 355
  Height = 204
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
    Left = 24
    Top = 48
    Width = 96
    Height = 16
    Caption = #35531#36664#20837#33290#23494#30908
  end
  object labelNewPassword: TLabel
    Left = 24
    Top = 76
    Width = 96
    Height = 16
    Caption = #35531#36664#20837#26032#23494#30908
  end
  object labelAccount: TLabel
    Left = 24
    Top = 19
    Width = 92
    Height = 16
    Caption = #35531#36664#20837'   '#24115#34399
  end
  object editOldPassword: TMaskEdit
    Left = 168
    Top = 44
    Width = 153
    Height = 24
    PasswordChar = '*'
    TabOrder = 0
    OnClick = editOldPasswordClick
  end
  object editNewPassword: TMaskEdit
    Left = 168
    Top = 75
    Width = 153
    Height = 24
    PasswordChar = '*'
    TabOrder = 1
    OnClick = editOldPasswordClick
  end
  object BitBtn1: TBitBtn
    Left = 40
    Top = 114
    Width = 105
    Height = 41
    Caption = #30906#23450
    TabOrder = 2
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 200
    Top = 114
    Width = 97
    Height = 41
    Caption = #21462#28040
    TabOrder = 3
    Kind = bkCancel
  end
  object editAccount: TEdit
    Left = 168
    Top = 14
    Width = 153
    Height = 24
    TabOrder = 4
  end
end
