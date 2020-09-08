
#include "stdafx.h"

#ifdef __linux__
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <wchar.h>
#include <string>
#include <locale> 
#include <codecvt>
#include "AddInNative.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "stddef.h"

static const wchar_t g_kClassNames[] = L"AddInNative"; //|OtherClass1|OtherClass2";

static const wchar_t *g_PropNames[] = { L"File" };
static const wchar_t *g_PropNamesRu[] = { L"‘айл" };

static const wchar_t *g_MethodNames[] = { L"BzipCompress", L"BzipExtract", L"BzipCompressInMemory", L"BzipExtractInMemory" };
static const wchar_t *g_MethodNamesRu[] = { L"ƒобавить¬јрхив", L"»звлечь‘айлы»зјрхива",L"ƒобавить¬јрхив¬пам€ти", L"»звлечь‘айлы»зјрхива¬пам€ти" };

static const wchar_t *g_Errors[] = { L"BzipCompress", L"BzipExtract" };
static const wchar_t *g_ErrorsRu[] = { L"ƒобавить¬јрхив", L"»звлечь‘айлы»зјрхива" };

uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len = 0);
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len = 0);
uint32_t getLenShortWcharStr(const WCHAR_T* Source);

//---------------------------------------------------------------------------//
long GetClassObject(const wchar_t* wsName, IComponentBase** pInterface)
{
	try
	{
		if (!*pInterface)
		{
			*pInterface = new CAddInNative();
			return (long)*pInterface;
		}
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
	if (!*pIntf)
		return -1;

	delete *pIntf;
	*pIntf = 0;
	return 0;
}
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
	static WCHAR_T* names = 0;
	if (!names)
		::convToShortWchar(&names, g_kClassNames);
	return names;
}
//---------------------------------------------------------------------------//
//CAddInNative
CAddInNative::CAddInNative()
{
	m_iMemory = 0;
	m_iConnect = 0;

}
//---------------------------------------------------------------------------//
CAddInNative::~CAddInNative()
{
}
//---------------------------------------------------------------------------//
bool CAddInNative::Init(void* pConnection)
{
	m_iConnect = (IAddInDefBase*)pConnection;
	return m_iConnect != NULL;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetInfo()
{
	return 2000;
}
//---------------------------------------------------------------------------//
void CAddInNative::Done()
{
}
//---------------------------------------------------------------------------//
bool CAddInNative::RegisterExtensionAs(WCHAR_T** wsLanguageExt)
{
	wchar_t *wsExtension = L"bZIPAddIn";
	int iActualSize = ::wcslen(wsExtension) + 1;
	WCHAR_T* dest = 0;
	if (m_iMemory)
	{
		if (m_iMemory->AllocMemory((void**)wsLanguageExt, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(wsLanguageExt, wsExtension, iActualSize);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNProps()
{
	return eLastProp;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindProp(const WCHAR_T* wsPropName)
{
	long plPropNum = -1;
	wchar_t* propName = 0;

	::convFromShortWchar(&propName, wsPropName);
	plPropNum = findName(g_PropNames, propName, eLastProp);

	if (plPropNum == -1)
		plPropNum = findName(g_PropNamesRu, propName, eLastProp);

	delete[] propName;

	return plPropNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetPropName(long lPropNum, long lPropAlias)
{
	return 0;
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::SetPropVal(const long lPropNum, tVariant* varPropVal)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropReadable(const long lPropNum)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropWritable(const long lPropNum)
{
	return false;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNMethods()
{
	return eLastMethod;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindMethod(const WCHAR_T* wsMethodName)
{
	long plMethodNum = -1;
	wchar_t* name = 0;

	::convFromShortWchar(&name, wsMethodName);

	plMethodNum = findName(g_MethodNames, name, eLastMethod);

	if (plMethodNum == -1)
		plMethodNum = findName(g_MethodNamesRu, name, eLastMethod);

	return plMethodNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetMethodName(const long lMethodNum,
	const long lMethodAlias)
{
	if (lMethodNum >= eLastMethod)
		return NULL;

	wchar_t *wsCurrentName = NULL;
	WCHAR_T *wsMethodName = NULL;
	int iActualSize = 0;

	switch (lMethodAlias)
	{
	case 0: // First language
		wsCurrentName = (wchar_t*)g_MethodNames[lMethodNum];
		break;
	case 1: // Second language
		wsCurrentName = (wchar_t*)g_MethodNamesRu[lMethodNum];
		break;
	default:
		return 0;
	}

	iActualSize = wcslen(wsCurrentName) + 1;

	if (m_iMemory && wsCurrentName)
	{
		if (m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
	}

	return wsMethodName;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNParams(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case BzipCompress:
	case BzipExtract:
		return 2;

	case BzipCompressInMemory:
	case BzipExtractInMemory:
		return 1;

	default:
		return 0;
	}
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetParamDefValue(const long lMethodNum, const long lParamNum,
	tVariant *pvarParamDefValue)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::HasRetVal(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case BzipCompress:
	case BzipExtract:	
	case BzipCompressInMemory:
	case BzipExtractInMemory:

		return true;

	default:
		return false;
	}
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsProc(const long lMethodNum,
	tVariant* paParams, const long lSizeArray)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsFunc(const long lMethodNum,
	tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{

	bool result = true;

	switch (lMethodNum)
	{
	case BzipCompress:

		if (!(TV_VT(paParams) == VTYPE_PWSTR) || !(TV_VT(paParams + 1) == VTYPE_PWSTR))
		{
			setVariantValue(pvarRetValue, false);
			return false;
		}

		fn_r = getStringFromVariant(paParams);	   // (char*)"C:\\temp\\051218.ware";
		fn_w = getStringFromVariant(paParams + 1); // (char*)"C:\\temp\\051218.ware.bz2";

		if (fn_r) {
			if ((fp_r = fopen(fn_r, "rb")) == NULL) {

				result = false;
			}
		}
		else {
			fp_r = stdin;
		}

		if (fn_w) {
			if ((fp_w = fopen(fn_w, "wb")) == NULL) {
				result = false;
				//strerror(errno);
			}
		}

		if (result)
		{

			mode[0] = 'w';
			mode[1] = '0' + level;
			mode[2] = '\0';

			if ((fn_w == NULL && (BZ2fp_w = BZ2_bzdopen(_fileno(stdout), mode)) == NULL)
				|| (fn_w != NULL && (BZ2fp_w = BZ2_bzopen(fn_w, mode)) == NULL)) {
				setVariantValue(pvarRetValue, false);
				return false;
			}
			while ((len = fread(buff, 1, 0x1000, fp_r)) > 0) {
				BZ2_bzwrite(BZ2fp_w, buff, len);
			}
			BZ2_bzclose(BZ2fp_w);
			if (fp_r != stdin)fclose(fp_r);
			if (fp_w != stdout)fclose(fp_w);
		}
		setVariantValue(pvarRetValue, result);
		
		return true;

	case BzipExtract:
		if (!(TV_VT(paParams) == VTYPE_PWSTR) || !(TV_VT(paParams + 1) == VTYPE_PWSTR))
		{
			setVariantValue(pvarRetValue, false);
			return false;
		}

		fn_r = getStringFromVariant(paParams);	   // "C:\\temp\\051218.ware.bak.bz2";
		fn_w = getStringFromVariant(paParams + 1); // "C:\\temp\\051218.ware";

		if (fn_w) {
			if ((fp_w = fopen(fn_w, "wb")) == NULL) {
	
				result = false;
			}
		}
		else {
			fp_w = stdout;
		}

		if (fn_r) {
			if ((fp_r = fopen(fn_r, "rb")) == NULL) {

				result = false;
			}
		}
		else {
			fp_w = stdout;
		}
		if (result)
		{

			if ((fn_r == NULL && (BZ2fp_r = BZ2_bzdopen(_fileno(stdin), "rb")) == NULL)
				|| (fn_r != NULL && (BZ2fp_r = BZ2_bzopen(fn_r, "rb")) == NULL)) {

				setVariantValue(pvarRetValue, false);
				return false;
			}
			while ((len = BZ2_bzread(BZ2fp_r, buff, 0x1000)) > 0) {
				fwrite(buff, 1, len, fp_w);
			}
			BZ2_bzclose(BZ2fp_r);
			if (fp_w != stdout) fclose(fp_w);
			if (fp_r != stdin)fclose(fp_r);
		}
		setVariantValue(pvarRetValue, result);
		return true;

	case BzipExtractInMemory:

		if (lSizeArray != 1 || !paParams)
			return false;

		if (TV_VT(paParams) != VTYPE_BLOB)
			return false;

		sourceLength = paParams->strLen;
		destLength = 25 * sourceLength + 600;    // Will change to real size.

		m_iMemory->AllocMemory((void**)&dest, destLength);
		returnCode = BZ2_bzBuffToBuffDecompress(dest, &destLength, paParams->pstrVal, sourceLength, 0, verbosity);

		if (returnCode == BZ_OK)
		{
			setVariantValue(pvarRetValue, dest);
		}
		else
		{
			return false;
		}

		return true;
	case BzipCompressInMemory:

		if (lSizeArray != 1 || !paParams)
			return false;

		if (TV_VT(paParams) != VTYPE_BLOB)
			return false;

		sourceLength = paParams->strLen;
		destLength = 1.01 * sourceLength + 600;    // Official formula, Big enough to hold output.  Will change to real size.
		
		m_iMemory->AllocMemory((void**)&dest, destLength);
		returnCode = BZ2_bzBuffToBuffCompress(dest, &destLength, paParams->pstrVal, sourceLength, blockSize100k, verbosity, workFactor);

		if (returnCode == BZ_OK)
		{
			setVariantValue(pvarRetValue, dest);
		}
		else
		{
			return false;
		}

		return true;
	default:
		return false;
	}
}
//---------------------------------------------------------------------------//
void CAddInNative::SetLocale(const WCHAR_T* loc)
{
#ifndef __linux__
	_wsetlocale(LC_ALL, loc);
#else
	int size = 0;
	char *mbstr = 0;
	wchar_t *tmpLoc = 0;
	convFromShortWchar(&tmpLoc, loc);
	size = wcstombs(0, tmpLoc, 0) + 1;
	mbstr = new char[size];

	if (!mbstr)
	{
		delete[] tmpLoc;
		return;
	}

	memset(mbstr, 0, size);
	size = wcstombs(mbstr, tmpLoc, wcslen(tmpLoc));
	setlocale(LC_ALL, mbstr);
	delete[] tmpLoc;
	delete[] mbstr;
#endif
}
//---------------------------------------------------------------------------//
bool CAddInNative::setMemManager(void* mem)
{
	m_iMemory = (IMemoryManager*)mem;
	return m_iMemory != 0;
}

long CAddInNative::findName(const wchar_t* names[], const wchar_t* name,
	const uint32_t size) const
{
	long ret = -1;
	for (uint32_t i = 0; i < size; i++)
	{
		if (!wcscmp(names[i], name))
		{
			ret = i;
			break;
		}
	}
	return ret;
}


bool CAddInNative::wstring_to_p(std::wstring str, tVariant* val) {
	char* t1;
	TV_VT(val) = VTYPE_PWSTR;
	m_iMemory->AllocMemory((void**)&t1, (str.length() + 1) * sizeof(WCHAR_T));
	memcpy(t1, str.c_str(), (str.length() + 1) * sizeof(WCHAR_T));
	val->pstrVal = t1;
	val->strLen = str.length();
	return true;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring CAddInNative::utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

std::string CAddInNative::ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

// converts the wide char array into a multibyte one
char* CAddInNative::wideCharToMultiByte(const wchar_t *lpwstr)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, NULL, 0, NULL, NULL);

	char *buffer = new char[size + 1];
	WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, buffer, size, NULL, NULL);

	return buffer;
}
//---------------------------------------------------------------------------//
char* CAddInNative::getStringFromVariant(tVariant* paParams)
{
	char *OdinSstring = 0;
	int size = 0;
	char *mbstr = 0;
	wchar_t* wsTmp = 0;
	char* loc = 0;

	switch (TV_VT(paParams))
	{
	case VTYPE_PSTR:
		OdinSstring = paParams->pstrVal;
		break;
	case VTYPE_PWSTR:
		loc = setlocale(LC_ALL, "");
		::convFromShortWchar(&wsTmp, TV_WSTR(paParams));
		size = wcstombs(0, wsTmp, 0) + 1;
		assert(size);
		mbstr = new char[size];
		assert(mbstr);
		memset(mbstr, 0, size);
		size = wcstombs(mbstr, wsTmp, getLenShortWcharStr(TV_WSTR(paParams)));
		OdinSstring = mbstr;
		setlocale(LC_ALL, loc);
		delete[] wsTmp;
		break;
	}
		 
	return OdinSstring;
}

//---------------------------------------------------------------------------//
void CAddInNative::addError(uint32_t wcode, const wchar_t* source,
	const wchar_t* descriptor, long code)
{
	if (m_iConnect)
	{
		WCHAR_T *err = 0;
		WCHAR_T *descr = 0;

		::convToShortWchar(&err, source);
		::convToShortWchar(&descr, descriptor);

		m_iConnect->AddError(wcode, err, descr, code);
		delete[] err;
		delete[] descr;
	}
}

void CAddInNative::setVariantValue(tVariant* paParams, bool value)
{
	TV_VT(paParams) = VTYPE_BOOL;
	TV_BOOL(paParams) = value;
}

void CAddInNative::setVariantValue(tVariant* paParams, char* value)
{
	m_iMemory->AllocMemory((void**)&paParams->pstrVal, destLength);
	TV_VT(paParams) = VTYPE_BLOB;
	memcpy((void*)paParams->pstrVal, (void*)value, destLength);
	paParams->strLen = destLength;
}

uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len)
{
	if (!len)
		len = ::wcslen(Source) + 1;

	if (!*Dest)
		*Dest = new WCHAR_T[len];

	WCHAR_T* tmpShort = *Dest;
	wchar_t* tmpWChar = (wchar_t*)Source;
	uint32_t res = 0;

	::memset(*Dest, 0, len * sizeof(WCHAR_T));
	do
	{
		*tmpShort++ = (WCHAR_T)*tmpWChar++;
		++res;
	} while (len-- && *tmpWChar);

	return res;
}
//---------------------------------------------------------------------------//
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len)
{
	if (!len)
		len = getLenShortWcharStr(Source) + 1;

	if (!*Dest)
		*Dest = new wchar_t[len];

	wchar_t* tmpWChar = *Dest;
	WCHAR_T* tmpShort = (WCHAR_T*)Source;
	uint32_t res = 0;

	::memset(*Dest, 0, len * sizeof(wchar_t));
	do
	{
		*tmpWChar++ = (wchar_t)*tmpShort++;
		++res;
	} while (len-- && *tmpShort);

	return res;
}
//---------------------------------------------------------------------------//
uint32_t getLenShortWcharStr(const WCHAR_T* Source)
{
	uint32_t res = 0;
	WCHAR_T *tmpShort = (WCHAR_T*)Source;

	while (*tmpShort++)
		++res;

	return res;
}
//---------------------------------------------------------------------------//
