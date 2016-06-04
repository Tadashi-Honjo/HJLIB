//--------------------------------------------------------------
/*!	@file
	@brief	HJUtility
			
	@author 本城 直志
	@date	11/11/15
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_UTILITY_H_
#define __HJ_UTILITY_H_

#if defined(UNICODE)
	#define HJ_UNICODE
#endif

//------Typedef
//本城 基本型
typedef signed		char	hj_s8;		//!<本城 signed 8bit
typedef unsigned	char	hj_u8;		//!<本城 unsigned 8bit
typedef signed		short	hj_s16;		//!<本城 signed 16bit
typedef unsigned	short	hj_u16;		//!<本城 unsigned 16bit
typedef signed		int		hj_s32;		//!<本城 signed 32bit
typedef unsigned	int		hj_u32;		//!<本城 unsigned 32bit
typedef				float	hj_f32;		//!<本城 hj_f32 32bit
typedef				double	hj_f64;		//!<本城 hj_f32 64bit
typedef				bool	hj_bool;	//!<本城 boolean
typedef	unsigned	int		hj_color32;	//!<本城 color
typedef size_t				hj_size_t;	//!<本城 size_t

//本城 文字型
typedef char				hj_c8;		//!<本城 char 8bit
typedef wchar_t				hj_wc16;	//!<本城 wchar 16bit
#if defined(HJ_UNICODE)
	typedef hj_wc16			hj_tchar;	//!<本城 tchar
#else
	typedef hj_c8			hj_tchar;	//!<本城 tchar
#endif // HJ_UNICODE

//------Define
//本城 ビットフラグ
#define HJ_BIT_0	(0x1)			//!<本城 0Bit
#define HJ_BIT_1	(0x2)			//!<本城 1Bit
#define HJ_BIT_2	(0x4)			//!<本城 2Bit
#define HJ_BIT_3	(0x8)			//!<本城 3Bit
#define HJ_BIT_4	(0x10)			//!<本城 4Bit
#define HJ_BIT_5	(0x20)			//!<本城 5Bit
#define HJ_BIT_6	(0x40)			//!<本城 6Bit
#define HJ_BIT_7	(0x80)			//!<本城 7Bit
#define HJ_BIT_8	(0x100)			//!<本城 8Bit
#define HJ_BIT_9	(0x200)			//!<本城 9Bit
#define HJ_BIT_10	(0x400)			//!<本城 10Bit
#define HJ_BIT_11	(0x800)			//!<本城 11Bit
#define HJ_BIT_12	(0x1000)		//!<本城 12Bit
#define HJ_BIT_13	(0x2000)		//!<本城 13Bit
#define HJ_BIT_14	(0x4000)		//!<本城 14Bit
#define HJ_BIT_15	(0x8000)		//!<本城 15Bit
#define HJ_BIT_16	(0x10000)		//!<本城 16Bit
#define HJ_BIT_17	(0x20000)		//!<本城 17Bit
#define HJ_BIT_18	(0x40000)		//!<本城 18Bit
#define HJ_BIT_19	(0x80000)		//!<本城 19Bit
#define HJ_BIT_20	(0x100000)		//!<本城 20Bit
#define HJ_BIT_21	(0x200000)		//!<本城 21Bit
#define HJ_BIT_22	(0x400000)		//!<本城 22Bit
#define HJ_BIT_23	(0x800000)		//!<本城 23Bit
#define HJ_BIT_24	(0x1000000)		//!<本城 24Bit
#define HJ_BIT_25	(0x2000000)		//!<本城 25Bit
#define HJ_BIT_26	(0x4000000)		//!<本城 26Bit
#define HJ_BIT_27	(0x8000000)		//!<本城 27Bit
#define HJ_BIT_28	(0x10000000)	//!<本城 28Bit
#define HJ_BIT_29	(0x20000000)	//!<本城 29Bit
#define HJ_BIT_30	(0x40000000)	//!<本城 30Bit
#define HJ_BIT_31	(0x80000000)	//!<本城 31Bit

///Unicode対応マクロ
#if defined(HJ_UNICODE)
	#define HJ_T(text)	L##text
#else
	#define HJ_T(text)	text
#endif // HJ_UNICODE

///本城 デリートマクロ
#define HJ_SAFE_DELETE(p)			{if(p){delete p; p=0;}};
#define HJ_SAFE_DELETE_ARRAY(p)		{if(p){delete[] p; p=0;}};
//	#define HJ_SAFE_SINGLETON_DELETE(v)	{if(v::GS()){delete(v::GS());}};	//本城 これいるかな？　現在はC++はNULLを削除しても何もならないので

//本城 メモリーリークチェック
#define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

//本城 配列
#define HJ_ARRAY_NUM(a)	(sizeof(a)/sizeof(a[0]))

// ダウンキャスト用(非推奨だけど一応)
template <class Target, class Source> inline Target polymorphic_downcast(Source p){
#ifdef HJ_DEBUG
	SUPER_ASSERT(dynamic_cast<Target>(p) == p, 0x05125415, HJ_T("polymorphic_downcast assert!!"));
#endif
	return static_cast<Target>(p);
}

#define HJ_STATIC_ASSERT(expr) typedef char STATIC_ASSERT_TYPE[(expr) ? 1 : -1]

#endif // __HJ_UTILITY_H_