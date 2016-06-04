//--------------------------------------------------------------
/*!	@file
	@brief	入力デバイス総合クラス
			
	@author 本城 直志
	@date	12/02/11
*/
//	Copyright (C) 2011 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_INPUT_
#define __HJ_INPUT_

// HJLib namespace
namespace hj{

// ----- Class
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	HJInputクラス
//
//
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsHJInput
{
public:
	clsHJInput();
	virtual ~clsHJInput();
	
	// --- Enum
	//! キーボードキー(DirectInput引用)
	enum enmKey{
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_DOWN,
		KEY_UP,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_ABNT_C1,
		KEY_ABNT_C2,
		KEY_ADD,
		KEY_APOSTROPHE,
		KEY_APPS,
		KEY_AT,
		KEY_AX,
		KEY_BACK,
		KEY_BACKSLASH,
		KEY_CALCULATOR,
		KEY_CAPITAL,
		KEY_COLON,
		KEY_COMMA,
		KEY_CONVERT,
		KEY_DECIMAL,
		KEY_DELETE,
		KEY_DIVIDE,
		KEY_END,
		KEY_EQUALS,
		KEY_ESCAPE,
		KEY_GRAVE,
		KEY_HOME,
		KEY_INSERT,
		KEY_KANA,
		KEY_KANJI,
		KEY_LBRACKET,
		KEY_LCONTROL,
		KEY_LMENU,
		KEY_LSHIFT,
		KEY_LWIN,
		KEY_MAIL,
		KEY_MEDIASELECT,
		KEY_MEDIASTOP,
		KEY_MINUS,
		KEY_MULTIPLY,
		KEY_MUTE,
		KEY_MYCOMPUTER,
		KEY_NEXT,
		KEY_NEXTTRACK,
		KEY_NOCONVERT,
		KEY_NUMLOCK,
		KEY_NUMPAD0,
		KEY_NUMPAD1,
		KEY_NUMPAD2,
		KEY_NUMPAD3,
		KEY_NUMPAD4,
		KEY_NUMPAD5,
		KEY_NUMPAD6,
		KEY_NUMPAD7,
		KEY_NUMPAD8,
		KEY_NUMPAD9,
		KEY_NUMPADCOMMA,
		KEY_NUMPADENTER,
		KEY_NUMPADEQUALS,
		KEY_OEM_102,
		KEY_PAUSE,
		KEY_PERIOD,
		KEY_PLAYPAUSE,
		KEY_POWER,
		KEY_PREVTRACK,
		KEY_PRIOR,
		KEY_RBRACKET,
		KEY_RCONTROL,
		KEY_RETURN,
		KEY_RMENU,
		KEY_RSHIFT,
		KEY_RWIN,
		KEY_SCROLL,
		KEY_SEMICOLON,
		KEY_SLASH,
		KEY_SLEEP,
		KEY_SPACE,
		KEY_STOP,
		KEY_SUBTRACT,
		KEY_SYSRQ,
		KEY_TAB,
		KEY_UNDERLINE,
		KEY_UNLABELED,
		KEY_VOLUMEDOWN,
		KEY_VOLUMEUP,
		KEY_WAKE,
		KEY_WEBBACK,
		KEY_WEBFAVORITES,
		KEY_WEBFORWARD,
		KEY_WEBHOME,
		KEY_WEBREFRESH,
		KEY_WEBSEARCH,
		KEY_WEBSTOP,
		KEY_YEN,
		KEY_MAXNUM,		//!< 最大数
	};

	// ゲームパッド
	enum enmPad{
		// デジタル
		PAD_UP,
		PAD_DOWN,
		PAD_LEFT,
		PAD_RIGHT,
		// アナログ
		PAD_A_UP,
		PAD_A_DOWN,
		PAD_A_LEFT,
		PAD_A_RIGHT,
		// ボタン
		PAD_BTN1,
		PAD_BTN2,
		PAD_BTN3,
		PAD_BTN4,
		PAD_BTN5,
		PAD_BTN6,
		PAD_BTN7,
		PAD_BTN8,
		PAD_BTN9,
		PAD_BTN10,
		PAD_BTN11,
		PAD_BTN12,
		// アナログ・デジタル
		PAD_AD_UP,
		PAD_AD_DOWN,
		PAD_AD_LEFT,
		PAD_AD_RIGHT,

		PAD_BOTTON_MAXNUM,	// ボタン最大

		PAD_STICK1_X,
		PAD_STICK1_Y,
		PAD_STICK2_X,
		PAD_STICK2_Y,
		PAD_SLIDER1,
		PAD_SLIDER2,

		PAD_MAXNUM, 
	};
	// --- Method
	virtual void Init(hj_u32 u32PadMaxNum);
	virtual void Update();		//!< 更新(オーバーライドして処理の終わりで基底も呼ぶようにする)
	// --- Is
	// キー
	hj_bool IsBeta(enmKey eKey)const{ return m_sKeyPushData[eKey].bPush;}											//!< ベタ押し(キー)
	hj_bool IsTrig(enmKey eKey)const{ return m_sKeyPushData[eKey].bPush && !m_sKeyPushData[eKey].bPush_old; }		//!< トリガー(キー)
	hj_bool IsRelease(enmKey eKey)const{ return !m_sKeyPushData[eKey].bPush && m_sKeyPushData[eKey].bPush_old; }		//!< リリース(キー)
	hj_bool IsRep(enmKey eKey)const{ return IsTrig(eKey) || (m_sKeyPushData[eKey].bRepCntMode && m_sKeyPushData[eKey].u32PushCnt==0); }	//!< 繰り返し(キー)

	// パッド
	virtual hj_bool	IsActivePad(hj_u32 u32PadIdx)const=0;					//!< パッドが有効か
	hj_bool IsBeta(enmPad ePad, hj_u32 u32PadIdx=0)const{ return m_psPadData[u32PadIdx].asPushData[ePad].bPush;}																								//!< ベタ押し(キー)
	hj_bool IsTrig(enmPad ePad, hj_u32 u32PadIdx=0)const{ return m_psPadData[u32PadIdx].asPushData[ePad].bPush && !m_psPadData[u32PadIdx].asPushData[ePad].bPush_old; }											//!< トリガー(キー)
	hj_bool IsRel(enmPad ePad, hj_u32 u32PadIdx=0)const	{ return !m_psPadData[u32PadIdx].asPushData[ePad].bPush && m_psPadData[u32PadIdx].asPushData[ePad].bPush_old; }											//!< リリース(キー)
	hj_bool IsRep(enmPad ePad, hj_u32 u32PadIdx=0)const { return IsTrig(ePad, u32PadIdx) || (m_psPadData[u32PadIdx].asPushData[ePad].bRepCntMode && m_psPadData[u32PadIdx].asPushData[ePad].u32PushCnt==0); }	//!< 繰り返し(キー)
	hj_bool IsActiveBeta(enmPad ePad)const;		//!< アクティブベタ押し(キー)
	hj_bool IsActiveTrig(enmPad ePad)const;		//!< アクティブトリガー(キー)
	hj_bool IsActiveRel(enmPad ePad)const;		//!< アクティブリリース(キー)
	hj_bool IsActiveRep(enmPad ePad)const;		//!< アクティブ繰り返し(キー)

	// --- Get
	// パッド
	virtual hj_f32	GetPadAnalog(enmPad ePad, hj_u32 u32PadIdx=0)const;			//!< パッドのアナログ値を取得
	virtual hj_f32	GetActivePadAnalog(enmPad ePad)const;						//!< アクティブパッドのアナログ値取得
	virtual hj_u32	GetPadActiveNum()const=0;									//!< アクティブなパッドの数
	virtual hj_u32	GetPadMaxNum()const{return m_u32PadMaxNum;}					//!< アクティブなパッドの数
	
protected:
	// --- Method
	virtual hj_bool CheckKeyPush(enmKey eKey)const=0;								//!< キーボードチェック用
	virtual hj_bool CheckPadPush(enmPad ePad, hj_u32 u32PadIdx)const=0;				//!< パッドボタンチェック用
	virtual hj_f32	GetAnalog(enmPad ePad, hj_u32 u32PadIdx)const=0;				//!< パッドのアナログ値を取得
	
private:

	// --- Struct
	//!< 押した時のデータ
	struct stcPushData{
		stcPushData():u32PushCnt(0), bPush(false), bPush_old(false), bRepCntMode(false){}
		hj_u32	u32PushCnt;
		hj_bool bPush;
		hj_bool bPush_old;
		hj_bool bRepCntMode;
	};

	//!< パッドデータ
	struct stcPadData{
		stcPushData asPushData[PAD_BOTTON_MAXNUM];
	};

	void PushDataUpdate(stcPushData* psPushData, hj_bool bPush);	// プッシュデータ設定用

	// --- Variable
	hj_u32			m_u32RepTime;					//!< 繰り返しの頻度
	hj_u32			m_u32RepWaitTime;				//!< 繰り返し開始までの時間
	stcPushData		m_sKeyPushData[KEY_MAXNUM];		//!< キーボードデータ
	hj_u32			m_u32PadMaxNum;					//!< パッドの最大数
	stcPadData*		m_psPadData;					//!< パッドデータ(パッド最大数用)
};

} //namespace hj



#endif // __HJ_INPUT_

