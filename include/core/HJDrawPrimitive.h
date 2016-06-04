//--------------------------------------------------------------
/*!	@file
	@brief	Primitive描画
			
	@author 本城 直志
	@date	12/03/25
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

#ifndef __HJ_DRAW_PRIMITIVE_H_
#define __HJ_DRAW_PRIMITIVE_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Draw2d基底クラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJDrawPrimitive
{
public:
	clsHJDrawPrimitive();
	virtual ~clsHJDrawPrimitive();
	
	// --- Method
	virtual void DrawRect(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32Width, hj_f32 f32Height, hj_color32 c32Color);						//!< 矩形描画
	virtual void DrawRectCenter(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32Width, hj_f32 f32Height, hj_color32 c32Color);				//!< 矩形描画(センタリング)
	virtual void DrawLine2d(hj_f32 f32BeginPosX, hj_f32 f32BeginPosY, hj_f32 f32EndPosX, hj_f32 f32EndPosY, hj_color32 c32Color);		//!< ライン描画
	virtual void DrawCircle(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32Length, hj_color32 c32Color);										//!< 円描画
	virtual void DrawLine3d(hj_f32 f32X1, hj_f32 f32Y1, hj_f32 f32Z1, hj_f32 f32X2, hj_f32 f32Y2, hj_f32 f32Z2, hj_color32 c32Color);	//!< 3dライン描画
	virtual void DrawGridXz(hj_f32 f32Length, hj_u32 u32Divide, hj_color32 c32Color, const stcMatrix44* psMatrix=NULL);					//!< XZ平面グリッド描画
	virtual void DrawAxis(hj_f32 f32Length, const stcMatrix44* psMatrix=NULL);															//!< xyz3軸描画
};

}// namespace hj

#endif // __HJ_DRAW_PRIMITIVE_H_