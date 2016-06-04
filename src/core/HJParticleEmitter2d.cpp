//--------------------------------------------------------------
/*!	@file
	@brief	パーティクルエミッター2d
			
	@author 本城 直志
	@date	12/02/12
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJParticleEmitter2d::clsHJParticleEmitter2d():m_apsParticles(NULL)
, m_u32BuffMaxNum(0)
, m_u32ParticleCnt(0)
, m_bEmit(true)
, m_f32GravityX(0.f), m_f32GravityY(0.f)
, m_f32PosX(0.f), m_f32PosY(0.f)
, m_pcDrawSprite(NULL)
, m_f32CraeteTime(0.f)
, m_f32CraeteTimeCnt(0.f)
, m_c32BaseColor(HJ_WHITE)
, m_f32BaseRotDeg(0.f)
, m_f32BaseScale(1.f)
, m_f32OffsetPosMin(0.f), m_f32OffsetPosMax(0.f)
, m_f32OffsetPosDegMin(0.f), m_f32OffsetPosDegMax(360.f)
, m_f32PosSpeedMax(0.f), m_f32PosSpeedMin(0.f)
, m_f32PosSpeedDegMax(360.f),	m_f32PosSpeedDegMin(0.f)
, m_f32SpeedAccel(0.f)
, m_f32RotDegSpeedMax(0.f), m_f32RotDegSpeedMin(0.f)
, m_f32ScaleSpeedMax(0.f), m_f32ScaleSpeedMin(0.f)
, m_f32AlphaSpeedMax(0.f), m_f32AlphaSpeedMin(0.f)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJParticleEmitter2d::~clsHJParticleEmitter2d()
{
	HJ_SAFE_DELETE(m_apsParticles);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJParticleEmitter2d::Init(hj_u32 u32BuffMaxNum, clsHJSprite* pcDrawSprite)
{
	m_u32BuffMaxNum = u32BuffMaxNum;
	m_apsParticles = NEW stcParticleData2d[u32BuffMaxNum];
	System::Memset(m_apsParticles, 0x00, m_u32BuffMaxNum*sizeof(stcParticleData2d));

	m_pcDrawSprite = pcDrawSprite;
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJParticleEmitter2d::Clear()
{
	m_u32ParticleCnt = 0;
	hj_u32 u32i=0;
	stcParticleData2d* psData = &m_apsParticles[0];
	for(u32i=0; u32i<m_u32BuffMaxNum; ++u32i, ++psData){
		psData->bActive = false;
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJParticleEmitter2d::Update()
{
	if(m_u32BuffMaxNum <= 0 || m_pcDrawSprite == NULL ){
		return;
	}

	// 生成処理
	if(m_bEmit && !IsBufferMax()){
//		m_f32CraeteTimeCnt += hjSystem::GetDeltaTime();
		m_f32CraeteTimeCnt += 1.f;	// 処理落ちで表示が不安定になるのでフレームに変更
		hj_f32 f32Rad=0.f, f32Speed=0.f;
		hj_f32 f32OffsetRad=0.f, f32Offset=0.f;
		while(m_f32CraeteTimeCnt >= m_f32CraeteTime ){
			m_f32CraeteTimeCnt -= m_f32CraeteTime;
			f32Rad = clsHJMath::DegToRad(clsHJMath::Random(m_f32PosSpeedDegMin, m_f32PosSpeedDegMax));
			f32Speed = clsHJMath::Random(m_f32PosSpeedMin, m_f32PosSpeedMax);
			f32OffsetRad = clsHJMath::DegToRad(clsHJMath::Random(m_f32OffsetPosDegMin, m_f32OffsetPosDegMax));
			f32Offset = clsHJMath::Random(m_f32OffsetPosMin, m_f32OffsetPosMax);

			// 追加
			AddParticle(
				(clsHJMath::Cos(f32OffsetRad)*f32Offset) + m_f32PosX,
				(clsHJMath::Sin(f32OffsetRad)*f32Offset) + m_f32PosY,
				clsHJMath::Cos(f32Rad)*f32Speed,
				clsHJMath::Sin(f32Rad)*f32Speed,
				m_f32SpeedAccel,
				clsHJMath::Random(m_f32RotDegSpeedMin, m_f32RotDegSpeedMax),
				clsHJMath::Random(m_f32ScaleSpeedMin, m_f32ScaleSpeedMax),
				clsHJMath::Random(m_f32AlphaSpeedMin, m_f32AlphaSpeedMax)
			);
		}
	}

	// 更新処理
	stcParticleData2d* psData = &m_apsParticles[0];
	hj_u32 u32i=0;
	for(u32i=0; u32i<m_u32BuffMaxNum; ++u32i, ++psData){
		if(psData->bActive){
			// 重力加速度
			psData->f32PosSpeedX += m_f32GravityX;
			psData->f32PosSpeedY += m_f32GravityY;
			// 速度の加速
			psData->f32PosSpeedX += psData->f32PosSpeedAccelX;
			psData->f32PosSpeedY += psData->f32PosSpeedAccelY;
			// 位置
			psData->f32PosX += psData->f32PosSpeedX;
			psData->f32PosY += psData->f32PosSpeedY;
			// 回転
			psData->f32RotDeg += psData->f32RotDegSpeed;
			// スケール
			psData->f32Scale += psData->f32ScaleSpeed;
			// アルファ
			psData->f32Alpha += psData->f32AlphaSpeed;

			// 終了処理
			if(psData->f32Alpha <= 0.f ){
				psData->bActive = false;
				--m_u32ParticleCnt;
			}
		}
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJParticleEmitter2d::Render()
{
	if(m_u32BuffMaxNum <= 0 || m_pcDrawSprite == NULL ){
		return;
	}

	clsHJDraw2d& rcDraw2d = *clsHJCoreMgr::GS()->GetDraw2d();
	stcParticleData2d* psData = &m_apsParticles[0];
	hj_u32 u32i=0;
	hj_bool bBegin = rcDraw2d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック

	// 最適化の為最初と最後だけ呼ぶ
	if(!bBegin) rcDraw2d.Begin();

	// カメラ版
	for(u32i=0; u32i<m_u32BuffMaxNum; ++u32i, ++psData){
		if(psData->bActive){
			m_pcDrawSprite->SetPosition( psData->f32PosX, psData->f32PosY );					// 位置
			m_pcDrawSprite->SetRotation( psData->f32RotDeg );									// 回転
			m_pcDrawSprite->SetScale( psData->f32Scale );										// スケール
//				m_pcDrawSprite->SetColor( HJ_COLOR_ARGB_SET(static_cast<hj_u8>(psData->f32Alpha*255.f), 255, 255, 255) );		// アルファ
			m_pcDrawSprite->SetColor( HJ_COLOR_ARGB_SET(static_cast<hj_u8>(psData->f32Alpha*255.f), HJ_COLOR_R_GET(psData->c32Color), HJ_COLOR_G_GET(psData->c32Color), HJ_COLOR_B_GET(psData->c32Color)) );	// 色
			m_pcDrawSprite->Render();
		}
	}
	if(!bBegin) rcDraw2d.End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	パーティクル追加
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsHJParticleEmitter2d::AddParticle(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32SpeedX, hj_f32 f32SpeedY, hj_f32 f32SpeedAccel, hj_f32 f32RotDegSpeed, hj_f32 f32ScaleSpeed, hj_f32 f32AlphaSpeed)
{
	stcParticleData2d* psData = &m_apsParticles[0];
	hj_u32 u32i=0;
	for(u32i=0; u32i<m_u32BuffMaxNum; ++u32i, ++psData){
		if(!psData->bActive){
			++m_u32ParticleCnt;
			psData->bActive = true;

			// 乗算用にしとく
			hj_f32 f32SpeedLengthParam = 1.0f;
			hj_f32 f32SpeedLength = clsHJMath::Sqrt((psData->f32PosSpeedX * psData->f32PosSpeedX) + (psData->f32PosSpeedX * psData->f32PosSpeedX));
			if(f32SpeedLength>0.f){
				f32SpeedLengthParam = 1.0f / f32SpeedLength;
			}

			psData->f32PosSpeedX = f32SpeedX;
			psData->f32PosSpeedY = f32SpeedY;
			psData->f32PosSpeedAccelX = f32SpeedX * f32SpeedLengthParam * f32SpeedAccel;
			psData->f32PosSpeedAccelY = f32SpeedY * f32SpeedLengthParam * f32SpeedAccel;
			psData->f32PosX = f32PosX;
			psData->f32PosY = f32PosY;
			psData->f32RotDeg = m_f32BaseRotDeg;
			psData->f32RotDegSpeed = f32RotDegSpeed;
			psData->f32Scale = m_f32BaseScale;
			psData->f32ScaleSpeed = f32ScaleSpeed;
			psData->c32Color = m_c32BaseColor;
			psData->f32Alpha = static_cast<hj_f32>(HJ_COLOR_A_GET(m_c32BaseColor)) / 255.f;
//			psData->f32Alpha = 1.0f;
			psData->f32AlphaSpeed = f32AlphaSpeed;
			return true;
		}
	}
	return false;	// バッファに空きがありません
}

}// namespace hj