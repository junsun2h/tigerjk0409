#pragma once



class RDX11ShaderMgr : public IShaderMgr
{
public:
	RDX11ShaderMgr();
	~RDX11ShaderMgr();

	void			init();
	void			Destroy();

	bool			CheckShader(CRenderElement* pRenderElement) override;
	
	IShader*		GetShader(UINT flag, eSHADER_TYPE shaderType) override;
	IShader*		CreateShader(UINT flag, eSHADER_TYPE shaderType);
	void			Begin(UINT flag) override;

	bool			CheckAndSet(IShader* pShader) override;
	IShader*		GetCurrentShader(eSHADER_TYPE type) override		{ return m_pCurrentShader[type]; }
	
	void			SetShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type) override;
	void			SetTexture(const CResourceTexture* pTexture, UINT slot) override;

	typedef ATL::CAtlMap<UINT, IShader*>	SHADER_MAP;	
	SHADER_MAP		m_Shaders[NUM_SHADER_TYPE];

	ID3D11Buffer*	m_ConstBuffer[NUM_SHADER_TYPE][MAX_SHADER_CONSTANT_SLOT];
	UINT			m_ConstBufferSize[NUM_SHADER_TYPE][MAX_SHADER_CONSTANT_SLOT];
	IShader*		m_pCurrentShader[NUM_SHADER_TYPE];
};
