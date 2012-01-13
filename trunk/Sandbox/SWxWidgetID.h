#pragma once

#include "IEngine.h"
#include "SAssetPanel.h"
#include "SPropertyPanel.h"
#include "SSceneHierarchyPanel.h"
#include "SMainFrame.h"


namespace SGLOBAL
{
	IEngine*				Engine();
	SAssetPanel*			AssetPanel();
	SPropertyPanel*			PropertyPanel();
	SSceneHierarchyPanel*	SceneHierarchyPanel();
	SMainFrame*				MainFrame();
};


enum eWIDGETID
{
	ID_MAINFRAME = wxID_HIGHEST+1,

	ID_MAIN_TOOLBAR,
	ID_TOOLBAR_SELECT,
	ID_TOOLBAR_POSITION,
	ID_TOOLBAR_ROTATION,
	ID_TOOLBAR_SCALE,

	ID_MENU_DEFAULT_LAYOUT,
	ID_MENU_LOAD_SAVED_LAYOUT,
	ID_MENU_SAVE_LAYOUT,

	ID_MENU_VIEW_PROPERTY,
	ID_MENU_VIEW_SCENE_HIERARCHY,
	ID_MENU_VIEW_ASSET,

	ID_PANEL_3DVIEW,
	ID_PANEL_PROPERTY,
	ID_PANEL_SCENE_HIERARCHY,
	ID_PANEL_ASSET,
	
	ID_ASSET_FILTER_TEXTCTRL,
	ID_ASSET_TREECTRL,
	ID_ASSET_DELETE,

	ID_SCENE_FILTER_TEXTCTRL,
	ID_SCENE_TREECTRL,
};