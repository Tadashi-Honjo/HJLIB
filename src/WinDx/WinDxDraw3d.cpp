//--------------------------------------------------------------
/*!	@file
	@brief	WinDx3D描画
			
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
	stcFmtData tasFmtData[] = {
		// Flg															Size
		{D3DFVF_XYZ | D3DFVF_DIFFUSE,									sizeof(clsHJDraw3d::stcFormat_PC)},		// FMT_PC
		{D3DFVF_XYZ | D3DFVF_TEX1,										sizeof(clsHJDraw3d::stcFormat_PT)},		// FMT_PT
		{D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,						sizeof(clsHJDraw3d::stcFormat_PCT)},	// FMT_PCT
		{D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1,		sizeof(clsHJDraw3d::stcFormat_PNCT)},	// FMT_PCTN
		{D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE,					sizeof(clsHJDraw3d::stcFormat_PNC)},	// FMT_PCN
		
	};
	HJ_STATIC_ASSERT( HJ_ARRAY_NUM(tasFmtData) == clsHJDraw3d::FMT_MAX_NUM);

	// プリミティブ情報
	struct stcPrimData{
		D3DPRIMITIVETYPE eType;
	};
	stcPrimData tasPrimData[ clsHJDraw3d::PT_MAX_NUM ] = {
		{D3DPT_POINTLIST},			//PT_POINTLIST
		{D3DPT_LINELIST},			//PT_LINELIST
		{D3DPT_LINESTRIP},			//PT_LINESTRIP
		{D3DPT_TRIANGLELIST},		//PT_TRIANGLELIST
		{D3DPT_TRIANGLESTRIP},		//PT_TRIANGLESTRIP
		{D3DPT_TRIANGLEFAN},		//PT_TRIANGLEFAN
	};
}
//! WinDxDraw3dクラス
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxDraw3d::clsWinDxDraw3d():clsHJDraw3d()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxDraw3d::~clsWinDxDraw3d()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画開始
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw3d::Begin()
{
	clsHJDraw3d::Begin();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画終了
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw3d::End()
{
	clsHJDraw3d::End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プラットフォーム用頂点に変換(内部でメモリ確保する)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_u8* clsWinDxDraw3d::ConvArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)
{
	stcFmtData				*pcFmtData			= &tasFmtData[eFmt];
	stcPrimData				*pcPrimData			= &tasPrimData[ePrim];
	hj_u32					u32ArrayDataSize	= pcFmtData->u32DataSize * u32ArrayNum;
	hj_u8*					pu8PfDataArray		= NEW hj_u8[u32ArrayDataSize];	// プラットフォーム用データ

	// データをコピー
	switch(eFmt){
		case FMT_PC:{		// 座標 + 色
			
			stcWinDxFormat_PC* 					psPfFmtData = reinterpret_cast<stcWinDxFormat_PC*>(pu8PfDataArray);
			const clsWinDxDraw3d::stcFormat_PC* psFmtData 	= reinterpret_cast<const clsWinDxDraw3d::stcFormat_PC*>(opvDataArray);
			for(hj_u32 u32i=0; u32i<u32ArrayNum; ++u32i, psFmtData++, psPfFmtData++){
				psPfFmtData->f32x 		= psFmtData->f32x;
				psPfFmtData->f32y 		= psFmtData->f32y;
				psPfFmtData->f32z 		= psFmtData->f32z;
				psPfFmtData->u32Color 	= psFmtData->u32Color;
			}
			break;
		}
		case FMT_PT:{		// 座標 + UV
			stcWinDxFormat_PT* 					psPfFmtData = reinterpret_cast<stcWinDxFormat_PT*>(pu8PfDataArray);
			const clsWinDxDraw3d::stcFormat_PT* psFmtData 	= reinterpret_cast<const clsWinDxDraw3d::stcFormat_PT*>(opvDataArray);
			for(hj_u32 u32i=0; u32i<u32ArrayNum; ++u32i, psFmtData++, psPfFmtData++){
				psPfFmtData->f32x 		= psFmtData->f32x;
				psPfFmtData->f32y 		= psFmtData->f32y;
				psPfFmtData->f32z 		= psFmtData->f32z;
				psPfFmtData->f32u 		= psFmtData->f32u;
				psPfFmtData->f32v 		= psFmtData->f32v;
			}
			break;
		}
		case FMT_PCT:{		// 座標 + 色 + UV 
			stcWinDxFormat_PCT* 					psPfFmtData = reinterpret_cast<stcWinDxFormat_PCT*>(pu8PfDataArray);
			const clsWinDxDraw3d::stcFormat_PCT* 	psFmtData 	= reinterpret_cast<const clsWinDxDraw3d::stcFormat_PCT*>(opvDataArray);
			for(hj_u32 u32i=0; u32i<u32ArrayNum; ++u32i, psFmtData++, psPfFmtData++){
				psPfFmtData->f32x 		= psFmtData->f32x;
				psPfFmtData->f32y 		= psFmtData->f32y;
				psPfFmtData->f32z 		= psFmtData->f32z;
				psPfFmtData->u32Color 	= psFmtData->u32Color;
				psPfFmtData->f32u 		= psFmtData->f32u;
				psPfFmtData->f32v 		= psFmtData->f32v;
			}
		}break;
		case FMT_PCTN:{		// 座標 + 色 + UV + 法線
			stcWinDxFormat_PNCT* 					psPfFmtData = reinterpret_cast<stcWinDxFormat_PNCT*>(pu8PfDataArray);
			const clsWinDxDraw3d::stcFormat_PNCT*	psFmtData 	= reinterpret_cast<const clsWinDxDraw3d::stcFormat_PNCT*>(opvDataArray);
			for(hj_u32 u32i=0; u32i<u32ArrayNum; ++u32i, psFmtData++, psPfFmtData++){
				psPfFmtData->f32x 		= psFmtData->f32x;
				psPfFmtData->f32y 		= psFmtData->f32y;
				psPfFmtData->f32z 		= psFmtData->f32z;
				psPfFmtData->f32nx 		= psFmtData->f32nx;
				psPfFmtData->f32ny 		= psFmtData->f32ny;
				psPfFmtData->f32nz 		= psFmtData->f32nz;
				psPfFmtData->u32Color 	= psFmtData->u32Color;
				psPfFmtData->f32u 		= psFmtData->f32u;
				psPfFmtData->f32v 		= psFmtData->f32v;
			}
		}break;
		default:{
			HJ_ASSERT_MSG(HJ_T("未対応タイプが指定されました。"));
			break;
		}break;
	};
	return pu8PfDataArray;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw3d::RenderArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)
{
	clsHJGraphics&			rcGraphics			= *clsHJCoreMgr::RS().GetGraphics();
	LPDIRECT3DDEVICE9		pDevice				= static_cast<clsWinDxGraphics*>(&rcGraphics)->GetDevice();
	stcFmtData				*pcFmtData			= &tasFmtData[eFmt];
	stcPrimData				*pcPrimData			= &tasPrimData[ePrim];
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
void clsWinDxDraw3d::RenderArrayIndex(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum)
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
void clsWinDxDraw3d::RenderArrayPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)
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
/*!	@brief	描画プラットフォーム高速版(インデックス)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxDraw3d::RenderArrayIndexPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum)
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