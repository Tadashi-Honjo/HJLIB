//--------------------------------------------------------------
/*!	@file
	@brief	WinDx2D描画
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------


// HJLib namespace
namespace hj{

namespace{
	

	// フォーマット情報
	struct stcFmtData{
		hj_u32 u32Flg;
		hj_u32 u32DataSize;
	};
	stcFmtData tasFmtData[ clsHJDraw2d::FMT_MAX_NUM ] = {
		// Flg													Size
		{D3DFVF_XYZ | D3DFVF_DIFFUSE,						sizeof(clsWinDxDraw2d::stcWinDxFormat_PC)},		// FMT_PC
		{D3DFVF_XYZ | D3DFVF_TEX1,							sizeof(clsWinDxDraw2d::stcWinDxFormat_PT)},		// FMT_PT
		{D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,			sizeof(clsWinDxDraw2d::stcWinDxFormat_PCT)},	// FMT_PCT
	};
	
	// プリミティブ情報
	struct stcPrimData{
		D3DPRIMITIVETYPE eType;
	};
	stcPrimData tasPrimData[ clsHJDraw2d::PT_MAX_NUM ] = {
		{D3DPT_POINTLIST},			//PT_POINTLIST
		{D3DPT_LINELIST},			//PT_LINELIST
		{D3DPT_LINESTRIP},			//PT_LINESTRIP
		{D3DPT_TRIANGLELIST},		//PT_TRIANGLELIST
		{D3DPT_TRIANGLESTRIP},		//PT_TRIANGLESTRIP
		{D3DPT_TRIANGLEFAN},		//PT_TRIANGLEFAN
	};
}
//! WinDxDraw2dクラス
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxDraw2d::clsWinDxDraw2d():clsHJDraw2d()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxDraw2d::~clsWinDxDraw2d()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画開始
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw2d::Begin()
{
	clsHJDraw2d::Begin();

	clsHJGraphics& rcGraphics = *clsHJCoreMgr::RS().GetGraphics();
	clsHJRenderTarget* pcRenderTarget = rcGraphics.GetSetRenderTarget();
	hj_u32 u32Width = rcGraphics.GetScreenWidth();
	hj_u32 u32Height = rcGraphics.GetScreenHeight();
	if(pcRenderTarget!=NULL){
		u32Width = pcRenderTarget->GetWidth();
		u32Height = pcRenderTarget->GetHeight();
	}

	// マトリクス設定
	rcGraphics.SetWorldMatrix( *stcMatrix44::GetIndentityMatrix() );	// ワールドマトリクス
	rcGraphics.SetViewMatrix( *stcMatrix44::GetIndentityMatrix() );		// ビューマトリクス

	stcMatrix44 sMtx;

	// プロジェクション設定
	sMtx.SetTranslate(stcVector3(-0.5f, 0.5f, 0.f));	// DirectXラスタライズルールによると0.5オフセットをかける
	sMtx.Scale( stcVector3( 2.f/static_cast<hj_f32>(u32Width), 2.f/static_cast<hj_f32>(u32Height), 0.f) );
	rcGraphics.SetProjectionMatrix( sMtx );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プラットフォーム用頂点に変換(内部でメモリ確保する)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_u8* clsWinDxDraw2d::ConvArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)
{
	stcFmtData				*pcFmtData			= &tasFmtData[eFmt];
	stcPrimData				*pcPrimData			= &tasPrimData[ePrim];
	hj_u32					u32ArrayDataSize	= pcFmtData->u32DataSize * u32ArrayNum;
	hj_u8*					pu8PfDataArray		= NEW hj_u8[u32ArrayDataSize];	// プラットフォーム用データ

	switch(eFmt){
		case FMT_PC:{		// 座標 + 色
			// データをコピー
			stcWinDxFormat_PC*		 			psPfFmtData = reinterpret_cast<stcWinDxFormat_PC*>(pu8PfDataArray);
			const clsWinDxDraw2d::stcFormat_PC* psFmtData 	= reinterpret_cast<const clsWinDxDraw2d::stcFormat_PC*>(opvDataArray);
			for(hj_u32 u32i=0; u32i<u32ArrayNum; ++u32i, psFmtData++, psPfFmtData++){
				psPfFmtData->f32x 		= psFmtData->f32x;
				psPfFmtData->f32y 		= psFmtData->f32y;
				psPfFmtData->f32z 		= 0.0f;
				psPfFmtData->u32Color 	= psFmtData->u32Color;
			}
			break;
		}
		case FMT_PT:{		// 座標 + UV
			// データをコピー
			stcWinDxFormat_PT* 					psPfFmtData = reinterpret_cast<stcWinDxFormat_PT*>(pu8PfDataArray);
			const clsWinDxDraw2d::stcFormat_PT* psFmtData 	= reinterpret_cast<const clsWinDxDraw2d::stcFormat_PT*>(opvDataArray);
			for(hj_u32 u32i=0; u32i<u32ArrayNum; ++u32i, psFmtData++, psPfFmtData++){
				psPfFmtData->f32x 		= psFmtData->f32x;
				psPfFmtData->f32y 		= psFmtData->f32y;
				psPfFmtData->f32z 		= 0.0f;
				psPfFmtData->f32U1 		= psFmtData->f32u;
				psPfFmtData->f32V1 		= psFmtData->f32v;
			}
			break;
		}
		case FMT_PCT:{		// 座標 + 色 + UV 
			// データをコピー
			stcWinDxFormat_PCT* 					psPfFmtData = reinterpret_cast<stcWinDxFormat_PCT*>(pu8PfDataArray);
			const clsWinDxDraw2d::stcFormat_PCT* 	psFmtData 	= reinterpret_cast<const clsWinDxDraw2d::stcFormat_PCT*>(opvDataArray);
			for(hj_u32 u32i=0; u32i<u32ArrayNum; ++u32i, psFmtData++, psPfFmtData++){
				psPfFmtData->f32x 		= psFmtData->f32x;
				psPfFmtData->f32y 		= psFmtData->f32y;
				psPfFmtData->f32z 		= 0.0f;
				psPfFmtData->u32Color 	= psFmtData->u32Color;
				psPfFmtData->f32U1 		= psFmtData->f32u;
				psPfFmtData->f32V1 		= psFmtData->f32v;
			}
			break;
		}
	};

	return pu8PfDataArray;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw2d::RenderArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)
{
	clsHJGraphics&			rcGraphics			= *clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9		pDevice				= static_cast<clsWinDxGraphics*>(&rcGraphics)->GetDevice();
	stcFmtData				*pcFmtData			= &tasFmtData[eFmt];
	stcPrimData				*pcPrimData			= &tasPrimData[ePrim];
	hj_u32					u32ArrayDataSize	= pcFmtData->u32DataSize * u32ArrayNum;
	hj_u32					u32DrawNum			= CalcPrimitiveDrawNum(ePrim, u32ArrayNum);
	hj_u8*					pu8PfDataArray		= ConvArray(eFmt, ePrim, opvDataArray, u32ArrayNum);

	// Graphicsに描画を通知する(シェーダを開始するので、マトリクス更新もあるのでここで)
	rcGraphics.RenderBegin();

	switch(eFmt){
		case FMT_PC:{		// 座標 + 色
			pDevice->SetTexture(0, NULL);
			break;
		}
	};
	
	pDevice->SetFVF( pcFmtData->u32Flg );
	pDevice->DrawPrimitiveUP( pcPrimData->eType, u32DrawNum, pu8PfDataArray, pcFmtData->u32DataSize );

	// Graphicsに描画を終了を通知する(シェーダを終了しないとだめなので)
	rcGraphics.RenderEnd();

	HJ_SAFE_DELETE_ARRAY(pu8PfDataArray);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画(インデックス)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw2d::RenderArrayIndex(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum)
{
	clsHJGraphics&			rcGraphics			= *clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9		pDevice				= static_cast<clsWinDxGraphics*>(&rcGraphics)->GetDevice();
	stcFmtData				*pcFmtData			= &tasFmtData[eFmt];
	stcPrimData				*pcPrimData			= &tasPrimData[ePrim];
	hj_u32					u32DrawNum			= CalcPrimitiveDrawNum(ePrim, u32IndexNum);
	hj_u8*					pu8PfVtxArray		= ConvArray(eFmt, ePrim, opvVertexArray, u32VertexNum);

	// Graphicsに描画を通知する(シェーダを開始するので、マトリクス更新もあるのでここで)
	rcGraphics.RenderBegin();

	switch(eFmt){
		case FMT_PC:{		// 座標 + 色
			pDevice->SetTexture(0, NULL);
			break;
		}
	};
	
	pDevice->SetFVF( pcFmtData->u32Flg );
	pDevice->DrawIndexedPrimitiveUP( pcPrimData->eType, 0, u32VertexNum, u32DrawNum, opvIndexArray, D3DFMT_INDEX32, pu8PfVtxArray, pcFmtData->u32DataSize );	//三角１つ描画

	// Graphicsに描画を終了を通知する(シェーダを終了しないとだめなので)
	rcGraphics.RenderEnd();

	HJ_SAFE_DELETE_ARRAY(pu8PfVtxArray);
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画(プラットフォーム高速版)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw2d::RenderArrayPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)
{
	clsHJGraphics&			rcGraphics			= *clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9		pDevice				= static_cast<clsWinDxGraphics*>(&rcGraphics)->GetDevice();
	stcFmtData				*pcFmtData			= &tasFmtData[eFmt];
	stcPrimData				*pcPrimData			= &tasPrimData[ePrim];
	hj_u32					u32DrawNum			= CalcPrimitiveDrawNum(ePrim, u32ArrayNum);

	// Graphicsに描画開始通知
	rcGraphics.RenderBegin();

	pDevice->SetFVF( pcFmtData->u32Flg );
	pDevice->DrawPrimitiveUP( pcPrimData->eType, u32DrawNum, opvDataArray, pcFmtData->u32DataSize );

	// Graphicsに描画終了通知
	rcGraphics.RenderEnd();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画(インデックス)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw2d::RenderArrayIndexPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum)
{
	clsHJGraphics&			rcGraphics			= *clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9		pDevice				= static_cast<clsWinDxGraphics*>(&rcGraphics)->GetDevice();
	stcFmtData				*pcFmtData			= &tasFmtData[eFmt];
	stcPrimData				*pcPrimData			= &tasPrimData[ePrim];
	hj_u32					u32DrawNum			= CalcPrimitiveDrawNum(ePrim, u32IndexNum);

	// Graphicsに描画開始通知
	rcGraphics.RenderBegin();

	pDevice->SetFVF( pcFmtData->u32Flg );
	pDevice->DrawIndexedPrimitiveUP( pcPrimData->eType, 0, u32VertexNum, u32DrawNum, opvIndexArray, D3DFMT_INDEX32, opvVertexArray, pcFmtData->u32DataSize );	//三角１つ描画

	// Graphicsに描画終了通知
	rcGraphics.RenderEnd();
}


}// namespace hj