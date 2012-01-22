#include "SEntitySelection.h"
#include "SPropertyPanel.h"
#include "SMainMenuBar.h"


void SEntitySelection::Clear()
{
	m_SelctionList.clear();

	GLOBAL::PropertyPanel()->SetEmpty();
}

void SEntitySelection::Select(IEntity* pEntity, bool multiSelection)
{
	assert( pEntity);

//	#define IsKeyDown(key) ((GetAsyncKeyState(key) & 0x8000)!=0)
// IsKeyDown(VK_SHIFT)
	
	if( multiSelection == false )
		m_SelctionList.clear();

	m_SelctionList.push_back(pEntity);
	GLOBAL::PropertyPanel()->SetObject(pEntity);
	SMainMenuBar* pMainMenuBar = (SMainMenuBar*)GLOBAL::MainFrame()->GetMenuBar();
	pMainMenuBar->EnableComponent(true);
}

void SEntitySelection::UnSelect()
{
	m_SelctionList.clear();

	SMainMenuBar* pMainMenuBar = (SMainMenuBar*)GLOBAL::MainFrame()->GetMenuBar();
	pMainMenuBar->EnableComponent(false);
}

IEntity* SEntitySelection::First()
{
	if( m_SelctionList.size() > 0 )
		return m_SelctionList[0];

	return NULL;
}