#pragma once

#include "CDefine.h"
#include "IResource.h"

#define E_TRYAGAIN MAKE_HRESULT(1,FACILITY_WIN32,123456)


///////////////////////////////Thread Prefix///////////////////////////////
//IOP_ = IO Thread
//PT_  = Process Thread
//MT_  = Main Thread
//RT_  = Render Thread
//////////////////////////////////////////////////////////////////////////



struct IDataLoader
{
	virtual ~IDataLoader(){}
	virtual bool PT_Decompress( void** ppData, SIZE_T* pcBytes ) = 0;
	virtual bool MT_Destroy() = 0;
	virtual bool IOT_Load() = 0;
};



struct IDataProcessor
{
	virtual ~IDataProcessor()	{}
	virtual bool			MT_Complete(IAssetMgr* pAssetMgr) = 0;
	virtual bool			MT_Destroy() = 0;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) = 0;
};



struct RESOURCE_REQUEST
{
	IDataLoader*	pDataLoader;
	IDataProcessor*	pDataProcessor;
	CALLBACK_LOAD_COMPLED	pCallBackComplete;
};



struct IAsyncLoader
{
	virtual ~IAsyncLoader(){}
	virtual void	MT_AddWorkItem( RESOURCE_REQUEST resourceRequest) = 0;
	virtual void	MT_WaitForAllItems(IAssetMgr* pAssetMgr) = 0;
	virtual void    MT_CompleteWork( UINT CurrentNumResourcesToService, IAssetMgr* pAssetMgr) = 0;
};
