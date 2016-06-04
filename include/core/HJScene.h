//--------------------------------------------------------------
/*!	@file
	@brief	シーン
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_SCENE_H_
#define __HJ_SCENE_H_

// HJLib namespace
namespace hj{
	
class clsHJRenderTarget;

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シーン基底クラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJScene
{
public:
	clsHJScene():m_pcRenderTarget(NULL), m_pcSprite(NULL){};
	virtual ~clsHJScene(){};

	// --- Method
	virtual void Init(clsHJRenderTarget* pcRenderTarget)=0;												//!< シーン初期化
	virtual void RenderScene()=0;																		//!< シーン描画

	// --- Set
	virtual void SetRenderTarget(clsHJRenderTarget* pcRenderTarget){ m_pcRenderTarget=pcRenderTarget; }	//!< レンダリングターゲット設定
	
protected:
	clsHJRenderTarget*	m_pcRenderTarget;	//!< レンダリングターゲット
	clsHJSprite*		m_pcSprite;			//!< レンダリング結果描画用スプライト
};

}// namespace hj

#endif // __HJ_SCENE_H_