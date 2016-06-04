//--------------------------------------------------------------
/*!	@file
	@brief	HJModel
			
	@author 本城 直志
	@date	14/02/24
*/
//	Copyright (C) 2014 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_MODEL_H_
#define __HJ_MODEL_H_

// HJLib namespace
namespace hj{

// モデルリソース
class clsModelResource : public clsHJResource
{
	typedef clsHJResource MyClassParent;
public:
	clsModelResource():MyClassParent(){}
	virtual ~clsModelResource(){}

	SpHJModel GetModel()const{ return m_spModel; }

protected:
	virtual hj_bool Builded( SpBinary spBinary );

private:
	SpBinary	m_spBinary;
	SpHJModel	m_spModel;
};
typedef clsHJSharedPtr< clsModelResource > SpModelResource;
	
//==================================================
/*!	@brief	モデル
*///================================================
class clsHJModel : public clsHJEntity
{
public:
	typedef clsHJEntity MyClassParent;
	
	clsHJModel();
	virtual ~clsHJModel();

	// --- Structure
	//! モデルデータヘッダ
	struct stcModelHeader{
		hj_c8		header[4];						//!< 'HJMD'
		hj_u8		u8VersionMajor, u8VersionMinor;	//!< バージョン
		hj_u8		u8Pad[2];
		hj_u32		u32MeshNum;						//!< メッシュ数
		// ↓にstcMeshOffsetHeaderがu32MeshNum分並ぶ
	};
	//! メッシュデータヘッダ
	struct stcMeshHeader{
		hj_u32	u32VertexNum;					//!< 頂点数
		hj_u32	u32VertexIndexNum;				//!< 頂点数
		hj_u32	u32VertexListOffset;			//!< 頂点リストへのオフセット(stcMeshHeaderからの)
		hj_u32	u32VertexIndexListOffset;		//!< 頂点インデックスリストのオフセット(stcMeshHeaderからの)
	};

	// --- Enum
	enum enmDrawMode{
		DRAW_MODE_NORMAL,		// 通常
		DRAW_MODE_WIREFRAME,	// ワイヤーフレーム
		DRAW_MODE_POINT,		// ポイント

		DRAW_MODE_NUM,
	};

	// --- Method
	virtual void Render();

	// --- Is
	virtual hj_bool IsSetModel()const{ return m_spBinaryBuff.Get()!=NULL; }

	// --- Get
	SpBinary	GetModelBuffer()const{return m_spBinaryBuff;}
	enmDrawMode	GetDrawMode()const{return m_eDrawMode;}

	// --- Set
	virtual void SetDrawMode(enmDrawMode eDrawMode){m_eDrawMode=eDrawMode;}		//!< 描画モード設定

private:
	virtual void SetModelBuffer(SpBinary spBinaryBuff);							//!< モデルバッファ設定(.hjmd)

	SpBinary		m_spBinaryBuff;
	enmDrawMode		m_eDrawMode;

	friend class clsModelResource;
};
	
}// namespace hj

#endif // __HJ_MODEL_H_