//--------------------------------------------------------------
/*!	@file
	@brief	HJGraphics
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}

//! シェーダ内でのテクスチャ名
const hj_c8 clsHJGraphics::m_tc8SetTextureShaderName[clsHJGraphics::m_tu8SetTextureMaxNum][32]={
	"texture01",
	"texture02",
	"texture03",
	"texture04",
	"texture05",
	"texture06",
	"texture07",
	"texture08",
};
	

//! clsHJGraphicsクラス
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJGraphics::clsHJGraphics():m_u32BackColor(HJ_BLACK)
, m_u32ScreenWidth(0), m_u32ScreenHeight(0)
, m_u32ScreenWidth_half(0), m_u32ScreenHeight_half(0)
, m_sWorldMtx()
, m_sViewMtx()
, m_sProjMtx()
, m_sWVPMtx()
, m_bMatrixUpdate(false)
, m_bChangeShader(false)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJGraphics::~clsHJGraphics()
{
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画開始
	@param		-
	@return		-
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::RenderBegin()
{
	// シェーダが更新されていたらマトリクスを再設定しとく
	if( m_spNowShader && m_bChangeShader ){
		m_bChangeShader = false;
		SetWorldMatrix(m_sWorldMtx);
		SetViewMatrix(m_sViewMtx);
		SetProjectionMatrix(m_sProjMtx);
	}

	// 更新(WVPを設定)
	UpdateTransformMatrix();	

	if( m_spNowShader ){
		m_spNowShader->Begin();
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画終了
	@param		-
	@return		-
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::RenderEnd()
{
	if( m_spNowShader ){
		m_spNowShader->End();
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	バックバッファ色
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::SetBackColor(hj_color32 u32Color)
{
	m_u32BackColor = u32Color;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	スクリーンサイズ設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::SetScreenSize(hj_u32 u32Width, hj_u32 u32Height)
{
	m_u32ScreenWidth = u32Width; m_u32ScreenHeight = u32Height;
	m_u32ScreenWidth_half = u32Width/2; m_u32ScreenHeight_half = u32Height/2;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダリングターゲット設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::SetRenderTarget( SpHJRenderTarget spRenderTarget )
{
	m_spSetRenderTarget=spRenderTarget;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ワールドマトリクス設定
	@param  rsMtx	設定マトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::SetWorldMatrix( const stcMatrix44& rsMtx )
{
	m_bMatrixUpdate = true;
	stcMatrix44::Copy(&m_sWorldMtx, rsMtx);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビューマトリクス設定
	@param  rsMtx	設定マトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::SetViewMatrix( const stcMatrix44& rsMtx )
{
	m_bMatrixUpdate = true;
	stcMatrix44::Copy(&m_sViewMtx, rsMtx);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プロジェクションマトリクス設定
	@param  rsMtx	設定マトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::SetProjectionMatrix( const stcMatrix44& rsMtx )
{
	m_bMatrixUpdate = true;
	stcMatrix44::Copy(&m_sProjMtx, rsMtx);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マトリクス更新
	@param  -
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::UpdateTransformMatrix()
{
	if( m_bMatrixUpdate ){
		stcMatrix44::Multiply(&m_sWVPMtx, m_sWorldMtx, m_sViewMtx);
		stcMatrix44::Multiply(&m_sWVPMtx, m_sWVPMtx, m_sProjMtx);
		m_bMatrixUpdate = false;
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シェーダー設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::SetShader( SpHJShader spShader )
{
	m_bChangeShader = (m_spNowShader.Get() != spShader.Get());
	m_spNowShader = spShader;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	設定シェーダーリセット
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJGraphics::ResetShader()
{
	m_bChangeShader = false;
	m_spNowShader.Reset();
}

}// namespace hj