VERSION 5.00
Object = "{194B359F-594E-4EE4-A804-84723D1CF1C4}#1.0#0"; "ATLDevice.dll"
Begin VB.Form Form1 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "Form1"
   ClientHeight    =   1140
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3045
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1140
   ScaleWidth      =   3045
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows の既定値
   Begin ATLDEVICELibCtl.DeviceControl DeviceControl1 
      Height          =   375
      Left            =   2400
      OleObjectBlob   =   "Form1.frx":0000
      TabIndex        =   1
      Top             =   0
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Label Label1 
      Alignment       =   2  '中央揃え
      Caption         =   "Label1"
      Height          =   255
      Index           =   4
      Left            =   1920
      TabIndex        =   5
      Top             =   360
      Width           =   975
   End
   Begin VB.Label Label1 
      Alignment       =   2  '中央揃え
      Caption         =   "Label1"
      Height          =   255
      Index           =   3
      Left            =   1560
      TabIndex        =   4
      Top             =   720
      Width           =   975
   End
   Begin VB.Label Label1 
      Alignment       =   2  '中央揃え
      Caption         =   "Label1"
      Height          =   255
      Index           =   2
      Left            =   480
      TabIndex        =   3
      Top             =   720
      Width           =   975
   End
   Begin VB.Label Label1 
      Alignment       =   2  '中央揃え
      Caption         =   "Label1"
      Height          =   255
      Index           =   1
      Left            =   0
      TabIndex        =   2
      Top             =   360
      Width           =   975
   End
   Begin VB.Label Label1 
      Alignment       =   2  '中央揃え
      Caption         =   "Label1"
      Height          =   255
      Index           =   0
      Left            =   960
      TabIndex        =   0
      Top             =   0
      Width           =   975
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub SetLabelString(index As Integer, letter As String, color As ColorConstants)
    Label1(index) = letter
    Label1(index).ForeColor = color
End Sub

Private Sub DeviceControl1_OnKernelExit()
    Dim i As Integer
    
    For i = 0 To 4
        SetLabelString i, "終了", vbBlue
    Next i
    
End Sub

Private Sub DeviceControl1_OnKernelStart()
    Dim i As Integer
    
    For i = 0 To 4
        SetLabelString i, "取得待ち", vbBlack
    Next i
End Sub

Private Sub DeviceControl1_OnWrite(ByVal address As Long, ByVal sz As Long)
    Dim i As Integer
    Dim work As Long
    work = DeviceControl1.Value
    i = work And &HF
        
    Select Case (work And &HF0)
    Case &H10
        SetLabelString i, "取得待ち", vbBlack
    Case &H20
        SetLabelString i, "食事", vbRed
    Case &H40
        SetLabelString i, "休憩", vbBlack
    End Select

End Sub

Private Sub Form_Load()
    Dim i As Integer
    
    For i = 0 To 4
        SetLabelString i, "終了", vbBlue
    Next i
    
    DeviceControl1.Connect
    DeviceControl1.Map 100, 4
End Sub

Private Sub Form_Unload(Cancel As Integer)
    DeviceControl1.Close
End Sub
