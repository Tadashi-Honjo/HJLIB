#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <tchar.h>
#include <vector>
#include <string>

class IMeshLoader
{
public:
   // 頂点エレメントの格納方法の種類の一覧
   enum GEOMETRY_TYPE
   {
		GEOMETRY_TYPE_POLYGONS,
		GEOMETRY_TYPE_POLYLIST,
		GEOMETRY_TYPE_TRIANGLES,
		GEOMETRY_TYPE_TRIFANS,
		GEOMETRY_TYPE_TRISTRIPS,
		GEOMETRY_TYPE_MAXNUM,

		GEOMETRY_TYPE_UNKNOWN = GEOMETRY_TYPE_MAXNUM,
   };

   // COLLADAから取得したデータを格納する構造体
   typedef struct _DATAARRAY
   {
      // 頂点エレメントごとにデータを格納するための構造体
      typedef struct _VERTEX_ELEMENT
      {
         // 頂点エレメントの種類
         std::wstring Semantic;
         // 頂点エレメントで必要となるデータ数。例えば頂点座標の場合 x, y, z で 3 。
         UINT Stride;
         // 実データ。Stride数分使用する。
         typedef struct _VECTOR
         {
            float vector[3];
         }VECTOR;
         // １つの頂点エレメント内のデータ配列
         std::vector<VECTOR> Vector;
         // インデックスデータを示す
         std::vector<UINT> Index;
      }VERTEX_ELEMENT;

      // ジオメトリ名
      std::wstring InstanceGeometryName;
      // 頂点エレメントの格納方法の種類
      GEOMETRY_TYPE GeometryType;
      // １ポリゴンの頂点数を示す配列。
      std::vector<UINT> VCount;
      // 頂点エレメントごとのデータ配列
      std::vector<VERTEX_ELEMENT> VertexElement;
      // マテリアル名
      std::wstring InstanceMaterialName;
      // テクスチャーへのパス。とりあえず１つのみとする。
      std::wstring ImageFileName;
      // サンプラーステートはとりあえず考慮しない
   }DATAARRAY;

public:
   std::vector<DATAARRAY> m_DataArray;

public:
   inline IMeshLoader(){};
   inline virtual ~IMeshLoader(){ Invalidate(); };
   inline virtual void Invalidate(){ m_DataArray.clear(); };
   // このメンバ関数は FBX, COLLADA ローダーを実装するクラス側で実体化する
   virtual void LoadMesh( const TCHAR* pFileName ) = 0;
};

#endif