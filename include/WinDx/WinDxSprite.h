//--------------------------------------------------------------
/*!	@file
	@brief	WinDxスプライト
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_SPRITE_H_
#define __WINDX_SPRITE_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDx スプライトクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxSprite : public clsHJSprite
{
public:
	clsWinDxSprite();
	virtual ~clsWinDxSprite();

	virtual void SetTexture( SpHJTexture spTexture );						//!< テクスチャ設定
	virtual void SetColor(hj_color32 u32Color);								//!< 色
	virtual void SetTextureUv(hj_f32 f32TexUvLeft, hj_f32 f32TexUvTop, hj_f32 f32TexUvRight, hj_f32 f32TexUvBottom);//!< テクスチャUV設定

protected:
	// --- Method
	virtual void RenderOneNoneBeginEnd();									//!< 単一レンダリング(begin~endを行わない、階層などを見ない)
	virtual void RenderOneNoneBeginEnd(const stcMatrix44& orsMtx);			//!< 単一行列変換後のレンダリング(begin~endを行わない、階層などを見ない)

	// --- Static Variable
	static clsWinDxDraw2d::stcWinDxFormat_PCT m_tasDefaultVertices[4];	//!< デフォルト描画用頂点バッファ

	// --- Variable
	clsWinDxDraw2d::stcWinDxFormat_PCT	m_asVertices[4];	//!< 描画用頂点バッファ
};

}// namespace hj

#endif // __WINDX_SPRITE_H_