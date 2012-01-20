#pragma once

#include "CDefine.h"
#include "CResource.h"


#define E_TRYAGAIN MAKE_HRESULT(1,FACILITY_WIN32,123456)


///////////////////////////////Thread Prefix///////////////////////////////
//IOP_ = IO Thread
//PT_  = Process Thread
//RT_  = Render Thread
// no prefix mean main thread
//////////////////////////////////////////////////////////////////////////

typedef void    (CALLBACK *CALLBACK_LOAD_COMPLED)();

struct IDataLoader
{
	virtual ~IDataLoader(){}
	virtual bool GetData( void** ppData, SIZE_T* pcBytes ) = 0;
	virtual bool IOT_Load() = 0;
	virtual bool Load() = 0;
};



struct IDataProcessor
{
	virtual ~IDataProcessor()	{}
	virtual bool	PopData() = 0;
	virtual bool	PT_Process( void* pData, SIZE_T cBytes ) = 0;
	virtual bool	Process( void* pData, SIZE_T cBytes ) = 0;
};



struct RESOURCE_REQUEST
{
	IDataLoader*	pDataLoader;
	IDataProcessor*	pDataProcessor;
	CALLBACK_LOAD_COMPLED	pCallBackComplete;
};



struct ILoader
{
	virtual CResourceBase*	LoadForward(char* fileName, char* name, eRESOURCE_FILE_TYPE type) = 0;
	virtual void			LoadBackword(char* fileName, char* name, eRESOURCE_FILE_TYPE type) = 0;
	virtual void			WaitForAllItems() = 0;
	virtual void			CompleteWork( UINT CurrentNumResourcesToService ) = 0;
};
