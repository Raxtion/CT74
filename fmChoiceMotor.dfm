object frmChoiceMotor: TfrmChoiceMotor
  Left = 972
  Top = 151
  BorderStyle = bsDialog
  BorderWidth = 5
  Caption = #36984#25799#39340#36948
  ClientHeight = 483
  ClientWidth = 238
  Color = 13430215
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = #26032#32048#26126#39636
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 16
  object BitBtn1: TBitBtn
    Tag = 3
    Left = 16
    Top = 416
    Width = 185
    Height = 57
    Caption = #38626'  '#38283
    ModalResult = 2
    TabOrder = 0
  end
  object BitBtn2: TBitBtn
    Left = 16
    Top = 16
    Width = 185
    Height = 65
    Caption = ' X'
    ModalResult = 1
    TabOrder = 1
    OnClick = btnPickAxisClick
  end
  object BitBtn3: TBitBtn
    Tag = 1
    Left = 16
    Top = 96
    Width = 185
    Height = 65
    Caption = 'Y'
    ModalResult = 1
    TabOrder = 2
    OnClick = btnPickAxisClick
  end
  object BitBtn4: TBitBtn
    Tag = 2
    Left = 16
    Top = 176
    Width = 185
    Height = 65
    Caption = 'Lane Changer'
    ModalResult = 1
    TabOrder = 3
    OnClick = btnPickAxisClick
  end
  object BitBtn5: TBitBtn
    Tag = 4
    Left = 16
    Top = 256
    Width = 185
    Height = 65
    Caption = 'Front Lifter'
    ModalResult = 1
    TabOrder = 4
    OnClick = btnPickAxisClick
  end
  object BitBtn6: TBitBtn
    Tag = 5
    Left = 16
    Top = 336
    Width = 185
    Height = 65
    Caption = 'Rear Lifter'
    ModalResult = 1
    TabOrder = 5
    OnClick = btnPickAxisClick
  end
end
