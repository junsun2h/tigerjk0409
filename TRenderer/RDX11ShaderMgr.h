#pragma once


class RDX11ShaderMgr : public IShaderMgr
{
public:
	RDX11ShaderMgr();
	~RDX11ShaderMgr();

	void			init();
	void			Destroy();

	IShader*		GetShader(EFFECT_TYPE type) override;
	void			SetCurrentShader(IShader* pShader) override { m_pCurrentShader = pShader; }
	IShader*		GetCurrentShader() override					{ return m_pCurrentShader; }

	void			UpdateShaderConstant(void* pScr, size_t size, SHADER_CONST_BUFFER_SLOT slot, SHADER_TYPE type) override;
	void			UpdateShaderResourceView(CResourceMtrl* pMtrl, eTEXTURE_TYPE textureType);

private:
	UINT			GetDXBufSize(SHADER_CONST_BUFFER_SLOT slot);


	typedef ATL::CAtlMap<UINT, ID3D11Buffer*>	CONST_BUFFER_MAP;
	CONST_BUFFER_MAP	m_ConstBufferMap[NUM_SHADER_TYPE];

	typedef ATL::CAtlMap<long, IShader*>	SHADER_MAP;
	SHADER_MAP			m_ShaderMap;
	IShader*			m_pCurrentShader;
};
