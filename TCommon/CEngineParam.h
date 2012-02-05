#pragma once

struct CENGINE_INIT_PARAM
{
	CENGINE_INIT_PARAM()
		: hWnd(NULL)
		, width(-1)
		, height(-1)
		, bFullScreen(false)
		, numOfProcessThread(1)
		, resourceFolder(NULL)
	{
	}	

	void*			hWnd;
	int				width;
	int				height;
	bool			bFullScreen;
	UINT			numOfProcessThread;
	const wchar_t*	resourceFolder;
};
