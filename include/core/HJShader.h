//--------------------------------------------------------------
/*!	@file
	@brief	HJShader
			
	@author 本城 直志
	@date	11/11/09
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_SHADER_H_
#define __HJ_SHADER_H_

// HjLib namespace
namespace hj{

// Shaderリソース
class clsShaderResource : public clsHJResource
{
	typedef clsHJResource MyClassParent;
public:
	clsShaderResource():MyClassParent(){}
	virtual ~clsShaderResource(){}

	SpHJShader GetShader()const{ return m_spShader; }

protected:
#ifdef HJ_DEBUG
	// コンパイルエラーを出すためにデバッグ時のみファイルパスでビルド
	virtual hj_bool Builded( const hj_tchar* otcFilePath );
#endif
	virtual hj_bool Builded( SpBinary spBinary );

private:
	SpHJShader	m_spShader;
};
typedef clsHJSharedPtr< clsShaderResource > SpShaderResource;


// clsHJShader
class clsHJShader
{
public:	
	clsHJShader(){};
	virtual ~clsHJShader(){};

	// --- Method
	virtual void Begin()=0;
	virtual void End()=0;

	// --- Set
	virtual void SetFloat(const hj_c8* pu8Name, hj_f32 f32Float)=0;
	virtual void SetVector2(const hj_c8* pu8Name, const stcVector2& rsVec2)=0;
	virtual void SetVector3(const hj_c8* pu8Name, const stcVector3& rsVec3)=0;
	virtual void SetVector4(const hj_c8* pu8Name, const stcVector4& rsVec4)=0;
	virtual void SetMatrix44(const hj_c8* pu8Name, const stcMatrix44& rsMatrix44)=0;
	virtual void SetTexture(const hj_c8* pu8Name, const SpHJTexture spTexture)=0;
	virtual void SetTechnique(const hj_c8* pu8Name)=0;

protected:
	virtual hj_bool CreateShaderFile(const hj_tchar* ptcPath)=0;
	virtual hj_bool CreateShaderFile(const hj_u8* pu8Buff, hj_size_t sizeFile)=0;

private:
	friend class clsShaderResource;
};


} //namespace hj

#endif // __HJ_SHADER_H_
