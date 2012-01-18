#include "EFileUtility.h"
#include "EEngine.h"

struct CFileInfo
{
	char dir[64];
	char name[64];
};

bool CollectFile(const wchar_t* path, CSTRING_LIST* pStringList)
{
	WIN32_FIND_DATAW wfd;
	BOOL bResult=TRUE;

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[MAX_PATH];
	wchar_t filename[MAX_PATH];
	wchar_t ext[8];
	char buf[MAX_PATH];

	wchar_t newpath[MAX_PATH];

	HANDLE hSrch = FindFirstFile( path, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE) 
		return false;

	_wsplitpath_s(path, drive, _MAX_DRIVE, dir, MAX_PATH, filename, MAX_PATH, ext, 8);

	while (bResult) 
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			if ( wcsncmp( wfd.cFileName, L".", MAX_PATH) && wcsncmp( wfd.cFileName, L"..", MAX_PATH) ) 
			{
				swprintf_s(newpath, MAX_PATH, L"%s%s%s\\*.*", drive, dir, wfd.cFileName);
				CollectFile(newpath, pStringList);
			}
		} 
		else 
		{
			size_t size;
			swprintf_s(filename, MAX_PATH, L"%s%s%s", drive, dir, wfd.cFileName);
			wcstombs_s(&size, buf, filename, MAX_PATH);
			pStringList->push_back( buf );
		}

		bResult = FindNextFile( hSrch, &wfd );
	}

	FindClose(hSrch);

	return true;
}

//----------------------------------------------------------------------------------------------------------------
void EFileUtility::GetExt(const char* str, char* pDest)
{
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char filename[MAX_PATH];
	
	_splitpath_s(str, drive, MAX_PATH, dir, MAX_PATH, filename, MAX_PATH, pDest, MAX_PATH);
}

void EFileUtility::GetExt(const wchar_t* str, wchar_t* pDest)
{
	wchar_t drive[MAX_PATH];
	wchar_t dir[MAX_PATH];
	wchar_t filename[MAX_PATH];
	
	_wsplitpath_s(str, drive, MAX_PATH, dir, MAX_PATH, filename, MAX_PATH, pDest, MAX_PATH);
}

void EFileUtility::GetFileName(const char* str, char* pDest)
{
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char ext[MAX_PATH];

	_splitpath_s(str, drive, MAX_PATH, dir, MAX_PATH, pDest, MAX_PATH, ext, MAX_PATH);
}

void EFileUtility::GetFileName(const wchar_t* str, wchar_t* pDest)
{
	wchar_t drive[MAX_PATH];
	wchar_t dir[MAX_PATH];
	wchar_t ext[MAX_PATH];

	_wsplitpath_s(str, drive, MAX_PATH, dir, MAX_PATH, pDest, MAX_PATH, ext, MAX_PATH);
}

void EFileUtility::GetDirectory(const char* str, char* pDest)
{
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char filename[MAX_PATH];
	char ext[MAX_PATH];

	_splitpath_s(str, drive, MAX_PATH, dir, MAX_PATH, filename, MAX_PATH, ext, MAX_PATH);
	sprintf_s( pDest, MAX_PATH, "%s%s", drive, dir);
}

void EFileUtility::GetDirectory(const wchar_t* str, wchar_t* pDest)
{
	wchar_t drive[MAX_PATH];
	wchar_t dir[MAX_PATH];
	wchar_t filename[MAX_PATH];
	wchar_t ext[MAX_PATH];

	_wsplitpath_s(str, drive, MAX_PATH, dir, MAX_PATH, filename, MAX_PATH, ext, MAX_PATH);
	swprintf_s( pDest, MAX_PATH, _T("%s%s"), drive, dir);
}

bool EFileUtility::CollectFileList(const char* path, CSTRING_LIST* pStringList)
{
	return true;
}

bool EFileUtility::CollectFileList(const wchar_t* path, CSTRING_LIST* pStringList)
{
	return CollectFile(path, pStringList);
}