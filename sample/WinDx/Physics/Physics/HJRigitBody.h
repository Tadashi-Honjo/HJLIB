//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ 
/*!	@file	HJRigitBody.h
	@brief	HJ¨GW Wbh{fB
			
	@author {ι Όu
	@date	14/03/31
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ 
#ifndef __HJ_PHYSICS_RIGITBODY_H_
#define __HJ_PHYSICS_RIGITBODY_H_

namespace hj{
namespace Physics{

class clsWorld;

//==============================
//! Wbg{fB
//==============================
class clsRigitBody{
public:
	clsRigitBody();
	virtual ~clsRigitBody();

	// --- Method
	virtual hj_bool ApplyForce();		//<! OΝ½f
	virtual hj_bool UnregistWorld();	//<! [h©ηo^π

	// --- Set
	virtual void SetEnable(hj_bool bEnable){m_bEnable=bEnable;};					//<! Lψέθ

	// --- Is
	virtual hj_bool IsEnable()const{return m_bEnable;}

private:
	// --- Variable
	hj_bool			m_bEnable;				//<! Lψ
	hj::stcVector3	m_sPosition;			//<! [JΐW
	hj::stcVector3	m_sRotation;			//<! ρ]
	hj::stcVector3	m_sScale;				//<! XP[
	hj::stcVector3	m_sSpeed;				//<! ¬x
	hj::stcVector3	m_sGravity;				//<! dΝ
	hj_f32			m_f32DumpingSpeed;		//<! ¬xΈl

	// ===== frinedpr
	friend class clsWorld;

	// --- Get
	hj_s32 GetRegistIndex()const	{return m_s32RegistIndex;}	//<! o^CfbNXζΎ
	clsWorld* GetRegistWorld()		{return m_pcRegistWorld;}	//<! o^[hζΎ
	// --- Set
	//! o^[hέθ
	void SetRegistWorld(clsWorld* pcWorld, hj_s32 s32Index){
		m_pcRegistWorld=pcWorld;
		m_s32RegistIndex=s32Index;
	}
	// --- Is
	hj_bool IsRegistWorld()const{return m_pcRegistWorld!=NULL && m_s32RegistIndex!=-1;}	//<! [hΙo^Οέ©

	// --- Variable
	clsWorld*	m_pcRegistWorld;
	hj_s32		m_s32RegistIndex;
};

//==============================
//! Wbg{fBbV
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
	hj_bool AddVertexData(const hj::stcVector3& orsPos);		//!< Έ_f[^ΗΑ
	hj_bool AddSideData(hj_u32 u32Index1, hj_u32 u32Index2);	//!< Σf[^ΗΑ

private:
	// --- Struture
	//! Έ_f[^
	struct stcVertexData{
		hj::stcVector3 sPos;
	};
	//! Σξρ(2d)
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
