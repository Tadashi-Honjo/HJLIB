//--------------------------------------------------------------
/*!	@file
	@brief	Matrix
			
	@author 本城 直志
	@date	11/05/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

stcMatrix44 stcMatrix44::m_tsIdentityMtx;

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44 コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
stcMatrix44::stcMatrix44( const stcMatrix44& orsMtx )
{
	Copy(this, orsMtx);
}
stcMatrix44::stcMatrix44( hj_f32 _11, hj_f32 _12, hj_f32 _13, hj_f32 _14,
            hj_f32 _21, hj_f32 _22, hj_f32 _23, hj_f32 _24,
            hj_f32 _31, hj_f32 _32, hj_f32 _33, hj_f32 _34,
            hj_f32 _41, hj_f32 _42, hj_f32 _43, hj_f32 _44 )
{
	m11=_11; m12=_12; m13=_13; m14=_14;
    m21=_21; m22=_22; m23=_23; m24=_24;
    m31=_31; m32=_32; m33=_33; m34=_34;
    m41=_41; m42=_42; m43=_43; m44=_44;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化(静的メンバ)
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void	stcMatrix44::Initialize()
{
	m_tsIdentityMtx.Identity();
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44コピー
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void	stcMatrix44::Copy(stcMatrix44* psOutMtx, const stcMatrix44& orsMtx)
{
	System::Memcpy(psOutMtx->m, orsMtx.m, sizeof(psOutMtx->m));
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44単位行列化
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void	stcMatrix44::Identity(stcMatrix44* psOutMtx)
{
#if defined(PF_WINDX)
	D3DXMatrixIdentity(psOutMtx->GetOrigPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44単位行列化
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::Identity()
{
	stcMatrix44::Identity(this);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void 	stcMatrix44::Multiply(stcMatrix44* psOutMtx, const stcMatrix44& orsMtx1, const stcMatrix44& orsMtx2)
{
#if defined(PF_WINDX)
	D3DXMatrixMultiply(psOutMtx->GetOrigPtr(), orsMtx1.GetOrigConstPtr(), orsMtx2.GetOrigConstPtr());
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44単位行列化
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::Inverse()
{
#if defined(PF_WINDX)
	D3DXMatrixInverse( GetOrigPtr(), NULL, GetOrigPtr() );
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}


// 直接変換
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	平行移動行列作成
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetTranslate(const stcVector3& orsPos)
{
#if defined(PF_WINDX)
	D3DXMatrixTranslation( GetOrigPtr(), orsPos.x, orsPos.y, orsPos.z);
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	X回転行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetRotationX(hj_f32 f32Deg)
{
#if defined(PF_WINDX)
	D3DXMatrixRotationX( GetOrigPtr(), clsHJMath::DegToRad(f32Deg));
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Y回転行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetRotationY(hj_f32 f32Deg)
{
#if defined(PF_WINDX)
	D3DXMatrixRotationY( GetOrigPtr(), clsHJMath::DegToRad(f32Deg));
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Z回転行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetRotationZ(hj_f32 f32Deg)
{
#if defined(PF_WINDX)
	D3DXMatrixRotationZ( GetOrigPtr(), clsHJMath::DegToRad(f32Deg));
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	回転行列作成
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetRotationXyz(const stcVector3& orsRot)
{
#if defined(PF_WINDX)
	D3DXMatrixRotationYawPitchRoll( GetOrigPtr(), clsHJMath::DegToRad(orsRot.y), clsHJMath::DegToRad(orsRot.x), clsHJMath::DegToRad(orsRot.z));
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	スケール行列作成
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetScale(const stcVector3& orsScale)
{
#if defined(PF_WINDX)
	D3DXMatrixScaling( GetOrigPtr(), orsScale.x, orsScale.y, orsScale.z);
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}


// 行列と乗算変換
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	平行移動行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::Translate(const stcVector3& orsPos)
{
	stcMatrix44 sMtxTmp;
	sMtxTmp.SetTranslate(orsPos);
	*this *= sMtxTmp;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	X回転行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::RotationX(hj_f32 f32Deg)
{
	stcMatrix44 sMtxTmp;
	sMtxTmp.SetRotationX(f32Deg);
	*this *= sMtxTmp;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Y回転行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::RotationY(hj_f32 f32Deg)
{
	stcMatrix44 sMtxTmp;
	sMtxTmp.SetRotationY(f32Deg);
	*this *= sMtxTmp;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Z回転行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::RotationZ(hj_f32 f32Deg)
{
	stcMatrix44 sMtxTmp;
	sMtxTmp.SetRotationZ(f32Deg);
	*this *= sMtxTmp;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	回転行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::RotationXyz(const stcVector3& orsRot)
{
	stcMatrix44 sMtxTmp;
	sMtxTmp.SetRotationXyz(orsRot);
	*this *= sMtxTmp;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	スケール行列乗算
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::Scale(const stcVector3& orsScale)
{
	stcMatrix44 sMtxTmp;
	sMtxTmp.SetScale(orsScale);
	*this *= sMtxTmp;
}

// --- other
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビューマトリクス設定
	@param 
			orsEye		位置
			rsAt		注視位置
			orsUp		上方ベクトル
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetCameraView(const stcVector3& orsEye, const stcVector3& orsAt, const stcVector3& orsUp)
{
#if defined(PF_WINDX)
	D3DXMatrixLookAtLH( 
		GetOrigPtr(), 
		orsEye.GetOrigConstPtr(),
		orsAt.GetOrigConstPtr(),
		orsUp.GetOrigConstPtr()
	);
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プロジェクションマトリクス設定
	@param	
			f32Fovy		視野角
			f32Aspect	画面アスペクト比
			f32NearZ	クリッピング距離（これより近いのは描画しません）
			f32FarZ		クリッピング距離（これより遠いのは描画しません)

	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void stcMatrix44::SetCameraProjection(hj_f32 f32Fovy, hj_f32 f32Aspect, hj_f32 f32NearZ, hj_f32 f32FarZ )
{
#if defined(PF_WINDX)
	D3DXMatrixPerspectiveFovLH( GetOrigPtr(), f32Fovy, f32Aspect, f32NearZ, f32FarZ );
#else
	HJ_ASSERT_MSG( HJ_T("not support!!") );
#endif
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44 assignment operators
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
stcMatrix44& stcMatrix44::operator *= ( const stcMatrix44& orsMtx )
{
	Multiply(this, *this, orsMtx);
	return *this;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix44 binary operators
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
stcMatrix44 stcMatrix44::operator * ( const stcMatrix44& orsMtx ) const
{
	stcMatrix44 sMtx(*this);
	Multiply(&sMtx, sMtx, orsMtx);
	return sMtx;
}

}// namespace hj