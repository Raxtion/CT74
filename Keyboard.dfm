object KeyboardWnd: TKeyboardWnd
  Left = 252
  Top = 177
  BorderStyle = bsDialog
  Caption = 'Keyboard'
  ClientHeight = 451
  ClientWidth = 280
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 280
    Height = 65
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 0
    DesignSize = (
      280
      65)
    object editSetValue: TEdit
      Left = 8
      Top = 32
      Width = 265
      Height = 25
      Anchors = [akLeft, akTop, akRight, akBottom]
      AutoSelect = False
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
      Text = 'editSetValue'
    end
    object textDefaultValue: TStaticText
      Left = 8
      Top = 10
      Width = 266
      Height = 22
      Anchors = [akLeft, akTop, akRight, akBottom]
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'textDefaultValue'
      Color = 16744448
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 1
    end
  end
  object panelString: TPanel
    Left = 0
    Top = 64
    Width = 737
    Height = 305
    BevelInner = bvLowered
    TabOrder = 1
    Visible = False
    object btnA: TBitBtn
      Tag = 65
      Left = 56
      Top = 128
      Width = 57
      Height = 49
      Caption = 'A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = OnKeyButtonClick
    end
    object btnE: TBitBtn
      Tag = 69
      Left = 152
      Top = 72
      Width = 57
      Height = 49
      Caption = 'E'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = OnKeyButtonClick
    end
    object btnD: TBitBtn
      Tag = 68
      Left = 192
      Top = 128
      Width = 57
      Height = 49
      Caption = 'D'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = OnKeyButtonClick
    end
    object btnC: TBitBtn
      Tag = 67
      Left = 216
      Top = 184
      Width = 57
      Height = 49
      Caption = 'C'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = OnKeyButtonClick
    end
    object btnF: TBitBtn
      Tag = 70
      Left = 256
      Top = 128
      Width = 57
      Height = 49
      Caption = 'F'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      OnClick = OnKeyButtonClick
    end
    object btnG: TBitBtn
      Tag = 71
      Left = 320
      Top = 128
      Width = 57
      Height = 49
      Caption = 'G'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
      OnClick = OnKeyButtonClick
    end
    object btnB: TBitBtn
      Tag = 66
      Left = 344
      Top = 184
      Width = 57
      Height = 49
      Caption = 'B'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
      OnClick = OnKeyButtonClick
    end
    object BitBtn1: TBitBtn
      Tag = 72
      Left = 384
      Top = 128
      Width = 57
      Height = 49
      Caption = 'H'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
      OnClick = OnKeyButtonClick
    end
    object BitBtn7: TBitBtn
      Tag = 78
      Left = 408
      Top = 184
      Width = 57
      Height = 49
      Caption = 'N'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 8
      OnClick = OnKeyButtonClick
    end
    object BitBtn3: TBitBtn
      Tag = 74
      Left = 448
      Top = 128
      Width = 57
      Height = 49
      Caption = 'J'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 9
      OnClick = OnKeyButtonClick
    end
    object BitBtn6: TBitBtn
      Tag = 77
      Left = 472
      Top = 184
      Width = 57
      Height = 49
      Caption = 'M'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 10
      OnClick = OnKeyButtonClick
    end
    object BitBtn2: TBitBtn
      Tag = 73
      Left = 472
      Top = 72
      Width = 57
      Height = 49
      Caption = 'I'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 11
      OnClick = OnKeyButtonClick
    end
    object BitBtn4: TBitBtn
      Tag = 75
      Left = 512
      Top = 128
      Width = 57
      Height = 49
      Caption = 'K'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 12
      OnClick = OnKeyButtonClick
    end
    object BitBtn5: TBitBtn
      Tag = 76
      Left = 576
      Top = 128
      Width = 57
      Height = 49
      Caption = 'L'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 13
      OnClick = OnKeyButtonClick
    end
    object BitBtn21: TBitBtn
      Tag = 77
      Left = 112
      Top = 240
      Width = 457
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 14
    end
    object BitBtn10: TBitBtn
      Tag = 81
      Left = 24
      Top = 72
      Width = 57
      Height = 49
      Caption = 'Q'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 15
      OnClick = OnKeyButtonClick
    end
    object BitBtn16: TBitBtn
      Tag = 87
      Left = 88
      Top = 72
      Width = 57
      Height = 49
      Caption = 'W'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 16
      OnClick = OnKeyButtonClick
    end
    object BitBtn11: TBitBtn
      Tag = 82
      Left = 216
      Top = 72
      Width = 57
      Height = 49
      Caption = 'R'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 17
      OnClick = OnKeyButtonClick
    end
    object BitBtn13: TBitBtn
      Tag = 84
      Left = 280
      Top = 72
      Width = 57
      Height = 49
      Caption = 'T'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 18
      OnClick = OnKeyButtonClick
    end
    object BitBtn18: TBitBtn
      Tag = 89
      Left = 344
      Top = 72
      Width = 57
      Height = 49
      Caption = 'Y'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 19
      OnClick = OnKeyButtonClick
    end
    object BitBtn14: TBitBtn
      Tag = 85
      Left = 408
      Top = 72
      Width = 57
      Height = 49
      Caption = 'U'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 20
      OnClick = OnKeyButtonClick
    end
    object BitBtn8: TBitBtn
      Tag = 79
      Left = 536
      Top = 72
      Width = 57
      Height = 49
      Caption = 'O'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 21
      OnClick = OnKeyButtonClick
    end
    object BitBtn9: TBitBtn
      Tag = 80
      Left = 608
      Top = 72
      Width = 57
      Height = 49
      Caption = 'P'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 22
      OnClick = OnKeyButtonClick
    end
    object BitBtn20: TBitBtn
      Tag = 95
      Left = 536
      Top = 184
      Width = 57
      Height = 49
      Caption = '_'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 23
      OnClick = OnKeyButtonClick
    end
    object BitBtn15: TBitBtn
      Tag = 86
      Left = 280
      Top = 184
      Width = 57
      Height = 49
      Caption = 'V'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 24
      OnClick = OnKeyButtonClick
    end
    object BitBtn17: TBitBtn
      Tag = 88
      Left = 152
      Top = 184
      Width = 57
      Height = 49
      Caption = 'X'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 25
      OnClick = OnKeyButtonClick
    end
    object BitBtn19: TBitBtn
      Tag = 90
      Left = 88
      Top = 184
      Width = 57
      Height = 49
      Caption = 'Z'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 26
      OnClick = OnKeyButtonClick
    end
    object BitBtn12: TBitBtn
      Tag = 83
      Left = 120
      Top = 128
      Width = 57
      Height = 49
      Caption = 'S'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 8404992
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 27
      OnClick = OnKeyButtonClick
    end
    object BitBtn22: TBitBtn
      Tag = 49
      Left = 8
      Top = 16
      Width = 57
      Height = 49
      Caption = '1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 28
      OnClick = OnKeyButtonClick
    end
    object BitBtn23: TBitBtn
      Tag = 50
      Left = 72
      Top = 16
      Width = 57
      Height = 49
      Caption = '2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 29
      OnClick = OnKeyButtonClick
    end
    object BitBtn24: TBitBtn
      Tag = 51
      Left = 136
      Top = 16
      Width = 57
      Height = 49
      Caption = '3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 30
      OnClick = OnKeyButtonClick
    end
    object BitBtn25: TBitBtn
      Tag = 54
      Left = 328
      Top = 16
      Width = 57
      Height = 49
      Caption = '6'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 31
      OnClick = OnKeyButtonClick
    end
    object BitBtn26: TBitBtn
      Tag = 53
      Left = 264
      Top = 16
      Width = 57
      Height = 49
      Caption = '5'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 32
      OnClick = OnKeyButtonClick
    end
    object BitBtn27: TBitBtn
      Tag = 52
      Left = 200
      Top = 16
      Width = 57
      Height = 49
      Caption = '4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 33
      OnClick = OnKeyButtonClick
    end
    object BitBtn28: TBitBtn
      Tag = 55
      Left = 392
      Top = 16
      Width = 57
      Height = 49
      Caption = '7'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 34
      OnClick = OnKeyButtonClick
    end
    object BitBtn29: TBitBtn
      Tag = 56
      Left = 456
      Top = 16
      Width = 57
      Height = 49
      Caption = '8'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 35
      OnClick = OnKeyButtonClick
    end
    object BitBtn30: TBitBtn
      Tag = 57
      Left = 520
      Top = 16
      Width = 57
      Height = 49
      Caption = '9'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 36
      OnClick = OnKeyButtonClick
    end
    object BitBtn31: TBitBtn
      Tag = 48
      Left = 584
      Top = 16
      Width = 57
      Height = 49
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 37
      OnClick = OnKeyButtonClick
    end
    object BitBtn32: TBitBtn
      Left = 648
      Top = 16
      Width = 81
      Height = 49
      Caption = #8592
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'Arial Black'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 38
      OnClick = btnBackClick
    end
    object BitBtn33: TBitBtn
      Tag = 46
      Left = 600
      Top = 184
      Width = 57
      Height = 49
      Caption = '.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 39
      OnClick = OnKeyButtonClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 370
    Width = 280
    Height = 81
    Align = alBottom
    BevelInner = bvLowered
    TabOrder = 2
    object btnOK: TBitBtn
      Tag = 46
      Left = 8
      Top = 16
      Width = 113
      Height = 49
      Caption = #30906#23450
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = btnOKClick
    end
    object btnCancel: TBitBtn
      Tag = 46
      Left = 144
      Top = 16
      Width = 113
      Height = 49
      Caption = #21462#28040
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = btnCancelClick
    end
  end
  object panelNum: TPanel
    Left = 0
    Top = 65
    Width = 281
    Height = 304
    BevelInner = bvLowered
    TabOrder = 3
    object labelMax: TLabel
      Left = 24
      Top = 8
      Width = 209
      Height = 13
      AutoSize = False
      Caption = 'labelMax'
    end
    object labelMin: TLabel
      Left = 24
      Top = 24
      Width = 241
      Height = 13
      AutoSize = False
      Caption = 'labelMin'
    end
    object btnNo1: TBitBtn
      Tag = 49
      Left = 16
      Top = 48
      Width = 57
      Height = 49
      Caption = '1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = OnKeyButtonClick
    end
    object btnNo2: TBitBtn
      Tag = 50
      Left = 80
      Top = 48
      Width = 57
      Height = 49
      Caption = '2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = OnKeyButtonClick
    end
    object btnNo3: TBitBtn
      Tag = 51
      Left = 144
      Top = 48
      Width = 57
      Height = 49
      Caption = '3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = OnKeyButtonClick
    end
    object btnNo4: TBitBtn
      Tag = 52
      Left = 16
      Top = 104
      Width = 57
      Height = 49
      Caption = '4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = OnKeyButtonClick
    end
    object btnNo5: TBitBtn
      Tag = 53
      Left = 80
      Top = 104
      Width = 57
      Height = 49
      Caption = '5'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      OnClick = OnKeyButtonClick
    end
    object btnNo6: TBitBtn
      Tag = 54
      Left = 144
      Top = 104
      Width = 57
      Height = 49
      Caption = '6'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
      OnClick = OnKeyButtonClick
    end
    object btnNo7: TBitBtn
      Tag = 55
      Left = 16
      Top = 160
      Width = 57
      Height = 49
      Caption = '7'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
      OnClick = OnKeyButtonClick
    end
    object btnNo8: TBitBtn
      Tag = 56
      Left = 80
      Top = 160
      Width = 57
      Height = 49
      Caption = '8'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
      OnClick = OnKeyButtonClick
    end
    object btnNo9: TBitBtn
      Tag = 57
      Left = 144
      Top = 160
      Width = 57
      Height = 49
      Caption = '9'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 8
      OnClick = OnKeyButtonClick
    end
    object btnNo0: TBitBtn
      Tag = 48
      Left = 80
      Top = 216
      Width = 57
      Height = 49
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 9
      OnClick = OnKeyButtonClick
    end
    object btnDot: TBitBtn
      Tag = 46
      Left = 144
      Top = 216
      Width = 57
      Height = 49
      Caption = '.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 10
      OnClick = OnKeyButtonClick
    end
    object btnBack: TBitBtn
      Left = 208
      Top = 48
      Width = 57
      Height = 49
      Caption = #8592
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'Arial Black'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 11
      OnClick = btnBackClick
    end
    object btnPositive: TBitBtn
      Tag = 51
      Left = 208
      Top = 104
      Width = 57
      Height = 49
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 12
      OnClick = btnPositiveClick
    end
    object btnNegative: TBitBtn
      Tag = 51
      Left = 208
      Top = 160
      Width = 57
      Height = 49
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHotLight
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 13
      OnClick = btnNegativeClick
    end
  end
end
