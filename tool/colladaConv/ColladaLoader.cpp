#include <tchar.h>
#include <vector>
#include <string>
#include <shlwapi.h>
#include "Exception.h"
#include "ColladaLoader.h"

// ****************************************************************************************************************
// COLLADA ― Digital Asset Schema リリース1.4.1
// ****************************************************************************************************************

#pragma comment( lib, "Shlwapi.lib" )

namespace{
   static const hj_wc16* towc16Geometory[] = {
   		L"polygons", // GEOMETRY_TYPE_POLYGONS
		L"polylist", // GEOMETRY_TYPE_POLYLIST
		L"triangles", // GEOMETRY_TYPE_TRIANGLES
		L"trifans", // GEOMETRY_TYPE_TRIFANS
		L"tristrips", // GEOMETRY_TYPE_TRISTRIPS
   };
   HJ_STATIC_ASSERT(HJ_ARRAY_NUM(towc16Geometory) == IMeshLoader::GEOMETRY_TYPE_MAXNUM);
}

CColladaLoader::CColladaLoader()
{
   m_pReader = NULL;
   m_pStream = NULL;
}

void CColladaLoader::Invalidate()
{
   if( m_pReader )
   {
      m_pReader->Release();
      m_pReader = NULL;
   }
   if( m_pStream )
   {
      m_pStream->Release();
      m_pStream = NULL;
   }
   IMeshLoader::Invalidate();
}

CColladaLoader::~CColladaLoader()
{
   Invalidate();
}

// xml を読み込む開始位置を先頭に移動する
void CColladaLoader::SetSeekTop() const
{
   LARGE_INTEGER li = { 0, 0 };
   if( FAILED( m_pStream->Seek( li, tagSTREAM_SEEK::STREAM_SEEK_SET, NULL ) ) )
      throw( CException( "CColladaLoader::SetSeekTop()でエラーが発生しました。", "IStream::Seek()でエラーが発生しました。", "" ) );

   if( FAILED( m_pReader->SetInput( m_pStream ) ) )
      throw( CException( "CColladaLoader::SetSeekTop()でエラーが発生しました。", "IxmlReader::SetInput()でエラーが発生しました。", "" ) );
}

// 属性値を取得する
const bool CColladaLoader::GetAttributeValue( const LPCWSTR pAttribute, std::wstring* pValue ) const
{
   bool Find = false;
   HRESULT hr;
   LPCWSTR pwszValue = NULL;

   pValue->clear();

   // 属性を検索
   hr = m_pReader->MoveToAttributeByName( pAttribute, NULL );
   switch( hr )
   {
   // 属性なし
   case S_FALSE:
      goto EXIT;
      break;
   case E_FAIL:
      throw( CException( "CColladaLoader::GetAttributeValue()でエラーが発生しました。", "IXmlReader::MoveToAttributeByName()でエラーが発生しました。", "" ) );
      break;
   }

   if( pValue != NULL )
   {
      // 属性値を取得
      if( FAILED( m_pReader->GetValue( &pwszValue, NULL ) ) )
         throw( CException( "CColladaLoader::GetAttributeValue()でエラーが発生しました。", "IXmlReader::GetValue()でエラーが発生しました。", "" ) );

      pValue->append( pwszValue );
   }

   Find = true;

EXIT:
   return Find;
}

// テキストを取得する
const bool CColladaLoader::GetText( std::wstring* pValue ) const
{
   bool Find = false;
   LPCWSTR pwszValue = NULL;
   XmlNodeType nodeType;
   pValue->clear();

   // 読み込み
   while( m_pReader->Read( &nodeType ) == S_OK )
   {
      switch( nodeType )
      {
      case XmlNodeType::XmlNodeType_Text:
         // 値を取得する
         if( FAILED( m_pReader->GetValue( &pwszValue, NULL ) ) )
            throw( CException( "CColladaLoader::GetText()でエラーが発生しました。", "IXmlReader::GetValue()でエラーが発生しました。", "" ) );

         pValue->append( pwszValue );
         Find = true;
         goto EXIT;
         break;

      case XmlNodeType::XmlNodeType_EndElement:
         goto EXIT;
         break;
      }
   }

EXIT:
   return Find;
}

const bool CColladaLoader::FindTagOrAttribute( const XMLPATHARRAY* pXmlPath, const LPCWSTR pwszLocalName )const
{
   bool Find = false;
   std::wstring pValue;

   if( wcscmp( pXmlPath->pTagName, pwszLocalName ) == 0 )
   {
      // 検索条件に属性が指定されていない
      if( pXmlPath->pAttribute == NULL )
      {
         Find = true;
         goto EXIT;
      }

      // 検索条件に属性が指定されている
      else
      {
         // 属性名で検索してみつかった
         if( GetAttributeValue( pXmlPath->pAttribute, &pValue ) == true )
         {
            // 検索条件に属性値がある
            if( pXmlPath->pAttributeValue != NULL )
            {
               // 属性値が一致した
               if( wcscmp( pXmlPath->pAttributeValue, pValue.data() ) == 0 )
               {
                  Find = true;
                  goto EXIT;
               }
            }
            // 検索条件に属性値がない
            else
            {
               Find = true;
               goto EXIT;
            }
         }
      }
   }

EXIT:
   return Find;
}

// ノード検索
const std::vector<std::wstring> CColladaLoader::GetElementsByNodeName( const XMLPATHARRAY* pXmlPath, UINT XmlPathArrayCount ) const
{
   std::vector<std::wstring> RetValue;

   XmlNodeType nodeType;
   LPCWSTR pwszLocalName = NULL;
   std::wstring pValue;
   UINT Depth = 0;
   int Level = -1;

   // シークを先頭に移動
   SetSeekTop();

   // 読み込み
   while( m_pReader->Read( &nodeType ) == S_OK )
   {
      switch( nodeType )
      {
      case XmlNodeType::XmlNodeType_Element:
         // 要素名を取得
         if( FAILED( m_pReader->GetLocalName( &pwszLocalName, NULL ) ) )
            throw( CException( "CColladaLoader::getElementsByNodeName()でエラーが発生しました。", "IXmlReader::GetLocalName()でエラーが発生しました。", "" ) );

         // 階層を取得
         if( FAILED( m_pReader->GetDepth( &Depth ) ) )
            throw( CException( "CColladaLoader::getElementsByNodeName()でエラーが発生しました。", "IXmlReader::GetDepth()でエラーが発生しました。", "" ) );

         // 階層がXMLPATHARRAYの要素数を越えている場合は、確実に対象外なのでスキップする
         if( Depth >= XmlPathArrayCount )
            continue;

         // 検索してみつかった場合
         if( FindTagOrAttribute( &pXmlPath[Depth], pwszLocalName ) == true )
         {
            // レベルが上のほうにあるときはもどす
            if( Level + 1 >= static_cast<int>( Depth ) )
            {
               Level = static_cast<int>( Depth );  // レベルアップ

               // 検索条件が最後の階層のとき
               if( Depth + 1 == XmlPathArrayCount )
               {
                  // テキストを出力する場合 
                  if( pXmlPath[Depth].pAttribute == NULL ||                                              // 検索条件に属性名が指定されていないとき
                      pXmlPath[Depth].pAttribute != NULL && pXmlPath[Depth].pAttributeValue != NULL ) // 検索条件に属性名と属性値が指定されているとき
                  {
                     // テキストを取得する
                     if( GetText( &pValue ) == true )
                     {
                        // 値を出力する
                        RetValue.push_back( pValue.data() );
                     }
                  }
                  // 属性値を出力する場合
                  else if( pXmlPath[Depth].pAttribute != NULL && pXmlPath[Depth].pAttributeValue == NULL )
                  {
                     if( GetAttributeValue( pXmlPath->pAttribute, &pValue ) == true )
                     {
                        RetValue.push_back( pValue.data() );
                     }
                  }
                  else
                     throw( CException( "CColladaLoader::getElementsByNodeName()でエラーが発生しました。", "pXmlPathの検索条件が不正です。", "" ) );
               }
            }
         }
         // 検索対象の要素名が見つからない
         else
         {
            if( Level >= static_cast<int>( Depth ) )
               Level =  static_cast<int>( Depth ) - 1;   // レベルダウン
         }
         break;
      }
   }

   return RetValue;
}

// target や url 属性値の頭の#を取り除く
void CColladaLoader::GetID( std::wstring* Target ) const
{
   if( Target->length() > 0 )
   {
      if( Target->find( '#', 0 ) == 0 )
         Target->erase( 0, 1 );
   }
}

//文字列を分割する
void CColladaLoader::Split( const std::vector<std::wstring>* pInStr, const wchar_t Delimiter, std::vector<std::wstring>* pOutStr, std::vector<hj_u32>* pOutSplitCount ) const
{
	UINT index1 = 0;
	UINT index2 = 0;
	hj_u32 indexListCnt = 0;

//	if(pOutSplitCount) pOutSplitCount->resize(pInStr->size());

	for( UINT i=0; i<pInStr->size(); i++ )
	{
		indexListCnt = 0;
		index1 = index2 = 0;
		while( index2 != std::wstring::npos )
		{
			index2 = (*pInStr)[i].find( Delimiter, index1 );

			if( index2 != std::wstring::npos )
			{
				std::wstring str = (*pInStr)[i].substr( index1, index2 - index1 ).data();
				if( str.length() > 0 )
					pOutStr->push_back( str.data() );
			}
			else
			{
				std::wstring str = (*pInStr)[i].substr( index1 ).data();
				if( str.length() > 0 )
					pOutStr->push_back( str.data() );
			}
			index1 = index2 + 1;
			++indexListCnt;
		}
		if(pOutSplitCount) pOutSplitCount->push_back(indexListCnt);
	}
}

const int CColladaLoader::CheckGeometryType( const std::wstring* pInstanceGeometry ) const
{
   std::wstring InstanceGeometryName = *pInstanceGeometry;
   // 頭の#を取り除く
   GetID( &InstanceGeometryName );

   for(hj_s32 s32i=0; s32i<GEOMETRY_TYPE_MAXNUM; ++s32i)
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
									   { towc16Geometory[s32i], L"count", NULL },
                                    };
      std::vector<std::wstring> check = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
      if( check.size() > 0 )
		return s32i;
   }

   return GEOMETRY_TYPE_UNKNOWN;
}


// inputタグの属性値を取得
const bool CColladaLoader::GetInputData( const std::vector<std::wstring>* pSemanticArray
                                       , const std::vector<std::wstring>* pSourceArray
                                       , const std::vector<std::wstring>* pOffsetArray
                                       , const LPCWSTR pSearchSemantic
                                       , std::wstring* pSource
                                       , UINT* pOffset ) const
{
   bool find = false;

   UINT index = 0;
   for( UINT i=0; i<pSemanticArray->size(); i++ )
   {
      if( wcscmp( (*pSemanticArray)[i].data(), pSearchSemantic ) == 0 )
      {
         pSource->clear();
         pSource->append( (*pSourceArray)[index].data() );
         GetID( pSource );

         *pOffset = _wtoi( (*pOffsetArray)[index].data() );

         find = true;
         break;
      }

      index++;
   }

   return find;
}

// Polygons
void CColladaLoader::GetPolygons( const std::wstring* pInstanceGeometry )
{
   DATAARRAY polygons;

   polygons.InstanceGeometryName = *pInstanceGeometry;
   // 頭の#を取り除く
   GetID( &polygons.InstanceGeometryName );

   // ***************************************************************************************
   // ジオメトリ種別を設定
   // ***************************************************************************************
   polygons.GeometryType = GEOMETRY_TYPE_POLYGONS;

   // ***************************************************************************************
   // ポリゴン数
   // ***************************************************************************************
   hj_s32 s32PolygonNum = -1;
	{
		std::vector<std::wstring> polygonNum;
		XMLPATHARRAY XmlPathArray[] = { 
									   { L"COLLADA", NULL, NULL },
									   { L"library_geometries", NULL, NULL },
									   { L"geometry", L"id", polygons.InstanceGeometryName.data() },
									   { L"mesh", NULL, NULL },
									   { L"polygons", L"count", NULL },
									};
		polygonNum = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
		if( polygonNum.size() != 1 ){
			throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "polygonsのcount数が不正です。", "" ) );
		}
		s32PolygonNum = _wtoi(polygonNum[0].c_str());
	}

   // ***************************************************************************************
   // semanticを取得
   // ***************************************************************************************
   std::vector<std::wstring> inputSemantic;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polygons.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polygons", NULL, NULL },
                                       { L"input", L"semantic", NULL },
                                    };
      inputSemantic = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSemantic.size() == 0 )
      throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "semantic属性が見つかりません。", "" ) );

   // ***************************************************************************************
   // sourceを取得
   // ***************************************************************************************
   std::vector<std::wstring> inputSource;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polygons.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polygons", NULL, NULL },
                                       { L"input", L"source", NULL },
                                    };
      inputSource = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSource.size() == 0 )
      throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "source属性が見つかりません。", "" ) );

   // 頭の#を取り除く
   for( UINT i=0; i<inputSource.size(); i++ )
      GetID( &inputSource[i] );

   // ***************************************************************************************
   // offset を取得
   // ***************************************************************************************
   std::vector<std::wstring> inputOffset;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polygons.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polygons", NULL, NULL },
                                       { L"input", L"offset", NULL },
                                    };
      inputOffset = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSemantic.size() == 0 )
      throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "offset属性が見つかりません。", "" ) );

   if( inputSemantic.size() != inputSource.size() )
      throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "semanticとsourceの要素数が一致しません。", "" ) );

   if( inputSemantic.size() != inputOffset.size() )
      throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "semanticとoffsetの要素数が一致しません。", "" ) );

   // ***************************************************************************************
   // pデータを daeファイル から取得
   // ***************************************************************************************
   std::vector<std::wstring> p;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polygons.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polygons", NULL, NULL },
                                       { L"p", NULL, NULL },
                                    };
      p = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
//   if( p.size() != 1 )
   if( p.size() != s32PolygonNum )
      throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "pの要素数が不正です。", "" ) );

   // vcountを使ってpolylistと同じ構造に
   std::vector<hj_u32> vcount;

   // ' 'と'\n' でばらす
   std::vector<UINT> uIndex;
   {
      std::vector<std::wstring> temp1, temp2;
      Split( &p, ' ', &temp1, &vcount);
      Split( &temp1, '\n', &temp2 );
      for( UINT i=0; i<temp2.size(); i++ )
         uIndex.push_back( _wtoi( temp2[i].data() ) );
   }

   polygons.VCount.resize(0);
   for(hj_u32 u32i=0; u32i<vcount.size(); ++u32i){
	   polygons.VCount.push_back(vcount[u32i]);
   }
  
   UINT SemanticSize = inputSemantic.size();
   
   // ***************************************************************************************
   // 頂点エレメントを取得
   // ***************************************************************************************
   for( UINT i=0; i<SemanticSize; i++ )
   {
      std::wstring source;
      UINT offset;
      std::vector<std::wstring> float_array, stride;
      DATAARRAY::VERTEX_ELEMENT element;

      // source、offsetを取得
      if( GetInputData( &inputSemantic, &inputSource, &inputOffset, inputSemantic[i].data(), &source, &offset ) == false )
         throw( CException( "CColladaLoader::GetInputData()でエラーが発生しました。", "要素が見つかりません。", "" ) );

      // ***************************************************************************************
      // sourceの値を取得
      // ***************************************************************************************

      // "VERTEX"のときのsourceの内容を取得する
      if( wcscmp( inputSemantic[i].data(), L"VERTEX" ) == 0 )
      {
         std::vector<std::wstring> sourceArray;
         {
            source = inputSource[i];
            GetID( &source );

            XMLPATHARRAY XmlPathArray[] = { 
                                             { L"COLLADA", NULL, NULL },
                                             { L"library_geometries", NULL, NULL },
                                             { L"geometry", L"id", polygons.InstanceGeometryName.data() },
                                             { L"mesh", NULL, NULL },
                                             { L"vertices", L"id", source.data() },
                                             { L"input", L"source", NULL },    // ここはsemantic="POSITION"のみだという前提
                                          };
            sourceArray = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
         }
         if( sourceArray.size() != 1 )
            throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "vertices要素のinput要素の数が不正です。", "" ) );

         source = sourceArray[0];
         GetID( &source );
      }
      // "VERTEX"以外のときのsourceの内容を取得する
      else
      {
         source = inputSource[i];
         GetID( &source );
      }

      // ***************************************************************************************
      // データ配列を取得
      // ***************************************************************************************
      {
         XMLPATHARRAY XmlPathArray[] = { 
                                          { L"COLLADA", NULL, NULL },
                                          { L"library_geometries", NULL, NULL },
                                          { L"geometry", L"id", polygons.InstanceGeometryName.data() },
                                          { L"mesh", NULL, NULL },
                                          { L"source", L"id", source.data() },
                                          { L"float_array", NULL, NULL },
                                       };
         float_array = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
      }
      if( float_array.size() != 1 )
         throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "float_array要素の数が不正です。", "" ) );

      // ' 'と'\n' でばらす
      std::vector<float> VectorArray;
      {
         std::vector<std::wstring> temp1, temp2;
         Split( &float_array, ' ', &temp1 );
         Split( &temp1, '\n', &temp2 );
         for( UINT j=0; j<temp2.size(); j++ )
            VectorArray.push_back( static_cast<float>( _wtof( temp2[j].data() ) ) );
      }

      element.Semantic = inputSemantic[i];

      size_t veccount = VectorArray.size();
      DATAARRAY::VERTEX_ELEMENT::VECTOR VectorElement;

      // ***************************************************************************************
      // stride取得
      // ***************************************************************************************
      {
         XMLPATHARRAY XmlPathArray[] = { 
                                          { L"COLLADA", NULL, NULL },
                                          { L"library_geometries", NULL, NULL },
                                          { L"geometry", L"id", polygons.InstanceGeometryName.data() },
                                          { L"mesh", NULL, NULL },
                                          { L"source", L"id", source.data() },
                                          { L"technique_common", NULL, NULL },
                                          { L"accessor", L"stride", NULL },
                                       };
         stride = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
      }
      if( stride.size() != 1 )
         throw( CException( "CColladaLoader::GetPolygons()でエラーが発生しました。", "stride要素の数が不正です。", "" ) );

      element.Stride = _wtoi( stride[0].data() );

      for( UINT i=0; i<veccount; i+=element.Stride )
      {
         for( UINT j=0; j<element.Stride; j++ )
            VectorElement.vector[j] = VectorArray[i+j];

         element.Vector.push_back( VectorElement );
      }
      
      // ***************************************************************************************
      // インデックスデータをセット
      // ***************************************************************************************
      for( UINT j=offset; j<uIndex.size(); j+=SemanticSize )
      {
         element.Index.push_back( uIndex[j] );
      }

      polygons.VertexElement.push_back( element );
   }

   // マテリアル取得
   GetMaterial(pInstanceGeometry, polygons);

   // データ格納
   m_DataArray.push_back( polygons );
}

// Polylists
void CColladaLoader::GetPolylists( const std::wstring* pInstanceGeometry )
{
	// 頭の#を取り除く
	std::wstring InstanceGemetryName = *pInstanceGeometry;
	GetID( &InstanceGemetryName );

	std::vector<std::wstring> polylists;
	{
		XMLPATHARRAY XmlPathArray[] =	{ 
											{ L"COLLADA", NULL, NULL },
											{ L"library_geometries", NULL, NULL },
											{ L"geometry", L"id", InstanceGemetryName.data() },
											{ L"mesh", NULL, NULL },
											{ L"polylist", L"material", NULL },
										};
		polylists = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
	}
	if( polylists.size() == 0 )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "polylistが見つかりません。", "" ) );

	for(hj_u32 u32i=0; u32i<polylists.size(); ++u32i){
		GetPolylist(pInstanceGeometry, &polylists[u32i]);
	}
}

// Polylist
void CColladaLoader::GetPolylist( const std::wstring* pInstanceGeometry, const std::wstring* pMaterialName )
{
	DATAARRAY polylist;

	polylist.InstanceGeometryName = *pInstanceGeometry;
	// 頭の#を取り除く
	GetID( &polylist.InstanceGeometryName );

   // ***************************************************************************************
   // ジオメトリ種別を設定
   // ***************************************************************************************
   polylist.GeometryType = GEOMETRY_TYPE_POLYLIST;

   // ***************************************************************************************
   // semanticを取得
   // ***************************************************************************************
   std::vector<std::wstring> inputSemantic;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
									   { L"polylist", L"material", pMaterialName->data() },
                                       { L"input", L"semantic", NULL },
                                    };
      inputSemantic = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSemantic.size() == 0 )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "semantic属性が見つかりません。", "" ) );

   // ***************************************************************************************
   // sourceを取得
   // ***************************************************************************************
   std::vector<std::wstring> inputSource;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polylist", L"material", pMaterialName->data() },
                                       { L"input", L"source", NULL },
                                    };
      inputSource = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSource.size() == 0 )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "source属性が見つかりません。", "" ) );

   // 頭の#を取り除く
   for( UINT i=0; i<inputSource.size(); i++ )
      GetID( &inputSource[i] );

   // ***************************************************************************************
   // offset を取得
   // ***************************************************************************************
   std::vector<std::wstring> inputOffset;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polylist", L"material", pMaterialName->data() },
                                       { L"input", L"offset", NULL },
                                    };
      inputOffset = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSemantic.size() == 0 )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "offset属性が見つかりません。", "" ) );

   if( inputSemantic.size() != inputSource.size() )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "semanticとsourceの要素数が一致しません。", "" ) );

   if( inputSemantic.size() != inputOffset.size() )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "semanticとoffsetの要素数が一致しません。", "" ) );

   // ***************************************************************************************
   // vcountデータを daeファイル から取得
   // ***************************************************************************************
   std::vector<std::wstring> vcount;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polylist", L"material", pMaterialName->data() },
                                       { L"vcount", NULL, NULL },
                                    };
      vcount = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( vcount.size() != 1 )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "vcountの要素数が不正です。", "" ) );

   // ' ' でばらす
   {
      std::vector<std::wstring> temp1, temp2;
      Split( &vcount, ' ', &temp1 );
      Split( &temp1, '\n', &temp2 );
      for( UINT i=0; i<temp2.size(); i++ )
         polylist.VCount.push_back( _wtoi( temp2[i].data() ) );
   }

   // ***************************************************************************************
   // pデータを daeファイル から取得
   // ***************************************************************************************
   std::vector<std::wstring> p;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"polylist", L"material", pMaterialName->data() },
                                       { L"p", NULL, NULL },
                                    };
      p = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( p.size() != 1 )
      throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "pの要素数が不正です。", "" ) );

   // ' 'と'\n' でばらす
   std::vector<UINT> uIndex;
   {
      std::vector<std::wstring> temp1, temp2;
      Split( &p, ' ', &temp1 );
      Split( &temp1, '\n', &temp2 );
      for( UINT i=0; i<temp2.size(); i++ )
         uIndex.push_back( _wtoi( temp2[i].data() ) );
   }
   
   UINT SemanticSize = inputSemantic.size();
   
   // ***************************************************************************************
   // 頂点エレメントを取得
   // ***************************************************************************************
   for( UINT i=0; i<SemanticSize; i++ )
   {
      std::wstring source;
      UINT offset;
      std::vector<std::wstring> float_array, stride;
      DATAARRAY::VERTEX_ELEMENT element;

      // source、offsetを取得
      if( GetInputData( &inputSemantic, &inputSource, &inputOffset, inputSemantic[i].data(), &source, &offset ) == false )
         throw( CException( "CColladaLoader::GetInputData()でエラーが発生しました。", "要素が見つかりません。", "" ) );

      // ***************************************************************************************
      // sourceの値を取得
      // ***************************************************************************************

      // "VERTEX"のときのsourceの内容を取得する
      if( wcscmp( inputSemantic[i].data(), L"VERTEX" ) == 0 )
      {
         std::vector<std::wstring> sourceArray;
         {
            source = inputSource[i];
            GetID( &source );

            XMLPATHARRAY XmlPathArray[] = { 
                                             { L"COLLADA", NULL, NULL },
                                             { L"library_geometries", NULL, NULL },
                                             { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                             { L"mesh", NULL, NULL },
                                             { L"vertices", L"id", source.data() },
                                             { L"input", L"source", NULL },    // ここはsemantic="POSITION"のみだという前提
                                          };
            sourceArray = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
         }
         if( sourceArray.size() != 1 )
            throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "vertices要素のinput要素の数が不正です。", "" ) );

         source = sourceArray[0];
         GetID( &source );
      }
      // "VERTEX"以外のときのsourceの内容を取得する
      else
      {
         source = inputSource[i];
         GetID( &source );
      }

      // ***************************************************************************************
      // データ配列を取得
      // ***************************************************************************************
      {
         XMLPATHARRAY XmlPathArray[] = { 
                                          { L"COLLADA", NULL, NULL },
                                          { L"library_geometries", NULL, NULL },
                                          { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                          { L"mesh", NULL, NULL },
                                          { L"source", L"id", source.data() },
                                          { L"float_array", NULL, NULL },
                                       };
         float_array = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
      }
      if( float_array.size() != 1 )
         throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "float_array要素の数が不正です。", "" ) );

      // ' 'と'\n' でばらす
      std::vector<float> VectorArray;
      {
         std::vector<std::wstring> temp1, temp2;
         Split( &float_array, ' ', &temp1 );
         Split( &temp1, '\n', &temp2 );
         for( UINT j=0; j<temp2.size(); j++ )
            VectorArray.push_back( static_cast<float>( _wtof( temp2[j].data() ) ) );
      }

      element.Semantic = inputSemantic[i];

      size_t veccount = VectorArray.size();
      DATAARRAY::VERTEX_ELEMENT::VECTOR VectorElement;

      // ***************************************************************************************
      // stride取得
      // ***************************************************************************************
      {
         XMLPATHARRAY XmlPathArray[] = { 
                                          { L"COLLADA", NULL, NULL },
                                          { L"library_geometries", NULL, NULL },
                                          { L"geometry", L"id", polylist.InstanceGeometryName.data() },
                                          { L"mesh", NULL, NULL },
                                          { L"source", L"id", source.data() },
                                          { L"technique_common", NULL, NULL },
                                          { L"accessor", L"stride", NULL },
                                       };
         stride = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
      }
      if( stride.size() != 1 )
         throw( CException( "CColladaLoader::GetPolylist()でエラーが発生しました。", "stride要素の数が不正です。", "" ) );

      element.Stride = _wtoi( stride[0].data() );

      for( UINT i=0; i<veccount; i+=element.Stride )
      {
         for( UINT j=0; j<element.Stride; j++ )
            VectorElement.vector[j] = VectorArray[i+j];

         element.Vector.push_back( VectorElement );
      }
      
      // ***************************************************************************************
      // インデックスデータをセット
      // ***************************************************************************************
      for( UINT j=offset; j<uIndex.size(); j+=SemanticSize )
      {
         element.Index.push_back( uIndex[j] );
      }

      polylist.VertexElement.push_back( element );
   }

   // マテリアル取得
//   GetMaterial(pInstanceGeometry, polylist);

   // データ格納
   m_DataArray.push_back( polylist );
}

// Trianglesデータを取得
void CColladaLoader::GetTriangles( const std::wstring* pInstanceGeometry )
{
   DATAARRAY triangles;

   triangles.InstanceGeometryName = *pInstanceGeometry;
   // 頭の#を取り除く
   GetID( &triangles.InstanceGeometryName );

   // ***************************************************************************************
   // ジオメトリ種別を設定
   // ***************************************************************************************
   triangles.GeometryType = GEOMETRY_TYPE_TRIANGLES;

   // ***************************************************************************************
   // semanticを取得
   // ***************************************************************************************
   std::vector<std::wstring> inputSemantic;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", triangles.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"triangles", NULL, NULL },
                                       { L"input", L"semantic", NULL },
                                    };
      inputSemantic = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSemantic.size() == 0 )
      throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "semantic属性が見つかりません。", "" ) );

   // ***************************************************************************************
   // sourceを取得
   // ***************************************************************************************
   std::vector<std::wstring> inputSource;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", triangles.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"triangles", NULL, NULL },
                                       { L"input", L"source", NULL },
                                    };
      inputSource = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSource.size() == 0 )
      throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "source属性が見つかりません。", "" ) );

   // 頭の#を取り除く
   for( UINT i=0; i<inputSource.size(); i++ )
      GetID( &inputSource[i] );

   // ***************************************************************************************
   // offset を取得
   // ***************************************************************************************
   std::vector<std::wstring> inputOffset;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", triangles.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"triangles", NULL, NULL },
                                       { L"input", L"offset", NULL },
                                    };
      inputOffset = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( inputSemantic.size() == 0 )
      throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "offset属性が見つかりません。", "" ) );

   if( inputSemantic.size() != inputSource.size() )
      throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "semanticとsourceの要素数が一致しません。", "" ) );

   if( inputSemantic.size() != inputOffset.size() )
      throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "semanticとoffsetの要素数が一致しません。", "" ) );

   // ***************************************************************************************
   // pデータを daeファイル から取得
   // ***************************************************************************************
   std::vector<std::wstring> p;
   {
      XMLPATHARRAY XmlPathArray[] = { 
                                       { L"COLLADA", NULL, NULL },
                                       { L"library_geometries", NULL, NULL },
                                       { L"geometry", L"id", triangles.InstanceGeometryName.data() },
                                       { L"mesh", NULL, NULL },
                                       { L"triangles", NULL, NULL },
                                       { L"p", NULL, NULL },
                                    };
      p = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
   }
   if( p.size() != 1 )
      throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "pの要素数が不正です。", "" ) );

   // ' 'と'\n' でばらす
   std::vector<UINT> uIndex;
   {
      std::vector<std::wstring> temp1, temp2;
      Split( &p, ' ', &temp1 );
      Split( &temp1, '\n', &temp2 );
      for( UINT i=0; i<temp2.size(); i++ )
         uIndex.push_back( _wtoi( temp2[i].data() ) );
   }
   
   UINT SemanticSize = inputSemantic.size();
   
   // ***************************************************************************************
   // 頂点エレメントを取得
   // ***************************************************************************************
   for( UINT i=0; i<SemanticSize; i++ )
   {
      std::wstring source;
      UINT offset;
      std::vector<std::wstring> float_array, stride;
      DATAARRAY::VERTEX_ELEMENT element;

      // source、offsetを取得
      if( GetInputData( &inputSemantic, &inputSource, &inputOffset, inputSemantic[i].data(), &source, &offset ) == false )
         throw( CException( "CColladaLoader::GetInputData()でエラーが発生しました。", "要素が見つかりません。", "" ) );

      // ***************************************************************************************
      // sourceの値を取得
      // ***************************************************************************************

      // "VERTEX"のときのsourceの内容を取得する
      if( wcscmp( inputSemantic[i].data(), L"VERTEX" ) == 0 )
      {
         std::vector<std::wstring> sourceArray;
         {
            source = inputSource[i];
            GetID( &source );

            XMLPATHARRAY XmlPathArray[] = { 
                                             { L"COLLADA", NULL, NULL },
                                             { L"library_geometries", NULL, NULL },
                                             { L"geometry", L"id", triangles.InstanceGeometryName.data() },
                                             { L"mesh", NULL, NULL },
                                             { L"vertices", L"id", source.data() },
                                             { L"input", L"source", NULL },    // ここはsemantic="POSITION"のみだという前提
                                          };
            sourceArray = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
         }
         if( sourceArray.size() != 1 )
            throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "vertices要素のinput要素の数が不正です。", "" ) );

         source = sourceArray[0];
         GetID( &source );
      }
      // "VERTEX"以外のときのsourceの内容を取得する
      else
      {
         source = inputSource[i];
         GetID( &source );
      }

      // ***************************************************************************************
      // データ配列を取得
      // ***************************************************************************************
      {
         XMLPATHARRAY XmlPathArray[] = { 
                                          { L"COLLADA", NULL, NULL },
                                          { L"library_geometries", NULL, NULL },
                                          { L"geometry", L"id", triangles.InstanceGeometryName.data() },
                                          { L"mesh", NULL, NULL },
                                          { L"source", L"id", source.data() },
                                          { L"float_array", NULL, NULL },
                                       };
         float_array = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
      }
      if( float_array.size() != 1 )
         throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "float_array要素の数が不正です。", "" ) );

      // ' 'と'\n' でばらす
      std::vector<float> VectorArray;
      {
         std::vector<std::wstring> temp1, temp2;
         Split( &float_array, ' ', &temp1 );
         Split( &temp1, '\n', &temp2 );
         for( UINT j=0; j<temp2.size(); j++ )
            VectorArray.push_back( static_cast<float>( _wtof( temp2[j].data() ) ) );
      }

      element.Semantic = inputSemantic[i];

      size_t veccount = VectorArray.size();
      DATAARRAY::VERTEX_ELEMENT::VECTOR VectorElement;

      // ***************************************************************************************
      // stride取得
      // ***************************************************************************************
      {
         XMLPATHARRAY XmlPathArray[] = { 
                                          { L"COLLADA", NULL, NULL },
                                          { L"library_geometries", NULL, NULL },
                                          { L"geometry", L"id", triangles.InstanceGeometryName.data() },
                                          { L"mesh", NULL, NULL },
                                          { L"source", L"id", source.data() },
                                          { L"technique_common", NULL, NULL },
                                          { L"accessor", L"stride", NULL },
                                       };
         stride = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
      }
      if( stride.size() != 1 )
         throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "stride要素の数が不正です。", "" ) );

      element.Stride = _wtoi( stride[0].data() );

      for( UINT i=0; i<veccount; i+=element.Stride )
      {
         for( UINT j=0; j<element.Stride; j++ )
            VectorElement.vector[j] = VectorArray[i+j];

         element.Vector.push_back( VectorElement );
      }
      
      // ***************************************************************************************
      // インデックスデータをセット
      // ***************************************************************************************
      for( UINT j=offset; j<uIndex.size(); j+=SemanticSize )
      {
         element.Index.push_back( uIndex[j] );
      }

      triangles.VertexElement.push_back( element );
   }

   // マテリアル取得
   GetMaterial(pInstanceGeometry, triangles);

   // データ格納
   m_DataArray.push_back( triangles );
}


// Materialデータ取得
void CColladaLoader::GetMaterial( const std::wstring* pInstanceGeometry, DATAARRAY& rsDataArray )
{
	// ***************************************************************************************
	// マテリアルインスタンス名を取得
	// ***************************************************************************************
	std::vector<std::wstring> instance_material;
	{
		XMLPATHARRAY XmlPathArray[] = { 
									   { L"COLLADA", NULL, NULL },
									   { L"library_visual_scenes", NULL, NULL },
									   { L"visual_scene", NULL, NULL },
									   { L"node", NULL, NULL },
//									   { L"node", NULL, NULL },
									   { L"instance_geometry", L"url", pInstanceGeometry->data() },
									   { L"bind_material", NULL, NULL },
									   { L"technique_common", NULL, NULL },
									   { L"instance_material", L"target", NULL },
									};
		instance_material = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
	}
	// とりあえず１個のみとする
	if( instance_material.size() != 1 )
		throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "instance_material要素の数が不正です。", "" ) );

	rsDataArray.InstanceMaterialName = instance_material[0];

	// 頭の#を取り除く
	GetID( &instance_material[0] );

	// ***************************************************************************************
	// インスタンスエフェクト名を取得
	// ***************************************************************************************
	std::vector<std::wstring> instance_effect;
	{
	  XMLPATHARRAY XmlPathArray[] = { 
									   { L"COLLADA", NULL, NULL },
									   { L"library_materials", NULL, NULL },
									   { L"material", L"id", instance_material[0].data() },
									   { L"instance_effect", L"url", NULL },
									};
	  instance_effect = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
	}
	// とりあえず１個のみとする
	if( instance_effect.size() != 1 )
	  throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "instance_effect要素の数が不正です。", "" ) );

	// 頭の#を取り除く
	GetID( &instance_effect[0] );

	// ***************************************************************************************
	// ライブラリエフェクトからテクスチャーIDを取得
	// ***************************************************************************************
	static const hj_wc16* towc16ShaderName[] = {
		L"phong",
		L"lambert",
	};
	static const hj_wc16* towc16MaterialName[] = {
		L"emission",
		L"ambient",
		L"diffuse",
	};
	static const hj_s32 tos32ShaderNameNum = HJ_ARRAY_NUM(towc16ShaderName);
	static const hj_s32 tos32MaterialNameNum = HJ_ARRAY_NUM(towc16MaterialName);

	std::vector<std::wstring> library_effects_texture;
	hj_bool bTexture = false;
	for(hj_s32 s32i=0; s32i<tos32ShaderNameNum; ++s32i){
		for(hj_s32 s32i=0; s32i<tos32ShaderNameNum; ++s32i){
			library_effects_texture.clear();
			XMLPATHARRAY XmlPathArray[] = {
									{ L"COLLADA", NULL, NULL },
									{ L"library_effects", NULL, NULL },
									{ L"effect", L"id", instance_effect[0].data() },
									{ L"profile_COMMON", NULL, NULL },
									{ L"technique", NULL, NULL },
//										{ L"phong", NULL, NULL },						// とりあえず phong 固定
//										{ L"diffuse", NULL, NULL },						// diffuse のテクスチャーを対象とする
									{ towc16ShaderName[s32i], NULL, NULL },
									{ towc16MaterialName[s32i], NULL, NULL },
									{ L"texture", L"texture", NULL },
								};
			library_effects_texture = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
			if( library_effects_texture.size() != 0 ){
				bTexture = true;
				break;
			}
		}
		if(bTexture){
			break;
		}
	}
	
//	if( !bTexture ){
//		throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "library_effectsからテクスチャーIDを取得できません", "" ) );
//	}
	
	if(bTexture)
	{
		// ***************************************************************************************
		// ライブラリイメージ名を取得
		// ***************************************************************************************
		std::vector<std::wstring> library_images;
		{
		  XMLPATHARRAY XmlPathArray[] = { 
										   { L"COLLADA", NULL, NULL },
										   { L"library_images", NULL, NULL },
										   { L"image", L"id", library_effects_texture[0].data() },
										   { L"init_from", NULL, NULL },
										};
		  library_images = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
		}
		// とりあえず１個のみとする
		if( library_images.size() != 1 )
		  throw( CException( "CColladaLoader::GetTriangles()でエラーが発生しました。", "library_images要素の数が不正です。", "" ) );

		// ***************************************************************************************
		// テクスチャーファイル名を格納
		// ***************************************************************************************
		rsDataArray.ImageFileName = library_images[0];
	}
}

void CColladaLoader::LoadMesh( const TCHAR* pFileName )
{
   const std::wstring Version = L"1.4.1";
//   const std::wstring Version = L"1.4.0";

   // メモリ開放
   Invalidate();

   if( FAILED( ::CreateXmlReader( __uuidof(IXmlReader), reinterpret_cast<void**>(&m_pReader), 0 ) ) )
      throw( CException( "CColladaLoader::LoadMesh()でエラーが発生しました。", "xmlファイルの読み込みに失敗しました。", "" ) );

   if( FAILED( ::SHCreateStreamOnFile( pFileName, STGM_READ, &m_pStream ) ) )
      throw( CException( "CColladaLoader::LoadMesh()でエラーが発生しました。", "ファイルが見つかりません。", "" ) );

   if( FAILED( m_pReader->SetInput( m_pStream ) ) )
      throw( CException( "CColladaLoader::LoadMesh()でエラーが発生しました。", "IXmlReader::SetInput()でエラーが発生しました。", "" ) );

   	// ***************************************************************************************
	// バージョンチェック
	// ***************************************************************************************
	std::vector<std::wstring> VersionData;
	{
		XMLPATHARRAY XmlPathArray[] =	{ 
										  { L"COLLADA", L"version", NULL },
										};
		VersionData = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
	}

	if( VersionData.empty() == true )
		throw( CException( "CColladaLoader::LoadMesh()でエラーが発生しました。", "COLLADAのバージョンを取得できません。", "" ) );

	if( wcscmp( VersionData[0].data(), Version.data() ) != 0 )
		throw( CException( "CColladaLoader::LoadMesh()でエラーが発生しました。", "COLLADAのバージョンが不正です。", "" ) );

	// ***************************************************************************************
	// ジオメトリインスタンス名を取得
	// ***************************************************************************************
	std::vector<std::wstring> instance_geometry;
	{
		XMLPATHARRAY XmlPathArray[] = { 
									  { L"COLLADA", NULL, NULL },
									  { L"library_visual_scenes", NULL, NULL },
									  { L"visual_scene", NULL, NULL },
//									  { L"node", NULL, NULL },
									  { L"node", NULL, NULL },
									  { L"instance_geometry", L"url", NULL },
								   };
		instance_geometry = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
	}

	// ***************************************************************************************
	// コントローラーインスタンス名を取得
	// ***************************************************************************************
	std::vector<std::wstring> instance_controller;
	{
		XMLPATHARRAY XmlPathArray[] = { 
									  { L"COLLADA", NULL, NULL },
									  { L"library_visual_scenes", NULL, NULL },
									  { L"visual_scene", NULL, NULL },
									  { L"node", NULL, NULL },
									  { L"instance_controller", L"url", NULL },
								   };
		instance_controller = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
	}
	if(instance_controller.size() > 0){
		std::vector<std::wstring> tmp;
		for( UINT i=0; i<instance_controller.size(); i++ ){
			Split( &instance_controller, '-', &tmp );
			instance_geometry.push_back(tmp[0]);
		};
	}

/*
	// ***************************************************************************************
	// コントローラー情報を取得
	// ***************************************************************************************
	try{
		for( UINT i=0; i<instance_controller.size(); i++ )
		{
			int type = CheckGeometryType( &instance_controller[i] );

			switch( type ){
			case GEOMETRY_TYPE_POLYLIST:
				GetPolylists( &instance_controller[i] );
				break;
			case GEOMETRY_TYPE_TRIANGLES:
				GetTriangles( &instance_controller[i] );
				break;
			default:
				throw( CException( "CColladaLoader::LoadMesh()でエラーが発生しました。", "未対応のジオメトリタイプが指定されました。", "" ) );
				break;
			};
		}
   }
   catch( CException ex )
   {
      throw( ex );
   }
*/

	// ***************************************************************************************
	// ジオメトリ情報を取得
	// ***************************************************************************************
	try{
		for( UINT i=0; i<instance_geometry.size(); i++ )
		{
			int type = CheckGeometryType( &instance_geometry[i] );

			switch( type )
			{
			case GEOMETRY_TYPE_POLYGONS:
				GetPolygons( &instance_geometry[i] );
				break;
			case GEOMETRY_TYPE_POLYLIST:
				GetPolylists( &instance_geometry[i] );
				break;
			case GEOMETRY_TYPE_TRIANGLES:
				GetTriangles( &instance_geometry[i] );
				break;
			default:
				throw( CException( "CColladaLoader::LoadMesh()でエラーが発生しました。", "未対応のジオメトリタイプが指定されました。", "" ) );
				break;
			};
		}
	}
	catch( CException ex ){
      throw( ex );
	}


}