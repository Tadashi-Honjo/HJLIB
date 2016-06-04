//--------------------------------------------------------------
/*!	@file
	@brief	HJShader
			
	@author 本城 直志
	@date	11/11/09
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

namespace hj{
	
// clsShaderResource
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビルド(clsHJFileMgrから呼ぶ用)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏z┏┏┏┏┏┏
#ifdef HJ_DEBUG
hj_bool clsShaderResource::Builded( const hj_tchar* otcFilePath )
{
	m_spShader.Reset( NEW clsPfShader() );
	return m_spShader->CreateShaderFile( otcFilePath );
}
#endif // HJ_DEBUG
hj_bool clsShaderResource::Builded( SpBinary spBinary )
{
	m_spShader.Reset( NEW clsPfShader() );
	return m_spShader->CreateShaderFile( spBinary->GetBuffer(), spBinary->GetSize() );
}

} //namespace hj
