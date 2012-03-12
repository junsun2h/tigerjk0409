#pragma once



struct RDX11Shader : public IShader
{
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;	

	GRAPHIC_DEVICE_DESC			m_RenderState;
	SHADER_COMPILE_DESC			m_Desc;

	RDX11Shader()
		: m_pVertexShader(NULL)
		, m_pPixelShader(NULL)
	{		
	}

	~RDX11Shader()
	{
		Release();
	}

	void Release()
	{
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPixelShader);
	}

	SHADER_COMPILE_DESC& GetDesc() override
	{
		return m_Desc;
	}
};




//---------------------------------------------------------------------------------------------------------------
class RDX11ShaderMgr : public IShaderMgr
{
public:
	RDX11ShaderMgr();
	~RDX11ShaderMgr();

	void			init();
	void			Destroy();

	bool			CheckShader(CRenderElement* pRenderElement) override;

	void			Begin(UINT flag) override			{	Begin(flag, flag);	}
	void			Begin(IShader* pVS, IShader* pPS) override;
	void			Begin(UINT vsFlag, UINT psFlag) override;

	void			SetShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type) override;
	void			SetTexture(const CResourceTexture* pTexture, UINT slot) override;

	void			Reload(UINT flag) override;

	RDX11Shader*	CreateShader(UINT flag, eSHADER_TYPE shaderType);
	RDX11Shader*	GetShader(UINT flag, eSHADER_TYPE shaderType);





	typedef ATL::CAtlMap<UINT, RDX11Shader*>	SHADER_MAP;	
	SHADER_MAP		m_Shaders[NUM_SHADER_TYPE];

	ID3D11Buffer*	m_ConstBuffer[NUM_SHADER_TYPE][MAX_SHADER_CONSTANT_SLOT];
	UINT			m_ConstBufferSize[NUM_SHADER_TYPE][MAX_SHADER_CONSTANT_SLOT];
	IShader*		m_pCurrentShader[NUM_SHADER_TYPE];
};
