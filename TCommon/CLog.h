#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>

#define TLOG_BUF_LEN		1024
#define TLOG_FILE			"TLog.txt"

namespace CLOG
{
	void InitLogSystem()
	{
		FILE *pFile;
		
		if( fopen_s( &pFile, TLOG_FILE, "w") != 0)
			return;

		if (pFile)
		{
			static unsigned char UTF8_BOM[3] = { 0xEF, 0xBB, 0xBF};

			fwrite(UTF8_BOM, sizeof(UTF8_BOM), 1, pFile);
		}

		char szTime[64]="";
		_strtime_s( szTime );

		fprintf(pFile, "------------------------------------------------------------------------------\n");
		fprintf(pFile, "-- Logging Start :%s)\n", szTime);
		fprintf(pFile, "------------------------------------------------------------------------------\n");

		fclose(pFile);
	}


	inline void Write(const char* szText)
	{
		FILE* pFile;
		if( fopen_s( &pFile, TLOG_FILE, "a") != 0)
			return;

		char szTime[64]="";
		_strtime_s( szTime );

		fprintf_s(pFile, "%s - %s\n", szTime, szText);

		fflush(pFile);
		fclose(pFile);
	}

	inline void Log(const char *pFormat, ...)
	{
		char temp[TLOG_BUF_LEN];

		va_list args;
		va_start(args,pFormat);
		vsprintf_s(temp,pFormat,args);
		va_end(args);

		Write(temp);
	}

	inline void Log( const wchar_t *pFormat, ... )
	{
		WCHAR temp[TLOG_BUF_LEN];

		va_list args;
		va_start(args,pFormat);
		vswprintf_s( temp, TLOG_BUF_LEN, pFormat, args );
		va_end(args);

		// Convert to a char*
		size_t convertedChars = 0;
		char nstring[TLOG_BUF_LEN];
		wcstombs_s(&convertedChars, nstring, wcslen(temp) + 1, temp, _TRUNCATE);
		strcat_s(nstring, " (char *)");

		Write( nstring );
	}
}