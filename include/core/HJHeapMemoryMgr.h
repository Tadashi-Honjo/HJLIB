//--------------------------------------------------------------
/*!	@file
	@brief	ヒープメモリ管理
			
	@author 本城 直志
	@date	12/03/25
*/
//	Copyright (C) 2012 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_HEAP_MEMORY_MGR_
#define __HJ_HEAP_MEMORY_MGR_


// HJLib namespace
namespace hj{
	
// ===== ヒープメモリブロック
class clsHJHeapMemoryBlock
{
public:
	clsHJHeapMemoryBlock(hj_u32 u32Size);
	~clsHJHeapMemoryBlock();
	
	// --- Struct
	//! タグ情報(頭と尻につきます)
	struct stcTag{
		hj_bool bUse;			//!< 使用済み
		hj_u32 u32MemorySize;	//!< 使用メモリサイズ
	};
	
	// --- Method
	void Marge();									//!< 右ブロックをマージ
	clsHJHeapMemoryBlock *Split(hj_u32 u32Size);	//!< ブロックを分割

	// --- Is
	hj_bool IsSplit( hj_u32 u32Size )const;			//!< 指定サイズに分割可能か
	hj_bool IsUse()const{return m_sHeadTag.bUse;}	//!< 使用しているかを設定
	
	// --- Get
	hj_u8* 					GetMemory()const		{ return (hj_u8*)this + GetHeadTagSize();	}							//!< 管理メモリへのポインタを取得
	hj_u32 					GetMemorySize()const	{ return m_sHeadTag.u32MemorySize;}										//!< 管理メモリサイズを取得
	hj_u32 					GetBlockSize()const		{ return GetHeadTagSize() + GetMemorySize() + GetEndTagSize();}			//!< ブロックサイズを取得
	clsHJHeapMemoryBlock*	GetNext()const			{ return (clsHJHeapMemoryBlock*)((hj_u8*)this + GetBlockSize()); }		//!< 次のブロックへのポインタを取得
	clsHJHeapMemoryBlock*	GetPrev()const;																					//!< 前のブロックへのポインタを取得

	static hj_u32			GetHeadTagSize()		{ return m_tou32TagSize; }	//!< 先端タグサイズを取得
	static hj_u32			GetEndTagSize()			{ return m_tou32TagSize; }	//!< 後端タグサイズを取得

	// --- Set
	void SetUse(hj_bool bUse){m_sHeadTag.bUse=bUse; WriteEndTag();}	//!< 使用中か設定

	// --- Static Variable
	static const hj_u32 m_tou32TagSize = sizeof(stcTag);		//!< メモリブロックタグのサイズ

private:
	// --- Method
	void WriteEndTag();						//!< 後端タグを書き込み
	// --- Set
	void SetMemorySize(hj_u32 u32Size);		//!< メモリ管理サイズ設定
	// --- Variable
	stcTag	m_sHeadTag;
};


// ----- Class
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ヒープメモリ管理
//
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJHeapMemoryMgr
{
public:	
	clsHJHeapMemoryMgr(hj_u32 u32Size);
	~clsHJHeapMemoryMgr();
	
	// --- Mehod
	hj_u8*	MemoryAllocate(hj_u32 u32Size);		//!< アロケート
	void	MemoryDelete(void* pvMemory);		//!< 削除
	void	DebugDraw(hj_f32 f32DrawPosX=0.f, hj_f32 f32DrawPosY=0.f, hj_f32 f32DrawWidth=500.f, hj_f32 f32DrawHeight=24.f);	//!< デバッグ表示

private:
	// ----- Variable
	clsHJHeapMemoryBlock* 	m_pcMemoryBlockAll;
	hj_u32					m_u32PoolSize;
	hj_u8*					m_apu8MemoryPool;
	hj_u8*					m_apu8MemoryPoolEnd;	// 終端識別用
	
};


} //namespace hj

#endif // __HJ_HEAP_MEMORY_MGR_