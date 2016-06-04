//--------------------------------------------------------------
/*!	@file
	@brief	WinDxWindow
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_WINDOW_H_
#define __WINDX_WINDOW_H_

// HJLib namespace
namespace hj{


class clsWinDxWindow : public clsHJSingleton< clsWinDxWindow >
{
public:
	clsWinDxWindow();
	~clsWinDxWindow();

	static LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);	//!< メッセージコールバック関数

	// --- Method
	//! ウィンドウ生成(アプリケーション名削除バージョン)
	hj_bool CreateWindowMy(const hj_tchar* optcAppName, const hj_tchar* optcIconResourceName, hj_s32 width, hj_s32 height, HINSTANCE l_hInst, hj_s32 l_nCmdShow);

	// --- Set
//	void SetAppName( const hj_c8* opc8AppName )	const {	return m_pc8AppName = opc8AppName;	}		//!< アプリケーション名

	// --- Get
	const HWND		GetWindowHandle()const		{	return hWnd;			}	//!< ウィンドウハンドル取得
	const hj_tchar* GetAppName()const			{	return m_optcAppName;	}	//!< アプリケーション名取得
	HINSTANCE		GetHInstance()const			{	return ml_hInst;		}	//!< hInst取得
	INT				GetCmdShow()const			{	return m_iCmdShow;		}	//!< iCmdShow取得

private:
	HWND				hWnd;				// ウィンドウハンドル
	const hj_tchar*		m_optcAppName;		// アプリケーション名
	HINSTANCE			ml_hInst;			// hInst
	INT					m_iCmdShow;			// iCmdShow
	HANDLE				m_hMutex;			// 多重起動制限用
};

} //namespace hj


#endif // __WINDX_WINDOW_H_
