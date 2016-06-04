//--------------------------------------------------------------
/*!	@file
	@brief	入力デバイス総合クラス
			
	@author 本城 直志
	@date	12/02/11
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_WINDX_INPUT_
#define __HJ_WINDX_INPUT_

// HJLib namespace
namespace hj{

//#define JOYSTICK_MAX	(16)	//最大ジョイスティック数

// ----- Class
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDxInputクラス
//
//
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxInput : public clsHJInput
{
public:	
	clsWinDxInput();
	virtual ~clsWinDxInput();

	// --- Struct 
	struct stcWinDxPadData{
		LPDIRECTINPUTDEVICE8	pDIDeviceJoy;						//!< ジョイスティックデバイス
		DIJOYSTATE				sJoystickData;						//!< ジョイパットの情報
		DIDEVCAPS				diDevCaps;							//!< ジョイスティックの能力情報
	};

	// --- Method
	void			Init(HINSTANCE hInst, HWND hWnd);				//!< 初期化
	virtual void	Update();										//!< 更新

	// --- Get
	LPDIRECTINPUT8		GetDxInput()const						{ return m_pDI;				}
	stcWinDxPadData*	GetGetDxPadDates()const					{ return m_psDxPadData;		}
	hj_u32				GetJoyStickNum()const					{ return m_u32JoyNum;		}
	hj_u32				GetJoyStickMax()const					{ return m_u32JoyMax;		}
	virtual hj_bool		IsActivePad(hj_u32 u32PadIdx)const;											//!< パッドが有効か
	virtual hj_u32		GetPadActiveNum()const					{ return m_u32JoyNum;		}		//!< アクティブなパッドの数
	
	// --- Set
	void			 SetJoyStickNum(hj_u32 u32JoyNum){ m_u32JoyNum=u32JoyNum;}
	
private:
	// --- Method
	hj_bool _CheckPadPush(enmPad ePad, hj_u32 u32PadIdx)const;			//!< パッドチェック用(内部用)

protected:
	// --- Method
	virtual hj_bool CheckKeyPush(enmKey eKey)const;						//!< キーボードチェック
	virtual hj_bool CheckPadPush(enmPad ePad, hj_u32 u32PadIdx)const;	//!< パッドボタンチェック用
	virtual hj_f32	GetAnalog(enmPad ePad, hj_u32 u32PadIdx)const;		//!< パッドのアナログ値を取得

	// --- Variable
	LPDIRECTINPUT8			m_pDI;					//!< DirectInputオブジェクト
	hj_f32					m_f32StickBottonParam;	//!< スティックが倒れたと判定する値(0.0~1.0)

	// キーボード
	LPDIRECTINPUTDEVICE8	m_pDIDeviceKey;		//!< キーボードデバイス
	BYTE					mKeyState[256];		//!< キーデータの状態

	//マウス
	LPDIRECTINPUTDEVICE8	m_pDIDeviceMou;		//!< マウスデバイス
	DIMOUSESTATE			mMouState;			//!< マウスの状態
	POINT					mMouXY;				//!< マウスの位置情報

	//ジョイパッド
	stcWinDxPadData*		m_psDxPadData;		//!< パッドデータ
	hj_u32					m_u32JoyNum;		//!< ジョイスティック認識数
	hj_u32					m_u32JoyMax;		//!< ジョイスティック最大数
//	
	
};


/*
#define KEY_ON 0x80		//押されている
#define CLICK_RIGHT	1
#define CLICK_LEFT	0



//ジョイスティック専用セットコールバック関数宣言
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* pdidInstance,VOID* pContext);

//ジョイスティック専用軸コールバック関数
BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvRef);

//--------------------------------------------------------
//			入力デバイス総合クラス
class clsInput : public clsHjSingleton< clsInput >
{
public:
	clsInput();		//コンストラクタ
	~clsInput();		//デストラクタ
	//---------------------------------
	//				総合
	H_RESULT InitInput(HINSTANCE hInst,HWND hWnd);		//すべての入力デバイスの初期化
	void CheckInput();									//すべての入力デバイスのチェック

	//取得
	LPDIRECTINPUT8 GetDxInput(){return m_pDI;};			//DirectInputオブジェクト取得

	int	GetKeepFrame(){return mKeepFrame;};				//押し続けたと判定する時間取得
	LONG GetPadStickPermit(){return mPadStickPermit;};	//スティックが倒されたときの許容値取得
	

	//設定
	void SetKeepFrame(hj_s32 Keep=15){mKeepFrame = Keep;};	//キーやボタンのキープ時間設定
	void SetPadStickPermit(LONG Permit=100){mPadStickPermit=Permit;};//スティックが倒されたときの許容値設定

	//--------------------------------------------------------
	//					キーボード
	BYTE GetKeyState(hj_s32 type);								//指定キーの状態を取得
	hj_bool CheckKeyState(hj_s32 key, hj_s32 state=KEY_ON);		//指定キーが指定の状態にあるか
	hj_bool CheckKeyTrigger(hj_s32 key, hj_s32 state=KEY_ON);	//キーの状態が変わったときだけ
	hj_bool CheckKeyKeep(hj_s32 key);							//キーを押し続けていたときだけ

	//--------------------------------------------------------
	//					マウス
	void GetMouPos(int* x,int* y);				//マウス位置取得
	void GetMouState(DIMOUSESTATE* dims);		//マウス状態取得
	void SetMouPos(hj_s32 x, hj_s32 y);				//マウス位置設定関数(システム用なので使わないでください)

	//--------------------------------------------------------
	//				ジョイスティック
	LPDIRECTINPUTDEVICE8* GetJoyStickDevice(){return m_ppDIDeviceJoy;};	//ジョイスティックデバイス取得
	hj_s32 GetJoyStickNum(){return mJoyNum;};								//ジョイスティック登録数取得
	hj_s32 GetJoyStickMax(){return mJoyMax;};								//ジョイスティック最大数取得
	void SetJoyStickNum(hj_s32 num){mJoyNum=num;};							//ジョイスティック登録数設定


	hj_bool CheckJoyKeyOn(hj_s32 joyId, hj_s32 Id);				//押しているか
	hj_bool CheckJoyTrigger(hj_s32 joyId, hj_s32 Id);			//押した瞬間のみ
	hj_bool CheckJoyKeep(hj_s32 joyId, hj_s32 Id);				//押し続けていたら
	hj_bool GetStickPos(hj_f32* x, hj_f32* y, hj_s32 joyId=0, JOYSTICK_STICK stick=JOYSTICK_STICK1);	//スティックの位置

private:
	H_RESULT UpdateKeyState();							//キーボードの状態を更新
	void UpdateJoystick();								//ジョイスティック更新
	hj_bool CheckJoyDown(DIJOYSTATE& Joystick, hj_s32 Id);	//ジョイスティックが倒されているか

private:
	LPDIRECTINPUT8			m_pDI;				//DirectInputオブジェクト
	int						mKeepFrame;			//押し続けたと判定する時間
	LONG					mPadStickPermit;	//スティックが倒されたときの許容値

	//キーボード
	LPDIRECTINPUTDEVICE8	m_pDIDeviceKey;		//キーボードデバイス
	BYTE					mOldKeyState[256];	//前フレームのキーデータの状態
	BYTE					mKeyState[256];		//キーデータの状態
	int						mKeyKeepCnt[256];	//キーごとに押し続ける時間をカウント

	//マウス
	LPDIRECTINPUTDEVICE8	m_pDIDeviceMou;		//マウスデバイス
	DIMOUSESTATE			mMouState;			//マウスの状態
	POINT					mMouXY;				//マウスの位置情報

	//ジョイパッド
	LPDIRECTINPUTDEVICE8*	m_ppDIDeviceJoy;						//ジョイスティックデバイス
	int						mJoyNum;								//ジョイスティック認識回数
	int						mJoyMax;								//ジョイスティック最大数
	DIDEVCAPS				mdiDevCaps;								//ジョイスティックの能力情報
	DIJOYSTATE				mJoystickData[JOYSTICK_MAX];			//ジョイパットの情報
	DIJOYSTATE				mJoystickOldData[JOYSTICK_MAX];			//ジョイパットの古い情報
	int						mJoyKeepCnt[JOYSTICK_MAX][JOYSTICK_END];//ボタン押し続ける時間をカウント
};
*/

} //namespace hj



#endif // __HJ_WINDX_INPUT_