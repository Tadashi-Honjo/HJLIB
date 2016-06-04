//--------------------------------------------------------------
/*!	@file
	@brief	HJDraw3d
			
	@author 本城 直志
	@date	13/12/12
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJDraw3d::clsHJDraw3d():m_bBegin(false)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJDraw3d::~clsHJDraw3d()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画開始
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDraw3d::Begin()
{
	clsHJGraphics& rcGraphics = *clsHJCoreMgr::RS().GetGraphics();
	clsHJRenderTarget* pcRenderTarget = rcGraphics.GetSetRenderTarget();

	// 設定
	rcGraphics.SetEnableLight(false);		// ライト
	rcGraphics.SetEnableZBuffer(true);		// Zバッファ

	// カリング変更
//	rcGraphics.SetCullMode( clsHJGraphics::CULL_NONE );

	// 開始
	m_bBegin = true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画終了
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDraw3d::End()
{
//	clsHJGraphics& rcGraphics = *clsHJCoreMgr::RS().GetGraphics();

	// レンダーステート戻す
//	rcGraphics.SetEnableLight(true);		// ライト
//	rcGraphics.SetEnableZBuffer(true);		// Zバッファ

	// マトリクス設定
//	rcGraphics.SetViewMatrix( *stcMatrix44::GetIndentityMatrix() );			// ビューマトリクス
//	rcGraphics.SetProjectionMatrix( *stcMatrix44::GetIndentityMatrix() );	// プロジェクションマトリクス

	// 終了
	m_bBegin = false;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プリミティブ描画数の計算
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_u32 clsHJDraw3d::CalcPrimitiveDrawNum( enmPrimitiveType ePrim, hj_u32 u32ArrayNum )const
{
	switch(ePrim){
		case PT_POINTLIST:
			return u32ArrayNum;
		case PT_LINELIST:
			return u32ArrayNum/2;
		case PT_LINESTRIP:
			return u32ArrayNum-1;
		case PT_TRIANGLELIST:
			return u32ArrayNum/3;
		case PT_TRIANGLESTRIP:
		case PT_TRIANGLEFAN:
			return u32ArrayNum-2;
	};

	return -1;
}

}// namespace hj