#pragma once



class RDX11ShaderMgr : public IShaderMgr
{
public:
	RDX11ShaderMgr();
	~RDX11ShaderMgr();

	void			init();
	void			Destroy();

	bool			AssignShader(CRenderElement* pRenderElement) override;
	IShader*		GetShader(eEFFECT_TYPE type) override;
	bool			SetCurrentShader(IShader* pShader) override;
	IShader*		GetCurrentVS() override					{ return m_pCurrentVS; }
	IShader*		GetCurrentPS() override					{ return m_pCurrentPS; }
	IShader*		GetCurrentGS() override					{ return m_pCurrentGS; }
	
	void			UpdateShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type) override;
	void			UpdateShaderResourceView(CResourceMtrl* pMtrl, eTEXTURE_TYPE textureType);
	void			UpdateTexture(CResourceTexture* pTexture, UINT slot) override;
	
	ID3D11Buffer*	m_ConstBuffer[NUM_SHADER_TYPE][MAX_SHADER_CONSTANT_SLOT];
	UINT			m_ConstBufferSize[NUM_SHADER_TYPE][MAX_SHADER_CONSTANT_SLOT];

	typedef ATL::CAtlMap<long, IShader*>	SHADER_MAP;

	SHADER_MAP		m_ShaderMap;
	IShader*		m_pCurrentVS;
	IShader*		m_pCurrentPS;
	IShader*		m_pCurrentGS;
};
