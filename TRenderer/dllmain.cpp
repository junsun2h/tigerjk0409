#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <CGuid.h>

#include "CQuad.h"

#include "IRDevice.h"

#include "RDefine.h"
#include "IRDX11Device.h"

#include "RDX11Global.h"


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
		return GLOBAL::RDevice();
	}
}