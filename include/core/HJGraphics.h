//--------------------------------------------------------------
/*!	@file
	@brief	HJGraphics
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_GRAPHICS_H_
#define __HJ_GRAPHICS_H_

// HJLib namespace
namespace hj{

class clsHJRenderTarget;
class clsHJTexture;
class clsHJShader;
	
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	グラフィックス基底
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJGraphics
{
public:
	clsHJGraphics();
	virtual ~clsHJGraphics();
	
	// --- Enum
	//! テクスチャブレンド
	enum enmBlend{
		BLEND_BLEND,	//線形
		BLEND_ADD,		//加算
		BLEND_SUBTRACT,	//減算
		BLEND_MULTIPLY,	//乗算
		BLEND_CHROMA,	//彩度を上げる(焼きこみ)
		BLEND_NPINV,	//ネガポジ反転
		BLEND_NONE,		//不透明
	};

	//! カリング
	enum enmCull{
		CULL_NONE,
		CULL_CW,
		CULL_CCW,
		CULL_MAX_NUM,
	};

	//! テクスチャサンプル
	enum enmTextureSampler{
		TEX_SAMPLER_NONE,
		TEX_SAMPLER_LINEAR,
		TEX_SAMPLER_POINT,
		TEX_SAMPLER_MAXNUM,
	};

	//! テクスチャアドレッシング
	enum enmTextureAddress{
		TEX_ADDRESS_WRAP,
		TEX_ADDRESS_CLAMP,
		TEX_ADDRESS_MAXNUM,
	};

	//! テクスチャ
	static const hj_u8 m_tu8SetTextureMaxNum = 8;								//!< テクスチャ最大数
	static const hj_c8 m_tc8SetTextureShaderName[m_tu8SetTextureMaxNum][32];	//!< シェーダ内でのテクスチャ名

	// --- Method
	virtual void RenderBegin();
	virtual void RenderEnd();

	// --- Set
	virtual void SetBlendMode(enmBlend eBlend)=0;												//!< テクスチャブレンド設定
	virtual void SetBackColor(hj_color32 u32Color);												//!< バックバッファ色
	virtual void SetScreenSize(hj_u32 u32Width, hj_u32 u32Height);								//!< スクリーンサイズ設定
	virtual void SetCullMode(enmCull eCull)=0;													//!< カリング設定
	virtual void SetRenderTarget( SpHJRenderTarget spRenderTarget );							//!< レンダリングターゲット設定
	virtual void SetTexture( SpHJTexture spTexture, hj_u8 u8TexNo=0 )=0;						//!< テクスチャ設定
	virtual void SetWorldMatrix( const stcMatrix44& rsMtx );									//!< ワールドマトリクス設定
	virtual void SetViewMatrix( const stcMatrix44& rsMtx );										//!< ビューマトリクス設定
	virtual void SetProjectionMatrix( const stcMatrix44& rsMtx );								//!< プロジェクションマトリクス設定
	virtual void UpdateTransformMatrix();														//!< マトリクス更新
	virtual void SetEnableLight(hj_bool bEnable)=0;												//!< ライト有効設定
	virtual void SetEnableZBuffer(hj_bool bEnable)=0;											//!< Zバッファ有効設定
	virtual void ClearRenderTarget()=0;															//!< レンダーターゲットのバッファをクリア
	virtual void SetRenderTargetBackBuffer()=0;													//!< レンダリングターゲットにバックバッファを設定
	virtual void SetTextureSampler(enmTextureSampler eTexSampler, hj_u8 u8TexNo=0 )=0;			//!< テクスチャサンプラー設定
	virtual void SetTextureAddress(enmTextureAddress eTexAddress, hj_u8 u8TexNo=0 )=0;			//!< テクスチャアドレッシング設定
	virtual void SetShader( SpHJShader spShader );												//!< シェーダー設定
	virtual void ResetShader();																	//!< 設定シェーダーリセット

	// --- Get
	virtual hj_color32			GetBackColor()const{ return m_u32BackColor; }					//!< 背景色取得
	virtual hj_u32				GetScreenWidth()const{ return m_u32ScreenWidth; }				//!< スクリーンサイズW取得
	virtual hj_u32				GetScreenHeight()const{ return m_u32ScreenHeight; }				//!< スクリーンサイズH取得
	virtual SpHJRenderTarget	GetSetRenderTarget()const{ return m_spSetRenderTarget;}			//!< 現在設定されているレンダリングターゲット取得
	virtual const stcMatrix44&	GetWorldMatrix()const{return m_sWorldMtx;};						//!< ワールドマトリクス取得
	virtual const stcMatrix44&	GetViewMatrix()const{return m_sViewMtx;};						//!< ビューマトリクス取得
	virtual const stcMatrix44&	GetProjectionMatrix()const{return m_sProjMtx;};					//!< プロジェクションマトリクス取得
	virtual const stcMatrix44&	GetWvpMatrix()const{return m_sWVPMtx;};							//!< WVPマトリクス取得

protected:
	// --- Variable
	hj_color32			m_u32BackColor;										//!< 背景色
	hj_u32				m_u32ScreenWidth, m_u32ScreenHeight;				//!< スクリーンサイズ
	hj_u32				m_u32ScreenWidth_half, m_u32ScreenHeight_half;		//!< スクリーンサイズ(半分)
	SpHJRenderTarget	m_spSetRenderTarget;								//!< 設定中のレンダリングターゲット
	SpHJShader			m_spNowShader;										//!< 設定中のシェーダ
	stcMatrix44			m_sWorldMtx, m_sViewMtx, m_sProjMtx, m_sWVPMtx;		//!< 各トランスフォームマトリクス
	hj_bool				m_bMatrixUpdate, m_bChangeShader;
};


} // namespace hj


#endif // __HJ_GRAPHICS_H_

