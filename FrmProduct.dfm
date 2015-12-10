object fmProduct: TfmProduct
  Left = 427
  Top = 30
  BorderStyle = bsDialog
  Caption = #29986#21697#21443#25976
  ClientHeight = 449
  ClientWidth = 1081
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
  object Label92: TLabel
    Left = 16
    Top = 418
    Width = 57
    Height = 16
    Caption = 'Setup EE'
  end
  object Label93: TLabel
    Left = 216
    Top = 418
    Width = 42
    Height = 16
    Caption = '('#32232#34399')'
  end
  object BitBtn1: TBitBtn
    Left = 848
    Top = 408
    Width = 97
    Height = 33
    TabOrder = 0
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 952
    Top = 408
    Width = 97
    Height = 33
    TabOrder = 1
    Kind = bkCancel
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1081
    Height = 401
    ActivePage = TabSheet1
    TabIndex = 0
    TabOrder = 2
    object TabSheet1: TTabSheet
      Caption = 'Common Page'
      object Label83: TLabel
        Left = 184
        Top = 330
        Width = 64
        Height = 16
        Caption = #33258#21205#27169#24335
      end
      object Label84: TLabel
        Left = 184
        Top = 346
        Width = 64
        Height = 16
        Caption = #23481#35377#28331#24230
      end
      object Label85: TLabel
        Left = 328
        Top = 338
        Width = 16
        Height = 16
        Caption = #24230
      end
      object Label86: TLabel
        Left = 256
        Top = 338
        Width = 12
        Height = 16
        Caption = #177' '
      end
      object Label88: TLabel
        Left = 576
        Top = 338
        Width = 16
        Height = 16
        Caption = #31186
      end
      object Label87: TLabel
        Left = 384
        Top = 338
        Width = 144
        Height = 16
        Caption = #31532#20108#27425#30495#31354#27298#28204#24310#36978
      end
      object GroupBox4: TGroupBox
        Left = 16
        Top = 8
        Width = 241
        Height = 137
        Caption = #24460#22739#21512#21443#25976
        TabOrder = 0
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
        TabOrder = 1
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
      object m_nRailOption: TRadioGroup
        Left = 16
        Top = 153
        Width = 145
        Height = 168
        Caption = #27969#36947#36984#25799
        Items.Strings = (
          #20840#37096#37117#38283#21855
          #21069#27969#36947
          #24460#27969#36947)
        TabOrder = 2
      end
      object GroupBox14: TGroupBox
        Left = 536
        Top = 9
        Width = 273
        Height = 312
        Caption = #20854#20182
        TabOrder = 3
        object Label10: TLabel
          Left = 8
          Top = 20
          Width = 80
          Height = 16
          Caption = #29105#22739#38957#22411#24335
        end
        object Label18: TLabel
          Left = 8
          Top = 60
          Width = 80
          Height = 16
          Caption = #29105#22739#38957#23610#23544
        end
        object Label89: TLabel
          Left = 8
          Top = 140
          Width = 64
          Height = 16
          Caption = #30495#31354#38283#21855
        end
        object Label90: TLabel
          Left = 8
          Top = 180
          Width = 64
          Height = 16
          Caption = #22739#21147#39511#35657
        end
        object Label94: TLabel
          Left = 8
          Top = 100
          Width = 64
          Height = 16
          Caption = #19979#27169#23610#23544
        end
        object Label109: TLabel
          Left = 8
          Top = 220
          Width = 64
          Height = 16
          Caption = #27169#20855#32232#34399
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
          Top = 136
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
          Top = 176
          Width = 89
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          TabOrder = 2
          Items.Strings = (
            'Fail'
            'True')
        end
        object m_cmbHeadScal: TComboBox
          Left = 96
          Top = 56
          Width = 89
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          TabOrder = 3
        end
        object btnHeadScalModify: TButton
          Left = 8
          Top = 272
          Width = 121
          Height = 25
          Hint = 'HeadScal'
          Caption = #22739#38957#23610#23544#20462#25913
          TabOrder = 4
          OnClick = btnScalModifyClick
        end
        object m_cmbModuleScal: TComboBox
          Left = 96
          Top = 96
          Width = 89
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          TabOrder = 5
        end
        object btnModuleScalModify: TButton
          Left = 136
          Top = 272
          Width = 121
          Height = 25
          Hint = 'ModuleScal'
          Caption = #19979#27169#23610#23544#20462#25913
          TabOrder = 6
          OnClick = btnScalModifyClick
        end
        object m_strModuleNum: TEdit
          Left = 96
          Top = 216
          Width = 161
          Height = 24
          TabOrder = 7
          Text = 'm_strModuleNum'
        end
      end
      object GroupBox15: TGroupBox
        Left = 824
        Top = 8
        Width = 217
        Height = 217
        Caption = 'Dummy Check'
        TabOrder = 4
        object Label91: TLabel
          Left = 16
          Top = 20
          Width = 51
          Height = 16
          Caption = 'Dummy'
        end
        object Label95: TLabel
          Left = 64
          Top = 44
          Width = 85
          Height = 16
          Caption = 'Dummy(1~5)'
        end
        object Label96: TLabel
          Left = 40
          Top = 68
          Width = 28
          Height = 16
          Caption = 'BLT'
        end
        object Label97: TLabel
          Left = 100
          Top = 68
          Width = 21
          Height = 16
          Caption = 'Tilt'
        end
        object Label98: TLabel
          Left = 152
          Top = 68
          Width = 26
          Height = 16
          Caption = 'Gap'
        end
        object Label99: TLabel
          Left = 16
          Top = 92
          Width = 8
          Height = 16
          Caption = '1'
        end
        object Label100: TLabel
          Left = 16
          Top = 116
          Width = 8
          Height = 16
          Caption = '2'
        end
        object Label101: TLabel
          Left = 16
          Top = 140
          Width = 8
          Height = 16
          Caption = '3'
        end
        object Label102: TLabel
          Left = 16
          Top = 164
          Width = 8
          Height = 16
          Caption = '4'
        end
        object Label103: TLabel
          Left = 16
          Top = 188
          Width = 8
          Height = 16
          Caption = '5'
        end
        object m_cmbDummyCheck: TComboBox
          Left = 104
          Top = 16
          Width = 89
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          TabOrder = 0
          Items.Strings = (
            'Fail'
            'True')
        end
        object m_dBLT1: TEdit
          Left = 32
          Top = 88
          Width = 49
          Height = 24
          TabOrder = 1
        end
        object m_dBLT2: TEdit
          Left = 32
          Top = 112
          Width = 49
          Height = 24
          TabOrder = 2
        end
        object m_dBLT3: TEdit
          Left = 32
          Top = 136
          Width = 49
          Height = 24
          TabOrder = 3
        end
        object m_dBLT4: TEdit
          Left = 32
          Top = 160
          Width = 49
          Height = 24
          TabOrder = 4
        end
        object m_dBLT5: TEdit
          Left = 32
          Top = 184
          Width = 49
          Height = 24
          TabOrder = 5
        end
        object m_dTilt1: TEdit
          Left = 88
          Top = 88
          Width = 49
          Height = 24
          TabOrder = 6
        end
        object m_dTilt2: TEdit
          Left = 88
          Top = 112
          Width = 49
          Height = 24
          TabOrder = 7
        end
        object m_dTilt3: TEdit
          Left = 88
          Top = 136
          Width = 49
          Height = 24
          TabOrder = 8
        end
        object m_dTilt4: TEdit
          Left = 88
          Top = 160
          Width = 49
          Height = 24
          TabOrder = 9
        end
        object m_dTilt5: TEdit
          Left = 88
          Top = 184
          Width = 49
          Height = 24
          TabOrder = 10
        end
        object m_dGap1: TEdit
          Left = 144
          Top = 88
          Width = 49
          Height = 24
          TabOrder = 11
        end
        object m_dGap2: TEdit
          Left = 144
          Top = 112
          Width = 49
          Height = 24
          TabOrder = 12
        end
        object m_dGap3: TEdit
          Left = 144
          Top = 136
          Width = 49
          Height = 24
          TabOrder = 13
        end
        object m_dGap4: TEdit
          Left = 144
          Top = 160
          Width = 49
          Height = 24
          TabOrder = 14
        end
        object m_dGap5: TEdit
          Left = 144
          Top = 184
          Width = 49
          Height = 24
          TabOrder = 15
        end
      end
      object GroupBox16: TGroupBox
        Left = 824
        Top = 232
        Width = 217
        Height = 129
        Caption = 'Tempture Check ('#8451')'
        TabOrder = 5
        object Label104: TLabel
          Left = 16
          Top = 84
          Width = 12
          Height = 16
          Caption = 'F '
        end
        object Label105: TLabel
          Left = 16
          Top = 52
          Width = 10
          Height = 16
          Caption = 'R'
        end
        object Label106: TLabel
          Left = 48
          Top = 28
          Width = 8
          Height = 16
          Caption = '1'
        end
        object Label107: TLabel
          Left = 104
          Top = 28
          Width = 8
          Height = 16
          Caption = '2'
        end
        object Label108: TLabel
          Left = 160
          Top = 28
          Width = 8
          Height = 16
          Caption = '3'
        end
        object m_dKeyTemp11: TEdit
          Left = 32
          Top = 81
          Width = 49
          Height = 24
          TabOrder = 0
        end
        object m_dKeyTemp12: TEdit
          Left = 88
          Top = 81
          Width = 49
          Height = 24
          TabOrder = 1
        end
        object m_dKeyTemp13: TEdit
          Left = 144
          Top = 81
          Width = 49
          Height = 24
          TabOrder = 2
        end
        object m_dKeyTemp01: TEdit
          Left = 32
          Top = 49
          Width = 49
          Height = 24
          TabOrder = 3
        end
        object m_dKeyTemp02: TEdit
          Left = 88
          Top = 49
          Width = 49
          Height = 24
          TabOrder = 4
        end
        object m_dKeyTemp03: TEdit
          Left = 144
          Top = 49
          Width = 49
          Height = 24
          TabOrder = 5
        end
      end
      object m_bNotLam: TCheckBox
        Left = 24
        Top = 337
        Width = 73
        Height = 17
        Caption = #19981#22739#21512
        TabOrder = 6
      end
      object m_dAutoRunTempRange: TEdit
        Left = 272
        Top = 333
        Width = 49
        Height = 24
        TabOrder = 7
        Text = 'm_dAutoRunTempRange'
      end
      object m_dVacDelayTime: TEdit
        Left = 536
        Top = 335
        Width = 33
        Height = 24
        TabOrder = 8
        Text = 'm_dVacDelayTime'
      end
      object GroupBox12: TGroupBox
        Left = 168
        Top = 153
        Width = 177
        Height = 169
        Caption = #22739#21147#26657#27491#35373#23450
        TabOrder = 9
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
      object GroupBox7: TGroupBox
        Left = 352
        Top = 153
        Width = 169
        Height = 169
        Caption = #33258#21205#26657#27491
        TabOrder = 10
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
    end
    object TabSheet2: TTabSheet
      Caption = 'Detail Page'
      ImageIndex = 1
      object Label110: TLabel
        Left = 230
        Top = 338
        Width = 260
        Height = 16
        Caption = '% ('#38477#20302#27773#32568#32568#21185#35492#24046' '#35531#36664#20837'0~100%)'
      end
      object Label111: TLabel
        Left = 24
        Top = 338
        Width = 144
        Height = 16
        Caption = #22739#21512#21069#32233#34909#22739#21147#19979#38477
      end
      object GroupBox3: TGroupBox
        Left = 8
        Top = 16
        Width = 241
        Height = 225
        Caption = #24460#22739#21512#27969#36947
        TabOrder = 0
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
        Left = 256
        Top = 16
        Width = 241
        Height = 225
        Caption = #21069#22739#21512#27969#36947
        TabOrder = 1
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
      object GroupBox1: TGroupBox
        Left = 8
        Top = 249
        Width = 489
        Height = 80
        Caption = 'Tray Layout'
        TabOrder = 2
        object Label1: TLabel
          Left = 16
          Top = 27
          Width = 32
          Height = 16
          Caption = #34892#25976
        end
        object Label2: TLabel
          Left = 16
          Top = 52
          Width = 32
          Height = 16
          Caption = #21015#25976
        end
        object Label11: TLabel
          Left = 176
          Top = 26
          Width = 48
          Height = 16
          Caption = #34892#38291#36317
        end
        object Label12: TLabel
          Left = 176
          Top = 50
          Width = 48
          Height = 16
          Caption = #21015#38291#36317
        end
        object Label13: TLabel
          Left = 310
          Top = 26
          Width = 24
          Height = 16
          Caption = 'mm'
        end
        object Label14: TLabel
          Left = 310
          Top = 50
          Width = 24
          Height = 16
          Caption = 'mm'
        end
        object m_nCols: TEdit
          Left = 72
          Top = 22
          Width = 73
          Height = 24
          TabOrder = 0
          Text = 'm_nCols'
        end
        object m_nRows: TEdit
          Left = 72
          Top = 46
          Width = 73
          Height = 24
          TabOrder = 1
          Text = 'Edit1'
        end
        object m_dColPitch: TEdit
          Left = 232
          Top = 22
          Width = 73
          Height = 24
          TabOrder = 2
          Text = 'Edit1'
        end
        object m_dRowPitch: TEdit
          Left = 232
          Top = 46
          Width = 73
          Height = 24
          TabOrder = 3
          Text = 'Edit1'
        end
      end
      object GroupBox6: TGroupBox
        Left = 504
        Top = 16
        Width = 273
        Height = 73
        Caption = #24460#22739#21512'['#28204#37325']'#36215#22987#20301#32622
        TabOrder = 3
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
        Left = 504
        Top = 272
        Width = 273
        Height = 89
        Caption = #24460#22739#21512'['#19979#27169#28204#39640']'#36215#22987#20301#32622
        TabOrder = 4
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
        Left = 784
        Top = 16
        Width = 273
        Height = 73
        Caption = #21069#22739#21512'['#28204#37325']'#36215#22987#20301#32622
        TabOrder = 5
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
        Left = 784
        Top = 96
        Width = 273
        Height = 169
        Caption = #21069#22739#21512'['#19978#27169#28204#39640']'#36215#22987#20301#32622
        TabOrder = 6
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
        Left = 784
        Top = 272
        Width = 273
        Height = 89
        Caption = #21069#22739#21512'['#19979#27169#28204#39640']'#36215#22987#20301#32622
        TabOrder = 7
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
      object GroupBox13: TGroupBox
        Left = 504
        Top = 96
        Width = 273
        Height = 169
        Caption = #24460#22739#21512'['#19978#27169#28204#39640']'#36215#22987#20301#32622
        TabOrder = 8
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
      object m_nDownPercent: TEdit
        Left = 176
        Top = 335
        Width = 49
        Height = 24
        TabOrder = 9
      end
    end
  end
  object m_strSetupEENum: TEdit
    Left = 80
    Top = 413
    Width = 129
    Height = 24
    Enabled = False
    TabOrder = 3
  end
end
