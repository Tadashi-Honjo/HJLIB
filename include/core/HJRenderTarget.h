//--------------------------------------------------------------
/*!	@file
	@brief	レンダーターゲット
			
	@author 本城 直志
	@date	11/10/18
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_RENDER_TARGET_H_
#define __HJ_RENDER_TARGET_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	レンダーターゲット基底クラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJRenderTarget
{
public:
	clsHJRenderTarget():m_u32Width(0), m_u32Height(0){};
	virtual ~clsHJRenderTarget(){};

	// --- Method
	virtual hj_bool CreateRenderTarget(hj_u32 u32Width, hj_u32 u32Height)=0;	//!< レンダーターゲット作成

	// --- Get
	virtual SpHJTexture GetTexture()const	{ return m_spTexture;	}			//!< テクスチャ取得
	virtual hj_u32 GetWidth()const			{ return m_u32Width;	}			//!< バッファ幅取得
	virtual hj_u32 GetHeight()const			{ return m_u32Height;	}			//!< バッファ高さ取得

protected:
	hj_u32			m_u32Width, m_u32Height;									// サイズ
	SpHJTexture		m_spTexture;												// テクスチャ
};

}// namespace hj

#endif // __HJ_RENDER_TARGET_H_