//--------------------------------------------------------------
/*!	@file
	@brief	WinDxThread
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
// HJLib namespace
namespace hj{

namespace{
	DWORD WINAPI ThreadFunc(LPVOID lpThreadParameter) {
		reinterpret_cast<clsWinDxThread*>(lpThreadParameter)->_ExecuteThread();
		ExitThread(TRUE);
	}

	static const hj_s32  atos32ThreadPrio[] = {
		THREAD_PRIORITY_HIGHEST,		// 基準 +2
		THREAD_PRIORITY_ABOVE_NORMAL,	// 基準 +1
		THREAD_PRIORITY_NORMAL,			// 基準
		THREAD_PRIORITY_BELOW_NORMAL,	// 基準 -1
		THREAD_PRIORITY_LOWEST,			// 基準 -2
	};
	HJ_STATIC_ASSERT(clsHJThread::PRIO_MAXNUM==HJ_ARRAY_NUM(atos32ThreadPrio));
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxThread::clsWinDxThread()
:clsHJThread()
, m_hThread(NULL)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxThread::~clsWinDxThread()
{
	DWORD dwParam;
	GetExitCodeThread(m_hThread, &dwParam);
	if(dwParam == STILL_ACTIVE) {
		TerminateThread(m_hThread, FALSE);
		CloseHandle(m_hThread);
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	initialize
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxThread::Initialize(ExecuteFunc fExe, void* pvArg, hj_size_t sizeStack)
{
	if(!clsHJThread::Initialize(fExe, pvArg, sizeStack)) return false;

	DWORD dwParam;
	m_hThread = CreateThread(NULL, (SIZE_T)sizeStack, &ThreadFunc, this, CREATE_SUSPENDED, &dwParam);

	// スレッドのプライオリティ設定
	if( !SetThreadPriority( GetThreadPrio() ) )
	{
		HJ_ASSERT_MSG( HJ_T("スレッドプライオリティの設定に失敗しました。") );
		return false;
	}
	
	return true;
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	suspend
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxThread::Suspend()
{
	return SuspendThread(m_hThread) != -1;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	resume
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxThread::Resume()
{
	return ResumeThread(m_hThread) != -1;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	setting priority
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxThread::SetThreadPriority(enmPriority ePrio)
{
	return ::SetThreadPriority(m_hThread, atos32ThreadPrio[ePrio])!=0;
}

}// namespace hj