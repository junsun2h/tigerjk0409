#pragma once

#include <dbghelp.h>

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
	HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

class CCrashDump
{
public:
	static void Init()
	{
		::SetUnhandledExceptionFilter(ExceptionFilter);
	}

private:
	static void GetDumpFileName( char pszFileName[] )
	{
		char szDumpFileName[MAX_PATH];
		char szFilename[MAX_PATH];
		::GetModuleFileNameA(NULL, szFilename, MAX_PATH);
		char* szdot = strrchr(szFilename, '.');
		::lstrcpynA(szDumpFileName, szFilename, (int)(szdot - szFilename + 1));

		SYSTEMTIME time;
		::GetLocalTime(&time);
		sprintf( pszFileName, "%d%02d%02d_%02d%02d%02d.dmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	}

	static long WINAPI ExceptionFilter(_EXCEPTION_POINTERS *pException)
	{
		char szDumpFileName[MAX_PATH];
		GetDumpFileName(szDumpFileName);
		if( !BuildDump(szDumpFileName, pException) )
			return EXCEPTION_CONTINUE_SEARCH;

		return EXCEPTION_EXECUTE_HANDLER;
	}

	static bool BuildDump( const char* pszDumpFileName, _EXCEPTION_POINTERS* pException )
	{
		char szDllHelpPath[MAX_PATH] = "";
		HMODULE hDLL = NULL;

		// DLL ·Îµù
		if(::GetModuleFileNameA(NULL, szDllHelpPath, _MAX_PATH))
		{
			char *pSlash = ::strrchr(szDllHelpPath, '\\');
			if( pSlash )
			{
				::lstrcpyA(pSlash + 1, "DBGHELP.DLL");
				hDLL = ::LoadLibraryA(szDllHelpPath);
			}
		}

		if( NULL == hDLL || INVALID_HANDLE_VALUE == hDLL )
		{
			MessageBoxA(NULL, "cannot open file DBGHELP.DLL", "ERROR", MB_OK);
			return false;
		}

		MINIDUMPWRITEDUMP pMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");
		if( pMiniDumpWriteDump == NULL )
		{
			return false;
		}

		HANDLE hFile = CreateFileA(
			pszDumpFileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if( INVALID_HANDLE_VALUE == hFile )	
		{
			return false;
		}

		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = pException;
		eInfo.ClientPointers = FALSE;

		const char*	szFaultReason;
		szFaultReason = pException ? GetFaultReason(pException):"UserDump";

		MINIDUMP_USER_STREAM_INFORMATION sUserInfo; 
		MINIDUMP_USER_STREAM m_vUseData[2];

		m_vUseData[0].Type = 0;
		m_vUseData[0].Buffer = (PVOID)szFaultReason;
		m_vUseData[0].BufferSize = strlen(szFaultReason);

		sUserInfo.UserStreamCount = 2; 
		sUserInfo.UserStreamArray = m_vUseData;

		bool bDumpped = false;
		if (pMiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			MiniDumpNormal,
			pException ? &eInfo : NULL,
			&sUserInfo,
			NULL) )
		{
			bDumpped = true;
		}

		CloseHandle(hFile);
		FreeLibrary(hDLL);

		return bDumpped;
	}

	static char* GetFaultReason(_EXCEPTION_POINTERS* pExPtrs)
	{
		if( NULL == pExPtrs || NULL == pExPtrs->ExceptionRecord || NULL ==  pExPtrs->ExceptionRecord->ExceptionCode )
			return "Unknown";

		if (::IsBadReadPtr(pExPtrs, sizeof(EXCEPTION_POINTERS))) 
			return "BAD EXCEPTION POINTERS";

		//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/exception_record_str.asp
		switch( pExPtrs->ExceptionRecord->ExceptionCode )
		{
		case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
		case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";
		case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";
		case EXCEPTION_GUARD_PAGE:               return "EXCEPTION_GUARD_PAGE";
		case EXCEPTION_INVALID_HANDLE:           return "EXCEPTION_INVALID_HANDLE";
		case 0xE06D7363:                         return "Microsoft C++ Exception";
		default:
			break;
		}

		return "Unknown";
	}
};