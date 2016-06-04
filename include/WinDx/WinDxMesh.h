//--------------------------------------------------------------
/*!	@file
	@brief	WinDxメッシュ
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_MESH_H_
#define __WINDX_MESH_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDx メッシュクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxMesh : public clsHJMesh
{
public:
	clsWinDxMesh();
	virtual ~clsWinDxMesh();
	
	// --- Method
	//! 描画
	virtual void Render();	
	//! 共通の頂点からプラットフォーム用のメッシュ作成
	virtual void CreateMesh(clsHJDraw3d::enmFormat eFmt, clsHJDraw3d::enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum);	

protected:
	// --- Variable
	hj_u8* m_pu8PfVertexList;	//!< 各頂点タイプリストが複製される
};

}// namespace hj

#endif // __WINDX_MESH_H_