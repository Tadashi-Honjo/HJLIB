//--------------------------------------------------------------
/*!	@file
	@brief	WinDx3D描画
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_DRAW_3D_H_
#define __WINDX_DRAW_3D_H_

// HJLib namespace
namespace hj{

//! WinDraw3dクラス
class clsWinDxDraw3d : public clsHJDraw3d
{
public:
	clsWinDxDraw3d();
	virtual ~clsWinDxDraw3d();

	// --- Struct
	struct stcWinDxFormat_PC{
		hj_f32 		f32x, f32y, f32z;
		hj_color32 	u32Color;
	};
	struct stcWinDxFormat_PT{
		hj_f32 		f32x, f32y, f32z;
		hj_f32 		f32u, f32v;
	};
	struct stcWinDxFormat_PCT{
		hj_f32 		f32x, f32y, f32z;
		hj_color32 	u32Color;
		hj_f32 		f32u, f32v;
	};
	struct stcWinDxFormat_PNCT{
		hj_f32 		f32x, f32y, f32z;
		hj_f32		f32nx, f32ny, f32nz;
		hj_color32 	u32Color;
		hj_f32 		f32u, f32v;
	};

	// --- Method
	// 描画開始
	virtual void Begin();		
	// 描画終了
	virtual void End();			
	// プラットフォーム用頂点に変換(内部でメモリ確保する)
	virtual hj_u8* ConvArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum);
	// 描画
	virtual void RenderArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum);
	// 描画(インデックス)
	virtual void RenderArrayIndex(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum);
	// 描画プラットフォーム高速版
	virtual void RenderArrayPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum);	
	// 描画プラットフォーム高速版(インデックス)
	virtual void RenderArrayIndexPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum);
};

}// namespace hj

#endif // __WINDX_DRAW_3D_H_