//--------------------------------------------------------------
/*!	@file
	@brief	入力デバイス総合クラス
			
	@author 本城 直志
	@date	12/02/11
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

namespace{
	static const hj_s32 tos32JoyStickMaxLength = 1000;	// スティックの振り幅(分解能)
	static const hj_s32 tos32JoyStickPlayLength = 300;	// スティックの遊び(誤作動回避用)
	static const hj_u8	tou8KeyOn = 0x80;

	//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
	/*!	@brief	ジョイスティック専用セットコールバック関数
	*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
	BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* pdidInstance,VOID* pContext)
	{
		SpHJInput spInput = clsHJCoreMgr::RS().GetInput();
		clsWinDxInput* pcInput = static_cast<clsWinDxInput*>( spInput.Get() );
		HRESULT hr;
		LPDIRECTINPUT8 pDxInput = pcInput->GetDxInput();
		clsWinDxInput::stcWinDxPadData* psDxPadData = pcInput->GetGetDxPadDates();
		hj_u32 JoyNum = pcInput->GetJoyStickNum();
		hj_u32 JoyMax = pcInput->GetJoyStickMax();

		// 列挙されたジョイスティックへのインターフェイスを取得する
		hr = pDxInput->CreateDevice( pdidInstance->guidInstance, &psDxPadData[JoyNum].pDIDeviceJoy, NULL );
		if SUCCEEDED( hr ){
			pcInput->SetJoyStickNum(++JoyNum);
			return ( JoyNum == JoyMax )?( DIENUM_STOP ):( DIENUM_CONTINUE );
		}
		return DIENUM_STOP;
	}

	//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
	/*!	@brief	ジョイスティック専用軸コールバック関数
	*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
	BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvRef)
	{
		// 軸の範囲設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg,sizeof(diprg));
		diprg.diph.dwSize=sizeof(diprg);
		diprg.diph.dwHeaderSize=sizeof(diprg.diph);  
		diprg.diph.dwHow = DIPH_BYID; 
		diprg.diph.dwObj =  lpddoi->dwType; // Specify the enumerated axis 
		diprg.lMin = -tos32JoyStickMaxLength;
		diprg.lMax = tos32JoyStickMaxLength;

		LPDIRECTINPUTDEVICE8 lpJoy = (LPDIRECTINPUTDEVICE8)pvRef;

		if FAILED( lpJoy->SetProperty( DIPROP_RANGE, &diprg.diph ) ) return DIENUM_STOP;

		return DIENUM_CONTINUE;
	}


	// キーボードのボタン変換用テーブル
	BYTE abyteKeyData[clsHJInput::KEY_MAXNUM] = {
		DIK_0, // KEY_0,
		DIK_1, // KEY_1,
		DIK_2, // KEY_2,
		DIK_3, // KEY_3,
		DIK_4, // KEY_4,
		DIK_5, // KEY_5,
		DIK_6, // KEY_6,
		DIK_7, // KEY_7,
		DIK_8, // KEY_8,
		DIK_9, // KEY_9,
		DIK_A, // KEY_A,
		DIK_B, // KEY_B,
		DIK_C, // KEY_C,
		DIK_D, // KEY_D,
		DIK_E, // KEY_E,
		DIK_F, // KEY_F,
		DIK_G, // KEY_G,
		DIK_H, // KEY_H,
		DIK_I, // KEY_I,
		DIK_J, // KEY_J,
		DIK_K, // KEY_K,
		DIK_L, // KEY_L,
		DIK_M, // KEY_M,
		DIK_N, // KEY_N,
		DIK_O, // KEY_O,
		DIK_P, // KEY_P,
		DIK_Q, // KEY_Q,
		DIK_R, // KEY_R,
		DIK_S, // KEY_S,
		DIK_T, // KEY_T,
		DIK_U, // KEY_U,
		DIK_V, // KEY_V,
		DIK_W, // KEY_W,
		DIK_X, // KEY_X,
		DIK_Y, // KEY_Y,
		DIK_Z, // KEY_Z,
		DIK_F1, // KEY_F1,
		DIK_F2, // KEY_F2,
		DIK_F3, // KEY_F3,
		DIK_F4, // KEY_F4,
		DIK_F5, // KEY_F5,
		DIK_F6, // KEY_F6,
		DIK_F7, // KEY_F7,
		DIK_F8, // KEY_F8,
		DIK_F9, // KEY_F9,
		DIK_F10, // KEY_F10,
		DIK_F11, // KEY_F11,
		DIK_F12, // KEY_F12,
		DIK_F13, // KEY_F13,
		DIK_F14, // KEY_F14,
		DIK_F15, // KEY_F15,
		DIK_DOWN, // KEY_DOWN,
		DIK_UP, // KEY_UP,
		DIK_LEFT, // KEY_LEFT,
		DIK_RIGHT, // KEY_RIGHT,
		DIK_ABNT_C1, // KEY_ABNT_C1,
		DIK_ABNT_C2, // KEY_ABNT_C2,
		DIK_ADD, // KEY_ADD,
		DIK_APOSTROPHE, // KEY_APOSTROPHE,
		DIK_APPS, // KEY_APPS,
		DIK_AT, // KEY_AT,
		DIK_AX, // KEY_AX,
		DIK_BACK, // KEY_BACK,
		DIK_BACKSLASH, // KEY_BACKSLASH,
		DIK_CALCULATOR, // KEY_CALCULATOR,
		DIK_CAPITAL, // KEY_CAPITAL,
		DIK_COLON, // KEY_COLON,
		DIK_COMMA, // KEY_COMMA,
		DIK_CONVERT, // KEY_CONVERT,
		DIK_DECIMAL, // KEY_DECIMAL,
		DIK_DELETE, // KEY_DELETE,
		DIK_DIVIDE, // KEY_DIVIDE,
		DIK_END, // KEY_END,
		DIK_EQUALS, // KEY_EQUALS,
		DIK_ESCAPE, // KEY_ESCAPE,
		DIK_GRAVE, // KEY_GRAVE,
		DIK_HOME, // KEY_HOME,
		DIK_INSERT, // KEY_INSERT,
		DIK_KANA, // KEY_KANA,
		DIK_KANJI, // KEY_KANJI,
		DIK_LBRACKET, // KEY_LBRACKET,
		DIK_LCONTROL, // KEY_LCONTROL,
		DIK_LMENU, // KEY_LMENU,
		DIK_LSHIFT, // KEY_LSHIFT,
		DIK_LWIN, // KEY_LWIN,
		DIK_MAIL, // KEY_MAIL,
		DIK_MEDIASELECT, // KEY_MEDIASELECT,
		DIK_MEDIASTOP, // KEY_MEDIASTOP,
		DIK_MINUS, // KEY_MINUS,
		DIK_MULTIPLY, // KEY_MULTIPLY,
		DIK_MUTE, // KEY_MUTE,
		DIK_MYCOMPUTER, // KEY_MYCOMPUTER,
		DIK_NEXT, // KEY_NEXT,
		DIK_NEXTTRACK, // KEY_NEXTTRACK,
		DIK_NOCONVERT, // KEY_NOCONVERT,
		DIK_NUMLOCK, // KEY_NUMLOCK,
		DIK_NUMPAD0, // KEY_NUMPAD0,
		DIK_NUMPAD1, // KEY_NUMPAD1,
		DIK_NUMPAD2, // KEY_NUMPAD2,
		DIK_NUMPAD3, // KEY_NUMPAD3,
		DIK_NUMPAD4, // KEY_NUMPAD4,
		DIK_NUMPAD5, // KEY_NUMPAD5,
		DIK_NUMPAD6, // KEY_NUMPAD6,
		DIK_NUMPAD7, // KEY_NUMPAD7,
		DIK_NUMPAD8, // KEY_NUMPAD8,
		DIK_NUMPAD9, // KEY_NUMPAD9,
		DIK_NUMPADCOMMA, // KEY_NUMPADCOMMA,
		DIK_NUMPADENTER, // KEY_NUMPADENTER,
		DIK_NUMPADEQUALS, // KEY_NUMPADEQUALS,
		DIK_OEM_102, // KEY_OEM_102,
		DIK_PAUSE, // KEY_PAUSE,
		DIK_PERIOD, // KEY_PERIOD,
		DIK_PLAYPAUSE, // KEY_PLAYPAUSE,
		DIK_POWER, // KEY_POWER,
		DIK_PREVTRACK, // KEY_PREVTRACK,
		DIK_PRIOR, // KEY_PRIOR,
		DIK_RBRACKET, // KEY_RBRACKET,
		DIK_RCONTROL, // KEY_RCONTROL,
		DIK_RETURN, // KEY_RETURN,
		DIK_RMENU, // KEY_RMENU,
		DIK_RSHIFT, // KEY_RSHIFT,
		DIK_RWIN, // KEY_RWIN,
		DIK_SCROLL, // KEY_SCROLL,
		DIK_SEMICOLON, // KEY_SEMICOLON,
		DIK_SLASH, // KEY_SLASH,
		DIK_SLEEP, // KEY_SLEEP,
		DIK_SPACE, // KEY_SPACE,
		DIK_STOP, // KEY_STOP,
		DIK_SUBTRACT, // KEY_SUBTRACT,
		DIK_SYSRQ, // KEY_SYSRQ,
		DIK_TAB, // KEY_TAB,
		DIK_UNDERLINE, // KEY_UNDERLINE,
		DIK_UNLABELED, // KEY_UNLABELED,
		DIK_VOLUMEDOWN, // KEY_VOLUMEDOWN,
		DIK_VOLUMEUP, // KEY_VOLUMEUP,
		DIK_WAKE, // KEY_WAKE,
		DIK_WEBBACK, // KEY_WEBBACK,
		DIK_WEBFAVORITES, // KEY_WEBFAVORITES,
		DIK_WEBFORWARD, // KEY_WEBFORWARD,
		DIK_WEBHOME, // KEY_WEBHOME,
		DIK_WEBREFRESH, // KEY_WEBREFRESH,
		DIK_WEBSEARCH, // KEY_WEBSEARCH,
		DIK_WEBSTOP, // KEY_WEBSTOP,
		DIK_YEN, // KEY_YEN,
	};
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxInput::clsWinDxInput():clsHJInput()
, m_pDI(NULL)
, m_f32StickBottonParam(0.5)
, m_pDIDeviceKey(NULL)
, m_pDIDeviceMou(NULL)
, m_u32JoyNum(0)
, m_u32JoyMax(0)
, m_psDxPadData(NULL)
{
//	Memset(mKeyState, 0x0, sizeof(mKeyState));
//	Memset(mJoystickData, 0x0, sizeof(mJoystickData));
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxInput::~clsWinDxInput()
{
	// キーボード
	if(m_pDIDeviceKey!=NULL){
		m_pDIDeviceKey->Unacquire();
		HJ_SAFE_RELEASE(m_pDIDeviceKey );
	}
	// マウス
	if(m_pDIDeviceMou!=NULL){
		m_pDIDeviceMou->Unacquire(); 
		HJ_SAFE_RELEASE(m_pDIDeviceMou );
	}

	//ジョイステック削除
	if(m_psDxPadData!=NULL){
		for(hj_u32 u32i=0; u32i<m_u32JoyNum ; ++u32i){
			m_psDxPadData[u32i].pDIDeviceJoy->Unacquire();
			m_psDxPadData[u32i].pDIDeviceJoy->Release();
		}
		HJ_SAFE_DELETE_ARRAY(m_psDxPadData);
	}

	HJ_SAFE_RELEASE(m_pDI);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxInput::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	//キーボード・マウスの初期化
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8,(LPVOID*)&m_pDI, NULL );
	m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDeviceKey, NULL );
	m_pDI->CreateDevice(GUID_SysMouse,    &m_pDIDeviceMou, NULL );

	// フォーマット
	m_pDIDeviceKey->SetDataFormat( &c_dfDIKeyboard );
	m_pDIDeviceMou->SetDataFormat( &c_dfDIMouse );

	// 協調レベル確定
	m_pDIDeviceKey->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	m_pDIDeviceMou->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );

	// 入力デバイスへのアクセス権を取得
	m_pDIDeviceKey->Acquire();
	m_pDIDeviceMou->Acquire();

	//ジョイスティックの初期化
	m_u32JoyMax = joyGetNumDevs();
	m_u32JoyNum=0;
	m_psDxPadData = NEW stcWinDxPadData[m_u32JoyMax];
	System::Memset(m_psDxPadData, 0x00, sizeof(stcWinDxPadData)*m_u32JoyMax);

	hr=m_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoyCallback,NULL,DIEDFL_ATTACHEDONLY);
	if(FAILED(hr)){
        for( hj_u32 u32i=0; u32i<m_u32JoyMax; u32i++ ){
			HJ_SAFE_RELEASE( m_psDxPadData[u32i].pDIDeviceJoy );
        }
		HJ_ASSERT_MSG( HJ_T("パッド認識コールバック失敗"));
	}
	
	// 接続されている全ジョイスティックを初期化する
	LPDIRECTINPUTDEVICE8 pDIDeviceJoy = NULL;
	for (hj_u32 u32i=0; u32i<m_u32JoyNum; u32i++){
		// データフォーマットの設定
		pDIDeviceJoy = m_psDxPadData[u32i].pDIDeviceJoy;
		hr = pDIDeviceJoy->SetDataFormat(&c_dfDIJoystick);
		if FAILED( hr ){
			HJ_SAFE_RELEASE( m_psDxPadData[u32i].pDIDeviceJoy );
			continue;
		}

		// 協調レベルの設定
		hr = pDIDeviceJoy->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		if FAILED( hr ){
			HJ_SAFE_RELEASE( m_psDxPadData[u32i].pDIDeviceJoy );
			continue;
		}

		// 軸モード設定
		hr = pDIDeviceJoy->EnumObjects(EnumAxesCallback, (void *)pDIDeviceJoy, DIDFT_AXIS );
		if FAILED( hr ){
			HJ_SAFE_RELEASE( m_psDxPadData[u32i].pDIDeviceJoy );
			continue;
		}

		// デバイス情報取得
		DIDEVCAPS caps = { sizeof(DIDEVCAPS) };
		pDIDeviceJoy->GetCapabilities( &caps );
		
		// 入力制御開始
		pDIDeviceJoy->Acquire();
	}

	// 基底の初期化
	clsHJInput::Init( m_u32JoyMax );
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	更新
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxInput::Update()
{
	HRESULT hr;

	// キーボード
	hr = m_pDIDeviceKey->Acquire();	// デバイスへのアクセス権のチェック
	if((hr == DI_OK) || (hr == S_FALSE)){
		m_pDIDeviceKey->GetDeviceState(sizeof(BYTE)*256,mKeyState);	
	}

	// パッド
	stcWinDxPadData* psPadData = NULL;
	LPDIRECTINPUTDEVICE8 pDIDeviceJoy = NULL;
	for ( hj_u32 u32i=0; u32i<m_u32JoyNum; ++u32i ){
		psPadData = &m_psDxPadData[u32i];
		pDIDeviceJoy = psPadData->pDIDeviceJoy;
        // ポーリングが必要な場合
        if(pDIDeviceJoy!=NULL){
			pDIDeviceJoy->Poll();
			// デバイス取得成功　次のデバイスへ
			if SUCCEEDED( pDIDeviceJoy->GetDeviceState(sizeof(DIJOYSTATE), &psPadData->sJoystickData ) ){
				continue;
			}
			// デバイス再アクセス
			pDIDeviceJoy->Acquire();
//			ZeroMemory( &psPadData->sJoystickData, sizeof(DIJOYSTATE) );	// 初期化すると0でトリガーが入ってしまうボタンがあるので
		}
    }
	
	clsHJInput::Update();	// 最後に基底を呼ぶ
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	パッドが有効か
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool	clsWinDxInput::IsActivePad(hj_u32 u32PadIdx)const
{
	return (m_psDxPadData[u32PadIdx].pDIDeviceJoy != NULL);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	キーボードチェック
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxInput::CheckKeyPush( enmKey eKey )const
{
	return (mKeyState[abyteKeyData[eKey]]&tou8KeyOn)==tou8KeyOn;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	パッドチェック
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxInput::CheckPadPush( enmPad ePad, hj_u32 u32PadIdx )const
{
	switch(ePad){
		case PAD_AD_UP: return _CheckPadPush(PAD_A_UP, u32PadIdx) || _CheckPadPush(PAD_UP, u32PadIdx);
		case PAD_AD_DOWN: return _CheckPadPush(PAD_A_DOWN, u32PadIdx) || _CheckPadPush(PAD_DOWN, u32PadIdx);
		case PAD_AD_LEFT: return _CheckPadPush(PAD_A_LEFT, u32PadIdx) || _CheckPadPush(PAD_LEFT, u32PadIdx);
		case PAD_AD_RIGHT: return _CheckPadPush(PAD_A_RIGHT, u32PadIdx) || _CheckPadPush(PAD_RIGHT, u32PadIdx);
		default:{
			return _CheckPadPush(ePad, u32PadIdx);
		}break;
	};
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	パッドのアナログ値を取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_f32 clsWinDxInput::GetAnalog( enmPad ePad, hj_u32 u32PadIdx )const
{
	if(!IsActivePad(u32PadIdx)) return 0.f;

	DIJOYSTATE* pcJoyStickData = &m_psDxPadData[u32PadIdx].sJoystickData;
	switch(ePad){
		case PAD_STICK1_X: return (clsHJMath::Abs(static_cast<hj_f32>(pcJoyStickData->lX)) > tos32JoyStickPlayLength) ? static_cast<hj_f32>( pcJoyStickData->lX ) / static_cast<hj_f32>(tos32JoyStickMaxLength) : 0.f;
		case PAD_STICK1_Y: return (clsHJMath::Abs(static_cast<hj_f32>(pcJoyStickData->lY)) > tos32JoyStickPlayLength) ? static_cast<hj_f32>( -pcJoyStickData->lY ) / static_cast<hj_f32>(tos32JoyStickMaxLength) : 0.f;	// 上を正方向にするので
		case PAD_STICK2_X: return (clsHJMath::Abs(static_cast<hj_f32>(pcJoyStickData->lRx)) > tos32JoyStickPlayLength) ? static_cast<hj_f32>( pcJoyStickData->lRx ) / static_cast<hj_f32>(tos32JoyStickMaxLength) : 0.f;
		case PAD_STICK2_Y: return (clsHJMath::Abs(static_cast<hj_f32>(pcJoyStickData->lRy)) > tos32JoyStickPlayLength) ? static_cast<hj_f32>( -pcJoyStickData->lRy ) / static_cast<hj_f32>(tos32JoyStickMaxLength) : 0.f; // 上を正方向にするので
		case PAD_SLIDER1: return (pcJoyStickData->lZ > tos32JoyStickPlayLength) ? clsHJMath::Abs(static_cast<hj_f32>( pcJoyStickData->lZ )) / static_cast<hj_f32>(tos32JoyStickMaxLength) : 0.f;
		case PAD_SLIDER2: return (pcJoyStickData->lZ < -tos32JoyStickPlayLength) ? clsHJMath::Abs(static_cast<hj_f32>( pcJoyStickData->lZ )) / static_cast<hj_f32>(tos32JoyStickMaxLength) : 0.f;
	};
	return 0.f;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	バッファチェック用
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxInput::_CheckPadPush( enmPad ePad, hj_u32 u32PadIdx )const
{
	DIJOYSTATE* pcJoyStickData = &m_psDxPadData[u32PadIdx].sJoystickData;
	switch(ePad){
		case PAD_A_UP: return GetPadAnalog(PAD_STICK1_Y, u32PadIdx) >= m_f32StickBottonParam;
		case PAD_A_DOWN: return GetPadAnalog(PAD_STICK1_Y, u32PadIdx) <= -m_f32StickBottonParam;
		case PAD_A_LEFT: return GetPadAnalog(PAD_STICK1_X, u32PadIdx) <= -m_f32StickBottonParam;
		case PAD_A_RIGHT: return GetPadAnalog(PAD_STICK1_X, u32PadIdx) >= m_f32StickBottonParam;
/*
		case JOY_STK2_UP: return ((pcJoyStickData->lRy < 0) && (abs(pcJoyStickData->lRy) >= m_f32StickBottonParam));
		case JOY_STK2_DOWN: return ((pcJoyStickData->lRy > 0) && (abs(pcJoyStickData->lRy) >= m_f32StickBottonParam));
		case JOY_STK2_LEFT: return ((pcJoyStickData->lRx < 0) && (abs(pcJoyStickData->lRx) >= m_f32StickBottonParam));
		case JOY_STK2_RIGHT: return ((pcJoyStickData->lRx > 0) && (abs(pcJoyStickData->lRx) >= m_f32StickBottonParam));
*/
		// ハッチハッド
		case PAD_UP:
		case PAD_DOWN:
		case PAD_LEFT:
		case PAD_RIGHT:{
			DWORD tmp = pcJoyStickData->rgdwPOV[0];
			// 中心以外
			hj_bool bCenter = LOWORD(tmp)==0xFFFF || tmp == -1;
			if(!bCenter){
				// 角度で判定
				tmp %= 36000;	// 一応範囲を360より小さく
				switch(ePad){
					case PAD_UP: return (tmp >= 31500 && tmp < 36000) || (tmp >= 0 && tmp <= 4500);
					case PAD_DOWN: return (tmp >= 13500 && tmp <= 22500);
					case PAD_LEFT: return (tmp >= 22500 && tmp <= 31500);
					case PAD_RIGHT: return (tmp >= 4500 && tmp <= 13500);
				};
			}
		}break;
		// 残りはボタンのハズだけど追加されたらバグる可能性があるので一応判別
		case PAD_BTN1:
		case PAD_BTN2:
		case PAD_BTN3:
		case PAD_BTN4:
		case PAD_BTN5:
		case PAD_BTN6:
		case PAD_BTN7:
		case PAD_BTN8:
		case PAD_BTN9:
		case PAD_BTN10:
		case PAD_BTN11:
		case PAD_BTN12:{
			return ((pcJoyStickData->rgbButtons[ePad-PAD_BTN1] & tou8KeyOn) == tou8KeyOn);
		}break;
	}
	return false;
}

} //namespace hj