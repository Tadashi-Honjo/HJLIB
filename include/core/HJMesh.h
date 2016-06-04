//--------------------------------------------------------------
/*!	@file
	@brief	HJメッシュ
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

#ifndef __HJ_MESH_H_
#define __HJ_MESH_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Meshクラス(共通頂点バッファからのメッシュ)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJMesh
{
public:
	clsHJMesh();
	virtual ~clsHJMesh();
	
	//! 描画
	virtual void Render()=0;
	//! 共通の頂点からプラットフォーム用のメッシュ作成
	virtual void CreateMesh(clsHJDraw3d::enmFormat eFmt, clsHJDraw3d::enmPrimitiveType ePrim, const void* pvDataArray, hj_u32 u32ArrayNum)=0;
	
protected:
	clsHJDraw3d::enmFormat			m_eFmt;
	clsHJDraw3d::enmPrimitiveType	m_ePrim;
	hj_u32							m_u32ArrayNum;
};

}// namespace hj

#endif // __HJ_MESH_H_