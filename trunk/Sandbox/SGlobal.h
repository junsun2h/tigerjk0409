#pragma once


#include "IEngine.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "wxmsw29ud_aui.lib" )
#pragma comment( lib, "wxmsw29ud_html.lib")
#pragma comment( lib, "wxmsw29ud_adv.lib")
#pragma comment( lib, "wxmsw29ud_core.lib")
#pragma comment( lib, "wxmsw29ud_propgrid.lib")
#pragma comment( lib, "wxbase29ud_xml.lib")
#pragma comment( lib, "wxbase29ud.lib")
#pragma comment( lib, "wxtiffd.lib")
#pragma comment( lib, "wxjpegd.lib")
#pragma comment( lib, "wxpngd.lib")
#pragma comment( lib, "wxzlibd.lib")
#pragma comment( lib, "wxregexud.lib")
#pragma comment( lib, "wxexpatd.lib")
#else
#pragma comment( lib, "wxmsw29u_aui.lib" )
#pragma comment( lib, "wxmsw29u_html.lib")
#pragma comment( lib, "wxmsw29u_adv.lib")
#pragma comment( lib, "wxmsw29u_core.lib")
#pragma comment( lib, "wxmsw29u_propgrid.lib")
#pragma comment( lib, "wxbase29u_xml.lib")
#pragma comment( lib, "wxbase29u.lib")
#pragma comment( lib, "wxtiff.lib")
#pragma comment( lib, "wxjpeg.lib")
#pragma comment( lib, "wxpng.lib")
#pragma comment( lib, "wxzlib.lib")
#pragma comment( lib, "wxregexu.lib")
#pragma comment( lib, "wxexpat.lib")
#endif


class SAssetPanel;
class SPropertyPanel;
class SSceneHierarchyPanel;
class SMainFrame;
class S3DViewPanel;
class SSelectionMgr;


namespace GLOBAL
{
	bool					InitDevice(CENGINE_INIT_PARAM& engineParam);

	IEntity*				SceneRoot();
	IEngine*				Engine();
	IEntityProxyCamera*		ObserverCamera();
	SAssetPanel*			AssetPanel();
	S3DViewPanel*			ViewPanel();
	SPropertyPanel*			PropertyPanel();
	SSceneHierarchyPanel*	SceneHierarchyPanel();
	SMainFrame*				MainFrame();
	SSelectionMgr*			SelectionMgr();
}


enum GLOBAL_WIDGET_ID
{
	ID_MAINFRAME = 6000, // wxID_HIGHEST + 1

	ID_PANEL_3DVIEW,
	ID_PANEL_PROPERTY,
	ID_PANEL_SCENE_HIERARCHY,
	ID_PANEL_ASSET,
};