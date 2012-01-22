#pragma once

#include "SMainToolBar.h"

class SMainMenuBar: public wxMenuBar
{
public:
	SMainMenuBar()
	{	
		// file
		{
			wxMenu* fileMenu = new wxMenu;

			fileMenu->Append(wxID_NEW, "New(&N)\tCtrl-N");
			fileMenu->Append(wxID_SAVE, "Save(&S)\tCtrl-S");
			fileMenu->Append(wxID_SAVEAS, "Save as(&A)\tCtrl-Shift-S");

			fileMenu->AppendSeparator();

			fileMenu->Append(wxID_EXIT, "Exit(&X)\tAlt-X");

			Append(fileMenu, "File(&F)");
		}
		// layout
		{
			wxMenu* viewMenu = new wxMenu;

			viewMenu->AppendCheckItem(ID_MENU_VIEW_PROPERTY, "Property Window(&P)");
			viewMenu->AppendCheckItem(ID_MENU_VIEW_SCENE_HIERARCHY, "Scene Hierarchy Window(&H)");
			viewMenu->AppendCheckItem(ID_MENU_VIEW_ASSET, "Asset Window(&A)");

			viewMenu->AppendSeparator();

			viewMenu->Append(ID_MENU_DEFAULT_LAYOUT, "Default Layout(&D)");
			viewMenu->Append(ID_MENU_LOAD_SAVED_LAYOUT, "Load saved Layout(&L)");
			
			viewMenu->AppendSeparator();

			viewMenu->Append(ID_MENU_SAVE_LAYOUT, "Save Layout(&S)");

			Append(viewMenu, "Layout(&L)");
		}
		// GameObject
		{
			wxMenu* viewMenu = new wxMenu;

			viewMenu->Append(ID_MENU_ENTITY_NEW, "Create(&C)");

			Append(viewMenu, "Entity(&E)");
		}
		// Component
		{
			wxMenu* viewMenu = new wxMenu;

			m_pComponetCamera = viewMenu->Append(ID_MENU_COMPONENT_CREATE_CAMERA, "ADD Camera");
			m_pComponetRenderer = viewMenu->Append(ID_MENU_COMPONENT_CREATE_RENDERER, "ADD Renderer");

			Append(viewMenu, "Component(&C)");

			EnableComponent( false );
		}
	}

	void UpdateLayoutMenuItemCheckStatus(wxFrame* pFrame)
	{
		Check(ID_MENU_VIEW_PROPERTY, pFrame->FindWindow(ID_PANEL_PROPERTY)->IsShown());
		Check(ID_MENU_VIEW_SCENE_HIERARCHY, pFrame->FindWindow(ID_PANEL_SCENE_HIERARCHY)->IsShown());
		Check(ID_MENU_VIEW_ASSET, pFrame->FindWindow(ID_PANEL_ASSET)->IsShown());
	}

	void EnableComponent(bool b)
	{
		m_pComponetCamera->Enable(b);
		m_pComponetRenderer->Enable(b); 
	}

private:
	wxMenuItem* m_pComponetCamera;
	wxMenuItem* m_pComponetRenderer; 	
};
