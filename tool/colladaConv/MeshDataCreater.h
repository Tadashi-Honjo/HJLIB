#ifndef MESHDATA_CREATER_H
#define MESHDATA_CREATER_H

#include "MeshLoader.h"
#include "MeshData.h"

class CMeshDataCreater
{
public:
   CMeshDataCreater();
   virtual ~CMeshDataCreater();
   void Invalidate();
   void LoadMesh( const hj_tchar* pFileName, CMeshData** ppMeshData );

private:
   void SetVertexElement( UINT Stride, const float pInVertexElement[3], stcVector3* pOutVertexElement );

	// Polygons形式の頂点エレメントデータをデータ構造にコンバートする。
	void PolygonsConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );
	// Polylist形式の頂点エレメントデータをデータ構造にコンバートする。
	void PolylistConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );
	// Triangles形式の頂点エレメントデータをデータ構造にコンバートする。
	void TrianglesConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );

	// シェーダーリソースビューとサンプラーを作成する
	void CreateMaterial( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );

   // メッシュローダー
   IMeshLoader* m_pMeshLoader;
};

#endif