#include "EEngine.h"
#include "EEntityProxyCamera.h"
#include "IRDevice.h"

#include <windows.h>



EEngine::EEngine()
	: m_pRenderer(NULL)
	, m_CurrentFrame(0)
{
	m_GlobalTimer.Start();
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
	m_AssetMgr.Clear();
	m_pRenderer->ShutDown();
	m_EntityMgr.Destroy();

	return false;
}

void EEngine::UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback)
{
	float deltaTime = m_GlobalTimer.GetElapsedTime();
	
	//////////////////////////////////////////////////////////////////////////
	// 1) update Render independent system


	//////////////////////////////////////////////////////////////////////////
	// 2) update culled space list if camera is moved
	if( pCamera != NULL && pCamera->GetLastTransformChangedFrame() == m_CurrentFrame )
		m_SceneMgr.UpdateVisibleOctreeNodeList(pCamera);

	//////////////////////////////////////////////////////////////////////////
	// 3) update render dependent system


	//////////////////////////////////////////////////////////////////////////
	// 4) make render object list
	m_SceneMgr.UpdateRenderObjectList();

	//////////////////////////////////////////////////////////////////////////
	// 5) render current frame

	if( pRenderCallback )
		pRenderCallback->PreRender();

	m_pRenderer->Render( pCamera->GetDesc() );

	if( pRenderCallback )
		pRenderCallback->PostRender();

	m_pRenderer->RenderUI();
	m_pRenderer->Present();

	m_CurrentFrame++;
}