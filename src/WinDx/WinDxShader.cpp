//--------------------------------------------------------------
/*!	@file
	@brief	HJShader
			
	@author 本城 直志
	@date	11/11/09
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HjLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxShader::clsWinDxShader() : clsHJShader()
, m_pcEffect(NULL)
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxShader::~clsWinDxShader()
{
	HJ_SAFE_RELEASE(m_pcEffect);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シェーダー作成(パス)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxShader::CreateShaderFile(const hj_tchar* ptcPath)
{
	HJ_SAFE_RELEASE(m_pcEffect);

	SpHJGraphics spGraphics = clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9 pDevice	= static_cast<clsWinDxGraphics*>(spGraphics.Get())->GetDevice();
	HRESULT hr;

	LPD3DXBUFFER pCompileErrBuff=NULL;
	hr = D3DXCreateEffectFromFile(pDevice, ptcPath, NULL, NULL, 0, NULL, &m_pcEffect, &pCompileErrBuff);

#ifdef _DEBUG
	if( pCompileErrBuff != NULL )
	{
		hj_c8* pc8CompileErr = reinterpret_cast<hj_c8*>( pCompileErrBuff->GetBufferPointer() );
#if defined(HJ_UNICODE)
		hj_tchar tcComplieErr[1024]; // UNICODEに変換しとく
		MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pc8CompileErr, -1, tcComplieErr, 1024);
		HJ_ASSERT_MSG( HJ_T("エフェクトファイルでコンパイルエラーがありました\n%s\n"), tcComplieErr);
#else
		HJ_ASSERT_MSG( HJ_T("エフェクトファイルでコンパイルエラーがありました\n%s\n"), pc8CompileErr);
#endif
	}
#endif // HJ_DEBUG

	return SUCCEEDED(hr);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シェーダー作成(バッファ)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxShader::CreateShaderFile(const hj_u8* pu8Buff, hj_size_t sizeFile)
{
	HJ_SAFE_RELEASE(m_pcEffect);

	SpHJGraphics spGraphics = clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9 pDevice	= static_cast<clsWinDxGraphics*>(spGraphics.Get())->GetDevice();
	HRESULT hr;

	LPD3DXBUFFER pCompileErrBuff=NULL;
	hr = D3DXCreateEffect(pDevice, pu8Buff, sizeFile, NULL, NULL, 0, NULL, &m_pcEffect, &pCompileErrBuff);

#ifdef _DEBUG
	if( pCompileErrBuff != NULL )
	{
		hj_c8* pc8CompileErr = reinterpret_cast<hj_c8*>( pCompileErrBuff->GetBufferPointer() );
#if defined(HJ_UNICODE)
		hj_tchar tcComplieErr[1024]; // UNICODEに変換しとく
		MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pc8CompileErr, -1, tcComplieErr, 1024);
		HJ_ASSERT_MSG( HJ_T("エフェクトファイルでコンパイルエラーがありました\n%s\n"), tcComplieErr);
#else
		HJ_ASSERT_MSG( HJ_T("エフェクトファイルでコンパイルエラーがありました\n%s\n"), pc8CompileErr);
#endif
	}
#endif // HJ_DEBUG

	return SUCCEEDED(hr);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シェーダー作成埋め込み用リソース(注：#includeディレクティブ使えません)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxShader::CreateShaderResource(hj_u32 u32Resourceid)
{
	HJ_SAFE_RELEASE(m_pcEffect);

	SpHJGraphics spGraphics = clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9 pDevice = static_cast<clsWinDxGraphics*>(spGraphics.Get())->GetDevice();

	LPTSTR lpResource = MAKEINTRESOURCE( u32Resourceid );

	HRESULT hr;
	LPD3DXBUFFER pCompileErrBuff=NULL;
	hr = D3DXCreateEffectFromResource(
										pDevice,
										NULL,
										MAKEINTRESOURCE( u32Resourceid ),
										NULL,
										NULL,
										0,
										NULL,
										&m_pcEffect,
										&pCompileErrBuff
	);

	switch(hr){
		case D3DERR_INVALIDCALL:
			OR(HJ_T("D3DERR_INVALIDCALL"));
			break;
		case D3DXERR_INVALIDDATA:
			OR(HJ_T("D3DXERR_INVALIDDATA"));
			break;
		case E_OUTOFMEMORY:
			OR(HJ_T("E_OUTOFMEMORY"));
			break;
	};
	return SUCCEEDED(hr);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	begin
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::Begin()
{
	m_pcEffect->Begin(NULL,0);
	m_pcEffect->BeginPass(0);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	end
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::End()
{
	m_pcEffect->EndPass();
	m_pcEffect->End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Float設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::SetFloat(const hj_c8* pu8Name, hj_f32 f32Float)
{
	m_pcEffect->SetFloat(pu8Name, f32Float);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Vector2設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::SetVector2(const hj_c8* pu8Name, const stcVector2& rsVec2)
{
	m_pcEffect->SetVector(pu8Name, &D3DXVECTOR4(rsVec2.x, rsVec2.y, 0.f, 1.f) );
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Vector3設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::SetVector3(const hj_c8* pu8Name, const stcVector3& rsVec3)
{
	m_pcEffect->SetVector(pu8Name, &D3DXVECTOR4(rsVec3.x, rsVec3.y, rsVec3.z, 1.f) );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Vector4設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::SetVector4(const hj_c8* pu8Name, const stcVector4& rsVec4)
{
	m_pcEffect->SetVector(pu8Name, &D3DXVECTOR4(rsVec4.x, rsVec4.y, rsVec4.z, rsVec4.w) );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Matrix44設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::SetMatrix44(const hj_c8* pu8Name, const stcMatrix44& rsMatrix44)
{
	m_pcEffect->SetMatrix(pu8Name, rsMatrix44.GetOrigConstPtr() );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクスチャ設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::SetTexture(const hj_c8* pu8Name, const SpHJTexture spTexture)
{
	const clsWinDxTexture* pcWinDxTexture = static_cast<const clsWinDxTexture*>(spTexture.Get());
	m_pcEffect->SetTexture( pu8Name, pcWinDxTexture->GetTexture() );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テクニック設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxShader::SetTechnique(const hj_c8* pu8Name)
{
	m_pcEffect->SetTechnique(pu8Name);
}

} //namespace hj
