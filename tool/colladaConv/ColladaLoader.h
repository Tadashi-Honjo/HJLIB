#ifndef COLLADA_LOADER_H
#define COLLADA_LOADER_H

// ��W���̊g���@�\���g�p����Ă��܂�: enum 'enum' �����薼�Ŏg�p����܂��B���̃��[�j���O��}������B
#pragma warning(disable: 4482)

// XMLLite���g������
#include <xmllite.h>
#pragma comment(lib, "xmllite.lib")

#include "MeshLoader.h"

class CColladaLoader : public IMeshLoader
{
private:
   // IXmlReader�̓��������R�[�h��WCHAR���ۂ��̂ō��킹��
   typedef struct _XMLPATHARRAY
   {
      LPCWSTR pTagName;            // �^�O��
      LPCWSTR pAttribute;          // ������
      LPCWSTR pAttributeValue;     // �����l
   }XMLPATHARRAY;

   IXmlReader* m_pReader;
   IStream* m_pStream;

   // xml ��ǂݍ��ފJ�n�ʒu��擪�Ɉړ�����
   void SetSeekTop() const;

   // �����l���擾����
   const bool GetAttributeValue( const LPCWSTR pAttribute, std::wstring* pValue ) const;
   // �e�L�X�g���擾����
   const bool GetText( std::wstring* pValue ) const;

   // ���݂̊K�w�̃^�O������ё����l�����������ƈ�v���邩�𔻒肷��
   const bool FindTagOrAttribute( const XMLPATHARRAY* pXmlPath, const LPCWSTR pwszLocalName )const;

   // �m�[�h����
   const std::vector<std::wstring> GetElementsByNodeName( const XMLPATHARRAY* pXmlPath, UINT XmlPathArrayCount ) const;

   // target �� url �����l�̓���#����菜��
   void GetID( std::wstring* Target ) const;

   // ��������΂炷
   void Split( const std::vector<std::wstring>* pInStr, const wchar_t Delimiter, std::vector<std::wstring>* pOutStr, std::vector<hj_u32>* pOutSplitCount=NULL) const;

   // input�^�O�̑����l���擾
   const bool GetInputData( const std::vector<std::wstring>* pSemanticArray
                          , const std::vector<std::wstring>* pSourceArray
                          , const std::vector<std::wstring>* pOffsetArray
                          , const LPCWSTR pSearchSemantic
                          , std::wstring* pSource
                          , UINT* pOffset ) const;

   // �W�I���g���̎�ނ��擾
   const int CheckGeometryType( const std::wstring* pInstanceGeometry ) const;

   // Polygons�f�[�^���擾
   void GetPolygons( const std::wstring* pInstanceGeometry );
   // Polylist�f�[�^���擾
   void GetPolylists( const std::wstring* pInstanceGeometry );
   void GetPolylist( const std::wstring* pInstanceGeometry, const std::wstring* pMaterialName );

   // Triangles�f�[�^���擾
   void GetTriangles( const std::wstring* pInstanceGeometry );

   // Material�f�[�^
   void GetMaterial( const std::wstring* pInstanceGeometry, DATAARRAY& rsDataArray );

public:
   CColladaLoader();
   virtual ~CColladaLoader();
   void Invalidate();
   void LoadMesh( const TCHAR* pFileName );
};

#endif