//--------------------------------------------------------------
/*!	@file
	@brief	2Dカメラ
			
	@author 本城 直志
	@date	10/11/25
*/
//	Copyright (C) 2010 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_CAMERA_2D_H_
#define __HJ_CAMERA_2D_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	2Dカメラクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJCamera2d
{
public:
	clsHJCamera2d();
	virtual ~clsHJCamera2d();

	// ----- Set
	void SetScreenSize(hj_u32 u32Width, hj_u32 u32Height);	//!< スクリーンサイズ設定
	void SetPosition(hj_f32 f32x, hj_f32 f32y);				//!< 位置設定
	void SetRotation(hj_f32 f32Deg);						//!< 回転角度設定
	void SetScale(hj_f32 f32Scale);							//!< スケール設定

	// ----- Get
	hj_f32 GetPosX()const{return m_sPosX;}												//!< 座標取得X
	hj_f32 GetPosY()const{return m_sPosY;}												//!< 座標取得Y
	hj_f32 GetScale()const{return m_f32Scale;};											//!< スケール取得
	hj_f32 GetRotation()const{return m_f32RotationDeg;};								//!< 回転角度取得
	const stcMatrix44& GetCameraMtx(){ UpdateCameraMtx(); return m_sCameraMtx44;};		//!< カメラ行列取得
	
	// --- Method
	void VertexTransform(stcVector3* psOut, const stcVector3& orsVertex); //!< カメラ行列で頂点変換

private:
	void UpdateCameraMtx();	//!< カメラ行列更新

private:
	// ----- Variable
	hj_f32			m_sPosX, m_sPosY;								//!< カメラ位置
	hj_f32			m_f32RotationDeg;								//!< カメラ回転角度
	hj_f32			m_f32Scale;										//!< カメラの倍率
	stcMatrix44		m_sCameraMtx44;									//!< カメラ行列
	hj_bool			m_bUpdateMtx;									//!< マトリクスの更新フラグ
	hj_f32			m_f32ScreenWidth, m_f32ScreenHeight;			//!< スクリーンのサイズ
	hj_f32			m_f32ScreenWidth_half, m_f32ScreenHeight_half;	//!< スクリーンの半分サイズ
};

} //namespace hj

#endif // __HJ_CAMERA_2D_H_