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
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample : ShaderDistortion");

	hj_f32 g_f32OffsetX = 0;
	hj_f32 g_f32OffsetY = 0;
	hj::clsHJDraw2d::stcFormat_PC g_sTestVertexs[4] = {
		{-200 + g_f32OffsetX, 200 + g_f32OffsetY, hj::HJ_RED},
		{ 200 + g_f32OffsetX, 200 + g_f32OffsetY, hj::HJ_BLUE},
		{-200 + g_f32OffsetX, -200 + g_f32OffsetY, hj::HJ_GREEN},
		{ 200 + g_f32OffsetX, -200 + g_f32OffsetY, hj::HJ_WHITE},
	};
}


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
	
	// ���[�U�[�p�X�V
	virtual	void Update();
	// ���[�U�[�p�`��
	virtual	void Render();
	// �C���X�^���X�폜
	virtual	void DeleteInstance(){}
	// ������
	void Init();

private:
	hj::SpHJRenderTarget	m_spScreenRenderTarget;
	hj::SpHJRenderTarget	m_spShaderRenderTarget;
	hj::SpTextureResource	m_spTexRes;
	hj::SpHJTexture			m_spTexture;
	hj::SpHJSprite			m_spSprite;
	hj::SpHJSprite			m_spMainScreenSprite;
	hj::SpHJSprite			m_spShaderScreenSprite;
	hj::SpShaderResource	m_spShaderRes;
	hj::SpHJShader			m_spShader;
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

	// �o�b�N�o�b�t�@�Ɠ����X�N���[���o�b�t�@�𐶐�
	m_spScreenRenderTarget.Reset( NEW hj::clsPfRenderTarget() );
	m_spScreenRenderTarget->CreateRenderTarget( spGraphics->GetScreenWidth(), spGraphics->GetScreenHeight() );
	m_spShaderRenderTarget.Reset( NEW hj::clsPfRenderTarget() );
	m_spShaderRenderTarget->CreateRenderTarget( spGraphics->GetScreenWidth(), spGraphics->GetScreenHeight() );

	// ���C���V�[���`���sprite
	m_spMainScreenSprite.Reset( NEW hj::clsPfSprite() );
	m_spMainScreenSprite->SetTexture( m_spScreenRenderTarget->GetTexture() );
	m_spShaderScreenSprite.Reset( NEW hj::clsPfSprite() );
	m_spShaderScreenSprite->SetTexture( m_spShaderRenderTarget->GetTexture() );

	// �e�N�X�`��
	m_spTexRes.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("Distortion.dds"), m_spTexRes );
	m_spTexture = m_spTexRes->GetTexture();

	// �X�v���C�g
	m_spSprite.Reset( NEW hj::clsPfSprite() );
	m_spSprite->SetTexture( m_spTexture );
	m_spSprite->SetPosition( 0.f, 0.f );

	// �V�F�[�_�[�쐬
	m_spShaderRes.Reset( NEW hj::clsShaderResource() );
	spFileMgr->RequestLoadResource< hj::clsShaderResource >( HJ_T("ShaderDistortion.fx"), m_spShaderRes );
	m_spShader = m_spShaderRes->GetShader();
}

//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�X�V
	
	@author �{��
*///��������������������������������������������������������
void clsMain::Update()
{
	static hj_f32 f32Cnt = 0.0f;
	f32Cnt += 1.0f;
	m_spSprite->SetRotation( f32Cnt * 0.01f);
	m_spSprite->SetPosition( hj::clsHJMath::Cos( f32Cnt * 0.01f) * 300.0f, 0.0f);
}
//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�`��
	
	@author �{��
*///��������������������������������������������������������
void clsMain::Render()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJDraw2d		spDraw2d	= rcCoreMgr.GetDraw2d();
	
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	// �V�[���`��
	spGraphics->SetRenderTarget( m_spScreenRenderTarget );
	spGraphics->SetBackColor( hj::HJ_BLACK );
	spGraphics->ClearRenderTarget();

	// ���_�o�b�t�@������
	spDraw2d->Begin();
	spDraw2d->RenderArray( hj::clsHJDraw2d::FMT_PC, hj::clsHJDraw2d::PT_TRIANGLESTRIP, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs));
	spDraw2d->End();

	// �c�ݗp�o�b�t�@
	spGraphics->SetRenderTarget( m_spShaderRenderTarget );
	spGraphics->SetBackColor( hj::HJ_BLACK );
	spGraphics->ClearRenderTarget();
	m_spSprite->Render();

	// �o�b�N�o�b�t�@�ɕ`��
	spGraphics->SetRenderTargetBackBuffer();	
	spGraphics->SetBackColor( hj::HJ_BLACK );
	spGraphics->ClearRenderTarget();

	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	// �c�݃V�F�[�_�[�ݒ�
	m_spShader->SetTechnique("TShaderDistortion");
	m_spShader->SetTexture( "textureScene", m_spScreenRenderTarget->GetTexture() );	// �`�悵���V�[���̃e�N�X�`����ݒ肵�Ă���
	spGraphics->SetShader( m_spShader );
	m_spShaderScreenSprite->Render();

	spGraphics->ResetShader();
}

//���������������������������������������������������������� 
/*!	@brief	WinMain�֐�
	
	@author �{��
*///��������������������������������������������������������
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT iCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// ���������[�N���o

	// �������p�����[�^
	hj::stcPfInitParam sInitParam;
	hj::String::Tcscpy_s( sInitParam.atcApplicationName, HJ_ARRAY_NUM( sInitParam.atcApplicationName ), oatcApplicationName );
	sInitParam.s32ScreenWidth	= 800;
	sInitParam.s32ScreenHeight	= 600;
	sInitParam.bFullScreen		= false;
	sInitParam.hInst			= hInst;
	sInitParam.iCmdShow			= iCmdShow;

	// ���C��
	clsMain	cMain;
	cMain.Initialize( sInitParam );	// ������

	// ���[�U�[������
	cMain.Init();

	// �w�i�F
	if( hj::SpHJGraphics spGraphics = hj::clsHJCoreMgr::GS()->GetGraphics() ){
		spGraphics->SetBackColor( hj::HJ_DARKGRAY );
	}

	// ���[�v�J�n
	hj_s32 s32Result = cMain.StartLoop();

	return s32Result;
}
