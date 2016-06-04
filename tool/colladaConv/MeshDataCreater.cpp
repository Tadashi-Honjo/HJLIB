#include <string>
#include "Exception.h"
#include "ColladaLoader.h"
#include "MeshData.h"
#include "MeshDataCreater.h"

CMeshDataCreater::CMeshDataCreater()
{
   // ���[�_�[��FBX�p�ɕύX����Ƃ��͂������C������
   m_pMeshLoader = new CColladaLoader();
}

void CMeshDataCreater::Invalidate()
{
   HJ_SAFE_DELETE( m_pMeshLoader );
}

CMeshDataCreater::~CMeshDataCreater()
{
   Invalidate();
}

void CMeshDataCreater::SetVertexElement( UINT Stride, const float pInVertexElement[3], stcVector3* pOutVertexElement )
{
   if( Stride >= 1 )
      pOutVertexElement->f32x = pInVertexElement[0]; // x
   if( Stride >= 2 )
      pOutVertexElement->f32y = pInVertexElement[1]; // y
   if( Stride >= 3 )
      pOutVertexElement->f32z = pInVertexElement[2]; // z
}

// Polygons�`���̒��_�G�������g�f�[�^���f�[�^�\���ɃR���o�[�g����B
void CMeshDataCreater::PolygonsConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
   UINT Index = 0, IndexP = 0;
   UINT VcountCnt = pDataArray->VCount.size(), VertexElementCnt = pDataArray->VertexElement.size();

	// Stride���̃`�F�b�N
	for( UINT i=0; i<VertexElementCnt; i++ )
	{
		if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"VERTEX" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "VERTEX��Stride���s���ł��B", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"NORMAL" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "NORMAL��Stride���s���ł��B", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"TEXCOORD" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 2 )
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "TEXCOORD��Stride���s���ł��B", "" ) );
		}
	}

	// ���_��
	UINT VertexCount = pDataArray->VertexElement[0].Index.size(); // index�������_��
	// �C���f�b�N�X��
	UINT IndexCount = 0;
	for( UINT i=0; i<VcountCnt; i++ ){
/*
		if(pDataArray->VCount[i] > 4){
			throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "polylist�̒��_�����s���ł��B(�ő�S)", "" ) );
		}
*/
		IndexCount += (pDataArray->VCount[i]-2) * 3;
	}

	// ���_���̗̈�m��
	pMesh->pVertex.resize(VertexCount);
	pMesh->pIndex.resize(IndexCount);

	for( UINT j=0; j<VertexElementCnt; j++ )
	{
		// ���_�̏ꍇ
		if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"VERTEX" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "VERTEX�̃C���f�b�N�X�����s���ł��B", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Vertex ) 
								);
			}
		}
		// �@���̏ꍇ
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"NORMAL" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "NORMAL�̃C���f�b�N�X�����s���ł��B", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Normal ) 
								);
			}
		}

		// �e�N�Z���̏ꍇ
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"TEXCOORD" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "TEXCOORD�̃C���f�b�N�X�����s���ł��B", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	2, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].UV ) 
								);
			}
		}
	}

	// �C���f�b�N�X
	Index = 0;
	IndexP = 0;
	for( UINT i=0; i<VcountCnt; i++ ){
		hj_u32 u32VCount = pDataArray->VCount[i];
//		if(u32VCount != 5) continue;
		hj_u32 u32TriNum = u32VCount - 2;
		hj_u32 u32VertexCnt=0, u32VertexIndex=0;

		for(hj_u32 u32TriCnt=0; u32TriCnt<u32TriNum; ++u32TriCnt){
			static const hj_s32 tos32PolyIndex[] = {
				0,
				2,
				1
			};
			for( UINT k=0; k<3; k++ ){
				if(u32TriCnt == 0){
					u32VertexIndex = ((u32VertexCnt+tos32PolyIndex[k]) % u32VCount);
				}else{
					switch(k){
						case 0:
							u32VertexIndex = 0;
							break;
						case 1:
						case 2:
							u32VertexIndex = ((u32VertexCnt+tos32PolyIndex[k]-1) % u32VCount);
							break;
					};
				}
				pMesh->pIndex[Index + k] = IndexP + u32VertexIndex;
			}
			Index += 3;
			u32VertexCnt += 3;
			--u32VertexCnt;
		}
		IndexP += u32VCount;
	}

	// �e�N�Z���� y �����ɂ������āA�t�]����
	for( UINT i=0; i<VertexCount; i++ )
	{
		pMesh->pVertex[i].UV.f32y *= -1.0f;
	}

	// ���b�V������o�^
	pMesh->InstanceGeometryName = pDataArray->InstanceGeometryName;
}
// Polylist�`���̒��_�G�������g�f�[�^���f�[�^�\���ɃR���o�[�g����B
void CMeshDataCreater::PolylistConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
   UINT Index = 0, IndexP = 0;
   UINT VcountCnt = pDataArray->VCount.size(), VertexElementCnt = pDataArray->VertexElement.size();

	// Stride���̃`�F�b�N
	for( UINT i=0; i<VertexElementCnt; i++ )
	{
		if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"VERTEX" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "VERTEX��Stride���s���ł��B", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"NORMAL" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "NORMAL��Stride���s���ł��B", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"TEXCOORD" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 2 )
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "TEXCOORD��Stride���s���ł��B", "" ) );
		}
	}

	// ���_��
	UINT VertexCount = pDataArray->VertexElement[0].Index.size(); // index�������_��
	// �C���f�b�N�X��
	UINT IndexCount = 0;
	for( UINT i=0; i<VcountCnt; i++ ){
		if(pDataArray->VCount[i] > 4){
			throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "polylist�̒��_�����s���ł��B(�ő�S)", "" ) );
		}
		IndexCount += (pDataArray->VCount[i]-2) * 3;
	}

	// ���_���̗̈�m��
	pMesh->pVertex.resize(VertexCount);
	pMesh->pIndex.resize(IndexCount);

	for( UINT j=0; j<VertexElementCnt; j++ )
	{
		// ���_�̏ꍇ
		if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"VERTEX" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "VERTEX�̃C���f�b�N�X�����s���ł��B", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Vertex ) 
								);
			}
		}
		// �@���̏ꍇ
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"NORMAL" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "NORMAL�̃C���f�b�N�X�����s���ł��B", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Normal ) 
								);
			}
		}

		// �e�N�Z���̏ꍇ
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"TEXCOORD" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()�ŃG���[���������܂����B", "TEXCOORD�̃C���f�b�N�X�����s���ł��B", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	2, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].UV ) 
								);
			}
		}
	}

	// �C���f�b�N�X
	Index = 0;
	IndexP = 0;
	for( UINT i=0; i<VcountCnt; i++ ){
		hj_u32 u32VCount = pDataArray->VCount[i];
//		if(u32VCount != 5) continue;
		hj_u32 u32TriNum = u32VCount - 2;
		hj_u32 u32VertexCnt=0, u32VertexIndex=0;

		for(hj_u32 u32TriCnt=0; u32TriCnt<u32TriNum; ++u32TriCnt){
			static const hj_s32 tos32PolyIndex[] = {
				0,
				2,
				1
			};
			for( UINT k=0; k<3; k++ ){
				if(u32TriCnt == 0){
					u32VertexIndex = ((u32VertexCnt+tos32PolyIndex[k]) % u32VCount);
				}else{
					switch(k){
						case 0:
							u32VertexIndex = 0;
							break;
						case 1:
						case 2:
							u32VertexIndex = ((u32VertexCnt+tos32PolyIndex[k]-1) % u32VCount);
							break;
					};
				}
				pMesh->pIndex[Index + k] = IndexP + u32VertexIndex;
			}
			Index += 3;
			u32VertexCnt += 3;
			--u32VertexCnt;
		}
		IndexP += u32VCount;
	}

	// �e�N�Z���� y �����ɂ������āA�t�]����
	for( UINT i=0; i<VertexCount; i++ )
	{
		pMesh->pVertex[i].UV.f32y *= -1.0f;
	}

	// ���b�V������o�^
	pMesh->InstanceGeometryName = pDataArray->InstanceGeometryName;
}

// Triangles�`���̒��_�G�������g�f�[�^���f�[�^�\���ɃR���o�[�g����B
void CMeshDataCreater::TrianglesConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
	UINT Index = 0;
	UINT VertexElementCnt = pDataArray->VertexElement.size();
	UINT VertexCount = 0;
	UINT IndexCount = 0;

	// Stride���̃`�F�b�N
	for( UINT i=0; i<VertexElementCnt; i++ )
	{
		if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"VERTEX" ) == 0 )
		{
			IndexCount = pDataArray->VertexElement[i].Index.size();
//			VertexCount = pDataArray->VertexElement[i].Vector.size();
			VertexCount = IndexCount; //�{�� UV�Ȃǂ̖�������̂ŃC���f�b�N�X���������_�쐬
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::TrianglesConvert()�ŃG���[���������܂����B", "VERTEX��Stride���s���ł��B", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"NORMAL" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::TrianglesConvert()�ŃG���[���������܂����B", "NORMAL��Stride���s���ł��B", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"TEXCOORD" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 2 )
				throw( CException( "CMeshDataCreater::TrianglesConvert()�ŃG���[���������܂����B", "TEXCOORD��Stride���s���ł��B", "" ) );
		}
	}
	if(VertexCount == 0)
		throw( CException( "CMeshDataCreater::TrianglesConvert()�ŃG���[���������܂����B", "���_�����s���ł��B", "" ) );
	if(IndexCount == 0)
		throw( CException( "CMeshDataCreater::TrianglesConvert()�ŃG���[���������܂����B", "�C���f�b�N�X�����s���ł��B", "" ) );
	
	// ���_���̗̈�m��
	pMesh->pVertex.resize(VertexCount);
	pMesh->pIndex.resize(IndexCount);

	UINT ind=0;
	UINT triangleCnt = IndexCount / 3;
	for( hj_u32 u32i=0; u32i<triangleCnt; u32i++ )
	{
		for( hj_u32 u32j=0; u32j<VertexElementCnt; u32j++ )
		{
			// ���_�̏ꍇ
			if( wcscmp( pDataArray->VertexElement[u32j].Semantic.data(), L"VERTEX" ) == 0 )
			{
				for( hj_u32 u32k=0; u32k<3; u32k++ ){
					ind = pDataArray->VertexElement[u32j].Index[Index + u32k];
					SetVertexElement( 3, pDataArray->VertexElement[u32j].Vector[ind].vector, &( pMesh->pVertex[Index + u32k].Vertex ) );
				}
			}
			// �@���̏ꍇ
			else if( wcscmp( pDataArray->VertexElement[u32j].Semantic.data(), L"NORMAL" ) == 0 )
			{
				for( UINT u32k=0; u32k<3; u32k++ ){
					ind = pDataArray->VertexElement[u32j].Index[Index + u32k];
					SetVertexElement( 3, pDataArray->VertexElement[u32j].Vector[ind].vector, &( pMesh->pVertex[Index + u32k].Normal ) );
				}
			}
			// �e�N�Z���̏ꍇ
			else if( wcscmp( pDataArray->VertexElement[u32j].Semantic.data(), L"TEXCOORD" ) == 0 )
			{
				for( UINT u32k=0; u32k<3; u32k++ ){
					ind = pDataArray->VertexElement[u32j].Index[Index + u32k];
					SetVertexElement( 2, pDataArray->VertexElement[u32j].Vector[ind].vector, (stcVector3*)&( pMesh->pVertex[Index + u32k].UV ) );
				}
			}
		}

		pMesh->pIndex[Index + 0] = Index + 0;
		pMesh->pIndex[Index + 1] = Index + 2;
		pMesh->pIndex[Index + 2] = Index + 1;
		Index += 3;
	}

	// �e�N�Z���� y �����ɂ������āA�t�]����
	for( UINT i=0; i<VertexCount; i++ ){
		pMesh->pVertex[i].UV.f32y *= -1.0f;
	}

	// ���b�V������o�^
	pMesh->InstanceGeometryName = pDataArray->InstanceGeometryName;
}

/*
void CMeshDataCreater::CreateVertexAndIndexBuffer( ID3D11Device* pD3DDevice, CMeshData::MESH* pMesh )
{
   // ���_�o�b�t�@���쐬

   D3D11_BUFFER_DESC BufferDesc;
   D3D11_SUBRESOURCE_DATA resource;

   // �����l��ݒ肷��
   resource.pSysMem = pMesh->pVertex;
   resource.SysMemPitch = 0;
   resource.SysMemSlicePitch = 0;

   // �o�b�t�@�̐ݒ�
   ::ZeroMemory( &BufferDesc, sizeof( BufferDesc ) );
   BufferDesc.ByteWidth             = sizeof( CMeshData::VERTEX ) * pMesh->VertexCount; // �o�b�t�@�T�C�Y
   BufferDesc.Usage                 = D3D11_USAGE_DEFAULT;                                   // ���\�[�X�g�p�@����肷��
   BufferDesc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;                              // �o�b�t�@�̎��
   BufferDesc.CPUAccessFlags        = 0;                                                     // CPU �A�N�Z�X
   BufferDesc.MiscFlags             = 0;                                                     // ���̑��̃t���O���ݒ肵�Ȃ�

   if( FAILED( pD3DDevice->CreateBuffer( &BufferDesc, &resource, &pMesh->pVertexBuffer ) ) )
      throw( CException( "CMeshDataCreater::CreateVertexAndIndexBuffer()�ŃG���[���������܂����B", "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", "" ) );

   // �C���f�b�N�X�o�b�t�@���쐬����

   // �����l��ݒ肷��
   resource.pSysMem = pMesh->pIndex;
   resource.SysMemPitch = 0;
   resource.SysMemSlicePitch = 0;

   // �o�b�t�@�̐ݒ�
   ::ZeroMemory( &BufferDesc, sizeof( BufferDesc ) );
   BufferDesc.ByteWidth             = sizeof( UINT ) * pMesh->IndexCount; // �o�b�t�@�T�C�Y
   BufferDesc.Usage                 = D3D11_USAGE_DEFAULT;                // ���\�[�X�g�p�@����肷��
   BufferDesc.BindFlags             = D3D11_BIND_INDEX_BUFFER;            // �o�b�t�@�̎��
   BufferDesc.CPUAccessFlags        = 0;                                  // CPU �A�N�Z�X
   BufferDesc.MiscFlags             = 0;                                  // ���̑��̃t���O���ݒ肵�Ȃ�

   if( FAILED( pD3DDevice->CreateBuffer( &BufferDesc, &resource, &pMesh->pIndexBuffer ) ) )
      throw( CException( "CMeshDataCreater::CreateVertexAndIndexBuffer()�ŃG���[���������܂����B", "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂����B", "" ) );
}
*/
void CMeshDataCreater::CreateMaterial( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
	// �}�e���A�������i�[����
	pMesh->Material.InstanceMaterialName = pDataArray->InstanceMaterialName;
	pMesh->Material.ImageFileName = pDataArray->ImageFileName;
/*
	pMesh->Material.pSamplerState = NULL;
	pMesh->Material.pSRView = NULL;
   // �e�N�X�`���[�̓ǂݍ���
   if( pDataArray->ImageFileName.length() > 0 )
   {
      // �V�F�[�_�[���\�[�X�r���[���쐬����
      D3DX11_IMAGE_LOAD_INFO info;
      ::ZeroMemory( &info, sizeof( D3DX11_IMAGE_LOAD_INFO ) );
      info.Width = D3DX11_DEFAULT; 
      info.Height = D3DX11_DEFAULT; 
      info.Depth = D3DX11_DEFAULT; 
      info.FirstMipLevel = D3DX11_DEFAULT;
      info.MipLevels = 0;                           // �~�b�v�}�b�v���B0 �܂��� D3DX11_DEFAULT ���g�p����Ƃ��ׂẴ~�b�v�}�b�v �`�F�[�����쐬����B
      info.Usage = D3D11_USAGE_DEFAULT; 
      info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      info.CpuAccessFlags = 0;
      info.MiscFlags = 0;
      info.Format = DXGI_FORMAT_FROM_FILE;
      info.Filter = D3DX11_FILTER_POINT;            // �e�N�X�`���[�ǂݍ��ݎ��Ɏg�p����t�B���^�[
      info.MipFilter = D3DX11_FILTER_POINT;         // �~�b�v�}�b�v�쐬���Ɏg�p����t�B���^�[
      info.pSrcInfo = NULL;
      if( FAILED( D3DX11CreateShaderResourceViewFromFileW( pD3DDevice, pDataArray->ImageFileName.data(), &info, NULL, &(pMesh->Material.pSRView), NULL ) ) )
         throw( CException( "CMeshDataCreater::CreateMaterial()�ŃG���[���������܂����B", "�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂����B", "" ) );

      D3D11_SAMPLER_DESC samplerDesc;
      samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  // �T���v�����O���Ɏg�p����t�B���^�B
      samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
      samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W
      samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W
      samplerDesc.MipLODBias = 0;                            // �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
      samplerDesc.MaxAnisotropy = 1;                         // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
      samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ��r�I�v�V�����B
      samplerDesc.BorderColor[0] = 0;                        // ���E�F
      samplerDesc.BorderColor[1] = 0;
      samplerDesc.BorderColor[2] = 0;
      samplerDesc.BorderColor[3] = 0;
      samplerDesc.MinLOD = 0;                                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
      samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l
      if( FAILED( pD3DDevice->CreateSamplerState( &samplerDesc, &(pMesh->Material.pSamplerState) ) ) )
         throw( CException( "CMeshDataCreater::CreateMaterial()�ŃG���[���������܂����B", "�T���v���[�X�e�[�g�̍쐬�Ɏ��s���܂����B", "" ) );
   }
*/
}


//void CMeshDataCreater::LoadMesh( ID3D11Device* pD3DDevice, const TCHAR* pFileName, CMeshData** ppMeshData )
void CMeshDataCreater::LoadMesh( const hj_tchar* pFileName, CMeshData** ppMeshData )
{
	try{
		// ���b�V����ǂݍ��݈ꎞ�o�b�t�@�Ɋi�[����
		m_pMeshLoader->LoadMesh( pFileName );

		// CMeshData() �͊O���Ń������Ǘ����s��
		*ppMeshData = new CMeshData();

		(*ppMeshData)->CreateMeshArray( m_pMeshLoader->m_DataArray.size() );


		for( UINT i=0; i<m_pMeshLoader->m_DataArray.size(); i++ )
		{

			// ���_�G�������g������
			switch( m_pMeshLoader->m_DataArray[i].GeometryType )
			{
				case IMeshLoader::GEOMETRY_TYPE_POLYGONS:
					PolygonsConvert( &m_pMeshLoader->m_DataArray[i], (*ppMeshData)->GetMeshArray(i) );
					break;
				case IMeshLoader::GEOMETRY_TYPE_POLYLIST:
					PolylistConvert( &m_pMeshLoader->m_DataArray[i], (*ppMeshData)->GetMeshArray(i) );
					break;
				case IMeshLoader::GEOMETRY_TYPE_TRIANGLES:
					TrianglesConvert( &m_pMeshLoader->m_DataArray[i], (*ppMeshData)->GetMeshArray(i) );
					break;
				default:
					throw( CException( "CMeshDataCreater::LoadMesh()�ŃG���[���������܂����B", "���Ή��̃W�I���g����ʂ��I������܂����B", "" ) );
					break;
			};

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬����
			//CreateVertexAndIndexBuffer( pD3DDevice, (*ppMeshData)->GetMeshArray(i) );
	        // �}�e���A��������
			CreateMaterial( &m_pMeshLoader->m_DataArray[i], (*ppMeshData)->GetMeshArray(i) );
		}
	}
	catch( CException ex )
	{
		throw( ex );
	}
}
