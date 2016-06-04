//--------------------------------------------------------------
/*!	@file
	@brief	カメラ
			
	@author 本城 直志
	@date	14/02/27
*/
//	Copyright (C) 2014 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_CAMERA_H_
#define __HJ_CAMERA_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	カメラクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJCamera
{
public:
	clsHJCamera();
	virtual ~clsHJCamera();

	// --- Method
	void ConvertCameraViewVector(stcVector3& rsVec);

	// --- Set
	void SetView(const stcVector3& orsEye, const stcVector3& orsAt, const stcVector3& orsUp);	//!< ビューマトリクス取得
	void SetProjection(hj_f32 f32Fovy, hj_f32 f32Aspect, hj_f32 f32NearZ, hj_f32 f32FarZ );		//!< プロジェクションマトリクス取得

	// --- Get
	const stcMatrix44&	GetViewMatrix()const		{return m_sViewMtx;}
	const stcMatrix44&	GetProjectionMatrix()const	{return m_sProjectionMtx;}
	const stcVector3&	GetPosition()const			{return m_sPosition;}

private:
	// ----- Variable
	stcVector3		m_sPosition;			//!< 位置
	stcMatrix44 	m_sViewMtx;				//!< カメラ行列
	stcMatrix44 	m_sViewInvMtx;			//!< ビュー逆行列
	stcMatrix44 	m_sProjectionMtx;		//!< プロジェクション行列
};

} //namespace hj

#endif // __HJ_CAMERA_H_