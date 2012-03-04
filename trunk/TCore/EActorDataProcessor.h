#pragma once


class EActorDataProcessor : public IDataProcessor
{
	CResourceActor*			m_pActor;
	bool					m_bForward;

	virtual void			Init(CResourceBase* pRsc, bool bForward) override;
	virtual bool			CompleteWork() override;
	virtual void			Process( void* pData, SIZE_T cBytes ) override;
	virtual eRESOURCE_FILE_TYPE	Type()	{return RESOURCE_FILE_ACTOR; }
};