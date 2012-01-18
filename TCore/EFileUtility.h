#pragma once

#include "IFileUtility.h"


class EFileUtility : public IFileUtility
{
public:
	void	GetExt(const char* str, char* pDest) override;
	void	GetExt(const wchar_t* str, wchar_t* pDest) override;
			
	void	GetFileName(const char* str, char* pDest) override;
	void	GetFileName(const wchar_t* str, wchar_t* pDest) override;
			
	void	GetDirectory(const char* str, char* pDest) override;
	void	GetDirectory(const wchar_t* str, wchar_t* pDest) override;
		 
	bool	CollectFileList(const char* path, CSTRING_LIST* pStringList) override;
	bool	CollectFileList(const wchar_t* path, CSTRING_LIST* pStringList) override;
};