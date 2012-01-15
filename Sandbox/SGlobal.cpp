#include "SGlobal.h"
#include "SSceneHierarchyPanel.h"


namespace GLOBAL
{
	SMainFrame*				g_MainFrame = NULL;
	IEngine*				g_Eng = NULL;
	IEntity*				g_SceneRoot = NULL;
	IEntityProxyCamera*		g_MainCamera = NULL;
	SAssetPanel*			g_AssetPanel = NULL;
	SPropertyPanel*			g_PropertyPanel = NULL;
	SSceneHierarchyPanel*	g_SceneHierarchyPanel = NULL;

	IEntity*				SceneRoot()				{ return g_SceneRoot; }
	IEngine*				Engine()				{ return g_Eng; }
	IEntityProxyCamera*		MainCamera()			{ return g_MainCamera; }
	SAssetPanel*			AssetPanel()			{ return g_AssetPanel; }
	SPropertyPanel*			PropertyPanel()			{ return g_PropertyPanel; }
	SSceneHierarchyPanel*	SceneHierarchyPanel()	{ return g_SceneHierarchyPanel; }
	SMainFrame*				MainFrame()				{ return g_MainFrame; }

	void SetupScene(int nWidth, int nHeight)
	{
		IEntityMgr* entityMgr = g_Eng->EntityMgr();

		g_SceneRoot = entityMgr->SpawnEntity( "SceneRoot" );

		//////////////////////////////////////////////////////////////////////////
		// Setup Camera
		IEntity* pEntity = entityMgr->SpawnEntity( "MainCamera" );
		g_MainCamera = (IEntityProxyCamera*)entityMgr->SpawnEntityProxy("Main Camera" , ENTITY_PROXY_CAMERA);

		g_MainCamera->SetProjParam( XM_PIDIV4,  nWidth, nHeight, 1, 10000);
		g_MainCamera->SetViewParam( CVector3(1000, -1000, 1000), CVector3(0, 0, 0), CVector3(0.0f, 0.0f, 1.0f) );

		pEntity->SetProxy( g_MainCamera );

		g_SceneRoot->AttachChild( pEntity );

		SceneHierarchyPanel()->Reload();
	}

	bool InitDevice(CENGINE_INIT_PARAM& engineParam)
	{
		HMODULE	engineDll = ::LoadLibrary( L"Engine.dll");
		if( engineDll == NULL )
			return false;

		typedef IEngine *(*CREATE_ENGINE)();
		CREATE_ENGINE FuncCreateEngine = NULL;

		FuncCreateEngine = (CREATE_ENGINE)::GetProcAddress( engineDll, "CreateEngine" );

		if( FuncCreateEngine == NULL )
			return false;

		g_Eng = FuncCreateEngine();
		if( g_Eng->StartUp( engineParam ) == false )
			return false;

		SetupScene( engineParam.width, engineParam.height );
		
		return true;
	}
}