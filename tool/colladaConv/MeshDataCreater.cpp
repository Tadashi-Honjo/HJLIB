#include <string>
#include "Exception.h"
#include "ColladaLoader.h"
#include "MeshData.h"
#include "MeshDataCreater.h"

CMeshDataCreater::CMeshDataCreater()
{
   // ローダーをFBX用に変更するときはここを修正する
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

// Polygons形式の頂点エレメントデータをデータ構造にコンバートする。
void CMeshDataCreater::PolygonsConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
   UINT Index = 0, IndexP = 0;
   UINT VcountCnt = pDataArray->VCount.size(), VertexElementCnt = pDataArray->VertexElement.size();

	// Stride数のチェック
	for( UINT i=0; i<VertexElementCnt; i++ )
	{
		if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"VERTEX" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "VERTEXのStrideが不正です。", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"NORMAL" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "NORMALのStrideが不正です。", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"TEXCOORD" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 2 )
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "TEXCOORDのStrideが不正です。", "" ) );
		}
	}

	// 頂点数
	UINT VertexCount = pDataArray->VertexElement[0].Index.size(); // index数が頂点数
	// インデックス数
	UINT IndexCount = 0;
	for( UINT i=0; i<VcountCnt; i++ ){
/*
		if(pDataArray->VCount[i] > 4){
			throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "polylistの頂点数が不正です。(最大４)", "" ) );
		}
*/
		IndexCount += (pDataArray->VCount[i]-2) * 3;
	}

	// 頂点数の領域確保
	pMesh->pVertex.resize(VertexCount);
	pMesh->pIndex.resize(IndexCount);

	for( UINT j=0; j<VertexElementCnt; j++ )
	{
		// 頂点の場合
		if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"VERTEX" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "VERTEXのインデックス数が不正です。", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Vertex ) 
								);
			}
		}
		// 法線の場合
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"NORMAL" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "NORMALのインデックス数が不正です。", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Normal ) 
								);
			}
		}

		// テクセルの場合
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"TEXCOORD" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "TEXCOORDのインデックス数が不正です。", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	2, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].UV ) 
								);
			}
		}
	}

	// インデックス
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

	// テクセルは y 方向にたいして、逆転する
	for( UINT i=0; i<VertexCount; i++ )
	{
		pMesh->pVertex[i].UV.f32y *= -1.0f;
	}

	// メッシュ名を登録
	pMesh->InstanceGeometryName = pDataArray->InstanceGeometryName;
}
// Polylist形式の頂点エレメントデータをデータ構造にコンバートする。
void CMeshDataCreater::PolylistConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
   UINT Index = 0, IndexP = 0;
   UINT VcountCnt = pDataArray->VCount.size(), VertexElementCnt = pDataArray->VertexElement.size();

	// Stride数のチェック
	for( UINT i=0; i<VertexElementCnt; i++ )
	{
		if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"VERTEX" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "VERTEXのStrideが不正です。", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"NORMAL" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "NORMALのStrideが不正です。", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"TEXCOORD" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 2 )
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "TEXCOORDのStrideが不正です。", "" ) );
		}
	}

	// 頂点数
	UINT VertexCount = pDataArray->VertexElement[0].Index.size(); // index数が頂点数
	// インデックス数
	UINT IndexCount = 0;
	for( UINT i=0; i<VcountCnt; i++ ){
		if(pDataArray->VCount[i] > 4){
			throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "polylistの頂点数が不正です。(最大４)", "" ) );
		}
		IndexCount += (pDataArray->VCount[i]-2) * 3;
	}

	// 頂点数の領域確保
	pMesh->pVertex.resize(VertexCount);
	pMesh->pIndex.resize(IndexCount);

	for( UINT j=0; j<VertexElementCnt; j++ )
	{
		// 頂点の場合
		if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"VERTEX" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "VERTEXのインデックス数が不正です。", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Vertex ) 
								);
			}
		}
		// 法線の場合
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"NORMAL" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "NORMALのインデックス数が不正です。", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	3, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].Normal ) 
								);
			}
		}

		// テクセルの場合
		else if( wcscmp( pDataArray->VertexElement[j].Semantic.data(), L"TEXCOORD" ) == 0 ){
			if(pDataArray->VertexElement[j].Index.size() != VertexCount){
				throw( CException( "CMeshDataCreater::PolylistConvert()でエラーが発生しました。", "TEXCOORDのインデックス数が不正です。", "" ) );
			}
			for( UINT k=0; k<VertexCount; k++ ){
				SetVertexElement( 	2, 
									pDataArray->VertexElement[j].Vector[pDataArray->VertexElement[j].Index[k]].vector,
									&( pMesh->pVertex[k].UV ) 
								);
			}
		}
	}

	// インデックス
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

	// テクセルは y 方向にたいして、逆転する
	for( UINT i=0; i<VertexCount; i++ )
	{
		pMesh->pVertex[i].UV.f32y *= -1.0f;
	}

	// メッシュ名を登録
	pMesh->InstanceGeometryName = pDataArray->InstanceGeometryName;
}

// Triangles形式の頂点エレメントデータをデータ構造にコンバートする。
void CMeshDataCreater::TrianglesConvert( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
	UINT Index = 0;
	UINT VertexElementCnt = pDataArray->VertexElement.size();
	UINT VertexCount = 0;
	UINT IndexCount = 0;

	// Stride数のチェック
	for( UINT i=0; i<VertexElementCnt; i++ )
	{
		if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"VERTEX" ) == 0 )
		{
			IndexCount = pDataArray->VertexElement[i].Index.size();
//			VertexCount = pDataArray->VertexElement[i].Vector.size();
			VertexCount = IndexCount; //本城 UVなどの問題もあるのでインデックス分だけ頂点作成
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::TrianglesConvert()でエラーが発生しました。", "VERTEXのStrideが不正です。", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"NORMAL" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 3 )
				throw( CException( "CMeshDataCreater::TrianglesConvert()でエラーが発生しました。", "NORMALのStrideが不正です。", "" ) );
		}
		else if( wcscmp( pDataArray->VertexElement[i].Semantic.data(), L"TEXCOORD" ) == 0 )
		{
			if( pDataArray->VertexElement[i].Stride != 2 )
				throw( CException( "CMeshDataCreater::TrianglesConvert()でエラーが発生しました。", "TEXCOORDのStrideが不正です。", "" ) );
		}
	}
	if(VertexCount == 0)
		throw( CException( "CMeshDataCreater::TrianglesConvert()でエラーが発生しました。", "頂点数が不正です。", "" ) );
	if(IndexCount == 0)
		throw( CException( "CMeshDataCreater::TrianglesConvert()でエラーが発生しました。", "インデックス数が不正です。", "" ) );
	
	// 頂点数の領域確保
	pMesh->pVertex.resize(VertexCount);
	pMesh->pIndex.resize(IndexCount);

	UINT ind=0;
	UINT triangleCnt = IndexCount / 3;
	for( hj_u32 u32i=0; u32i<triangleCnt; u32i++ )
	{
		for( hj_u32 u32j=0; u32j<VertexElementCnt; u32j++ )
		{
			// 頂点の場合
			if( wcscmp( pDataArray->VertexElement[u32j].Semantic.data(), L"VERTEX" ) == 0 )
			{
				for( hj_u32 u32k=0; u32k<3; u32k++ ){
					ind = pDataArray->VertexElement[u32j].Index[Index + u32k];
					SetVertexElement( 3, pDataArray->VertexElement[u32j].Vector[ind].vector, &( pMesh->pVertex[Index + u32k].Vertex ) );
				}
			}
			// 法線の場合
			else if( wcscmp( pDataArray->VertexElement[u32j].Semantic.data(), L"NORMAL" ) == 0 )
			{
				for( UINT u32k=0; u32k<3; u32k++ ){
					ind = pDataArray->VertexElement[u32j].Index[Index + u32k];
					SetVertexElement( 3, pDataArray->VertexElement[u32j].Vector[ind].vector, &( pMesh->pVertex[Index + u32k].Normal ) );
				}
			}
			// テクセルの場合
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

	// テクセルは y 方向にたいして、逆転する
	for( UINT i=0; i<VertexCount; i++ ){
		pMesh->pVertex[i].UV.f32y *= -1.0f;
	}

	// メッシュ名を登録
	pMesh->InstanceGeometryName = pDataArray->InstanceGeometryName;
}

/*
void CMeshDataCreater::CreateVertexAndIndexBuffer( ID3D11Device* pD3DDevice, CMeshData::MESH* pMesh )
{
   // 頂点バッファを作成

   D3D11_BUFFER_DESC BufferDesc;
   D3D11_SUBRESOURCE_DATA resource;

   // 初期値を設定する
   resource.pSysMem = pMesh->pVertex;
   resource.SysMemPitch = 0;
   resource.SysMemSlicePitch = 0;

   // バッファの設定
   ::ZeroMemory( &BufferDesc, sizeof( BufferDesc ) );
   BufferDesc.ByteWidth             = sizeof( CMeshData::VERTEX ) * pMesh->VertexCount; // バッファサイズ
   BufferDesc.Usage                 = D3D11_USAGE_DEFAULT;                                   // リソース使用法を特定する
   BufferDesc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;                              // バッファの種類
   BufferDesc.CPUAccessFlags        = 0;                                                     // CPU アクセス
   BufferDesc.MiscFlags             = 0;                                                     // その他のフラグも設定しない

   if( FAILED( pD3DDevice->CreateBuffer( &BufferDesc, &resource, &pMesh->pVertexBuffer ) ) )
      throw( CException( "CMeshDataCreater::CreateVertexAndIndexBuffer()でエラーが発生しました。", "頂点バッファの作成に失敗しました。", "" ) );

   // インデックスバッファを作成する

   // 初期値を設定する
   resource.pSysMem = pMesh->pIndex;
   resource.SysMemPitch = 0;
   resource.SysMemSlicePitch = 0;

   // バッファの設定
   ::ZeroMemory( &BufferDesc, sizeof( BufferDesc ) );
   BufferDesc.ByteWidth             = sizeof( UINT ) * pMesh->IndexCount; // バッファサイズ
   BufferDesc.Usage                 = D3D11_USAGE_DEFAULT;                // リソース使用法を特定する
   BufferDesc.BindFlags             = D3D11_BIND_INDEX_BUFFER;            // バッファの種類
   BufferDesc.CPUAccessFlags        = 0;                                  // CPU アクセス
   BufferDesc.MiscFlags             = 0;                                  // その他のフラグも設定しない

   if( FAILED( pD3DDevice->CreateBuffer( &BufferDesc, &resource, &pMesh->pIndexBuffer ) ) )
      throw( CException( "CMeshDataCreater::CreateVertexAndIndexBuffer()でエラーが発生しました。", "インデックスバッファの作成に失敗しました。", "" ) );
}
*/
void CMeshDataCreater::CreateMaterial( IMeshLoader::DATAARRAY* pDataArray, CMeshData::MESH* pMesh )
{
	// マテリアル情報を格納する
	pMesh->Material.InstanceMaterialName = pDataArray->InstanceMaterialName;
	pMesh->Material.ImageFileName = pDataArray->ImageFileName;
/*
	pMesh->Material.pSamplerState = NULL;
	pMesh->Material.pSRView = NULL;
   // テクスチャーの読み込み
   if( pDataArray->ImageFileName.length() > 0 )
   {
      // シェーダーリソースビューを作成する
      D3DX11_IMAGE_LOAD_INFO info;
      ::ZeroMemory( &info, sizeof( D3DX11_IMAGE_LOAD_INFO ) );
      info.Width = D3DX11_DEFAULT; 
      info.Height = D3DX11_DEFAULT; 
      info.Depth = D3DX11_DEFAULT; 
      info.FirstMipLevel = D3DX11_DEFAULT;
      info.MipLevels = 0;                           // ミップマップ数。0 または D3DX11_DEFAULT を使用するとすべてのミップマップ チェーンを作成する。
      info.Usage = D3D11_USAGE_DEFAULT; 
      info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      info.CpuAccessFlags = 0;
      info.MiscFlags = 0;
      info.Format = DXGI_FORMAT_FROM_FILE;
      info.Filter = D3DX11_FILTER_POINT;            // テクスチャー読み込み時に使用するフィルター
      info.MipFilter = D3DX11_FILTER_POINT;         // ミップマップ作成時に使用するフィルター
      info.pSrcInfo = NULL;
      if( FAILED( D3DX11CreateShaderResourceViewFromFileW( pD3DDevice, pDataArray->ImageFileName.data(), &info, NULL, &(pMesh->Material.pSRView), NULL ) ) )
         throw( CException( "CMeshDataCreater::CreateMaterial()でエラーが発生しました。", "シェーダーリソースビューの作成に失敗しました。", "" ) );

      D3D11_SAMPLER_DESC samplerDesc;
      samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  // サンプリング時に使用するフィルタ。
      samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
      samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 ～ 1 の範囲外にある v テクスチャー座標
      samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 ～ 1 の範囲外にある w テクスチャー座標
      samplerDesc.MipLODBias = 0;                            // 計算されたミップマップ レベルからのバイアス
      samplerDesc.MaxAnisotropy = 1;                         // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
      samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // 比較オプション。
      samplerDesc.BorderColor[0] = 0;                        // 境界色
      samplerDesc.BorderColor[1] = 0;
      samplerDesc.BorderColor[2] = 0;
      samplerDesc.BorderColor[3] = 0;
      samplerDesc.MinLOD = 0;                                // アクセス可能なミップマップの下限値
      samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // アクセス可能なミップマップの上限値
      if( FAILED( pD3DDevice->CreateSamplerState( &samplerDesc, &(pMesh->Material.pSamplerState) ) ) )
         throw( CException( "CMeshDataCreater::CreateMaterial()でエラーが発生しました。", "サンプラーステートの作成に失敗しました。", "" ) );
   }
*/
}


//void CMeshDataCreater::LoadMesh( ID3D11Device* pD3DDevice, const TCHAR* pFileName, CMeshData** ppMeshData )
void CMeshDataCreater::LoadMesh( const hj_tchar* pFileName, CMeshData** ppMeshData )
{
	try{
		// メッシュを読み込み一時バッファに格納する
		m_pMeshLoader->LoadMesh( pFileName );

		// CMeshData() は外部でメモリ管理を行う
		*ppMeshData = new CMeshData();

		(*ppMeshData)->CreateMeshArray( m_pMeshLoader->m_DataArray.size() );


		for( UINT i=0; i<m_pMeshLoader->m_DataArray.size(); i++ )
		{

			// 頂点エレメントを処理
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
					throw( CException( "CMeshDataCreater::LoadMesh()でエラーが発生しました。", "未対応のジオメトリ種別が選択されました。", "" ) );
					break;
			};

			// 頂点バッファとインデックスバッファを作成する
			//CreateVertexAndIndexBuffer( pD3DDevice, (*ppMeshData)->GetMeshArray(i) );
	        // マテリアルを処理
			CreateMaterial( &m_pMeshLoader->m_DataArray[i], (*ppMeshData)->GetMeshArray(i) );
		}
	}
	catch( CException ex )
	{
		throw( ex );
	}
}
