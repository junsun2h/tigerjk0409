#pragma once


class EMotionDataProcessor : public IDataProcessor
{
	CResourceMotion*			m_pMotion;

	virtual void				Init(CResourceBase* pRsc, bool bForward) override;
	virtual bool				CompleteWork() override;
	virtual void				Process( void* pData, SIZE_T cBytes ) override;
	virtual eRESOURCE_FILE_TYPE	Type()	{return RESOURCE_FILE_MOTION; }
};