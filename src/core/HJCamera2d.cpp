//--------------------------------------------------------------
/*!	@file
	@brief	2Dカメラ
			
	@author 本城 直志
	@date	10/11/25
*/
//	Copyright (C) 2010 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HjLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJCamera2d::clsHJCamera2d():
m_sPosX(0.f)
, m_sPosY(0.f)
, m_f32RotationDeg(0.f)
, m_f32Scale(1.f)
, m_sCameraMtx44()
, m_bUpdateMtx(true)
, m_f32ScreenWidth(0.f), m_f32ScreenHeight(0.f)
, m_f32ScreenWidth_half(0.f), m_f32ScreenHeight_half(0.f)
{
	UpdateCameraMtx();	//カメラ行列更新
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJCamera2d::~clsHJCamera2d()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	スクリーンサイズ設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera2d::SetScreenSize(hj_u32 u32Width, hj_u32 u32Height)
{
	m_f32ScreenWidth = static_cast<hj_f32>(u32Width); 
	m_f32ScreenHeight = static_cast<hj_f32>(u32Height);
	m_f32ScreenWidth_half = m_f32ScreenWidth*0.5f; 
	m_f32ScreenHeight_half = m_f32ScreenHeight*0.5f;
	m_bUpdateMtx=true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	位置設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera2d::SetPosition(hj_f32 f32x, hj_f32 f32y)
{
	m_sPosX=f32x;
	m_sPosY=f32y; 
	m_bUpdateMtx=true;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	回転角度設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera2d::SetRotation(hj_f32 f32Deg)
{
	m_f32RotationDeg=f32Deg; 
	m_bUpdateMtx=true;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	スケール設定
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera2d::SetScale(hj_f32 f32Scale)
{ 
	m_f32Scale=f32Scale; 
	m_bUpdateMtx=true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	カメラ行列で頂点変換
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera2d::VertexTransform(stcVector3* psOut, const stcVector3& orsVertex)
{
	UpdateCameraMtx();	//カメラ行列更新
	stcVector3::TransformMatrix(psOut, orsVertex, m_sCameraMtx44);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	カメラ行列更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJCamera2d::UpdateCameraMtx()
{	
	if(!m_bUpdateMtx){
		return;
	}
	m_bUpdateMtx = false;	// 更新フラグOFF

	//カメラ行列計算
	m_sCameraMtx44.Identity();
	m_sCameraMtx44.RotationZ(m_f32RotationDeg);
	m_sCameraMtx44.Scale(stcVector3(m_f32Scale, m_f32Scale, 1.f));
	m_sCameraMtx44.Translate(stcVector3(m_sPosX, m_sPosY, 0.f));
	m_sCameraMtx44.Inverse();	// 逆行列化
}

} //namespace hj
