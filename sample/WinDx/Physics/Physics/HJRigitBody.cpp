//���������������������������������������������������������� 
/*!	@file	HJRigitBody.cpp
	@brief	HJ�����G���W�� ���W�b�h�{�f�B
			
	@author �{�� ���u
	@date	14/03/31
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//���������������������������������������������������������� 

// --- Include
#include "HJPhysicsDef.h"
#include "HJRigitBody.h"

namespace hj{
namespace Physics{

//==============================
// clsRigitBody
//==============================
//���������������������������������������������������������� 
/*!	@brief	�R���X�g���N�^
	@author �{��
*///���������������������������������������������������������� 
clsRigitBody::clsRigitBody():
m_bEnable(true)
, m_sPosition(0.f, 0.f, 0.f)
, m_sRotation(0.f, 0.f, 0.f)
, m_sScale(1.f, 1.f, 1.f)
, m_sSpeed(0.f, 0.f, 0.f)
, m_sGravity(0.f, 0.f, 0.f)
, m_f32DumpingSpeed(0.9f)
// friend
, m_pcRegistWorld(NULL)
, m_s32RegistIndex(-1)
{
}
//���������������������������������������������������������� 
/*!	@brief	�f�X�g���N�^
	@author �{��
*///���������������������������������������������������������� 
clsRigitBody::~clsRigitBody()
{
}

//���������������������������������������������������������� 
/*!	@brief	�O�͔��f
	@author �{��
*///���������������������������������������������������������� 
hj_bool clsRigitBody::ApplyForce()
{
	if( IsEnable() ){
		return false;
	}

	// �d��
	const hj::stcVector3 orsGravity = (m_pcRegistWorld != NULL) ? m_pcRegistWorld->GetGravity() : m_sGravity;
	m_sSpeed += orsGravity;

	// ���x����
	m_sSpeed *= m_f32DumpingSpeed;
	
	// �ʒu
	m_sPosition += m_sSpeed;

	return true;
}


//���������������������������������������������������������� 
/*!	@brief	���[���h����o�^����
	@author �{��
*///���������������������������������������������������������� 
hj_bool clsRigitBody::UnregistWorld()
{
	if( !IsRegistWorld() ){
		return false;
	}
	return m_pcRegistWorld->RegistRigitBody( this );
}

//==============================
// clsRigitBody
// clsRigitBody -> clsRigitBodyMesh
//==============================
//���������������������������������������������������������� 
/*!	@brief	�R���X�g���N�^
	@author �{��
*///���������������������������������������������������������� 
clsRigitBodyMesh::clsRigitBodyMesh(hj_u32 u32VertexMaxNum, hj_u32 u32SideMaxNum)
:clsSuper()
, m_u32VertexMaxNum(u32VertexMaxNum)
, m_u32SideMaxNum(u32SideMaxNum)
, m_vecVertexData()
, m_vecSideData()
{
	m_vecVertexData.reserve(u32VertexMaxNum);
	m_vecSideData.reserve(u32SideMaxNum);
}
//���������������������������������������������������������� 
/*!	@brief	�f�X�g���N�^
	@author �{��
*///���������������������������������������������������������� 
clsRigitBodyMesh::~clsRigitBodyMesh()
{
}

//���������������������������������������������������������� 
/*!	@brief	���_�f�[�^�ǉ�
	@author �{��
*///�������������������������������������������������������� 
hj_bool clsRigitBodyMesh::AddVertexData(const hj::stcVector3& orsPos)
{
	if( m_vecVertexData.size() >= m_u32VertexMaxNum ){
		return false;
	}

	stcVertexData sVertexData;
	sVertexData.sPos = orsPos;
	m_vecVertexData.push_back(sVertexData);

	return true;
}
//���������������������������������������������������������� 
/*!	@brief	�Ӄf�[�^�ǉ�
	@author �{��
*///��������������������������������������������������������
hj_bool clsRigitBodyMesh::AddSideData(hj_u32 u32Index1, hj_u32 u32Index2)
{
	if(m_vecVertexData.size() >= m_u32VertexMaxNum) return false;

	stcSideData sSideData;
	sSideData.u32Index1 = u32Index1;
	sSideData.u32Index2 = u32Index2;
	m_vecSideData.push_back(sSideData);

	return true;
}

} // namespace Physics
} // namespace hj

