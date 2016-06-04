//--------------------------------------------------------------
/*!	@file
	@brief	WinDxシーン
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}
//! clsWinDxSceneクラス
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxScene::clsWinDxScene():clsHJScene()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxScene::~clsWinDxScene()
{
	HJ_SAFE_DELETE(m_pcSprite);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シーン初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxScene::Init(clsHJRenderTarget* pcRenderTarget)
{
	m_pcRenderTarget = pcRenderTarget;
	HJ_SAFE_DELETE(m_pcSprite);
	m_pcSprite = NEW clsWinDxSprite();
	m_pcSprite->SetTexture(m_pcRenderTarget->GetTexture());	// テクスチャ設定
	m_pcSprite->SetCenter(false);							// センタリングOFF
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シーン描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxScene::RenderScene()
{
//	m_pcSprite->SetScale(1.5f, 1.5f);
	m_pcSprite->Render();
}

}// namespace hj