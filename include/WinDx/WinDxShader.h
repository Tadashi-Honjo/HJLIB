//--------------------------------------------------------------
/*!	@file
	@brief	WinDx Shader
			
	@author 本城 直志
	@date	11/11/09
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_SHADER_H_
#define __WINDX_SHADER_H_

// HjLib namespace
namespace hj{

class clsWinDxShader : public clsHJShader
{
public:	
	clsWinDxShader();
	virtual ~clsWinDxShader();

	// --- Method
	virtual void Begin();
	virtual void End();

	// --- Set
	virtual void SetFloat(const hj_c8* pu8Name, hj_f32 f32Float);
	virtual void SetVector2(const hj_c8* pu8Name, const stcVector2& rsVec2);
	virtual void SetVector3(const hj_c8* pu8Name, const stcVector3& rsVec3);
	virtual void SetVector4(const hj_c8* pu8Name, const stcVector4& rsVec4);
	virtual void SetMatrix44(const hj_c8* pu8Name, const stcMatrix44& rsMatrix44);
	virtual void SetTexture(const hj_c8* pu8Name, const SpHJTexture spTexture);
	virtual void SetTechnique(const hj_c8* pu8Name);

protected:
	virtual hj_bool CreateShaderFile(const hj_tchar* ptcPath);
	virtual hj_bool CreateShaderFile(const hj_u8* pu8Buff, hj_size_t sizeFile);
	virtual hj_bool CreateShaderResource(hj_u32 u32Resourceid);	//!< シェーダー作成埋め込み用リソース(注：#includeディレクティブ使えません)

private:
	LPD3DXEFFECT m_pcEffect;	//!< エフェクト
};


} //namespace hj

#endif // __WINDX_SHADER_H_
