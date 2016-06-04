//--------------------------------------------------------------
/*!	@file
	@brief	WinDxGraphics
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

#include <Dwmapi.h>

namespace
{
	// デスクトップコンポジションの無効化
	typedef HRESULT (WINAPI * DwmEnableCompositionFunction)(__in UINT uCompositionAction);
	HRESULT TurnOffAero()
	{
		HRESULT aResult = S_OK;
		HMODULE dwmapiDllHandle = LoadLibrary(L"dwmapi.dll");  
		if (NULL != dwmapiDllHandle ) // not on Vista/Windows7 so no aero so no need to account for aero.
		{
			DwmEnableCompositionFunction DwmEnableComposition;
			DwmEnableComposition = (DwmEnableCompositionFunction) ::GetProcAddress(dwmapiDllHandle, "DwmEnableComposition");
			if( NULL != DwmEnableComposition ){
				DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
			}
			FreeLibrary(dwmapiDllHandle); 
		}
		return aResult;
	}

} // namespace

// HJLib namespace
namespace hj{

namespace{
	// カリングデータ
	struct stcCullData{
		hj_u32 u32DxCullId;
	};
	stcCullData tasCullData[clsHJGraphics::CULL_MAX_NUM] = {
		{D3DCULL_NONE},		// CULL_NONE
		{D3DCULL_CW},		// CULL_CW
		{D3DCULL_CCW},		// CULL_CCW
	};
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxGraphics::clsWinDxGraphics(): clsHJGraphics()
, m_bFullScreenMode(false)
, m_pDirect3D(NULL)
, m_pDevice(NULL)
, m_pBackBufDepthSurf(NULL)
, m_pBackBufSurf(NULL)
, m_hWnd(NULL)
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxGraphics::~clsWinDxGraphics()
{
	//オブジェクト開放
	HJ_SAFE_RELEASE(m_pDevice);
	HJ_SAFE_RELEASE(m_pDirect3D);

	//バックバッファサーフェス
	//HJ_SAFE_RELEASE(m_pBackBufDepthSurf);
	//HJ_SAFE_RELEASE(m_pBackBufSurf);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	DirectGraphics初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::InitDirectGraphics( HWND l_hWnd, hj_s32 width, hj_s32 height, hj_bool bFullScreen )
{
	//----------------------------------
	// 「Direct3D」オブジェクトの作成
	if( NULL == ( m_pDirect3D = Direct3DCreate9( D3D_SDK_VERSION )))
	{
		HJ_ASSERT_MSG( HJ_T("Direct3D Create Error!!"));
	}

	//現在の画面モードを取得
	if (FAILED(m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mDisplayMode)))
	{
		HJ_ASSERT_MSG( HJ_T("Display Mode Error!!"));
	}

	//性能チェック
	D3DCAPS9 Caps;
	if (FAILED(m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps)))
	{
		HJ_ASSERT_MSG( HJ_T("Performance Error!!"));
	}

	// シェーダバージョンチェック
	if( Caps.PixelShaderVersion < D3DPS_VERSION( m_tou16PixelShaderMajorVer, m_tou16PixelShaderMinorVer) ){
		HJ_ASSERT_MSG( HJ_T("PixelShader Version Error!!"));
	}
	if( Caps.VertexShaderVersion < D3DVS_VERSION(m_tou16VertexShaderMajorVer, m_tou16VertexShaderMinorVer) ){
		HJ_ASSERT_MSG( HJ_T("VertexShader Version Error!!"));
	}

	// スクリーンサイズ設定(解像度)
	SetScreenSize(width, height);

	m_hWnd = l_hWnd;
	m_bFullScreenMode = bFullScreen;

	// フルスクリーン対応するためにデスクトップコンポジションの無効化する必要がある
	if( m_bFullScreenMode ){
		TurnOffAero();
	}

	//「DIRECT3Dデバイス」オブジェクトの作成
	D3DDISPLAYMODE dmode;
	m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode);

	//Direct3D パラメータ設定（ウインドウ）
	ZeroMemory(&mPresentParamWin, sizeof(mPresentParamWin));
	mPresentParamWin.Windowed						= TRUE;
	mPresentParamWin.SwapEffect						= D3DSWAPEFFECT_DISCARD;
	mPresentParamWin.BackBufferFormat				= dmode.Format;
	mPresentParamWin.BackBufferCount				= 1;
	mPresentParamWin.EnableAutoDepthStencil			= TRUE;
	mPresentParamWin.AutoDepthStencilFormat			= D3DFMT_D24S8;
	mPresentParamWin.FullScreen_RefreshRateInHz		= D3DPRESENT_RATE_DEFAULT;
	mPresentParamWin.hDeviceWindow					= m_hWnd;

	//Direct3D パラメータ設定（フルスクリーン）
	mPresentParamFull = mPresentParamWin;
	mPresentParamFull.BackBufferWidth				= m_u32ScreenWidth;
	mPresentParamFull.BackBufferHeight				= m_u32ScreenHeight;
	mPresentParamFull.Windowed						= FALSE;
	mPresentParamFull.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;
	mPresentParamFull.FullScreen_RefreshRateInHz	= dmode.RefreshRate;

	//スクリーンモードチェック
	m_pNowParam = m_bFullScreenMode ? &mPresentParamFull : &mPresentParamWin;

	// ハードウェア
	if( FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, m_pNowParam, &m_pDevice)) )
	{
		HJ_ASSERT_MSG( HJ_T("Hardware Device Error!!"));

		// ソフトウェア
		if( FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING, m_pNowParam, &m_pDevice)) )
		{
			HJ_ASSERT_MSG( HJ_T("Software Device Error!!"));

			// REF
			if( FAILED( m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, m_pNowParam, &m_pDevice)) )
			{
				HJ_ASSERT_MSG( HJ_T("REF Device Error!!"));
			}
		}
	}

	//レンダリング設定
	SetRenderStates();

	//バックバッファ取得
	m_pDevice->GetDepthStencilSurface(&m_pBackBufDepthSurf);
	m_pDevice->GetRenderTarget(0, &m_pBackBufSurf);

	LPDIRECT3DVERTEXDECLARATION9 g_pVertexDeclaration;
	D3DVERTEXELEMENT9 decl[] ={
	  //Position
	  { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	  //Normal
	  { 0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	  //Base tex coords
	  { 0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	  //Diffuse color
	  { 0, 32,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	  D3DDECL_END()
	};
	HRESULT hr = m_pDevice->CreateVertexDeclaration(decl , &g_pVertexDeclaration);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	画面更新＆デバイスチェック
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
HRESULT clsWinDxGraphics::DisplayUpDate()
{
	//画面更新
	HRESULT hr = m_pDevice->Present(NULL, NULL, NULL, NULL);
	hr = S_OK;

#if 0
	//エラーチェック
	if(hr == D3DERR_DRIVERINTERNALERROR)
	{
		//致命的なエラー
		HJ_ASSERT_MSG( HJ_T("Driver Internal Error!!"));
		return hr;
	}
	//デバイスが消失
	else if(hr == D3DERR_DEVICELOST)
	{
		//１秒くらい眠らせておきます
		System::Sleep(1000);

		//デバイスの状態をチェックします
		hr = m_pDevice->TestCooperativeLevel();

		//状態によって処理
		if(hr == D3DERR_DEVICELOST)
		{
			//デバイスが消失で何もできない
			System::Sleep(100);	//ちょっとだけ眠らせて次のフレームでリトライ
			return hr;
		}
		else if(hr == D3DERR_DEVICENOTRESET)
		{
			m_pDevice->Reset(m_pNowParam);
		}
	}
#endif
	return hr;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	フルスクリーン・ウィンドウモード変更関数
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
HRESULT clsWinDxGraphics::ChangeDisplayMode()
{
	HRESULT hr;
	m_bFullScreenMode = !m_bFullScreenMode;

	D3DDISPLAYMODE DMode;
	m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DMode);

	//現在の状態用のパラメータを指す
	m_pNowParam = m_bFullScreenMode ? &mPresentParamFull : &mPresentParamWin;

	//フルスクリーンモードなら
	if(m_bFullScreenMode){
		m_pNowParam->FullScreen_RefreshRateInHz = DMode.RefreshRate;
	}

	// デバイスリセット失敗
	hr = m_pDevice->Reset(m_pNowParam);
	if(FAILED(hr)){
		return hr;
	}

	//ウィンドウモードのみ
	if( !m_bFullScreenMode )
	{
		DrawMenuBar(m_hWnd);

		//ウィンドウ位置
		SetWindowPos(
			m_hWnd, HWND_TOP, 
			GetSystemMetrics(SM_CXSCREEN)/2 - m_u32ScreenWidth_half, 
			GetSystemMetrics(SM_CYSCREEN)/2 - m_u32ScreenHeight_half,
			m_u32ScreenWidth, m_u32ScreenHeight, SWP_SHOWWINDOW
		);
	}
	else 
	{
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, m_u32ScreenWidth, m_u32ScreenHeight, SWP_SHOWWINDOW);
	}

	D3DVIEWPORT9 vp;
	vp.X=0;	vp.Y=0;
	vp.Width = m_pNowParam->BackBufferWidth;
	vp.Height = m_pNowParam->BackBufferHeight;
	vp.MinZ=0.0f;vp.MaxZ=1.0f;
	m_pDevice->SetViewport(&vp);

	//レンダリング設定
	SetRenderStates();

	return hr;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	現在のレンダリングターゲットをクリア
	@param  rcRenderTarget	設定レンダリングターゲット
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::ClearRenderTarget()
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_u32BackColor, 1.0f, 0);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダリングターゲットにバックバッファを設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetRenderTargetBackBuffer()
{
	clsHJGraphics::SetRenderTarget( SpHJRenderTarget() );
	m_pDevice->SetRenderTarget(0, m_pBackBufSurf );
	m_pDevice->SetDepthStencilSurface( m_pBackBufDepthSurf );	//深度バッファ
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	カリング設定
	@param  eCull	設定カリング
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetCullMode(enmCull eCull)
{
	m_pDevice->SetRenderState( D3DRS_CULLMODE, tasCullData[eCull].u32DxCullId );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダリングターゲット設定
	@param  rcRenderTarget	設定レンダリングターゲット
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetRenderTarget( SpHJRenderTarget spRenderTarget )
{
	clsHJGraphics::SetRenderTarget( spRenderTarget );	// 親にも設定

	// テクスチャを設定
	LPDIRECT3DSURFACE9 pTexSurface = NULL;
	clsWinDxRenderTarget* pcWinDxRenderTarget = static_cast<clsWinDxRenderTarget*>( spRenderTarget.Get() );
	clsWinDxTexture* pcWinDxTexture = static_cast<clsWinDxTexture*>( spRenderTarget->GetTexture().Get() );
	pcWinDxTexture->GetTexture()->GetSurfaceLevel(0, &pTexSurface);
	m_pDevice->SetRenderTarget(0, pTexSurface );

	// 深度バッファ設定
	m_pDevice->SetDepthStencilSurface( pcWinDxRenderTarget->GetDepthSurface() );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャ設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetTexture( SpHJTexture spTexture, hj_u8 u8TexNo)
{
	if(u8TexNo < 0 && u8TexNo >= m_tu8SetTextureMaxNum ){
		return;
	}
	clsWinDxTexture& rcWinDxTex = *static_cast<clsWinDxTexture*>( spTexture.Get() );
	m_pDevice->SetTexture(u8TexNo, rcWinDxTex.GetTexture());
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ワールドマトリクス設定
	@param  rsMtx	設定マトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetWorldMatrix( const stcMatrix44& rsMtx )
{
	clsHJGraphics::SetWorldMatrix(rsMtx);

	if( m_spNowShader ){
		m_spNowShader->SetMatrix44("worldMtx", rsMtx);
	}else{
		m_pDevice->SetTransform(D3DTS_WORLD, rsMtx.GetOrigConstPtr());
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビューマトリクス設定
	@param  rsMtx	設定マトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetViewMatrix( const stcMatrix44& rsMtx )
{
	clsHJGraphics::SetViewMatrix(rsMtx);

	if( m_spNowShader ){
		m_spNowShader->SetMatrix44("viewMtx", rsMtx);
	}else{
		m_pDevice->SetTransform(D3DTS_VIEW, rsMtx.GetOrigConstPtr());
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プロジェクションマトリクス設定
	@param  rsMtx	設定マトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetProjectionMatrix( const stcMatrix44& rsMtx )
{
	clsHJGraphics::SetProjectionMatrix(rsMtx);

	if( m_spNowShader ){
		m_spNowShader->SetMatrix44("projectionMtx", rsMtx);
	}else{
		m_pDevice->SetTransform(D3DTS_PROJECTION, rsMtx.GetOrigConstPtr());
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マトリクス更新
	@param  -
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::UpdateTransformMatrix()
{
	if( !m_bMatrixUpdate ){
		return;
	}
	clsHJGraphics::UpdateTransformMatrix();
	if( m_spNowShader ){
		m_spNowShader->SetMatrix44("wvpMtx", m_sWVPMtx);
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ライト有効設定
	@param  bEnable	設定値
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetEnableLight(hj_bool bEnable)
{
	m_pDevice->SetRenderState( D3DRS_LIGHTING, bEnable );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Zバッファ有効設定
	@param  bEnable	設定値
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetEnableZBuffer(hj_bool bEnable)
{
	m_pDevice->SetRenderState( D3DRS_ZENABLE, bEnable );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダリングステート設定関数
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetRenderStates()
{
	m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);		//zバッファー処理を有効にする
//	m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);//カリングを有効(CCW)
	m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);//カリングを有効(CCW)
	m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE);		//ライトを有効にする
	m_pDevice->SetRenderState( D3DRS_AMBIENT, 0x00444444 );	//アンビエントライト（環境光）を設定する
	m_pDevice->SetRenderState( D3DRS_SPECULARENABLE,TRUE);	//スペキュラ(鏡面反射）を有効にする
	m_pDevice->SetRenderState( D3DRS_COLORVERTEX,TRUE);		// 頂点カラー有効

	//アルファブレンディングを設定する(透過処理)
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	SetBlendMode(BLEND_BLEND);

	//テクスチャセット
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); //ARG1とARG2のα値を乗算してα値を取得します。
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); //テクスチャのα値
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE); //頂点のα値
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE ); 
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE );
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	// サンプラー
	SetTextureSampler(TEX_SAMPLER_LINEAR);

	// テクスチャアドレッシング
	SetTextureAddress(TEX_ADDRESS_WRAP);

	//アルファテスト
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
    m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ライト
	m_pDevice->LightEnable(0, FALSE);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ブレンド方法設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetBlendMode(enmBlend Blend)
{
	switch(Blend){
		//線形
		case BLEND_BLEND:{
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			break;
		}
		//加算
		case BLEND_ADD:{
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;
		}
		//減算
		case BLEND_SUBTRACT:{
			m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			break;
		}
		//乗算
		case BLEND_MULTIPLY:{
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			break;
		}
		//彩度を上げる(焼きこみ)
		case BLEND_CHROMA:{
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			break;
		}
		//ネガポジ反転
		case BLEND_NPINV:{
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
			break;
		}
		//不透明
		case BLEND_NONE:{
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			break;
		}
	}
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャサンプラー設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetTextureSampler(enmTextureSampler eTexSampler, hj_u8 u8TexNo )
{
	switch(eTexSampler){
		case TEX_SAMPLER_NONE:
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MINFILTER, D3DTEXF_NONE );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
			break;
		case TEX_SAMPLER_LINEAR:
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
			break;
		case TEX_SAMPLER_POINT:
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MINFILTER, D3DTEXF_POINT );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
			break;
	};
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャアドレッシング設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxGraphics::SetTextureAddress(enmTextureAddress eTexAddress, hj_u8 u8TexNo )
{
	switch(eTexAddress){
		case TEX_ADDRESS_WRAP:
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
			break;
		case TEX_ADDRESS_CLAMP:
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
			m_pDevice->SetSamplerState( u8TexNo, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP );
			break;
	};
}

} //namespace hj
