//--------------------------------------------------------------
/*!	@file
	@brief	HJLibシステム
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_SYSTEM_H_
#define __HJ_LIB_SYSTEM_H_

//------Define
#if defined(_UNICODE)
	#define HJ_UNICODE	// HJLib用UNICODE定義
#endif

// プリプロセッサに変更
// デバッグ定義
#if defined(_DEBUG)
	#define HJ_DEBUG
#else
	#define HJ_RELEASE
#endif

//------Typedef
typedef signed		char	hj_s8;		//!< signed 8bit
typedef unsigned	char	hj_u8;		//!< unsigned 8bit
typedef signed		short	hj_s16;		//!< signed 16bit
typedef unsigned	short	hj_u16;		//!< unsigned 16bit
typedef signed		int		hj_s32;		//!< signed 32bit
typedef unsigned	int		hj_u32;		//!< unsigned 32bit
typedef				float	hj_f32;		//!< hj_f32 32bit
typedef				double	hj_f64;		//!< hj_f32 64bit
typedef				int		hj_int;		//!< int
typedef				float	hj_float;	//!< float
typedef				double	hj_double;	//!< double
typedef				bool	hj_bool;	//!< boolean
typedef	unsigned	int		hj_color32;	//!< color
typedef size_t				hj_size_t;	//!< size_t
typedef char				hj_c8;		//!< char 8bit
typedef wchar_t				hj_wc16;	//!< wchar 16bit
#if defined(HJ_UNICODE)
	typedef hj_wc16			hj_tchar;	//!< tchar
#else
	typedef hj_c8			hj_tchar;	//!< tchar
#endif


// STL
//typedef ::std::vector			hj_vector;
#define hj_vector ::std::vector // 実実装するまではdefine代用


//------Define
// 最大値
#define HJ_S8_MIN (SCHAR_MIN)
#define HJ_S8_MAX (SCHAR_MAX)
#define HJ_U8_MAX (UCHAR_MAX)
#define HJ_S16_MIN (SHRT_MIN)
#define HJ_S16_MAX (SHRT_MAX)
#define HJ_U16_MAX (USHRT_MAX)
#define HJ_S32_MIN (INT_MIN)
#define HJ_S32_MAX (INT_MAX)
#define HJ_U32_MAX (UINT_MAX)
#define HJ_F32_MIN (FLT_MIN)
#define HJ_F32_MAX (FLT_MAX)
#define HJ_F64_MIN (DBL_MIN)
#define HJ_F64_MAX (DBL_MAX)

// ビットフラグ
#define HJ_BIT_0	(0x1)			//!< 0Bit
#define HJ_BIT_1	(0x2)			//!< 1Bit
#define HJ_BIT_2	(0x4)			//!< 2Bit
#define HJ_BIT_3	(0x8)			//!< 3Bit
#define HJ_BIT_4	(0x10)			//!< 4Bit
#define HJ_BIT_5	(0x20)			//!< 5Bit
#define HJ_BIT_6	(0x40)			//!< 6Bit
#define HJ_BIT_7	(0x80)			//!< 7Bit
#define HJ_BIT_8	(0x100)			//!< 8Bit
#define HJ_BIT_9	(0x200)			//!< 9Bit
#define HJ_BIT_10	(0x400)			//!< 10Bit
#define HJ_BIT_11	(0x800)			//!< 11Bit
#define HJ_BIT_12	(0x1000)		//!< 12Bit
#define HJ_BIT_13	(0x2000)		//!< 13Bit
#define HJ_BIT_14	(0x4000)		//!< 14Bit
#define HJ_BIT_15	(0x8000)		//!< 15Bit
#define HJ_BIT_16	(0x10000)		//!< 16Bit
#define HJ_BIT_17	(0x20000)		//!< 17Bit
#define HJ_BIT_18	(0x40000)		//!< 18Bit
#define HJ_BIT_19	(0x80000)		//!< 19Bit
#define HJ_BIT_20	(0x100000)		//!< 20Bit
#define HJ_BIT_21	(0x200000)		//!< 21Bit
#define HJ_BIT_22	(0x400000)		//!< 22Bit
#define HJ_BIT_23	(0x800000)		//!< 23Bit
#define HJ_BIT_24	(0x1000000)		//!< 24Bit
#define HJ_BIT_25	(0x2000000)		//!< 25Bit
#define HJ_BIT_26	(0x4000000)		//!< 26Bit
#define HJ_BIT_27	(0x8000000)		//!< 27Bit
#define HJ_BIT_28	(0x10000000)	//!< 28Bit
#define HJ_BIT_29	(0x20000000)	//!< 29Bit
#define HJ_BIT_30	(0x40000000)	//!< 30Bit
#define HJ_BIT_31	(0x80000000)	//!< 31Bit

// ビット処理
#define HJ_BIT_CHECK(flg, bit) (flg & bit)
#define HJ_BIT_CLEAR(flg, bit) (flg = (flg & ~bit))

// サイズ用
#define HJ_KB(n) (n*1024)
#define HJ_MB(n) (n*1048576)


///Unicode対応マクロ
#if defined(HJ_UNICODE)
	#define HJ_T(text)	L##text
#else
	#define HJ_T(text)	text
#endif // HJ_UNICODE

// --- Function
void OR(const hj_tchar* optcEx, ...);												//!< デバッグ出力(デバッグコンソール)
void HJ_ASSERT_MSG(const hj_tchar* optcEx, ...);									//!< アサート
void HJ_ASSERT(hj_bool b, const hj_tchar* optcEx, ...);								//!< アサート

// --- Math
#ifndef HJ_MAX
#define HJ_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef HJ_MIN
#define HJ_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// --- file
#ifndef HJ_MAX_PATH
	#define HJ_MAX_PATH (128)
#endif

// HJLib namespace
namespace hj{
namespace System{
	// memcpy
	void* Memcpy(void *buf1, const void *buf2, hj_size_t n);
	// memset
	void* Memset(void *buf, hj_s32 ch, hj_size_t n);

	// 毎フレームの経過時間(秒)
	void	SetDeltaTime(hj_f32 f32DeltaTime);	// 外部からは使用禁止
	hj_f32  GetDeltaTime();

	// ゲーム起動からの時間(秒)
	void	AddGameTime(hj_f32 f32DeltaTime);	// 外部からは使用禁止
	hj_f32  GetGameTime();

	// スレッドをスリープ
	void	Sleep(hj_u32 u32MSec);

}// namespace System
}// namespace hj

// ダウンキャスト用(非推奨だけど一応)
template <class Target, class Source> inline Target polymorphic_downcast(Source p){
#ifdef HJ_DEBUG
	HJ_ASSERT(dynamic_cast<Target>(p) == p, HJ_T("polymorphic_downcast assert!!"));
#endif
	return static_cast<Target>(p);
}


#define HJ_STATIC_ASSERT(expr) typedef char STATIC_ASSERT_TYPE[(expr) ? 1 : -1]

//------Inline
#if 0	// 10/10/14 いるのか不明なので消しておく
/// エラー出力(ErrorPrint)
inline void EP(const wchar_t* wcMessage){
	cout << "Error: " << wcMessage << endl;
}
/// SDLエラー出力(SDLErrorPrint)
inline void SDL_EP(const wchar_t* wcMessage){
	cout << "SDLError: " << wcMessage << ": " << SDL_GetError() << endl;
}

// エンディアン変換(Big⇔Little)
inline s16 ConvertEndian(s16 data){
	return (data << 8) | (data >> 8);
}
inline u16 ConvertEndian(u16 data){
	return (data << 8) | (data >> 8);
}
inline s32 ConvertEndian(s32 data){
	return (data << 24) | ((data & BYTE_1) << 8) | ((data & BYTE_2) >> 8) | (data >> 24);
}
inline u32 ConvertEndian(u32 data){
	return (data << 24) | ((data & BYTE_1) << 8) | ((data & BYTE_2) >> 8) | (data >> 24);
}
/* 浮動少数は論理演算ができないのでまた今度修正します
inline f32 ConvertEndian(f32 data){
	return (data << 24) | ((data & BYTE_1) << 8) | ((data & BYTE_2) >> 8) | (data >> 24);
}
inline f64 ConvertEndian(f64 data){
	return (data << 56) | ((data & BYTE_1) << 40) | ((data & BYTE_2) << 24) | ((data & BYTE_3) << 8) |
		(data >> 56) | ((data & BYTE_6) >> 40) | ((data & BYTE_5) >> 24) | ((data & BYTE_4) >> 8));
}
*/

//------Function
static void errorAndExit(const char* message)
{
	printf("%s: %s\n", message, SDL_GetError());
	Exit(1);
}
#endif

#endif // __HJ_LIB_SYSTEM_H_