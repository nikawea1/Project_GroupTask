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
#include "DbConfig.h"

#include <jpeg.hpp>
#include <string>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	try {
		String connStr = GetDatabaseConnectionString();
		if (ADOConnection1) { ADOConnection1->Connected = false; ADOConnection1->ConnectionString = connStr; ADOConnection1->Connected = true; }
		if (ADOConnection2) { ADOConnection2->Connected = false; ADOConnection2->ConnectionString = connStr; ADOConnection2->Connected = true; }
		if (ADOConnection3) { ADOConnection3->Connected = false; ADOConnection3->ConnectionString = connStr; ADOConnection3->Connected = true; }
		if (ADOConnection4) { ADOConnection4->Connected = false; ADOConnection4->ConnectionString = connStr; ADOConnection4->Connected = true; }
		if (ADOConnection5) { ADOConnection5->Connected = false; ADOConnection5->ConnectionString = connStr; ADOConnection5->Connected = true; }
	}
	catch (...) {}

	DBGrid2->OnDrawColumnCell = DBGrid2DrawColumnCell;

	TDBGridOptions gridOptions = DBGrid2->Options;
	gridOptions = gridOptions >> dgTitles;
	DBGrid2->Options = gridOptions;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false;
	Form2->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DBGrid2CellClick(TColumn *Column)
{
}
//---------------------------------------------------------------------------
void TForm1::ApplyTheme()
{
}
//---------------------------------------------------------------------------
void TForm1::UpdateTodayTasksChecklist()
{
	if (Edit2->Text.IsEmpty()) {
		ShowMessage("Пользователь не выбран");
		return;
	}

	String user = Edit2->Text;
	String today = FormatDateTime("yyyy.mm.dd", Date());

	CheckListBox1->Clear();

	TADOQuery *query = new TADOQuery(this);
	try {
		query->Connection = ADOQuery6->Connection;
		query->SQL->Text = "SELECT task_name FROM tasks WHERE usename = :usename AND task_deadline = :today";
		query->Parameters->ParamByName("usename")->Value = user;
		query->Parameters->ParamByName("today")->Value = today;
		query->Open();

		while (!query->Eof)
		{
			String taskName = query->FieldByName("task_name")->AsString;
			CheckListBox1->Items->Add(taskName);
			query->Next();
		}

		CheckListBox1->Update();
	}
	catch (Exception &e) {
		ShowMessage("Ошибка загрузки задач: " + e.Message);
	}

	query->Free();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateCategoriesComboBox()
{
	if (Edit2->Text.IsEmpty()) return;

	String user = Edit2->Text;
	String currentCategory = ComboBox1->Text;

	ComboBox1->Items->Clear();
	ComboBox1->Items->Add("Все категории");

	TADOQuery *query = new TADOQuery(this);
	try {
		query->Connection = ADOQuery2->Connection;
		query->SQL->Text =
			"SELECT DISTINCT task_category FROM tasks "
			"WHERE usename = :usename "
			"AND task_category IS NOT NULL "
			"AND task_category <> '' "
			"ORDER BY task_category";
		query->Parameters->ParamByName("usename")->Value = user;
		query->Open();

		while (!query->Eof) {
			String category = query->FieldByName("task_category")->AsString;
			if (category != "" && ComboBox1->Items->IndexOf(category) == -1) {
				ComboBox1->Items->Add(category);
			}
			query->Next();
		}

		if (ComboBox1->Items->IndexOf(currentCategory) != -1) {
			ComboBox1->Text = currentCategory;
		} else {
			ComboBox1->ItemIndex = 0;
		}
	}
	__finally {
		query->Close();
		delete query;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateStatusesComboBox()
{
	if (Edit2->Text.IsEmpty()) return;

	String user = Edit2->Text;
	String currentStatus = ComboBox2->Text;

	ComboBox2->Items->Clear();
	ComboBox2->Items->Add("Все статусы");

	TADOQuery *query = new TADOQuery(this);
	try {
		query->Connection = ADOQuery2->Connection;
		query->SQL->Text =
			"SELECT DISTINCT status FROM tasks "
			"WHERE usename = :usename "
			"AND status IS NOT NULL "
			"AND status <> '' "
			"ORDER BY status";
		query->Parameters->ParamByName("usename")->Value = user;
		query->Open();

		while (!query->Eof) {
			String status = query->FieldByName("status")->AsString;
			if (status != "" && ComboBox2->Items->IndexOf(status) == -1) {
				ComboBox2->Items->Add(status);
			}
			query->Next();
		}

		if (ComboBox2->Items->IndexOf(currentStatus) != -1) {
			ComboBox2->Text = currentStatus;
		} else {
			ComboBox2->ItemIndex = 0;
		}
	}
	__finally {
		query->Close();
		delete query;
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Form4->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form2->Close();
	Form3->Close();
	Form4->Close();
	Form5->Close();
	Form6->Close();
	Form6->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{

	ADOQuery2->Close();
	ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "'";
	ADOQuery2->Open();
	DBGrid2->DataSource->DataSet = ADOQuery2;

    RadioGroup1->ItemIndex = -1;
	RadioGroup2->ItemIndex = -1;
	ComboBox1->ItemIndex = 0;
	ComboBox2->ItemIndex = 0;

	Edit1->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
	ADOQuery2->Prior();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender)
{
	ADOQuery2->Next();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button12Click(TObject *Sender)
{

    String taskName = InputBox("Добавление задачи", "Введите название задачи:", "");
    if (taskName.IsEmpty()) {
        ShowMessage("Название задачи не может быть пустым!");
        return;
    }

	String taskCategory = InputBox("Категория задачи", "Введите категорию задачи:", "");

    String taskDesc = InputBox("Описание задачи", "Введите описание задачи:", "");

    String defaultDeadline = FormatDateTime("yyyy.mm.dd", Date() + 7);
    String deadline = InputBox("Дедлайн задачи", "Введите дедлайн (гггг.мм.дд):", defaultDeadline);
    if (deadline.IsEmpty()) {
        deadline = defaultDeadline;
    }

    try {
        ADOQuery2->Insert();
        ADOQuery2->Fields->FieldByName("usename")->AsString = Edit2->Text;
        ADOQuery2->Fields->FieldByName("task_name")->AsString = taskName;
        ADOQuery2->Fields->FieldByName("status")->AsString = "в процессе";
        ADOQuery2->Fields->FieldByName("task_category")->AsString = taskCategory;
        ADOQuery2->Fields->FieldByName("task_desc")->AsString = taskDesc;
        ADOQuery2->Fields->FieldByName("task_deadline")->AsString = deadline;

        String currentDate = FormatDateTime("yyyy.mm.dd", Date());
        ADOQuery2->Fields->FieldByName("task_date")->AsString = currentDate;

        try {
            ADOQuery2->Fields->FieldByName("created_date")->AsString = currentDate;
        }
        catch (...) {
		}

        ADOQuery2->Post();

        UpdateCategoriesComboBox();

        ADOQuery2->Close();
        ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "'";
        ADOQuery2->Open();

        ShowMessage("Задача '" + taskName + "' успешно добавлена!");
    }
    catch (Exception &e) {
        ShowMessage("Ошибка при добавлении задачи: " + e.Message);
    }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
    if (ADOQuery2->IsEmpty()) {
        ShowMessage("Нет выбранной задачи для удаления!");
        return;
    }

    String taskName = ADOQuery2->FieldByName("task_name")->AsString;
    String category = ADOQuery2->FieldByName("task_category")->AsString;

    if (MessageDlg("Вы действительно хотите удалить задачу: " + taskName + "?",
                   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
    {
        ComboBox1->Items->Clear();

		ADOQuery2->Delete();

        UpdateCategoriesComboBox();

        ADOQuery2->Close();
        ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "'";
        ADOQuery2->Open();

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Change(TObject *Sender)
{

	String searchText = Edit1->Text.Trim();
	if (searchText != "")
	{
		

		ADOQuery2->Close();
		ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = :usename AND task_name LIKE :task_name";
		ADOQuery2->Parameters->ParamByName("usename")->Value = Edit2->Text;
		ADOQuery2->Parameters->ParamByName("task_name")->Value = searchText + "%";
		ADOQuery2->Open();
		DBGrid2->DataSource->DataSet = ADOQuery2;
	}
	else
	{

		Form1->ADOQuery2->Close();

		Form1->ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Form1->Edit2->Text + "'";
		Form1->ADOQuery2->Open();
		Form1->DBGrid2->DataSource->DataSet = Form1->ADOQuery2;
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	ADOQuery1->SQL->Text = "SELECT * FROM users";

	Panel1->Parent = TabSheet1;
	Panel1->Align = alClient;
	Panel1->BevelOuter = bvNone;
    Panel1->SendToBack();

    Panel2->Parent = TabSheet2;
    Panel2->Align = alClient;
	Panel2->BevelOuter = bvNone;
    Panel2->SendToBack();

    Panel3->Parent = TabSheet3;
    Panel3->Align = alClient;
	Panel3->BevelOuter = bvNone;
    Panel3->SendToBack();

    Panel4->Parent = TabSheet4;
    Panel4->Align = alClient;
    Panel4->BevelOuter = bvNone;
	Panel4->SendToBack();

	RichEdit1->Parent = Panel1;
	 RichEdit1->Text="Чек-лист задач";
	RichEdit1->Transparent = true;
	RichEdit1->ReadOnly = true;
	RichEdit1->BorderStyle = bsNone;
	RichEdit1->ScrollBars = ssNone;
	RichEdit1->Enabled = false;

	RichEdit2->Parent = Panel2;
	RichEdit2->Text="Управление пользователями";
	RichEdit2->Transparent = true;
	RichEdit2->ReadOnly = true;
	RichEdit2->BorderStyle = bsNone;
	RichEdit2->ScrollBars = ssNone;
	RichEdit2->Enabled = false;

	RichEdit3->Parent = Panel3;
	RichEdit3->Text="Архив задач";
	RichEdit3->Transparent = true;
	RichEdit3->ReadOnly = true;
	RichEdit3->BorderStyle = bsNone;
	RichEdit3->ScrollBars = ssNone;
	RichEdit3->Enabled = false;

	RichEdit4->Parent = Panel4;
	RichEdit4->Text="Управление групповыми задачами";
	RichEdit4->Transparent = true;
	RichEdit4->ReadOnly = true;
	RichEdit4->BorderStyle = bsNone;
	RichEdit4->ScrollBars = ssNone;
	RichEdit4->Enabled = false;

	RichEdit5->Text="Задача              Статус         Описание  Категория   Дедлайн";
	RichEdit5->Transparent = true;
	RichEdit5->ReadOnly = true;
	RichEdit5->BorderStyle = bsNone;
	RichEdit5->ScrollBars = ssNone;
	RichEdit5->Enabled = false;

	RichEdit6->Text="Инструменты";
	RichEdit6->Transparent = true;
	RichEdit6->ReadOnly = true;
	RichEdit6->BorderStyle = bsNone;
	RichEdit6->ScrollBars = ssNone;
	RichEdit6->Enabled = false;

}
//---------------------------------------------------------------------------

int currentId = 1;
void __fastcall TForm1::DBGrid2DblClick(TObject *Sender)
{

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button17Click(TObject *Sender)
{

	Form6->Show();

		Form6->ADOQuery1->Open();

	if (Form1->Edit2->Text == "admin") {
		Form6->Edit1->Visible = true;
		Form6->Button9->Visible = true;
		Form6->ADOQuery1->Close();
		Form6->ADOQuery1->SQL->Text = "SELECT * FROM tasks";
		Form6->ADOQuery1->Open();
		return;

	} else  {

			Form6->Edit1->Visible = false;
			Form6->Button9->Visible = false;
			Form6->Button11->Visible = false;

			String currentUser = Form1->Edit2->Text;

			Form6->ADOQuery1->Close();
			Form6->ADOQuery1->SQL->Text = "SELECT * FROM tasks WHERE usename = :username ORDER BY task_date DESC";
			Form6->ADOQuery1->Parameters->ParamByName("username")->Value = currentUser;
			Form6->ADOQuery1->Open();

			Form6->DBGrid1->Refresh();

		}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button19Click(TObject *Sender)
{
	ADOQuery1->Open();
	ADOQuery1->Edit();

	if (ADOQuery1->Locate("username", LabeledEdit6->Text, TLocateOptions()))
	{
		ADOQuery1->Edit();
		ADOQuery1->Fields->FieldByName("password")->AsString = LabeledEdit7->Text;
		ADOQuery1->Post();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button20Click(TObject *Sender)
{
	 TLocateOptions Options;
	 if(ADOQuery1->Locate("username", LabeledEdit6->Text, Options))
	 ADOQuery1->Delete();
	 else ShowMessage("Эта запись пустая");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button21Click(TObject *Sender)
{
	if(LabeledEdit6->Text != "" && LabeledEdit7->Text != "")
	{
		ADOQuery1->Insert();
		ADOQuery1->Fields->FieldByName("username")->AsString = LabeledEdit6->Text;
		ADOQuery1->Fields->FieldByName("password")->AsString = LabeledEdit7->Text;

		ADOQuery1->Post();

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit3Change(TObject *Sender)
{
	String searchText1 = Edit3->Text.Trim();
	if (searchText1 != "")
	{
		ADOQuery1->Close();
		ADOQuery1->SQL->Text = "SELECT * FROM users WHERE username LIKE :username";
		ADOQuery1->Parameters->ParamByName("username")->Value = searchText1 + "%";
		ADOQuery1->Open();
		DBGrid1->DataSource->DataSet = ADOQuery1;
	}
	else
	{
		ADOQuery1->Close();
		ADOQuery1->SQL->Text = "SELECT * FROM users";
		ADOQuery1->Open();
		DBGrid1->DataSource->DataSet = ADOQuery1;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch1Click(TObject *Sender)
{
	isDarkTheme = !isDarkTheme;
	if (isDarkTheme)
	{
		Form1->Color = TColor(0x008D4B49);
		Form1->Font->Color = clWhite;

		Button2->Font->Color = clWhite;
		DBGrid1->Color = TColor(0x00444444);
		DBGrid2->Color = TColor(0x00444444);

		ToggleSwitch1->Color = TColor(0x00444444);
		ToggleSwitch1->Font->Color = clWhite;

	}
	else
	{
		Form1->Color = TColor(0xF5DEAD);
		Form1->Font->Color = clBlack;

		Button2->Font->Color = clBlack;
		DBGrid1->Color = clWhite;
		DBGrid1->Font->Color = clBlack;
		DBGrid2->Color = clWhite;
		DBGrid2->Font->Color = clBlack;

		ToggleSwitch1->Color = clWhite;
		ToggleSwitch1->Font->Color = clBlack;

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2Click(TObject *Sender)
{
	

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

void __fastcall TForm1::N4Click(TObject *Sender)
{
   

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

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{

    ADOQuery2->Close();
    ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "'";
	ADOQuery2->Open();

	UpdateCategoriesComboBox();

	String user = Edit2->Text;
	String cat = ComboBox1->Text;

	ADOQuery2->Close();

	if (cat == "Все категории" || cat == "")
	{
		ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "'";
	}
	else
	{
		ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "' AND task_category = '" + cat + "'";
	}

	ADOQuery2->Open();
    DBGrid2->DataSource->DataSet = ADOQuery2;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox2Change(TObject *Sender)
{
    ADOQuery2->Close();
    ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "'";
	ADOQuery2->Open();

	UpdateStatusesComboBox();

	String user = Edit2->Text;
	String status = ComboBox2->Text;

	ADOQuery2->Close();

	if (status == "Все статусы" || status == "")
	{
		ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "'";
	}
	else
	{
		ADOQuery2->SQL->Text =
			"SELECT * FROM tasks WHERE usename = '" + user + "' AND status = '" + status + "'";
	}

	ADOQuery2->Open();
	DBGrid2->DataSource->DataSet = ADOQuery2;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button22Click(TObject *Sender)
{
	String taskName   = ADOQuery2->FieldByName("task_name")->AsString;
	String status     = ADOQuery2->FieldByName("status")->AsString;
	String category   = ADOQuery2->FieldByName("task_category")->AsString;
	String description= ADOQuery2->FieldByName("task_desc")->AsString;
	String date       = ADOQuery2->FieldByName("task_date")->AsString;
	String deadline   = ADOQuery2->FieldByName("task_deadline")->AsString;

	Form5->Label1->Caption = "Задача: " + taskName;
	Form5->Label2->Caption = "Статус: " + status;
	Form5->Label3->Caption = "Категория: " + category;
	Form5->Label4->Caption = "Описание: " + description;
	Form5->Label5->Caption = "Дата создания: " + date;
	Form5->Label6->Caption = "Дедлайн: " + deadline;

	

	if (Form5->Visible)
	{
		Form5->Hide();
		Sleep(50);
	}
	Form5->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N6Click(TObject *Sender)
{
	Form2->Show();
	Form1->Hide();

	Form1->Edit2->Text = "";

	Form2->Edit1->Text = "";
	Form2->Edit2->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioGroup1Click(TObject *Sender)
{
    switch(RadioGroup1->ItemIndex)
    {
        case 0:
        {
            String currentDate = FormatDateTime("yyyy.mm.dd", Date());
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "' AND task_deadline < '" + currentDate + "'";
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
        case 1:
        {
            if (Edit2->Text.IsEmpty()) return;
            String user = Edit2->Text;
            String today = FormatDateTime("yyyy.mm.dd", Date());
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "' AND task_deadline = '" + today + "'";
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
        case 2:
        {
            if (Edit2->Text.IsEmpty()) return;
            String user = Edit2->Text;
            TDateTime tomorrow = Date() + 1;
            String tomorrowStr = FormatDateTime("yyyy.mm.dd", tomorrow);
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "' AND task_deadline = '" + tomorrowStr + "'";
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
        case 3:
        {
            if (Edit2->Text.IsEmpty()) return;
            String user = Edit2->Text;
            TDateTime today = Date();
            int dayOfWeek = DayOfWeek(today);
            TDateTime startOfWeek = today - (dayOfWeek - 2);
            if (dayOfWeek == 1) startOfWeek = today - 6;
            TDateTime endOfWeek = startOfWeek + 6;
            String startStr = FormatDateTime("yyyy.mm.dd", startOfWeek);
            String endStr = FormatDateTime("yyyy.mm.dd", endOfWeek);
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "' AND task_deadline BETWEEN '" + startStr + "' AND '" + endStr + "' ORDER BY task_deadline ASC";
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
        case 4:
        {
            String dateFrom = LabeledEdit4->Text;
            String dateTo = LabeledEdit5->Text;
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "' AND task_date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'";
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioGroup2Click(TObject *Sender)
{
    if (Edit2->Text.IsEmpty()) return;
    String user = Edit2->Text;

    switch(RadioGroup2->ItemIndex)
    {
        case 0:
        {
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "' ORDER BY task_category";
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
        case 1:
        {
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = :user AND task_date IS NOT NULL ORDER BY task_date ASC";
            ADOQuery2->Parameters->ParamByName("user")->Value = user;
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
        case 2:
        {
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = :user AND task_date IS NOT NULL ORDER BY task_date DESC";
            ADOQuery2->Parameters->ParamByName("user")->Value = user;
            ADOQuery2->Open();
            DBGrid2->DataSource->DataSet = ADOQuery2;
            break;
        }
        case 3:
        {
            ADOQuery2->Close();
            ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "' ORDER BY task_deadline ASC";
			ADOQuery2->Open();
			DBGrid2->DataSource->DataSet = ADOQuery2;
			break;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckListBox1Click(TObject *Sender)
{

}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton1Click(TObject *Sender)
{
	UpdateTodayTasksChecklist();
	ADOQuery2->Close();
	ADOQuery2->Open();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton2Click(TObject *Sender)
{
	if (Edit2->Text.IsEmpty()) {
		ShowMessage("Пользователь не авторизован");
		return;
	}

	String user = Edit2->Text;
	String selectedDate = FormatDateTime("yyyy.mm.dd", DateTimePicker1->Date);

	CheckListBox1->Clear();

	TADOQuery *query = new TADOQuery(this);
	try {
		query->Connection = ADOQuery6->Connection;
		query->SQL->Text = "SELECT task_name FROM tasks WHERE usename = :usename AND task_deadline = :selectedDate";
		query->Parameters->ParamByName("usename")->Value = user;
		query->Parameters->ParamByName("selectedDate")->Value = selectedDate;
		query->Open();

		while (!query->Eof)
        {
			String taskName = query->FieldByName("task_name")->AsString;
			CheckListBox1->Items->Add(taskName);
			query->Next();
		}

		CheckListBox1->Update();
	}
	catch (Exception &e) {
		ShowMessage("Ошибка: " + e.Message);
	}

	query->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckListBox1ClickCheck(TObject *Sender)
{

    int index = CheckListBox1->ItemIndex;
    if (index < 0) return;

    String taskName = CheckListBox1->Items->Strings[index];

    String newStatus = CheckListBox1->Checked[index] ? "выполнено" : "в процессе";

    TADOQuery *query = new TADOQuery(this);
    try {
        query->Connection = ADOQuery6->Connection;
        query->SQL->Text = "UPDATE tasks SET status = :status WHERE task_name = :taskName AND usename = :usename";
        query->Parameters->ParamByName("status")->Value = newStatus;
        query->Parameters->ParamByName("taskName")->Value = taskName;
        query->Parameters->ParamByName("usename")->Value = Edit2->Text;
        query->ExecSQL();
    }
    catch (Exception &e) {
        ShowMessage("Ошибка при обновлении статуса: " + e.Message);
    }

    query->Free();

    ADOQuery2->Close();
    ADOQuery2->Open();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DateTimePicker1Change(TObject *Sender)
{
		if (RadioButton2->Checked) {
                 String user = Edit2->Text;
			String selectedDate = FormatDateTime("yyyy.mm.dd", DateTimePicker1->Date);

			CheckListBox1->Clear();

			TADOQuery *query = new TADOQuery(this);

			try {
				query->Connection = ADOQuery6->Connection;
				query->SQL->Text = "SELECT task_name, status FROM tasks WHERE usename = :usename AND task_deadline = :selectedDate";
				query->Parameters->ParamByName("usename")->Value = user;
				query->Parameters->ParamByName("selectedDate")->Value = selectedDate;
				query->Open();

				while (!query->Eof) {
					String taskName = query->FieldByName("task_name")->AsString;
					String status = query->FieldByName("status")->AsString;

					int index = CheckListBox1->Items->Add(taskName);
					CheckListBox1->Checked[index] = (status == "выполнено");

					query->Next();
				}

				CheckListBox1->Update();
			}
			catch (Exception &e) {
				ShowMessage("Ошибка при загрузке задач: " + e.Message);
			}

		query->Free();

		}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button23Click(TObject *Sender)
{

    String groupName = InputBox("Создание группы", "Введите название группы:", "");
    if (groupName.IsEmpty()) {
        ShowMessage("Название группы не может быть пустым!");
        return;
    }

    String groupKey = "";
    String chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    randomize();

    bool keyIsUnique = false;
    do {
        groupKey = "";
        for (int i = 0; i < 10; i++) {
            int index = rand() % chars.Length();
            groupKey += chars[index+1];
        }

        ADOQuery7->Close();
        ADOQuery7->SQL->Text = "SELECT * FROM groups WHERE group_key = '" + groupKey + "'";
        ADOQuery7->Open();
        keyIsUnique = ADOQuery7->IsEmpty();
        ADOQuery7->Close();

    } while (!keyIsUnique);

    ADOQuery7->SQL->Text =
        "INSERT INTO groups (group_name, group_key, created_by) VALUES ('" +
        groupName + "', '" + groupKey + "', '" + Edit2->Text + "')";
    ADOQuery7->ExecSQL();

    ADOQuery7->SQL->Text =
        "INSERT INTO group_members (group_key, username) VALUES ('" +
        groupKey + "', '" + Edit2->Text + "')";
	ADOQuery7->ExecSQL();

	Edit4->Text = groupKey;
	Edit4->ReadOnly = true;
	Edit4->Color = clInfoBk;

	ShowMessage("Группа '" + groupName + "' создана!\nКлюч: " + groupKey);

    ComboBox4->Clear();
    ComboBox4->Items->Add("Не назначено");
    ComboBox4->Items->Add(Edit2->Text);
	ComboBox4->ItemIndex = 0;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button24Click(TObject *Sender)
{

	String groupKey = Edit5->Text.Trim();
    if (groupKey.IsEmpty()) {
        ShowMessage("Введите ключ группы!");
        return;
    }

    ADOQuery7->Close();
    ADOQuery7->SQL->Text = "SELECT group_name FROM groups WHERE group_key = '" + groupKey + "'";
    ADOQuery7->Open();

    if (ADOQuery7->IsEmpty()) {
        ShowMessage("Группа с таким ключом не найдена!");
        ADOQuery7->Close();
        return;
    }

    String groupName = ADOQuery7->FieldByName("group_name")->AsString;
    ADOQuery7->Close();

    ADOQuery7->SQL->Text = "SELECT * FROM group_members WHERE group_key = '" + groupKey + "' AND username = '" + Edit2->Text + "'";
    ADOQuery7->Open();

    if (!ADOQuery7->IsEmpty()) {
        ShowMessage("Вы уже состоите в этой группе!");
        ADOQuery7->Close();
        return;
    }
    ADOQuery7->Close();

	ADOQuery7->SQL->Text = "INSERT INTO group_members (group_key, username) VALUES ('" + groupKey + "', '" + Edit2->Text + "')";
	ADOQuery7->ExecSQL();

	ShowMessage("Вы успешно присоединились к группе: " + groupName);
	Edit5->Text = "";

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button27Click(TObject *Sender)
{
	ComboBox3->Clear();
	ComboBox3->Items->Add("Все группы");

	ADOQuery7->Close();
	ADOQuery7->SQL->Text = "SELECT group_key FROM group_members WHERE username = '" + Edit2->Text + "'";
	ADOQuery7->Open();

	while (!ADOQuery7->Eof) {
		String groupKey = ADOQuery7->FieldByName("group_key")->AsString;

		TADOQuery *tempQuery = new TADOQuery(this);
		try {
			tempQuery->Connection = ADOConnection1;
			tempQuery->SQL->Text = "SELECT group_name, created_by FROM groups WHERE group_key = '" + groupKey + "'";
			tempQuery->Open();

			if (!tempQuery->IsEmpty()) {
				String groupName = tempQuery->FieldByName("group_name")->AsString;
				String createdBy = tempQuery->FieldByName("created_by")->AsString;

				if (createdBy == Edit2->Text) {
					groupName = groupName + " (вы создатель)";
				}

				ComboBox3->Items->AddObject(groupName, (TObject*)new String(groupKey));
			}

			tempQuery->Close();
		}
		__finally {
			delete tempQuery;
		}

		ADOQuery7->Next();
	}

	ADOQuery7->Close();

	if (ComboBox3->Items->Count > 0) {
		ComboBox3->ItemIndex = 0;
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button25Click(TObject *Sender)
{

        if (ComboBox3->ItemIndex <= 0) {
        ShowMessage("Выберите группу!");
        return;
    }

    String groupKey = *((String*)ComboBox3->Items->Objects[ComboBox3->ItemIndex]);

    ADOQuery7->Close();
    ADOQuery7->SQL->Text = "SELECT created_by FROM groups WHERE group_key = '" + groupKey + "'";
    ADOQuery7->Open();

    if (ADOQuery7->IsEmpty() || ADOQuery7->FieldByName("created_by")->AsString != Edit2->Text) {
        ShowMessage("Только создатель группы может добавлять участников!");
        ADOQuery7->Close();
        return;
    }
    ADOQuery7->Close();

    String username = InputBox("Добавление участника", "Введите имя пользователя:", "");
    if (username.IsEmpty()) {
        ShowMessage("Имя пользователя не может быть пустым!");
        return;
    }

    ADOQuery7->SQL->Text = "SELECT * FROM users WHERE username = '" + username + "'";
    ADOQuery7->Open();

    if (ADOQuery7->IsEmpty()) {
        ShowMessage("Пользователь с таким именем не существует!");
        ADOQuery7->Close();
        return;
    }
    ADOQuery7->Close();

    ADOQuery7->SQL->Text = "SELECT * FROM group_members WHERE group_key = '" + groupKey + "' AND username = '" + username + "'";
    ADOQuery7->Open();

    if (!ADOQuery7->IsEmpty()) {
        ShowMessage("Этот пользователь уже в группе!");
        ADOQuery7->Close();
        return;
    }
    ADOQuery7->Close();

    ADOQuery7->SQL->Text = "INSERT INTO group_members (group_key, username) VALUES ('" + groupKey + "', '" + username + "')";
    ADOQuery7->ExecSQL();

    ShowMessage("Пользователь " + username + " добавлен в группу!");

	ComboBox3Change(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox3Change(TObject *Sender)
{

    ComboBox4->Clear();
    ComboBox4->Items->Add("Все участники");

    if (ComboBox3->ItemIndex > 0) {
        String groupKey = *((String*)ComboBox3->Items->Objects[ComboBox3->ItemIndex]);

        ADOQuery7->Close();
        ADOQuery7->SQL->Text = "SELECT username FROM group_members WHERE group_key = '" + groupKey + "' ORDER BY username";
        ADOQuery7->Open();

        while (!ADOQuery7->Eof) {
            ComboBox4->Items->Add(ADOQuery7->FieldByName("username")->AsString);
            ADOQuery7->Next();
        }
        ADOQuery7->Close();

        ADOQuery8->Close();
        ADOQuery8->SQL->Text = "SELECT * FROM group_tasks WHERE group_key = '" + groupKey + "' ORDER BY task_deadline";
        ADOQuery8->Open();

    } else {
        ADOQuery8->Close();
        ADOQuery8->SQL->Text =
            "SELECT gt.* FROM group_tasks gt "
            "INNER JOIN group_members gm ON gt.group_key = gm.group_key "
            "WHERE gm.username = '" + Edit2->Text + "' "
            "ORDER BY gt.task_deadline";
        ADOQuery8->Open();

        ADOQuery7->Close();
        ADOQuery7->SQL->Text =
            "SELECT DISTINCT gm.username FROM group_members gm "
            "INNER JOIN group_members my_groups ON gm.group_key = my_groups.group_key "
            "WHERE my_groups.username = '" + Edit2->Text + "' "
            "ORDER BY gm.username";
        ADOQuery7->Open();

        while (!ADOQuery7->Eof) {
            ComboBox4->Items->Add(ADOQuery7->FieldByName("username")->AsString);
            ADOQuery7->Next();
        }
        ADOQuery7->Close();
    }

    ComboBox4->ItemIndex = 0;

	DBGrid3->Refresh();

	if (DBGrid3->Columns->Count > 0) {
		for (int i = 0; i < DBGrid3->Columns->Count; i++) {
			TColumn* column = DBGrid3->Columns->Items[i];
			String fieldName = column->FieldName.LowerCase();

			if (fieldName.Pos("name") > 0) {
				column->Width = 150;
			}
			else if (fieldName.Pos("desc") > 0) {
				column->Width = 200;
			}
			else if (fieldName.Pos("date") > 0 || fieldName.Pos("deadline") > 0) {
				column->Width = 100;
			}
			else if (fieldName.Pos("status") > 0) {
				column->Width = 100;
			}
			else if (fieldName.Pos("category") > 0) {
				column->Width = 120;
			}
			else if (fieldName.Pos("key") > 0) {
				column->Width = 80;
			}
			else {
				column->Width = 100;
			}
		}

		DBGrid3->Refresh();
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button26Click(TObject *Sender)
{

	if (ComboBox3->ItemIndex <= 0) {
		ShowMessage("Выберите группу!");
		return;
	}

	String groupKey = *((String*)ComboBox3->Items->Objects[ComboBox3->ItemIndex]);

	String assignedTo = InputBox("Назначение задачи", "Введите имя пользователя для назначения задачи:", "");
	if (assignedTo.IsEmpty()) {
		ShowMessage("Имя пользователя не может быть пустым!");
		return;
	}

	TADOQuery *checkQuery = new TADOQuery(this);
	checkQuery->Connection = ADOConnection1;
	checkQuery->SQL->Text = "SELECT * FROM group_members WHERE group_key = :group_key AND username = :username";
	checkQuery->Parameters->ParamByName("group_key")->Value = groupKey;
	checkQuery->Parameters->ParamByName("username")->Value = assignedTo;
	checkQuery->Open();

	if (checkQuery->IsEmpty()) {
		ShowMessage("Пользователь '" + assignedTo + "' не состоит в этой группе!");
		checkQuery->Close();
		delete checkQuery;
		return;
	}
	checkQuery->Close();
	delete checkQuery;

	String taskName = InputBox("Название задачи", "Введите название задачи:", "");
	if (taskName.IsEmpty()) {
		ShowMessage("Название задачи не может быть пустым!");
		return;
	}

	String taskCategory = InputBox("Категория задачи", "Введите категорию задачи:", "");
	String taskDesc = InputBox("Описание задачи", "Введите описание задачи:", "");
	String deadline = InputBox("Дедлайн задачи", "Введите дедлайн (гггг.мм.дд):", FormatDateTime("yyyy.mm.dd", Date() + 7));

	TADOQuery *insertQuery = new TADOQuery(this);
	insertQuery->Connection = ADOConnection1;
	insertQuery->SQL->Text =
		"INSERT INTO group_tasks (group_key, task_name, assigned_to, status, "
		"task_category, task_desc, task_date, task_deadline, created_by, created_date) "
		"VALUES (:group_key, :task_name, :assigned_to, 'назначено', "
		":task_category, :task_desc, :task_date, :task_deadline, :created_by, :created_date)";

	String currentDate = FormatDateTime("yyyy.mm.dd", Date());

	insertQuery->Parameters->ParamByName("group_key")->Value = groupKey;
	insertQuery->Parameters->ParamByName("task_name")->Value = taskName;
	insertQuery->Parameters->ParamByName("assigned_to")->Value = assignedTo;
	insertQuery->Parameters->ParamByName("task_category")->Value = taskCategory;
	insertQuery->Parameters->ParamByName("task_desc")->Value = taskDesc;
	insertQuery->Parameters->ParamByName("task_date")->Value = currentDate;
	insertQuery->Parameters->ParamByName("task_deadline")->Value = deadline;
	insertQuery->Parameters->ParamByName("created_by")->Value = Edit2->Text;
	insertQuery->Parameters->ParamByName("created_date")->Value = currentDate;

	insertQuery->ExecSQL();
	delete insertQuery;

	ShowMessage("Задача '" + taskName + "' успешно назначена пользователю " + assignedTo);

	String selectedUser = ComboBox4->Text;
	String sql = "SELECT * FROM group_tasks WHERE group_key = '" + groupKey + "'";

	if (selectedUser != "Все участники" && !selectedUser.IsEmpty()) {
		sql = sql + " AND assigned_to = '" + selectedUser + "'";
	}

	sql = sql + " ORDER BY task_deadline";

	ADOQuery8->Close();
	ADOQuery8->SQL->Text = sql;
	ADOQuery8->Open();
	DBGrid3->Refresh();

	ComboBox4Change(Sender);

	DBGrid3->Refresh();

	if (DBGrid3->Columns->Count > 0) {
		for (int i = 0; i < DBGrid3->Columns->Count; i++) {
			TColumn* column = DBGrid3->Columns->Items[i];
			String fieldName = column->FieldName.LowerCase();

			if (fieldName.Pos("name") > 0) {
				column->Width = 150;
			}
			else if (fieldName.Pos("desc") > 0) {
				column->Width = 200;
			}
			else if (fieldName.Pos("date") > 0 || fieldName.Pos("deadline") > 0) {
				column->Width = 100;
			}
			else if (fieldName.Pos("status") > 0) {
				column->Width = 100;
			}
			else if (fieldName.Pos("category") > 0) {
				column->Width = 120;
			}
			else if (fieldName.Pos("key") > 0) {
				column->Width = 80;
			}
			else {
				column->Width = 100;
			}
		}

		DBGrid3->Refresh();
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox4Change(TObject *Sender)
{

		if (ComboBox3->ItemIndex <= 0) return;

    String groupKey = *((String*)ComboBox3->Items->Objects[ComboBox3->ItemIndex]);
    String selectedUser = ComboBox4->Text;

    String sql = "SELECT * FROM group_tasks WHERE group_key = '" + groupKey + "'";

    if (selectedUser != "Все участники" && !selectedUser.IsEmpty()) {
        sql = sql + " AND assigned_to = '" + selectedUser + "'";
    }

    sql = sql + " ORDER BY task_deadline";

    ADOQuery8->Close();
	ADOQuery8->SQL->Text = sql;

	ADOQuery8->Open();

}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button29Click(TObject *Sender)
{
	if (ComboBox3->ItemIndex <= 0) {
		ShowMessage("Выберите группу!");
		return;
	}

	String groupKey = *((String*)ComboBox3->Items->Objects[ComboBox3->ItemIndex]);

    TADOQuery *checkCreator = new TADOQuery(this);
    checkCreator->Connection = ADOConnection1;
    checkCreator->SQL->Text = "SELECT created_by FROM groups WHERE group_key = :group_key";
    checkCreator->Parameters->ParamByName("group_key")->Value = groupKey;
    checkCreator->Open();

    if (checkCreator->IsEmpty() || checkCreator->FieldByName("created_by")->AsString != Edit2->Text) {
        ShowMessage("Только создатель группы может исключать пользователей!");
        checkCreator->Close();
        delete checkCreator;
        return;
    }
    checkCreator->Close();
    delete checkCreator;

    String usernameToRemove = InputBox("Исключение пользователя", "Введите имя пользователя для исключения:", "");
    if (usernameToRemove.IsEmpty()) {
        ShowMessage("Имя пользователя не может быть пустым!");
        return;
    }

    TADOQuery *checkUser = new TADOQuery(this);
    checkUser->Connection = ADOConnection1;
    checkUser->SQL->Text = "SELECT * FROM group_members WHERE group_key = :group_key AND username = :username";
    checkUser->Parameters->ParamByName("group_key")->Value = groupKey;
    checkUser->Parameters->ParamByName("username")->Value = usernameToRemove;
    checkUser->Open();

    if (checkUser->IsEmpty()) {
        ShowMessage("Пользователь '" + usernameToRemove + "' не состоит в этой группе!");
        checkUser->Close();
        delete checkUser;
        return;
    }
    checkUser->Close();
    delete checkUser;

    if (usernameToRemove == Edit2->Text) {
        ShowMessage("Вы не можете исключить себя из группы! Если хотите удалить группу, используйте соответствующую функцию.");
        return;
    }

    TADOQuery *deleteQuery = new TADOQuery(this);
	deleteQuery->Connection = ADOConnection1;
	deleteQuery->SQL->Text = "DELETE FROM group_members WHERE group_key = :group_key AND username = :username";
	deleteQuery->Parameters->ParamByName("group_key")->Value = groupKey;
	deleteQuery->Parameters->ParamByName("username")->Value = usernameToRemove;
	deleteQuery->ExecSQL();
    delete deleteQuery;

	ShowMessage("Пользователь " + usernameToRemove + " успешно исключен из группы!");

	ComboBox3Change(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button30Click(TObject *Sender)
{
	if (MessageDlg("Вы действительно хотите архивировать ВСЕ выполненные задачи?\n\n"
				   "Эта операция переместит все задачи со статусом 'выполнено' в архив.",
				   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	{
		String user = Edit2->Text;
		String archiveDate = FormatDateTime("yyyy.mm.dd", Date());

		TADOQuery *archiveQuery = new TADOQuery(this);
		archiveQuery->Connection = ADOConnection1;
		archiveQuery->SQL->Text =
			"INSERT INTO archive_tasks (usename, task_name, status, task_desc, "
			"task_category, task_date, task_deadline, archive_date) "
			"SELECT usename, task_name, status, task_desc, "
			"task_category, task_date, task_deadline, :archive_date "
			"FROM tasks WHERE usename = :usename AND status = 'выполнено'";

		archiveQuery->Parameters->ParamByName("archive_date")->Value = archiveDate;
		archiveQuery->Parameters->ParamByName("usename")->Value = user;
		archiveQuery->ExecSQL();
        delete archiveQuery;

		TADOQuery *deleteQuery = new TADOQuery(this);
        deleteQuery->Connection = ADOConnection1;
		deleteQuery->SQL->Text = "DELETE FROM tasks WHERE usename = :usename AND status = 'выполнено'";
		deleteQuery->Parameters->ParamByName("usename")->Value = user;
		int deletedCount = deleteQuery->ExecSQL();
        delete deleteQuery;

		ADOQuery2->Close();
		ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "'";
		ADOQuery2->Open();

		UpdateCategoriesComboBox();
		UpdateTodayTasksChecklist();

	String user1 = Edit2->Text;

	ADOQuery10->Close();
	ADOQuery10->SQL->Text = "SELECT * FROM archive_tasks WHERE usename = :usename ORDER BY archive_date DESC";
	ADOQuery10->Parameters->ParamByName("usename")->Value = user1;
	ADOQuery10->Open();

	DBGrid4->DataSource = DataSource5;
	DataSource5->DataSet = ADOQuery10;
	DBGrid4->Refresh();

			UpdateCategoriesComboBox();
		UpdateTodayTasksChecklist();
		ShowMessage("Успешно архивировано задач: " + IntToStr(deletedCount) + "!");
	}
			ADOQuery10->Open();

		if (DBGrid4->Columns->Count > 0) {
			for (int i = 0; i < DBGrid4->Columns->Count; i++) {
				TColumn* column = DBGrid4->Columns->Items[i];
				String fieldName = column->FieldName.LowerCase();

				if (fieldName.Pos("name") > 0) {
					column->Width = 150;
                }
				else if (fieldName.Pos("desc") > 0) {
					column->Width = 200;
				}
				else if (fieldName.Pos("date") > 0 || fieldName.Pos("deadline") > 0) {
					column->Width = 100;
				}
				else if (fieldName.Pos("status") > 0) {
					column->Width = 100;
				}
                else if (fieldName.Pos("category") > 0) {
					column->Width = 120;
				}
				else if (fieldName.Pos("key") > 0) {
					column->Width = 80;
				}
				else {
					column->Width = 100;
				}
			}

			DBGrid4->Refresh();
		}

		DBGrid4->Refresh();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button31Click(TObject *Sender)
{
	if (ADOQuery2->IsEmpty()) {
        ShowMessage("Нет выбранной задачи для архивации!");
		return;
    }

	String usename = ADOQuery2->FieldByName("usename")->AsString;
    String taskName = ADOQuery2->FieldByName("task_name")->AsString;
	String status = ADOQuery2->FieldByName("status")->AsString;
    String taskDesc = ADOQuery2->FieldByName("task_desc")->AsString;
	String taskCategory = ADOQuery2->FieldByName("task_category")->AsString;
	String taskDate = ADOQuery2->FieldByName("task_date")->AsString;
    String taskDeadline = ADOQuery2->FieldByName("task_deadline")->AsString;

	if (MessageDlg("Вы действительно хотите архивировать задачу: " + taskName + "?",
                   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	{
        TADOQuery *archiveQuery = new TADOQuery(this);
        archiveQuery->Connection = ADOConnection1;
		archiveQuery->SQL->Text =
			"INSERT INTO archive_tasks (usename, task_name, status, task_desc, "
            "task_category, task_date, task_deadline, archive_date) "
			"VALUES (:usename, :task_name, :status, :task_desc, "
			":task_category, :task_date, :task_deadline, :archive_date)";

        String archiveDate = FormatDateTime("yyyy.mm.dd", Date());

		archiveQuery->Parameters->ParamByName("usename")->Value = usename;
		archiveQuery->Parameters->ParamByName("task_name")->Value = taskName;
        archiveQuery->Parameters->ParamByName("status")->Value = status;
        archiveQuery->Parameters->ParamByName("task_desc")->Value = taskDesc;
        archiveQuery->Parameters->ParamByName("task_category")->Value = taskCategory;
		archiveQuery->Parameters->ParamByName("task_date")->Value = taskDate;
        archiveQuery->Parameters->ParamByName("task_deadline")->Value = taskDeadline;
        archiveQuery->Parameters->ParamByName("archive_date")->Value = archiveDate;

		archiveQuery->ExecSQL();
        delete archiveQuery;

		ADOQuery2->Delete();

        ADOQuery2->Close();
		ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "'";
        ADOQuery2->Open();

		String user = Edit2->Text;

	ADOQuery10->Close();
	ADOQuery10->SQL->Text = "SELECT * FROM archive_tasks WHERE usename = :usename ORDER BY archive_date DESC";
	ADOQuery10->Parameters->ParamByName("usename")->Value = user;
	ADOQuery10->Open();

	DBGrid4->DataSource = DataSource5;
	DataSource5->DataSet = ADOQuery10;
	DBGrid4->Refresh();

			UpdateCategoriesComboBox();
		UpdateTodayTasksChecklist();

        ShowMessage("Задача '" + taskName + "' успешно архивирована!");
	}

       ADOQuery10->Open();

		if (DBGrid4->Columns->Count > 0) {
            for (int i = 0; i < DBGrid4->Columns->Count; i++) {
                TColumn* column = DBGrid4->Columns->Items[i];
				String fieldName = column->FieldName.LowerCase();

                if (fieldName.Pos("name") > 0) {
					column->Width = 150;
                }
                else if (fieldName.Pos("desc") > 0) {
                    column->Width = 200;
				}
                else if (fieldName.Pos("date") > 0 || fieldName.Pos("deadline") > 0) {
                    column->Width = 100;
                }
				else if (fieldName.Pos("status") > 0) {
                    column->Width = 100;
                }
                else if (fieldName.Pos("category") > 0) {
					column->Width = 120;
                }
                else if (fieldName.Pos("key") > 0) {
                    column->Width = 80;
				}
                else {
                    column->Width = 100;
                }
			}

            DBGrid4->Refresh();
		}

		DBGrid4->Refresh();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button32Click(TObject *Sender)
{
    String usename = ADOQuery10->FieldByName("usename")->AsString;
    String taskName = ADOQuery10->FieldByName("task_name")->AsString;
	String status = ADOQuery10->FieldByName("status")->AsString;
    String taskDesc = ADOQuery10->FieldByName("task_desc")->AsString;
    String taskCategory = ADOQuery10->FieldByName("task_category")->AsString;
    String taskDate = ADOQuery10->FieldByName("task_date")->AsString;
	String taskDeadline = ADOQuery10->FieldByName("task_deadline")->AsString;

    if (MessageDlg("Вы действительно хотите вернуть задачу: " + taskName + " из архива?",
				   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
    {
        TADOQuery *restoreQuery = new TADOQuery(this);
		restoreQuery->Connection = ADOConnection1;
        restoreQuery->SQL->Text =
            "INSERT INTO tasks (usename, task_name, status, task_desc, "
            "task_category, task_date, task_deadline) "
			"VALUES (:usename, :task_name, :status, :task_desc, "
            ":task_category, :task_date, :task_deadline)";

		restoreQuery->Parameters->ParamByName("usename")->Value = usename;
		restoreQuery->Parameters->ParamByName("task_name")->Value = taskName;
        restoreQuery->Parameters->ParamByName("status")->Value = status;
        restoreQuery->Parameters->ParamByName("task_desc")->Value = taskDesc;
        restoreQuery->Parameters->ParamByName("task_category")->Value = taskCategory;
		restoreQuery->Parameters->ParamByName("task_date")->Value = taskDate;
        restoreQuery->Parameters->ParamByName("task_deadline")->Value = taskDeadline;

        restoreQuery->ExecSQL();
		delete restoreQuery;

        ADOQuery10->Delete();

		ADOQuery2->Close();
		ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + Edit2->Text + "'";
		ADOQuery2->Open();

			String user = Edit2->Text;

	ADOQuery10->Close();
	ADOQuery10->SQL->Text = "SELECT * FROM archive_tasks WHERE usename = :usename ORDER BY archive_date DESC";
	ADOQuery10->Parameters->ParamByName("usename")->Value = user;
	ADOQuery10->Open();

	DBGrid4->DataSource = DataSource5;
	DataSource5->DataSet = ADOQuery10;
	DBGrid4->Refresh();
		UpdateCategoriesComboBox();
		UpdateTodayTasksChecklist();

        ShowMessage("Задача '" + taskName + "' успешно возвращена из архива!");
	}
		if (DBGrid4->Columns->Count > 0) {
			for (int i = 0; i < DBGrid4->Columns->Count; i++) {
				TColumn* column = DBGrid4->Columns->Items[i];
				String fieldName = column->FieldName.LowerCase();

				if (fieldName.Pos("name") > 0) {
					column->Width = 150;
				}
				else if (fieldName.Pos("desc") > 0) {
					column->Width = 200;
				}
				else if (fieldName.Pos("date") > 0 || fieldName.Pos("deadline") > 0) {
					column->Width = 100;
				}
				else if (fieldName.Pos("status") > 0) {
					column->Width = 100;
				}
				else if (fieldName.Pos("category") > 0) {
                    column->Width = 120;
				}
				else if (fieldName.Pos("key") > 0) {
					column->Width = 80;
				}
				else {
					column->Width = 100;
				}
			}

			DBGrid4->Refresh();
		}

		DBGrid4->Refresh();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button33Click(TObject *Sender)
{

		String user = Edit2->Text;

	ADOQuery10->Close();
	ADOQuery10->SQL->Text = "SELECT * FROM archive_tasks WHERE usename = :usename ORDER BY archive_date DESC";
	ADOQuery10->Parameters->ParamByName("usename")->Value = user;
	ADOQuery10->Open();

	DBGrid4->DataSource = DataSource5;
	DataSource5->DataSet = ADOQuery10;
	DBGrid4->Refresh();
		UpdateCategoriesComboBox();
		UpdateTodayTasksChecklist();

	   ADOQuery10->Open();

		DBGrid4->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button34Click(TObject *Sender)
{

    if (ADOQuery10->IsEmpty()) {
        ShowMessage("В архиве нет задач для возврата!");
        return;
    }

    if (MessageDlg("Вы действительно хотите вернуть ВСЕ задачи из архива?\n\n"
                   "Эта операция переместит все задачи из архива обратно в основную таблицу.",
                   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
    {
        String user = Edit2->Text;

        TADOQuery *restoreQuery = new TADOQuery(this);
        restoreQuery->Connection = ADOConnection1;
        restoreQuery->SQL->Text =
            "INSERT INTO tasks (usename, task_name, status, task_desc, "
            "task_category, task_date, task_deadline) "
            "SELECT usename, task_name, status, task_desc, "
            "task_category, task_date, task_deadline "
            "FROM archive_tasks WHERE usename = :usename";

        restoreQuery->Parameters->ParamByName("usename")->Value = user;
        int restoredCount = restoreQuery->ExecSQL();
        delete restoreQuery;

        TADOQuery *deleteQuery = new TADOQuery(this);
        deleteQuery->Connection = ADOConnection1;
        deleteQuery->SQL->Text = "DELETE FROM archive_tasks WHERE usename = :usename";
        deleteQuery->Parameters->ParamByName("usename")->Value = user;
        deleteQuery->ExecSQL();
        delete deleteQuery;

        ADOQuery2->Close();
        ADOQuery2->SQL->Text = "SELECT * FROM tasks WHERE usename = '" + user + "'";
		ADOQuery2->Open();

    ADOQuery10->Close();
    ADOQuery10->SQL->Text = "SELECT * FROM archive_tasks WHERE usename = :usename ORDER BY archive_date DESC";
    ADOQuery10->Parameters->ParamByName("usename")->Value = user;
    ADOQuery10->Open();

    if (DBGrid4->Columns->Count > 0) {
        for (int i = 0; i < DBGrid4->Columns->Count; i++) {
            TColumn* column = DBGrid4->Columns->Items[i];
            String fieldName = column->FieldName.LowerCase();

            if (fieldName.Pos("name") > 0) column->Width = 150;
            else if (fieldName.Pos("desc") > 0) column->Width = 200;
            else if (fieldName.Pos("date") > 0 || fieldName.Pos("deadline") > 0) column->Width = 100;
            else if (fieldName.Pos("status") > 0) column->Width = 100;
            else if (fieldName.Pos("category") > 0) column->Width = 120;
            else if (fieldName.Pos("key") > 0) column->Width = 80;
            else column->Width = 100;
		}
		DBGrid4->Refresh();
	}
		UpdateCategoriesComboBox();
		UpdateTodayTasksChecklist();

		ShowMessage("Успешно возвращено задач из архива: " + IntToStr(restoredCount) + "!");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
	Form7->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBGrid2DrawColumnCell(TObject *Sender, const TRect &Rect,
		  int DataCol, TColumn *Column, TGridDrawState State)
{
    TDBGrid *Grid = (TDBGrid *)Sender;
    String status = Grid->DataSource->DataSet->FieldByName("status")->AsString.LowerCase();

    TColor textColor = clBlack;
    TColor backgroundColor = clWhite;

    if (Form7->RadioButton1->Checked) {
        if (status == "выполнено") {
            backgroundColor = TColor(0x00B0FFCA);
        }
        else if (status == "брошено" || status == "не выполнено") {
            backgroundColor = TColor(0x00ABABFF);
        }
        else if (status == "в процессе" || status == "планируется" || status == "отложено") {
            backgroundColor = TColor(0x00A9FCFC);
        }
	}
	else if (Form7->RadioButton2->Checked) {

		if (status == "выполнено") {
			textColor = TColor(0x003DD13F);
        }
        else if (status == "брошено" || status == "не выполнено") {
			textColor = TColor(0x001111D4);
		}
        else if (status == "в процессе" || status == "планируется" || status == "отложено") {
			textColor = TColor(0x0000DBF7);
        }
	}

    Grid->Canvas->Font->Color = textColor;
    Grid->Canvas->Brush->Color = backgroundColor;
	Grid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button28Click(TObject *Sender)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SearchBox1Change(TObject *Sender)
{
		 String searchText = SearchBox1->Text.Trim();

    if (searchText != "")
    {
        ADOQuery10->Close();
        ADOQuery10->SQL->Text =
            "SELECT * FROM archive_tasks "
            "WHERE usename = :usename "
            "AND (task_name LIKE :search_text OR task_desc LIKE :search_text OR task_category LIKE :search_text) "
            "ORDER BY archive_date DESC";

        ADOQuery10->Parameters->ParamByName("usename")->Value = Edit2->Text;
        ADOQuery10->Parameters->ParamByName("search_text")->Value = "%" + searchText + "%";
        ADOQuery10->Open();

        DBGrid4->DataSource->DataSet = ADOQuery10;
    }
    else
    {
        String user = Edit2->Text;

        ADOQuery10->Close();
        ADOQuery10->SQL->Text = "SELECT * FROM archive_tasks WHERE usename = :usename ORDER BY archive_date DESC";
        ADOQuery10->Parameters->ParamByName("usename")->Value = user;
        ADOQuery10->Open();

        DBGrid4->DataSource->DataSet = ADOQuery10;
	}
		if (DBGrid4->Columns->Count > 0) {
			for (int i = 0; i < DBGrid4->Columns->Count; i++) {
				TColumn* column = DBGrid4->Columns->Items[i];
				String fieldName = column->FieldName.LowerCase();

				if (fieldName.Pos("name") > 0) {
					column->Width = 120;
				}
				else if (fieldName.Pos("desc") > 0) {
					column->Width = 150;
				}
				else if (fieldName.Pos("date") > 0 || fieldName.Pos("deadline") > 0) {
					column->Width = 100;
				}
				else if (fieldName.Pos("status") > 0) {
					column->Width = 100;
				}
				else if (fieldName.Pos("category") > 0) {
					column->Width = 120;
				}
				else if (fieldName.Pos("key") > 0) {
					column->Width = 80;
				}
				else {
					column->Width = 100;
				}
			}

			DBGrid4->Refresh();
		}

    DBGrid4->Refresh();
}
//---------------------------------------------------------------------------

