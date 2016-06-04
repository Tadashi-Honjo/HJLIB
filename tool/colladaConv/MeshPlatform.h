//--------------------------------------------------------------
/*!	@file
	@brief	MeshPlatform

	@author 本城 直志
	@date	14/02/20
*/
//	Copyright (C) 2014 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef _MESHPLATFORM_H_
#define _MESHPLATFORM_H_

class CMeshData;

//! プラットフォーム
enum enmPlatform{
	PLATFORM_WINDX, // Win DirectX
	PLATFORM_MAXNUM,
};

// --- Method
bool outputFilePlatform(enmPlatform ePlatform, const hj_tchar* optcOutputPath, CMeshData* pcMeshData );
enmPlatform convPlatform(const hj_tchar* optcName);


#endif // _MESHPLATFORM_H_
