//--------------------------------------------------------------
/*!	@file
	@brief	Main
			
	@author 本城 直志
	@date	11/05/24
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_MAIN_H_
#define __HJ_LIB_MAIN_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Main
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJMain
{
public:
	clsHJMain();
	virtual ~clsHJMain();
	
	virtual hj_bool	Initialize(const stcPfInitParam& orsInitParam)=0;	//!< 初期化
	virtual hj_s32	StartLoop()=0;										//!< ループ開始
	virtual void	DeleteInstance()=0;									//!< 終了時のインスタンス開放(デストラクタではHeap開放できません)
	virtual void	EndLoop(){m_bLoopEnd=true;}							//!< ループ終了
	virtual	void	Update()=0;											//!< ユーザー用更新
	virtual	void	Render()=0;											//!< ユーザー用描画

protected:
	hj_bool m_bLoopEnd;
};
	
}// namespace hj

#endif // __HJ_LIB_CORE_H_