//--------------------------------------------------------------
/*!	@file
	@brief	Vector
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

// --- Vector
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3加算
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void 	stcVector3::Add(stcVector3* psOutVec, const stcVector3& orsVec1, const stcVector3& orsVec2)
{
#if defined(PF_WINDX)
	D3DXVec3Add(psOutVec->GetOrigPtr(), orsVec1.GetOrigConstPtr(), orsVec2.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3減算
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void 	stcVector3::Sub(stcVector3* psOutVec, const stcVector3& orsVec1, const stcVector3& orsVec2)
{
#if defined(PF_WINDX)
	D3DXVec3Subtract(psOutVec->GetOrigPtr(), orsVec1.GetOrigConstPtr(), orsVec2.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3スケール
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void 	stcVector3::Scale(stcVector3* psOutVec, const stcVector3& orsVec, hj_f32 f32Scale)
{
#if defined(PF_WINDX)
	D3DXVec3Scale(psOutVec->GetOrigPtr(), orsVec.GetOrigConstPtr(), f32Scale);
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3外積
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void 	stcVector3::Cross(stcVector3* psOutVec, const stcVector3& orsVec1, const stcVector3& orsVec2)
{
#if defined(PF_WINDX)
	D3DXVec3Cross(psOutVec->GetOrigPtr(), orsVec1.GetOrigConstPtr(), orsVec2.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3内積
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 	stcVector3::Dot(const stcVector3& orsVec1, const stcVector3& orsVec2)
{
#if defined(PF_WINDX)
	return D3DXVec3Dot(orsVec1.GetOrigConstPtr(), orsVec2.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
	return 0.f;
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3正規化(xyz)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void 	stcVector3::Normalize3(stcVector3* psOutVec, const stcVector3& orsVec)
{
#if defined(PF_WINDX)
	D3DXVec3Normalize( psOutVec->GetOrigPtr(), orsVec.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3大きさ(xyz)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 	stcVector3::Length3(const stcVector3& orsVec)
{
#if defined(PF_WINDX)
	return D3DXVec3Length( orsVec.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
	return 0.f;
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3大きさ2乗(xyz)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 	stcVector3::Length3Sq(const stcVector3& orsVec)
{
#if defined(PF_WINDX)
	return D3DXVec3LengthSq( orsVec.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
	return 0.f;
#endif
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3正規化(xy)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void 	stcVector3::Normalize2(stcVector3* psOutVec, const stcVector3& orsVec)
{
#if defined(PF_WINDX)
	D3DXVECTOR2 sOut, sIn(orsVec.x, orsVec.y);
	D3DXVec2Normalize( &sOut, &sIn);
	psOutVec->Set(sOut.x, sOut.y, 0.f);
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3大きさ(xy)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 	stcVector3::Length2(const stcVector3& orsVec)
{
#if defined(PF_WINDX)
	return D3DXVec2Length( &D3DXVECTOR2(orsVec.x, orsVec.y) );
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
	return 0.f;
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3大きさ2乗(xy)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 	stcVector3::Length2Sq(const stcVector3& orsVec)
{
#if defined(PF_WINDX)
	return D3DXVec2LengthSq( &D3DXVECTOR2(orsVec.x, orsVec.y) );
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
	return 0.f;
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	行列変換
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void	stcVector3::TransformMatrix(stcVector3* psOutVec, const stcVector3& orsVec, const stcMatrix44& orsMtx)
{
#if defined(PF_WINDX)
	D3DXVec3TransformCoord( psOutVec->GetOrigPtr(), orsVec.GetOrigConstPtr(), orsMtx.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	行列変換
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void	stcVector3::TransformMatrix(const stcMatrix44& orsMtx)
{
	stcVector3 sVecTmp(*this);	// 念のため別バッファに設定
	stcVector3::TransformMatrix(this, sVecTmp, orsMtx);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3 assignment operators
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
stcVector3& stcVector3::operator += ( const stcVector3& orsVec )
{
	Add(this, *this, orsVec);
	return *this;
}
stcVector3& stcVector3::operator -= ( const stcVector3& orsVec )
{
	Sub(this, *this, orsVec);
	return *this;
}
stcVector3& stcVector3::operator *= ( hj_f32 f32Param )
{
	Scale(this, *this, f32Param);
	return *this;
}
stcVector3& stcVector3::operator /= ( hj_f32 f32Param )
{
	Scale(this, *this, 1.0f/f32Param);
	return *this;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3 binary operators
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
stcVector3 stcVector3::operator + ( const stcVector3& orsVec) const
{
	stcVector3 sTmp;
	Add(&sTmp, *this, orsVec);
	return sTmp;
}
stcVector3 stcVector3::operator - ( const stcVector3& orsVec) const
{
	stcVector3 sTmp;
	Sub(&sTmp, *this, orsVec);
	return sTmp;
}
stcVector3 stcVector3::operator * ( hj_f32 f32Param) const
{
	stcVector3 sTmp;
	Scale(&sTmp, *this, f32Param);
	return sTmp;
}
stcVector3 stcVector3::operator / ( hj_f32 f32Param) const
{
	stcVector3 sTmp;
	Scale(&sTmp, *this, 1.0f/f32Param);
	return sTmp;
}

hj_bool stcVector3::operator == ( const stcVector3& orsVec) const
{
	return (this->x == orsVec.x && this->y == orsVec.y && this->z == orsVec.z);
}
hj_bool stcVector3::operator != ( const stcVector3& orsVec) const
{
	return !(*this == orsVec);
}


}// namespace hj