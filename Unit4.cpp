//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include <string>
#include "DbConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
	try {
		if (ADOConnection1) {
			ADOConnection1->Connected = false;
			ADOConnection1->ConnectionString = GetDatabaseConnectionString();
			ADOConnection1->Connected = true;
		}
	}
	catch (...) {}
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button1Click(TObject *Sender)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button2Click(TObject *Sender)
{
}

//---------------------------------------------------------------------------

void __fastcall TForm4::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form2->Close();
	Form3->Close();
	Form5->Close();
    Form6->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::DBGrid1CellClick(TColumn *Column)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::DBGrid1ColEnter(TObject *Sender)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormCreate(TObject *Sender)
{

	srand(time(NULL));
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormShow(TObject *Sender)
{
	String username = Form1->Edit2->Text;
    Caption = "Статистика пользователя: " + username;

	StringGrid1->Visible = false;
	Image1->Visible = false;
	ProgressBar1->Visible = false;
	ProgressBar2->Visible = false;
	ProgressBar3->Visible = false;
	ProgressBar4->Visible = false;
	Label1->Visible = false;
    Label2->Visible = false;
	Label3->Visible = false;
    Label4->Visible = false;
	Label5->Visible = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button4Click(TObject *Sender)
{
   String username = Form1->Edit2->Text;

    Image1->Picture = NULL;
    Image1->Canvas->Brush->Color = clWhite;
	Image1->Canvas->FillRect(Rect(0, 0, Image1->Width, Image1->Height));

    TADOQuery *query = new TADOQuery(this);
    try {
        query->Connection = Form1->ADOConnection1;

        query->SQL->Text =
			"SELECT status, COUNT(*) as task_count "
            "FROM tasks WHERE usename = :username "
            "GROUP BY status";
        query->Parameters->ParamByName("username")->Value = username;
        query->Open();

        int totalTasks = 0;
        TStringList *statusList = new TStringList();
        TList *countList = new TList();

        while (!query->Eof) {
            String status = query->FieldByName("status")->AsString;
            int count = query->FieldByName("task_count")->AsInteger;

            statusList->Add(status);
            int *countPtr = new int(count);
            countList->Add(countPtr);
            totalTasks += count;
            query->Next();
        }

        if (totalTasks > 0 && statusList->Count > 0) {
			Image1->Canvas->Font->Name = "Century Gothic";
			Image1->Canvas->Font->Size = 7;

            int barWidth = 50;
            int spacing = 25;
            int startX = 80;
            int startY = Image1->Height - 100;
            int maxHeight = 200;

            int maxCount = 0;
            for (int i = 0; i < countList->Count; i++) {
                int count = *(int*)countList->Items[i];
                if (count > maxCount) maxCount = count;
            }

            Image1->Canvas->Pen->Color = clBlack;
            Image1->Canvas->Pen->Width = 2;
            Image1->Canvas->MoveTo(startX - 10, startY + 10);
            Image1->Canvas->LineTo(startX + (barWidth + spacing) * statusList->Count, startY + 10);
            Image1->Canvas->MoveTo(startX - 10, startY + 10);
            Image1->Canvas->LineTo(startX - 10, startY - maxHeight - 10);

            for (int i = 0; i < statusList->Count; i++) {
                String status = statusList->Strings[i];
                int count = *(int*)countList->Items[i];
                int barHeight = (count * maxHeight) / maxCount;

                TColor barColor;
                if (status == "Выполнено") barColor = TColor(0x98FB98);
                else if (status == "В процессе") barColor = TColor(0xFFFACD);
                else if (status == "Просрочено") barColor = TColor(0xFFB6C1);
                else if (status == "В планах") barColor = TColor(0xADD8E6);
                else barColor = TColor(0xD3D3D3);

                int x = startX + i * (barWidth + spacing);

				Image1->Canvas->Brush->Color = barColor;
				 Image1->Canvas->Font->Size = 6;
                Image1->Canvas->Pen->Color = clBlack;
                Image1->Canvas->Pen->Width = 1;
                Image1->Canvas->Rectangle(x, startY - barHeight, x + barWidth, startY);

                Image1->Canvas->Font->Color = clBlack;
                Image1->Canvas->Brush->Style = bsClear;
                String countText = IntToStr(count);
                int textWidth = Image1->Canvas->TextWidth(countText);
                Image1->Canvas->TextOut(x + (barWidth - textWidth) / 2, startY - barHeight - 15, countText);

                String statusText = status;
                if (statusText.Length() > 8) {
                    statusText = statusText.SubString(1, 7) + "..";
                }
                textWidth = Image1->Canvas->TextWidth(statusText);
                Image1->Canvas->TextOut(x + (barWidth - textWidth) / 2, startY + 15, statusText);
            }

            Image1->Canvas->Font->Size = 12;
            Image1->Canvas->Font->Style = TFontStyles() << fsBold;
            Image1->Canvas->TextOut(50, 20, "Статистика задач - " + username);
            Image1->Canvas->Font->Size = 10;
            Image1->Canvas->Font->Style = TFontStyles();
            Image1->Canvas->TextOut(50, 40, "Всего задач: " + IntToStr(totalTasks));
        }
        else {
            Image1->Canvas->Font->Size = 12;
            Image1->Canvas->TextOut(50, 50, "Нет данных для пользователя: " + username);
        }

        for (int i = 0; i < countList->Count; i++) {
            delete (int*)countList->Items[i];
        }
        delete countList;
        delete statusList;
    }
    __finally {
        query->Close();
        delete query;
    }

    Image1->Visible = true;
    StringGrid1->Visible = false;
    ProgressBar1->Visible = false;
    ProgressBar2->Visible = false;
    ProgressBar3->Visible = false;
    ProgressBar4->Visible = false;
    Label1->Visible = false;
    Label2->Visible = false;
    Label3->Visible = false;
    Label4->Visible = false;
	Label5->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button6Click(TObject *Sender)
{
	String username = Form1->Edit2->Text;

	TADOQuery *query = new TADOQuery(this);
	try {
        query->Connection = Form1->ADOConnection1;

        query->SQL->Text = "SELECT * FROM tasks WHERE usename = :username";
        query->Parameters->ParamByName("username")->Value = username;
        query->Open();

        if (!query->IsEmpty()) {
			query->Close();
            query->SQL->Text =
				"SELECT status, COUNT(*) as task_count "
				"FROM tasks WHERE usename = :username "
                "GROUP BY status";
            query->Parameters->ParamByName("username")->Value = username;
            query->Open();

            int totalTasks = 0;

            int completed = 0;
			int inProgress = 0;
			int overdue = 0;
			int planned = 0;

            query->First();
            while (!query->Eof) {
                String status = query->FieldByName("status")->AsString;
                int count = query->FieldByName("task_count")->AsInteger;

				if (status == "выполнено")
                    completed = count;
				else if (status == "в процессе")
                    inProgress = count;
				else if (status == "не выполнено")
                    overdue = count;
				else if (status == "планируется")
					planned = count;

                totalTasks += count;
                query->Next();
            }

            if (totalTasks > 0) {
                int completedPercent = (completed * 100) / totalTasks;
                int inProgressPercent = (inProgress * 100) / totalTasks;
				int overduePercent = (overdue * 100) / totalTasks;
                int plannedPercent = (planned * 100) / totalTasks;

                ProgressBar1->Position = completedPercent;
                ProgressBar2->Position = inProgressPercent;
                ProgressBar3->Position = overduePercent;
                ProgressBar4->Position = plannedPercent;

                Label1->Caption = "Выполнено: " + IntToStr(completed) + " (" + IntToStr(completedPercent) + "%)";
                Label2->Caption = "В процессе: " + IntToStr(inProgress) + " (" + IntToStr(inProgressPercent) + "%)";
				Label3->Caption = "Не выполнено: " + IntToStr(overdue) + " (" + IntToStr(overduePercent) + "%)";
				Label4->Caption = "В планах: " + IntToStr(planned) + " (" + IntToStr(plannedPercent) + "%)";

                Label5->Caption = "Всего задач: " + IntToStr(totalTasks);
            }
            else {
                Label1->Caption = "Нет данных";
                ProgressBar1->Position = 0;
                ProgressBar2->Position = 0;
                ProgressBar3->Position = 0;
                ProgressBar4->Position = 0;
            }
        }
        else {
            Label1->Caption = "Нет задач";
            ProgressBar1->Position = 0;
            ProgressBar2->Position = 0;
            ProgressBar3->Position = 0;
            ProgressBar4->Position = 0;
        }
    }
    __finally {
        query->Close();
        delete query;
    }

    ProgressBar1->Visible = true;
    ProgressBar2->Visible = true;
    ProgressBar3->Visible = true;
    ProgressBar4->Visible = true;
    Label1->Visible = true;
    Label2->Visible = true;
    Label3->Visible = true;
    Label4->Visible = true;
    Label5->Visible = true;

    Image1->Visible = false;
	StringGrid1->Visible = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button5Click(TObject *Sender)
{
	String username = Form1->Edit2->Text;

	Image1->Picture = NULL;
    Image1->Canvas->Brush->Color = clWhite;
    Image1->Canvas->FillRect(Rect(0, 0, Image1->Width, Image1->Height));

    TADOQuery *query = new TADOQuery(this);
    try {
		query->Connection = Form1->ADOConnection1;

        query->SQL->Text =
            "SELECT status, COUNT(*) as task_count "
			"FROM tasks WHERE usename = :username "
            "GROUP BY status";
        query->Parameters->ParamByName("username")->Value = username;
        query->Open();

		int totalTasks = 0;
        TStringList *statusList = new TStringList();
        TList *countList = new TList();

        while (!query->Eof) {
            String status = query->FieldByName("status")->AsString;
			int count = query->FieldByName("task_count")->AsInteger;

            statusList->Add(status);
            int *countPtr = new int(count);
            countList->Add(countPtr);
            totalTasks += count;
			query->Next();
        }

        if (totalTasks > 0 && statusList->Count > 0) {
			Image1->Canvas->Font->Name = "Century Gothic";
			Image1->Canvas->Font->Size = 5;
            int centerX = Image1->Width / 2;
			int centerY = Image1->Height / 2;
            int radius = 120;

            float startAngle = 0.0;

            TColor colors[] = {
				TColor(0xFFCDD2),
				TColor(0xC8E6C9),
				TColor(0xB3E5FC),
				TColor(0xFFF9C4),
                TColor(0xE1BEE7),
                TColor(0x00FFFF),
				TColor(0xFFA500),
                TColor(0x800080)
			};

            for (int i = 0; i < statusList->Count; i++) {
                int count = *(int*)countList->Items[i];
                float angle = (count * 360.0) / totalTasks;

                TColor segmentColor = colors[i % 8];

				Image1->Canvas->Font->Size = 5;
                Image1->Canvas->Brush->Color = segmentColor;
                Image1->Canvas->Pen->Color = clBlack;
                Image1->Canvas->Pen->Width = 2;

                Image1->Canvas->Pie(
                    centerX - radius, centerY - radius,
                    centerX + radius, centerY + radius,
                    centerX + (int)(radius * cos(startAngle * M_PI / 180)),
                    centerY - (int)(radius * sin(startAngle * M_PI / 180)),
                    centerX + (int)(radius * cos((startAngle + angle) * M_PI / 180)),
                    centerY - (int)(radius * sin((startAngle + angle) * M_PI / 180))
                );

                int legendX = 30;
                int legendY = 50 + i * 25;

                Image1->Canvas->Brush->Color = segmentColor;
                Image1->Canvas->Rectangle(legendX, legendY, legendX + 20, legendY + 15);

                Image1->Canvas->Brush->Color = clWhite;
                Image1->Canvas->Font->Color = clBlack;
				Image1->Canvas->Font->Size = 7;

                String legendText = statusList->Strings[i] + " - " + IntToStr(count) +
                                  " (" + FormatFloat("0.0", (count * 100.0) / totalTasks) + "%)";
                Image1->Canvas->TextOut(legendX + 25, legendY, legendText);

                startAngle += angle;
            }

            Image1->Canvas->Font->Size = 12;
			Image1->Canvas->Font->Style = TFontStyles() << fsBold;
			Image1->Canvas->Font->Name = "Century Gothic";
            Image1->Canvas->TextOut(centerX - 100, 20, "Распределение задач - " + username);
        }
        else {
            Image1->Canvas->Font->Size = 12;
            Image1->Canvas->TextOut(50, 50, "Нет данных для пользователя: " + username);
        }

        for (int i = 0; i < countList->Count; i++) {
            delete (int*)countList->Items[i];
        }
        delete countList;
        delete statusList;
    }
    __finally {
        query->Close();
        delete query;
    }

    Image1->Visible = true;
    StringGrid1->Visible = false;
    ProgressBar1->Visible = false;
    ProgressBar2->Visible = false;
    ProgressBar3->Visible = false;
    ProgressBar4->Visible = false;
    Label1->Visible = false;
    Label2->Visible = false;
    Label3->Visible = false;
    Label4->Visible = false;
	Label5->Visible = false;
}
//---------------------------------------------------------------------------

