#pragma once

struct RENDER_TEXT_QUAD;

struct IFontRenderer
{
	virtual void		SetFontTexture( CResourceTexture* pTexture) = 0;
	virtual void		Destroy() = 0;
	virtual void		Render( RENDER_TEXT_QUAD*  pText ) = 0;
};