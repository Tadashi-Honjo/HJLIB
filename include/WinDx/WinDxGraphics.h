//--------------------------------------------------------------
/*!	@file
	@brief	WinDxGraphics
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_GRAPHICS_H_
#define __WINDX_GRAPHICS_H_

// HJLib namespace
namespace hj{


//! WinDxGraphics
class clsWinDxGraphics : public clsHJGraphics
{
public:
	clsWinDxGraphics();
	~clsWinDxGraphics();

	// --- Variable
	// 使用するシェーダバージョン
	static const hj_u8	m_tou16PixelShaderMajorVer = 2;		// ピクセルシェーダバージョン
	static const hj_u8	m_tou16PixelShaderMinorVer = 0;
	static const hj_u8	m_tou16VertexShaderMajorVer = 2;	// 頂点シェーダバージョン
	static const hj_u8	m_tou16VertexShaderMinorVer = 0;

	// --- Method
	void InitDirectGraphics(HWND l_hWnd, hj_s32 width, hj_s32 height, hj_bool bFullScreen);	//!< DirectGraphics初期化関数
	HRESULT DisplayUpDate();																//!< 画面更新＆デバイスチェック
	HRESULT ChangeDisplayMode();															//!< フルスクリーン・ウィンドウモード変更関数
	virtual void ClearRenderTarget();														//!< 現在のバッファをクリア

	// ----- Get
	hj_bool	IsFullScreen() const{	return m_bFullScreenMode;	}							//!< フルスクリーン
	const LPDIRECT3DDEVICE9	GetDevice() const{return m_pDevice;	};							//!< デバイスの取得
	
	// ----- Set
	virtual void SetRenderTargetBackBuffer();												//!< レンダリングターゲットにバックバッファを設定
	virtual void SetCullMode(enmCull eCull);												//!< カリング設定
	virtual void SetRenderTarget( SpHJRenderTarget spRenderTarget );						//!< レンダリングターゲット設定
	virtual void SetTexture( SpHJTexture spTexture, hj_u8 u8TexNo=0 );						//!< テクスチャ設定
	virtual void SetWorldMatrix( const stcMatrix44& rsMtx );								//!< ワールドマトリクス設定
	virtual void SetViewMatrix( const stcMatrix44& rsMtx );									//!< ビューマトリクス設定
	virtual void SetProjectionMatrix( const stcMatrix44& rsMtx );							//!< プロジェクションマトリクス設定
	virtual void UpdateTransformMatrix();													//!< マトリクス更新
	virtual void SetEnableLight(hj_bool bEnable);											//!< ライト有効設定
	virtual void SetEnableZBuffer(hj_bool bEnable);											//!< Zバッファ有効設定
	
	//レンダリングステータス
	void SetRenderStates();																	//!< レンダリングステート設定
	virtual void SetBlendMode(enmBlend Blend);												//!< ブレンド方法設定
	virtual void SetTextureSampler(enmTextureSampler eTexSampler, hj_u8 u8TexNo=0 );		//!< テクスチャサンプラー設定
	virtual void SetTextureAddress(enmTextureAddress eTexAddress, hj_u8 u8TexNo=0 );		//!< テクスチャアドレッシング設定

private:
	hj_bool					m_bFullScreenMode;						//!< フルスクリーンモード
	LPDIRECT3D9				m_pDirect3D;							//!< Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pDevice;								//!< Direct3Dデバイス
	D3DDISPLAYMODE			mDisplayMode;							//!< ディスプレイモード
	D3DPRESENT_PARAMETERS	mPresentParamFull;						//!< プレゼンパラメーター（full）
	D3DPRESENT_PARAMETERS	mPresentParamWin;						//!< プレゼンパラメーター（win）
	D3DPRESENT_PARAMETERS*	m_pNowParam;							//!< 使用中のプレゼンパラメーター
	LPDIRECT3DSURFACE9		m_pScreenBufSurf;						
	LPDIRECT3DSURFACE9		m_pBackBufDepthSurf, m_pBackBufSurf;	//!< バックバッファ
	HWND					m_hWnd;
	
};

} // namespace hj


#endif // __WINDX_GRAPHICS_H_
