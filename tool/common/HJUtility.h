//--------------------------------------------------------------
/*!	@file
	@brief	HJUtility
			
	@author �{�� ���u
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
//�{�� ��{�^
typedef signed		char	hj_s8;		//!<�{�� signed 8bit
typedef unsigned	char	hj_u8;		//!<�{�� unsigned 8bit
typedef signed		short	hj_s16;		//!<�{�� signed 16bit
typedef unsigned	short	hj_u16;		//!<�{�� unsigned 16bit
typedef signed		int		hj_s32;		//!<�{�� signed 32bit
typedef unsigned	int		hj_u32;		//!<�{�� unsigned 32bit
typedef				float	hj_f32;		//!<�{�� hj_f32 32bit
typedef				double	hj_f64;		//!<�{�� hj_f32 64bit
typedef				bool	hj_bool;	//!<�{�� boolean
typedef	unsigned	int		hj_color32;	//!<�{�� color
typedef size_t				hj_size_t;	//!<�{�� size_t

//�{�� �����^
typedef char				hj_c8;		//!<�{�� char 8bit
typedef wchar_t				hj_wc16;	//!<�{�� wchar 16bit
#if defined(HJ_UNICODE)
	typedef hj_wc16			hj_tchar;	//!<�{�� tchar
#else
	typedef hj_c8			hj_tchar;	//!<�{�� tchar
#endif // HJ_UNICODE

//------Define
//�{�� �r�b�g�t���O
#define HJ_BIT_0	(0x1)			//!<�{�� 0Bit
#define HJ_BIT_1	(0x2)			//!<�{�� 1Bit
#define HJ_BIT_2	(0x4)			//!<�{�� 2Bit
#define HJ_BIT_3	(0x8)			//!<�{�� 3Bit
#define HJ_BIT_4	(0x10)			//!<�{�� 4Bit
#define HJ_BIT_5	(0x20)			//!<�{�� 5Bit
#define HJ_BIT_6	(0x40)			//!<�{�� 6Bit
#define HJ_BIT_7	(0x80)			//!<�{�� 7Bit
#define HJ_BIT_8	(0x100)			//!<�{�� 8Bit
#define HJ_BIT_9	(0x200)			//!<�{�� 9Bit
#define HJ_BIT_10	(0x400)			//!<�{�� 10Bit
#define HJ_BIT_11	(0x800)			//!<�{�� 11Bit
#define HJ_BIT_12	(0x1000)		//!<�{�� 12Bit
#define HJ_BIT_13	(0x2000)		//!<�{�� 13Bit
#define HJ_BIT_14	(0x4000)		//!<�{�� 14Bit
#define HJ_BIT_15	(0x8000)		//!<�{�� 15Bit
#define HJ_BIT_16	(0x10000)		//!<�{�� 16Bit
#define HJ_BIT_17	(0x20000)		//!<�{�� 17Bit
#define HJ_BIT_18	(0x40000)		//!<�{�� 18Bit
#define HJ_BIT_19	(0x80000)		//!<�{�� 19Bit
#define HJ_BIT_20	(0x100000)		//!<�{�� 20Bit
#define HJ_BIT_21	(0x200000)		//!<�{�� 21Bit
#define HJ_BIT_22	(0x400000)		//!<�{�� 22Bit
#define HJ_BIT_23	(0x800000)		//!<�{�� 23Bit
#define HJ_BIT_24	(0x1000000)		//!<�{�� 24Bit
#define HJ_BIT_25	(0x2000000)		//!<�{�� 25Bit
#define HJ_BIT_26	(0x4000000)		//!<�{�� 26Bit
#define HJ_BIT_27	(0x8000000)		//!<�{�� 27Bit
#define HJ_BIT_28	(0x10000000)	//!<�{�� 28Bit
#define HJ_BIT_29	(0x20000000)	//!<�{�� 29Bit
#define HJ_BIT_30	(0x40000000)	//!<�{�� 30Bit
#define HJ_BIT_31	(0x80000000)	//!<�{�� 31Bit

///Unicode�Ή��}�N��
#if defined(HJ_UNICODE)
	#define HJ_T(text)	L##text
#else
	#define HJ_T(text)	text
#endif // HJ_UNICODE

///�{�� �f���[�g�}�N��
#define HJ_SAFE_DELETE(p)			{if(p){delete p; p=0;}};
#define HJ_SAFE_DELETE_ARRAY(p)		{if(p){delete[] p; p=0;}};
//	#define HJ_SAFE_SINGLETON_DELETE(v)	{if(v::GS()){delete(v::GS());}};	//�{�� ���ꂢ�邩�ȁH�@���݂�C++��NULL���폜���Ă������Ȃ�Ȃ��̂�

//�{�� �������[���[�N�`�F�b�N
#define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

//�{�� �z��
#define HJ_ARRAY_NUM(a)	(sizeof(a)/sizeof(a[0]))

// �_�E���L���X�g�p(�񐄏������ǈꉞ)
template <class Target, class Source> inline Target polymorphic_downcast(Source p){
#ifdef HJ_DEBUG
	SUPER_ASSERT(dynamic_cast<Target>(p) == p, 0x05125415, HJ_T("polymorphic_downcast assert!!"));
#endif
	return static_cast<Target>(p);
}

#define HJ_STATIC_ASSERT(expr) typedef char STATIC_ASSERT_TYPE[(expr) ? 1 : -1]

#endif // __HJ_UTILITY_H_