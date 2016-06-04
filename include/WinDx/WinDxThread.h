//--------------------------------------------------------------
/*!	@file
	@brief	WinDxThread
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_THREAD_H_
#define __WINDX_THREAD_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDx Threadクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxThread : public clsHJThread
{
public:
	clsWinDxThread();
	virtual ~clsWinDxThread();

	// --- Method
	virtual hj_bool Initialize(ExecuteFunc fExe, void* pvArg, hj_size_t sizeStack=m_tosizeDefaultStack);
	virtual hj_bool Suspend();
	virtual hj_bool Resume();

	// --- Set
	virtual hj_bool	SetThreadPriority(enmPriority ePrio);
	
protected:
	// --- Variable
	HANDLE	m_hThread;
};

}// namespace hj

#endif // __WINDX_THREAD_H_