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
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample");

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
	
	virtual	void Update();	//!<本城 ユーザー用更新
	virtual	void Render();	//!<本城 ユーザー用描画

	//本城 インスタンス削除
	virtual	void DeleteInstance(){}
	
	void Init();

private:
	hj::SpTextureResource	m_spTexRes;
	hj::SpHJTexture			m_spTexture;
	hj::SpHJSprite			m_spSprite;
	hj::SpHJSprite			m_spChildSprite;
	hj::SpHJCamera2d		m_spCamera2d;
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

	// スプライト
	m_spSprite.Reset( NEW hj::clsPfSprite() );
	m_spSprite->SetTexture( m_spTexture );
	m_spSprite->SetPosition(0.f, 0.f);

	// スプライト
	m_spChildSprite.Reset( NEW hj::clsPfSprite() );
	m_spChildSprite->SetTexture( m_spTexture );
	m_spChildSprite->SetPosition(100.0f, 100.0f);
	m_spChildSprite->SetScale(0.5f, 1.0f);
	m_spChildSprite->SetParentSprite( *m_spSprite );

	// カメラ2D
	m_spCamera2d.Reset( NEW hj::clsPfCamera2d() );
	m_spCamera2d->SetPosition(0.f, 0.f);
	m_spCamera2d->SetScreenSize( spGraphics->GetScreenWidth(), spGraphics->GetScreenHeight());
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

	// 子スプライト
	m_spChildSprite->SetRotation(f32Cnt * 0.01f);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用描画
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Render()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJDraw2d		spDraw2d	= rcCoreMgr.GetDraw2d();
	
	spGraphics->SetBackColor( hj::HJ_BLACK );
	spGraphics->ClearRenderTarget();
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	//本城 頂点バッファ直書き
	static const hj_f32 g_f32OffsetX = 0;
	static const hj_f32 g_f32OffsetY = 0;
	static const hj::clsHJDraw2d::stcFormat_PC g_sTestVertexs[4] = {
		{-100 + g_f32OffsetX, -100 + g_f32OffsetY, 0xFFFF0000},
		{ 100 + g_f32OffsetX, -100 + g_f32OffsetY, 0xFF00FF00},
		{-100 + g_f32OffsetX,  100 + g_f32OffsetY, 0xFF0000FF},
		{ 100 + g_f32OffsetX,  100 + g_f32OffsetY, 0xFFFFFFFF},
	};
	spDraw2d->Begin();
	spDraw2d->RenderArray( hj::clsHJDraw2d::FMT_PC, hj::clsHJDraw2d::PT_TRIANGLESTRIP, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs));
	spDraw2d->End();

	m_spSprite->Render();
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
	hj_s32 s32Result = 0;

	//本城 初期化
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
