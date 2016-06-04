//--------------------------------------------------------------
/*!	@file
	@brief	スプライト
			
	@author 本城 直志
	@date	11/10/18
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_SPRITE_H_
#define __HJ_SPRITE_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Spriteクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJSprite
{
public:
	clsHJSprite();
	virtual ~clsHJSprite();

	// --- Set
	virtual void SetTexture(SpHJTexture spTexture)				{m_spTexture=spTexture;};											//!< テクスチャ設定
	virtual void SetPosition(hj_f32 f32PosX, hj_f32 f32PosY)	{m_bUpdateMtx=true; m_f32PosX=f32PosX; m_f32PosY=f32PosY;};			//!< 位置
	virtual void SetRotation(hj_f32 f32Deg)						{m_bUpdateMtx=true; m_f32Deg=f32Deg;};								//!< 回転
	virtual void SetScale(hj_f32 f32ScaleX, hj_f32 f32ScaleY)	{m_bUpdateMtx=true; m_f32ScaleX=f32ScaleX; m_f32ScaleY=f32ScaleY;};	//!< スケールxy
	virtual void SetScale(hj_f32 f32Scale)						{m_bUpdateMtx=true; m_f32ScaleX=f32Scale; m_f32ScaleY=f32Scale;};	//!< スケール
	virtual void SetColor(hj_color32 u32Color)					{m_u32Color=u32Color;};												//!< 色
	virtual void SetTextureUv(hj_f32 f32TexUvLeft, hj_f32 f32TexUvTop, hj_f32 f32TexUvRight, hj_f32 f32TexUvBottom){m_f32TexUvLeft=f32TexUvLeft; m_f32TexUvTop=f32TexUvTop; m_f32TexUvRight=f32TexUvRight; m_f32TexUvBottom=f32TexUvBottom;};//!< テクスチャUV設定
	virtual void SetCenter(hj_bool bCenter)						{m_bCenter = bCenter;};												//!< センタリング設定
	virtual void SetParentSprite(clsHJSprite& rcParent)			{m_bUpdateMtx=true; m_pcParent=&rcParent; rcParent.SetChildSprite(this);};	//!< 親スプライト設定

	// --- Get
	virtual stcMatrix44* GetMatrix() { UpdateMatrix(); return &m_sTransformMtx; }												//!< ローカルマトリクス取得
	virtual stcMatrix44* GetWorldMatrix() { UpdateMatrix(); return &m_sWorldTransformMtx; }										//!< ワールドマトリクス取得

	// --- Method
	virtual void Render();									//!< レンダリング(子も含む)
	virtual void Render(const stcMatrix44& orsMtx);			//!< 行列変換後のレンダリング(子も含む)
	virtual void UpdateMatrix();							//!< ローカルマトリクス更新

	// 内部連結用
	void SetChildSprite(clsHJSprite* pcChild){ if(m_pcChild!=NULL){m_pcChild->SetSiblingSprite(pcChild);}else{m_pcChild=pcChild;}}					//!< 子を設定
	void SetSiblingSprite(clsHJSprite* pcSibling){ if(m_pcSibling!=NULL){m_pcSibling->SetSiblingSprite(pcSibling);}else{m_pcSibling=pcSibling;}}	//!< 兄弟を設定
	
private:
	void UpdateLocalMatrix();
	void UpdateMatrixHierarchy(const stcMatrix44* opsMtx=NULL);														//!< 階層マトリクスの更新用
	clsHJSprite* GetRootParent(){ if(m_pcParent != NULL){ return m_pcParent->GetRootParent(); }else{ return this; } };	//!< 大元の親を取得する

protected:
	// --- Method
	virtual void RenderOneNoneBeginEnd()=0;										//!< 単一レンダリング(begin~endを行わない、階層などを見ない)
	virtual void RenderOneNoneBeginEnd(const stcMatrix44& orsMtx)=0;			//!< 単一行列変換後のレンダリング(begin~endを行わない、階層などを見ない)

	// --- Variable
	SpHJTexture		m_spTexture;				//!< テクスチャ
	hj_bool			m_bCenter;					//!< センタリング描画
	hj_f32			m_f32PosX, m_f32PosY;		//!< 位置
	hj_f32			m_f32Deg;					//!< 回転
	hj_f32			m_f32ScaleX, m_f32ScaleY;	//!< スケール
	hj_color32		m_u32Color;					//!< カラー
	hj_f32			m_f32TexUvLeft, m_f32TexUvTop, m_f32TexUvRight, m_f32TexUvBottom;		//!< テクスチャ切り取り座標
	stcMatrix44		m_sTransformMtx;			//!< 変換マトリクス(ローカルマトリクス)
	stcMatrix44		m_sWorldTransformMtx;		//!< 親を含むワールド変換マトリクス
	hj_bool			m_bUpdateMtx;				//!< マトリクス更新チェック
	clsHJSprite*	m_pcParent;					//!< 親スプライト
	clsHJSprite*	m_pcChild;					//!< 子スプライト
	clsHJSprite*	m_pcSibling;				//!< 兄弟スプライト
};

}// namespace hj

#endif // __HJ_SPRITE_H_