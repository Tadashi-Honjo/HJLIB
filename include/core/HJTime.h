//--------------------------------------------------------------
/*!	@file
	@brief	HJTime
			
	@author 本城 直志
	@date	11/11/06
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_TIME_
#define __HJ_TIME_


// HjLib namespace
namespace hj{


// ----- Class
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	HJTimeクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJTime{
public:	
	clsHJTime();
	virtual ~clsHJTime();

	// --- Enum
	//! 曜日
	enum enmDayOfWeek{
		MONDAY,		// 月
		TUESDAY,	// 火
		WEDNESDAY,	// 水
		THURSDAY,	// 木
		FRIDAY,		// 金
		SATURDAY,	// 土
		SUNDAY,		// 日
	};
	
	// --- Struct
	//! 時間
	struct stcTime{
		hj_u16	u16Year;		// 年
		hj_u8	u8Month;		// 月
		hj_u8	u8DayOfWeek;	// 曜日
		hj_u8	u8Day;			// 日
		hj_u8	u8Hour;			// 時
		hj_u8	u8Minute;		// 分
		hj_u8	u8Second;		// 秒
		hj_u16	u16MilliSeconds;// ミリ秒
	};

	// --- Mehod
	virtual void	GetLocalTime( stcTime* psOutTime )=0;	//!< ローカルタイム取得
	virtual void	GetSystemTime( stcTime* psOutTime )=0;	//!< システムタイム取得
	virtual hj_u32	GetWakeUpMSec()const=0;					//!< 起動してからのミリ秒取得

	// --- Set
	virtual void	SetTime( stcTime* psOutTime, hj_u16	u16Year, hj_u8 u8Month, hj_u8 u8DayOfWeek, hj_u8 u8Day, hj_u8 u8Hour, hj_u8 u8Minute, hj_u8 u8Second, hj_u16 u16MilliSeconds );	// 全設定
};


} //namespace hj

#endif // __HJ_TIME_
