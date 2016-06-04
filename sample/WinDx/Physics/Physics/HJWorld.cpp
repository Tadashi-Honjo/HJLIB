//���������������������������������������������������������� 
/*!	@file	HJWorld.cpp
	@brief	HJ�����G���W�� ���[���h
			
	@author �{�� ���u
	@date	14/03/31
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//���������������������������������������������������������� 

#include "HJPhysicsDef.h"
#include "HJWorld.h"
#include "HJRigitBody.h"

namespace hj{
namespace Physics{

// =========== clsWorld
const hj_f32 clsWorld::m_tof32DefaultGravityAccel = 9.80665f; // �d�͉����x
//���������������������������������������������������������� 
/*!	@brief	�R���X�g���N�^
	@author �{��
*///��������������������������������������������������������
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
//���������������������������������������������������������� 
/*!	@brief	�f�X�g���N�^
	@author �{��
*///��������������������������������������������������������
clsWorld::~clsWorld()
{
}

//���������������������������������������������������������� 
/*!	@brief	������
	@author �{��
*///��������������������������������������������������������
void clsWorld::Initialize(hj_s32 s32Fps)
{
	SetFps(s32Fps);
}

//���������������������������������������������������������� 
/*!	@brief	�X�V
	@author �{��
*///��������������������������������������������������������
void clsWorld::Update()
{
	// === �O�͂̔��f
	// �d�͂�Q�[������^������͂ƃg���N�i��]����́j���O�͂Ƃ��ĉ����x�ɕϊ����A���̂֗^����B


	// === �u���[�h�t�F�[�Y
	// �Փ˂���\���̂Ȃ����̂��Ȃ�


	// === �i���[�t�F�[�Y
	// �Փ˂���\���̂�����̂��ڍׂȏՓˏ��̎Z�o���s��


	// === �S���\���o�[
	// �Փˌ��o����Z�o���ꂽ�Փˏ������ɏՓ˂��Ă��鍄�̂𔽔������邽�߂̌v�Z���s��


	// === �ʒu�X�V(�C���e�O���[�g)
	// ���̂̑��x��ϕ����邱�ƂŃ^�C���X�e�b�v��̈ʒu���Z�o����B
	// �����̖@��������i���x�Ɗp���x�͂��̂܂܈ێ������
}

//���������������������������������������������������������� 
/*!	@brief	�f�o�b�O�`��
	@author �{��
*///��������������������������������������������������������
void clsWorld::Render_debug()
{
}

//���������������������������������������������������������� 
/*!	@brief	���W�b�g�{�f�B�o�^
	@author �{��
*///��������������������������������������������������������
hj_bool clsWorld::RegistRigitBody(clsRigitBody* pcRigitBody)
{
	if( !pcRigitBody ){
		return false;
	}
	// �o�^�ς�
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

//���������������������������������������������������������� 
/*!	@brief	���W�b�g�{�f�B�ǉ�
	@author �{��
*///��������������������������������������������������������
hj_bool clsWorld::UnregistRigitBody(clsRigitBody* pcRigitBody)
{
	if( !pcRigitBody ){
		return false;
	}
	// �o�^���ĂȂ�
	if( !pcRigitBody->IsRegistWorld() ){
		return false;
	}
	// ���[���h���Ⴄ
	if( pcRigitBody->GetRegistWorld() != this){
		return false;
	}

	hj_s32 s32RegistIndex = pcRigitBody->GetRegistIndex();
	m_vecRigitBody[s32RegistIndex] = NULL;
	pcRigitBody->SetRegistWorld(NULL, -1);
	return true;
}

//���������������������������������������������������������� 
/*!	@brief	�d�͎���̃p�����[�^�X�V
	@author �{��
*///��������������������������������������������������������
void clsWorld::UpdateGravityParam()
{
	m_f32GravityAccelFrame = m_f32GravityAccel / static_cast<hj_f32>(m_s32Fps);
	m_sGravity.Set(0.f, -m_f32GravityAccelFrame, 0.f);
}

} // namespace Physics
} // namespace hj

