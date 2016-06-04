//--------------------------------------------------------------
/*!	@file
	@brief	WinDxレンダーターゲット
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}
//! clsWinDxRenderTargetクラス
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxRenderTarget::clsWinDxRenderTarget():clsHJRenderTarget()
, m_pDepthSurf(NULL)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxRenderTarget::~clsWinDxRenderTarget()
{
	HJ_SAFE_RELEASE(m_pDepthSurf);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダーターゲット作成
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxRenderTarget::CreateRenderTarget(hj_u32 u32Width, hj_u32 u32Height)
{
	SpHJGraphics spGraphics = clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9 pDevice = static_cast<clsWinDxGraphics*>(spGraphics.Get())->GetDevice();
	m_u32Width = u32Width;
	m_u32Height = u32Height;
		
	// テクスチャの作成
	m_spTexture.Reset( NEW clsPfTexture() );
	if( !m_spTexture->CreateRenderTargetTexture(u32Width, u32Height) ){
		HJ_ASSERT_MSG( HJ_T("RenderTarget Create Texture Error!!"));
		return false;
	}

	// 深度バッファ作成
	if( !SUCCEEDED( pDevice->CreateDepthStencilSurface(u32Width, u32Height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &m_pDepthSurf, NULL)) )
	{
		HJ_ASSERT_MSG( HJ_T("RenderTarget Create DepthSurface Error!!"));
		return false;
	}

	return true;
}

}// namespace hj