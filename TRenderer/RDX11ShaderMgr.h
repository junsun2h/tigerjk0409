#pragma once


class RDX11ShaderMgr : public IShaderMgr
{
public:
	RDX11ShaderMgr();
	~RDX11ShaderMgr();

	void			init();
	void			Destroy();

	IShader*		GetShader(eEFFECT_TYPE type) override;
	bool			SetCurrentShader(IShader* pShader) override;
	IShader*		GetCurrentVS() override					{ return m_pCurrentVS; }
	IShader*		GetCurrentPS() override					{ return m_pCurrentPS; }
	IShader*		GetCurrentGS() override					{ return m_pCurrentGS; }
	void			UpdateShaderConstant(void* pScr, size_t size, SHADER_CONST_BUFFER_SLOT slot, eSHADER_TYPE type) override;
	void			UpdateShaderResourceView(CResourceMtrl* pMtrl, eTEXTURE_TYPE textureType);

private:
	UINT			GetDXBufSize(SHADER_CONST_BUFFER_SLOT slot);


	typedef ATL::CAtlMap<UINT, ID3D11Buffer*>	CONST_BUFFER_MAP;
	CONST_BUFFER_MAP	m_ConstBufferMap[NUM_SHADER_TYPE];

	typedef ATL::CAtlMap<long, IShader*>	SHADER_MAP;

	SHADER_MAP			m_ShaderMap;
	IShader*			m_pCurrentVS;
	IShader*			m_pCurrentPS;
	IShader*			m_pCurrentGS;
};
