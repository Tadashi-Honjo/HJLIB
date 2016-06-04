//--------------------------------------------------------------
/*!	@file
	@brief	MeshPlatform

	@author �{�� ���u
	@date	14/02/20
*/
//	Copyright (C) 2014 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef _MESHPLATFORM_H_
#define _MESHPLATFORM_H_

class CMeshData;

//! �v���b�g�t�H�[��
enum enmPlatform{
	PLATFORM_WINDX, // Win DirectX
	PLATFORM_MAXNUM,
};

// --- Method
bool outputFilePlatform(enmPlatform ePlatform, const hj_tchar* optcOutputPath, CMeshData* pcMeshData );
enmPlatform convPlatform(const hj_tchar* optcName);


#endif // _MESHPLATFORM_H_
