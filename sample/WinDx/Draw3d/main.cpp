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
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample Draw3d");

	static const hj_f32 g_f32OffsetX = 0;
	static const hj_f32 g_f32OffsetY = 0;
	static const hj_f32 g_f32SizeHalf = 0.5f;
	static const hj_f32 g_f32SizeHalf2 = g_f32SizeHalf;
	static const hj::clsHJDraw3d::stcFormat_PCT g_sTestVertexs[] = {
		// z
		{-g_f32SizeHalf, -g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf, -g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf, -g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf,  g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 1.f},


		{-g_f32SizeHalf, -g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf, -g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf, -g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf,  g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 1.f},

		// x
		{ g_f32SizeHalf2, -g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{ g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{ g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf2,  g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},


		{-g_f32SizeHalf2, -g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{-g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{-g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{-g_f32SizeHalf2,  g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},

		// y
		{-g_f32SizeHalf,  g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf,  g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf,  g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf,  g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},


		{-g_f32SizeHalf, -g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf, -g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf, -g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf, -g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf, -g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf, -g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},
	};

} // namespace


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	MainClass
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;
public:
	clsMain():MyClassParent(){}
	virtual ~clsMain(){}
	
	virtual	void Update();	// ユーザー用更新
	virtual	void Render();	// ユーザー用描画

	// インスタンス削除
	virtual	void DeleteInstance(){};
	// 初期化
	void Init();

private:
	hj::SpTextureResource	m_spTexRes;
	hj::SpHJTexture			m_spTexture;
	hj::SpHJSprite			m_spSprite;
	hj::SpHJMesh			m_spMesh;
	
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Init()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJFileMgr		spFileMgr	= rcCoreMgr.GetFileMgr();

	// テクスチャ
	m_spTexRes.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("test.dds"), m_spTexRes );
	m_spTexture = m_spTexRes->GetTexture();

	//本城 スプライト
	m_spSprite.Reset( NEW hj::clsPfSprite() );
	m_spSprite->SetTexture( m_spTexture );
	m_spSprite->SetPosition(0.f, 0.f);

	// メッシュ(初期化でプラットフォーム用に変換して高速化)
	m_spMesh.Reset( NEW hj::clsPfMesh() );
	m_spMesh->CreateMesh( hj::clsHJDraw3d::FMT_PCT, hj::clsHJDraw3d::PT_TRIANGLELIST, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs));
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用更新
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Update()
{
	static hj_f32 f32Cnt = 0.0f;
	f32Cnt += 1.0f;
	m_spSprite->SetRotation(f32Cnt * 0.01f);
	m_spSprite->SetPosition( cos(f32Cnt * 0.01f) * 100.0f, 0.0f);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用描画
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Render()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJDraw3d		spDraw3d	= rcCoreMgr.GetDraw3d();
	
	//本城 描画
	spGraphics->SetBackColor( hj::HJ_GRAY );
	spGraphics->ClearRenderTarget();
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );
	spGraphics->SetTexture( m_spTexture );

	hj::stcMatrix44 sMtx;

	// ビューマトリクス
	static const hj_f32 tof32CameraLength = 30.f;
	sMtx.SetCameraView(
		hj::stcVector3(tof32CameraLength, tof32CameraLength, -tof32CameraLength),
		hj::stcVector3(0.f, 0.f, 0.f),
		hj::stcVector3(0.f, 1.f, 0.f)
	);
	spGraphics->SetViewMatrix(sMtx);

	// プロジェクション
	sMtx.SetCameraProjection( hj::clsHJMath::m_tof32PI/4.0f, 800.f/600.f, 0.01f, 1000000.0f);
	spGraphics->SetProjectionMatrix( sMtx );

	static const hj_s32 tos32MeshNum				= 32;
	static const hj_f32 tof32MeshInterval			= 1.f;
	static const hj_f32 tof32MeshIntervalAll		= static_cast<hj_f32>(tos32MeshNum)*tof32MeshInterval;
	static const hj_f32 tof32MeshIntervalAll_half	= tof32MeshIntervalAll * 0.5f;

	for(hj_s32 s32i=0; s32i<tos32MeshNum; ++s32i)
	{
		for(hj_s32 s32j=0; s32j<tos32MeshNum; ++s32j)
		{
			for(hj_s32 s32k=0; s32k<tos32MeshNum; ++s32k)
			{
				sMtx.SetRotationY( hj::System::GetGameTime() * 100.f);
				sMtx.RotationX( hj::System::GetGameTime() * 100.f);
				sMtx.Translate( hj::stcVector3(	(tof32MeshInterval*static_cast<hj_f32>(s32i))-tof32MeshIntervalAll_half, (tof32MeshInterval*static_cast<hj_f32>(s32j))-tof32MeshIntervalAll_half, (tof32MeshInterval*static_cast<hj_f32>(s32k))-tof32MeshIntervalAll_half) );
				spGraphics->SetWorldMatrix(sMtx);
#if 0
				//本城 頂点バッファ直書き
				spDraw3d->Begin();
				spDraw3d->RenderArray( hj::clsHJDraw3d::FMT_PCT, hj::clsHJDraw3d::PT_TRIANGLELIST, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs));
				spDraw3d->End();
#else
				m_spMesh->Render();
#endif
			}
		}
	}
	
	// sprite 
//	m_spSprite->Render();
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
	if( cMain.Initialize( sInitParam ) )
	{
		//本城 ユーザー初期化
		cMain.Init();

		//本城 背景色
		hj::SpHJGraphics spGraphics	= hj::clsHJCoreMgr::RS().GetGraphics();
		spGraphics->SetBackColor( hj::HJ_GRAY );

		//本城 ループ開始
		s32Result = cMain.StartLoop();
	}

	return s32Result;
}
