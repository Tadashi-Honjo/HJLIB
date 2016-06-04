//--------------------------------------------------------------
/*!	@file
	@brief	MeshPlatform

	@author �{�� ���u
	@date	14/02/20
*/
//	Copyright (C) 2014 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#include "stdafx.h"
#include "Exception.h"
#include "ColladaLoader.h"
#include "MeshData.h"
#include "MeshDataCreater.h"
#include "MeshPlatform.h"

namespace{

	#define VERSION_MAJOR (0)
	#define VERSION_MINOR (1)
	
	//! �o�̓��f���f�[�^�w�b�_
	struct stcModelHeader{
		hj_c8		header[4];						// 'HJMD'
		hj_u8		u8VersionMajor, u8VersionMinor;	// �o�[�W����
		hj_u8		u8Pad[2];
		hj_u32		u32MeshNum;						// ���b�V����
		// ����stcMeshOffsetHeader��u32MeshNum������
	};
	struct stcMeshHeader{
		hj_u32	u32VertexNum;
		hj_u32	u32VertexIndexNum;
		hj_u32	u32VertexListOffset;			// ���_���X�g�ւ̃I�t�Z�b�g(stcMeshHeader�����)
		hj_u32	u32VertexIndexListOffset;		// ���_�C���f�b�N�X���X�g�̃I�t�Z�b�g(stcMeshHeader�����)
	};

	// ========= WinDx
	struct stcWinDxFormat_PNCT{
		hj_f32 		f32x, f32y, f32z;
		hj_f32		f32nx, f32ny, f32nz;
		hj_color32 	u32Color;
		hj_f32 		f32u, f32v;
	};
	bool outputMeshFileWinDx(const hj_tchar* optcOutputPath, CMeshData* pcMeshData)
	{
		FILE *fpWrite=NULL;
		hj_size_t sizeWriteCnt = 0;
		hj_size_t sizeTmp = 0;
		if(_tfopen_s(&fpWrite, optcOutputPath, HJ_T("wb")) != 0 || fpWrite==NULL){
			printf("�o�̓t�@�C�����o�͂ł��܂���B");
			return false;
		}
		
		// ========= ���f���f�[�^
		stcModelHeader sModelHeader={0};
		sizeTmp = sizeof(stcModelHeader);

		// �V�O�l�`��
		sModelHeader.header[0] = 'H';
		sModelHeader.header[1] = 'J';
		sModelHeader.header[2] = 'M';
		sModelHeader.header[3] = 'D';

		// �o�[�W����
		sModelHeader.u8VersionMajor = VERSION_MAJOR;
		sModelHeader.u8VersionMinor = VERSION_MINOR;

		// ���b�V����
		sModelHeader.u32MeshNum = pcMeshData->GetMeshCount();
		
		// ��������
		sizeWriteCnt += sizeTmp;

		// ========= ���b�V���w�b�_�[
		stcMeshHeader* psMeshHeaderList = NEW stcMeshHeader[sModelHeader.u32MeshNum]; 
		sizeTmp = sizeof(stcMeshHeader)*sModelHeader.u32MeshNum;
		::memset(psMeshHeaderList, 0x00, sizeTmp );

		// ��������
		sizeWriteCnt += sizeTmp;

		// ========= ���b�V�����
		fseek(fpWrite, sizeWriteCnt, SEEK_SET);

		const CMeshData::MESH* opsMesh = NULL;
		stcMeshHeader* psMeshHeader	= NULL;
		for(hj_u32 u32i=0; u32i<sModelHeader.u32MeshNum; ++u32i)
		{
			psMeshHeader = &psMeshHeaderList[u32i];
			opsMesh = pcMeshData->GetMeshArray(u32i);

			// --- ���_���X�g
			if(opsMesh->pVertex.size() > 0){
				stcWinDxFormat_PNCT* psVertexList = NULL;
				stcWinDxFormat_PNCT* psVertex = NULL;
				const CMeshData::VERTEX* psMeshVertex = NULL;

				psVertexList = NEW stcWinDxFormat_PNCT[opsMesh->pVertex.size()];
				sizeTmp = sizeof(stcWinDxFormat_PNCT)*opsMesh->pVertex.size();
				::memset(psVertexList, 0x00, sizeTmp);

				for(hj_u32 u32j=0; u32j<opsMesh->pVertex.size(); ++u32j){
					psVertex = &psVertexList[u32j];
					psMeshVertex = &opsMesh->pVertex[u32j];

					// �R�s�[
					psVertex->f32x = psMeshVertex->Vertex.f32x;
					psVertex->f32y = psMeshVertex->Vertex.f32y;
					psVertex->f32z = psMeshVertex->Vertex.f32z;
					psVertex->f32nx = psMeshVertex->Normal.f32x;
					psVertex->f32ny = psMeshVertex->Normal.f32y;
					psVertex->f32nz = psMeshVertex->Normal.f32z;
					psVertex->u32Color = 0xFFFFFFFF;
					psVertex->f32u = psMeshVertex->UV.f32x;
					psVertex->f32v = psMeshVertex->UV.f32y;
				}

				// ��������
				fwrite(psVertexList, sizeTmp, 1, fpWrite);

				// ���_��
				psMeshHeader->u32VertexNum = opsMesh->pVertex.size();
				// �I�t�Z�b�g�ʒu
				psMeshHeader->u32VertexListOffset = sizeWriteCnt;
				sizeWriteCnt += sizeTmp;

				HJ_SAFE_DELETE_ARRAY(psVertexList);
			}

			// --- �C���f�b�N�X���X�g
			if(opsMesh->pIndex.size() > 0){
				UINT* pu32IndexList = NULL;

				pu32IndexList = NEW UINT[opsMesh->pIndex.size()];
				sizeTmp = sizeof(UINT)*opsMesh->pIndex.size();
				::memset(pu32IndexList, 0x00, sizeTmp);

				for(hj_u32 u32j=0; u32j<opsMesh->pIndex.size(); ++u32j){
					pu32IndexList[u32j] = opsMesh->pIndex[u32j];
				}

				// ��������
				fwrite(pu32IndexList, sizeTmp, 1, fpWrite);

				// �C���f�b�N�X��
				psMeshHeader->u32VertexIndexNum = opsMesh->pIndex.size();
				// �I�t�Z�b�g�ʒu
				psMeshHeader->u32VertexIndexListOffset = sizeWriteCnt;
				sizeWriteCnt += sizeTmp;

				HJ_SAFE_DELETE_ARRAY(pu32IndexList);
			}
		}

		// �擪�ɖ߂��ăI�t�Z�b�g�������񂾂��̂���������
		sizeWriteCnt = 0;
		fseek(fpWrite, sizeWriteCnt, SEEK_SET);

		// ========= ���f���f�[�^
		sizeTmp = sizeof(stcModelHeader);
		fwrite(&sModelHeader, sizeTmp, 1, fpWrite); // �I�t�Z�b�g������̂ōŌ�ɂ�����x��������

		// ========= ���b�V���w�b�_�[
		sizeTmp = sizeof(stcMeshHeader)*sModelHeader.u32MeshNum;
		fwrite(psMeshHeaderList, sizeTmp, 1, fpWrite);

		// �J��
		HJ_SAFE_DELETE_ARRAY(psMeshHeaderList);

		return true;
	}


	//! �v���b�g�t�H�[�����Ƃ̃f�[�^
	struct stcPlatformData{
		hj_tchar atcMatchName[16]; // ���̓t�@�C���`�F�b�N�p�̊g���q
		(bool)(*fpOutput)(const hj_tchar* optcOutputPath, CMeshData* pcMeshData);
	};

	const stcPlatformData atosPlatformData[PLATFORM_MAXNUM] = {
		// PLATFORM_WINDX
		{
			HJ_T("WINDX"),
			outputMeshFileWinDx,
		},
	};
	const stcPlatformData* getPlatformData(enmPlatform ePlatform)
	{
		return &atosPlatformData[ePlatform];
	}


} // namespace

// --- Method
// �v���b�g�t�H�[�����Ƃ̏o��
bool outputFilePlatform(enmPlatform ePlatform, const hj_tchar* optcOutputPath, CMeshData* pcMeshData )
{
	return getPlatformData(ePlatform)->fpOutput(optcOutputPath, pcMeshData);
}
// �ϊ�
enmPlatform convPlatform(const hj_tchar* optcName)
{
	for(hj_s32 s32i=0; s32i<PLATFORM_MAXNUM; ++s32i){
		if(_tcscmp(getPlatformData(static_cast<enmPlatform>(s32i))->atcMatchName, optcName) == 0){
			return static_cast<enmPlatform>(s32i);
		}
	}
	return PLATFORM_MAXNUM;
}
