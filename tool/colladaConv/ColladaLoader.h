#ifndef COLLADA_LOADER_H
#define COLLADA_LOADER_H

// 非標準の拡張機能が使用されています: enum 'enum' が限定名で使用されます。このワーニングを抑制する。
#pragma warning(disable: 4482)

// XMLLiteを使うため
#include <xmllite.h>
#pragma comment(lib, "xmllite.lib")

#include "MeshLoader.h"

class CColladaLoader : public IMeshLoader
{
private:
   // IXmlReaderの内部文字コードがWCHARっぽいので合わせる
   typedef struct _XMLPATHARRAY
   {
      LPCWSTR pTagName;            // タグ名
      LPCWSTR pAttribute;          // 属性名
      LPCWSTR pAttributeValue;     // 属性値
   }XMLPATHARRAY;

   IXmlReader* m_pReader;
   IStream* m_pStream;

   // xml を読み込む開始位置を先頭に移動する
   void SetSeekTop() const;

   // 属性値を取得する
   const bool GetAttributeValue( const LPCWSTR pAttribute, std::wstring* pValue ) const;
   // テキストを取得する
   const bool GetText( std::wstring* pValue ) const;

   // 現在の階層のタグ名および属性値が検索条件と一致するかを判定する
   const bool FindTagOrAttribute( const XMLPATHARRAY* pXmlPath, const LPCWSTR pwszLocalName )const;

   // ノード検索
   const std::vector<std::wstring> GetElementsByNodeName( const XMLPATHARRAY* pXmlPath, UINT XmlPathArrayCount ) const;

   // target や url 属性値の頭の#を取り除く
   void GetID( std::wstring* Target ) const;

   // 文字列をばらす
   void Split( const std::vector<std::wstring>* pInStr, const wchar_t Delimiter, std::vector<std::wstring>* pOutStr, std::vector<hj_u32>* pOutSplitCount=NULL) const;

   // inputタグの属性値を取得
   const bool GetInputData( const std::vector<std::wstring>* pSemanticArray
                          , const std::vector<std::wstring>* pSourceArray
                          , const std::vector<std::wstring>* pOffsetArray
                          , const LPCWSTR pSearchSemantic
                          , std::wstring* pSource
                          , UINT* pOffset ) const;

   // ジオメトリの種類を取得
   const int CheckGeometryType( const std::wstring* pInstanceGeometry ) const;

   // Polygonsデータを取得
   void GetPolygons( const std::wstring* pInstanceGeometry );
   // Polylistデータを取得
   void GetPolylists( const std::wstring* pInstanceGeometry );
   void GetPolylist( const std::wstring* pInstanceGeometry, const std::wstring* pMaterialName );

   // Trianglesデータを取得
   void GetTriangles( const std::wstring* pInstanceGeometry );

   // Materialデータ
   void GetMaterial( const std::wstring* pInstanceGeometry, DATAARRAY& rsDataArray );

public:
   CColladaLoader();
   virtual ~CColladaLoader();
   void Invalidate();
   void LoadMesh( const TCHAR* pFileName );
};

#endif