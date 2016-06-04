//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@file
	@brief	HJLib Thread Sample
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 HonjoTadashi. All Rights Reserved. 
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 

namespace{

	// アプリケーション名
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample Thread");
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	MainClass
	
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;
public:
	clsMain(): MyClassParent(){}
	virtual ~clsMain(){}
	
	// ユーザー用更新
	virtual	void Update();
	// ユーザー用描画
	virtual	void Render();
	// インスタンス削除
	virtual	void DeleteInstance(){}
	// 初期化
	void Init();

	// 同時に呼んだら止まるメソッド
	void CheckCallThread()
	{
		static bool bCheck = false;
		HJ_ASSERT(!bCheck, HJ_T("同時に呼ばれています。"));
		bCheck = true;
		hj::System::Sleep(1000);
		bCheck = false;
	};

	// スレッド用メソッド
	static void ExecuteThread(void* pvArg);

	// --- Get
	hj::SpHJThread			GetThread()	{	return m_spThread;	}
	hj::SpHJCriticalSection	GetCS()		{	return m_spCS;		}

private:
	hj::SpHJThread				m_spThread;
	hj::SpHJCriticalSection		m_spCS;
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Init()
{
	// スレッド
	m_spThread.Reset( NEW hj::clsPfThread() );
	m_spThread->Initialize( ExecuteThread, this);
	m_spThread->Resume();
//	m_spThread->SetThreadPriority( hj::clsHJThread::PRIO_HIGHEST );

	// クリティカルセクション
	m_spCS.Reset( NEW hj::clsPfCriticalSection() );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	スレッド更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::ExecuteThread(void* pvArg)
{
	clsMain* pcMain = reinterpret_cast<clsMain*>(pvArg);
	hj_s32 s32Cnt = 0;
	do{
		hj::System::Sleep(500);
		{
			hj::clsHJCriticalSectionScope cCsScope( pcMain->GetCS() );
			pcMain->CheckCallThread();
			OR( HJ_T("thread Execute!!") );
			++s32Cnt;
		}
//		if(s32Cnt >= 10){
//			pcMain->GetThread()->Suspend();
//		}
	}while(s32Cnt < 5);

	// スレッド終了
	{
		hj::clsHJCriticalSectionScope cCsScope( pcMain->GetCS() );
		pcMain->CheckCallThread();
		OR(HJ_T("thread End!!"));
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Update()
{
	// メインスレッド
	{
		hj::clsHJCriticalSectionScope cCsScope( GetCS() );
		CheckCallThread();
		OR(HJ_T("MainThread Execute!!"));
	}
	hj::System::Sleep(500);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ユーザー用描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsMain::Render()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinMain関数
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
