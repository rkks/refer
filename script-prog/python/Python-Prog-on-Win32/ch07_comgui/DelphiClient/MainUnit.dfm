�
 TFRMMAIN 0[  TPF0TfrmMainfrmMainLeft� ToplWidthGHeight�Caption,Doubletalk Browser - Cut-Down Delphi EditionFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style Menu	MainMenu1OnCreate
FormCreatePixelsPerInch`
TextHeight TPageControlPageControl1Left Top Width?Height
ActivePage
tabJournalAlignalClientTabOrder  	TTabSheet
tabJournalCaption&Journal TListBox
lstJournalLeft Top WidthHeightcAlignalLeft
ItemHeightStylelbOwnerDrawFixedTabOrder 
OnDrawItemlstJournalDrawItem   	TTabSheet
tabAccountCaption&Account List TPanelpnlAccountLeftLeft Top Width� Heighta
BevelOuterbvNoneTabOrder  TLabelLabel1Left Top Width� HeightAlignalTopCaptionList of Accounts:  TListBoxlstAllAccountsLeft TopWidth� HeightTAlignalClient
ItemHeightTabOrder OnClicklstAllAccountsClick    	TTabSheet	TabSheet1CaptionAccount &Details TLabellblAccountDetailsCaptionLeft Top Width7HeightAlignalTopCaption"Details of Account (none selected)Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.StylefsBold 
ParentFont  TStringGridgrdAccountDetailsLeft TopWidth7HeightSAlignalClientDefaultColWidth`DefaultRowHeight	FixedCols RowCountTabOrder 	ColWidths/O� ``    	TTabSheettabGraphicsCaption&Callback Graphics TLabelLabel2LeftTopWidth�HeightCaptionYCallback Graphics are much harder in Delphi.  You need to create a COM object explicitly.    
TStatusBar
StatusBar1Left TopWidth?HeightPanels SimplePanel	  	TMainMenu	MainMenu1Left� 	TMenuItemFile1Caption&File 	TMenuItemOpen1Caption&OpenOnClick
Open1Click  	TMenuItemN1Caption-  	TMenuItemExit1CaptionE&xit  	TMenuItemN2Caption-  	TMenuItemN1DemoData1DTJ1Caption&1. DemoData1.DTJOnClickN1DemoData1DTJ1Click  	TMenuItemN2DemoData2DTJ1Caption&2. DemoData2.DTJOnClickN2DemoData2DTJ1Click    TOpenDialogOpenDialog1Filter&Doubletalk Journal files (*.DTJ)|*.DTJFilterIndex Left�   