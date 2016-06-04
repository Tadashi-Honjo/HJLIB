//--------------------------------------------------------------
/*!	@file
	@brief	CoreMgr
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxCoreMgr::clsWinDxCoreMgr()
: clsHJCoreMgr()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxCoreMgr::~clsWinDxCoreMgr()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	インスタンス作成
	@param  orsInitParam	初期化パラメータ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxCoreMgr::CreateInstance(const stcPfInitParam& orsInitParam)
{
	DefaultInitialize();	// デフォルト初期化

	// Heap
//	m_spMainHeapMgr.Reset( NEW clsHJHeapMemoryMgr( orsInitParam.u32MainHeapSize ) );	// 今は使わない

	// Graphics
	{
		clsWinDxGraphics* pcGraphics = NEW clsWinDxGraphics();
		m_spGraphics.Reset( pcGraphics );
		pcGraphics->InitDirectGraphics(clsWinDxWindow::RS().GetWindowHandle(), orsInitParam.s32ScreenWidth, orsInitParam.s32ScreenHeight, orsInitParam.bFullScreen);
	}

	// Draw2d
	m_spDraw2d.Reset( NEW clsWinDxDraw2d() );
	// Draw3d
	m_spDraw3d.Reset( NEW clsWinDxDraw3d() );
	// DrawPrimitive
	m_spDrawPrimitive.Reset( NEW clsHJDrawPrimitive() );
	// Time
	m_spTime.Reset( NEW clsWinDxTime() );
	// FpsMgr
	m_spFpsMgr.Reset( NEW clsHJFpsMgr() );

	// Input
	{
		clsWinDxInput* pcDxInput = NEW clsWinDxInput();
		m_spInput.Reset( pcDxInput );
		pcDxInput->Init( orsInitParam.hInst, clsWinDxWindow::RS().GetWindowHandle() );	// 内部のコールバックが初期化時に来るので設定した後に初期化
	}

	// FileMgr
	m_spFileMgr.Reset( NEW clsWinDxFileMgr() );

	// sound
	clsWinDxSound::Initalize();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	インスタンス削除
	@return -
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxCoreMgr::DeleteInstance()
{
	clsWinDxSound::Uninitalize();
}

}// namespace hj