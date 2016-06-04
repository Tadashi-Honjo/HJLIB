//--------------------------------------------------------------
/*!	@file
	@brief	3D描画
			
	@author 本城 直志
	@date	11/03/01
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_DRAW_3D_H_
#define __HJ_DRAW_3D_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Draw3d基底クラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJDraw3d
{
public:
	clsHJDraw3d();
	~clsHJDraw3d();

	// --- Enum
	//! フォーマット
	enum enmFormat{
		FMT_PC,		// 座標 + 色
		FMT_PT,		// 座標 + UV
		FMT_PCT,	// 座標 + 色 + UV 
		FMT_PCTN,	// 座標 + 色 + UV + 法線
		FMT_PCN,	// 座標 + 色 + 法線

		FMT_MAX_NUM,
	};
	//! プリミティブタイプ
	enum enmPrimitiveType{
	    PT_POINTLIST,
		PT_LINELIST,
		PT_LINESTRIP,
		PT_TRIANGLELIST,
		PT_TRIANGLESTRIP,
		PT_TRIANGLEFAN,

		PT_MAX_NUM
	};

	// --- struct
	//! フォーマット情報
	struct stcFormat_PC{
		hj_f32		f32x, f32y, f32z;
		hj_color32	u32Color;
	};
	struct stcFormat_PT{
		hj_f32		f32x, f32y, f32z;
		hj_f32		f32u, f32v;
	};
	struct stcFormat_PCT{
		hj_f32		f32x, f32y, f32z;
		hj_color32	u32Color;
		hj_f32		f32u, f32v;
	};
	struct stcFormat_PNCT{
		hj_f32		f32x, f32y, f32z;
		hj_f32		f32nx, f32ny, f32nz;
		hj_color32	u32Color;
		hj_f32		f32u, f32v;
	};

	struct stcFormat_PNC{
		hj_f32		f32x, f32y, f32z;
		hj_f32		f32nx, f32ny, f32nz;
		hj_color32	u32Color;
	};

	// --- Method
	virtual void Begin();	//!< 描画開始
	virtual void End();		//!< 描画終了

	//! プリミティブ描画数の計算
	virtual hj_u32 CalcPrimitiveDrawNum( enmPrimitiveType ePrim, hj_u32 u32ArrayNum )const;
	//! プラットフォーム用頂点に変換(内部でメモリ確保する)
	virtual hj_u8* ConvArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)=0;
	//! 描画
	virtual void RenderArray(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)=0;
	//! 描画(インデックス)
	virtual void RenderArrayIndex(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum)=0;
	//! 描画プラットフォーム高速版
	virtual void RenderArrayPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)=0;
	//! 描画プラットフォーム高速版(インデックス)
	virtual void RenderArrayIndexPlatform(enmFormat eFmt, enmPrimitiveType ePrim, const void* opvVertexArray, hj_u32 u32VertexNum, const void* opvIndexArray, hj_u32 u32IndexNum)=0;

	// --- Is
	hj_bool IsBegin()const{return m_bBegin;};	//!< 開始中か

private:
	hj_bool m_bBegin;
};

}// namespace hj

#endif // __HJ_DRAW_3D_H_