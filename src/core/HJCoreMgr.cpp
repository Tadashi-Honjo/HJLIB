//--------------------------------------------------------------
/*!	@file
	@brief	CoreMgr
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJCoreMgr::clsHJCoreMgr()
: m_spMainHeapMgr(NULL)
, m_spGraphics(NULL)
, m_spDraw2d(NULL)
, m_spDraw3d(NULL)
, m_spDrawPrimitive(NULL)
, m_spTime(NULL)
, m_spFpsMgr(NULL)
, m_spInput(NULL)
, m_spFileMgr(NULL)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJCoreMgr::~clsHJCoreMgr()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デフォルト初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCoreMgr::DefaultInitialize()
{
	stcMatrix44::Initialize();
}

}// namespace hj