#include "EEngine.h"
#include "IRDevice.h"

#include <windows.h>



EEngine::EEngine()
	: m_pRenderer(NULL)
{

}

EEngine::~EEngine()
{

}

bool EEngine::StartUp(const CENGINE_INIT_PARAM &param)
{
	//////////////////////////////////////////////////////////////////////////
	// load Render DLL
	HMODULE	renderDll = ::LoadLibrary( L"Renderer.dll");
	if( renderDll == NULL )
	{
		assert(0);
		return false;
	}

	typedef IRDevice *(*CREATE_RENDERER)();
	CREATE_RENDERER FuncCreateRenderer = (CREATE_RENDERER)::GetProcAddress( renderDll, "CreateDX11Renderer" );

	if( FuncCreateRenderer == NULL )
		return false;

	m_pRenderer = FuncCreateRenderer();
	m_pRenderer->StartUp( param );

	//////////////////////////////////////////////////////////////////////////
	// initialize Asset manager
	m_AssetMgr.Init( param.numOfProcessThread, m_pRenderer);

	return true;
}

bool EEngine::ShutDown()
{
	m_ActorMgr.Clear();
	m_EntityMgr.Clear();
	m_AssetMgr.Clear();
	m_pRenderer->ShutDown();

	return false;
}