/*

//--------------------------------------------------------------------------------------
void RDX11Font::DrawFormattedTextLine( RECT& rc, D3DXCOLOR clr, const WCHAR* strMsg, ... )
{
WCHAR strBuffer[512];

va_list args;
va_start( args, strMsg );
vswprintf_s( strBuffer, 512, strMsg, args );
strBuffer[511] = L'\0';
va_end( args );

DrawTextLine( rc, strBuffer, clr );
}
*/