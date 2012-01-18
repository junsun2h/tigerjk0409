#pragma once

#include "CDefine.h"
#include <vector>
#include <map>


typedef std::vector<std::string>			CSTRING_LIST;
typedef	std::map<std::string, std::string>	CSTRING_MAP;	// filename, fullpath


struct IFileUtility
{
	virtual void			GetExt(const char* str, char* pDest) = 0;
	virtual void			GetExt(const wchar_t* str, wchar_t* pDest) = 0;

	virtual void			GetFileName(const char* str, char* pDest) = 0;
	virtual void			GetFileName(const wchar_t* str, wchar_t* pDest) = 0;

	virtual void			GetDirectory(const char* str, char* pDest) = 0;
	virtual void			GetDirectory(const wchar_t* str, wchar_t* pDest) = 0;

	virtual bool			CollectFileList(const char* path, CSTRING_LIST* pStringList) = 0;
	virtual bool			CollectFileList(const wchar_t* path, CSTRING_LIST* pStringList) = 0;
};