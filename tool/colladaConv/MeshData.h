#ifndef MESHDATA_H
#define MESHDATA_H

#include <string>
#include <vector>

struct stcVector3{
	hj_f32 f32x, f32y, f32z;
};

// ���b�V�����N���X
class CMeshData
{
public:

   // ���_���\����
   typedef struct _VERTEX
   {
      stcVector3  Vertex; // ���_�f�[�^
      stcVector3  Normal; // �@���x�N�g��
      stcVector3  UV;     // �e�N�Z��
   }VERTEX;

   // �}�e���A���\����
   typedef struct _MATERIAL
   {
      std::wstring InstanceMaterialName;  // �e�N�X�`���[�̎�ʖ�
      std::wstring ImageFileName;         // ���s�t�@�C������e�N�X�`���[�܂ł̑��΃p�X
   }MATERIAL;

   typedef struct _MESH
   {
      std::wstring InstanceGeometryName;// ���b�V����
	  std::vector<VERTEX> pVertex;      // ���_�G�������g�̃f�[�^
      std::vector<UINT> pIndex;			// �C���f�b�N�X�̃f�[�^
      MATERIAL Material;                // �}�e���A��
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