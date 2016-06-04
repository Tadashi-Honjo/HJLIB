//--------------------------------------------------------------
/*!	@file
	@brief	WinDx Main
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_MAIN_H_
#define __WINDX_MAIN_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Main
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxMain : public clsHJMain
{
public:
	clsWinDxMain();
	virtual ~clsWinDxMain();
	
	hj_bool			Initialize(const stcPfInitParam& orsInitParam);		//!< 初期化
	hj_s32			StartLoop();										//!< ループ開始

	virtual	void	Update()=0;											//!< ユーザー用更新
	virtual	void	Render()=0;											//!< ユーザー用描画
};
	
}// namespace hj

#endif // __WINDX_MAIN_H_