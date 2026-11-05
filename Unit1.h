//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.CheckLst.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:
	TADOConnection *ADOConnection1;
	TDataSource *DataSource1;
	TADOTable *ADOTable1;
	TTimer *Timer1;
	TADOQuery *ADOQuery1;
	TDBGrid *DBGrid1;
	TButton *Button2;
	TDBNavigator *DBNavigator1;
	TADOConnection *ADOConnection2;
	TDataSource *DataSource2;
	TADOTable *ADOTable2;
	TADOQuery *ADOQuery2;
	TDBGrid *DBGrid2;
	TButton *Button8;
	TEdit *Edit1;
	TButton *Button10;
	TButton *Button11;
	TButton *Button12;
	TButton *Button13;
	TLabeledEdit *LabeledEdit4;
	TLabeledEdit *LabeledEdit5;
	TADODataSet *ADODataSet1;
	TEdit *Edit2;
	TButton *Button17;
	TDBNavigator *DBNavigator2;
	TLabeledEdit *LabeledEdit6;
	TEdit *Edit3;
	TButton *Button19;
	TButton *Button20;
	TButton *Button21;
	TLabeledEdit *LabeledEdit7;
	TToggleSwitch *ToggleSwitch1;
	TMainMenu *MainMenu1;
	TMenuItem *N3;
	TMenuItem *N2;
	TMenuItem *N4;
	TLabel *Label4;
	TPageControl *PageControl1;
	TMenuItem *N5;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TAutoIncField *ADOQuery1Код;
	TWideStringField *ADOQuery1username;
	TWideStringField *ADOQuery1password;
	TAutoIncField *ADOQuery2Код;
	TWideStringField *ADOQuery2usename;
	TWideStringField *ADOQuery2task_name;
	TWideStringField *ADOQuery2status;
	TWideStringField *ADOQuery2task_desc;
	TWideStringField *ADOQuery2task_category;
	TWideStringField *ADOQuery2task_date;
	TWideStringField *ADOQuery2task_deadline;
	TComboBox *ComboBox1;
	TADOQuery *ADOQuery3;
	TComboBox *ComboBox2;
	TButton *Button22;
	TADOQuery *ADOQuery4;
	TMenuItem *N6;
	TCheckListBox *CheckListBox1;
	TDateTimePicker *DateTimePicker1;
	TRadioGroup *RadioGroup1;
	TRadioGroup *RadioGroup2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TADOConnection *ADOConnection3;
	TDataSource *DataSource3;
	TADOQuery *ADOQuery5;
	TDBGrid *DBGrid3;
	TButton *Button23;
	TButton *Button24;
	TEdit *Edit4;
	TEdit *Edit5;
	TDBNavigator *DBNavigator3;
	TComboBox *ComboBox3;
	TADOQuery *ADOQuery6;
	TAutoIncField *ADOQuery6Код;
	TWideStringField *ADOQuery6usename;
	TWideStringField *ADOQuery6task_name;
	TWideStringField *ADOQuery6status;
	TWideStringField *ADOQuery6task_desc;
	TWideStringField *ADOQuery6task_category;
	TWideStringField *ADOQuery6task_date;
	TWideStringField *ADOQuery6task_deadline;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TButton *Button25;
	TButton *Button26;
	TADOQuery *ADOQuery7;
	TADOConnection *ADOConnection4;
	TDataSource *DataSource4;
	TButton *Button27;
	TComboBox *ComboBox4;
	TButton *Button28;
	TADOQuery *ADOQuery8;
	TADOQuery *ADOQuery9;
	TAutoIncField *ADOQuery9Код;
	TWideStringField *ADOQuery9group_key;
	TWideStringField *ADOQuery9task_name;
	TWideStringField *ADOQuery9assigned_to;
	TWideStringField *ADOQuery9status;
	TWideStringField *ADOQuery9task_category;
	TWideStringField *ADOQuery9task_desc;
	TWideStringField *ADOQuery9task_date;
	TWideStringField *ADOQuery9task_deadline;
	TWideStringField *ADOQuery9created_by;
	TWideStringField *ADOQuery9created_date;
	TButton *Button29;
	TButton *Button30;
	TButton *Button31;
	TDBGrid *DBGrid4;
	TADOConnection *ADOConnection5;
	TDataSource *DataSource5;
	TADOQuery *ADOQuery10;
	TButton *Button32;
	TButton *Button33;
	TButton *Button34;
	TMenuItem *N1;
	TLabel *Label6;
	TLabel *Label7;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TRichEdit *RichEdit1;
	TRichEdit *RichEdit2;
	TRichEdit *RichEdit3;
	TRichEdit *RichEdit4;
	TSearchBox *SearchBox1;
	TRichEdit *RichEdit5;
	TRichEdit *RichEdit6;
	TADOTable *ADOTable3;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall DBGrid2CellClick(TColumn *Column);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall DBGrid2DblClick(TObject *Sender);
	void __fastcall Button17Click(TObject *Sender);
	void __fastcall Button19Click(TObject *Sender);
	void __fastcall Button20Click(TObject *Sender);
	void __fastcall Button21Click(TObject *Sender);
	void __fastcall Edit3Change(TObject *Sender);
	void __fastcall ToggleSwitch1Click(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall ComboBox2Change(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);

	void __fastcall UpdateCategoriesComboBox();
	void __fastcall UpdateStatusesComboBox();
	void __fastcall UpdateTodayTasksChecklist();
	void __fastcall N6Click(TObject *Sender);
	void __fastcall RadioGroup1Click(TObject *Sender);
	void __fastcall RadioGroup2Click(TObject *Sender);
	void __fastcall CheckListBox1Click(TObject *Sender);
	void __fastcall RadioButton1Click(TObject *Sender);
	void __fastcall RadioButton2Click(TObject *Sender);
	void __fastcall CheckListBox1ClickCheck(TObject *Sender);
	void __fastcall DateTimePicker1Change(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall Button24Click(TObject *Sender);
	void __fastcall Button27Click(TObject *Sender);
	void __fastcall Button25Click(TObject *Sender);
	void __fastcall ComboBox3Change(TObject *Sender);
	void __fastcall Button26Click(TObject *Sender);
	void __fastcall ComboBox4Change(TObject *Sender);
	void __fastcall Button29Click(TObject *Sender);
	void __fastcall Button30Click(TObject *Sender);
	void __fastcall Button31Click(TObject *Sender);
	void __fastcall Button32Click(TObject *Sender);
	void __fastcall Button33Click(TObject *Sender);
	void __fastcall Button34Click(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall DBGrid2DrawColumnCell(TObject *Sender, const TRect &Rect, int DataCol,
          TColumn *Column, TGridDrawState State);
	void __fastcall Button28Click(TObject *Sender);
	void __fastcall SearchBox1Change(TObject *Sender);

private:

	void ApplyTheme();
public:
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
