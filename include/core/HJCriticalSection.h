//--------------------------------------------------------------
/*!	@file
	@brief	HJCriticalSection
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_CRITICALSECTION_H_
#define __HJ_CRITICALSECTION_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	CriticalSectionクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJCriticalSection
{
public:
	clsHJCriticalSection();
	virtual ~clsHJCriticalSection();
	
	// --- Method
	virtual void Enter()=0;
	virtual void Leave()=0;

protected:
};


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	CriticalSectionScopeクラス(スコープ用ユーティリティ)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJCriticalSectionScope
{
public:
	clsHJCriticalSectionScope( SpHJCriticalSection spCS ):m_spCS(spCS){ m_spCS->Enter(); }
	virtual ~clsHJCriticalSectionScope(){ m_spCS->Leave(); }
private:
	SpHJCriticalSection m_spCS;
};

}// namespace hj

#endif // __HJ_CRITICALSECTION_H_