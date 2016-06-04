//--------------------------------------------------------------
/*!	@file
	@brief	WinDxメッシュ
			
	@author 本城 直志
	@date	13/12/14
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxMesh::clsWinDxMesh():clsHJMesh()
, m_pu8PfVertexList(NULL)
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxMesh::~clsWinDxMesh()
{
	HJ_SAFE_DELETE_ARRAY(m_pu8PfVertexList);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxMesh::Render()
{
	clsHJDraw3d& rcDraw3d = *clsHJCoreMgr::RS().GetDraw3d();
	rcDraw3d.RenderArrayPlatform(m_eFmt, m_ePrim, m_pu8PfVertexList, m_u32ArrayNum);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	共通の頂点からプラットフォーム用のメッシュ作成
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxMesh::CreateMesh(clsHJDraw3d::enmFormat eFmt, clsHJDraw3d::enmPrimitiveType ePrim, const void* opvDataArray, hj_u32 u32ArrayNum)
{
	clsHJDraw3d& rcDraw3d = *clsHJCoreMgr::RS().GetDraw3d();

	HJ_SAFE_DELETE_ARRAY(m_pu8PfVertexList);
	m_eFmt = eFmt;
	m_ePrim = ePrim;
	m_u32ArrayNum = u32ArrayNum;
	m_pu8PfVertexList = rcDraw3d.ConvArray(eFmt, ePrim, opvDataArray, u32ArrayNum);
}

}// namespace hj