//���������������������������������������������������������� 
/*!	@file
	@brief	HJLib Sample Main
			
	@author �{�� ���u
	@date	11/05/24
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//���������������������������������������������������������� 

#include <core/HJString.h>

namespace{

	//�{�� �A�v���P�[�V������
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample Draw3d");

	static const hj_f32 g_f32OffsetX = 0;
	static const hj_f32 g_f32OffsetY = 0;
	static const hj_f32 g_f32SizeHalf = 0.5f;
	static const hj_f32 g_f32SizeHalf2 = g_f32SizeHalf;
	static const hj::clsHJDraw3d::stcFormat_PCT g_sTestVertexs[] = {
		// z
		{-g_f32SizeHalf, -g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf, -g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf, -g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf,  g_f32SizeHalf, g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 1.f},


		{-g_f32SizeHalf, -g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf, -g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf, -g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf,  g_f32SizeHalf, -g_f32SizeHalf2, hj::HJ_WHITE, 1.f, 1.f},

		// x
		{ g_f32SizeHalf2, -g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{ g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{ g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf2,  g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},


		{-g_f32SizeHalf2, -g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{-g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{-g_f32SizeHalf2, -g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf2,  g_f32SizeHalf, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{-g_f32SizeHalf2,  g_f32SizeHalf,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},

		// y
		{-g_f32SizeHalf,  g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf,  g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf,  g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf,  g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf,  g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},


		{-g_f32SizeHalf, -g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 0.f, 0.f},
		{ g_f32SizeHalf, -g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf, -g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},

		{ g_f32SizeHalf, -g_f32SizeHalf2,  g_f32SizeHalf, hj::HJ_WHITE, 1.f, 0.f},
		{-g_f32SizeHalf, -g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 0.f, 1.f},
		{ g_f32SizeHalf, -g_f32SizeHalf2, -g_f32SizeHalf, hj::HJ_WHITE, 1.f, 1.f},
	};

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
	
	virtual	void Update();	// ���[�U�[�p�X�V
	virtual	void Render();	// ���[�U�[�p�`��

	// �C���X�^���X�폜
	virtual	void DeleteInstance(){};
	// ������
	void Init();

private:
	hj::SpTextureResource	m_spTexRes;
	hj::SpHJTexture			m_spTexture;
	hj::SpHJSprite			m_spSprite;
	hj::SpHJMesh			m_spMesh;
	
};

//���������������������������������������������������������� 
/*!	@brief	������
	
	@author �{��
*///��������������������������������������������������������
void clsMain::Init()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJFileMgr		spFileMgr	= rcCoreMgr.GetFileMgr();

	// �e�N�X�`��
	m_spTexRes.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("test.dds"), m_spTexRes );
	m_spTexture = m_spTexRes->GetTexture();

	//�{�� �X�v���C�g
	m_spSprite.Reset( NEW hj::clsPfSprite() );
	m_spSprite->SetTexture( m_spTexture );
	m_spSprite->SetPosition(0.f, 0.f);

	// ���b�V��(�������Ńv���b�g�t�H�[���p�ɕϊ����č�����)
	m_spMesh.Reset( NEW hj::clsPfMesh() );
	m_spMesh->CreateMesh( hj::clsHJDraw3d::FMT_PCT, hj::clsHJDraw3d::PT_TRIANGLELIST, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs));
}

//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�X�V
	
	@author �{��
*///��������������������������������������������������������
void clsMain::Update()
{
	static hj_f32 f32Cnt = 0.0f;
	f32Cnt += 1.0f;
	m_spSprite->SetRotation(f32Cnt * 0.01f);
	m_spSprite->SetPosition( cos(f32Cnt * 0.01f) * 100.0f, 0.0f);
}
//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�`��
	
	@author �{��
*///��������������������������������������������������������
void clsMain::Render()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJDraw3d		spDraw3d	= rcCoreMgr.GetDraw3d();
	
	//�{�� �`��
	spGraphics->SetBackColor( hj::HJ_GRAY );
	spGraphics->ClearRenderTarget();
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );
	spGraphics->SetTexture( m_spTexture );

	hj::stcMatrix44 sMtx;

	// �r���[�}�g���N�X
	static const hj_f32 tof32CameraLength = 30.f;
	sMtx.SetCameraView(
		hj::stcVector3(tof32CameraLength, tof32CameraLength, -tof32CameraLength),
		hj::stcVector3(0.f, 0.f, 0.f),
		hj::stcVector3(0.f, 1.f, 0.f)
	);
	spGraphics->SetViewMatrix(sMtx);

	// �v���W�F�N�V����
	sMtx.SetCameraProjection( hj::clsHJMath::m_tof32PI/4.0f, 800.f/600.f, 0.01f, 1000000.0f);
	spGraphics->SetProjectionMatrix( sMtx );

	static const hj_s32 tos32MeshNum				= 32;
	static const hj_f32 tof32MeshInterval			= 1.f;
	static const hj_f32 tof32MeshIntervalAll		= static_cast<hj_f32>(tos32MeshNum)*tof32MeshInterval;
	static const hj_f32 tof32MeshIntervalAll_half	= tof32MeshIntervalAll * 0.5f;

	for(hj_s32 s32i=0; s32i<tos32MeshNum; ++s32i)
	{
		for(hj_s32 s32j=0; s32j<tos32MeshNum; ++s32j)
		{
			for(hj_s32 s32k=0; s32k<tos32MeshNum; ++s32k)
			{
				sMtx.SetRotationY( hj::System::GetGameTime() * 100.f);
				sMtx.RotationX( hj::System::GetGameTime() * 100.f);
				sMtx.Translate( hj::stcVector3(	(tof32MeshInterval*static_cast<hj_f32>(s32i))-tof32MeshIntervalAll_half, (tof32MeshInterval*static_cast<hj_f32>(s32j))-tof32MeshIntervalAll_half, (tof32MeshInterval*static_cast<hj_f32>(s32k))-tof32MeshIntervalAll_half) );
				spGraphics->SetWorldMatrix(sMtx);
#if 0
				//�{�� ���_�o�b�t�@������
				spDraw3d->Begin();
				spDraw3d->RenderArray( hj::clsHJDraw3d::FMT_PCT, hj::clsHJDraw3d::PT_TRIANGLELIST, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs));
				spDraw3d->End();
#else
				m_spMesh->Render();
#endif
			}
		}
	}
	
	// sprite 
//	m_spSprite->Render();
}

//���������������������������������������������������������� 
/*!	@brief	WinMain�֐�
	
	@author �{��
*///��������������������������������������������������������
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT iCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// ���������[�N���o

	//�{�� �������p�����[�^
	hj::stcPfInitParam sInitParam;
	hj::String::Tcscpy_s( sInitParam.atcApplicationName, HJ_ARRAY_NUM( sInitParam.atcApplicationName ), oatcApplicationName);
	sInitParam.s32ScreenWidth	= 800;
	sInitParam.s32ScreenHeight	= 600;
	sInitParam.bFullScreen		= false;
	sInitParam.hInst			= hInst;
	sInitParam.iCmdShow			= iCmdShow;

	//�{�� ���C��
	clsMain	cMain;
	//�{�� ������
	hj_s32 s32Result = 0;
	if( cMain.Initialize( sInitParam ) )
	{
		//�{�� ���[�U�[������
		cMain.Init();

		//�{�� �w�i�F
		hj::SpHJGraphics spGraphics	= hj::clsHJCoreMgr::RS().GetGraphics();
		spGraphics->SetBackColor( hj::HJ_GRAY );

		//�{�� ���[�v�J�n
		s32Result = cMain.StartLoop();
	}

	return s32Result;
}
