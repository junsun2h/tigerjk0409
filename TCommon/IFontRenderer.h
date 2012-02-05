#pragma once


struct IFontRenderer
{
	virtual bool		SetFontFile(const char* fontDDS) = 0;
	virtual void		Destroy() = 0;
	virtual void		Render( RENDER_TEXT_BUFFER& text ) = 0;
};