//--------------------------------------------------------------
/*!	@file
	@brief	WinDxシーン
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_SCENE_H_
#define __WINDX_SCENE_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDxレンダーターゲットクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxScene : public clsHJScene
{
public:
	clsWinDxScene();
	virtual ~clsWinDxScene();

	// --- Method
	virtual void Init(clsHJRenderTarget* pcRenderTarget);											//!< シーン初期化
	virtual void RenderScene();																		//!< シーン描画
private:
};

}// namespace hj

#endif // __WINDX_SCENE_H_