//--------------------------------------------------------------
/*!	@file
	@brief	HJEntity
			
	@author 本城 直志
	@date	12/08/15
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_ENTITY_H_
#define __HJ_ENTITY_H_

// HJLib namespace
namespace hj{
	
//==================================================
/*!	@brief	キャラクター
			
			・例:階層の構造
			Parent ---	Child
						  |
						Sibling --- Child --- Child
						  |			  |
						Sibling		Sibling
	
	@author 本城 直志
*///==================================================
class clsHJEntity
{
public:
	clsHJEntity();
	virtual ~clsHJEntity();

	// --- Method
	virtual void UpdateMatrix();							//!< ローカルマトリクス更新
	virtual void AttachParent(clsHJEntity& rcParent);		//!< 親を設定
	virtual void RemoveParent();							//!< 親を解除
	
	// --- Get
	virtual const stcVector3* GetPosition()const	{return &m_sPos;};					//!< 位置取得
	virtual const stcVector3* GetRotation()const	{return &m_sRotation;};				//!< 回転取得
	virtual const stcVector3* GetScale()const		{return &m_sScale;};				//!< スケール取得
	virtual const stcMatrix44* GetLocalMatrix()const{return &m_sTransformMtx;}			//!< ローカルマトリクス(親含まない)
	virtual const stcMatrix44* GetWorldMatrix()const{return &m_sWorldTransformMtx;}		//!< ワールドマトリクス
	
	// --- Set
	virtual void SetPosition(const stcVector3& rsPos){m_sPos=rsPos; m_bUpdateMtx=true;};
	virtual void SetRotation(const stcVector3& rsRot){m_sRotation=rsRot; m_bUpdateMtx=true;};
	virtual void SetScale(const stcVector3& rsScale){m_sScale=rsScale; m_bUpdateMtx=true;};
	
	// --- Is
	hj_bool	IsParent()const{return m_pcParent != NULL;}

	// --- 内部用(外部からの使用禁止)
	void SetChild(clsHJEntity* pcEntity){m_pcChild=pcEntity;};
	void SetSibling(clsHJEntity* pcEntity){m_pcSibling=pcEntity;};	
	void SetPrevSibling(clsHJEntity* pcEntity){m_pcPrevSibling=pcEntity;};
	void AddChild(clsHJEntity* pcEntity);
	void AddSibling(clsHJEntity* pcPrevSibling, clsHJEntity* pcEntity);
	
private:
	// --- Method
	virtual void UpdateLocalMatrix();
	void UpdateMatrixHierarchy(const stcMatrix44* opsMtx=NULL);	 //!< 階層マトリクスの更新用

	//! 大元の親を取得する
	clsHJEntity* GetRootParent(){ if(m_pcParent != NULL){ return m_pcParent->GetRootParent(); }else{ return this; } };	

	// --- Variable
	stcVector3		m_sPos;					//!< 位置
	stcVector3		m_sRotation;			//!< 回転
	stcVector3		m_sScale;				//!< スケール
	clsHJEntity*	m_pcParent;				//!< 親
	clsHJEntity*	m_pcChild;				//!< 子
	clsHJEntity*	m_pcSibling;			//!< 兄弟
	clsHJEntity*	m_pcPrevSibling;		//!< 前の兄弟
	stcMatrix44		m_sTransformMtx;		//!< 変換マトリクス(ローカルマトリクス)
	stcMatrix44		m_sWorldTransformMtx;	//!< 親を含むワールド変換マトリクス
	hj_bool			m_bUpdateMtx;			//!< マトリクス更新チェック
};
	
}// namespace hj

#endif // __HJ_ENTITY_H_