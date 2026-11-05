//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Data.DB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Data.Win.ADODB.hpp>
//---------------------------------------------------------------------------
class TForm6 : public TForm
{
__published:
	TDateTimePicker *DateTimePicker1;
	TDBGrid *DBGrid1;
	TADOConnection *ADOConnection1;
	TDataSource *DataSource1;
	TADOQuery *ADOQuery1;
	TButton *Button1;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TLabel *Label1;
	TEdit *Edit1;
	TAutoIncField *ADOQuery1Код;
	TWideStringField *ADOQuery1usename;
	TWideStringField *ADOQuery1task_name;
	TWideStringField *ADOQuery1status;
	TWideStringField *ADOQuery1task_desc;
	TWideStringField *ADOQuery1task_category;
	TWideStringField *ADOQuery1task_date;
	TWideStringField *ADOQuery1task_deadline;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TButton *Button11;
	TADOQuery *ADOQuery2;
	TAutoIncField *ADOQuery2Код;
	TWideStringField *ADOQuery2usename;
	TWideStringField *ADOQuery2task_name;
	TWideStringField *ADOQuery2status;
	TWideStringField *ADOQuery2task_desc;
	TWideStringField *ADOQuery2task_category;
	TWideStringField *ADOQuery2task_date;
	TWideStringField *ADOQuery2task_deadline;
	TWideStringField *ADOQuery2archive_date;
	void __fastcall DateTimePicker1Change(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);

private:
public:
	__fastcall TForm6(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm6 *Form6;
//---------------------------------------------------------------------------
#endif
