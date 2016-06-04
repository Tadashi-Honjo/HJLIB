//--------------------------------------------------------------
/*!	@file
	@brief	HJFpsマネージャ
			
	@author 本城 直志
	@date	11/11/06
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HjLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJFpsMgr::clsHJFpsMgr():
m_u32FrameTime(0)
, m_u32OldTime(0)
, m_u32Fps(0)
, m_f32NowFps(0.f)
, m_f32NowFpsSmooth(0.f)
{
	//FPSを設定
	SetFps( m_tou32DefaultFps );

	//時間を取得しておく
	m_u32OldTime = timeGetTime();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJFpsMgr::~clsHJFpsMgr()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	時間を調べてFpsを調整する
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJFpsMgr::ChkWait()
{
	clsHJTime& rcTime = *clsHJCoreMgr::GS()->GetTime();
	hj_u32 u32NowTimeMSec = rcTime.GetWakeUpMSec();	//現在の時間の取得

	//処理前の時間と処理後の時間差を求める
	hj_u32 u32MSecDiff = u32NowTimeMSec - m_u32OldTime;

	//処理落ちしていないか
	if( u32MSecDiff < m_u32FrameTime ){
		System::Sleep( m_u32FrameTime - u32MSecDiff );
		System::SetDeltaTime( static_cast<hj_f32>(m_u32FrameTime) / 1000.f );
	// 処理落ち
	}else{
		System::SetDeltaTime( static_cast<hj_f32>(u32MSecDiff) / 1000.f );
	}
	System::AddGameTime( System::GetDeltaTime() );

	// 現在のFPSに変換
	if(u32MSecDiff > 0.0f){
		m_f32NowFps = static_cast<hj_f32>(m_u32FrameTime) / static_cast<hj_f32>(u32MSecDiff) * static_cast<hj_f32>(m_u32Fps);
		m_f32NowFpsSmooth += (m_f32NowFps - m_f32NowFpsSmooth) * 0.1f;
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	処理前の時間をセットする
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJFpsMgr::SetOldTime()
{
	clsHJTime& rcTime = *clsHJCoreMgr::GS()->GetTime();
	m_u32OldTime = rcTime.GetWakeUpMSec();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	FPSを設定する
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJFpsMgr::SetFps(hj_u32 u32Fps)
{
	m_u32Fps = u32Fps;
	m_u32FrameTime = static_cast<hj_u32>(1000.0f / (hj_f32)u32Fps);	//FPSから1フレームの時間の算出

	m_f32NowFps = static_cast<hj_f32>(u32Fps);
	m_f32NowFpsSmooth = static_cast<hj_f32>(u32Fps);
}

} //namespace hj
