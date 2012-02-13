#pragma once


class SCanvas;
class SEntityTreeCtrl;
class SSceneTreeCtrl;
class SAnimationToolBar;
class SMotionTreeCtrl;


enum PROPERTY_WIDGET_ID
{
	ID_PROPERTY_ENTITY_TREECTRL,
	ID_PROPERTY_SCENE_TREECTRL,
	ID_PROPERTY_MOTION_TREECTRL,
	ID_PROPERTY_GRID,
	ID_PROPERTY_ANIMATION_TOOLBAR	
};

enum PROPERTY_SUB_WINDOWS
{
	TEXTURE_CANVAS	= BIT(1),
	PROPERTY_GRID	= BIT(2),
	ENTITY_TREE_CTRL = BIT(3),
	SCENE_TREE_CTRL	= BIT(4),
	ANIMATION_TOOLBAR = BIT(5),
	MOTION_TREE_CTRL = BIT(6),
};

class SPropertyGrid;

class SPropertyPanel : public wxPanel
{
	DECLARE_DYNAMIC_CLASS(SPropertyPanel)
	wxDECLARE_NO_COPY_CLASS(SPropertyPanel);

public:
	SPropertyPanel();
	SPropertyPanel(wxWindow* parent);

	void					SetObject( IEntity* pEntity );
	void					SetObject( const CResourceBase* pResource );

	void					SetObject( const CResourceTexture* pResource );
	void					SetObject( const CResourceMesh* pResource );
	void					SetObject( const CResourceActor* pResource );
	void					SetObject( const CResourceMotion* pResource );

	void					Empty();

private:
	void					OrganizeInside(DWORD flag);

	SCanvas*				m_pTextureCanvas;
	SPropertyGrid*			m_pGridMgr;
	SEntityTreeCtrl*		m_pEntityTreeCtrl;
	SSceneTreeCtrl*			m_pSceneTreeCtrl;
	SAnimationToolBar*		m_pAnimationTooBar;
	SMotionTreeCtrl*		m_pMotionTreeCtrl;

	DECLARE_EVENT_TABLE()
};