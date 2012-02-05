#pragma once


struct IRDX11Device : public IRDevice
{
	virtual void RecreateBuffer(ID3D11Buffer** ppBuffer, void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT) =0;
	virtual void RenderGeometry(CResourceGeometry* pGeometry) = 0;

};