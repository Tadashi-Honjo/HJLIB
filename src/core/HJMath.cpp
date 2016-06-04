//--------------------------------------------------------------
/*!	@file
	@brief	数学クラス
			
	@author 本城 直志
	@date	11/05/14
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

// --- Math
const hj_f32 clsHJMath::m_tof32PI = 3.141592654f;	// π

// まとめる
#if defined(PF_WINDX) || defined(PF_WINGLES)
	#define HJ_PF_MATH_BASIC
#endif

#if defined(HJ_PF_MATH_BASIC)
	const hj_s32 clsHJMath::m_tos32RandMax = RAND_MAX ;	//!< random最大値
#else
	#error "not support!!"
#endif


namespace{
	// 乗算だけで対応できるように事前計算
	hj_f32	tof32DegToRadParam = clsHJMath::m_tof32PI / 180.f;
	hj_f32	tof32RadToDegParam = 180.f / clsHJMath::m_tof32PI;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Math
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
// abs
hj_f32	clsHJMath::Abs(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC) 
	return ::abs(of32Param);
#else
	#error "not support!!"
#endif
}
// sqrt
hj_f32	clsHJMath::Sqrt(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::sqrtf(of32Param);
#else
	#error "not support!!"
#endif
}
// pow
hj_f32	clsHJMath::Pow(const hj_f32 f32X, const hj_f32 f32Y)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::powf(f32X, f32Y);
#else
	#error "not support!!"
#endif
}

// random
hj_s32 clsHJMath::Random()
{
#if defined(HJ_PF_MATH_BASIC)
	return ::rand();
#else
	#error "not support!!"
#endif
}

// Random(min~max)
hj_s32 clsHJMath::Random(hj_s32 s32Min, hj_s32 s32Max)
{
	hj_s32 s32Interval = s32Max - s32Min;
	if(s32Interval==0) s32Interval=1;
	return (clsHJMath::Random() % (s32Interval+1)) + s32Min;
}

// Random(min~max) float
hj_f32 clsHJMath::Random(hj_f32 f32Min, hj_f32 f32Max)
{
	hj_f32 f32Param = static_cast<hj_f32>(clsHJMath::Random()) / static_cast<hj_f32>(clsHJMath::m_tos32RandMax);
	return ((f32Max - f32Min) * f32Param) + f32Min;
}

//!< floor() float
hj_f32 clsHJMath::Floor(hj_f32 f32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::floorf(f32Param);
#else
	#error "not support!!"
#endif
}

// --- Angle
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	deg→rad
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::DegToRad(const hj_f32 of32Deg)
{
	return of32Deg * tof32DegToRadParam;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	rad→deg
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::RadToDeg(const hj_f32 of32Rad)
{
	return of32Rad * tof32RadToDegParam;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	degを0~360に変換
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::DegNormalize(hj_f32 f32Deg)
{
	if(f32Deg < 0 ){
		return static_cast<hj_f32>((static_cast<hj_s32>(f32Deg)%360))+360;
	}else{
		return static_cast<hj_f32>((static_cast<hj_s32>(f32Deg)%360));
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	近い方のdeg値
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::DegFastDeg(hj_f32 f32DegTarget, hj_f32 f32Deg)
{
	f32DegTarget = clsHJMath::DegNormalize(f32DegTarget);
	f32Deg = clsHJMath::DegNormalize(f32Deg);
	hj_f32 f32DegOut = f32DegTarget;
	hj_f32 f32DegDiff = f32DegTarget - f32Deg;
	if( f32DegTarget >= f32Deg ){
		if( f32DegDiff >= 180.0f){
			f32DegOut = f32DegTarget - 360.f;
		}
	}else{
		if( f32DegDiff <= -180.0f){
			f32DegOut = f32DegTarget + 360.f;
		}
	}
//	return f32DegOut;
	return f32DegOut - f32Deg;
}

// --- Trigonometric Function
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	sin
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::Sin(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::sinf(of32Param);
#else
	#error "not support!!"
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	cos
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::Cos(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::cosf(of32Param);
#else
	#error "not support!!"
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	tan
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::Tan(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::tanf(of32Param);
#else
	#error "not support!!"
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	asin
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::Asin(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::asinf(of32Param);
#else
	#error "not support!!"
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	acos
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::Acos(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::acosf(of32Param);
#else
	#error "not support!!"
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	atan
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::Atan(const hj_f32 of32Param)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::atanf(of32Param);
#else
	#error "not support!!"
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	atan2
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32	clsHJMath::Atan2(const hj_f32 of32x, const hj_f32 of32y)
{
#if defined(HJ_PF_MATH_BASIC)
	return ::atan2(of32x, of32y);
#else
	#error "not support!!"
#endif
}

}// namespace hj