#include <tchar.h>
#include <vector>
#include <string>
#include <shlwapi.h>
#include "Exception.h"
#include "ColladaLoader.h"

// ****************************************************************************************************************
// COLLADA �\ Digital Asset Schema �����[�X1.4.1
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

// xml ��ǂݍ��ފJ�n�ʒu��擪�Ɉړ�����
void CColladaLoader::SetSeekTop() const
{
   LARGE_INTEGER li = { 0, 0 };
   if( FAILED( m_pStream->Seek( li, tagSTREAM_SEEK::STREAM_SEEK_SET, NULL ) ) )
      throw( CException( "CColladaLoader::SetSeekTop()�ŃG���[���������܂����B", "IStream::Seek()�ŃG���[���������܂����B", "" ) );

   if( FAILED( m_pReader->SetInput( m_pStream ) ) )
      throw( CException( "CColladaLoader::SetSeekTop()�ŃG���[���������܂����B", "IxmlReader::SetInput()�ŃG���[���������܂����B", "" ) );
}

// �����l���擾����
const bool CColladaLoader::GetAttributeValue( const LPCWSTR pAttribute, std::wstring* pValue ) const
{
   bool Find = false;
   HRESULT hr;
   LPCWSTR pwszValue = NULL;

   pValue->clear();

   // ����������
   hr = m_pReader->MoveToAttributeByName( pAttribute, NULL );
   switch( hr )
   {
   // �����Ȃ�
   case S_FALSE:
      goto EXIT;
      break;
   case E_FAIL:
      throw( CException( "CColladaLoader::GetAttributeValue()�ŃG���[���������܂����B", "IXmlReader::MoveToAttributeByName()�ŃG���[���������܂����B", "" ) );
      break;
   }

   if( pValue != NULL )
   {
      // �����l���擾
      if( FAILED( m_pReader->GetValue( &pwszValue, NULL ) ) )
         throw( CException( "CColladaLoader::GetAttributeValue()�ŃG���[���������܂����B", "IXmlReader::GetValue()�ŃG���[���������܂����B", "" ) );

      pValue->append( pwszValue );
   }

   Find = true;

EXIT:
   return Find;
}

// �e�L�X�g���擾����
const bool CColladaLoader::GetText( std::wstring* pValue ) const
{
   bool Find = false;
   LPCWSTR pwszValue = NULL;
   XmlNodeType nodeType;
   pValue->clear();

   // �ǂݍ���
   while( m_pReader->Read( &nodeType ) == S_OK )
   {
      switch( nodeType )
      {
      case XmlNodeType::XmlNodeType_Text:
         // �l���擾����
         if( FAILED( m_pReader->GetValue( &pwszValue, NULL ) ) )
            throw( CException( "CColladaLoader::GetText()�ŃG���[���������܂����B", "IXmlReader::GetValue()�ŃG���[���������܂����B", "" ) );

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
      // ���������ɑ������w�肳��Ă��Ȃ�
      if( pXmlPath->pAttribute == NULL )
      {
         Find = true;
         goto EXIT;
      }

      // ���������ɑ������w�肳��Ă���
      else
      {
         // �������Ō������Ă݂�����
         if( GetAttributeValue( pXmlPath->pAttribute, &pValue ) == true )
         {
            // ���������ɑ����l������
            if( pXmlPath->pAttributeValue != NULL )
            {
               // �����l����v����
               if( wcscmp( pXmlPath->pAttributeValue, pValue.data() ) == 0 )
               {
                  Find = true;
                  goto EXIT;
               }
            }
            // ���������ɑ����l���Ȃ�
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

// �m�[�h����
const std::vector<std::wstring> CColladaLoader::GetElementsByNodeName( const XMLPATHARRAY* pXmlPath, UINT XmlPathArrayCount ) const
{
   std::vector<std::wstring> RetValue;

   XmlNodeType nodeType;
   LPCWSTR pwszLocalName = NULL;
   std::wstring pValue;
   UINT Depth = 0;
   int Level = -1;

   // �V�[�N��擪�Ɉړ�
   SetSeekTop();

   // �ǂݍ���
   while( m_pReader->Read( &nodeType ) == S_OK )
   {
      switch( nodeType )
      {
      case XmlNodeType::XmlNodeType_Element:
         // �v�f�����擾
         if( FAILED( m_pReader->GetLocalName( &pwszLocalName, NULL ) ) )
            throw( CException( "CColladaLoader::getElementsByNodeName()�ŃG���[���������܂����B", "IXmlReader::GetLocalName()�ŃG���[���������܂����B", "" ) );

         // �K�w���擾
         if( FAILED( m_pReader->GetDepth( &Depth ) ) )
            throw( CException( "CColladaLoader::getElementsByNodeName()�ŃG���[���������܂����B", "IXmlReader::GetDepth()�ŃG���[���������܂����B", "" ) );

         // �K�w��XMLPATHARRAY�̗v�f�����z���Ă���ꍇ�́A�m���ɑΏۊO�Ȃ̂ŃX�L�b�v����
         if( Depth >= XmlPathArrayCount )
            continue;

         // �������Ă݂������ꍇ
         if( FindTagOrAttribute( &pXmlPath[Depth], pwszLocalName ) == true )
         {
            // ���x������̂ق��ɂ���Ƃ��͂��ǂ�
            if( Level + 1 >= static_cast<int>( Depth ) )
            {
               Level = static_cast<int>( Depth );  // ���x���A�b�v

               // �����������Ō�̊K�w�̂Ƃ�
               if( Depth + 1 == XmlPathArrayCount )
               {
                  // �e�L�X�g���o�͂���ꍇ 
                  if( pXmlPath[Depth].pAttribute == NULL ||                                              // ���������ɑ��������w�肳��Ă��Ȃ��Ƃ�
                      pXmlPath[Depth].pAttribute != NULL && pXmlPath[Depth].pAttributeValue != NULL ) // ���������ɑ������Ƒ����l���w�肳��Ă���Ƃ�
                  {
                     // �e�L�X�g���擾����
                     if( GetText( &pValue ) == true )
                     {
                        // �l���o�͂���
                        RetValue.push_back( pValue.data() );
                     }
                  }
                  // �����l���o�͂���ꍇ
                  else if( pXmlPath[Depth].pAttribute != NULL && pXmlPath[Depth].pAttributeValue == NULL )
                  {
                     if( GetAttributeValue( pXmlPath->pAttribute, &pValue ) == true )
                     {
                        RetValue.push_back( pValue.data() );
                     }
                  }
                  else
                     throw( CException( "CColladaLoader::getElementsByNodeName()�ŃG���[���������܂����B", "pXmlPath�̌����������s���ł��B", "" ) );
               }
            }
         }
         // �����Ώۂ̗v�f����������Ȃ�
         else
         {
            if( Level >= static_cast<int>( Depth ) )
               Level =  static_cast<int>( Depth ) - 1;   // ���x���_�E��
         }
         break;
      }
   }

   return RetValue;
}

// target �� url �����l�̓���#����菜��
void CColladaLoader::GetID( std::wstring* Target ) const
{
   if( Target->length() > 0 )
   {
      if( Target->find( '#', 0 ) == 0 )
         Target->erase( 0, 1 );
   }
}

//������𕪊�����
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
   // ����#����菜��
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


// input�^�O�̑����l���擾
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
   // ����#����菜��
   GetID( &polygons.InstanceGeometryName );

   // ***************************************************************************************
   // �W�I���g����ʂ�ݒ�
   // ***************************************************************************************
   polygons.GeometryType = GEOMETRY_TYPE_POLYGONS;

   // ***************************************************************************************
   // �|���S����
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
			throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "polygons��count�����s���ł��B", "" ) );
		}
		s32PolygonNum = _wtoi(polygonNum[0].c_str());
	}

   // ***************************************************************************************
   // semantic���擾
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
      throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "semantic������������܂���B", "" ) );

   // ***************************************************************************************
   // source���擾
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
      throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "source������������܂���B", "" ) );

   // ����#����菜��
   for( UINT i=0; i<inputSource.size(); i++ )
      GetID( &inputSource[i] );

   // ***************************************************************************************
   // offset ���擾
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
      throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "offset������������܂���B", "" ) );

   if( inputSemantic.size() != inputSource.size() )
      throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "semantic��source�̗v�f������v���܂���B", "" ) );

   if( inputSemantic.size() != inputOffset.size() )
      throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "semantic��offset�̗v�f������v���܂���B", "" ) );

   // ***************************************************************************************
   // p�f�[�^�� dae�t�@�C�� ����擾
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
      throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "p�̗v�f�����s���ł��B", "" ) );

   // vcount���g����polylist�Ɠ����\����
   std::vector<hj_u32> vcount;

   // ' '��'\n' �ł΂炷
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
   // ���_�G�������g���擾
   // ***************************************************************************************
   for( UINT i=0; i<SemanticSize; i++ )
   {
      std::wstring source;
      UINT offset;
      std::vector<std::wstring> float_array, stride;
      DATAARRAY::VERTEX_ELEMENT element;

      // source�Aoffset���擾
      if( GetInputData( &inputSemantic, &inputSource, &inputOffset, inputSemantic[i].data(), &source, &offset ) == false )
         throw( CException( "CColladaLoader::GetInputData()�ŃG���[���������܂����B", "�v�f��������܂���B", "" ) );

      // ***************************************************************************************
      // source�̒l���擾
      // ***************************************************************************************

      // "VERTEX"�̂Ƃ���source�̓��e���擾����
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
                                             { L"input", L"source", NULL },    // ������semantic="POSITION"�݂̂��Ƃ����O��
                                          };
            sourceArray = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
         }
         if( sourceArray.size() != 1 )
            throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "vertices�v�f��input�v�f�̐����s���ł��B", "" ) );

         source = sourceArray[0];
         GetID( &source );
      }
      // "VERTEX"�ȊO�̂Ƃ���source�̓��e���擾����
      else
      {
         source = inputSource[i];
         GetID( &source );
      }

      // ***************************************************************************************
      // �f�[�^�z����擾
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
         throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "float_array�v�f�̐����s���ł��B", "" ) );

      // ' '��'\n' �ł΂炷
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
      // stride�擾
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
         throw( CException( "CColladaLoader::GetPolygons()�ŃG���[���������܂����B", "stride�v�f�̐����s���ł��B", "" ) );

      element.Stride = _wtoi( stride[0].data() );

      for( UINT i=0; i<veccount; i+=element.Stride )
      {
         for( UINT j=0; j<element.Stride; j++ )
            VectorElement.vector[j] = VectorArray[i+j];

         element.Vector.push_back( VectorElement );
      }
      
      // ***************************************************************************************
      // �C���f�b�N�X�f�[�^���Z�b�g
      // ***************************************************************************************
      for( UINT j=offset; j<uIndex.size(); j+=SemanticSize )
      {
         element.Index.push_back( uIndex[j] );
      }

      polygons.VertexElement.push_back( element );
   }

   // �}�e���A���擾
   GetMaterial(pInstanceGeometry, polygons);

   // �f�[�^�i�[
   m_DataArray.push_back( polygons );
}

// Polylists
void CColladaLoader::GetPolylists( const std::wstring* pInstanceGeometry )
{
	// ����#����菜��
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
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "polylist��������܂���B", "" ) );

	for(hj_u32 u32i=0; u32i<polylists.size(); ++u32i){
		GetPolylist(pInstanceGeometry, &polylists[u32i]);
	}
}

// Polylist
void CColladaLoader::GetPolylist( const std::wstring* pInstanceGeometry, const std::wstring* pMaterialName )
{
	DATAARRAY polylist;

	polylist.InstanceGeometryName = *pInstanceGeometry;
	// ����#����菜��
	GetID( &polylist.InstanceGeometryName );

   // ***************************************************************************************
   // �W�I���g����ʂ�ݒ�
   // ***************************************************************************************
   polylist.GeometryType = GEOMETRY_TYPE_POLYLIST;

   // ***************************************************************************************
   // semantic���擾
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
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "semantic������������܂���B", "" ) );

   // ***************************************************************************************
   // source���擾
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
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "source������������܂���B", "" ) );

   // ����#����菜��
   for( UINT i=0; i<inputSource.size(); i++ )
      GetID( &inputSource[i] );

   // ***************************************************************************************
   // offset ���擾
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
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "offset������������܂���B", "" ) );

   if( inputSemantic.size() != inputSource.size() )
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "semantic��source�̗v�f������v���܂���B", "" ) );

   if( inputSemantic.size() != inputOffset.size() )
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "semantic��offset�̗v�f������v���܂���B", "" ) );

   // ***************************************************************************************
   // vcount�f�[�^�� dae�t�@�C�� ����擾
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
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "vcount�̗v�f�����s���ł��B", "" ) );

   // ' ' �ł΂炷
   {
      std::vector<std::wstring> temp1, temp2;
      Split( &vcount, ' ', &temp1 );
      Split( &temp1, '\n', &temp2 );
      for( UINT i=0; i<temp2.size(); i++ )
         polylist.VCount.push_back( _wtoi( temp2[i].data() ) );
   }

   // ***************************************************************************************
   // p�f�[�^�� dae�t�@�C�� ����擾
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
      throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "p�̗v�f�����s���ł��B", "" ) );

   // ' '��'\n' �ł΂炷
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
   // ���_�G�������g���擾
   // ***************************************************************************************
   for( UINT i=0; i<SemanticSize; i++ )
   {
      std::wstring source;
      UINT offset;
      std::vector<std::wstring> float_array, stride;
      DATAARRAY::VERTEX_ELEMENT element;

      // source�Aoffset���擾
      if( GetInputData( &inputSemantic, &inputSource, &inputOffset, inputSemantic[i].data(), &source, &offset ) == false )
         throw( CException( "CColladaLoader::GetInputData()�ŃG���[���������܂����B", "�v�f��������܂���B", "" ) );

      // ***************************************************************************************
      // source�̒l���擾
      // ***************************************************************************************

      // "VERTEX"�̂Ƃ���source�̓��e���擾����
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
                                             { L"input", L"source", NULL },    // ������semantic="POSITION"�݂̂��Ƃ����O��
                                          };
            sourceArray = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
         }
         if( sourceArray.size() != 1 )
            throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "vertices�v�f��input�v�f�̐����s���ł��B", "" ) );

         source = sourceArray[0];
         GetID( &source );
      }
      // "VERTEX"�ȊO�̂Ƃ���source�̓��e���擾����
      else
      {
         source = inputSource[i];
         GetID( &source );
      }

      // ***************************************************************************************
      // �f�[�^�z����擾
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
         throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "float_array�v�f�̐����s���ł��B", "" ) );

      // ' '��'\n' �ł΂炷
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
      // stride�擾
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
         throw( CException( "CColladaLoader::GetPolylist()�ŃG���[���������܂����B", "stride�v�f�̐����s���ł��B", "" ) );

      element.Stride = _wtoi( stride[0].data() );

      for( UINT i=0; i<veccount; i+=element.Stride )
      {
         for( UINT j=0; j<element.Stride; j++ )
            VectorElement.vector[j] = VectorArray[i+j];

         element.Vector.push_back( VectorElement );
      }
      
      // ***************************************************************************************
      // �C���f�b�N�X�f�[�^���Z�b�g
      // ***************************************************************************************
      for( UINT j=offset; j<uIndex.size(); j+=SemanticSize )
      {
         element.Index.push_back( uIndex[j] );
      }

      polylist.VertexElement.push_back( element );
   }

   // �}�e���A���擾
//   GetMaterial(pInstanceGeometry, polylist);

   // �f�[�^�i�[
   m_DataArray.push_back( polylist );
}

// Triangles�f�[�^���擾
void CColladaLoader::GetTriangles( const std::wstring* pInstanceGeometry )
{
   DATAARRAY triangles;

   triangles.InstanceGeometryName = *pInstanceGeometry;
   // ����#����菜��
   GetID( &triangles.InstanceGeometryName );

   // ***************************************************************************************
   // �W�I���g����ʂ�ݒ�
   // ***************************************************************************************
   triangles.GeometryType = GEOMETRY_TYPE_TRIANGLES;

   // ***************************************************************************************
   // semantic���擾
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
      throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "semantic������������܂���B", "" ) );

   // ***************************************************************************************
   // source���擾
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
      throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "source������������܂���B", "" ) );

   // ����#����菜��
   for( UINT i=0; i<inputSource.size(); i++ )
      GetID( &inputSource[i] );

   // ***************************************************************************************
   // offset ���擾
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
      throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "offset������������܂���B", "" ) );

   if( inputSemantic.size() != inputSource.size() )
      throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "semantic��source�̗v�f������v���܂���B", "" ) );

   if( inputSemantic.size() != inputOffset.size() )
      throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "semantic��offset�̗v�f������v���܂���B", "" ) );

   // ***************************************************************************************
   // p�f�[�^�� dae�t�@�C�� ����擾
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
      throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "p�̗v�f�����s���ł��B", "" ) );

   // ' '��'\n' �ł΂炷
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
   // ���_�G�������g���擾
   // ***************************************************************************************
   for( UINT i=0; i<SemanticSize; i++ )
   {
      std::wstring source;
      UINT offset;
      std::vector<std::wstring> float_array, stride;
      DATAARRAY::VERTEX_ELEMENT element;

      // source�Aoffset���擾
      if( GetInputData( &inputSemantic, &inputSource, &inputOffset, inputSemantic[i].data(), &source, &offset ) == false )
         throw( CException( "CColladaLoader::GetInputData()�ŃG���[���������܂����B", "�v�f��������܂���B", "" ) );

      // ***************************************************************************************
      // source�̒l���擾
      // ***************************************************************************************

      // "VERTEX"�̂Ƃ���source�̓��e���擾����
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
                                             { L"input", L"source", NULL },    // ������semantic="POSITION"�݂̂��Ƃ����O��
                                          };
            sourceArray = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
         }
         if( sourceArray.size() != 1 )
            throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "vertices�v�f��input�v�f�̐����s���ł��B", "" ) );

         source = sourceArray[0];
         GetID( &source );
      }
      // "VERTEX"�ȊO�̂Ƃ���source�̓��e���擾����
      else
      {
         source = inputSource[i];
         GetID( &source );
      }

      // ***************************************************************************************
      // �f�[�^�z����擾
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
         throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "float_array�v�f�̐����s���ł��B", "" ) );

      // ' '��'\n' �ł΂炷
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
      // stride�擾
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
         throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "stride�v�f�̐����s���ł��B", "" ) );

      element.Stride = _wtoi( stride[0].data() );

      for( UINT i=0; i<veccount; i+=element.Stride )
      {
         for( UINT j=0; j<element.Stride; j++ )
            VectorElement.vector[j] = VectorArray[i+j];

         element.Vector.push_back( VectorElement );
      }
      
      // ***************************************************************************************
      // �C���f�b�N�X�f�[�^���Z�b�g
      // ***************************************************************************************
      for( UINT j=offset; j<uIndex.size(); j+=SemanticSize )
      {
         element.Index.push_back( uIndex[j] );
      }

      triangles.VertexElement.push_back( element );
   }

   // �}�e���A���擾
   GetMaterial(pInstanceGeometry, triangles);

   // �f�[�^�i�[
   m_DataArray.push_back( triangles );
}


// Material�f�[�^�擾
void CColladaLoader::GetMaterial( const std::wstring* pInstanceGeometry, DATAARRAY& rsDataArray )
{
	// ***************************************************************************************
	// �}�e���A���C���X�^���X�����擾
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
	// �Ƃ肠�����P�݂̂Ƃ���
	if( instance_material.size() != 1 )
		throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "instance_material�v�f�̐����s���ł��B", "" ) );

	rsDataArray.InstanceMaterialName = instance_material[0];

	// ����#����菜��
	GetID( &instance_material[0] );

	// ***************************************************************************************
	// �C���X�^���X�G�t�F�N�g�����擾
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
	// �Ƃ肠�����P�݂̂Ƃ���
	if( instance_effect.size() != 1 )
	  throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "instance_effect�v�f�̐����s���ł��B", "" ) );

	// ����#����菜��
	GetID( &instance_effect[0] );

	// ***************************************************************************************
	// ���C�u�����G�t�F�N�g����e�N�X�`���[ID���擾
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
//										{ L"phong", NULL, NULL },						// �Ƃ肠���� phong �Œ�
//										{ L"diffuse", NULL, NULL },						// diffuse �̃e�N�X�`���[��ΏۂƂ���
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
//		throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "library_effects����e�N�X�`���[ID���擾�ł��܂���", "" ) );
//	}
	
	if(bTexture)
	{
		// ***************************************************************************************
		// ���C�u�����C���[�W�����擾
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
		// �Ƃ肠�����P�݂̂Ƃ���
		if( library_images.size() != 1 )
		  throw( CException( "CColladaLoader::GetTriangles()�ŃG���[���������܂����B", "library_images�v�f�̐����s���ł��B", "" ) );

		// ***************************************************************************************
		// �e�N�X�`���[�t�@�C�������i�[
		// ***************************************************************************************
		rsDataArray.ImageFileName = library_images[0];
	}
}

void CColladaLoader::LoadMesh( const TCHAR* pFileName )
{
   const std::wstring Version = L"1.4.1";
//   const std::wstring Version = L"1.4.0";

   // �������J��
   Invalidate();

   if( FAILED( ::CreateXmlReader( __uuidof(IXmlReader), reinterpret_cast<void**>(&m_pReader), 0 ) ) )
      throw( CException( "CColladaLoader::LoadMesh()�ŃG���[���������܂����B", "xml�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B", "" ) );

   if( FAILED( ::SHCreateStreamOnFile( pFileName, STGM_READ, &m_pStream ) ) )
      throw( CException( "CColladaLoader::LoadMesh()�ŃG���[���������܂����B", "�t�@�C����������܂���B", "" ) );

   if( FAILED( m_pReader->SetInput( m_pStream ) ) )
      throw( CException( "CColladaLoader::LoadMesh()�ŃG���[���������܂����B", "IXmlReader::SetInput()�ŃG���[���������܂����B", "" ) );

   	// ***************************************************************************************
	// �o�[�W�����`�F�b�N
	// ***************************************************************************************
	std::vector<std::wstring> VersionData;
	{
		XMLPATHARRAY XmlPathArray[] =	{ 
										  { L"COLLADA", L"version", NULL },
										};
		VersionData = GetElementsByNodeName( XmlPathArray, _countof( XmlPathArray ) );
	}

	if( VersionData.empty() == true )
		throw( CException( "CColladaLoader::LoadMesh()�ŃG���[���������܂����B", "COLLADA�̃o�[�W�������擾�ł��܂���B", "" ) );

	if( wcscmp( VersionData[0].data(), Version.data() ) != 0 )
		throw( CException( "CColladaLoader::LoadMesh()�ŃG���[���������܂����B", "COLLADA�̃o�[�W�������s���ł��B", "" ) );

	// ***************************************************************************************
	// �W�I���g���C���X�^���X�����擾
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
	// �R���g���[���[�C���X�^���X�����擾
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
	// �R���g���[���[�����擾
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
				throw( CException( "CColladaLoader::LoadMesh()�ŃG���[���������܂����B", "���Ή��̃W�I���g���^�C�v���w�肳��܂����B", "" ) );
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
	// �W�I���g�������擾
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
				throw( CException( "CColladaLoader::LoadMesh()�ŃG���[���������܂����B", "���Ή��̃W�I���g���^�C�v���w�肳��܂����B", "" ) );
				break;
			};
		}
	}
	catch( CException ex ){
      throw( ex );
	}


}