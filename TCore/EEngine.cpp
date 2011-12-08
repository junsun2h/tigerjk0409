#include "EEngine.h"
#include <windows.h>

EEngine* s_pEngine = NULL;

EEngine::EEngine()
	: m_pRenderer(NULL)
{

}

EEngine::~EEngine()
{

}

bool EEngine::StartUp(const CENGINE_INIT_PARAM &param)
{
	if( s_pEngine != NULL )
	{
		assert(0);
		return false;
	}

	s_pEngine = this;

	// load Render DLL
	HMODULE	renderDll = ::LoadLibrary( L"Renderer.dll");
	if( renderDll == NULL )
	{
		assert(0);
		return false;
	}

	// make renderer
	typedef IRenderer *(*CREATE_RENDERER)();
	CREATE_RENDERER FuncCreateRenderer = (CREATE_RENDERER)::GetProcAddress( renderDll, "CreateDX11Renderer" );

	if( FuncCreateRenderer == NULL )
		return false;

	m_pRenderer = FuncCreateRenderer();
	m_pRenderer->StartUp( param );

	return true;
}

bool EEngine::ShutDown()
{
	return false;
}

IRenderer* EEngine::GetRenderer()
{
	return m_pRenderer;
}