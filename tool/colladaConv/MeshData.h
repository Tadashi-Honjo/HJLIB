#ifndef MESHDATA_H
#define MESHDATA_H

#include <string>
#include <vector>

struct stcVector3{
	hj_f32 f32x, f32y, f32z;
};

// メッシュ情報クラス
class CMeshData
{
public:

   // 頂点情報構造体
   typedef struct _VERTEX
   {
      stcVector3  Vertex; // 頂点データ
      stcVector3  Normal; // 法線ベクトル
      stcVector3  UV;     // テクセル
   }VERTEX;

   // マテリアル構造体
   typedef struct _MATERIAL
   {
      std::wstring InstanceMaterialName;  // テクスチャーの種別名
      std::wstring ImageFileName;         // 実行ファイルからテクスチャーまでの相対パス
   }MATERIAL;

   typedef struct _MESH
   {
      std::wstring InstanceGeometryName;// メッシュ名
	  std::vector<VERTEX> pVertex;      // 頂点エレメントのデータ
      std::vector<UINT> pIndex;			// インデックスのデータ
      MATERIAL Material;                // マテリアル
   }MESH;

private:
	std::vector<MESH> m_pMeshArray;
	UINT m_MeshCount;

public:
	CMeshData();
	virtual ~CMeshData();
	void Invalidate();

	UINT GetVertexBufferStrideSize(){
	  return sizeof( CMeshData::VERTEX );
	};
	void CreateMeshArray( UINT MeshCount ){
		m_MeshCount = MeshCount;
		m_pMeshArray.resize(m_MeshCount);
	};

	// --- Get
	UINT GetMeshCount() { return m_MeshCount; };
	MESH* GetMeshArray( UINT Index ) { return ( Index < m_MeshCount ) ? &m_pMeshArray[Index] : NULL;	};
};

#endif