//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@file
	@brief	HJLib Sample Main
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 

#include <core/HJString.h>

namespace{
	//本城 アプリケーション名
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample DrawModel");

} // namespace


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	MainClass
	
	@author 本城
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
		, m_eCullMode( hj::clsHJGraphics::CULL_CW )
	{
	}
	virtual ~clsMain(){}
	
	// ユーザー用更新
	virtual	void Update();	
	// ユーザー用描画
	virtual	void Render();
	// インスタンス削除
	virtual	void DeleteInstance(){}
	// 初期化
	void Init();

private:
	hj::SpTextureResource		m_spTexResDiffuse;
	hj::SpTextureResource		m_spTexResSpecular;
	hj::SpTextureResource		m_spTexResNormal;
	hj::SpHJTexture				m_spTexDiffuse;
	hj::SpHJTexture				m_spTexSpecular;
	hj::SpHJTexture				m_spTexNormal;
	hj::SpModelResource			m_spModelRes;
	hj::SpShaderResource		m_spShaderRes;
	hj::SpHJModel				m_spModel;
	hj::SpHJShader				m_spShader;
	hj::stcVector3				m_sCameraPos, m_sCameraLookPos;
	hj_f32						m_f32CameraDegX, m_f32CameraDegY;
	hj_f32						m_f32CameraLength;
	hj::clsHJGraphics::enmCull	m_eCullMode;
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Init()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJFileMgr		spFileMgr	= rcCoreMgr.GetFileMgr();

	// テクスチャ
	m_spTexResDiffuse.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("diffuse.dds"), m_spTexResDiffuse );
	m_spTexDiffuse = m_spTexResDiffuse->GetTexture();

	m_spTexResSpecular.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("specular.dds"), m_spTexResSpecular );
	m_spTexSpecular = m_spTexResSpecular->GetTexture();

	m_spTexResNormal.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("normal.dds"), m_spTexResNormal );
	m_spTexNormal = m_spTexResNormal->GetTexture();

	// モデル
	m_spModelRes.Reset( NEW hj::clsModelResource() );
	spFileMgr->RequestLoadResource< hj::clsModelResource >( HJ_T("test.hjmd"), m_spModelRes );
	m_spModel = m_spModelRes->GetModel();

	// シェーダー作成
	m_spShaderRes.Reset( NEW hj::clsShaderResource() );
	spFileMgr->RequestLoadResource< hj::clsShaderResource >( HJ_T("ModelShader.fx"), m_spShaderRes );
	m_spShader = m_spShaderRes->GetShader();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用更新
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Update()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJInput		spInput		= rcCoreMgr.GetInput();

	static const hj_f32 tof32RotSpeed = 2.f;

	// rot
	m_f32CameraDegX += spInput->GetPadAnalog(hj::clsHJInput::PAD_STICK2_Y) * tof32RotSpeed;
	m_f32CameraDegY -= spInput->GetPadAnalog(hj::clsHJInput::PAD_STICK2_X) * tof32RotSpeed;
	
	// length
	static const hj_f32 tof32LengthSpeed = 0.5f;
	m_f32CameraLength += spInput->GetPadAnalog(hj::clsHJInput::PAD_SLIDER1) * tof32LengthSpeed;
	m_f32CameraLength -= spInput->GetPadAnalog(hj::clsHJInput::PAD_SLIDER2) * tof32LengthSpeed;
	m_f32CameraLength = HJ_MAX( m_f32CameraLength, 1.f);

	// position
	hj_f32 f32RadX = hj::clsHJMath::DegToRad(m_f32CameraDegX);
	hj_f32 f32RadY = hj::clsHJMath::DegToRad(m_f32CameraDegY);
	m_sCameraPos.x = m_sCameraLookPos.x + (hj::clsHJMath::Sin(f32RadY) * hj::clsHJMath::Cos(f32RadX) * m_f32CameraLength);
	m_sCameraPos.z = m_sCameraLookPos.y + (hj::clsHJMath::Cos(f32RadY) * hj::clsHJMath::Cos(f32RadX) * m_f32CameraLength);
	m_sCameraPos.y = m_sCameraLookPos.z + (hj::clsHJMath::Sin(f32RadX) * m_f32CameraLength);

	// Change Draw Mode
	if(spInput->IsTrig( hj::clsHJInput::PAD_BTN1 ))
	{
		if(m_spModel->GetDrawMode() == hj::clsHJModel::DRAW_MODE_NUM-1){
			m_spModel->SetDrawMode(static_cast<hj::clsHJModel::enmDrawMode>(0));
		}else{
			m_spModel->SetDrawMode(static_cast<hj::clsHJModel::enmDrawMode>(m_spModel->GetDrawMode()+1));
		}
	}

	// Change Cull Mode
	if( spInput->IsTrig( hj::clsHJInput::PAD_BTN3 ) ){
		if(m_eCullMode == hj::clsHJGraphics::CULL_MAX_NUM-1){
			m_eCullMode = static_cast<hj::clsHJGraphics::enmCull>(0);
		}else{
			m_eCullMode = static_cast<hj::clsHJGraphics::enmCull>(m_eCullMode+1);
		}
	}
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用描画
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Render()
{
	hj::clsHJCoreMgr&		rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics		spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJDrawPrimitive	spPrimitive = rcCoreMgr.GetDrawPrimitive();
	
	//本城 描画
	spGraphics->SetBackColor( hj::HJ_DARKSLATEGRAY );
	spGraphics->ClearRenderTarget();
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	// カメラ情報
	static const hj_f32 tof32CameraLength = 30.f;
	const hj::stcVector3& osUp = hj::stcVector3(0.f, 1.f, 0.f);

	// grid
	spPrimitive->DrawGridXz(10.0f, 10, hj::HJ_WHITE );
	// axis
	spPrimitive->DrawAxis(10.0f);

	// WVP
	hj::stcMatrix44 sMtx;
	// view
	sMtx.SetCameraView(	m_sCameraPos, m_sCameraLookPos, osUp );
	spGraphics->SetViewMatrix(sMtx);
	// projection
	sMtx.SetCameraProjection( hj::clsHJMath::m_tof32PI/4.0f, 800.f/600.f, 0.01f, 1000000.0f );
	spGraphics->SetProjectionMatrix(sMtx);
	// world
	sMtx.Identity();
	spGraphics->SetWorldMatrix(sMtx);

	//本城 シェーダー設定
	m_spShader->SetTechnique("TModelShader");
	m_spShader->SetTexture("TextureDiffuse", m_spTexDiffuse);
	m_spShader->SetTexture("TextureSpecular", m_spTexSpecular);
	m_spShader->SetTexture("TextureNormal", m_spTexNormal);
	m_spShader->SetVector3("EyePos", m_sCameraPos );
	spGraphics->SetShader( m_spShader );

	// Set Cull Mode
	spGraphics->SetCullMode(m_eCullMode);

	// x 10
	static const hj_u32 otDrawNum = 10;
	for(hj_u32 i=0; i<otDrawNum; ++i)
	{
		// world
		m_spModel->SetRotation( hj::stcVector3(0.f, hj::System::GetGameTime() * 30.f, 0.f));
		m_spModel->SetPosition( hj::stcVector3((static_cast<hj_f32>(i)*60.f)-300.f, 0.f, 0.f));
		// render
		m_spModel->Render();
	}

	spGraphics->ResetShader();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinMain関数
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT iCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// メモリリーク検出

	//本城 初期化パラメータ
	hj::stcPfInitParam sInitParam;
	hj::String::Tcscpy_s( sInitParam.atcApplicationName, HJ_ARRAY_NUM( sInitParam.atcApplicationName ), oatcApplicationName);
	sInitParam.s32ScreenWidth	= 800;
	sInitParam.s32ScreenHeight	= 600;
	sInitParam.bFullScreen		= false;
	sInitParam.hInst			= hInst;
	sInitParam.iCmdShow			= iCmdShow;

	//本城 メイン
	clsMain	cMain;
	//本城 初期化
	hj_s32 s32Result = 0;
	if( cMain.Initialize( sInitParam ) ){

		//本城 ユーザー初期化
		cMain.Init();

		// 背景色
		hj::SpHJGraphics spGraphics = hj::clsHJCoreMgr::GS()->GetGraphics();
		spGraphics->SetBackColor( hj::HJ_DARKGRAY );

		//本城 ループ開始
		s32Result = cMain.StartLoop();
	}

	return s32Result;
}
