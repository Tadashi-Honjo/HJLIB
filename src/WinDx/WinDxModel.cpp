//--------------------------------------------------------------
/*!	@file
	@brief	WinDxモデル
			
	@author 本城 直志
	@date	14/02/24
*/
//	Copyright (C) 2014 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
}

//! clsWinDxRenderTargetクラス
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxModel::clsWinDxModel():MyClassParent()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxModel::~clsWinDxModel()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxModel::Render()
{
	if(!IsSetModel()){
		return;
	}

	// super
	MyClassParent::Render();

	clsHJCoreMgr& rcCoreMgr = clsHJCoreMgr::RS();
	clsHJGraphics& rcGraphics = *rcCoreMgr.GetGraphics();
	clsHJDraw3d& rcDraw3d = *rcCoreMgr.GetDraw3d();
	SpBinary spModelBuff = GetModelBuffer();
	const hj_u8* opu8Buffer = spModelBuff->GetBuffer();
	
	const stcModelHeader* opsModelHeader = reinterpret_cast<const stcModelHeader*>(opu8Buffer);
	const stcMeshHeader* aopsMeshHeader = reinterpret_cast<const stcMeshHeader*>(opu8Buffer + sizeof(stcModelHeader));
	const stcMeshHeader* opsMeshHeader = NULL;
	const clsWinDxDraw3d::stcWinDxFormat_PNCT* opsVertexs = NULL;
	const hj_u32* opu32Indexs = NULL;

	// Draw Mode
	enmDrawMode eDrawMode = GetDrawMode();
	clsHJDraw3d::enmPrimitiveType ePrimType = clsHJDraw3d::PT_TRIANGLELIST;
	switch(eDrawMode){
		case DRAW_MODE_WIREFRAME:
			ePrimType = clsHJDraw3d::PT_LINELIST;
			break;
		case DRAW_MODE_POINT:
			ePrimType = clsHJDraw3d::PT_POINTLIST;
			break;
	};

	// World Matrix
	rcGraphics.SetWorldMatrix(*GetWorldMatrix());

	// Begin
	hj_bool bBegin = rcDraw3d.IsBegin();
	if(!bBegin) rcDraw3d.Begin();

	// Draw Mesh
	for(hj_u32 u32i=0; u32i<opsModelHeader->u32MeshNum; ++u32i){
		opsMeshHeader = &aopsMeshHeader[u32i];
		opsVertexs = reinterpret_cast<const clsWinDxDraw3d::stcWinDxFormat_PNCT*>(opu8Buffer + opsMeshHeader->u32VertexListOffset);
		opu32Indexs = reinterpret_cast<const hj_u32*>(opu8Buffer + opsMeshHeader->u32VertexIndexListOffset);
		rcDraw3d.RenderArrayIndexPlatform(clsHJDraw3d::FMT_PCTN, ePrimType, opsVertexs, opsMeshHeader->u32VertexNum, opu32Indexs, opsMeshHeader->u32VertexIndexNum );
	}

	// End
	if( !bBegin ){
		rcDraw3d.End();
	}
}

}// namespace hj