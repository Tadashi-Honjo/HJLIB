//--------------------------------------------------------------
/*!	@file
	@brief	WinDx Time
			
	@author 本城 直志
	@date	11/11/06
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_TIME_H_
#define __WINDX_TIME_H_


// HjLib namespace
namespace hj{


// ----- Class
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDx Timeクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxTime : public clsHJTime{
public:	
	clsWinDxTime();
	virtual ~clsWinDxTime();

	// --- Mehod
	virtual void	GetLocalTime( stcTime* psOutTime );		//!< ローカルタイム取得
	virtual void	GetSystemTime( stcTime* psOutTime );	//!< システムタイム取得
	virtual hj_u32	GetWakeUpMSec()const;					//!< 起動してからのミリ秒取得
};


} //namespace hj

#endif // __WINDX_TIME_H_
