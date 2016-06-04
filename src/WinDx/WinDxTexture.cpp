//--------------------------------------------------------------
/*!	@file
	@brief	WinDxテクスチャ
			
	@author 本城 直志
	@date	11/10/18
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxTexture::clsWinDxTexture()
: clsHJTexture()
, m_pTexture(NULL)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxTexture::~clsWinDxTexture()
{
	HJ_SAFE_RELEASE(m_pTexture);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャ読み込み
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxTexture::ReadTexture(const hj_tchar* ptcPath)
{
	SpHJGraphics spGraphics = clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9 pDevice = static_cast<clsWinDxGraphics*>(spGraphics.Get())->GetDevice();

	//D3DXGetImageInfoFromFileで使用する画像ファイルの情報
	D3DXIMAGE_INFO iinfo;
	HRESULT	hRet = D3DXGetImageInfoFromFile(ptcPath, &iinfo);
	if( hRet != D3D_OK) return false;

	//テクスチャをファイルから作成
	hRet = D3DXCreateTextureFromFileEx(
		pDevice,
		ptcPath,
		iinfo.Width, iinfo.Height, // 取得した画像ファイルのサイズを指定する
		1, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL, NULL,
		&m_pTexture
	);
	if( hRet != D3D_OK) return false;

	// サイズ取得
	LPDIRECT3DSURFACE9 pSurface;
	m_pTexture->GetSurfaceLevel(0, &pSurface);
	D3DSURFACE_DESC SufDesc;
	pSurface->GetDesc( &SufDesc );
	m_u32Width = SufDesc.Width;
	m_u32Height = SufDesc.Height;
	pSurface->Release();

	return true;
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャ読み込み(バッファ指定)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxTexture::ReadTexture(const hj_u8* pu8Buff, hj_size_t sizeFile)
{
	SpHJGraphics spGraphics = clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9 pDevice = static_cast<clsWinDxGraphics*>(spGraphics.Get())->GetDevice();

	//D3DXGetImageInfoFromFileInMemoryで使用する画像ファイルの情報
	D3DXIMAGE_INFO iinfo;
	HRESULT	hRet = D3DXGetImageInfoFromFileInMemory(pu8Buff, sizeFile, &iinfo);
	if( hRet != D3D_OK) return false;

	//テクスチャをファイルから作成
	hRet = D3DXCreateTextureFromFileInMemoryEx(
		pDevice,
		pu8Buff,
		sizeFile,
		iinfo.Width, iinfo.Height, // 取得した画像ファイルのサイズを指定する
		1, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL, NULL,
		&m_pTexture
	);
	if( hRet != D3D_OK) return false;

	// サイズ取得
	LPDIRECT3DSURFACE9 pSurface;
	m_pTexture->GetSurfaceLevel(0, &pSurface);
	D3DSURFACE_DESC SufDesc;
	pSurface->GetDesc( &SufDesc );
	m_u32Width = SufDesc.Width;
	m_u32Height = SufDesc.Height;
	pSurface->Release();

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダーターゲット用テクスチャの作成
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxTexture::CreateRenderTargetTexture(hj_u32 u32Width, hj_u32 u32Height)
{
	SpHJGraphics spGraphics = clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9 pDevice	= static_cast<clsWinDxGraphics*>(spGraphics.Get())->GetDevice();
	m_u32Width = u32Width;
	m_u32Height = u32Height;
#if 0
	return SUCCEEDED( D3DXCreateTexture( pDevice, u32Width, u32Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture) );
	//	return SUCCEEDED( D3DXCreateTexture( pDevice, u32Width, u32Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture) );
#else
	return SUCCEEDED( D3DXCreateTexture( pDevice, u32Width, u32Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture) );
#endif

}

}// namespace hj