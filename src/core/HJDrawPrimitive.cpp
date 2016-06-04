//--------------------------------------------------------------
/*!	@file
	@brief	Primitive描画
			
	@author 本城 直志
	@date	12/03/25
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
	clsHJDraw2d::stcFormat_PC g_sRectVertexs[4] = {
		{-1.f, -1.f, HJ_WHITE},
		{ 1.f, -1.f, HJ_WHITE},
		{-1.f,  1.f, HJ_WHITE},
		{ 1.f,  1.f, HJ_WHITE},
	};

	clsHJDraw2d::stcFormat_PC g_sLineVertexs2d[2] = {
		{ 0.f, 0.f, HJ_WHITE},
		{ 1.f, 1.f, HJ_WHITE},
	};

	clsHJDraw3d::stcFormat_PC g_sLineVertexs3d[2] = {
		{ 0.f, 0.f, 0.f, HJ_WHITE},
		{ 1.f, 1.f, 1.f, HJ_WHITE},
	};

	// 円の描画用
	const hj_u32 tou32CircleVertexNum = 32;
	clsHJDraw2d::stcFormat_PC g_sCircleVertexs[tou32CircleVertexNum] = {
		{ 0.f, 0.f, HJ_WHITE},
	};

}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJDrawPrimitive::clsHJDrawPrimitive()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJDrawPrimitive::~clsHJDrawPrimitive()
{
}
	
	
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	矩形描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDrawPrimitive::DrawRect(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32Width, hj_f32 f32Height, hj_color32 c32Color)
{
	clsHJDraw2d& rcDraw2d = *clsHJCoreMgr::RS().GetDraw2d();
	hj_bool bBegin = !rcDraw2d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック

	clsHJDraw2d::stcFormat_PC* pcVertex = g_sRectVertexs;
	pcVertex->f32x = f32PosX;
	pcVertex->f32y = f32PosY;
	pcVertex->u32Color = c32Color;
	++pcVertex;

	pcVertex->f32x = f32PosX + f32Width;
	pcVertex->f32y = f32PosY;
	pcVertex->u32Color = c32Color;
	++pcVertex;

	pcVertex->f32x = f32PosX;
	pcVertex->f32y = f32PosY - f32Height;
	pcVertex->u32Color = c32Color;
	++pcVertex;

	pcVertex->f32x = f32PosX + f32Width;
	pcVertex->f32y = f32PosY - f32Height;
	pcVertex->u32Color = c32Color;
	++pcVertex;
	
	if(bBegin)rcDraw2d.Begin();
	rcDraw2d.RenderArray(clsHJDraw2d::FMT_PC, clsHJDraw2d::PT_TRIANGLESTRIP, g_sRectVertexs, 4);
	if(bBegin)rcDraw2d.End();
}
	
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	矩形描画(センタリング)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDrawPrimitive::DrawRectCenter(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32Width, hj_f32 f32Height, hj_color32 c32Color)
{
	DrawRect(f32PosX-(f32Width*0.5f), f32PosY+(f32Height*0.5f), f32Width, f32Height, c32Color);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ライン描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDrawPrimitive::DrawLine2d(hj_f32 f32BeginPosX, hj_f32 f32BeginPosY, hj_f32 f32EndPosX, hj_f32 f32EndPosY, hj_color32 c32Color)
{
	clsHJDraw2d& rcDraw2d = *clsHJCoreMgr::RS().GetDraw2d();
	hj_bool bBegin = !rcDraw2d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック

	clsHJDraw2d::stcFormat_PC* pcVertex = g_sLineVertexs2d;
	pcVertex->f32x = f32BeginPosX;
	pcVertex->f32y = f32BeginPosY;
	pcVertex->u32Color = c32Color;
	++pcVertex;

	pcVertex->f32x = f32EndPosX;
	pcVertex->f32y = f32EndPosY;
	pcVertex->u32Color = c32Color;

	if(bBegin)rcDraw2d.Begin();
	rcDraw2d.RenderArray(clsHJDraw2d::FMT_PC, clsHJDraw2d::PT_LINELIST, g_sLineVertexs2d, 2);
	if(bBegin)rcDraw2d.End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ライン描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDrawPrimitive::DrawCircle(hj_f32 f32PosX, hj_f32 f32PosY, hj_f32 f32Length, hj_color32 c32Color)
{
	clsHJDraw2d& rcDraw2d = *clsHJCoreMgr::RS().GetDraw2d();
	hj_bool bBegin = !rcDraw2d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック
	hj_u32 u32i=0;
	hj_f32 f32Rad = 0.f;
	hj_f32 f32IntervalRad = clsHJMath::DegToRad(360.f) / static_cast<hj_f32>(tou32CircleVertexNum-1);
	clsHJDraw2d::stcFormat_PC* pcVertex = g_sCircleVertexs;

	// 代入していく
	for(u32i=0; u32i<tou32CircleVertexNum; ++u32i, ++pcVertex, f32Rad+=f32IntervalRad){
		pcVertex->f32x = f32PosX + (clsHJMath::Cos(f32Rad) * f32Length);
		pcVertex->f32y = f32PosY + (clsHJMath::Sin(f32Rad) * f32Length);
		pcVertex->u32Color = c32Color;
	}

	if(bBegin)rcDraw2d.Begin();
	rcDraw2d.RenderArray(clsHJDraw2d::FMT_PC, clsHJDraw2d::PT_LINESTRIP, g_sCircleVertexs, tou32CircleVertexNum);
	if(bBegin)rcDraw2d.End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	3dライン描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDrawPrimitive::DrawLine3d(hj_f32 f32X1, hj_f32 f32Y1, hj_f32 f32Z1, hj_f32 f32X2, hj_f32 f32Y2, hj_f32 f32Z2, hj_color32 c32Color)
{
	clsHJDraw3d& rcDraw3d = *clsHJCoreMgr::RS().GetDraw3d();
	hj_bool bBegin = !rcDraw3d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック

	clsHJDraw3d::stcFormat_PC* pcVertex = g_sLineVertexs3d;
	pcVertex->f32x = f32X1;
	pcVertex->f32y = f32Y1;
	pcVertex->f32z = f32Z1;
	pcVertex->u32Color = c32Color;
	++pcVertex;

	pcVertex->f32x = f32X2;
	pcVertex->f32y = f32Y2;
	pcVertex->f32z = f32Z2;
	pcVertex->u32Color = c32Color;

	if(bBegin)rcDraw3d.Begin();
	rcDraw3d.RenderArray(clsHJDraw3d::FMT_PC, clsHJDraw3d::PT_LINELIST, g_sLineVertexs3d, 2);
	if(bBegin)rcDraw3d.End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	XZ平面グリッド描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDrawPrimitive::DrawGridXz(hj_f32 f32Length, hj_u32 u32Divide, hj_color32 c32Color, const stcMatrix44* psMatrix)
{
	clsHJGraphics& rcGraphics = *clsHJCoreMgr::RS().GetGraphics();
	clsHJDraw3d& rcDraw3d = *clsHJCoreMgr::RS().GetDraw3d();
	
	hj_bool bBegin = !rcDraw3d.IsBegin();	// 外部ですでに開始されている可能性があるのでチェック

	// matrix
	if(psMatrix)	rcGraphics.SetWorldMatrix(*psMatrix);
	else			rcGraphics.SetWorldMatrix(*stcMatrix44::GetIndentityMatrix());

	if(bBegin)rcDraw3d.Begin();

	hj_f32 f32Length_half = f32Length * 0.5f;
	hj_f32 f32OneLength = f32Length / static_cast<hj_f32>(u32Divide);
	hj_f32 f32Tmp = 0.f;

	for(hj_u32 u32i=0; u32i<u32Divide+1; ++u32i){
		f32Tmp = (f32OneLength*static_cast<hj_f32>(u32i)) - f32Length_half;
		// x軸
		DrawLine3d(
			//pos1
			f32Tmp,
			0.f,
			-f32Length_half,
			//pos2
			f32Tmp,
			0.f,
			f32Length_half,
			// color
			c32Color
		);
		// z軸
		DrawLine3d(
			//pos1
			-f32Length_half,
			0.f,
			f32Tmp,
			//pos2
			f32Length_half,
			0.f,
			f32Tmp,
			// color
			c32Color
		);
	}

	if(bBegin)rcDraw3d.End();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	xyz3軸描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJDrawPrimitive::DrawAxis(hj_f32 f32Length, const stcMatrix44* psMatrix)
{
	clsHJDraw3d& rcDraw3d = *clsHJCoreMgr::RS().GetDraw3d();
	hj_bool bBegin = !rcDraw3d.IsBegin();

	if(bBegin)rcDraw3d.Begin();

	// x
	DrawLine3d(	0.f, 0.f, 0.f, f32Length, 0.f, 0.f, HJ_RED);
	// y
	DrawLine3d(	0.f, 0.f, 0.f, 0.f, f32Length, 0.f, HJ_GREEN);
	// z
	DrawLine3d(	0.f, 0.f, 0.f, 0.f, 0.f, f32Length, HJ_BLUE);

	if(bBegin)rcDraw3d.End();
}

}// namespace hj