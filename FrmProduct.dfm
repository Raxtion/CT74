object fmProduct: TfmProduct
  Left = 237
  Top = 3
  BorderStyle = bsDialog
  Caption = #29986#21697#21443#25976
  ClientHeight = 588
  ClientWidth = 1105
  Color = 13430215
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = #26032#32048#26126#39636
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 16
  object Label83: TLabel
    Left = 40
    Top = 544
    Width = 64
    Height = 16
    Caption = #33258#21205#27169#24335
  end
  object Label84: TLabel
    Left = 40
    Top = 560
    Width = 64
    Height = 16
    Caption = #23481#35377#28331#24230
  end
  object Label85: TLabel
    Left = 184
    Top = 552
    Width = 16
    Height = 16
    Caption = #24230
  end
  object Label86: TLabel
    Left = 112
    Top = 552
    Width = 12
    Height = 16
    Caption = #177' '
  end
  object Label88: TLabel
    Left = 528
    Top = 552
    Width = 16
    Height = 16
    Caption = #31186
  end
  object Label87: TLabel
    Left = 336
    Top = 552
    Width = 144
    Height = 16
    Caption = #31532#20108#27425#30495#31354#27298#28204#24310#36978
  end
  object Label92: TLabel
    Left = 592
    Top = 552
    Width = 57
    Height = 16
    Caption = 'Setup EE'
  end
  object Label93: TLabel
    Left = 792
    Top = 552
    Width = 42
    Height = 16
    Caption = '('#32232#34399')'
  end
  object m_bNotLam: TCheckBox
    Left = 232
    Top = 552
    Width = 73
    Height = 17
    Caption = #19981#22739#21512
    TabOrder = 13
  end
  object BitBtn1: TBitBtn
    Left = 856
    Top = 544
    Width = 97
    Height = 33
    TabOrder = 0
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 960
    Top = 544
    Width = 97
    Height = 33
    TabOrder = 1
    Kind = bkCancel
  end
  object GroupBox1: TGroupBox
    Left = 24
    Top = 384
    Width = 233
    Height = 153
    Caption = 'Tray Layout'
    TabOrder = 2
    object Label1: TLabel
      Left = 16
      Top = 29
      Width = 32
      Height = 16
      Caption = #34892#25976
    end
    object Label2: TLabel
      Left = 16
      Top = 54
      Width = 32
      Height = 16
      Caption = #21015#25976
    end
    object Label11: TLabel
      Left = 16
      Top = 84
      Width = 48
      Height = 16
      Caption = #34892#38291#36317
    end
    object Label12: TLabel
      Left = 16
      Top = 108
      Width = 48
      Height = 16
      Caption = #21015#38291#36317
    end
    object Label13: TLabel
      Left = 150
      Top = 84
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label14: TLabel
      Left = 150
      Top = 108
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object m_nCols: TEdit
      Left = 72
      Top = 24
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'm_nCols'
    end
    object m_nRows: TEdit
      Left = 72
      Top = 48
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
    object m_dColPitch: TEdit
      Left = 72
      Top = 80
      Width = 73
      Height = 24
      TabOrder = 2
      Text = 'Edit1'
    end
    object m_dRowPitch: TEdit
      Left = 72
      Top = 104
      Width = 73
      Height = 24
      TabOrder = 3
      Text = 'Edit1'
    end
  end
  object GroupBox3: TGroupBox
    Left = 16
    Top = 152
    Width = 241
    Height = 225
    Caption = #24460#22739#21512#27969#36947
    TabOrder = 3
    object Label3: TLabel
      Left = 24
      Top = 32
      Width = 64
      Height = 16
      Caption = #22739#21512#39640#24230
    end
    object Label4: TLabel
      Left = 24
      Top = 80
      Width = 64
      Height = 16
      Caption = #25509#29255#39640#24230
    end
    object Label16: TLabel
      Left = 190
      Top = 36
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label17: TLabel
      Left = 190
      Top = 148
      Width = 20
      Height = 16
      Caption = 'sec'
    end
    object Label9: TLabel
      Left = 4
      Top = 120
      Width = 90
      Height = 16
      Caption = '('#21560#30495#31354#39640#24230')'
    end
    object Label61: TLabel
      Left = 24
      Top = 56
      Width = 64
      Height = 16
      Caption = #20986#29255#39640#24230
    end
    object Label62: TLabel
      Left = 190
      Top = 172
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label35: TLabel
      Left = 8
      Top = 104
      Width = 80
      Height = 16
      Caption = #31532#20108#27573#39640#24230
    end
    object Label36: TLabel
      Left = 190
      Top = 196
      Width = 20
      Height = 16
      Caption = 'sec'
    end
    object Label37: TLabel
      Left = 190
      Top = 60
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label38: TLabel
      Left = 8
      Top = 144
      Width = 80
      Height = 16
      Caption = #31532#20108#27573#36895#24230
    end
    object Label39: TLabel
      Left = 8
      Top = 168
      Width = 80
      Height = 16
      Caption = #31532#19977#27573#39640#24230
    end
    object Label40: TLabel
      Left = 190
      Top = 116
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label41: TLabel
      Left = 8
      Top = 192
      Width = 80
      Height = 16
      Caption = #31532#19977#27573#36895#24230
    end
    object Label44: TLabel
      Left = 190
      Top = 84
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object m_dLamStop0: TEdit
      Left = 104
      Top = 80
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
    object m_dLamVacHeight0: TEdit
      Left = 104
      Top = 32
      Width = 73
      Height = 24
      Enabled = False
      TabOrder = 2
      Text = 'm_dLamVacHeight0'
      Visible = False
    end
    object m_dLamGetPos0: TEdit
      Left = 104
      Top = 56
      Width = 73
      Height = 24
      TabOrder = 3
      Text = 'Edit1'
    end
    object m_dLamSecondHeight0: TEdit
      Left = 104
      Top = 112
      Width = 73
      Height = 24
      TabOrder = 4
      Text = 'Edit1'
    end
    object m_dLamSecondTime0: TEdit
      Left = 104
      Top = 144
      Width = 73
      Height = 24
      TabOrder = 5
      Text = 'Edit1'
    end
    object m_dLamThirdHeight0: TEdit
      Left = 104
      Top = 168
      Width = 73
      Height = 24
      TabOrder = 6
      Text = 'Edit1'
    end
    object m_dLamThirdTime0: TEdit
      Left = 104
      Top = 192
      Width = 73
      Height = 24
      TabOrder = 7
      Text = 'Edit1'
    end
    object m_dLamHeight0: TEdit
      Left = 104
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'm_dLamHeight0'
    end
  end
  object GroupBox2: TGroupBox
    Left = 280
    Top = 152
    Width = 241
    Height = 225
    Caption = #21069#22739#21512#27969#36947
    TabOrder = 4
    object Label5: TLabel
      Left = 24
      Top = 32
      Width = 64
      Height = 16
      Caption = #22739#21512#39640#24230
    end
    object Label6: TLabel
      Left = 24
      Top = 80
      Width = 64
      Height = 16
      Caption = #25509#29255#39640#24230
    end
    object Label7: TLabel
      Left = 190
      Top = 36
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label8: TLabel
      Left = 190
      Top = 84
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label15: TLabel
      Left = 4
      Top = 120
      Width = 90
      Height = 16
      Caption = '('#21560#30495#31354#39640#24230')'
    end
    object Label59: TLabel
      Left = 24
      Top = 56
      Width = 64
      Height = 16
      Caption = #20986#29255#39640#24230
    end
    object Label60: TLabel
      Left = 190
      Top = 60
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label45: TLabel
      Left = 190
      Top = 148
      Width = 20
      Height = 16
      Caption = 'sec'
    end
    object Label51: TLabel
      Left = 190
      Top = 172
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label52: TLabel
      Left = 8
      Top = 104
      Width = 80
      Height = 16
      Caption = #31532#20108#27573#39640#24230
    end
    object Label53: TLabel
      Left = 190
      Top = 196
      Width = 20
      Height = 16
      Caption = 'sec'
    end
    object Label75: TLabel
      Left = 8
      Top = 144
      Width = 80
      Height = 16
      Caption = #31532#20108#27573#36895#24230
    end
    object Label76: TLabel
      Left = 8
      Top = 168
      Width = 80
      Height = 16
      Caption = #31532#19977#27573#39640#24230
    end
    object Label77: TLabel
      Left = 190
      Top = 116
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label78: TLabel
      Left = 8
      Top = 192
      Width = 80
      Height = 16
      Caption = #31532#19977#27573#36895#24230
    end
    object m_dLamStop1: TEdit
      Left = 104
      Top = 80
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
    object m_dLamVacHeight1: TEdit
      Left = 104
      Top = 32
      Width = 73
      Height = 24
      Enabled = False
      TabOrder = 2
      Text = 'm_dLamVacHeight1'
      Visible = False
    end
    object m_dLamGetPos1: TEdit
      Left = 104
      Top = 56
      Width = 73
      Height = 24
      TabOrder = 3
      Text = 'm_dLamGetPos1'
    end
    object m_dLamSecondHeight1: TEdit
      Left = 104
      Top = 112
      Width = 73
      Height = 24
      TabOrder = 4
      Text = 'Edit1'
    end
    object m_dLamSecondTime1: TEdit
      Left = 104
      Top = 144
      Width = 73
      Height = 24
      TabOrder = 5
      Text = 'Edit1'
    end
    object m_dLamThirdHeight1: TEdit
      Left = 104
      Top = 168
      Width = 73
      Height = 24
      TabOrder = 6
      Text = 'Edit1'
    end
    object m_dLamThirdTime1: TEdit
      Left = 104
      Top = 192
      Width = 73
      Height = 24
      TabOrder = 7
      Text = 'Edit1'
    end
    object m_dLamHeight1: TEdit
      Left = 104
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'm_dLamHeight0'
    end
  end
  object GroupBox4: TGroupBox
    Left = 16
    Top = 8
    Width = 241
    Height = 137
    Caption = #24460#22739#21512#21443#25976
    TabOrder = 5
    object Label19: TLabel
      Left = 16
      Top = 32
      Width = 64
      Height = 16
      Caption = #22739#21512#26178#38291
    end
    object Label20: TLabel
      Left = 16
      Top = 64
      Width = 64
      Height = 16
      Caption = #22739#21512#28331#24230
    end
    object Label21: TLabel
      Left = 16
      Top = 96
      Width = 64
      Height = 16
      Caption = #22739#21512#21147#37327
    end
    object Label22: TLabel
      Left = 174
      Top = 39
      Width = 16
      Height = 16
      Caption = #31186
    end
    object Label23: TLabel
      Left = 174
      Top = 72
      Width = 16
      Height = 16
      Caption = #24230
    end
    object Label24: TLabel
      Left = 174
      Top = 101
      Width = 32
      Height = 16
      Caption = #20844#26020
    end
    object m_dLamTime0: TEdit
      Left = 96
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'm_dLamHeight0'
    end
    object m_dLamTemp0: TEdit
      Left = 96
      Top = 64
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'm_dLamHeight0'
    end
    object m_dLamPress0: TEdit
      Left = 96
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 2
      Text = 'm_dLamHeight0'
    end
  end
  object GroupBox5: TGroupBox
    Left = 280
    Top = 8
    Width = 241
    Height = 137
    Caption = #21069#22739#21512#21443#25976
    TabOrder = 6
    object Label25: TLabel
      Left = 16
      Top = 32
      Width = 64
      Height = 16
      Caption = #22739#21512#26178#38291
    end
    object Label26: TLabel
      Left = 16
      Top = 64
      Width = 64
      Height = 16
      Caption = #22739#21512#28331#24230
    end
    object Label27: TLabel
      Left = 16
      Top = 96
      Width = 64
      Height = 16
      Caption = #22739#21512#21147#37327
    end
    object Label28: TLabel
      Left = 174
      Top = 39
      Width = 16
      Height = 16
      Caption = #31186
    end
    object Label29: TLabel
      Left = 174
      Top = 72
      Width = 16
      Height = 16
      Caption = #24230
    end
    object Label30: TLabel
      Left = 174
      Top = 101
      Width = 32
      Height = 16
      Caption = #20844#26020
    end
    object m_dLamTime1: TEdit
      Left = 96
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'm_dLamHeight0'
    end
    object m_dLamTemp1: TEdit
      Left = 96
      Top = 64
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'm_dLamHeight0'
    end
    object m_dLamPress1: TEdit
      Left = 96
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 2
      Text = 'm_dLamHeight0'
    end
  end
  object GroupBox6: TGroupBox
    Left = 536
    Top = 8
    Width = 273
    Height = 73
    Caption = #24460#22739#21512'['#28204#37325']'#36215#22987#20301#32622
    TabOrder = 7
    object Label31: TLabel
      Left = 24
      Top = 37
      Width = 36
      Height = 16
      Caption = '(X,Y)'
    end
    object Label34: TLabel
      Left = 222
      Top = 41
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object m_dLoadCellPosX0: TEdit
      Left = 64
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'm_dLoadCellPosX0'
    end
    object m_dLoadCellPosY0: TEdit
      Left = 144
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
  end
  object GroupBox8: TGroupBox
    Left = 536
    Top = 272
    Width = 273
    Height = 81
    Caption = #24460#22739#21512'['#19979#27169#28204#39640']'#36215#22987#20301#32622
    TabOrder = 8
    object Label42: TLabel
      Left = 222
      Top = 41
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label33: TLabel
      Left = 24
      Top = 37
      Width = 36
      Height = 16
      Caption = '(X,Y)'
    end
    object m_dLaserDownPosX0: TEdit
      Left = 64
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'Edit1'
    end
    object m_dLaserDownPosY0: TEdit
      Left = 144
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
  end
  object GroupBox9: TGroupBox
    Left = 816
    Top = 8
    Width = 273
    Height = 73
    Caption = #21069#22739#21512'['#28204#37325']'#36215#22987#20301#32622
    TabOrder = 9
    object Label43: TLabel
      Left = 24
      Top = 37
      Width = 36
      Height = 16
      Caption = '(X,Y)'
    end
    object Label46: TLabel
      Left = 222
      Top = 41
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object m_dLoadCellPosX1: TEdit
      Left = 64
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'Edit1'
    end
    object m_dLoadCellPosY1: TEdit
      Left = 144
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
  end
  object GroupBox10: TGroupBox
    Left = 816
    Top = 96
    Width = 273
    Height = 169
    Caption = #21069#22739#21512'['#19978#27169#28204#39640']'#36215#22987#20301#32622
    TabOrder = 10
    object Label47: TLabel
      Left = 8
      Top = 37
      Width = 52
      Height = 16
      Caption = '(X1,Y1)'
    end
    object Label50: TLabel
      Left = 222
      Top = 41
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label48: TLabel
      Left = 8
      Top = 69
      Width = 52
      Height = 16
      Caption = '(X2,Y2)'
    end
    object Label49: TLabel
      Left = 222
      Top = 73
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label63: TLabel
      Left = 8
      Top = 101
      Width = 52
      Height = 16
      Caption = '(X3,Y3)'
    end
    object Label64: TLabel
      Left = 222
      Top = 105
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label65: TLabel
      Left = 8
      Top = 133
      Width = 52
      Height = 16
      Caption = '(X4,Y4)'
    end
    object Label66: TLabel
      Left = 222
      Top = 137
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object m_dLaserUpPosX10: TEdit
      Left = 64
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'Edit1'
    end
    object m_dLaserUpPosY10: TEdit
      Left = 144
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
    object m_dLaserUpPosX11: TEdit
      Left = 64
      Top = 64
      Width = 73
      Height = 24
      TabOrder = 2
      Text = 'm_dLaserUpPosX11'
    end
    object m_dLaserUpPosY11: TEdit
      Left = 144
      Top = 64
      Width = 73
      Height = 24
      TabOrder = 3
      Text = 'Edit1'
    end
    object m_dLaserUpPosX12: TEdit
      Left = 64
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 4
      Text = 'Edit1'
    end
    object m_dLaserUpPosY12: TEdit
      Left = 144
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 5
      Text = 'Edit1'
    end
    object m_dLaserUpPosX13: TEdit
      Left = 64
      Top = 128
      Width = 73
      Height = 24
      TabOrder = 6
      Text = 'Edit1'
    end
    object m_dLaserUpPosY13: TEdit
      Left = 144
      Top = 128
      Width = 73
      Height = 24
      TabOrder = 7
      Text = 'Edit1'
    end
  end
  object GroupBox11: TGroupBox
    Left = 816
    Top = 272
    Width = 273
    Height = 81
    Caption = #21069#22739#21512'['#19979#27169#28204#39640']'#36215#22987#20301#32622
    TabOrder = 11
    object Label54: TLabel
      Left = 222
      Top = 41
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label32: TLabel
      Left = 24
      Top = 37
      Width = 36
      Height = 16
      Caption = '(X,Y)'
    end
    object m_dLaserDownPosX1: TEdit
      Left = 64
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'Edit1'
    end
    object m_dLaserDownPosY1: TEdit
      Left = 144
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
  end
  object m_nRailOption: TRadioGroup
    Left = 280
    Top = 384
    Width = 241
    Height = 153
    Caption = #27969#36947#36984#25799
    Items.Strings = (
      #20840#37096#37117#38283#21855
      #21069#27969#36947
      #24460#27969#36947)
    TabOrder = 12
  end
  object GroupBox12: TGroupBox
    Left = 536
    Top = 368
    Width = 177
    Height = 169
    Caption = #22739#21147#26657#27491#35373#23450
    TabOrder = 14
    object Label55: TLabel
      Left = 8
      Top = 40
      Width = 64
      Height = 16
      AutoSize = False
      Caption = #23481#35377#35492#24046
    end
    object Label56: TLabel
      Left = 150
      Top = 36
      Width = 13
      Height = 16
      Caption = '%'
    end
    object Label57: TLabel
      Left = 8
      Top = 72
      Width = 96
      Height = 16
      AutoSize = False
      Caption = #28204#37327#31337#23450#26178#38291
    end
    object Label58: TLabel
      Left = 94
      Top = 100
      Width = 16
      Height = 16
      Caption = #31186
    end
    object m_dPressCalRange: TEdit
      Left = 80
      Top = 32
      Width = 65
      Height = 24
      TabOrder = 0
      Text = 'm_dLamHeight0'
    end
    object m_dPressCalTime: TEdit
      Left = 16
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'm_dLamHeight0'
    end
  end
  object GroupBox13: TGroupBox
    Left = 536
    Top = 96
    Width = 273
    Height = 169
    Caption = #24460#22739#21512'['#19978#27169#28204#39640']'#36215#22987#20301#32622
    TabOrder = 15
    object Label67: TLabel
      Left = 8
      Top = 37
      Width = 52
      Height = 16
      Caption = '(X1,Y1)'
    end
    object Label68: TLabel
      Left = 222
      Top = 41
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label69: TLabel
      Left = 8
      Top = 69
      Width = 52
      Height = 16
      Caption = '(X2,Y2)'
    end
    object Label70: TLabel
      Left = 222
      Top = 73
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label71: TLabel
      Left = 8
      Top = 101
      Width = 52
      Height = 16
      Caption = '(X3,Y3)'
    end
    object Label72: TLabel
      Left = 222
      Top = 105
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object Label73: TLabel
      Left = 8
      Top = 133
      Width = 52
      Height = 16
      Caption = '(X4,Y4)'
    end
    object Label74: TLabel
      Left = 222
      Top = 137
      Width = 24
      Height = 16
      Caption = 'mm'
    end
    object m_dLaserUpPosX01: TEdit
      Left = 64
      Top = 64
      Width = 73
      Height = 24
      TabOrder = 0
      Text = 'Edit1'
    end
    object m_dLaserUpPosY01: TEdit
      Left = 144
      Top = 64
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'Edit1'
    end
    object m_dLaserUpPosX02: TEdit
      Left = 64
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 2
      Text = 'Edit1'
    end
    object m_dLaserUpPosY02: TEdit
      Left = 144
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 3
      Text = 'Edit1'
    end
    object m_dLaserUpPosX03: TEdit
      Left = 64
      Top = 128
      Width = 73
      Height = 24
      TabOrder = 4
      Text = 'Edit1'
    end
    object m_dLaserUpPosY03: TEdit
      Left = 144
      Top = 128
      Width = 73
      Height = 24
      TabOrder = 5
      Text = 'Edit1'
    end
    object m_dLaserUpPosX00: TEdit
      Left = 64
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 6
      Text = 'Edit1'
    end
    object m_dLaserUpPosY00: TEdit
      Left = 144
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 7
      Text = 'Edit1'
    end
  end
  object GroupBox7: TGroupBox
    Left = 720
    Top = 368
    Width = 169
    Height = 169
    Caption = #33258#21205#26657#27491
    TabOrder = 16
    object Label79: TLabel
      Left = 8
      Top = 36
      Width = 64
      Height = 16
      Caption = #38291#38548#30436#25976
    end
    object Label80: TLabel
      Left = 8
      Top = 69
      Width = 96
      Height = 16
      Caption = #33258#21205#20572#27231#35492#24046
    end
    object Label81: TLabel
      Left = 88
      Top = 98
      Width = 13
      Height = 16
      Caption = '%'
    end
    object Label82: TLabel
      Left = 143
      Top = 35
      Width = 16
      Height = 16
      Caption = #30436
    end
    object m_nAutoInterval: TEdit
      Left = 80
      Top = 32
      Width = 57
      Height = 24
      TabOrder = 0
      Text = 'm_nAutoInterval'
    end
    object m_dAutoStopRange: TEdit
      Left = 8
      Top = 92
      Width = 73
      Height = 24
      TabOrder = 1
      Text = 'm_dAutoStopRange'
    end
  end
  object m_dAutoRunTempRange: TEdit
    Left = 128
    Top = 548
    Width = 49
    Height = 24
    TabOrder = 17
    Text = 'm_dAutoRunTempRange'
  end
  object m_dVacDelayTime: TEdit
    Left = 488
    Top = 548
    Width = 33
    Height = 24
    TabOrder = 18
    Text = 'm_dVacDelayTime'
  end
  object GroupBox14: TGroupBox
    Left = 896
    Top = 368
    Width = 193
    Height = 169
    Caption = #20854#20182
    TabOrder = 19
    object Label10: TLabel
      Left = 8
      Top = 20
      Width = 80
      Height = 16
      Caption = #29105#22739#38957#22411#24335
    end
    object Label18: TLabel
      Left = 8
      Top = 44
      Width = 80
      Height = 16
      Caption = #29105#22739#38957#23610#23544
    end
    object Label89: TLabel
      Left = 8
      Top = 68
      Width = 64
      Height = 16
      Caption = #30495#31354#38283#21855
    end
    object Label90: TLabel
      Left = 8
      Top = 92
      Width = 64
      Height = 16
      Caption = #22739#21147#39511#35657
    end
    object Label91: TLabel
      Left = 8
      Top = 116
      Width = 51
      Height = 16
      Caption = 'Dummy'
    end
    object m_cmbHeadType: TComboBox
      Left = 96
      Top = 16
      Width = 89
      Height = 24
      Style = csDropDownList
      ItemHeight = 16
      TabOrder = 0
      Items.Strings = (
        #23526#24515
        #31354#24515)
    end
    object m_cmbVacummOn: TComboBox
      Left = 96
      Top = 64
      Width = 89
      Height = 24
      Style = csDropDownList
      ItemHeight = 16
      TabOrder = 1
      Items.Strings = (
        'Fail'
        'True')
    end
    object m_cmbPressCheck: TComboBox
      Left = 96
      Top = 88
      Width = 89
      Height = 24
      Style = csDropDownList
      ItemHeight = 16
      TabOrder = 2
      Items.Strings = (
        'Fail'
        'True')
    end
    object m_cmbDummyCheck: TComboBox
      Left = 96
      Top = 112
      Width = 89
      Height = 24
      Style = csDropDownList
      ItemHeight = 16
      TabOrder = 3
      Items.Strings = (
        'Fail'
        'True')
    end
    object m_cmbHeadScal: TComboBox
      Left = 96
      Top = 40
      Width = 89
      Height = 24
      Style = csDropDownList
      ItemHeight = 16
      TabOrder = 4
    end
    object btnHeadScalModify: TButton
      Left = 40
      Top = 144
      Width = 121
      Height = 25
      Caption = #22739#38957#23610#23544#20462#25913
      TabOrder = 5
      OnClick = btnHeadScalModifyClick
    end
  end
  object m_strSetupEENum: TEdit
    Left = 656
    Top = 548
    Width = 129
    Height = 24
    Enabled = False
    TabOrder = 20
  end
end
