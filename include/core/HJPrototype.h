//--------------------------------------------------------------
/*!	@file
	@brief	HJPrototype宣言
			
	@author 本城 直志
	@date	15/01/31
*/
//	Copyright (C) 2015 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_PROTOTYPE_H_
#define __HJ_PROTOTYPE_H_

// --- System Include
#include "HJSharedPtr.h"

// HJLib namespace
namespace hj{

class clsHJHeapMemoryMgr;
typedef clsHJSharedPtr< clsHJHeapMemoryMgr > SpHJHeapMemoryMgr;

class clsHJGraphics;
typedef clsHJSharedPtr< clsHJGraphics > SpHJGraphics;

class clsHJDraw2d;
typedef clsHJSharedPtr< clsHJDraw2d > SpHJDraw2d;

class clsHJDraw3d;
typedef clsHJSharedPtr< clsHJDraw3d > SpHJDraw3d;

class clsHJDrawPrimitive;
typedef clsHJSharedPtr< clsHJDrawPrimitive > SpHJDrawPrimitive;

class clsHJTime;
typedef clsHJSharedPtr< clsHJTime > SpHJTime;

class clsHJFpsMgr;
typedef clsHJSharedPtr< clsHJFpsMgr > SpHJFpsMgr;

class clsHJInput;
typedef clsHJSharedPtr< clsHJInput > SpHJInput;

class clsHJFileMgr;
typedef clsHJSharedPtr< clsHJFileMgr > SpHJFileMgr;

class clsHJMain;
typedef clsHJSharedPtr< clsHJMain > SpHJMain;

class clsHJTexture;
typedef clsHJSharedPtr< clsHJTexture > SpHJTexture;

class clsHJSprite;
typedef clsHJSharedPtr< clsHJSprite > SpHJSprite;

class clsHJRenderTarget;
typedef clsHJSharedPtr< clsHJRenderTarget > SpHJRenderTarget;

class clsHJScene;
typedef clsHJSharedPtr< clsHJScene > SpHJScene;

class clsHJShader;
typedef clsHJSharedPtr< clsHJShader > SpHJShader;

class clsHJParticleEmitter2d;
typedef clsHJSharedPtr< clsHJParticleEmitter2d > SpHJParticleEmitter2d;

class clsHJAsciiFont;
typedef clsHJSharedPtr< clsHJAsciiFont > SpHJAsciiFont;

class clsHJSound;
typedef clsHJSharedPtr< clsHJSound > SpHJSound;

class clsHJStreamSound;
typedef clsHJSharedPtr< clsHJStreamSound > SpHJStreamSound;

class clsHJMesh;
typedef clsHJSharedPtr< clsHJMesh > SpHJMesh;

class clsHJModel;
typedef clsHJSharedPtr< clsHJModel > SpHJModel;

class clsHJThread;
typedef clsHJSharedPtr< clsHJThread > SpHJThread;

class clsHJCriticalSection;
typedef clsHJSharedPtr< clsHJCriticalSection > SpHJCriticalSection;

class clsHJCamera;
typedef clsHJSharedPtr< clsHJCamera > SpHJCamera;

class clsHJCamera2d;
typedef clsHJSharedPtr< clsHJCamera2d > SpHJCamera2d;

class clsBinary;
typedef clsHJSharedPtr< clsBinary > SpBinary;

class clsHJResource;
typedef clsHJSharedPtr< clsHJResource > SpHJResource;



} //namespace hj

#endif // __HJ_PROTOTYPE_H_