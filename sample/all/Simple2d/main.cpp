//���������������������������������������������������������� 
/*!	@file
	@brief	HJLib Sample Simple2d
			
	@author �{�� ���u
	@date	13/10/05
*/
//	Copyright (C) 2012 HonjoTadashi. All Rights Reserved. 
//���������������������������������������������������������� 

#include <core/HJString.h>

namespace{

	//�{�� �A�v���P�[�V������
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample Simple2d");

} // namespace


//���������������������������������������������������������� 
/*!	@brief	MainClass
	
	@author �{��
*///��������������������������������������������������������
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;
public:
	clsMain():MyClassParent(){}
	virtual ~clsMain(){}
	virtual	void Update(){};
	virtual	void Render();
	virtual	void DeleteInstance(){}
};

//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�`��
	
	@author �{��
*///��������������������������������������������������������
void clsMain::Render()
{
	hj::clsHJCoreMgr&	rcCoreMgr = hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics = rcCoreMgr.GetGraphics();
	hj::SpHJDraw2d		spDraw2d = rcCoreMgr.GetDraw2d();
	
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	static const hj_f32 g_f32OffsetX = 0;
	static const hj_f32 g_f32OffsetY = 0;
	static const hj::clsHJDraw2d::stcFormat_PC g_sTestVertexs[4] = {
		{-100 + g_f32OffsetX, -100 + g_f32OffsetY, 0xFFFF0000},
		{ 100 + g_f32OffsetX, -100 + g_f32OffsetY, 0xFF00FF00},
		{-100 + g_f32OffsetX,  100 + g_f32OffsetY, 0xFF0000FF},
		{ 100 + g_f32OffsetX,  100 + g_f32OffsetY, 0xFFFFFFFF},
	};

	spDraw2d->Begin();
	spDraw2d->RenderArray( hj::clsHJDraw2d::FMT_PC, hj::clsHJDraw2d::PT_TRIANGLESTRIP, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs) );
	spDraw2d->End();
}

//���������������������������������������������������������� 
/*!	@brief	WinMain�֐�
	
	@author �{��
*///��������������������������������������������������������
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT iCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// ���������[�N���o

	//�{�� �������p�����[�^
	hj::stcPfInitParam	sInitParam;
	hj::String::Tcscpy_s( sInitParam.atcApplicationName, HJ_ARRAY_NUM( sInitParam.atcApplicationName ), oatcApplicationName);
	sInitParam.s32ScreenWidth	= 800;
	sInitParam.s32ScreenHeight	= 600;
	sInitParam.bFullScreen		= false;
	sInitParam.hInst			= hInst;
	sInitParam.iCmdShow			= iCmdShow;

	//�{�� ���C��
	clsMain	cMain;
	cMain.Initialize( sInitParam );	//�{�� ������

	//�{�� �w�i�F
	hj::SpHJGraphics spGraphics = hj::clsHJCoreMgr::GS()->GetGraphics();
	spGraphics->SetBackColor( hj::HJ_GRAY );

	//�{�� ���[�v�J�n
	hj_s32 s32Result = cMain.StartLoop();

	return s32Result;
}
