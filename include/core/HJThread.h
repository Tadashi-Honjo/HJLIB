//--------------------------------------------------------------
/*!	@file
	@brief	HJスレッド
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

#ifndef __HJ_THREAD_H_
#define __HJ_THREAD_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	HJThreadクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJThread
{
protected:
	static const hj_size_t m_tosizeDefaultStack = HJ_MB(1);
public:
	typedef void (*ExecuteFunc)(void* pvArg);

	clsHJThread();
	virtual ~clsHJThread();

	// --- Enum
	enum enmPriority{
		PRIO_HIGHEST,		// 基準 +2
		PRIO_ABOVE_NORMAL,	// 基準 +1
		PRIO_NORMAL,		// 基準
		PRIO_BELOW_NORMAL,	// 基準 -1
		PRIO_LOWEST,		// 基準 -2

		PRIO_MAXNUM,
	};
	
	// --- Method
	virtual hj_bool Initialize(ExecuteFunc fExe, void* pvArg, hj_size_t sizeStack=m_tosizeDefaultStack);
	virtual hj_bool Suspend()=0;
	virtual hj_bool Resume()=0;

	// --- Get
	hj_size_t		GetStackSize()const		{ return m_sizeStack;	}
	enmPriority		GetThreadPrio()const	{ return m_ePrio;		}

	// --- Set
	virtual hj_bool	SetThreadPriority(enmPriority ePrio)=0;

public:
	void _ExecuteThread(); // 外部のコールバックから呼び出し用(システム以外使用禁止)

private:
	ExecuteFunc m_fExe;
	void*		m_pvArg;
	hj_size_t	m_sizeStack;
	enmPriority	m_ePrio;
};

}// namespace hj

#endif // __HJ_THREAD_H_