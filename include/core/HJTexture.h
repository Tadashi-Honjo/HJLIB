//--------------------------------------------------------------
/*!	@file
	@brief	テクスチャ
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_TEXTURE_H_
#define __HJ_TEXTURE_H_

// HJLib namespace
namespace hj{

// テクスチャリソース
class clsTextureResource : public clsHJResource
{
	typedef clsHJResource MyClassParent;
public:
	clsTextureResource():MyClassParent(){};
	virtual ~clsTextureResource(){};

	SpHJTexture GetTexture()const{ return m_spTexture; }

protected:
	virtual hj_bool Builded( SpBinary spBinary );

private:
	SpHJTexture	m_spTexture;
};
typedef clsHJSharedPtr< clsTextureResource > SpTextureResource;

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Texture基底クラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJTexture
{
public:
	clsHJTexture():m_u32Width(0), m_u32Height(0){};
	virtual ~clsHJTexture(){};

	// レンダーターゲット用テクスチャの作成
	virtual hj_bool CreateRenderTargetTexture(hj_u32 u32Width, hj_u32 u32Height)=0;

	// --- Get
	virtual hj_u32	GetWidth()const		{ return m_u32Width; }
	virtual hj_u32	GetHeight()const	{ return m_u32Height; }

protected:
	// テクスチャ読み込み
	virtual hj_bool ReadTexture(const hj_tchar* ptcPath)=0;
	virtual hj_bool ReadTexture(const hj_u8* pu8Buff, hj_size_t sizeFile)=0;

	hj_u32 m_u32Width, m_u32Height;

	friend class clsTextureResource;
};

}// namespace hj

#endif // __HJ_TEXTURE_H_