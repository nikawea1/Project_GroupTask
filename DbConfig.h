#ifndef DbConfigH
#define DbConfigH

#include <vcl.h>

inline String GetDatabaseConnectionString()
{
	String exeDir = ExtractFilePath(Application->ExeName);
	String candidates[] = {
		exeDir + "планер_задач_пробник.mdb",
		exeDir + "..\\..\\планер_задач_пробник.mdb",
		exeDir + "..\\планер_задач_пробник.mdb",
		exeDir + "Database\\планер_задач_пробник.mdb",
		ExtractFilePath(ParamStr(0)) + "планер_задач_пробник.mdb"
	};

	String dbPath = "";
	for (int i = 0; i < 5; ++i) {
		if (FileExists(candidates[i])) {
			dbPath = ExpandFileName(candidates[i]);
			break;
		}
	}

	if (dbPath.IsEmpty()) {
		dbPath = ExpandFileName(exeDir + "планер_задач_пробник.mdb");
	}

	return "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dbPath + ";Persist Security Info=False";
}

#endif
