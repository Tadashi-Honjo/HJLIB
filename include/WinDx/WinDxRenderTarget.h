//--------------------------------------------------------------
/*!	@file
	@brief	WinDxレンダーターゲット
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_RENDER_TARGET_H_
#define __WINDX_RENDER_TARGET_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDxレンダーターゲットクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxRenderTarget : public clsHJRenderTarget
{
public:
	clsWinDxRenderTarget();
	virtual ~clsWinDxRenderTarget();

	// --- Method
	virtual hj_bool CreateRenderTarget(hj_u32 u32Width, hj_u32 u32Height);	// レンダーターゲット作成

	// --- Set
	void	SetTexture( SpHJTexture spTexture ){m_spTexture=spTexture;};							// テクスチャ設定(バッファサイズは同じで)
	void	SetDepthSurface( LPDIRECT3DSURFACE9 pDepthSurface ){m_pDepthSurf=pDepthSurface;};		// 深度バッファ設定(バッファサイズは同じで)

	// --- Get
	LPDIRECT3DSURFACE9 GetDepthSurface()const{ return m_pDepthSurf; };

private:
	// --- Variable
	LPDIRECT3DSURFACE9	m_pDepthSurf;	// 深度バッファ(Z、ステンシル)
};

}// namespace hj

#endif // __WINDX_RENDER_TARGET_H_