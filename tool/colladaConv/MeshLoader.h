#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <tchar.h>
#include <vector>
#include <string>

class IMeshLoader
{
public:
   // ���_�G�������g�̊i�[���@�̎�ނ̈ꗗ
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

   // COLLADA����擾�����f�[�^���i�[����\����
   typedef struct _DATAARRAY
   {
      // ���_�G�������g���ƂɃf�[�^���i�[���邽�߂̍\����
      typedef struct _VERTEX_ELEMENT
      {
         // ���_�G�������g�̎��
         std::wstring Semantic;
         // ���_�G�������g�ŕK�v�ƂȂ�f�[�^���B�Ⴆ�Β��_���W�̏ꍇ x, y, z �� 3 �B
         UINT Stride;
         // ���f�[�^�BStride�����g�p����B
         typedef struct _VECTOR
         {
            float vector[3];
         }VECTOR;
         // �P�̒��_�G�������g���̃f�[�^�z��
         std::vector<VECTOR> Vector;
         // �C���f�b�N�X�f�[�^������
         std::vector<UINT> Index;
      }VERTEX_ELEMENT;

      // �W�I���g����
      std::wstring InstanceGeometryName;
      // ���_�G�������g�̊i�[���@�̎��
      GEOMETRY_TYPE GeometryType;
      // �P�|���S���̒��_���������z��B
      std::vector<UINT> VCount;
      // ���_�G�������g���Ƃ̃f�[�^�z��
      std::vector<VERTEX_ELEMENT> VertexElement;
      // �}�e���A����
      std::wstring InstanceMaterialName;
      // �e�N�X�`���[�ւ̃p�X�B�Ƃ肠�����P�݂̂Ƃ���B
      std::wstring ImageFileName;
      // �T���v���[�X�e�[�g�͂Ƃ肠�����l�����Ȃ�
   }DATAARRAY;

public:
   std::vector<DATAARRAY> m_DataArray;

public:
   inline IMeshLoader(){};
   inline virtual ~IMeshLoader(){ Invalidate(); };
   inline virtual void Invalidate(){ m_DataArray.clear(); };
   // ���̃����o�֐��� FBX, COLLADA ���[�_�[����������N���X���Ŏ��̉�����
   virtual void LoadMesh( const TCHAR* pFileName ) = 0;
};

#endif