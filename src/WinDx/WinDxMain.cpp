//--------------------------------------------------------------
/*!	@file
	@brief	WinDx Main
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxMain::clsWinDxMain()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxMain::~clsWinDxMain()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxMain::Initialize(const stcPfInitParam& orsInitParam)
{
	do{
		// コアマネージャ作成
		NEW clsWinDxCoreMgr();

		// ウィンドウ作成
		NEW clsWinDxWindow();
		if( !clsWinDxWindow::RS().CreateWindowMy( orsInitParam.atcApplicationName, orsInitParam.atcIconResourceName, orsInitParam.s32ScreenWidth, orsInitParam.s32ScreenHeight, orsInitParam.hInst, orsInitParam.iCmdShow ) ){
			HJ_ASSERT_MSG( HJ_T("ウィンドウ作成失敗。") );
			break;
		}

		// 管理インスタンス作成
		clsWinDxCoreMgr::RS().CreateInstance( orsInitParam );

		// 成功
		return true;

	}while(false);

	// 失敗
	clsWinDxCoreMgr::RS().DeleteInstance();
	if(clsWinDxCoreMgr::GS()) delete(clsWinDxCoreMgr::GS());
	if(clsWinDxWindow::GS()) delete(clsWinDxWindow::GS());

	return false;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ループ開始
	@return 結果
@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_s32 clsWinDxMain::StartLoop()
{
	MSG msg;	// メッセージ
	ZeroMemory( &msg, sizeof(msg) );

	clsHJCoreMgr&	rcCoreMgr	= clsHJCoreMgr::RS();
	SpHJFpsMgr		spFpsMgr	= rcCoreMgr.GetFpsMgr();
	SpHJInput		spInput		= rcCoreMgr.GetInput();
	SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();

	clsHJFpsMgr&		rcFpsMgr	= *spFpsMgr;
	clsHJInput&			rcInput		= *spInput;
	clsWinDxGraphics&	rcGraphics	= *static_cast<clsWinDxGraphics*>( spGraphics.Get() );
	LPDIRECT3DDEVICE9	pDevice		= rcGraphics.GetDevice();
		
	// ループ
	while( msg.message!=WM_QUIT && !m_bLoopEnd )
	{
		if(PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// 処理前の時間設定
			rcFpsMgr.SetOldTime();

			// キー更新
			rcInput.Update();

			// メインループ
			Update();

			//シーンの開始
			if( SUCCEEDED( pDevice->BeginScene() ))
			{
				// バックバッファに切り替えて描画
				rcGraphics.SetRenderTargetBackBuffer();
				rcGraphics.ClearRenderTarget();

				// 描画呼び出し
				Render();

				//シーン終了
				pDevice->EndScene();
			}

			// 画面更新
			if( FAILED(rcGraphics.DisplayUpDate()) ){
				break;
			}

			// Fpsチェック
			rcFpsMgr.ChkWait();

			// FPS表示
//			OR( HJ_T("FPS : %.2f"), rcFpsMgr.GetNowFps());
		}
	}

	// Main管理のインスタンスを削除させておく
	DeleteInstance();

	// 抜ける時に削除
	clsWinDxCoreMgr::RS().DeleteInstance();
	delete( clsWinDxCoreMgr::GS() );
	delete( clsWinDxWindow::GS() );

	// 結果
	return msg.wParam;
}


}// namespace hj