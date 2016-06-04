//--------------------------------------------------------------
/*!	@file
	@brief	WinDxテクスチャ
			
	@author 本城 直志
	@date	11/10/18
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_TEXTURE_H_
#define __WINDX_TEXTURE_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDxTextureクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxTexture : public clsHJTexture
{
public:
	clsWinDxTexture();
	virtual ~clsWinDxTexture();

	// レンダーターゲット用テクスチャの作成
	virtual hj_bool CreateRenderTargetTexture(hj_u32 u32Width, hj_u32 u32Height);

	// --- Get
	const LPDIRECT3DTEXTURE9 GetTexture() const {return m_pTexture;};	// テクスチャ取得

	// --- Set
	void SetTextureBuff( LPDIRECT3DTEXTURE9 pTexture ){m_pTexture=pTexture;};	// テクスチャ設定

protected:
	// テクスチャ読み込み
	virtual hj_bool ReadTexture(const hj_tchar* ptcPath);
	virtual hj_bool ReadTexture(const hj_u8* pu8Buff, hj_size_t sizeFile);

private:

	LPDIRECT3DTEXTURE9	m_pTexture;
};

}// namespace hj

#endif // __WINDX_TEXTURE_H_