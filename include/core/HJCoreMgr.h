//--------------------------------------------------------------
/*!	@file
	@brief	CoreMgr
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_CORE_H_
#define __HJ_LIB_CORE_H_

// HJLib namespace
namespace hj{

struct stcPfInitParam;

class clsHJCoreMgr : public clsHJSingleton<clsHJCoreMgr>
{
public:
	clsHJCoreMgr();
	virtual ~clsHJCoreMgr();
	
	// --- Method
	virtual void CreateInstance(const stcPfInitParam& orsInitParam)=0;	// インスタンス作成
	virtual void DeleteInstance()=0;									// インスタンス削除
	virtual void DefaultInitialize();									// デフォルト初期化
	
	// --- Get
	SpHJHeapMemoryMgr		GetMainHeap()const		{ return m_spMainHeapMgr;	}	// MainHeap
	SpHJGraphics			GetGraphics()const		{ return m_spGraphics; 		}	// Graphics
	SpHJDraw2d				GetDraw2d()const		{ return m_spDraw2d; 		}	// 2D描画
	SpHJDraw3d				GetDraw3d()const		{ return m_spDraw3d; 		}	// 3D描画
	SpHJDrawPrimitive		GetDrawPrimitive()const	{ return m_spDrawPrimitive; }	// Primitive描画
	SpHJTime				GetTime()const			{ return m_spTime;			}	// タイム
	SpHJFpsMgr				GetFpsMgr()const		{ return m_spFpsMgr;		}	// FPSマネージャ
	SpHJInput				GetInput()const			{ return m_spInput;			}	// Input
	SpHJFileMgr				GetFileMgr()const		{ return m_spFileMgr;		}	// File管理
	
protected:
	// --- Variable
	SpHJHeapMemoryMgr		m_spMainHeapMgr;
	SpHJGraphics			m_spGraphics;
	SpHJDraw2d				m_spDraw2d;
	SpHJDraw3d				m_spDraw3d;
	SpHJDrawPrimitive		m_spDrawPrimitive;
	SpHJTime				m_spTime;
	SpHJFpsMgr				m_spFpsMgr;
	SpHJInput				m_spInput;
	SpHJFileMgr				m_spFileMgr;
};
	
}// namespace hj

#endif // __HJ_LIB_CORE_H_