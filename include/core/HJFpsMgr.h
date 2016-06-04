//--------------------------------------------------------------
/*!	@file
	@brief	HJFpsマネージャ
			
	@author 本城 直志
	@date	11/11/06
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_FPS_MANAGER_
#define __HJ_FPS_MANAGER_


// HjLib namespace
namespace hj{


// ----- Class
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	FPS管理クラス
//
//			フレームレートを計測し、管理します
//
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJFpsMgr{
public:	
	clsHJFpsMgr();
	~clsHJFpsMgr();

	// --- Mehod
	void ChkWait();					//!< 時間を調べてFpsを調整する

	// --- Set
	void SetOldTime();				//!< 処理前の時間をセットする
	void SetFps(hj_u32 u32Fps);		//!< FPSを設定する

	// --- Get
	hj_u32 GetFps()const			{	return m_u32Fps;			}	//!< FPS目標値取得
	hj_f32 GetNowFps()const			{	return m_f32NowFps;			}	//!< 現在のFPS取得
	hj_f32 GetNowFpsSmooth()const	{	return m_f32NowFpsSmooth;	}	//!< 補間をかけた現在のFPS取得

	// --- Variable
	static const hj_u32 m_tou32DefaultFps = 60;

private:
	// ----- Variable
	hj_u32	m_u32FrameTime;		//!< 1フレームの時間
	hj_u32	m_u32OldTime;		//!< 各タスクの処理前の時間
	hj_u32	m_u32Fps;			//!< FPS目標値
	hj_f32	m_f32NowFps;		//!< 現在のFPS
	hj_f32	m_f32NowFpsSmooth;	//!< 補間をかけたFPS
};


} //namespace hj

#endif
