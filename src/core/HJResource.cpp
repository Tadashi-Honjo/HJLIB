//--------------------------------------------------------------
/*!	@file
	@brief	HJリソース
			
	@author 本城 直志
	@date	15/02/01
*/
//	Copyright (C) 2015 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

namespace hj{
	
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビルド(clsHJFileMgrから呼ぶ用)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJResource::Build( const hj_tchar* otcFilePath )
{
	m_builded = Builded( otcFilePath );
}
void clsHJResource::Build( SpBinary spBinary )
{
	m_builded = Builded( spBinary );
}

} //namespace hj
