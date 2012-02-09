#pragma once


class EMotionDataProcessor : public IDataProcessor
{
	std::string				m_name;
public:
	EMotionDataProcessor(std::string name );
	~EMotionDataProcessor();

	virtual bool			PopData() override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
	virtual CResourceBase*	Process( void* pData, SIZE_T cBytes ) override;
};