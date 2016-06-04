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

	// �A�v���P�[�V������
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample : ShaderBlur2d");

	hj_f32 g_f32OffsetX = 0;
	hj_f32 g_f32OffsetY = 0;
	hj::clsHJDraw2d::stcFormat_PC g_sTestVertexs[4] = {
		{-200 + g_f32OffsetX, 200 + g_f32OffsetY, hj::HJ_RED	},
		{ 200 + g_f32OffsetX, 200 + g_f32OffsetY, hj::HJ_BLUE	},
		{-200 + g_f32OffsetX, -200 + g_f32OffsetY, hj::HJ_GREEN	},
		{ 200 + g_f32OffsetX, -200 + g_f32OffsetY, hj::HJ_WHITE	},
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
	clsMain():MyClassParent(), m_sPos(0.f, 0.f, 0.f), m_sSpeedVec(0.5f,0.5f,0.5f){}
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
	hj::stcVector3			m_sPos;
	hj::stcVector3			m_sSpeedVec;
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
	m_spScreenRenderTarget->CreateRenderTarget( spGraphics->GetScreenWidth(), spGraphics->GetScreenHeight());
	m_spShaderRenderTarget.Reset( NEW hj::clsPfRenderTarget() );
	m_spShaderRenderTarget->CreateRenderTarget( spGraphics->GetScreenWidth(), spGraphics->GetScreenHeight());

	// ���C���V�[���`���sprite
	m_spMainScreenSprite.Reset( NEW hj::clsPfSprite() );
	m_spMainScreenSprite->SetTexture( m_spScreenRenderTarget->GetTexture() );
	m_spShaderScreenSprite.Reset( NEW hj::clsPfSprite() );
	m_spShaderScreenSprite->SetTexture( m_spShaderRenderTarget->GetTexture() );

	// �e�N�X�`��
	m_spTexRes.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("test.dds"), m_spTexRes );
	m_spTexture = m_spTexRes->GetTexture();

	// �X�v���C�g
	m_spSprite.Reset( NEW hj::clsPfSprite() );
	m_spSprite->SetTexture( m_spTexture );
	m_spSprite->SetPosition( 0.f, 0.f );

	// �V�F�[�_�[�쐬
	m_spShaderRes.Reset( NEW hj::clsShaderResource() );
	spFileMgr->RequestLoadResource< hj::clsShaderResource >( HJ_T("Shader.fx"), m_spShaderRes );
	m_spShader = m_spShaderRes->GetShader();
}

//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�X�V
	
	@author �{��
*///��������������������������������������������������������
void clsMain::Update()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJInput		spInput		= rcCoreMgr.GetInput();

	hj_f32 f32PosX_old, f32PosY_old;
	f32PosX_old = m_sPos.x;
	f32PosY_old = m_sPos.y;

	// �ړ�
	hj_f32 f32MoveSpeed = 10.f;
	m_sPos.x += spInput->GetPadAnalog( hj::clsHJInput::PAD_STICK1_X ) * f32MoveSpeed;
	m_sPos.y += spInput->GetPadAnalog( hj::clsHJInput::PAD_STICK1_Y ) * f32MoveSpeed;
	m_spSprite->SetPosition( m_sPos.x, m_sPos.y);

	// ���x�Ƃ��Ă���(���K�������Ƃ�)
	hj_f32 f32SpeedMaxParam = (1.f / 300.f) * 0.5f;
	m_sSpeedVec.x = (-(m_sPos.x - f32PosX_old) * f32SpeedMaxParam) + 0.5f;
	m_sSpeedVec.y = ((m_sPos.y - f32PosY_old) * f32SpeedMaxParam) + 0.5f;
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

	// ========== �V�[���`��
	spGraphics->SetRenderTarget( m_spScreenRenderTarget );
	spGraphics->SetBackColor( hj::HJ_BLACK );
	spGraphics->ClearRenderTarget();

	// ���_�o�b�t�@������
	spDraw2d->Begin();
	spDraw2d->RenderArray( hj::clsHJDraw2d::FMT_PC, hj::clsHJDraw2d::PT_TRIANGLESTRIP, g_sTestVertexs, HJ_ARRAY_NUM(g_sTestVertexs) );
	spDraw2d->End();

	// �I�u�W�F�N�g��������
	m_spSprite->Render();

	// ========= ���x�}�b�v
	spGraphics->SetRenderTarget( m_spShaderRenderTarget );
	spGraphics->SetBackColor( hj::HJ_COLOR_ARGB_SET(255, 128, 128, 128) );
	spGraphics->ClearRenderTarget();

	// ���x��������
	m_spShader->SetTechnique("TShaderSpeedMap");
	m_spShader->SetVector3("speed", m_sSpeedVec);
	spGraphics->SetShader( m_spShader );
	m_spSprite->Render();
	spGraphics->ResetShader();
	
	// �o�b�N�o�b�t�@�ɕ`��
	spGraphics->SetRenderTargetBackBuffer();	
	spGraphics->SetBackColor( hj::HJ_MAGENTA );
	spGraphics->ClearRenderTarget();
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	// �`��
#if 0
	m_spShaderScreenSprite->Render();
#else
	// ���X�ݒ�
	m_spShader->SetTechnique("TShaderSpeedBlur");
	m_spShader->SetTexture( "textureScene", m_spScreenRenderTarget->GetTexture() );	
	m_spShader->SetTexture( "textureSpeed", m_spShaderRenderTarget->GetTexture() );

	// �V�[����`��(�o�b�t�@�E�T�C�Y�������Ȃ̂Ń_�~�[�Ƃ��čs����͂�)
	spGraphics->SetShader( m_spShader );
	m_spMainScreenSprite->Render();
	spGraphics->ResetShader();
#endif
//	m_spSprite->Render();
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
	hj::String::Tcscpy_s( sInitParam.atcApplicationName, HJ_ARRAY_NUM( sInitParam.atcApplicationName ), oatcApplicationName);
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
