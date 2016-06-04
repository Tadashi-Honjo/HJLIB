//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@file
	@brief	HJLib Sample Main
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 

namespace{

	// アプリケーション名
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample PointLight");
	const hj_u32 tou32ScreenSizeW = 1280;
	const hj_u32 tou32ScreenSizeH = 720;
	const hj_f32 tof32Aspect = static_cast<hj_f32>(tou32ScreenSizeW)/static_cast<hj_f32>(tou32ScreenSizeH);
	const hj_f32 tof32ClipNear = 1.0f;
	const hj_f32 tof32ClipFar = 100.f;

} // namespace


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	MainClass
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;

public:
	clsMain() 
		: MyClassParent()
		, m_sCameraPos(0.f,5.f,20.f)
		, m_sCameraLookPos(0.f,0.f,0.f)
		, m_f32CameraDegX(20.f)
		, m_f32CameraDegY(45.f)
		, m_f32CameraLength(20.f)
		, m_eCullMode(hj::clsHJGraphics::CULL_CW)
	{
	}
	virtual ~clsMain(){}
	
	virtual	void Update();	//!< ユーザー用更新
	virtual	void Render();	//!< ユーザー用描画

	// インスタンス削除
	virtual	void DeleteInstance(){};

	void Init();

private:
	hj::SpTextureResource		m_spTexResDiffuse;
	hj::SpTextureResource		m_spTexResField;
	hj::SpModelResource			m_spModelRes;
	hj::SpShaderResource		m_spShaderRes;
	hj::stcVector3				m_sCameraPos, m_sCameraLookPos;
	hj_f32						m_f32CameraDegX, m_f32CameraDegY;
	hj_f32						m_f32CameraLength;
	hj::clsHJGraphics::enmCull	m_eCullMode;
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Init()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJFileMgr		spFileMgr	= rcCoreMgr.GetFileMgr();

	// テクスチャ
	m_spTexResDiffuse.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("diffuse.dds"), m_spTexResDiffuse );
	m_spTexResField.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("field.dds"), m_spTexResField );

	// モデル
	m_spModelRes.Reset( NEW hj::clsModelResource() );
	spFileMgr->RequestLoadResource< hj::clsModelResource >( HJ_T("test.hjmd"), m_spModelRes );

	// シェーダー作成
	m_spShaderRes.Reset( NEW hj::clsShaderResource() );
	spFileMgr->RequestLoadResource< hj::clsShaderResource >( HJ_T("Shader.fx"), m_spShaderRes );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Update()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJInput		spInput		= rcCoreMgr.GetInput();

	static const hj_f32 tof32RotSpeed = 2.f;

	// camera rot
	m_f32CameraDegX += spInput->GetPadAnalog(hj::clsHJInput::PAD_STICK2_Y) * tof32RotSpeed;
	m_f32CameraDegY -= spInput->GetPadAnalog(hj::clsHJInput::PAD_STICK2_X) * tof32RotSpeed;
	
	// camera length
	static const hj_f32 tof32LengthSpeed = 0.5f;
	m_f32CameraLength += spInput->GetPadAnalog(hj::clsHJInput::PAD_SLIDER1) * tof32LengthSpeed;
	m_f32CameraLength -= spInput->GetPadAnalog(hj::clsHJInput::PAD_SLIDER2) * tof32LengthSpeed;
	m_f32CameraLength = HJ_MAX( m_f32CameraLength, 1.f);

	// camera position
	hj_f32 f32RadX = hj::clsHJMath::DegToRad(m_f32CameraDegX);
	hj_f32 f32RadY = hj::clsHJMath::DegToRad(m_f32CameraDegY);
	m_sCameraPos.x = m_sCameraLookPos.x + (hj::clsHJMath::Sin(f32RadY) * hj::clsHJMath::Cos(f32RadX) * m_f32CameraLength);
	m_sCameraPos.z = m_sCameraLookPos.y + (hj::clsHJMath::Cos(f32RadY) * hj::clsHJMath::Cos(f32RadX) * m_f32CameraLength);
	m_sCameraPos.y = m_sCameraLookPos.z + (hj::clsHJMath::Sin(f32RadX) * m_f32CameraLength);

	// Change Draw Mode
	hj::SpHJModel spModel = m_spModelRes->GetModel();
	if( spInput->IsTrig( hj::clsHJInput::PAD_BTN1 ) )
	{
		if(spModel->GetDrawMode() == hj::clsHJModel::DRAW_MODE_NUM-1){
			spModel->SetDrawMode(static_cast<hj::clsHJModel::enmDrawMode>(0));
		}else{
			spModel->SetDrawMode(static_cast<hj::clsHJModel::enmDrawMode>(spModel->GetDrawMode()+1));
		}
	}

	// Change Cull Mode
	if( spInput->IsTrig( hj::clsHJInput::PAD_BTN3 ) )
	{
		if(m_eCullMode == hj::clsHJGraphics::CULL_MAX_NUM-1){
			m_eCullMode = static_cast<hj::clsHJGraphics::enmCull>(0);
		}else{
			m_eCullMode = static_cast<hj::clsHJGraphics::enmCull>(m_eCullMode+1);
		}
	}
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Render()
{
	hj::clsHJCoreMgr&		rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics		spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJDraw3d			spDraw3d	= rcCoreMgr.GetDraw3d();
	hj::SpHJDrawPrimitive	spPrimitive = rcCoreMgr.GetDrawPrimitive();
	
	// 描画
	spGraphics->SetBackColor( hj::HJ_DARKSLATEGRAY );
	spGraphics->ClearRenderTarget();
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	// カメラ情報
	static const hj_f32 tof32CameraLength = 30.f;
	const hj::stcVector3 osUp(0.f, 1.f, 0.f);

	// world
	hj::SpHJModel spModel = m_spModelRes->GetModel();
//	spModel->SetRotation(hj::stcVector3(0.f, hj::System::GetGameTime() * 30.f, 0.f));
	spModel->SetRotation( hj::stcVector3(0.f, 30.f, 5.f));
	spModel->SetPosition( hj::stcVector3(-1.f, 0.9f, -1.f));
	hj_f32 f32Scale = 0.1f;
	spModel->SetScale( hj::stcVector3(f32Scale, f32Scale, f32Scale) );

	hj::stcMatrix44 sMtx;

	// Set Cull Mode
//	spGraphics->SetCullMode(m_eCullMode);
	spGraphics->SetCullMode(hj::clsHJGraphics::CULL_CW);

	static const hj_f32 tof32FieldSize_half = 5.f;
	static const hj::clsHJDraw3d::stcFormat_PCT atosFieldPoly[] = {
		{ -tof32FieldSize_half, 0.f, -tof32FieldSize_half, hj::HJ_WHITE,	0.f, 0.f },
		{ tof32FieldSize_half, 0.f, -tof32FieldSize_half, hj::HJ_WHITE,	1.f, 0.f },
		{ -tof32FieldSize_half, 0.f, tof32FieldSize_half, hj::HJ_WHITE, 0.f, 1.f },
		{ tof32FieldSize_half, 0.f, tof32FieldSize_half, hj::HJ_WHITE, 1.f, 1.f },
	};
	static const hj_u32 tou32FieldPolyNum = HJ_ARRAY_NUM( atosFieldPoly );
	

	// ===== シーン描画
	// 通常
	{
		spGraphics->SetRenderTargetBackBuffer();
		spGraphics->SetBackColor( hj::HJ_COLOR_ARGB_SET(255,0,32,0) );
		spGraphics->ClearRenderTarget();

		// view
		sMtx.SetCameraView(	m_sCameraPos, m_sCameraLookPos, osUp );
		spGraphics->SetViewMatrix(sMtx);

		// projection
		sMtx.SetCameraProjection(hj::clsHJMath::m_tof32PI/4.0f, tof32Aspect, tof32ClipNear, tof32ClipFar);
		spGraphics->SetProjectionMatrix(sMtx);

		// world
		sMtx.Identity();
		spGraphics->SetWorldMatrix(sMtx);

#if 0
		// grid
		spPrimitive->DrawGridXz(10.0f, 10, hj::HJ_GRAY);
		// axis
		spPrimitive->DrawAxis(10.0f);
#endif

		hj::SpHJShader spShader = m_spShaderRes->GetShader();

		// シェーダー設定
		spShader->SetTechnique("TShader");
		spGraphics->SetShader( spShader );
		spGraphics->SetCullMode(hj::clsHJGraphics::CULL_CW);

		// ライトのゆらめきテスト
		const hj_f32 of32LightPow = hj::clsHJMath::Random( 0.5f, 0.6f );
		spShader->SetFloat("PointLightPower", of32LightPow );

		// model
		spShader->SetTexture("TextureDiffuse", m_spTexResDiffuse->GetTexture() );
		spModel->Render();

		// field
		sMtx.Identity();
		spGraphics->SetWorldMatrix(sMtx);
		spShader->SetTexture("TextureDiffuse", m_spTexResField->GetTexture() );
		spDraw3d->RenderArray( hj::clsHJDraw3d::FMT_PCT, hj::clsHJDraw3d::PT_TRIANGLESTRIP, atosFieldPoly, tou32FieldPolyNum);

		spGraphics->ResetShader();
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinMain関数
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT iCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// メモリリーク検出

	// 初期化パラメータ
	hj::stcPfInitParam	sInitParam;
	hj::String::Tcscpy_s( sInitParam.atcApplicationName, HJ_ARRAY_NUM( sInitParam.atcApplicationName ), oatcApplicationName);
	sInitParam.s32ScreenWidth	= tou32ScreenSizeW;
	sInitParam.s32ScreenHeight	= tou32ScreenSizeH;
	sInitParam.bFullScreen		= false;
	sInitParam.hInst			= hInst;
	sInitParam.iCmdShow			= iCmdShow;

	// メイン
	clsMain	cMain;

	// 初期化
	hj_s32 s32Result = 0;
	if( cMain.Initialize( sInitParam ) )
	{
		// ユーザー初期化
		cMain.Init();

		// 背景色
		if( hj::SpHJGraphics spGraphics = hj::clsHJCoreMgr::GS()->GetGraphics() ){
			spGraphics->SetBackColor( hj::HJ_DARKGRAY );
		}

		// ループ開始
		s32Result = cMain.StartLoop();
	}

	return s32Result;
}
