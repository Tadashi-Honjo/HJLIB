//--------------------------------------------------------------
/*!	@file
	@brief	HJEntity
			
	@author 本城 直志
	@date	12/08/15
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace {
}

// =============================== clsCharacter
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJEntity::clsHJEntity():m_sPos(0.f, 0.f, 0.f)
, m_sRotation(0.f, 0.f, 0.f)
, m_sScale(1.f, 1.f, 1.f)
, m_pcParent(NULL)
, m_pcChild(NULL)
, m_pcSibling(NULL)
, m_pcPrevSibling(NULL)
, m_sTransformMtx()
, m_sWorldTransformMtx()
, m_bUpdateMtx(true)
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJEntity::~clsHJEntity()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マトリクス更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJEntity::UpdateMatrix()
{
	if(!m_bUpdateMtx) return;

	UpdateLocalMatrix();

	// 一番親からマトリクス更新
	clsHJEntity* pcRootParent = GetRootParent();
	pcRootParent->UpdateMatrixHierarchy();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	親を設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJEntity::AttachParent(clsHJEntity& rcParent)
{
	// すでに親がいる場合解除
	if(IsParent()){
		RemoveParent();
	}

	rcParent.AddChild(this);
	m_pcParent = &rcParent;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	親を解除
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJEntity::RemoveParent()
{
	if( !IsParent() ){
		return;
	}

	// 前に兄弟が設定されていたら自分の兄弟を設定しとく
	if(m_pcPrevSibling != NULL){
		m_pcPrevSibling->SetSibling(m_pcSibling);

	// 自分がChildとして登録していたら兄弟をChildにしとく
	}else{
		if(m_pcSibling != NULL){
			m_pcSibling->SetPrevSibling(NULL);
		}
		m_pcParent->SetChild(m_pcSibling);
	}

	m_pcParent = NULL;
	m_pcSibling = NULL;
	m_pcPrevSibling = NULL;
}



//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ローカルマトリクス更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJEntity::UpdateLocalMatrix()
{
	if( !m_bUpdateMtx ) return;

	m_bUpdateMtx = false;
	m_sTransformMtx.Identity();
	m_sTransformMtx.Scale( m_sScale );					// スケール
	m_sTransformMtx.RotationXyz(m_sRotation);			// 回転
	m_sTransformMtx.Translate( m_sPos );				// 位置
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	階層マトリクスの更新用
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJEntity::UpdateMatrixHierarchy(const stcMatrix44* opsMtx)
{
	// ローカルマトリクス更新
	UpdateLocalMatrix();

	if(opsMtx != NULL){
		stcMatrix44::Multiply( &m_sWorldTransformMtx, m_sTransformMtx, *opsMtx);
	}else{
		stcMatrix44::Copy( &m_sWorldTransformMtx, m_sTransformMtx);
	}

	// 兄弟
	if(m_pcSibling != NULL){
		m_pcSibling->UpdateMatrixHierarchy( &m_sWorldTransformMtx );
	}
	// 子供
	if(m_pcChild != NULL){
		m_pcChild->UpdateMatrixHierarchy( &m_sWorldTransformMtx );
	}
}

// --- 内部用
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	子を追加
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJEntity::AddChild(clsHJEntity* pcEntity)
{
	if(m_pcChild != NULL){
		m_pcChild->AddSibling(this, pcEntity);
	}else{
		m_pcChild = pcEntity;
	}
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	兄弟を追加
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJEntity::AddSibling(clsHJEntity* pcPrevSibling, clsHJEntity* pcEntity)
{
	if(m_pcSibling != NULL){
		m_pcSibling->AddSibling(this, pcEntity);
	}else{
		m_pcPrevSibling = pcPrevSibling;
		m_pcSibling = pcEntity;
	}
}

}// namespace hj