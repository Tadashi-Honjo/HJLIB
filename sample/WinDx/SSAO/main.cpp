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
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample SSAO");
	const hj_u32 tou32ScreenSizeW = 1280;
	const hj_u32 tou32ScreenSizeH = 720;
	const hj_f32 tof32Aspect = static_cast<hj_f32>(tou32ScreenSizeW)/static_cast<hj_f32>(tou32ScreenSizeH);
	const hj_f32 tof32ClipNear = 1.f;
	const hj_f32 tof32ClipFar = 100.f;

} // namespace


//���������������������������������������������������������� 
/*!	@brief	MainClass
*///��������������������������������������������������������
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;
public:
	clsMain()
		: MyClassParent()
		, m_sCameraPos(0.f,5.f,20.f)
		, m_sCameraLookPos(0.f,0.f,0.f)
		, m_f32CameraDegX(20.f)
		, m_f32CameraDegY(45.f)
		, m_f32CameraLength(20.f)
		, m_eCullMode( hj::clsHJGraphics::CULL_CW )
	{
	}
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
	hj::SpTextureResource		m_spTexResDiffuse;
	hj::SpTextureResource		m_spTexResField;
	hj::SpModelResource			m_spModelRes;
	hj::SpShaderResource		m_spShaderRes;
	hj::SpHJTexture				m_spTexDiffuse, m_spTexField;
	hj::SpHJModel				m_spModel;
	hj::SpHJShader				m_spShader;
	hj::SpHJRenderTarget		m_spDepthRenderTarget;
	hj::SpHJSprite				m_spSprite;
	hj::stcVector3				m_sCameraPos, m_sCameraLookPos;
	hj_f32						m_f32CameraDegX, m_f32CameraDegY;
	hj_f32						m_f32CameraLength;
	hj::clsHJGraphics::enmCull	m_eCullMode;
};

//���������������������������������������������������������� 
/*!	@brief	������
*///��������������������������������������������������������
void clsMain::Init()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics	spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJFileMgr		spFileMgr	= rcCoreMgr.GetFileMgr();

	const hj_u32 u32ScreenW = spGraphics->GetScreenWidth();
	const hj_u32 u32ScreenH = spGraphics->GetScreenHeight();

	// �e�N�X�`��
	m_spTexResDiffuse.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("diffuse.dds"), m_spTexResDiffuse );
	m_spTexDiffuse = m_spTexResDiffuse->GetTexture();
	m_spTexResField.Reset( NEW hj::clsTextureResource() );
	spFileMgr->RequestLoadResource< hj::clsTextureResource >( HJ_T("field.dds"), m_spTexResField );
	m_spTexField = m_spTexResField->GetTexture();

	// ���f��
	m_spModelRes.Reset( NEW hj::clsModelResource() );
	spFileMgr->RequestLoadResource< hj::clsModelResource >( HJ_T("test.hjmd"), m_spModelRes );
	m_spModel = m_spModelRes->GetModel();

	// �V�F�[�_�[�쐬
	m_spShaderRes.Reset( NEW hj::clsShaderResource() );
	spFileMgr->RequestLoadResource< hj::clsShaderResource >( HJ_T("Shader.fx"), m_spShaderRes );
	m_spShader = m_spShaderRes->GetShader();

	// �[�x�����_�[�^�[�Q�b�g
	m_spDepthRenderTarget.Reset( NEW hj::clsPfRenderTarget() );
	m_spDepthRenderTarget->CreateRenderTarget( tou32ScreenSizeW, tou32ScreenSizeH );

	// �X�v���C�g
	m_spSprite.Reset( NEW hj::clsPfSprite() );
	m_spSprite->SetTexture( m_spDepthRenderTarget->GetTexture() );
	hj_f32 f32Scale = 0.3f;
	m_spSprite->SetScale( f32Scale, f32Scale );
	m_spSprite->SetPosition( -(1.f - f32Scale) * tou32ScreenSizeW * 0.5f, (1.f - f32Scale) * tou32ScreenSizeH * 0.5f );
}

//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�X�V
*///��������������������������������������������������������
void clsMain::Update()
{
	hj::clsHJCoreMgr&	rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJInput		spInput		= rcCoreMgr.GetInput();

	static const hj_f32 tof32RotSpeed = 2.f;

	// camera rot
	m_f32CameraDegX += spInput->GetPadAnalog( hj::clsHJInput::PAD_STICK2_Y ) * tof32RotSpeed;
	m_f32CameraDegY -= spInput->GetPadAnalog( hj::clsHJInput::PAD_STICK2_X ) * tof32RotSpeed;
	
	// camera length
	static const hj_f32 tof32LengthSpeed = 0.5f;
	m_f32CameraLength += spInput->GetPadAnalog( hj::clsHJInput::PAD_SLIDER1 ) * tof32LengthSpeed;
	m_f32CameraLength -= spInput->GetPadAnalog( hj::clsHJInput::PAD_SLIDER2 ) * tof32LengthSpeed;
	m_f32CameraLength = HJ_MAX( m_f32CameraLength, 1.f);

	// camera position
	hj_f32 f32RadX = hj::clsHJMath::DegToRad(m_f32CameraDegX);
	hj_f32 f32RadY = hj::clsHJMath::DegToRad(m_f32CameraDegY);
	m_sCameraPos.x = m_sCameraLookPos.x + (hj::clsHJMath::Sin(f32RadY) * hj::clsHJMath::Cos(f32RadX) * m_f32CameraLength);
	m_sCameraPos.z = m_sCameraLookPos.y + (hj::clsHJMath::Cos(f32RadY) * hj::clsHJMath::Cos(f32RadX) * m_f32CameraLength);
	m_sCameraPos.y = m_sCameraLookPos.z + (hj::clsHJMath::Sin(f32RadX) * m_f32CameraLength);

	// Change Draw Mode
	if( spInput->IsTrig( hj::clsHJInput::PAD_BTN1 )){
		if(m_spModel->GetDrawMode() == hj::clsHJModel::DRAW_MODE_NUM-1){
			m_spModel->SetDrawMode(static_cast<hj::clsHJModel::enmDrawMode>(0));
		}else{
			m_spModel->SetDrawMode(static_cast<hj::clsHJModel::enmDrawMode>(m_spModel->GetDrawMode()+1));
		}
	}

	// Change Cull Mode
	if( spInput->IsTrig( hj::clsHJInput::PAD_BTN3 )){
		if(m_eCullMode == hj::clsHJGraphics::CULL_MAX_NUM-1){
			m_eCullMode = static_cast<hj::clsHJGraphics::enmCull>(0);
		}else{
			m_eCullMode = static_cast<hj::clsHJGraphics::enmCull>(m_eCullMode+1);
		}
	}
}
//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�`��
*///��������������������������������������������������������
void clsMain::Render()
{
	hj::clsHJCoreMgr&		rcCoreMgr	= hj::clsHJCoreMgr::RS();
	hj::SpHJGraphics		spGraphics	= rcCoreMgr.GetGraphics();
	hj::SpHJDraw3d			spDraw3d	= rcCoreMgr.GetDraw3d();
	hj::SpHJDrawPrimitive	spPrimitive = rcCoreMgr.GetDrawPrimitive();
	
	// �`��
	spGraphics->SetBackColor( hj::HJ_DARKSLATEGRAY );
	spGraphics->ClearRenderTarget();
	spGraphics->SetBlendMode( hj::clsHJGraphics::BLEND_BLEND );

	// �J�������
	static const hj_f32 tof32CameraLength = 30.f;
	const hj::stcVector3& osUp = hj::stcVector3(0.f, 1.f, 0.f);

	// world
	m_spModel->SetRotation( hj::stcVector3(0.f, hj::System::GetGameTime() * 30.f, 0.f) );
	m_spModel->SetPosition( hj::stcVector3(0.f, 2.f, 0.f) );
	hj_f32 f32Scale = 0.1f;
	m_spModel->SetScale( hj::stcVector3(f32Scale, f32Scale, f32Scale) );

	hj::stcMatrix44 sMtx;

	// Set Cull Mode
	spGraphics->SetCullMode(m_eCullMode);

	static const hj_f32 tof32FieldSize_half = 5.f;
	static const hj::clsHJDraw3d::stcFormat_PCT atosFieldPoly[] = {
		{ -tof32FieldSize_half, 0.f, -tof32FieldSize_half, hj::HJ_WHITE,	0.f, 0.f },
		{ tof32FieldSize_half, 0.f, -tof32FieldSize_half, hj::HJ_WHITE,	1.f, 0.f },
		{ -tof32FieldSize_half, 0.f, tof32FieldSize_half, hj::HJ_WHITE, 0.f, 1.f },
		{ tof32FieldSize_half, 0.f, tof32FieldSize_half, hj::HJ_WHITE, 1.f, 1.f },
	};
	static const hj_u32 tou32FieldPolyNum = HJ_ARRAY_NUM( atosFieldPoly );

	// view
	sMtx.SetCameraView(	m_sCameraPos, m_sCameraLookPos, osUp );
	spGraphics->SetViewMatrix(sMtx);

	// projection
	sMtx.SetCameraProjection( hj::clsHJMath::m_tof32PI/4.0f, tof32Aspect, tof32ClipNear, tof32ClipFar);
	spGraphics->SetProjectionMatrix(sMtx);

	// world
	sMtx.Identity();
	spGraphics->SetWorldMatrix(sMtx);
	
	// ===== �V�[���`��
	{
		// change render target
		spGraphics->SetRenderTarget( m_spDepthRenderTarget );
		spGraphics->SetBackColor( hj::HJ_WHITE );
		spGraphics->ClearRenderTarget();

		// �V�F�[�_�[�ݒ�
		m_spShader->SetTechnique("TDepth");
		spGraphics->SetShader( m_spShader );

		// field
		spDraw3d->RenderArray( hj::clsHJDraw3d::FMT_PCT, hj::clsHJDraw3d::PT_TRIANGLESTRIP, atosFieldPoly, tou32FieldPolyNum);

		// scene render
		m_spModel->Render();

		spGraphics->ResetShader();
	}

	// �ʏ�
	{
		spGraphics->SetRenderTargetBackBuffer();
		spGraphics->SetBackColor( hj::HJ_BLACK );
		spGraphics->ClearRenderTarget();

		// grid
		spPrimitive->DrawGridXz( 10.f, 10, hj::HJ_GRAY);
		// axis
		spPrimitive->DrawAxis( 10.f );

		// �V�F�[�_�[�ݒ�
		m_spShader->SetTechnique("TShader");
		m_spShader->SetTexture("TextureDepth", m_spDepthRenderTarget->GetTexture() );
		spGraphics->SetShader( m_spShader );

		spGraphics->SetCullMode( hj::clsHJGraphics::CULL_CW );

		// scene render
		m_spShader->SetTexture("TextureDiffuse", m_spTexDiffuse);
		m_spModel->Render();

		// field
		m_spShader->SetTexture("TextureDiffuse", m_spTexField );
		sMtx.Identity();
		spGraphics->SetWorldMatrix(sMtx);
		spDraw3d->RenderArray( hj::clsHJDraw3d::FMT_PCT, hj::clsHJDraw3d::PT_TRIANGLESTRIP, atosFieldPoly, tou32FieldPolyNum);

		spGraphics->ResetShader();

		// �V���h�E�}�b�v
		m_spSprite->Render();
	}
}

//���������������������������������������������������������� 
/*!	@brief	WinMain�֐�
*///��������������������������������������������������������
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT iCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// ���������[�N���o

	// �������p�����[�^
	hj::stcPfInitParam	sInitParam;
	hj::String::Tcscpy_s( sInitParam.atcApplicationName, HJ_ARRAY_NUM( sInitParam.atcApplicationName ), oatcApplicationName);
	sInitParam.s32ScreenWidth	= tou32ScreenSizeW;
	sInitParam.s32ScreenHeight	= tou32ScreenSizeH;
	sInitParam.bFullScreen		= false;
	sInitParam.hInst			= hInst;
	sInitParam.iCmdShow			= iCmdShow;

	// ���C��
	clsMain	cMain;
	// ������
	hj_s32 s32Result = 0;
	if( cMain.Initialize( sInitParam ) )
	{
		// ���[�U�[������
		cMain.Init();

		// �w�i�F
		if( hj::SpHJGraphics spGraphics = hj::clsHJCoreMgr::GS()->GetGraphics() ){
			spGraphics->SetBackColor( hj::HJ_DARKGRAY );
		}

		// ���[�v�J�n
		s32Result = cMain.StartLoop();
	}

	return s32Result;
}
