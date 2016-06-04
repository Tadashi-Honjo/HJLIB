//--------------------------------------------------------------
/*!	@file
	@brief	入力デバイス総合クラス
			
	@author 本城 直志
	@date	12/02/11
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HjLib namespace
namespace hj{

	namespace{
		static const hj_f32 tof32AnalogThreshold = 0.05f;
	}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJInput::clsHJInput():
m_u32RepTime(7)
, m_u32RepWaitTime(30)
, m_u32PadMaxNum(0)
, m_psPadData(NULL)
{
	stcPushData* psPushData = NULL;

	// キーチェック
	for(hj_u32 u32i=0; u32i<KEY_MAXNUM; ++u32i){
		psPushData = &m_sKeyPushData[u32i];
		psPushData->u32PushCnt	= 0;
		psPushData->bPush		= false;
		psPushData->bPush_old	= false;
		psPushData->bRepCntMode = false;
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJInput::~clsHJInput()
{
	HJ_SAFE_DELETE_ARRAY(m_psPadData);
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJInput::Init(hj_u32 u32PadMaxNum)
{
	m_u32PadMaxNum = u32PadMaxNum;
	if(m_u32PadMaxNum > 0){
		m_psPadData = NEW stcPadData[m_u32PadMaxNum];
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJInput::Update()
{
	stcPushData* psPushData = NULL;

	// キーチェック
	for(hj_u32 u32i=0; u32i<KEY_MAXNUM; ++u32i){
		PushDataUpdate( &m_sKeyPushData[u32i], CheckKeyPush(static_cast<enmKey>(u32i)) );
	}

	// パッド更新
	stcPadData* psPadData = NULL;
	for(hj_u32 u32i=0; u32i<m_u32PadMaxNum; ++u32i){
		if(IsActivePad(u32i)){
			psPadData = &m_psPadData[u32i];
			// ボタン
			for(hj_u32 u32j=0; u32j<PAD_BOTTON_MAXNUM; ++u32j){
				PushDataUpdate( &psPadData->asPushData[u32j], CheckPadPush( static_cast<enmPad>(u32j), u32i) );
			}
		}
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	アクティブベタ押し(キー)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsHJInput::IsActiveBeta(enmPad ePad)const
{
	for(hj_u32 u32i=0; u32i<m_u32PadMaxNum; ++u32i){
		if(IsActivePad(u32i) && IsBeta(ePad, u32i)){
			return true;
		}
	}
	return false;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	アクティブトリガー(キー)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsHJInput::IsActiveTrig(enmPad ePad)const
{
	for(hj_u32 u32i=0; u32i<m_u32PadMaxNum; ++u32i){
		if(IsActivePad(u32i) && IsTrig(ePad, u32i)){
			return true;
		}
	}
	return false;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	アクティブリリース(キー)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsHJInput::IsActiveRel(enmPad ePad)const
{
	for(hj_u32 u32i=0; u32i<m_u32PadMaxNum; ++u32i){
		if(IsActivePad(u32i) && IsRel(ePad, u32i)){
			return true;
		}
	}
	return false;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	アクティブ繰り返し(キー)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsHJInput::IsActiveRep(enmPad ePad)const
{
	for(hj_u32 u32i=0; u32i<m_u32PadMaxNum; ++u32i){
		if(IsActivePad(u32i) && IsRep(ePad, u32i)){
			return true;
		}
	}
	return false;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	パッドのアナログ値を取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 clsHJInput::GetPadAnalog(enmPad ePad, hj_u32 u32PadIdx)const
{
	hj_f32 f32Analog = GetAnalog(ePad, u32PadIdx);
	if( clsHJMath::Abs( f32Analog ) >= tof32AnalogThreshold)  return f32Analog;
	return 0.f;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	アクティブパッドのアナログ値取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 clsHJInput::GetActivePadAnalog(enmPad ePad)const
{
	for(hj_u32 u32i=0; u32i<m_u32PadMaxNum; ++u32i){
		if(IsActivePad(u32i)){
			hj_f32 f32Analog = GetPadAnalog(ePad, u32i);
			if( clsHJMath::Abs( f32Analog ) >= tof32AnalogThreshold){
				return f32Analog;
			}
		}
	}
	return 0.f;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プッシュデータ設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJInput::PushDataUpdate(stcPushData* psPushData, hj_bool bPush){
	psPushData->bPush_old = psPushData->bPush;

	if(bPush){
		psPushData->bPush = true; 
		psPushData->u32PushCnt += 1;

		// リピート処理
		if(!psPushData->bRepCntMode){
			if(psPushData->u32PushCnt >= m_u32RepWaitTime){
				psPushData->bRepCntMode = true;
				psPushData->u32PushCnt = 0;
			}
		}else{
			if(psPushData->u32PushCnt >= m_u32RepTime){
				psPushData->u32PushCnt = 0;
			}
		}
	}else{
		psPushData->bPush = false;
		psPushData->u32PushCnt = 0;
		psPushData->bRepCntMode = false;
	}
}

} //namespace hj