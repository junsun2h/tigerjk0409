#pragma once

#include "RDefine.h"
#include "IRDevice.h"

struct IRDX11Device : public IRDevice
{
	virtual void RecreateBuffer(ID3D11Buffer** ppBuffer, void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT) =0;
};