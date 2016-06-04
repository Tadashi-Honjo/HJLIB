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
clsHJTime::clsHJTime()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJTime::~clsHJTime()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	全設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJTime::SetTime( stcTime* psOutTime, hj_u16	u16Year, hj_u8 u8Month, hj_u8 u8DayOfWeek, hj_u8 u8Day, hj_u8 u8Hour, hj_u8 u8Minute, hj_u8 u8Second, hj_u16 u16MilliSeconds )
{
	psOutTime->u16Year = u16Year;
	psOutTime->u8Month = u8Month;
	psOutTime->u8DayOfWeek = u8DayOfWeek;
	psOutTime->u8Day = u8Day;
	psOutTime->u8Hour = u8Hour;
	psOutTime->u8Minute = u8Minute;
	psOutTime->u8Second = u8Second;
	psOutTime->u16MilliSeconds = u16MilliSeconds;
}

} //namespace hj
