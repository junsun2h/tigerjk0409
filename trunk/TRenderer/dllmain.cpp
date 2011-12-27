#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "RDX11Device.h"

RDX11Device g_RDevice;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C"
{
	DLL_EXPORT IRDevice* CreateDX11Renderer()
	{
		return &g_RDevice;
	}
}