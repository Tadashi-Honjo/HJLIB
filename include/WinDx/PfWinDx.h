//--------------------------------------------------------------
/*!	@file
	@brief	プラットフォームヘッダ
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __PF_WINDX_H_
#define __PF_WINDX_H_

//DirectXオブジェクト開放マクロ
#define HJ_SAFE_RELEASE(p) { if(p){ (p)->Release(); (p)=NULL;}}

// HJLib namespace
namespace hj{
	// --- Structure
	//! 初期化パラメータ
	struct stcPfInitParam{
		hj_tchar	atcApplicationName[128];	//!< アプリケーション名
		hj_tchar	atcIconResourceName[128];	//!< アイコンのリソース名
		hj_u32		u32MainHeapSize;			//!< メインヒープサイズ
		hj_s32		s32ScreenWidth;				//!< ScreenSizeW
		hj_s32		s32ScreenHeight;			//!< ScreenSizeH
		hj_bool		bFullScreen;				//!< フルスクリーン

		HINSTANCE	hInst;
		INT			iCmdShow;
	};
} // namespace hj

// --- Include
#include "WinDxMain.h"
#include "WinDxWindow.h"
#include "WinDxCoreMgr.h"
#include "WinDxGraphics.h"
#include "WinDxTexture.h"
#include "WinDxRenderTarget.h"
#include "WinDxDraw2d.h"
#include "WinDxDraw3d.h"
#include "WinDxScene.h"
#include "WinDxSprite.h"
#include "WinDxInput.h"
#include "WinDxTime.h"
#include "WinDxShader.h"
#include "WinDxSound.h"
#include "WinDxSoundWave.h"
#include "WinDxFileMgr.h"
#include "WinDxMesh.h"
#include "WinDxModel.h"
#include "WinDxThread.h"
#include "WinDxCriticalSection.h"

// HJLib namespace
namespace hj{

// --- Typedef
typedef clsWinDxMain			clsPfMain;
typedef clsWinDxTexture			clsPfTexture;
typedef clsWinDxSprite			clsPfSprite;
typedef clsWinDxRenderTarget	clsPfRenderTarget;
typedef clsWinDxScene			clsPfScene;
typedef clsWinDxShader			clsPfShader;
typedef clsHJParticleEmitter2d	clsPfParticleEmitter2d;
typedef clsHJAsciiFont			clsPfAsciiFont;
typedef clsWinDxSoundWave		clsPfSound;
typedef clsWinDxStreamSoundWave	clsPfStreamSound;
typedef clsWinDxMesh			clsPfMesh;
typedef clsWinDxModel			clsPfModel;
typedef clsWinDxThread			clsPfThread;
typedef clsWinDxCriticalSection clsPfCriticalSection;
typedef clsHJCamera				clsPfCamera;
typedef clsHJCamera2d			clsPfCamera2d;

} // namespace hj

#endif // __PF_WINDX_H_
