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

typedef void    (CALLBACK *CALLBACK_LOAD_COMPLED)(CResourceBase* pResource);


struct IDataProcessor
{
	virtual void				Init(CResourceBase* pRsc, bool bForward) = 0;
	virtual bool				CompleteWork() = 0;
	virtual void				PT_Process( void* pData, SIZE_T cBytes ) { Process(pData, cBytes); };
	virtual void				Process( void* pData, SIZE_T cBytes ) = 0;
	virtual eRESOURCE_FILE_TYPE	Type() = 0;
};


struct ILoader
{
	virtual	bool					IsIOThread() = 0;
	virtual	bool					IsDataProcThread() = 0;

	virtual	bool					Init( UINT NumProcessingThreads ) =0;
	virtual const CResourceBase*	Load(const char* name, eRESOURCE_FILE_TYPE type, bool bForward) = 0;
	virtual const CResourceBase*	Load(const char* fullpath, const char* name, eRESOURCE_FILE_TYPE type, bool bForward) = 0;

	virtual void					WaitForAllItems() = 0;
	virtual void					CompleteWork( UINT completeLimit ) = 0;
};
