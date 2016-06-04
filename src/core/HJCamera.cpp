//--------------------------------------------------------------
/*!	@file
	@brief	カメラ
			
	@author 本城 直志
	@date	14/02/27
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HjLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJCamera::clsHJCamera():
m_sPosition(0.f, 0.f, 0.f)
, m_sViewMtx()
, m_sViewInvMtx()
, m_sProjectionMtx()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJCamera::~clsHJCamera()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビューマトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera::ConvertCameraViewVector(stcVector3& rsVec)
{
	stcVector3 sVec = rsVec;
	rsVec.x = (m_sViewInvMtx.m11*(sVec.x))+(m_sViewInvMtx.m21*(rsVec.y))+(m_sViewInvMtx.m31*(sVec.z));
	rsVec.y = (m_sViewInvMtx.m12*(sVec.x))+(m_sViewInvMtx.m22*(rsVec.y))+(m_sViewInvMtx.m32*(sVec.z));
	rsVec.z = (m_sViewInvMtx.m13*(sVec.x))+(m_sViewInvMtx.m23*(rsVec.y))+(m_sViewInvMtx.m33*(sVec.z));

//	rsVec.transformMatrix(m_sViewInvMtx);
//	rsVec -= m_sPosition;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ビューマトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera::SetView(const stcVector3& orsEye, const stcVector3& orsAt, const stcVector3& orsUp)
{
	m_sPosition = orsEye;
	m_sViewMtx.SetCameraView(orsEye, orsAt, orsUp);
	m_sViewInvMtx = m_sViewMtx;
	m_sViewInvMtx.Inverse();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	プロジェクションマトリクス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera::SetProjection(hj_f32 f32Fovy, hj_f32 f32Aspect, hj_f32 f32NearZ, hj_f32 f32FarZ )
{
	m_sProjectionMtx.SetCameraProjection(f32Fovy, f32Aspect, f32NearZ, f32FarZ);
}

} //namespace hj
