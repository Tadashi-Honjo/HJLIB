//���������������������������������������������������������� 
/*!	@file
	@brief	HJLib Thread Sample
			
	@author �{�� ���u
	@date	13/12/14
*/
//	Copyright (C) 2013 HonjoTadashi. All Rights Reserved. 
//���������������������������������������������������������� 

namespace{

	// �A�v���P�[�V������
	const hj_tchar oatcApplicationName[] = HJ_T("HJLib Sample Thread");
}

//���������������������������������������������������������� 
/*!	@brief	MainClass
	
	@author �{��
*///��������������������������������������������������������
class clsMain : public hj::clsPfMain
{
	typedef hj::clsPfMain MyClassParent;
public:
	clsMain(): MyClassParent(){}
	virtual ~clsMain(){}
	
	// ���[�U�[�p�X�V
	virtual	void Update();
	// ���[�U�[�p�`��
	virtual	void Render();
	// �C���X�^���X�폜
	virtual	void DeleteInstance(){}
	// ������
	void Init();

	// �����ɌĂ񂾂�~�܂郁�\�b�h
	void CheckCallThread()
	{
		static bool bCheck = false;
		HJ_ASSERT(!bCheck, HJ_T("�����ɌĂ΂�Ă��܂��B"));
		bCheck = true;
		hj::System::Sleep(1000);
		bCheck = false;
	};

	// �X���b�h�p���\�b�h
	static void ExecuteThread(void* pvArg);

	// --- Get
	hj::SpHJThread			GetThread()	{	return m_spThread;	}
	hj::SpHJCriticalSection	GetCS()		{	return m_spCS;		}

private:
	hj::SpHJThread				m_spThread;
	hj::SpHJCriticalSection		m_spCS;
};

//���������������������������������������������������������� 
/*!	@brief	������
*///��������������������������������������������������������
void clsMain::Init()
{
	// �X���b�h
	m_spThread.Reset( NEW hj::clsPfThread() );
	m_spThread->Initialize( ExecuteThread, this);
	m_spThread->Resume();
//	m_spThread->SetThreadPriority( hj::clsHJThread::PRIO_HIGHEST );

	// �N���e�B�J���Z�N�V����
	m_spCS.Reset( NEW hj::clsPfCriticalSection() );
}

//���������������������������������������������������������� 
/*!	@brief	�X���b�h�X�V
*///��������������������������������������������������������
void clsMain::ExecuteThread(void* pvArg)
{
	clsMain* pcMain = reinterpret_cast<clsMain*>(pvArg);
	hj_s32 s32Cnt = 0;
	do{
		hj::System::Sleep(500);
		{
			hj::clsHJCriticalSectionScope cCsScope( pcMain->GetCS() );
			pcMain->CheckCallThread();
			OR( HJ_T("thread Execute!!") );
			++s32Cnt;
		}
//		if(s32Cnt >= 10){
//			pcMain->GetThread()->Suspend();
//		}
	}while(s32Cnt < 5);

	// �X���b�h�I��
	{
		hj::clsHJCriticalSectionScope cCsScope( pcMain->GetCS() );
		pcMain->CheckCallThread();
		OR(HJ_T("thread End!!"));
	}
}

//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�X�V
*///��������������������������������������������������������
void clsMain::Update()
{
	// ���C���X���b�h
	{
		hj::clsHJCriticalSectionScope cCsScope( GetCS() );
		CheckCallThread();
		OR(HJ_T("MainThread Execute!!"));
	}
	hj::System::Sleep(500);
}
//���������������������������������������������������������� 
/*!	@brief	���[�U�[�p�`��
*///��������������������������������������������������������
void clsMain::Render()
{
}

//���������������������������������������������������������� 
/*!	@brief	WinMain�֐�
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
