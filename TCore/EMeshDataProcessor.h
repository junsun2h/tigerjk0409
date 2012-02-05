#pragma once


class EMeshDataProcessor : public IDataProcessor
{
	std::string					m_name;
	std::vector<CResourceBase*>	m_pResources;
	
	long					PT_ReadMesh(BYTE** ppSrcBits, std::string name);

public:
	EMeshDataProcessor(std::string name );
	~EMeshDataProcessor();

	virtual bool			PopData() override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
	virtual CResourceBase*	Process( void* pData, SIZE_T cBytes ) override;
};