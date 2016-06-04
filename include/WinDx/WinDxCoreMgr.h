//--------------------------------------------------------------
/*!	@file
	@brief	WinCoreMgr
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_CORE_H_
#define __WINDX_CORE_H_


// HJLib namespace
namespace hj{

class clsWinDxCoreMgr : public clsHJCoreMgr
{
public:
	clsWinDxCoreMgr();
	virtual ~clsWinDxCoreMgr();
	
	virtual void CreateInstance(const stcPfInitParam& orsInitParam);	// インスタンス作成
	virtual void DeleteInstance();										// インスタンス削除
};
	
}// namespace hj

#endif // __WINDX_CORE_H_