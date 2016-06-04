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

	// Polygons�`���̒��_�G�������g�f�[�^���f�[�^�\���ɃR���o�[�g����B
	void PolygonsConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );
	// Polylist�`���̒��_�G�������g�f�[�^���f�[�^�\���ɃR���o�[�g����B
	void PolylistConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );
	// Triangles�`���̒��_�G�������g�f�[�^���f�[�^�\���ɃR���o�[�g����B
	void TrianglesConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );

	// �V�F�[�_�[���\�[�X�r���[�ƃT���v���[���쐬����
	void CreateMaterial( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh );

   // ���b�V�����[�_�[
   IMeshLoader* m_pMeshLoader;
};

#endif