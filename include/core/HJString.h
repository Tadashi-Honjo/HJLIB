//--------------------------------------------------------------
/*!	@file
	@brief	文字列処理
			とりあえずラップしておく
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_STRING_H_
#define __HJ_LIB_STRING_H_

// HJLib namespace
namespace hj{
namespace String{

	//文字列コピー
	void Strcpy_s(hj_c8* pc8Dst, hj_size_t sizeDst, const hj_c8* opc8Src);
	void Wcscpy_s(hj_wc16* pc16Dst, hj_size_t sizeDst, const hj_wc16* opc16Src);
	void Tcscpy_s(hj_tchar* ptcDst, hj_size_t sizeDst, const hj_tchar* optcSrc);

	// 文字列生成
	void Sprintf_s(hj_c8* pc8Dst, hj_size_t sizeDst, const hj_c8* opc8Format, ...);
	void Wsprintf_s(hj_wc16* pc16Dst, hj_size_t sizeDst, const hj_wc16* opwc16Format, ...);
	void Tcsprintf_s(hj_tchar* ptcDst, hj_size_t sizeDst, const hj_tchar* optcFormat, ...);

	// 文字列長さ
	hj_size_t Strlen(const hj_c8* pc8Str);
	hj_size_t Wcslen(const hj_wc16* pc16Str);
	hj_size_t Tcslen(const hj_tchar* ptcStr);

	// 比較
	hj_s32 Strcmp(const hj_c8* pc8Str1, const hj_c8* pc8Str2);
	hj_s32 Wcscmp(const hj_wc16* pc16Str1, const hj_wc16* pc16Str2);
	hj_s32 Tcscmp(const hj_tchar* ptcStr1, const hj_tchar* ptcStr2);

}// namespace String
}// namespace hj

#endif // __HJ_LIB_STRING_H_