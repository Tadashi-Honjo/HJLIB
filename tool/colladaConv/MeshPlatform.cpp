//--------------------------------------------------------------
/*!	@file
	@brief	MeshPlatform

	@author 本城 直志
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
	
	//! 出力モデルデータヘッダ
	struct stcModelHeader{
		hj_c8		header[4];						// 'HJMD'
		hj_u8		u8VersionMajor, u8VersionMinor;	// バージョン
		hj_u8		u8Pad[2];
		hj_u32		u32MeshNum;						// メッシュ数
		// ↓にstcMeshOffsetHeaderがu32MeshNum分並ぶ
	};
	struct stcMeshHeader{
		hj_u32	u32VertexNum;
		hj_u32	u32VertexIndexNum;
		hj_u32	u32VertexListOffset;			// 頂点リストへのオフセット(stcMeshHeaderからの)
		hj_u32	u32VertexIndexListOffset;		// 頂点インデックスリストのオフセット(stcMeshHeaderからの)
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
			printf("出力ファイルが出力できません。");
			return false;
		}
		
		// ========= モデルデータ
		stcModelHeader sModelHeader={0};
		sizeTmp = sizeof(stcModelHeader);

		// シグネチャ
		sModelHeader.header[0] = 'H';
		sModelHeader.header[1] = 'J';
		sModelHeader.header[2] = 'M';
		sModelHeader.header[3] = 'D';

		// バージョン
		sModelHeader.u8VersionMajor = VERSION_MAJOR;
		sModelHeader.u8VersionMinor = VERSION_MINOR;

		// メッシュ数
		sModelHeader.u32MeshNum = pcMeshData->GetMeshCount();
		
		// 書き込み
		sizeWriteCnt += sizeTmp;

		// ========= メッシュヘッダー
		stcMeshHeader* psMeshHeaderList = NEW stcMeshHeader[sModelHeader.u32MeshNum]; 
		sizeTmp = sizeof(stcMeshHeader)*sModelHeader.u32MeshNum;
		::memset(psMeshHeaderList, 0x00, sizeTmp );

		// 書き込み
		sizeWriteCnt += sizeTmp;

		// ========= メッシュ情報
		fseek(fpWrite, sizeWriteCnt, SEEK_SET);

		const CMeshData::MESH* opsMesh = NULL;
		stcMeshHeader* psMeshHeader	= NULL;
		for(hj_u32 u32i=0; u32i<sModelHeader.u32MeshNum; ++u32i)
		{
			psMeshHeader = &psMeshHeaderList[u32i];
			opsMesh = pcMeshData->GetMeshArray(u32i);

			// --- 頂点リスト
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

					// コピー
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

				// 書き込み
				fwrite(psVertexList, sizeTmp, 1, fpWrite);

				// 頂点数
				psMeshHeader->u32VertexNum = opsMesh->pVertex.size();
				// オフセット位置
				psMeshHeader->u32VertexListOffset = sizeWriteCnt;
				sizeWriteCnt += sizeTmp;

				HJ_SAFE_DELETE_ARRAY(psVertexList);
			}

			// --- インデックスリスト
			if(opsMesh->pIndex.size() > 0){
				UINT* pu32IndexList = NULL;

				pu32IndexList = NEW UINT[opsMesh->pIndex.size()];
				sizeTmp = sizeof(UINT)*opsMesh->pIndex.size();
				::memset(pu32IndexList, 0x00, sizeTmp);

				for(hj_u32 u32j=0; u32j<opsMesh->pIndex.size(); ++u32j){
					pu32IndexList[u32j] = opsMesh->pIndex[u32j];
				}

				// 書き込み
				fwrite(pu32IndexList, sizeTmp, 1, fpWrite);

				// インデックス数
				psMeshHeader->u32VertexIndexNum = opsMesh->pIndex.size();
				// オフセット位置
				psMeshHeader->u32VertexIndexListOffset = sizeWriteCnt;
				sizeWriteCnt += sizeTmp;

				HJ_SAFE_DELETE_ARRAY(pu32IndexList);
			}
		}

		// 先頭に戻ってオフセット書き込んだものを書き込み
		sizeWriteCnt = 0;
		fseek(fpWrite, sizeWriteCnt, SEEK_SET);

		// ========= モデルデータ
		sizeTmp = sizeof(stcModelHeader);
		fwrite(&sModelHeader, sizeTmp, 1, fpWrite); // オフセットがあるので最後にもう一度書き込む

		// ========= メッシュヘッダー
		sizeTmp = sizeof(stcMeshHeader)*sModelHeader.u32MeshNum;
		fwrite(psMeshHeaderList, sizeTmp, 1, fpWrite);

		// 開放
		HJ_SAFE_DELETE_ARRAY(psMeshHeaderList);

		return true;
	}


	//! プラットフォームごとのデータ
	struct stcPlatformData{
		hj_tchar atcMatchName[16]; // 入力ファイルチェック用の拡張子
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
// プラットフォームごとの出力
bool outputFilePlatform(enmPlatform ePlatform, const hj_tchar* optcOutputPath, CMeshData* pcMeshData )
{
	return getPlatformData(ePlatform)->fpOutput(optcOutputPath, pcMeshData);
}
// 変換
enmPlatform convPlatform(const hj_tchar* optcName)
{
	for(hj_s32 s32i=0; s32i<PLATFORM_MAXNUM; ++s32i){
		if(_tcscmp(getPlatformData(static_cast<enmPlatform>(s32i))->atcMatchName, optcName) == 0){
			return static_cast<enmPlatform>(s32i);
		}
	}
	return PLATFORM_MAXNUM;
}
