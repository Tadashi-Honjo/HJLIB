//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@file	HJWorld.h
	@brief	HJ物理エンジン ワールド
			
	@author 本城 直志
	@date	14/03/31
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
#ifndef __HJ_PHYSICS_WORLD_H_
#define __HJ_PHYSICS_WORLD_H_

namespace hj{
namespace Physics{

class clsRigitBody;

//! ワールド
class clsWorld
{
public:
	clsWorld();
	virtual ~clsWorld();

	// --- Static Variable
	static const hj_f32 m_tof32DefaultGravityAccel/* = 9.80665f*/; //<! 重力加速度

	// --- Structure
	//! リジットボディペア
	struct stcRigitBodyPair{
		hj_s32 s32Index1, s32Index2;
	};
	
	// --- Method
	virtual void Initialize(hj_s32 s32Fps);
	virtual void Update();
	virtual void Render_debug();
	virtual hj_bool RegistRigitBody(clsRigitBody* pcRigitBody);		//<! リジッドボディ登録
	virtual hj_bool UnregistRigitBody(clsRigitBody* pcRigitBody);	//<! リジッドボディ登録解除

	// --- Get
	hj_s32 GetFps()const					{ return m_s32Fps;}					//<! FPS取得
	hj_f32 GetGravityAccel()const			{ return m_f32GravityAccel;}		//<! 重力加速度取得
	hj_f32 GetGravityAccelFrame()const		{ return m_f32GravityAccelFrame;}	//<! 重力加速度(フレーム)取得
	const hj::stcVector3& GetGravity()const	{ return m_sGravity;}				//<! 重力ベクトル取得

	// --- Set
	void SetFps(hj_s32 s32Fps){ m_s32Fps=s32Fps; UpdateGravityParam(); }										//<! FPS設定
	void SetGravityAccel(hj_f32 f32GravityAceel){ m_f32GravityAccel=f32GravityAceel; UpdateGravityParam(); }	//<! 重力加速度設定

private:
	// --- Method
	void UpdateGravityParam(); //<! 重力周りのパラメータ更新

	// --- Variable
	static const hj_s32 m_tos32RigitBodyMaxNum = 256;				//<! リジッドボディ最大数
	static const hj_s32 m_tos32RigitBodyPairMaxNum = 1024;			//<! リジッドボディペア最大数
	hj_vector<clsRigitBody*> 		m_vecRigitBody;					//<! リジッドボディリスト
	hj_vector<stcRigitBodyPair> 	m_vecRigitBodyPair;				//<! リジッドボディペアリスト
	hj_s32							m_s32Fps;						//<! FPS
	hj_f32							m_f32GravityAccel;				//<! 重力加速度
	hj_f32							m_f32GravityAccelFrame;			//<! 重力加速度(フレーム)
	hj::stcVector3					m_sGravity;						//<! 重力加速度ベクトル
};

} // namespace Physics
} // namespace hj

#endif // __HJ_PHYSICS_WORLD_H_
