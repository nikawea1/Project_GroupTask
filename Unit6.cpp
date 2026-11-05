//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "DbConfig.h"

#include <Vcl.Dialogs.hpp>
#include <Variant.hpp>
#include <ComObj.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
	: TForm(Owner)
{
	try {
		if (ADOConnection1) {
			ADOConnection1->Connected = false;
			ADOConnection1->ConnectionString = GetDatabaseConnectionString();
			ADOConnection1->Connected = true;
		}
		ADOQuery1->Open();
	}
	catch (...) {}
}
//---------------------------------------------------------------------------
void __fastcall TForm6::DateTimePicker1Change(TObject *Sender)
{

	TDateTime selectedDateTime = DateTimePicker1->DateTime;
	String selectedDate = FormatDateTime("yyyy.mm.dd", selectedDateTime);

	if (Form1->Edit2->Text == "admin")
	{
		String selectedUser = Edit1->Text.Trim();

		if (!selectedUser.IsEmpty())
		{
            ADOQuery1->SQL->Text =
				"SELECT * FROM tasks WHERE "
				"(task_date = :selectedDate OR task_deadline = :selectedDate) "
                "AND usename = :selectedUser "
                "ORDER BY task_date ASC";
			ADOQuery1->Parameters->ParamByName("selectedUser")->Value = selectedUser;
        }
		else
		{
			ADOQuery1->SQL->Text =
				"SELECT * FROM tasks WHERE "
				"task_date = :selectedDate OR task_deadline = :selectedDate "
				"ORDER BY task_date ASC";
		}
	}
	else
	{
		String currentUser = Form1->Edit2->Text;

		ADOQuery1->SQL->Text =
			"SELECT * FROM tasks WHERE "
			"(task_date = :selectedDate OR task_deadline = :selectedDate) "
			"AND usename = :currentUser "
			"ORDER BY task_date ASC";
		ADOQuery1->Parameters->ParamByName("currentUser")->Value = currentUser;
	}

	ADOQuery1->Parameters->ParamByName("selectedDate")->Value = selectedDate;
	ADOQuery1->Open();

	DBGrid1->DataSource = DataSource1;
	DataSource1->DataSet = ADOQuery1;
	DBGrid1->Refresh();

}
//---------------------------------------------------------------------------
void __fastcall TForm6::FormCreate(TObject *Sender)
{

	if (Form1->Edit2->Text == "admin") {
		Edit1->Visible = true;
		Button9->Visible = true;
		ADOQuery1->Close();
		ADOQuery1->SQL->Text = "SELECT * FROM tasks";
		ADOQuery1->Open();
		return;

	} else  {

			Edit1->Visible = false;
			Button9->Visible = false;
			Button11->Visible = false;

			String currentUser = Form1->Edit2->Text;

			ADOQuery1->Close();
			ADOQuery1->SQL->Text = "SELECT * FROM tasks WHERE usename = :username ORDER BY task_date DESC";
			ADOQuery1->Parameters->ParamByName("username")->Value = currentUser;
			ADOQuery1->Open();

			DBGrid1->Refresh();
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm6::FormClose(TObject *Sender, TCloseAction &Action)
{

	ADOQuery1->Close();
}
//---------------------------------------------------------------------------

Variant Workbook;
Variant Worksheet;
TDBGrid* DBGrid1;
String excelFilePath = "output.xlsx";

String userName;

void __fastcall TForm6::Button6Click(TObject *Sender)
{

	String currentUser = Form1->Edit2->Text;

	if (currentUser.IsEmpty()) {
		ShowMessage("Пользователь не авторизован!");
		return;
	}

	ADOQuery1->Close();
	ADOQuery1->SQL->Text = "SELECT * FROM tasks WHERE usename = :username ORDER BY task_date DESC";
	ADOQuery1->Parameters->ParamByName("username")->Value = currentUser;
	ADOQuery1->Open();

	DBGrid1->Refresh();

}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button5Click(TObject *Sender)
{

	try
	{
        if (ADOQuery1->IsEmpty())
        {
            ShowMessage("Нет данных для экспорта!");
            return;
        }

        Variant wordApp = CreateOleObject("Word.Application");
        wordApp.OlePropertySet("Visible", true);

        Variant wordDocs = wordApp.OlePropertyGet("Documents");
        Variant wordDoc = wordDocs.OleFunction("Add");
        wordDoc.OleProcedure("Activate");

        Variant wordParagraphs = wordDoc.OlePropertyGet("Paragraphs");

        Variant titlePara = wordParagraphs.OleFunction("Add");
        titlePara.OlePropertyGet("Range").OlePropertySet("Text",
            "Отчет задач за " + DateToStr(Date()));
        titlePara.OlePropertySet("Alignment", 1);
        Variant titleFont = titlePara.OlePropertyGet("Range").OlePropertyGet("Font");
        titleFont.OlePropertySet("Size", 16);
        titleFont.OlePropertySet("Bold", true);
        titleFont.OlePropertySet("Name", WideString("Times New Roman"));

        wordParagraphs.OleProcedure("Add");
        wordParagraphs.OleProcedure("Add");

        Variant tablePara = wordParagraphs.OleFunction("Item", 3);
        Variant tableRange = tablePara.OlePropertyGet("Range");
        tablePara.OlePropertySet("Alignment", 1);

        Variant wordTable = wordDoc.OlePropertyGet("Tables").OleFunction("Add",
            tableRange, ADOQuery1->RecordCount + 1, 6);

        wordTable.OlePropertySet("PreferredWidthType", 3);
        wordTable.OlePropertySet("PreferredWidth", 100);

        const int colCount = 6;
        UnicodeString headers[colCount] = {
            "Название", "Статус", "Категория", "Описание",
            "Дата создания", "Дедлайн"
        };

        for (int col = 1; col <= colCount; col++)
        {
            Variant cell = wordTable.OleFunction("Cell", 1, col);
            cell.OlePropertyGet("Range").OlePropertySet("Text", headers[col-1]);

            Variant font = cell.OlePropertyGet("Range").OlePropertyGet("Font");
            font.OlePropertySet("Bold", true);
            font.OlePropertySet("Size", 10);
            font.OlePropertySet("Name", WideString("Arial"));
            cell.OlePropertySet("VerticalAlignment", 1);
        }

        int row = 2;
        ADOQuery1->First();
        while (!ADOQuery1->Eof)
        {
            for (int col = 1; col <= colCount; col++)
            {
                Variant cell = wordTable.OleFunction("Cell", row, col);
                Variant fieldValue;

                switch(col) {
                    case 1: fieldValue = ADOQuery1->FieldByName("task_name")->AsString; break;
                    case 2: fieldValue = ADOQuery1->FieldByName("status")->AsString; break;
                    case 3: fieldValue = ADOQuery1->FieldByName("task_category")->AsString; break;
                    case 4: fieldValue = ADOQuery1->FieldByName("task_desc")->AsString; break;
                    case 5: fieldValue = ADOQuery1->FieldByName("task_date")->AsString; break;
                    case 6: fieldValue = ADOQuery1->FieldByName("task_deadline")->AsString; break;
                }

                cell.OlePropertyGet("Range").OlePropertySet("Text", fieldValue);

                Variant dataFont = cell.OlePropertyGet("Range").OlePropertyGet("Font");
                dataFont.OlePropertySet("Size", 9);
                dataFont.OlePropertySet("Name", WideString("Arial"));
                dataFont.OlePropertySet("Bold", false);
            }
            row++;
            ADOQuery1->Next();
        }

        Variant borders = wordTable.OlePropertyGet("Borders");
        borders.OlePropertySet("InsideLineStyle", 1);
        borders.OlePropertySet("OutsideLineStyle", 1);

        wordTable.OleFunction("AutoFitBehavior", 1);

        String username = ADOQuery1->FieldByName("usename")->AsString;
        TADOQuery *statsQuery = new TADOQuery(this);
        try {
            statsQuery->Connection = Form1->ADOConnection1;

            statsQuery->SQL->Text = "SELECT COUNT(*) as total FROM tasks WHERE usename = :username";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Open();
            int totalTasks = statsQuery->FieldByName("total")->AsInteger;
            statsQuery->Close();

            statsQuery->SQL->Text = "SELECT COUNT(*) as completed FROM tasks WHERE usename = :username AND status = 'Выполнено'";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Open();
            int completedTasks = statsQuery->FieldByName("completed")->AsInteger;
            statsQuery->Close();

            float completionPercentage = (totalTasks > 0) ? (completedTasks * 100.0 / totalTasks) : 0;

            statsQuery->SQL->Text = "SELECT COUNT(*) as overdue FROM tasks WHERE usename = :username AND status <> 'Выполнено' AND task_deadline < :today";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Parameters->ParamByName("today")->Value = Date();
            statsQuery->Open();
            int overdueTasks = statsQuery->FieldByName("overdue")->AsInteger;
            statsQuery->Close();

            statsQuery->SQL->Text = "SELECT COUNT(*) as inwork FROM tasks WHERE usename = :username AND (status = 'В планах' OR status = 'В процессе')";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Open();
            int inWorkTasks = statsQuery->FieldByName("inwork")->AsInteger;
            statsQuery->Close();

            String stats = "\n\nСТАТИСТИКА:\n";
            stats += "Всего задач: " + IntToStr(totalTasks) + "\n";
            stats += "Выполнено: " + IntToStr(completedTasks) + "\n";
            stats += "Процент выполнения: " + FormatFloat("0.0", completionPercentage) + "%\n";
            stats += "Просрочено: " + IntToStr(overdueTasks) + "\n";
            stats += "В работе (планы + процесс): " + IntToStr(inWorkTasks);

            wordParagraphs.OleProcedure("Add");
            Variant statsPara = wordParagraphs.OleFunction("Item", wordParagraphs.OlePropertyGet("Count"));
            statsPara.OlePropertyGet("Range").OlePropertySet("Text", stats);
            statsPara.OlePropertySet("Alignment", 1);
            Variant statsFont = statsPara.OlePropertyGet("Range").OlePropertyGet("Font");
            statsFont.OlePropertySet("Bold", true);
            statsFont.OlePropertySet("Size", 11);
        }
        __finally {
            statsQuery->Close();
            delete statsQuery;
        }

        String savePath = ExtractFilePath(Application->ExeName) +
                         "Tasks_" + username + "_" + FormatDateTime("yyyy-mm-dd", Date()) + ".docx";

        if (FileExists(savePath)) DeleteFile(savePath);

        wordDoc.OleProcedure("SaveAs", savePath);
        ShowMessage("Документ успешно сохранен:\n" + savePath);

        wordApp.OleProcedure("Quit");
    }
    catch(Exception &e)
    {
        ShowMessage("Ошибка при экспорте в Word: " + e.Message);
    }

}

void __fastcall TForm6::Button3Click(TObject *Sender)
{

    if (ADOQuery1->IsEmpty())
    {
        ShowMessage("Таблица уже пуста!");
        return;
    }

        ADOQuery1->Close();

        ADOQuery1->SQL->Text = "SELECT * FROM tasks WHERE 1=0";
        ADOQuery1->Open();

        DBGrid1->Refresh();

        ShowMessage("Список задач очищен!");

}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button4Click(TObject *Sender)
{

	if (ADOQuery1->IsEmpty()) {
		ShowMessage("Нет данных для экспорта!");
        return;
	}

	try {
        Variant excel = CreateOleObject("Excel.Application");
        excel.OlePropertySet("Visible", true);

        Variant workbooks = excel.OlePropertyGet("Workbooks");
        Variant workbook = workbooks.OleFunction("Add");
        Variant sheet = workbook.OlePropertyGet("Worksheets", 1);

        sheet.OlePropertyGet("Cells", 1, 1).OlePropertySet("Value", "Список задач");
        Variant titleRange = sheet.OlePropertyGet("Range", "A1:F1");
        titleRange.OleProcedure("Merge");
        titleRange.OlePropertyGet("Font").OlePropertySet("Bold", true);
        titleRange.OlePropertyGet("Font").OlePropertySet("Size", 14);
        titleRange.OlePropertySet("HorizontalAlignment", 3);

        UnicodeString headers[6] = {"Название", "Статус", "Категория", "Описание", "Дата создания", "Дедлайн"};

        for (int col = 0; col < 6; col++) {
            Variant cell = sheet.OlePropertyGet("Cells", 3, col+1);
            cell.OlePropertySet("Value", headers[col]);
            cell.OlePropertyGet("Font").OlePropertySet("Bold", true);
        }

        int row = 4;
        ADOQuery1->First();
        while (!ADOQuery1->Eof) {
            sheet.OlePropertyGet("Cells", row, 1).OlePropertySet("Value", ADOQuery1->FieldByName("task_name")->AsString);
            sheet.OlePropertyGet("Cells", row, 2).OlePropertySet("Value", ADOQuery1->FieldByName("status")->AsString);
            sheet.OlePropertyGet("Cells", row, 3).OlePropertySet("Value", ADOQuery1->FieldByName("task_category")->AsString);
            sheet.OlePropertyGet("Cells", row, 4).OlePropertySet("Value", ADOQuery1->FieldByName("task_desc")->AsString);
            sheet.OlePropertyGet("Cells", row, 5).OlePropertySet("Value", ADOQuery1->FieldByName("task_date")->AsString);
            sheet.OlePropertyGet("Cells", row, 6).OlePropertySet("Value", ADOQuery1->FieldByName("task_deadline")->AsString);

            row++;
            ADOQuery1->Next();
        }

        String username = ADOQuery1->FieldByName("usename")->AsString;
        TADOQuery *statsQuery = new TADOQuery(this);
        try {
            statsQuery->Connection = Form1->ADOConnection1;

            statsQuery->SQL->Text = "SELECT COUNT(*) as total FROM tasks WHERE usename = :username";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Open();
            int totalTasks = statsQuery->FieldByName("total")->AsInteger;
            statsQuery->Close();

            statsQuery->SQL->Text = "SELECT COUNT(*) as completed FROM tasks WHERE usename = :username AND status = 'Выполнено'";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Open();
            int completedTasks = statsQuery->FieldByName("completed")->AsInteger;
            statsQuery->Close();

            float completionPercentage = (totalTasks > 0) ? (completedTasks * 100.0 / totalTasks) : 0;

            statsQuery->SQL->Text = "SELECT COUNT(*) as overdue FROM tasks WHERE usename = :username AND status <> 'Выполнено' AND task_deadline < :today";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Parameters->ParamByName("today")->Value = Date();
            statsQuery->Open();
            int overdueTasks = statsQuery->FieldByName("overdue")->AsInteger;
            statsQuery->Close();

            statsQuery->SQL->Text = "SELECT COUNT(*) as inwork FROM tasks WHERE usename = :username AND (status = 'В планах' OR status = 'В процессе')";
            statsQuery->Parameters->ParamByName("username")->Value = username;
            statsQuery->Open();
            int inWorkTasks = statsQuery->FieldByName("inwork")->AsInteger;
            statsQuery->Close();

            sheet.OlePropertyGet("Cells", row + 1, 1).OlePropertySet("Value", "СТАТИСТИКА:");
            sheet.OlePropertyGet("Cells", row + 2, 1).OlePropertySet("Value", "Всего задач: " + IntToStr(totalTasks));
            sheet.OlePropertyGet("Cells", row + 3, 1).OlePropertySet("Value", "Выполнено: " + IntToStr(completedTasks));
            sheet.OlePropertyGet("Cells", row + 4, 1).OlePropertySet("Value", "Процент выполнения: " + FormatFloat("0.0", completionPercentage) + "%");
            sheet.OlePropertyGet("Cells", row + 5, 1).OlePropertySet("Value", "Просрочено: " + IntToStr(overdueTasks));
            sheet.OlePropertyGet("Cells", row + 6, 1).OlePropertySet("Value", "В работе (планы + процесс): " + IntToStr(inWorkTasks));

            Variant statsRange = sheet.OlePropertyGet("Range", "A" + IntToStr(row + 1) + ":A" + IntToStr(row + 6));
            statsRange.OlePropertyGet("Font").OlePropertySet("Bold", true);
        }
        __finally {
            statsQuery->Close();
            delete statsQuery;
        }

        Variant dataRange = sheet.OlePropertyGet("Range", "A3:F" + IntToStr(row-1));
        dataRange.OlePropertyGet("Columns").OleFunction("AutoFit");
        dataRange.OlePropertyGet("Borders").OlePropertySet("LineStyle", 1);

        String savePath = ExtractFilePath(Application->ExeName) + "Tasks_" + username + "_" + FormatDateTime("yyyy-mm-dd", Date()) + ".xlsx";

        if (FileExists(savePath)) DeleteFile(savePath);

        workbook.OleProcedure("SaveAs", savePath, 51);
        ShowMessage("Данные успешно экспортированы в Excel:\n" + savePath);

        workbook.OleProcedure("Close", false);
        excel.OleProcedure("Quit");
    }
    catch (Exception &e) {
        ShowMessage("Ошибка при экспорте в Excel: " + e.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button7Click(TObject *Sender)
{

	if (ADOQuery1->IsEmpty()) {
		ShowMessage("Нет данных для экспорта!");
        return;
    }

	TSaveDialog *saveDialog = new TSaveDialog(this);
	saveDialog->Filter = "Текстовые файлы (*.txt)|*.txt";
    saveDialog->DefaultExt = "txt";

    String username = "tasks";
    if (!ADOQuery1->IsEmpty()) {
        username = ADOQuery1->FieldByName("usename")->AsString;
    }
    saveDialog->FileName = "Tasks_" + username + "_" + FormatDateTime("yyyy-mm-dd", Date()) + ".txt";

    if (saveDialog->Execute()) {
        TStringList *sl = new TStringList();
        try {
            sl->Add("СПИСОК ЗАДАЧ");
            sl->Add("Дата экспорта: " + FormatDateTime("dd.mm.yyyy", Date()));
            sl->Add("");
            sl->Add("=================================================================");

            ADOQuery1->First();
            int taskNumber = 1;
            while (!ADOQuery1->Eof) {
                sl->Add("ЗАДАЧА #" + IntToStr(taskNumber));
                sl->Add("Название: " + ADOQuery1->FieldByName("task_name")->AsString);
                sl->Add("Статус: " + ADOQuery1->FieldByName("status")->AsString);
                sl->Add("Категория: " + ADOQuery1->FieldByName("task_category")->AsString);
                sl->Add("Описание: " + ADOQuery1->FieldByName("task_desc")->AsString);
                sl->Add("Дата создания: " + ADOQuery1->FieldByName("task_date")->AsString);
                sl->Add("Дедлайн: " + ADOQuery1->FieldByName("task_deadline")->AsString);
                sl->Add("-----------------------------------------------------------------");

                taskNumber++;
                ADOQuery1->Next();
            }

            TADOQuery *statsQuery = new TADOQuery(this);
            try {
                statsQuery->Connection = Form1->ADOConnection1;

                statsQuery->SQL->Text = "SELECT COUNT(*) as total FROM tasks WHERE usename = :username";
                statsQuery->Parameters->ParamByName("username")->Value = username;
                statsQuery->Open();
                int totalTasks = statsQuery->FieldByName("total")->AsInteger;
                statsQuery->Close();

                statsQuery->SQL->Text = "SELECT COUNT(*) as completed FROM tasks WHERE usename = :username AND status = 'Выполнено'";
                statsQuery->Parameters->ParamByName("username")->Value = username;
                statsQuery->Open();
                int completedTasks = statsQuery->FieldByName("completed")->AsInteger;
                statsQuery->Close();

                float completionPercentage = (totalTasks > 0) ? (completedTasks * 100.0 / totalTasks) : 0;

                statsQuery->SQL->Text = "SELECT COUNT(*) as overdue FROM tasks WHERE usename = :username AND status <> 'Выполнено' AND task_deadline < :today";
                statsQuery->Parameters->ParamByName("username")->Value = username;
                statsQuery->Parameters->ParamByName("today")->Value = Date();
                statsQuery->Open();
                int overdueTasks = statsQuery->FieldByName("overdue")->AsInteger;
                statsQuery->Close();

                statsQuery->SQL->Text = "SELECT COUNT(*) as inwork FROM tasks WHERE usename = :username AND (status = 'В планах' OR status = 'В процессе')";
                statsQuery->Parameters->ParamByName("username")->Value = username;
                statsQuery->Open();
                int inWorkTasks = statsQuery->FieldByName("inwork")->AsInteger;
                statsQuery->Close();

                sl->Add("");
                sl->Add("СТАТИСТИКА:");
                sl->Add("Всего задач: " + IntToStr(totalTasks));
                sl->Add("Выполнено: " + IntToStr(completedTasks));
                sl->Add("Процент выполнения: " + FormatFloat("0.0", completionPercentage) + "%");
                sl->Add("Просрочено: " + IntToStr(overdueTasks));
                sl->Add("В работе (планы + процесс): " + IntToStr(inWorkTasks));
            }
            __finally {
                statsQuery->Close();
                delete statsQuery;
            }

            sl->SaveToFile(saveDialog->FileName);
            ShowMessage("Данные успешно экспортированы в TXT:\n" + saveDialog->FileName);
        }
        __finally {
            delete sl;
            delete saveDialog;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button8Click(TObject *Sender)
{
    if (ADOQuery1->IsEmpty()) {
        ShowMessage("Нет данных для статистики!");
        return;
    }

    String username = "";
    if (!ADOQuery1->IsEmpty()) {
        username = ADOQuery1->FieldByName("usename")->AsString;
    }

    TADOQuery *statsQuery = new TADOQuery(this);
    try {
        statsQuery->Connection = Form1->ADOConnection1;

        statsQuery->SQL->Text = "SELECT COUNT(*) as total FROM tasks WHERE usename = :username";
        statsQuery->Parameters->ParamByName("username")->Value = username;
        statsQuery->Open();
        int totalTasks = statsQuery->FieldByName("total")->AsInteger;
        statsQuery->Close();

        statsQuery->SQL->Text = "SELECT COUNT(*) as completed FROM tasks WHERE usename = :username AND status = 'Выполнено'";
        statsQuery->Parameters->ParamByName("username")->Value = username;
        statsQuery->Open();
        int completedTasks = statsQuery->FieldByName("completed")->AsInteger;
        statsQuery->Close();

        float completionPercentage = (totalTasks > 0) ? (completedTasks * 100.0 / totalTasks) : 0;

        statsQuery->SQL->Text = "SELECT COUNT(*) as overdue FROM tasks WHERE usename = :username AND status <> 'Выполнено' AND task_deadline < :today";
        statsQuery->Parameters->ParamByName("username")->Value = username;
        statsQuery->Parameters->ParamByName("today")->Value = Date();
        statsQuery->Open();
        int overdueTasks = statsQuery->FieldByName("overdue")->AsInteger;
        statsQuery->Close();

        statsQuery->SQL->Text = "SELECT COUNT(*) as inwork FROM tasks WHERE usename = :username AND (status = 'В планах' OR status = 'В процессе')";
        statsQuery->Parameters->ParamByName("username")->Value = username;
        statsQuery->Open();
        int inWorkTasks = statsQuery->FieldByName("inwork")->AsInteger;
        statsQuery->Close();

        String statsText = "СТАТИСТИКА ПОЛЬЗОВАТЕЛЯ: " + username + "\n\n";
        statsText += "Всего задач: " + IntToStr(totalTasks) + "\n";
        statsText += "Выполнено: " + IntToStr(completedTasks) + "\n";
        statsText += "Процент выполнения: " + FormatFloat("0.0", completionPercentage) + "%\n";
        statsText += "Просрочено: " + IntToStr(overdueTasks) + "\n";
        statsText += "В работе (планы + процесс): " + IntToStr(inWorkTasks);

        ShowMessage(statsText);

    }
    __finally {
        statsQuery->Close();
        delete statsQuery;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button9Click(TObject *Sender)
{
	if (Form1->Edit2->Text != "admin") {
		ShowMessage("Только администратор может загружать задачи других пользователей!");
		return;
	}

	String username = Edit1->Text.Trim();
	if (username.IsEmpty()) {
		ShowMessage("Введите имя пользователя!");
		return;
	}

	TADOQuery *checkQuery = new TADOQuery(this);
	try {
		checkQuery->Connection = Form1->ADOConnection1;
		checkQuery->SQL->Text = "SELECT username FROM users WHERE username = :username";
		checkQuery->Parameters->ParamByName("username")->Value = username;
		checkQuery->Open();

		if (checkQuery->IsEmpty()) {
			ShowMessage("Пользователь '" + username + "' не найден!");
		} else {
			ADOQuery1->Close();
			ADOQuery1->SQL->Text = "SELECT * FROM tasks WHERE usename = :username ORDER BY task_date DESC";
			ADOQuery1->Parameters->ParamByName("username")->Value = username;
			ADOQuery1->Open();

			ShowMessage("Задачи пользователя '" + username + "' загружены!");
		}
	}
	__finally {
		checkQuery->Close();
		delete checkQuery;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button10Click(TObject *Sender)
{

    String currentUser = Form1->Edit2->Text;

    if (currentUser.IsEmpty()) {
        ShowMessage("Пользователь не авторизован!");
        return;
    }

    if (MessageDlg("Загрузить ВСЕ задачи пользователя из архива? Текущий список будет очищен.",
        mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes) {

        ADOQuery1->Close();
        ADOQuery1->SQL->Text = "SELECT * FROM archive_tasks WHERE usename = :username ORDER BY archive_date DESC";
        ADOQuery1->Parameters->ParamByName("username")->Value = currentUser;
        ADOQuery1->Open();

        DBGrid1->DataSource = DataSource1;
        DataSource1->DataSet = ADOQuery1;
        DBGrid1->Refresh();

        ShowMessage("Архив задач загружен! Загружено задач: " + IntToStr(ADOQuery1->RecordCount));
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button11Click(TObject *Sender)
{
		ADOQuery1->Close();
		ADOQuery1->SQL->Text = "SELECT * FROM tasks";
		ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button1Click(TObject *Sender)
{
    if (ADOQuery1->IsEmpty())
    {
        ShowMessage("Нет записей для удаления.");
        return;
    }

    ADOQuery1->Delete();

	DBGrid1->Refresh();
}
//---------------------------------------------------------------------------

