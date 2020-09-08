
#ifndef __ADDINNATIVE_H__
#define __ADDINNATIVE_H__



#ifndef __linux__
#include <wtypes.h>
#endif //__linux__

#include "ComponentBase.h"
#include "AddInDefBase.h"
#include "IMemoryManager.h"
#include "bzlib.h"

///////////////////////////////////////////////////////////////////////////////
// class CAddInNative
class CAddInNative : public IComponentBase
{
public:
	enum Props
	{
		eLastProp      // Always last
	};

	enum Methods
	{
		BzipCompress,
		BzipExtract,
		BzipCompressInMemory, 
		BzipExtractInMemory,
		eLastMethod      // Always last
	};

	CAddInNative(void);
	virtual ~CAddInNative();
	// IInitDoneBase
	virtual bool ADDIN_API Init(void*);
	virtual bool ADDIN_API setMemManager(void* mem);
	virtual long ADDIN_API GetInfo();
	virtual void ADDIN_API Done();
	// ILanguageExtenderBase
	virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T** wsLanguageExt);
	virtual long ADDIN_API GetNProps();
	virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);
	virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);
	virtual bool ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal);
	virtual bool ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal);
	virtual bool ADDIN_API IsPropReadable(const long lPropNum);
	virtual bool ADDIN_API IsPropWritable(const long lPropNum);
	virtual long ADDIN_API GetNMethods();
	virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);
	virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum,
		const long lMethodAlias);
	virtual long ADDIN_API GetNParams(const long lMethodNum);
	virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum,
		tVariant *pvarParamDefValue);
	virtual bool ADDIN_API HasRetVal(const long lMethodNum);
	virtual bool ADDIN_API CallAsProc(const long lMethodNum,
		tVariant* paParams, const long lSizeArray);
	virtual bool ADDIN_API CallAsFunc(const long lMethodNum,
		tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray);
	operator IComponentBase*() { return (IComponentBase*)this; }
	// LocaleBase
	virtual void ADDIN_API SetLocale(const WCHAR_T* loc);
private:
	IAddInDefBase      *m_iConnect;
	IMemoryManager     *m_iMemory;

	int level = 9;
	char *fn_r;
	char *fn_w;

	char buff[0x1000];
	char mode[10];
	
	int blockSize100k = 9;
	int verbosity = 0;
	int workFactor = 30; // 0 = USE THE DEFAULT VALUE

	BZFILE *BZ2fp_w = NULL;
	BZFILE *BZ2fp_r = NULL;
	FILE *fp_r = NULL;
	FILE *fp_w = NULL;

	unsigned int sourceLength = 0;
	unsigned int destLength = 0;

	char* dest;
	int len;
	int returnCode;

	long findName(const wchar_t * names[], const wchar_t * name, const uint32_t size) const;
	bool wstring_to_p(std::wstring str, tVariant * val);
	std::wstring utf8_decode(const std::string & str);
	std::string ws2s(const std::wstring & wstr);
	char * wideCharToMultiByte(const wchar_t * lpwstr);
	char * getStringFromVariant(tVariant * paParams);
	void addError(uint32_t wcode, const wchar_t * source, const wchar_t * descriptor, long code);

	void setVariantValue(tVariant * paParams, bool value);
	void setVariantValue(tVariant * paParams, char * value);


};
#endif //__ADDINNATIVE_H__
