//--------------------------------------------------------------
/*!	@file
	@brief	文字列処理
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{
namespace String{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	文字列コピー

@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void Strcpy_s(hj_c8* pc8Dst, hj_size_t sizeDst, const hj_c8* opc8Src)
{
	::strcpy_s(pc8Dst, sizeDst, opc8Src);
}
void Wcscpy_s(hj_wc16* pc16Dst, hj_size_t sizeDst, const hj_wc16* opc16Src)
{
	::wcscpy_s(pc16Dst, sizeDst, opc16Src);
}
void Tcscpy_s(hj_tchar* ptcDst, hj_size_t sizeDst, const hj_tchar* optcSrc)
{
	::_tcscpy_s(ptcDst, sizeDst, optcSrc);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	文字列コピー

@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void Sprintf_s(hj_c8* pc8Dst, hj_size_t sizeDst, const hj_c8* opc8Format, ...)
{
	va_list arg;
    va_start(arg, opc8Format);
    vsprintf_s(pc8Dst, sizeDst, opc8Format, arg);
    va_end(arg);
}

void Wsprintf_s(hj_wc16* pc16Dst, hj_size_t sizeDst, const hj_wc16* opwc16Format, ...)
{
	va_list arg;
    va_start(arg, opwc16Format);
    vswprintf_s(pc16Dst, sizeDst, opwc16Format, arg);
    va_end(arg);
}

void Tcsprintf_s(hj_tchar* ptcDst, hj_size_t sizeDst, const hj_tchar* optcFormat, ...)
{
	va_list arg;
    va_start(arg, optcFormat);
    _vstprintf_s(ptcDst, sizeDst, optcFormat, arg);
    va_end(arg);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	文字列長さ

@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_size_t Strlen(const hj_c8* pc8Str)
{
	return ::strlen(pc8Str);
}
hj_size_t Wcslen(const hj_wc16* pc16Str)
{
	return ::wcslen(pc16Str);
}
hj_size_t Tcslen(const hj_tchar* ptcStr)
{
	return ::_tcslen(ptcStr);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	比較

@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_s32 Strcmp(const hj_c8* pc8Str1, const hj_c8* pc8Str2)
{
	return ::strcmp(pc8Str1, pc8Str2);
}
hj_s32 Wcscmp(const hj_wc16* pc16Str1, const hj_wc16* pc16Str2)
{
	return ::wcscmp(pc16Str1, pc16Str2);
}
hj_s32 Tcscmp(const hj_tchar* ptcStr1, const hj_tchar* ptcStr2)
{
	return ::_tcscmp(ptcStr1, ptcStr2);
}

}// namespace String
}// namespace hj