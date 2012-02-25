#include "wx/wx.h"
#include "wx/aui/aui.h"
#include "wx/filename.h"
#include "wx/treectrl.h"

#include "CDefine.h"
#include "CEngineParam.h"

#include "IEntityProxy.h"
#include "IEntityMgr.h"
#include "IAssetMgr.h"
#include "ILoader.h"
#include "IEntity.h"
#include "ISpaceMgr.h"

#include "SGlobal.h"
#include "SSceneHierarchyPanel.h"
#include "SAssetPanel.h"
#include "SSelectionMgr.h"


namespace GLOBAL
{
	SMainFrame*				g_MainFrame = NULL;
	IEngine*				g_Eng = NULL;
	IEntity*				g_SceneRoot = NULL;
	IEntityProxyCamera*		g_Observer = NULL;
	SAssetPanel*			g_AssetPanel = NULL;
	SPropertyPanel*			g_PropertyPanel = NULL;
	SSceneHierarchyPanel*	g_SceneHierarchyPanel = NULL;
	S3DViewPanel*			g_ViewPanel = NULL;
	SSelectionMgr			g_SelectionMgr;

	IEntity*				SceneRoot()				{ return g_SceneRoot; }
	IEngine*				Engine()				{ return g_Eng; }
	IEntityProxyCamera*		ObserverCamera()		{ return g_Observer; }
	SAssetPanel*			AssetPanel()			{ return g_AssetPanel; }
	SPropertyPanel*			PropertyPanel()			{ return g_PropertyPanel; }
	SSceneHierarchyPanel*	SceneHierarchyPanel()	{ return g_SceneHierarchyPanel; }
	SMainFrame*				MainFrame()				{ return g_MainFrame; }
	S3DViewPanel*			ViewPanel()				{ return g_ViewPanel; }
	SSelectionMgr*			SelectionMgr()			{ return &g_SelectionMgr; }


	void SetupScene(int nWidth, int nHeight)
	{
		IEntityMgr* entityMgr = g_Eng->EntityMgr();

		g_SceneRoot = entityMgr->SpawnEntity( "Root" );

		//////////////////////////////////////////////////////////////////////////
		// Setup Camera
		IEntity* pCameraEntity = entityMgr->SpawnEntity( "Observer" );

		IEntityProxyCamera* pCamera = (IEntityProxyCamera*)pCameraEntity->CreateProxy(ENTITY_PROXY_CAMERA);

		pCamera->SetProjParam( XM_PIDIV4,  nWidth, nHeight, 1, 10000);
		pCamera->SetViewParam( CVector3(500.0f, 500.0f, 500.0f), CVector3(0, 0, 0), CVector3(0.0f, 0.0f, 1.0f) );

		g_Observer = pCamera;

		g_SceneHierarchyPanel->Reload();
		wxCommandEvent e;
		GLOBAL::AssetPanel()->OnReload(e);
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
		if( g_Eng->StartUp( &engineParam ) == false )
			return false;

		SetupScene( engineParam.width, engineParam.height );
		
		return true;
	}
}