//--------------------------------------------------------------
/*!	@file
	@brief	HJLibシステム
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

namespace{
	const hj_u32 tou32MaxBuff = 1024;
	hj_f32 m_f32DeltaTime	= 0.f;
	hj_f32 m_f32GameTime = 0.f;

	void OR_VA_LIST(const hj_tchar* optcEx, const va_list& argptr)
	{
		hj_tchar atcBuf[tou32MaxBuff];
		_vstprintf_s(atcBuf, tou32MaxBuff, optcEx, argptr);	// 文字数指定だった・・・
		OutputDebugString( atcBuf );
		OutputDebugString( HJ_T("\n") );
	}
} // namespace

// デバッグ出力
void OR(const hj_tchar* optcEx, ...)
{
	va_list argptr;
	va_start(argptr, optcEx);
	OR_VA_LIST(optcEx, argptr);
	va_end(argptr);
}
// アサート
void HJ_ASSERT(hj_bool b, const hj_tchar* optcEx, ...)
{
	if( b ){
		return;
	}
	va_list argptr;
	va_start(argptr, optcEx);
	OR_VA_LIST(optcEx, argptr);
	va_end(argptr);
	assert(false);
}
// 強制アサート
void HJ_ASSERT_MSG( const hj_tchar* optcEx, ...)
{
	va_list argptr;
	va_start(argptr, optcEx);
	OR_VA_LIST(optcEx, argptr);
	va_end(argptr);
	assert(false);
}

// HJLib namespace
namespace hj{
namespace System{

// memcpy
void* Memcpy(void *buf1, const void *buf2, hj_size_t n){
	return ::memcpy( buf1, buf2, n);
}
// memset
void* Memset(void *buf, hj_s32 ch, hj_size_t n){
	return ::memset( buf, ch, n);
}

// 毎フレームの経過時間(秒)
void SetDeltaTime(hj_f32 f32DeltaTime){
	m_f32DeltaTime = f32DeltaTime;
}
hj_f32 GetDeltaTime(){
	return m_f32DeltaTime;
}

// ゲーム時間加算(秒)
void AddGameTime(hj_f32 f32DeltaTime){
	m_f32GameTime += f32DeltaTime;
}
// ゲーム時間取得
hj_f32 GetGameTime(){
	return m_f32GameTime;
}

// スレッドをスリープ
void Sleep(hj_u32 u32MSec)
{
#ifdef PF_WINDX
	::Sleep(u32MSec);
#else
	HJ_ASSERT(false);
#endif
}

} // namespace System
} // namespace hj
