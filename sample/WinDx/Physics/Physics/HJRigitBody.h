//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@file	HJRigitBody.h
	@brief	HJ物理エンジン リジッドボディ
			
	@author 本城 直志
	@date	14/03/31
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
#ifndef __HJ_PHYSICS_RIGITBODY_H_
#define __HJ_PHYSICS_RIGITBODY_H_

namespace hj{
namespace Physics{

class clsWorld;

//==============================
//! リジットボディ
//==============================
class clsRigitBody{
public:
	clsRigitBody();
	virtual ~clsRigitBody();

	// --- Method
	virtual hj_bool ApplyForce();		//<! 外力反映
	virtual hj_bool UnregistWorld();	//<! ワールドから登録解除

	// --- Set
	virtual void SetEnable(hj_bool bEnable){m_bEnable=bEnable;};					//<! 有効設定

	// --- Is
	virtual hj_bool IsEnable()const{return m_bEnable;}

private:
	// --- Variable
	hj_bool			m_bEnable;				//<! 有効
	hj::stcVector3	m_sPosition;			//<! ローカル座標
	hj::stcVector3	m_sRotation;			//<! 回転
	hj::stcVector3	m_sScale;				//<! スケール
	hj::stcVector3	m_sSpeed;				//<! 速度
	hj::stcVector3	m_sGravity;				//<! 重力
	hj_f32			m_f32DumpingSpeed;		//<! 速度減衰値

	// ===== frined用途
	friend class clsWorld;

	// --- Get
	hj_s32 GetRegistIndex()const	{return m_s32RegistIndex;}	//<! 登録インデックス取得
	clsWorld* GetRegistWorld()		{return m_pcRegistWorld;}	//<! 登録ワールド取得
	// --- Set
	//! 登録ワールド設定
	void SetRegistWorld(clsWorld* pcWorld, hj_s32 s32Index){
		m_pcRegistWorld=pcWorld;
		m_s32RegistIndex=s32Index;
	}
	// --- Is
	hj_bool IsRegistWorld()const{return m_pcRegistWorld!=NULL && m_s32RegistIndex!=-1;}	//<! ワールドに登録済みか

	// --- Variable
	clsWorld*	m_pcRegistWorld;
	hj_s32		m_s32RegistIndex;
};

//==============================
//! リジットボディメッシュ
//==============================
class clsRigitBodyMesh : public clsRigitBody
{
public:
	typedef clsRigitBody clsSuper;

	static const hj_u32 m_tou32DefaultVertexMaxNum = 64;
	static const hj_u32 m_tou32DefaultSideMaxNum = 32;

	clsRigitBodyMesh(hj_u32 u32VertexMaxNum=m_tou32DefaultVertexMaxNum, hj_u32 u32SideMaxNum=m_tou32DefaultSideMaxNum);
	virtual ~clsRigitBodyMesh();

	// --- Method
	hj_bool AddVertexData(const hj::stcVector3& orsPos);		//!< 頂点データ追加
	hj_bool AddSideData(hj_u32 u32Index1, hj_u32 u32Index2);	//!< 辺データ追加

private:
	// --- Struture
	//! 頂点データ
	struct stcVertexData{
		hj::stcVector3 sPos;
	};
	//! 辺情報(2d)
	struct stcSideData{
		hj_u32 u32Index1, u32Index2;
	};

	// --- Variable
	hj_u32						m_u32VertexMaxNum, m_u32SideMaxNum;
	hj_vector<stcVertexData>	m_vecVertexData;
	hj_vector<stcSideData>		m_vecSideData;
};

} // namespace Physics
} // namespace hj

#endif // __HJ_PHYSICS_RIGITBODY_H_
