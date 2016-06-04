//--------------------------------------------------------------
/*!	@file
	@brief	WinDxスプライト
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}

// デフォルト頂点バッファ
clsWinDxDraw2d::stcWinDxFormat_PCT clsWinDxSprite::m_tasDefaultVertices[4] = {
	//	f32x	f32y,	f32z	f32rhw	u32Color	f32U1	f32V1
	{	-0.5f,	0.5f,	0.f,	HJ_WHITE,	0.f,	0.f},
	{	0.5f,	0.5f,	0.f,	HJ_WHITE,	1.f,	0.f},
	{	-0.5f,	-0.5f,	0.f,	HJ_WHITE,	0.f,	1.f},
	{	0.5f,	-0.5f,	0.f,	HJ_WHITE,	1.f,	1.f},
};

//! clsWinDxRenderTargetクラス
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxSprite::clsWinDxSprite():clsHJSprite()
{
	// 頂点バッファ初期化
	System::Memcpy(m_asVertices, m_tasDefaultVertices, sizeof(clsWinDxDraw2d::stcWinDxFormat_PCT)*4);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxSprite::~clsWinDxSprite()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャ設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxSprite::SetTexture(SpHJTexture spTexture)
{
	clsHJSprite::SetTexture(spTexture);

	hj_f32 f32Width_half = static_cast<hj_f32>( m_spTexture->GetWidth() ) * (m_f32TexUvRight - m_f32TexUvLeft) * 0.5f;
	hj_f32 f32Height_half = static_cast<hj_f32>( m_spTexture->GetHeight() ) * (m_f32TexUvBottom - m_f32TexUvTop) * 0.5f;

	// 頂点バッファに反映
	clsWinDxDraw2d::stcWinDxFormat_PCT* psData = m_asVertices;
	psData->f32x=-f32Width_half; psData->f32y=f32Height_half;
	++psData;
	psData->f32x=f32Width_half;	psData->f32y=f32Height_half;
	++psData;
	psData->f32x=-f32Width_half; psData->f32y=-f32Height_half;
	++psData;
	psData->f32x=f32Width_half; psData->f32y=-f32Height_half;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	色設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxSprite::SetColor(hj_color32 u32Color)
{
	clsHJSprite::SetColor(u32Color);

	// 頂点バッファに反映
	m_asVertices[0].u32Color = u32Color;
	m_asVertices[1].u32Color = u32Color;
	m_asVertices[2].u32Color = u32Color;
	m_asVertices[3].u32Color = u32Color;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャUV設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxSprite::SetTextureUv(hj_f32 f32TexUvLeft, hj_f32 f32TexUvTop, hj_f32 f32TexUvRight, hj_f32 f32TexUvBottom)
{
	if( !m_spTexture ){
		return;
	}

	clsHJSprite::SetTextureUv(f32TexUvLeft, f32TexUvTop, f32TexUvRight, f32TexUvBottom);

	// 頂点バッファに反映
	clsWinDxDraw2d::stcWinDxFormat_PCT* psData = m_asVertices;

	hj_f32 f32Width_half = static_cast<hj_f32>( m_spTexture->GetWidth() ) * (m_f32TexUvRight - m_f32TexUvLeft) * 0.5f;
	hj_f32 f32Height_half = static_cast<hj_f32>( m_spTexture->GetHeight() ) * (m_f32TexUvBottom - m_f32TexUvTop) * 0.5f;

	psData->f32x=-f32Width_half; psData->f32y=f32Height_half;
	psData->f32U1=f32TexUvLeft; psData->f32V1=f32TexUvTop;
	++psData;
	psData->f32x=f32Width_half; psData->f32y=f32Height_half;
	psData->f32U1=f32TexUvRight; psData->f32V1=f32TexUvTop;
	++psData;
	psData->f32x=-f32Width_half; psData->f32y=-f32Height_half;
	psData->f32U1=f32TexUvLeft; psData->f32V1=f32TexUvBottom;
	++psData;
	psData->f32x=f32Width_half; psData->f32y=-f32Height_half;
	psData->f32U1=f32TexUvRight; psData->f32V1=f32TexUvBottom;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダリング
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxSprite::RenderOneNoneBeginEnd()
{
	if( !m_spTexture ){
		return;
	}

	// マトリクス更新
	if(m_bUpdateMtx){
		UpdateMatrix();
	}

	SpHJGraphics		spGraphics = clsHJCoreMgr::RS().GetGraphics();
	clsWinDxGraphics&	rcGraphics	= *static_cast<clsWinDxGraphics*>( spGraphics.Get() );
	clsHJDraw2d&		rcDraw2d	= *clsHJCoreMgr::GS()->GetDraw2d();

	// 行列設定
	rcGraphics.SetWorldMatrix( m_sWorldTransformMtx );

	// テクスチャ設定
	rcGraphics.SetTexture( m_spTexture );

	// 描画
	rcDraw2d.RenderArrayPlatform(clsHJDraw2d::FMT_PCT, clsHJDraw2d::PT_TRIANGLESTRIP, m_asVertices, 4);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	行列変換後のレンダリング
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxSprite::RenderOneNoneBeginEnd(const stcMatrix44& orsMtx)
{
	if( !m_spTexture ){
		return;
	}

	// マトリクス更新
	if(m_bUpdateMtx){
		UpdateMatrix();
	}

	SpHJGraphics	spGraphics	= clsHJCoreMgr::RS().GetGraphics();
	SpHJDraw2d&		spDraw2d	= clsHJCoreMgr::RS().GetDraw2d();

	// 行列設定
	stcMatrix44 sMtx;
	stcMatrix44::Multiply( &sMtx, m_sWorldTransformMtx, orsMtx );
	spGraphics->SetWorldMatrix( sMtx );

	// テクスチャ設定
	spGraphics->SetTexture( m_spTexture );

	// 描画
	spDraw2d->RenderArrayPlatform(clsHJDraw2d::FMT_PCT, clsHJDraw2d::PT_TRIANGLESTRIP, m_asVertices, 4);
}

}// namespace hj