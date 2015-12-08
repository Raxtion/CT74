object frmMotorCheck: TfrmMotorCheck
  Left = 602
  Top = 127
  BorderStyle = bsDialog
  Caption = #39340#36948#28204#35430
  ClientHeight = 509
  ClientWidth = 518
  Color = clSkyBlue
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Arial Black'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 23
  object labelPosition: TLabel
    Left = 0
    Top = 0
    Width = 518
    Height = 42
    Align = alTop
    AutoSize = False
    Caption = 'labelPosition'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Panel2: TPanel
    Left = 0
    Top = 161
    Width = 161
    Height = 275
    Align = alLeft
    Color = 13430215
    TabOrder = 0
    object btnJog: TSpeedButton
      Left = 16
      Top = 16
      Width = 129
      Height = 49
      GroupIndex = 7
      Down = True
      Caption = 'JOG'
      OnClick = btnJogClick
    end
    object btnPitch1: TSpeedButton
      Tag = 1
      Left = 16
      Top = 73
      Width = 129
      Height = 51
      GroupIndex = 7
      Caption = '1 mm'
      OnClick = btnJogClick
    end
    object btnPitch01: TSpeedButton
      Tag = 2
      Left = 16
      Top = 134
      Width = 129
      Height = 49
      GroupIndex = 7
      Caption = '0.1 mm'
      OnClick = btnJogClick
    end
    object btnPitch001: TSpeedButton
      Tag = 3
      Left = 16
      Top = 193
      Width = 129
      Height = 49
      GroupIndex = 7
      Caption = '0.01 mm'
      OnClick = btnJogClick
    end
  end
  object Panel1: TPanel
    Left = 357
    Top = 161
    Width = 161
    Height = 275
    Align = alRight
    Color = 13430215
    TabOrder = 1
    object btnFastSpeed: TSpeedButton
      Left = 16
      Top = 16
      Width = 129
      Height = 49
      GroupIndex = 8
      Caption = #24555' '#36895
      OnClick = btnFastSpeedClick
    end
    object btnMidSpeed: TSpeedButton
      Tag = 1
      Left = 16
      Top = 73
      Width = 129
      Height = 51
      GroupIndex = 8
      Down = True
      Caption = #20013' '#36895
      OnClick = btnFastSpeedClick
    end
    object btnSlowSpeed: TSpeedButton
      Tag = 2
      Left = 16
      Top = 134
      Width = 129
      Height = 49
      GroupIndex = 8
      Caption = #20302' '#36895
      OnClick = btnFastSpeedClick
    end
    object bntVerySlowSpeed: TSpeedButton
      Tag = 3
      Left = 16
      Top = 193
      Width = 129
      Height = 49
      GroupIndex = 8
      Caption = #24494' '#36895
      OnClick = btnFastSpeedClick
    end
  end
  object Panel3: TPanel
    Left = 161
    Top = 161
    Width = 196
    Height = 275
    Align = alClient
    Color = 13430215
    TabOrder = 2
    object Label1: TLabel
      Left = 56
      Top = 16
      Width = 81
      Height = 29
      AutoSize = False
      Caption = #21069#26997#38480
    end
    object shapePL: TShape
      Left = 16
      Top = 16
      Width = 33
      Height = 29
      Hint = #21407#40670
      Brush.Color = clGreen
      ParentShowHint = False
      Shape = stCircle
      ShowHint = True
    end
    object shapeML: TShape
      Left = 16
      Top = 56
      Width = 33
      Height = 27
      Hint = #21407#40670
      Brush.Color = clGreen
      ParentShowHint = False
      Shape = stCircle
      ShowHint = True
    end
    object Label3: TLabel
      Left = 56
      Top = 56
      Width = 89
      Height = 27
      AutoSize = False
      Caption = #24460#26997#38480
    end
    object Label2: TLabel
      Left = 56
      Top = 96
      Width = 89
      Height = 28
      AutoSize = False
      Caption = #21407#40670
    end
    object shapeOrg: TShape
      Left = 16
      Top = 96
      Width = 33
      Height = 28
      Hint = #21407#40670
      Brush.Color = clGreen
      ParentShowHint = False
      Shape = stCircle
      ShowHint = True
    end
    object shapeInp: TShape
      Left = 16
      Top = 137
      Width = 33
      Height = 27
      Hint = #21407#40670
      Brush.Color = clGreen
      ParentShowHint = False
      Shape = stCircle
      ShowHint = True
    end
    object Label4: TLabel
      Left = 56
      Top = 137
      Width = 73
      Height = 27
      AutoSize = False
      Caption = 'INP'
    end
    object shapeAlarm: TShape
      Left = 16
      Top = 175
      Width = 33
      Height = 29
      Hint = #21407#40670
      Brush.Color = clMaroon
      ParentShowHint = False
      Shape = stCircle
      ShowHint = True
    end
    object Label5: TLabel
      Left = 56
      Top = 175
      Width = 73
      Height = 29
      AutoSize = False
      Caption = 'Alarm'
    end
    object btnHome: TSpeedButton
      Left = 118
      Top = 88
      Width = 75
      Height = 41
      Caption = 'Home'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'Arial Black'
      Font.Style = []
      ParentFont = False
      OnClick = btnHomeClick
    end
    object btnServoOnOff: TSpeedButton
      Left = 40
      Top = 224
      Width = 130
      Height = 41
      AllowAllUp = True
      GroupIndex = 38
      Down = True
      Caption = 'Servo On'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'Arial Black'
      Font.Style = []
      ParentFont = False
      OnClick = btnServoOnOffClick
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 436
    Width = 518
    Height = 73
    Align = alBottom
    Color = 13430215
    TabOrder = 3
    object BitBtn1: TBitBtn
      Left = 112
      Top = 16
      Width = 96
      Height = 42
      Caption = #30906'  '#23450
      ModalResult = 1
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      Left = 296
      Top = 16
      Width = 96
      Height = 42
      Caption = #21462'  '#28040
      ModalResult = 2
      TabOrder = 1
    end
  end
  object Panel5: TPanel
    Left = 0
    Top = 42
    Width = 518
    Height = 119
    Align = alTop
    Color = 13430215
    TabOrder = 4
    object btnFWD: TSpeedButton
      Left = 101
      Top = 23
      Width = 80
      Height = 73
      Caption = '+'
      Glyph.Data = {
        360C0000424D360C000000000000360000002800000020000000200000000100
        180000000000000C000000000000000000000000000000000000C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0404040404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0404040404040656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C04040404040406565658A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040
        404040406565658A8A8AAFAFAF8A8A8A656565404040404040C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040
        406565658A8A8AAFAFAFAFAFAFAFAFAF8A8A8A656565404040404040C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565
        658A8A8AAFAFAFAFAFAFFF0000AFAFAFAFAFAF8A8A8A656565404040404040C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A
        8AAFAFAFAFAFAFFF0000FF0000FF0000AFAFAFAFAFAF8A8A8A65656540404040
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAF
        AFAFAFAFFF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAF8A8A8A65656540
        4040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAF8A8A8A65
        6565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAFAFFF00
        00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAF8A
        8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAFAFFF0000FF00
        00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAF
        AFAF8A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAFAFAFAFAFAFAFAFAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAFAFAFAFAF
        AFAFAFAFAF8A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C04040404040406565658A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A8A8A8A8A
        8A8A8A8A8A8A8A8A8A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0
        4040404040406565656565656565656565656565656565656565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656565
        6565656565656565656565656565656565404040404040C0C0C0C0C0C0C0C0C0
        4040404040404040404040404040404040404040404040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040404040404040404040404040404040404040404040C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8A8A8A
        8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565656565
        6565656565656565656565656565656565656565656565656565656540404040
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040404040
        40404040404040404040404040404040404040404040404040404040404040C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0}
      OnMouseDown = btnFWDMouseDown
      OnMouseUp = btnFWDMouseUp
    end
    object btnStop: TSpeedButton
      Left = 208
      Top = 23
      Width = 81
      Height = 73
      Caption = 'STOP'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'Arial Black'
      Font.Style = []
      ParentFont = False
      OnClick = btnStopClick
    end
    object btnRWD: TSpeedButton
      Left = 312
      Top = 23
      Width = 80
      Height = 73
      Caption = '--'
      Glyph.Data = {
        360C0000424D360C000000000000360000002800000020000000200000000100
        180000000000000C000000000000000000000000000000000000C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040404040
        40404040404040404040404040404040404040404040404040404040404040C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565656565
        6565656565656565656565656565656565656565656565656565656540404040
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8A8A8A
        8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        4040404040404040404040404040404040404040404040406565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656540
        4040404040404040404040404040404040404040404040C0C0C0C0C0C0C0C0C0
        4040404040406565656565656565656565656565656565656565658A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A65656565
        6565656565656565656565656565656565404040404040C0C0C0C0C0C0C0C0C0
        C0C0C04040404040406565658A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8AAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAF8A8A8A8A8A8A8A
        8A8A8A8A8A8A8A8A8A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAFAFAFAFAFAFAFAFAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAFAFAFAFAF
        AFAFAFAFAF8A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAFAFFF0000FF00
        00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAF
        AFAF8A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAFAFFF00
        00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAF8A
        8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAFAFAFAF
        AFFF0000FF0000FF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAF8A8A8A65
        6565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A8AAFAF
        AFAFAFAFFF0000FF0000FF0000FF0000FF0000AFAFAFAFAFAF8A8A8A65656540
        4040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565658A8A
        8AAFAFAFAFAFAFFF0000FF0000FF0000AFAFAFAFAFAF8A8A8A65656540404040
        4040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040406565
        658A8A8AAFAFAFAFAFAFFF0000AFAFAFAFAFAF8A8A8A656565404040404040C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040404040
        406565658A8A8AAFAFAFAFAFAFAFAFAF8A8A8A656565404040404040C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04040
        404040406565658A8A8AAFAFAF8A8A8A656565404040404040C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C04040404040406565658A8A8A656565404040404040C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0404040404040656565404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0404040404040404040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0}
      OnMouseDown = btnRWDMouseDown
      OnMouseUp = btnRWDMouseUp
    end
  end
  object Timer1: TTimer
    Interval = 80
    OnTimer = Timer1Timer
    Left = 456
    Top = 56
  end
end
