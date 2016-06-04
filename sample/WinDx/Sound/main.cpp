//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@file
	@brief	HJLib Sound Sample Main
			
	@author 本城 直志
	@date	12/08/19
*/
//	Copyright (C) 2012 HonjoTadashi. All Rights Reserved. 
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 

namespace{

	// アプリケーション名
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sound Sample");

} // namespace


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	MainClass
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;
public:
	clsMain():MyClassParent(), m_bPlayBgm(false), m_f32Volume(1.f){}
	virtual ~clsMain(){}

	// ユーザー用更新
	virtual	void Update();
	// ユーザー用描画
	virtual	void Render();
	// インスタンス削除
	virtual	void DeleteInstance(){
		m_spSound1->Stop();
		m_spSound1.Reset();
		m_spSound2->Stop();
		m_spSound2.Reset();
		m_spSoundStreamRes.Reset();
		m_spSoundRes.Reset();
	}
	// 初期化
	void Init();

private:
	hj_bool							m_bPlayBgm;
	hj_f32							m_f32Volume;
	hj::SpSoundStreamResource		m_spSoundStreamRes;
	hj::SpSoundResource				m_spSoundRes;
	hj::SpHJSound					m_spSound1, m_spSound2;
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Init()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJFileMgr		spFileMgr	= rcCoreMgr.GetFileMgr();

	m_spSoundStreamRes.Reset( NEW hj::clsSoundStreamResource() );
	spFileMgr->RequestLoadResource< hj::clsSoundStreamResource >( HJ_T("sound/sample_bgm.wav"), m_spSoundStreamRes );
	m_spSound1 = m_spSoundStreamRes->GetSound();
	m_bPlayBgm = true;
	m_spSound1->Play(true);

	m_spSoundRes.Reset( NEW hj::clsSoundResource() );
	spFileMgr->RequestLoadResource< hj::clsSoundResource >( HJ_T("sound/sample_se.wav"), m_spSoundRes );
	m_spSound2 = m_spSoundRes->GetSound();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用更新
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Update()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJInput		spInput		= rcCoreMgr.GetInput();

	// BGM
	if( spInput->IsTrig( hj::clsHJInput::KEY_Z ) )
	{
		if( m_bPlayBgm ){
//			m_spSound1->Stop();
			m_spSound1->Pause();
		}else{
			m_spSound1->Play(true);
		}
		m_bPlayBgm = !m_bPlayBgm;
	}
	// SE
	else if( spInput->IsTrig( hj::clsHJInput::KEY_X ) ){
		m_spSound2->Play();
	}

	// ボリューム変更
	hj_bool bChangeVolume = false;
	if( spInput->IsRep( hj::clsHJInput::KEY_UP ) ){
		m_f32Volume = HJ_MIN(m_f32Volume+0.1f, 1.f);
		bChangeVolume = true;
	}else if( spInput->IsRep( hj::clsHJInput::KEY_DOWN ) ){
		m_f32Volume = HJ_MAX(m_f32Volume-0.1f, 0.f);
		bChangeVolume = true;
	}
	if( bChangeVolume ){
		m_spSound1->SetVolume(m_f32Volume);
		m_spSound2->SetVolume(m_f32Volume);
	}

	// ストリーミングは更新が必要
	m_spSound1->Update();	
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用描画
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Render()
{
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
	sInitParam.s32ScreenWidth	= 800;
	sInitParam.s32ScreenHeight	= 600;
	sInitParam.bFullScreen		= false;
	sInitParam.hInst			= hInst;
	sInitParam.iCmdShow			= iCmdShow;

	// メイン
	clsMain	cMain;
	cMain.Initialize( sInitParam );	// 初期化

	// ユーザー初期化
	cMain.Init();

	// 背景色
	if( hj::SpHJGraphics spGraphics = hj::clsHJCoreMgr::GS()->GetGraphics() ){
		spGraphics->SetBackColor( hj::HJ_DARKGRAY );
	}

	// ループ開始
	hj_s32 s32Result = cMain.StartLoop();

	return s32Result;
}
