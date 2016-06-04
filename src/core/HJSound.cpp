//--------------------------------------------------------------
/*!	@file
	@brief	HJSound
			
	@author 本城 直志
	@date	12/08/19
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

// =============================== clsSoundResource
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビルド処理
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsSoundResource::Builded( const hj_tchar* otcFilePath )
{
	m_spSound.Reset( NEW clsPfSound() );
	return m_spSound->LoadFile( otcFilePath );
}

// =============================== clsSoundStreamResource
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビルド処理
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsSoundStreamResource::Builded( const hj_tchar* otcFilePath )
{
	m_spSound.Reset( NEW clsPfStreamSound() );
	return m_spSound->LoadFile( otcFilePath );
}

}// namespace hj
