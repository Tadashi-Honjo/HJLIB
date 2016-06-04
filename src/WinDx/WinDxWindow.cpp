//--------------------------------------------------------------
/*!	@file
	@brief	WinDxWindow
			
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
clsWinDxWindow::clsWinDxWindow():
hWnd(0)
, m_optcAppName(NULL)
, ml_hInst(0)
, m_iCmdShow(0)
, m_hMutex(NULL)
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxWindow::~clsWinDxWindow()
{
	if(m_hMutex!=NULL){
		ReleaseMutex( m_hMutex );
		CloseHandle( m_hMutex );
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ウィンドウ生成
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxWindow::CreateWindowMy(const hj_tchar* optcAppName, const hj_tchar* optcIconResourceName, hj_s32 width, hj_s32 height, HINSTANCE l_hInst, INT l_nCmdShow)
{
	hj_bool bResult = false;
	do{
		m_hMutex = CreateMutex(NULL, FALSE, optcAppName);
//		m_hMutex = CreateMutex(NULL, FALSE, HJ_T("Multiple Launch Check"));
		// すでに起動しているか判定
		if(GetLastError() == ERROR_ALREADY_EXISTS){
			// 起動済み
			MessageBox(NULL, HJ_T("多重起動はできません。"), optcAppName, MB_OK);
			break;
		}

		// ウィンドウの初期化
		WNDCLASSEX  wndclass ;
		//ステータスの設定
		wndclass.cbSize        =	sizeof (wndclass);
		wndclass.style         =	CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc   =	WndProc;
		wndclass.cbClsExtra    =	0;
		wndclass.cbWndExtra    =	0;
		wndclass.hInstance     =	l_hInst;
	//    wndclass.hIcon         =	LoadIcon (NULL, IDI_APPLICATION) ;
		wndclass.hIcon         =	LoadIcon (l_hInst, optcIconResourceName);
		wndclass.hCursor       =	LoadCursor (NULL, IDC_ARROW);
		wndclass.hbrBackground =	(HBRUSH) GetStockObject (BLACK_BRUSH);
		wndclass.lpszMenuName  =	NULL ;
		wndclass.lpszClassName =	optcAppName;
		wndclass.hIconSm       =	LoadIcon (l_hInst, optcIconResourceName);

		//ウィンドウの調整
		width = width + GetSystemMetrics(SM_CXDLGFRAME) * 2;
		height = height + GetSystemMetrics(SM_CYDLGFRAME) * 2;

		//ウィンドウクラスの登録
		if(!RegisterClassEx (&wndclass)) {
			HJ_ASSERT_MSG( HJ_T("WindowClass Create Error!!"));	//ウィンドウクラスの作成に失敗しました
			break;
		}

		//ウィンドウの生成
		hWnd = CreateWindow( optcAppName, optcAppName,
	//					WS_OVERLAPPEDWINDOW,  
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
						GetSystemMetrics(SM_CXSCREEN)/2 - width/2,
						GetSystemMetrics(SM_CYSCREEN)/2 - height/2,
						width,
						height,
						NULL,
						NULL, 
						l_hInst,
						(LPSTR)NULL
		);

		if(hWnd == NULL){
			HJ_ASSERT_MSG( HJ_T("Window Create Error!!"));	//ウィンドウの生成に失敗しました
			break;
		}

		//ウィンドウの表示
		ShowWindow (hWnd,SW_SHOW);
		UpdateWindow (hWnd);

		m_optcAppName	= optcAppName;
		ml_hInst		= l_hInst;
		m_iCmdShow		= l_nCmdShow;

		bResult = true;

	}while(false);

	return bResult;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	メッセージコールバック関数
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
LRESULT CALLBACK clsWinDxWindow::WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{	
	switch(iMsg)
	{
#if 0 // アプリケーション側で用意する
	//キーの認識
	case WM_KEYDOWN:
		switch((CHAR)wParam)
		{
		//Escで終了
		case VK_ESCAPE:
			{
				PostQuitMessage(0);
			}
			break;
		//フルスクリーン
		case VK_F1:
			{
				clsHJGraphics& rcGraphics = *clsHJCoreMgr::RS().GetGraphics();
				static_cast<clsWinDxGraphics*>(&rcGraphics)->ChangeDisplayMode();
			}
			break;
		}
		return 0;
#endif
	//マウス位置
	case WM_MOUSEMOVE:
#if 0
		if(clsInput::GS() != NULL)
		{
			clsInput::GS()->SetMouPos(LOWORD(lParam), HIWORD(lParam));//位置設定
		}
		return 0;
#endif
	//ウィンドウ生成
	case WM_CREATE:{
		}
		return 0;
	//ウィンドウクローズ
	case WM_CLOSE:{
			::SendMessage( hWnd, WM_DESTROY, 0, 0);
		}
		return 0;
	//終了時
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
		}
		return 0;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam) ;
}


} //namespace hj
