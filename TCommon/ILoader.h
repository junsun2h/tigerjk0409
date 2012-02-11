#pragma once

struct CResourceBase;
enum eRESOURCE_FILE_TYPE;


#define E_TRYAGAIN MAKE_HRESULT(1,FACILITY_WIN32,123456)


inline void ECopyData(void* dest, BYTE** src, int size)
{
	CopyMemory( dest, *src, size );
	*src += size;
}

inline void ECopyString(char* dest, BYTE** src)
{
	BYTE nameLength;
	ECopyData(&nameLength, src, 1);
	strcpy_s( dest, 64, (char*)*src);
	*src += nameLength;
}

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
	virtual bool			PopData() = 0;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) = 0;
	virtual CResourceBase*	Process( void* pData, SIZE_T cBytes ) = 0;
};



struct RESOURCE_REQUEST
{
	IDataLoader*	pDataLoader;
	IDataProcessor*	pDataProcessor;
	CALLBACK_LOAD_COMPLED	pCallBackComplete;
};



struct ILoader
{
	virtual	bool			IsIOThread() = 0;
	virtual	bool			IsDataProcThread() = 0;

	virtual	bool			Init( UINT NumProcessingThreads ) =0;
	virtual CResourceBase*	LoadForward(char* name, eRESOURCE_FILE_TYPE type) = 0;
	virtual CResourceBase*	LoadForward(const char* fullPath, char* name, eRESOURCE_FILE_TYPE type) = 0;
	virtual void			LoadBackword(char* fileName, char* name, eRESOURCE_FILE_TYPE type) = 0;
	virtual void			WaitForAllItems() = 0;
	virtual void			CompleteWork( UINT CurrentNumResourcesToService ) = 0;
};
