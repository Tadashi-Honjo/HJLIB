//--------------------------------------------------------------
/*!	@file
	@brief	数学クラス
			
	@author 本城 直志
	@date	11/05/14
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_MATH_H_
#define __HJ_LIB_MATH_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	数学クラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJMath
{
public:
	clsHJMath(){}								//!< constucter
	~clsHJMath(){}								//!< destucter

	// --- Variable
	static const hj_f32	m_tof32PI;						//!< π 
	static const hj_s32	m_tos32RandMax;					//!< random最大値
	
	// --- Math
	static hj_f32	Abs(const hj_f32 of32Param);							//!< 絶対値
	static hj_f32	Sqrt(const hj_f32 of32Param);							//!< sqrt
	static hj_f32	Pow(const hj_f32 f32X, const hj_f32 f32Y);				//!< pow
	static hj_s32	Random();												//!< random
	static hj_s32	Random(hj_s32 s32Min, hj_s32 s32Max);					//!< Random(min~max)
	static hj_f32	Random(hj_f32 f32Min, hj_f32 f32Max);					//!< Random(min~max) float
	static hj_f32	Floor(hj_f32 f32Param);									//!< floor() float
	
	// --- Angle
	static hj_f32	DegToRad(const hj_f32 of32Deg);		//!< deg→rad
	static hj_f32	RadToDeg(const hj_f32 of32Rad);		//!< rad→deg
	static hj_f32	DegNormalize( hj_f32 f32Deg);	//!< degを0~360に変換
	static hj_f32	DegFastDeg(hj_f32 f32DegTarget, hj_f32 f32Deg);	//!< 近い方のdeg値
	
	// --- Trigonometric Function
	static hj_f32	Sin(const hj_f32 of32Param);						//!< sin
	static hj_f32	Cos(const hj_f32 of32Param);						//!< cos
	static hj_f32	Tan(const hj_f32 of32Param);						//!< tan
	static hj_f32	Asin(const hj_f32 of32Param);						//!< asin
	static hj_f32	Acos(const hj_f32 of32Param);						//!< acos
	static hj_f32	Atan(const hj_f32 of32Param);						//!< atan
	static hj_f32	Atan2(const hj_f32 of32x, const hj_f32 of32y);		//!< atan2
};
	
}// namespace hj

#endif // __HJ_LIB_MATH_H_