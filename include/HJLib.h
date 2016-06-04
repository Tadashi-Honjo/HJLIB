//--------------------------------------------------------------
/*!	@file
	@brief	HJLibCore
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_H_
#define __HJ_LIB_H_

// プラットフォームチェック＆事前インクルード
#if defined(PF_WINDX)
	#include "WinDx/WinDxInit.h"		// WinDx
#elif defined(PF_WINGLES)
	#include "WinGlEs/WinGlEsInit.h"	// WinGlEs
#else
	#error "Platform Non Define!!"
#endif

// --- Include
#include "core/HJCore.h"	// コア

// プラットフォーム
#if defined(PF_WINDX)
	#include "WinDx/PfWinDx.h"		// WinDx
#elif defined(PF_WINGLES)
	#include "WinGlEs/PfWinGlEs.h"	// WinGlEs
#endif


#endif // __HJ_LIB_H_