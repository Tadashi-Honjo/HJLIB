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
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample Physics");

	const hj_u32 tou32ScreenSizeW = 800;
	const hj_u32 tou32ScreenSizeH = 600;

	hj_f32 g_f32OffsetX = 0;
	hj_f32 g_f32OffsetY = 0;
	hj_f32 g_f32SizeHalf = 30.0f;
	hj_f32 g_f32SizeHalf2 = g_f32SizeHalf;
	hj::clsHJDraw2d::stcFormat_PC g_sTestVertexs[] = {
		{-g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_BLUE},
		{ g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_BLUE},
		{-g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_BLUE},
		{ g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_BLUE},
	};
}





//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	MainClass
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;
public:
	clsMain():MyClassParent(), m_pcPhysicsWorld(NULL)
	{
	}
	virtual ~clsMain(){}
	
	virtual	void Update();	//!<本城 ユーザー用更新
	virtual	void Render();	//!<本城 ユーザー用描画

	//本城 インスタンス削除
	virtual	void DeleteInstance(){
		HJ_SAFE_DELETE(m_pcPhysicsWorld);
	};
	
	void Init();

private:
	Physics::clsWorld*	m_pcPhysicsWorld;
	hj::SpTextureResource	m_spTexRes;
	hj::SpHJTexture			m_spTexture;
	hj::SpHJSprite			m_spSprite;
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Init()
{
	hj::clsHJCoreMgr& rcCoreMgr = hj::clsHJCoreMgr::RS();
	hj::clsHJGraphics& rcGraphics = *rcCoreMgr.GetGraphics();
	hj::clsHJFileMgr& rcFileMgr = *rcCoreMgr.GetFileMgr();

	// テクスチャ
	m_spTexRes.Reset( NEW hj::clsTextureResource() );
	rcFileMgr.RequestLoadResource< hj::clsTextureResource >( HJ_T("test.dds"), m_spTexRes );
	m_spTexture = m_spTexRes->GetTexture();

	// スプライト
	m_spSprite.Reset( NEW hj::clsPfSprite() );
	m_spSprite->SetTexture( m_spTexture );
	m_spSprite->SetPosition(0.f, 0.f);

	// 物理
	m_pcPhysicsWorld = NEW Physics::clsWorld();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用更新
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Update()
{
	m_pcPhysicsWorld->Update();
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用描画
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Render()
{
	hj::clsHJCoreMgr&		rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics		spGraphics	= rcCoreMgr.GetGraphics();
	
	// 描画
	spGraphics->SetBackColor( hj::HJ_DARKSLATEGRAY );
	spGraphics->ClearRenderTarget();
	
#if 0
	hj::stcMatrix44 sMtx;

	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );
	spGraphics->SetTexture( m_spTexture );

	// ビューマトリクス
	static const hj_f32 tof32CameraLength = 5.f;
	sMtx.SetCameraView(
		stcVector3(tof32CameraLength, tof32CameraLength, -tof32CameraLength),
		stcVector3(0.f, 0.f, 0.f),
		stcVector3(0.f, 1.f, 0.f)
	);
	spGraphics->SetViewMatrix(sMtx);

	// プロジェクション
	sMtx.SetCameraProjection( hj::clsHJMath::m_tof32PI/4.0f, static_cast<hj_f32>(tou32ScreenSizeW)/static_cast<hj_f32>(tou32ScreenSizeH), 0.01f, 100.0f);
	spGraphics->SetProjectionMatrix( sMtx );

	sMtx.setRotationY( hj::System::GetGameTime() * 100.f);
	sMtx.rotationX( hj::System::GetGameTime() * 100.f);
	sMtx.translate( hj::stcVector3(0.f, 0.f, 0.f) );
	spGraphics->SetWorldMatrix( sMtx );

	// 描画
	m_pcMesh->Render();
#else
	hj::SpHJDraw2d spDraw2d = rcCoreMgr.GetDraw2d();
	spDraw2d->Begin();
	spDraw2d->RenderArray( hj::clsHJDraw2d::FMT_PC, hj::clsHJDraw2d::PT_TRIANGLESTRIP, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs));
	spDraw2d->End();
#endif

	// 物理描画
	m_pcPhysicsWorld->Render_debug();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinMain関数
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT iCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// メモリリーク検出

	// 初期化パラメータ
	hj::stcPfInitParam sInitParam;
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
	if( cMain.Initialize( sInitParam ) ){

		// ユーザー初期化
		cMain.Init();

		// 背景色
		hj::SpHJGraphics spGraphics = hj::clsHJCoreMgr::GS()->GetGraphics();
		spGraphics->SetBackColor( hj::HJ_DARKGRAY );

		// ループ開始
		s32Result = cMain.StartLoop();
	}

	return s32Result;
}
