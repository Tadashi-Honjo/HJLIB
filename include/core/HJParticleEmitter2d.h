//--------------------------------------------------------------
/*!	@file
	@brief	パーティクルエミッター2d
			
	@author 本城 直志
	@date	12/02/12
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_PARTICLE_EMITTER_2D_H_
#define __HJ_PARTICLE_EMITTER_2D_H_

// HJLib namespace
namespace hj{

class clsHJSprite;

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	パーティクルエミッター2d
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJParticleEmitter2d
{
public:
	clsHJParticleEmitter2d();
	virtual ~clsHJParticleEmitter2d();

	// --- Struct
	//! パーティクルデータ
	struct stcParticleData2d{
		hj_bool bActive;
		hj_f32	f32PosX, f32PosY;
		hj_f32	f32RotDeg;
		hj_f32	f32Scale;
		hj_color32 c32Color;
		hj_f32	f32Alpha;
		hj_f32	f32PosSpeedX, f32PosSpeedY;
		hj_f32	f32PosSpeedAccelX, f32PosSpeedAccelY;
		hj_f32	f32SpeedAccel;
		hj_f32	f32RotDegSpeed;
		hj_f32	f32ScaleSpeed;
		hj_f32	f32AlphaSpeed;
	};

	// --- Method
	void Init(hj_u32 u32BuffMaxNum, clsHJSprite* pcDrawSprite);
	void Clear();
	void Update();
	void Render();

	// --- Is
	hj_bool IsEmit()const			{ return m_bEmit; }									//!< 放出中か
	hj_bool IsBufferMax()const		{ return m_u32ParticleCnt >= m_u32BuffMaxNum; }		//!< バッファが一杯か
	hj_bool IsAliveParticle()const	{ return m_u32ParticleCnt>0; }						//!< パーティクルか残っているか

	// --- Set
	//! 放出設定
	void SetEmit(hj_bool bEmit){m_bEmit = bEmit;};
	//! 重力加速度を設定
	void SetGravity(hj_f32 f32GravityX, hj_f32 f32GravityY){m_f32GravityX=f32GravityX; m_f32GravityY=f32GravityY;}
	//! 描画に使用するスプライト設定
	void SetDrawSprite(clsHJSprite* pcDrawSprite){m_pcDrawSprite=pcDrawSprite;}
	//! 生成時間(フレーム)
	void SetCreateTime(hj_f32 f32CraeteTime){m_f32CraeteTime=f32CraeteTime;}
	//! 生成時の色
	void SetBaseColor(hj_color32 c32BaseColor){m_c32BaseColor=c32BaseColor;}
	//! 生成時の回転値
	void SetBaseRotation(hj_f32 f32BaseRotDeg){m_f32BaseRotDeg=f32BaseRotDeg;}
	//! 生成時のスケール値
	void SetBaseScale(hj_f32 f32BaseScale){m_f32BaseScale=f32BaseScale;}

	//! 放出位置を設定　
	void SetPosition(hj_f32 f32PosX, hj_f32 f32PosY){m_f32PosX=f32PosX; m_f32PosY=f32PosY;}
	//! 放出位置からのオフセット範囲設定
	void SetOffsetPosMinMax(hj_f32 f32OffsetPosMin, hj_f32 f32OffsetPosMax, hj_f32 f32OffsetPosDegMin=0.f, hj_f32 f32OffsetPosDegMax=360.f){
			m_f32OffsetPosMin=f32OffsetPosMin; m_f32OffsetPosMax=f32OffsetPosMax;
			m_f32OffsetPosDegMin=f32OffsetPosDegMin; m_f32OffsetPosDegMax=f32OffsetPosDegMax;
	}
	//! パーティクルの初速度範囲の設定
	void SetSpeedMinMax(hj_f32 f32PosSpeedMin, hj_f32 f32PosSpeedMax, hj_f32 f32PosSpeedDegMin=0.f, hj_f32 f32PosSpeedDegMax=360.f){
			m_f32PosSpeedMin=f32PosSpeedMin; m_f32PosSpeedMax=f32PosSpeedMax;
			m_f32PosSpeedDegMin=f32PosSpeedDegMin; m_f32PosSpeedDegMax=f32PosSpeedDegMax;
	}

	//! 速度に対しての加速度
	void SetSpeedAccel(hj_f32 f32SpeedAccel){m_f32SpeedAccel=f32SpeedAccel;};
	//! パーティクルの回転速度範囲の設定
	void SetRotationSpeedMinMax(hj_f32 f32RotDegSpeedMin, hj_f32 f32RotDegSpeedMax){m_f32RotDegSpeedMin=f32RotDegSpeedMin; m_f32RotDegSpeedMax=f32RotDegSpeedMax;}
	//! パーティクルのスケール速度範囲の設定
	void SetScaleSpeedMinMax(hj_f32 f32ScaleSpeedMin, hj_f32 f32ScaleSpeedMax){m_f32ScaleSpeedMin=f32ScaleSpeedMin; m_f32ScaleSpeedMax=f32ScaleSpeedMax;}
	//! パーティクルのアルファ速度範囲の設定
	void SetAlphaSpeedMinMax(hj_f32 f32AlphaSpeedMin, hj_f32 f32AlphaSpeedMax){m_f32AlphaSpeedMin=f32AlphaSpeedMin; m_f32AlphaSpeedMax=f32AlphaSpeedMax;}

private:
	// --- Method
	hj_bool	AddParticle(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32SpeedX, hj_f32 f32SpeedY, hj_f32 f32SpeedAccel, hj_f32 f32RotDegSpeed, hj_f32 f32ScaleSpeed, hj_f32 f32AlphaSpeed );	//!< パーティクル追加

	// --- Variable
	stcParticleData2d*	m_apsParticles;					//!< パーティクルバッファ
	hj_u32				m_u32BuffMaxNum;				//!< パーティクル最大数
	hj_u32				m_u32ParticleCnt;				//!< パーティクル生成数
	hj_bool				m_bEmit;						//!< 放出するか
	hj_f32				m_f32PosX, m_f32PosY;			//!< 発生位置
	hj_f32				m_f32GravityX, m_f32GravityY;	//!< 重力加速度
	clsHJSprite*		m_pcDrawSprite;					//!< 描画スプライト(こいつに設定して描画する)
	hj_f32				m_f32CraeteTime;				//!< 生成時間
	hj_f32				m_f32CraeteTimeCnt;				//!< 生成用カウント
	hj_color32			m_c32BaseColor;					//!< 色
	hj_f32				m_f32BaseRotDeg;				//!< 回転初期値
	hj_f32				m_f32BaseScale;					//!< スケール初期値
	hj_f32				m_f32OffsetPosMin, m_f32OffsetPosMax;
	hj_f32				m_f32OffsetPosDegMin, m_f32OffsetPosDegMax;
	hj_f32				m_f32PosSpeedMax,		m_f32PosSpeedMin;
	hj_f32				m_f32PosSpeedDegMax,	m_f32PosSpeedDegMin;
	hj_f32				m_f32SpeedAccel;
	hj_f32				m_f32RotDegSpeedMax,	m_f32RotDegSpeedMin;
	hj_f32				m_f32ScaleSpeedMax,		m_f32ScaleSpeedMin;
	hj_f32				m_f32AlphaSpeedMax,		m_f32AlphaSpeedMin;
};

}// namespace hj

#endif // __HJ_PARTICLE_EMITTER_2D_H_