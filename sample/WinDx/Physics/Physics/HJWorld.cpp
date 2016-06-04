//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@file	HJWorld.cpp
	@brief	HJ物理エンジン ワールド
			
	@author 本城 直志
	@date	14/03/31
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 

#include "HJPhysicsDef.h"
#include "HJWorld.h"
#include "HJRigitBody.h"

namespace hj{
namespace Physics{

// =========== clsWorld
const hj_f32 clsWorld::m_tof32DefaultGravityAccel = 9.80665f; // 重力加速度
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWorld::clsWorld():
m_vecRigitBody()
, m_vecRigitBodyPair()
, m_s32Fps(60)
, m_f32GravityAccel(m_tof32DefaultGravityAccel)
, m_f32GravityAccelFrame(0.f)
, m_sGravity(0.f,0.f,0.f)
{
	m_vecRigitBody.reserve(m_tos32RigitBodyMaxNum);
	m_vecRigitBodyPair.reserve(m_tos32RigitBodyPairMaxNum);
#if 0
	for(hj_s32 s32i=0; s32i<m_tos32RigitBodyMaxNum; ++s32i){
		m_vecRigitBody[s32i] = NULL;
	}
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWorld::~clsWorld()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWorld::Initialize(hj_s32 s32Fps)
{
	SetFps(s32Fps);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	更新
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWorld::Update()
{
	// === 外力の反映
	// 重力やゲームから与えられる力とトルク（回転する力）を外力として加速度に変換し、剛体へ与える。


	// === ブロードフェーズ
	// 衝突する可能性のないものを省く


	// === ナローフェーズ
	// 衝突する可能性のあるものを詳細な衝突情報の算出を行う


	// === 拘束ソルバー
	// 衝突検出から算出された衝突情報を元に衝突している剛体を反発させるための計算を行う


	// === 位置更新(インテグレート)
	// 剛体の速度を積分することでタイムステップ後の位置を算出する。
	// 完成の法則から並進速度と角速度はそのまま維持される
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デバッグ描画
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWorld::Render_debug()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	リジットボディ登録
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWorld::RegistRigitBody(clsRigitBody* pcRigitBody)
{
	if( !pcRigitBody ){
		return false;
	}
	// 登録済み
	if( pcRigitBody->IsRegistWorld() ){
		return false;
	}

	for(hj_s32 s32i=0; s32i<m_tos32RigitBodyMaxNum; ++s32i){
		if(m_vecRigitBody[s32i] == NULL){
			m_vecRigitBody[s32i] = pcRigitBody;
			pcRigitBody->SetRegistWorld(this, s32i);
			return true;
		}
	}
	return false;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	リジットボディ追加
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWorld::UnregistRigitBody(clsRigitBody* pcRigitBody)
{
	if( !pcRigitBody ){
		return false;
	}
	// 登録してない
	if( !pcRigitBody->IsRegistWorld() ){
		return false;
	}
	// ワールドが違う
	if( pcRigitBody->GetRegistWorld() != this){
		return false;
	}

	hj_s32 s32RegistIndex = pcRigitBody->GetRegistIndex();
	m_vecRigitBody[s32RegistIndex] = NULL;
	pcRigitBody->SetRegistWorld(NULL, -1);
	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	重力周りのパラメータ更新
	@author 本城
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWorld::UpdateGravityParam()
{
	m_f32GravityAccelFrame = m_f32GravityAccel / static_cast<hj_f32>(m_s32Fps);
	m_sGravity.Set(0.f, -m_f32GravityAccelFrame, 0.f);
}

} // namespace Physics
} // namespace hj

