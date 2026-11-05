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
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
	: TForm(Owner)
{
    RadioButton3->Checked = true;
    RadioButton1->Checked = false;
	RadioButton2->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button1Click(TObject *Sender)
{
	Form2->Show();
	Form1->Hide();
	Form7->Hide();

	Form1->Edit2->Text = "";

		Form2->Edit1->Text = "";
		Form2->Edit2->Text = "";

}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton1Click(TObject *Sender)
{
    RadioButton2->Checked = false;
    RadioButton3->Checked = false;
	Form1->DBGrid2->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton2Click(TObject *Sender)
{
    RadioButton1->Checked = false;
    RadioButton3->Checked = false;
	Form1->DBGrid2->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton3Click(TObject *Sender)
{
    RadioButton1->Checked = false;
    RadioButton2->Checked = false;
    Form1->DBGrid2->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button3Click(TObject *Sender)
{
    String currentUser = Form1->Edit2->Text;

    if (currentUser.Trim() == "") {
        MessageDlg("Пользователь не авторизован!", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    String oldPassword;
    if (InputQuery("Смена пароля", "Введите старый пароль:", oldPassword) == false) {
        return;
    }

    TADOQuery *checkQuery = new TADOQuery(NULL);
    try {
        checkQuery->Connection = Form1->ADOQuery1->Connection;
        checkQuery->SQL->Text = "SELECT password FROM users WHERE username = :username";
        checkQuery->Parameters->ParamByName("username")->Value = currentUser;
        checkQuery->Open();

        if (checkQuery->RecordCount == 0) {
            MessageDlg("Пользователь не найден в базе данных!", mtError, TMsgDlgButtons() << mbOK, 0);
            checkQuery->Close();
            delete checkQuery;
            return;
        }

        String storedPassword = checkQuery->FieldByName("password")->AsString;

        if (oldPassword != storedPassword) {
            MessageDlg("Старый пароль неверен!", mtError, TMsgDlgButtons() << mbOK, 0);
            checkQuery->Close();
            delete checkQuery;
            return;
        }

        checkQuery->Close();
    }
    __finally {
        delete checkQuery;
    }

    String newPassword;
    if (InputQuery("Смена пароля", "Введите новый пароль:", newPassword) == false) {
        return;
    }

    if (newPassword.Length() < 4) {
        MessageDlg("Новый пароль должен содержать не менее 4 символов!", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    if (newPassword == oldPassword) {
        MessageDlg("Новый пароль не должен совпадать со старым!", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    String confirmPassword;
    if (InputQuery("Смена пароля", "Подтвердите новый пароль:", confirmPassword) == false) {
        return;
    }

    if (newPassword != confirmPassword) {
        MessageDlg("Пароли не совпадают!", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
	}

    TADOQuery *updateQuery = new TADOQuery(NULL);
	try {
		updateQuery->Connection = Form1->ADOQuery1->Connection;
		updateQuery->SQL->Text = "UPDATE users SET [password] = :newPassword WHERE username = :username";
		updateQuery->Parameters->ParamByName("newPassword")->Value = newPassword;
        updateQuery->Parameters->ParamByName("username")->Value = currentUser;
		updateQuery->ExecSQL();

        MessageDlg("Пароль успешно изменен!", mtInformation, TMsgDlgButtons() << mbOK, 0);
	}
	__finally {
		delete updateQuery;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button2Click(TObject *Sender)
{
    String currentUser = Form1->Edit2->Text;

    if (currentUser.Trim() == "") {
        MessageDlg("Пользователь не авторизован!", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    if (currentUser == "admin") {
        MessageDlg("Нельзя удалить аккаунт администратора!", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    String password;
    if (InputQuery("Удаление аккаунта", "Введите ваш пароль для подтверждения:", password) == false) {
        return;
    }

    TADOQuery *checkQuery = new TADOQuery(NULL);
    checkQuery->Connection = Form1->ADOQuery1->Connection;
    checkQuery->SQL->Text = "SELECT [password] FROM users WHERE username = :username";
    checkQuery->Parameters->ParamByName("username")->Value = currentUser;
    checkQuery->Open();

    if (checkQuery->RecordCount == 0) {
        MessageDlg("Пользователь не найден в базе данных!", mtError, TMsgDlgButtons() << mbOK, 0);
        checkQuery->Close();
        delete checkQuery;
        return;
    }

    String storedPassword = checkQuery->FieldByName("password")->AsString;

    if (password != storedPassword) {
        MessageDlg("Пароль неверен! Удаление аккаунта отменено.", mtError, TMsgDlgButtons() << mbOK, 0);
        checkQuery->Close();
        delete checkQuery;
        return;
    }

    checkQuery->Close();
    delete checkQuery;

    if (MessageDlg("Вы действительно хотите удалить свой аккаунт?\n\nЭто действие невозможно отменить!",
                   mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) != mrYes) {
        return;
    }

    TADOQuery *deleteQuery = new TADOQuery(NULL);
    deleteQuery->Connection = Form1->ADOQuery1->Connection;

    try {
        deleteQuery->SQL->Text = "DELETE FROM tasks WHERE usename = :username";
        deleteQuery->Parameters->ParamByName("username")->Value = currentUser;
        deleteQuery->ExecSQL();

        deleteQuery->SQL->Text = "DELETE FROM users WHERE username = :username";
        deleteQuery->Parameters->ParamByName("username")->Value = currentUser;
        deleteQuery->ExecSQL();

        MessageDlg("Аккаунт успешно удален!", mtInformation, TMsgDlgButtons() << mbOK, 0);

        Form2->Show();
        Form1->Hide();
        Form7->Hide();

        Form1->Edit2->Text = "";
        Form2->Edit1->Text = "";
        Form2->Edit2->Text = "";
    }
    catch (Exception &e) {
        MessageDlg("Ошибка при удалении аккаунта: " + e.Message, mtError, TMsgDlgButtons() << mbOK, 0);
    }

    deleteQuery->Close();
    delete deleteQuery;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::SpeedButton1Click(TObject *Sender)
{
	Form2->Show();
	Form1->Hide();
	Form7->Hide();

	Form1->Edit2->Text = "";

		Form2->Edit1->Text = "";
		Form2->Edit2->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton4Click(TObject *Sender)
{
	RadioButton8->Checked = false;
	RadioButton5->Checked = false;
	RadioButton6->Checked = false;
	RadioButton7->Checked = false;
	Form1->Color = TColor(0x00FEEADA);

	Form1->Panel1->Color = TColor(0x00FBDAC4);
    Form1->Panel2->Color = TColor(0x00FBDAC4);
	Form1->Panel3->Color = TColor(0x00FBDAC4);
	Form1->Panel4->Color = TColor(0x00FBDAC4);

	Form1->Label4->Font->Color = clBlack;
	Form1->RichEdit1->Font->Color = clBlack;
	Form1->RichEdit2->Font->Color = clBlack;
	Form1->RichEdit3->Font->Color = clBlack;
	Form1->RichEdit4->Font->Color = clBlack;

	Form1->RichEdit1->Color = clWindow;
	Form1->RichEdit2->Color = clWindow;
	Form1->RichEdit3->Color = clWindow;
	Form1->RichEdit4->Color = clWindow;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton5Click(TObject *Sender)
{
	RadioButton4->Checked = false;
	RadioButton8->Checked = false;
	RadioButton6->Checked = false;
	RadioButton7->Checked = false;

	Form1->Color = TColor(0x005C403A);

	Form1->Panel1->Color = TColor(0x004D3832);
	Form1->Panel2->Color = TColor(0x004D3832);
	Form1->Panel3->Color = TColor(0x004D3832);
	Form1->Panel4->Color = TColor(0x004D3832);

	Form1->Label4->Font->Color = clWhite;
	Form1->RichEdit1->Font->Color = clWhite;
	Form1->RichEdit2->Font->Color = clWhite;
	Form1->RichEdit3->Font->Color = clWhite;
	Form1->RichEdit4->Font->Color = clWhite;

	Form1->RichEdit1->Color = TColor(0x004D3832);
	Form1->RichEdit2->Color = TColor(0x004D3832);
	Form1->RichEdit3->Color = TColor(0x004D3832);
	Form1->RichEdit4->Color = TColor(0x004D3832);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton6Click(TObject *Sender)
{
    RadioButton4->Checked = false;
	RadioButton5->Checked = false;
	RadioButton8->Checked = false;
	RadioButton7->Checked = false;

	Form1->Color = TColor(0x00DAFEFE);

	Form1->Panel1->Color = TColor(0x00C4DAFB);
	Form1->Panel2->Color = TColor(0x00C4DAFB);
	Form1->Panel3->Color = TColor(0x00C4DAFB);
	Form1->Panel4->Color = TColor(0x00C4DAFB);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton8Click(TObject *Sender)
{
	RadioButton4->Checked = false;
	RadioButton5->Checked = false;
	RadioButton6->Checked = false;
	RadioButton7->Checked = false;

    Form1->Color = TColor(0x00F0EAFE);

    Form1->Panel1->Color = TColor(0x00E6DCFA);
    Form1->Panel2->Color = TColor(0x00E6DCFA);
    Form1->Panel3->Color = TColor(0x00E6DCFA);
    Form1->Panel4->Color = TColor(0x00E6DCFA);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton7Click(TObject *Sender)
{
    RadioButton4->Checked = false;
    RadioButton5->Checked = false;
	RadioButton6->Checked = false;
	RadioButton8->Checked = false;

    Form1->Color = TColor(0x00FEDEE7);

    Form1->Panel1->Color = TColor(0x00F5D2DC);
    Form1->Panel2->Color = TColor(0x00F5D2DC);
    Form1->Panel3->Color = TColor(0x00F5D2DC);
    Form1->Panel4->Color = TColor(0x00F5D2DC);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button4Click(TObject *Sender)
{

    ScrollBox1->Visible = true;
    Button5->Visible = true;

    Label4->WordWrap = true;
    Label4->AutoSize = true;
	Label4->Width = ScrollBox1->ClientWidth - 20;

    ScrollBox1->HorzScrollBar->Visible = false;
	ScrollBox1->VertScrollBar->Visible = true;

	Label4->WordWrap = true;
	Label4->AutoSize = true;
	Label4->Width = ScrollBox1->ClientWidth - 20;
	Label4->Left = 10;
	Label4->Top = 10;

	ScrollBox1->Visible = true;
    Button5->Visible = true;

    Label4->Caption =
        "РУКОВОДСТВО ПОЛЬЗОВАТЕЛЯ\n"
		"----------------------------------\n\n"
		"1. Регистрация и авторизация:\n"
		"- Введите логин и пароль.\n"
		"- При ошибке система сообщит об ошибке.\n\n"
		"2. Управление задачами:\n"
		"- Создание, редактирование, удаление.\n"
		"- Установка срока, статуса и приоритета.\n\n"
		"3. Поиск и фильтрация:\n"
		"- Поиск по названию.\n"
		"- Фильтр по статусу.\n\n"
		"4. Темы интерфейса:\n"
		"- Светлая\n"
		"- Тёмная\n"
		"- Жёлтая\n"
		"- Розовая\n"
		"- Лавандовая\n\n"
		"5. Администрирование:\n"
		"- Просмотр списка пользователей.\n"
		"- Удаление пользователей.\n\n"
		"6. Смена пароля:\n"
		"- Введите старый пароль.\n"
		"- Введите новый пароль дважды.\n";

	Label4->Width = ScrollBox1->ClientWidth - 20;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button5Click(TObject *Sender)
{
    ScrollBox1->Visible = false;
    Button5->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::RadioButton9Click(TObject *Sender)
{
    RadioButton4->Checked = false;
    RadioButton5->Checked = false;
    RadioButton6->Checked = false;
    RadioButton7->Checked = false;
    RadioButton8->Checked = false;

    Form1->Color = TColor(0x00764BD2);

    Form1->Panel1->Color = TColor(0x00633AB9);
    Form1->Panel2->Color = TColor(0x00633AB9);
    Form1->Panel3->Color = TColor(0x00633AB9);
    Form1->Panel4->Color = TColor(0x00633AB9);

    Form1->Label4->Font->Color = clWhite;
    Form1->RichEdit1->Font->Color = clWhite;
    Form1->RichEdit2->Font->Color = clWhite;
    Form1->RichEdit3->Font->Color = clWhite;
    Form1->RichEdit4->Font->Color = clWhite;

    Form1->RichEdit1->Color = TColor(0x00633AB9);
    Form1->RichEdit2->Color = TColor(0x00633AB9);
    Form1->RichEdit3->Color = TColor(0x00633AB9);
	Form1->RichEdit4->Color = TColor(0x00633AB9);
}
//---------------------------------------------------------------------------

