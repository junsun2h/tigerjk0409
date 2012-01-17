#pragma once

#include "wx/wx.h"
#include "IEngine.h"


class SAssetPanel;
class SPropertyPanel;
class SSceneHierarchyPanel;
class SMainFrame;
class S3DViewPanel;


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
}


enum GLOBAL_WIDGET_ID
{
	ID_MAINFRAME = wxID_HIGHEST+1,

	ID_PANEL_3DVIEW,
	ID_PANEL_PROPERTY,
	ID_PANEL_SCENE_HIERARCHY,
	ID_PANEL_ASSET,
};