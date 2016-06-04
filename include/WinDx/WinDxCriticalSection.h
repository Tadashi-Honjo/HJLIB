//--------------------------------------------------------------
/*!	@file
	@brief	WinDxCriticalSection
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_CRITICALSECTION_H_
#define __WINDX_CRITICALSECTION_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDx CriticalSectionクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxCriticalSection : public clsHJCriticalSection
{
public:
	clsWinDxCriticalSection();
	virtual ~clsWinDxCriticalSection();
	
	// --- Method
	virtual void Enter();
	virtual void Leave();

protected:
	CRITICAL_SECTION m_CS;
	hj_bool	m_bEnter;
};

}// namespace hj

#endif // __WINDX_CRITICALSECTION_H_