//--------------------------------------------------------------
/*!	@file
	@brief	HJTime
			
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
clsWinDxTime::clsWinDxTime() : clsHJTime()
{
	//タイマー精度を上げる
	timeBeginPeriod(1);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxTime::~clsWinDxTime()
{
	//タイマーの精度を戻す
	timeEndPeriod(1);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ローカルタイム取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxTime::GetLocalTime( stcTime* psOutTime )
{
	SYSTEMTIME stTime;
	::GetLocalTime( &stTime );
	SetTime( psOutTime
			, stTime.wYear
			, static_cast<hj_u8>( stTime.wMonth )
			, static_cast<hj_u8>( stTime.wDayOfWeek )
			, static_cast<hj_u8>( stTime.wDay )
			, static_cast<hj_u8>( stTime.wHour )
			, static_cast<hj_u8>( stTime.wMinute )
			, static_cast<hj_u8>( stTime.wSecond )
			, stTime.wMilliseconds 
		);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	システムタイム取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxTime::GetSystemTime( stcTime* psOutTime )
{
	SYSTEMTIME stTime;
	::GetSystemTime( &stTime );
	SetTime( psOutTime
			, stTime.wYear
			, static_cast<hj_u8>( stTime.wMonth )
			, static_cast<hj_u8>( stTime.wDayOfWeek )
			, static_cast<hj_u8>( stTime.wDay )
			, static_cast<hj_u8>( stTime.wHour )
			, static_cast<hj_u8>( stTime.wMinute )
			, static_cast<hj_u8>( stTime.wSecond )
			, stTime.wMilliseconds 
		);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	起動してからのミリ秒取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_u32	clsWinDxTime::GetWakeUpMSec()const
{
	return timeGetTime();
}

} //namespace hj
