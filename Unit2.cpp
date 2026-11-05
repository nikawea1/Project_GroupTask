//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include "Unit7.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
   String login1 = "admin";
    String password1 = "11";

	if (Edit1->Text.Trim() == "" || Edit2->Text.Trim() == "") {
        MessageDlg("Заполните все поля данными",
				   mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

	if (Edit1->Text == login1 && Edit2->Text == password1) {
        Form1->Show();
		Form2->Hide();

		Form1->Edit2->Text = Edit1->Text;

		Form1->Edit2->Visible = false;

        Form1->ADOQuery1->SQL->Text = "SELECT * FROM users";
        Form1->ADOQuery1->Open();
        Form1->DBGrid1->Visible = true;
        Form1->DBGrid1->Enabled = true;

		Form1->PageControl1->Pages[1]->TabVisible = true;

        return;
    }

    Form1->ADOQuery1->Close();
    Form1->ADOQuery1->SQL->Text =
		"SELECT * FROM users WHERE username= " + QuotedStr(Edit1->Text);
    Form1->ADOQuery1->Open();

    if (Form1->ADOQuery1->RecordCount == 1) {
        if (Form1->ADOQuery1->FieldByName("password")->AsString == Edit2->Text) {
            Form1->Show();
            Form2->Hide();

			Form1->Edit2->Text = Edit1->Text;
			Form1->Edit2->Visible = false;

			Form1->ADOQuery2->Close();
			Form1->ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Form1->Edit2->Text + "'";
			Form1->ADOQuery2->Open();
            Form1->DBGrid2->DataSource->DataSet = Form1->ADOQuery2;

			Form1->PageControl1->Pages[1]->TabVisible = false;

		Form1->ComboBox1->Items->Clear();
		   Form1->ComboBox1->Items->Clear();
		Form1->ComboBox1->Items->Add("Все категории");

		TADOQuery *q = new TADOQuery(Form1);
		q->Connection = Form1->ADOQuery2->Connection;
		q->SQL->Text = "SELECT DISTINCT task_category FROM tasks WHERE usename = :usename AND task_category IS NOT NULL AND task_category <> ''";
		q->Parameters->ParamByName("usename")->Value = Form1->Edit2->Text;
		q->Open();

		while (!q->Eof)
		{
			String cat = q->FieldByName("task_category")->AsString;
			if (cat != "" && Form1->ComboBox1->Items->IndexOf(cat) == -1)
				Form1->ComboBox1->Items->Add(cat);
			q->Next();
		}

		q->Close();
		delete q;

		Form1->ComboBox1->ItemIndex = 0;

		Form1->ADOQuery2->Close();
		Form1->ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Form1->Edit2->Text + "'";
		Form1->ADOQuery2->Open();
		Form1->DBGrid2->DataSource->DataSet = Form1->ADOQuery2;

			Form1->ComboBox2->Items->Clear();
			Form1->ComboBox2->Items->Add("Все статусы");

			TADOQuery *q2 = new TADOQuery(Form1);
			q2->Connection = Form1->ADOQuery2->Connection;
			q2->SQL->Text =
				"SELECT DISTINCT status FROM tasks "
				"WHERE usename = :usename "
				"AND status IS NOT NULL AND status <> ''";
			q2->Parameters->ParamByName("usename")->Value = Form1->Edit2->Text;
			q2->Open();

			while (!q2->Eof)
			{
				String s = q2->FieldByName("status")->AsString;
				if (s != "" && Form1->ComboBox2->Items->IndexOf(s) == -1)
					Form1->ComboBox2->Items->Add(s);
				q2->Next();
			}

			q2->Close();
			delete q2;

			Form1->ComboBox2->ItemIndex = 0;

		}
		else {
			MessageDlg("Неверный пароль",
					   mtError, TMsgDlgButtons() << mbOK, 0);
		}
	}
	else {
		MessageDlg("Неверный логин или пароль",
				   mtError, TMsgDlgButtons() << mbOK, 0);
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button2Click(TObject *Sender)
{
	Form3->Show();
	Form2->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form1->Close();
	Form3->Close();
	Form4->Close();
	Form5->Close();
	Form6->Close();
	Form7->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Edit2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		Button1Click(Button1);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
	{
		Edit2->SetFocus();
		Key = 0;
	}
}
//---------------------------------------------------------------------------

