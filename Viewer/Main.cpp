#include <Windows.h>
#include "IEngine.h"


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

HWND gHwnd = NULL;
IEngine* gEng = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_CLOSE:
		{
			HMENU hMenu;
			hMenu = GetMenu(hWnd);
			if( hMenu != NULL )
				DestroyMenu( hMenu );
			DestroyWindow( hWnd );
			return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

bool InitWinApp(HINSTANCE hInstance, int nCmdShow, int width, int height, LPCWSTR appName)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= appName;
	wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE; //WS_MINIMIZEBOX
	DWORD dwEXStyle = WS_EX_CONTROLPARENT;

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	AdjustWindowRectEx(&rect, dwStyle, true, dwEXStyle);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	gHwnd = CreateWindow(appName, appName, dwStyle,	100, 100, w, h-20, NULL, NULL, hInstance, NULL);

	if (!gHwnd)
		return FALSE;

	ShowWindow(gHwnd, nCmdShow);
	UpdateWindow(gHwnd);

	return TRUE;
}

bool InitEngine(HINSTANCE hInstance)
{
	CENGINE_INIT_PARAM engineParam;
	engineParam.hInstance = hInstance;
	engineParam.hWnd = gHwnd;
	engineParam.width = WIN_WIDTH;
	engineParam.height = WIN_HEIGHT;

	HMODULE	engineDll = ::LoadLibrary( L"Engine.dll");
	if( engineDll == NULL )
		return false;

	typedef IEngine *(*CREATE_ENGINE)();
	CREATE_ENGINE FuncCreateEngine = NULL;

	FuncCreateEngine = (CREATE_ENGINE)::GetProcAddress( engineDll, "CreateEngine" );

	if( FuncCreateEngine == NULL )
		return 0;

	gEng = FuncCreateEngine();
	gEng->StartUp( engineParam );

	return true;
}

bool SetupViewer()
{
	return true;
}

void MainLoop()
{
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message  )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			gEng->GetRenderer()->Render(0);
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpcmdline, int nCmdShow)
{
#ifdef _DEBUG
	AllocConsole();
	//	freopen_s(NULL, "CONOUT$", "w", stdout);
#endif

	if ( !InitWinApp(hInstance, nCmdShow, WIN_WIDTH, WIN_HEIGHT, L"exampleWorld") )
		return FALSE;

	if ( !InitEngine(hInstance) )
		return FALSE;

	//	if( !SetupViewer()  )
	//		return FALSE;

	MainLoop();

	return 0;
}