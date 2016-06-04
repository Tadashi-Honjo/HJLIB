//--------------------------------------------------------------
/*!	@file
	@brief	HJスプライト
			
	@author 本城 直志
	@date	11/10/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJSprite::clsHJSprite()
: m_bCenter(true)
, m_f32PosX(0.0f)
, m_f32PosY(0.0f)
, m_f32Deg(0.0f)
, m_f32ScaleX(1.0f)
, m_f32ScaleY(1.0f)
, m_u32Color(HJ_WHITE)
, m_f32TexUvLeft(0.0f)
, m_f32TexUvTop(0.0f)
, m_f32TexUvRight(1.0f)
, m_f32TexUvBottom(1.0f)
, m_sTransformMtx()
, m_sWorldTransformMtx()
, m_bUpdateMtx(true)
, m_pcParent(NULL)
, m_pcChild(NULL)
, m_pcSibling(NULL)
{
	m_sTransformMtx.Identity();
	m_sWorldTransformMtx.Identity();
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJSprite::~clsHJSprite()
{
	// 自分で消すようにした
//	HJ_SAFE_DELETE(m_pcChild);
//	HJ_SAFE_DELETE(m_pcSibling);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マトリクス更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJSprite::Render()
{
	clsHJDraw2d& rcDraw2d = *clsHJCoreMgr::GS()->GetDraw2d();
	hj_bool bDraw2dBeginEnd = !rcDraw2d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック

	// 最適化の為、階層のはじめで呼ぶ
	if(bDraw2dBeginEnd) rcDraw2d.Begin();
	
	RenderOneNoneBeginEnd();
	// 兄弟
//	if(m_pcSibling != NULL) m_pcSibling->RenderOneNoneBeginEnd();
	if(m_pcSibling != NULL) m_pcSibling->Render();
	
	// 子供
//	if(m_pcChild != NULL) m_pcChild->RenderOneNoneBeginEnd();
	if(m_pcChild != NULL) m_pcChild->Render();
	
	if(bDraw2dBeginEnd) rcDraw2d.End();
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マトリクス更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJSprite::Render(const stcMatrix44& orsMtx)
{
	clsHJDraw2d& rcDraw2d = *clsHJCoreMgr::GS()->GetDraw2d();
	hj_bool bDraw2dBeginEnd = !rcDraw2d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック

	// 最適化の為、階層のはじめで呼ぶ
	if(bDraw2dBeginEnd) rcDraw2d.Begin();
	
	RenderOneNoneBeginEnd(orsMtx);

	// 兄弟
//	if(m_pcSibling != NULL) m_pcSibling->RenderOneNoneBeginEnd(orsMtx);
	if(m_pcSibling != NULL) m_pcSibling->Render(orsMtx);
	// 子供
//	if(m_pcChild != NULL) m_pcChild->RenderOneNoneBeginEnd(orsMtx);
	if(m_pcChild != NULL) m_pcChild->Render(orsMtx);
	
	if(bDraw2dBeginEnd) rcDraw2d.End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マトリクス更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJSprite::UpdateMatrix()
{
	UpdateLocalMatrix();

	// 一番親からマトリクス更新
	clsHJSprite* pcRootParent = GetRootParent();
	pcRootParent->UpdateMatrixHierarchy();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ローカルマトリクス更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJSprite::UpdateLocalMatrix()
{
	if( !m_bUpdateMtx ) return;

	m_bUpdateMtx = false;
	m_sTransformMtx.Identity();
	m_sTransformMtx.Scale( stcVector3(m_f32ScaleX, m_f32ScaleY, 1.0f) );	// スケール
	m_sTransformMtx.RotationZ(m_f32Deg);									// Z回転
	m_sTransformMtx.Translate( stcVector3(m_f32PosX, m_f32PosY, 0.0f) );	// 位置
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	階層マトリクスの更新用
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJSprite::UpdateMatrixHierarchy(const stcMatrix44* opsMtx)
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
		m_pcSibling->UpdateMatrixHierarchy( opsMtx );
	}
	// 子供
	if(m_pcChild != NULL){
		m_pcChild->UpdateMatrixHierarchy( &m_sWorldTransformMtx );
	}
}

}// namespace hj