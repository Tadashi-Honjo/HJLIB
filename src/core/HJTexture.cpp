//--------------------------------------------------------------
/*!	@file
	@brief	テクスチャ
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

namespace hj{
	
// clsHJTextureResource
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビルド(clsHJFileMgrから呼ぶ用)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏z┏┏┏┏┏┏
hj_bool clsTextureResource::Builded( SpBinary spBinary )
{
	m_spTexture.Reset( NEW clsPfTexture() );
	m_spTexture->ReadTexture( spBinary->GetBuffer(), spBinary->GetSize() );
	return true;
}

} //namespace hj
