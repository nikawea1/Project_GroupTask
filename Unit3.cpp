//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button1Click(TObject *Sender)
{

  	if (Edit1->Text != "" && Edit2->Text != "")
	{
		if (Edit1->Text.Length() < 4)
		{
			MessageDlg("Логин должен содержать не менее 4 символов", mtError, TMsgDlgButtons() << mbOK, 0);
			return;
		}
		if (Edit2->Text.Length() < 4)
		{
			MessageDlg("Пароль должен содержать не менее 4 символов", mtError, TMsgDlgButtons() << mbOK, 0);
			return;
		}
		Form1->ADOQuery1->SQL->Text = "select * from users where username=" + QuotedStr(Edit1->Text);
		Form1->ADOQuery1->Open();
		if (Form1->ADOQuery1->RecordCount == 0)
		{
			Form1->ADOQuery1->Close();
			Form1->ADOQuery1->SQL->Text = "Insert Into users (username, [password]) Values (:V1, :V2)";
			Form1->ADOQuery1->Parameters->ParamByName("V1")->Value = Edit1->Text;
			Form1->ADOQuery1->Parameters->ParamByName("V2")->Value = Edit2->Text;
			Form1->ADOQuery1->ExecSQL();
			Form2->Show();
			Form3->Hide();
			ShowMessage("Вы успешно зарегистировались!");
		}
		else
		{
			MessageDlg("Пользователь с таким именем уже существует. Введите другое имя.", mtError, TMsgDlgButtons() << mbOK, 0);
		}
	}
	else
	{
		MessageDlg("Заполните все поля данными.", mtError, TMsgDlgButtons() << mbOK, 0);
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form1->Close();
	Form2->Close();
	Form4->Close();
	Form5->Close();
    Form6->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Edit2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		Button1Click(Button1);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
	{
		Edit2->SetFocus();
		Key = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button2Click(TObject *Sender)
{
	Form3->Hide();
	Form2->Show();
}
//---------------------------------------------------------------------------

