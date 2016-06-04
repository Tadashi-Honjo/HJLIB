//--------------------------------------------------------------
/*!	@file
	@brief	New
			
	@author 本城 直志
	@date	12/03/31
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_NEW_H_
#define __HJ_NEW_H_

namespace hj{

/// デリートマクロ
#define HJ_SAFE_DELETE_ARRAY(p)			{if(p){delete[] p; p=NULL;}};
#define HJ_SAFE_DELETE(p)				{if(p){delete p; p=NULL;}};
#define HJ_SAFE_DELETE_HEAP(p)			{if(p){delete((void*)p, clsHJCoreMgr::GS()->GetMainHeap()); p=NULL;}};
#define HJ_SAFE_DELETE_ARRAY_HEAP(p)	{if(p){delete[]((void*)p, clsHJCoreMgr::GS()->GetMainHeap()); p=NULL;}};

// メモリーリークチェック
#define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define NEW_HEAP new( clsHJCoreMgr::GS()->GetMainHeap() )
#define NEW_HEAP(pcHeap) new(pcHeap) )

#if 0
// アロケータ用
void* operator new(size_t size, clsHJHeapMemoryMgr* pcHeap){
//	OR(HJ_T("AllocSize : %d"), size);
	if(pcHeap){
		return pcHeap->memoryAllocate(size);
	}else{
		return malloc(size);
	}
}
void operator delete(void* pv, clsHJHeapMemoryMgr* pcHeap){
	if(pcHeap){
		delete(pv, (void*)0);
		pcHeap->memoryDelete(pv);
	}else{
		free(pv);
	}
}

// 配列版
void* operator new[](size_t iSize, clsHJHeapMemoryMgr* pcHeap){
	if(pcHeap){
		return pcHeap->memoryAllocate(iSize);
	}else{
	    return malloc(iSize);
	}
}
void operator delete[](void* pv, clsHJHeapMemoryMgr* pcHeap){
	if(pcHeap){
		pcHeap->memoryDelete(pv);
	}else{
		free(pv);
	}
}
#endif 
	
// 配列
#define HJ_ARRAY_NUM(a)	(sizeof(a)/sizeof(a[0]))

}// namespace hj


#endif // __HJ_NEW_H_