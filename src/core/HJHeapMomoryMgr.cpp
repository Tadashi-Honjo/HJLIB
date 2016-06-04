//--------------------------------------------------------------
/*!	@file
	@brief	ヒープメモリ管理
			
	@author 本城 直志
	@date	12/03/25
*/
//	Copyright (C) 2012 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

// ===== ヒープメモリブロック
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJHeapMemoryBlock::clsHJHeapMemoryBlock(hj_u32 u32Size)
{
	SetMemorySize(u32Size);
	SetUse(false);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJHeapMemoryBlock::~clsHJHeapMemoryBlock()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マージ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJHeapMemoryBlock::Marge()
{
	// 右ブロックを取得
	clsHJHeapMemoryBlock *pcNextBlock = GetNext();
	// タグを変更
	hj_u32 u32NewSize = GetMemorySize() + GetEndTagSize() + GetHeadTagSize() + pcNextBlock->GetMemorySize();
	SetMemorySize(u32NewSize);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ブロックを分割
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJHeapMemoryBlock *clsHJHeapMemoryBlock::Split(hj_u32 u32Size)
{
	// 新規ブロックを作るサイズが無ければNULL
	hj_u32 u32NeedSize = GetHeadTagSize() + u32Size + GetEndTagSize();
	if (u32NeedSize > GetMemorySize()) return NULL;
	
	// 新規ブロックのメモリサイズを算出
	hj_u32 u32NewBlockMemSize = GetMemorySize() - u32NeedSize;
	
	// 既存サイズを引数サイズに縮小
	SetMemorySize(u32Size);
	
	// 新規ブロックを作成
	clsHJHeapMemoryBlock *pcNewBlock = GetNext();
	new (pcNewBlock) clsHJHeapMemoryBlock(u32NewBlockMemSize);
	
	return pcNewBlock;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	指定サイズに分割可能か
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsHJHeapMemoryBlock::IsSplit(hj_u32 u32Size)const
{
	// 新規ブロックを作るサイズが無ければNULL
	hj_u32 u32NeedSize = GetHeadTagSize() + u32Size + GetEndTagSize();
	return (u32NeedSize <= GetMemorySize());
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	前のブロックへのポインタを取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJHeapMemoryBlock* clsHJHeapMemoryBlock::GetPrev()const
{
	hj_u32 *pu32PreSize = (hj_u32*)((hj_u8*)this - GetEndTagSize()); 
	return (clsHJHeapMemoryBlock*)((hj_u8*)this - *pu32PreSize);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	後端タグを書き込み
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJHeapMemoryBlock::WriteEndTag()
{
//	new((hj_u8*)this + GetBlockSize() - GetEndTagSize()) ENDTAG(GetBlockSize());
	stcTag* psTag = (stcTag*)((hj_u8*)this + GetBlockSize() - GetEndTagSize());
	System::Memcpy(psTag, &m_sHeadTag, m_tou32TagSize);	// ヘッダー丸コピ
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	メモリ管理サイズ設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJHeapMemoryBlock::SetMemorySize(hj_u32 u32Size)
{
	m_sHeadTag.u32MemorySize = u32Size;
	WriteEndTag();
}

// ========== clsHJHeapMemoryMgr
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJHeapMemoryMgr::clsHJHeapMemoryMgr(hj_u32 u32Size):
m_pcMemoryBlockAll(NULL)
, m_u32PoolSize(u32Size)
, m_apu8MemoryPool(NULL)
, m_apu8MemoryPoolEnd(NULL)
{
	m_apu8MemoryPool = new hj_u8[m_u32PoolSize];
	m_apu8MemoryPoolEnd = m_apu8MemoryPool + u32Size;
	
	// 始めに最大サイズのブロックを作成しておく
	m_pcMemoryBlockAll = new(m_apu8MemoryPool) clsHJHeapMemoryBlock(m_u32PoolSize);
	m_pcMemoryBlockAll->SetUse(false);	// 未使用領域
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJHeapMemoryMgr::~clsHJHeapMemoryMgr()
{
	delete[] m_apu8MemoryPool;	// 開放はメモリー領域のみ
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	アロケート
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_u8* clsHJHeapMemoryMgr::MemoryAllocate(hj_u32 u32Size)
{
	clsHJHeapMemoryBlock* pcMemoryBlock = m_pcMemoryBlockAll;
	do{
		// 分割出来るなら分割して渡す
		if( !pcMemoryBlock->IsUse() && pcMemoryBlock->IsSplit(u32Size) ){
			pcMemoryBlock->Split(u32Size);
			pcMemoryBlock->SetUse(true);
			return pcMemoryBlock->GetMemory();
//			return (void*)pcMemoryBlock->Split(u32Size);
		}
		pcMemoryBlock = pcMemoryBlock->GetNext();
	}while( (hj_u8*)pcMemoryBlock < m_apu8MemoryPoolEnd );

	HJ_ASSERT_MSG( HJ_T("Heap Memory Not Enough") );
	return NULL;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	削除
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJHeapMemoryMgr::MemoryDelete(void* pvMemory)
{
//	delete(pu8Memory);

	// ブロックに変換する
	clsHJHeapMemoryBlock* pcMemoryBlock = (clsHJHeapMemoryBlock*)((hj_u8*)pvMemory - clsHJHeapMemoryBlock::GetHeadTagSize());
	pcMemoryBlock->SetUse(false);	// 無効領域にする
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	削除
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJHeapMemoryMgr::DebugDraw(hj_f32 f32DrawPosX, hj_f32 f32DrawPosY, hj_f32 f32DrawWidth, hj_f32 f32DrawHeight)
{
	clsHJDraw2d&			rcDraw2d	= *clsHJCoreMgr::RS().GetDraw2d();
	clsHJDrawPrimitive&		rcDrawPrim	= *clsHJCoreMgr::RS().GetDrawPrimitive();

	clsHJHeapMemoryBlock* pcMemoryBlock = m_pcMemoryBlockAll;
	hj_f32 f32MemoryBlockWidth = 0.0f;
	hj_color32 c32Color = HJ_WHITE;

	// 複数描画するのでbeginしとく
	hj_f32 f32MemoryPoolRatioParam = 1.0f / static_cast<hj_f32>(m_u32PoolSize);	// 乗算用に計算しとく
	hj_bool bBegin = !rcDraw2d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック
	if(bBegin)rcDraw2d.Begin();

	do{
		if( !pcMemoryBlock->IsUse() ){
			c32Color = HJ_DIMGRAY;
		}else{
			c32Color = HJ_WHITE;
		}
		f32MemoryBlockWidth = (static_cast<hj_f32>(pcMemoryBlock->GetBlockSize()) * f32MemoryPoolRatioParam) * f32DrawWidth;
		rcDrawPrim.DrawRect(f32DrawPosX, f32DrawPosY, f32MemoryBlockWidth, f32DrawHeight, c32Color);
		f32DrawPosX += f32MemoryBlockWidth;
		pcMemoryBlock = pcMemoryBlock->GetNext();
	}while( (hj_u8*)pcMemoryBlock < m_apu8MemoryPoolEnd );

	if(bBegin)rcDraw2d.End();
}
	
} //namespace hj
