//--------------------------------------------------------------
/*!	@file
	@brief	WinDxSound
			
	@author 本城 直志
	@date	12/08/19
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

IXAudio2				*clsWinDxSound::m_pXAudio2	= NULL;
IXAudio2MasteringVoice	*clsWinDxSound::m_pMVoice	= NULL;


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	システム初期化
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSound::Initalize()
{
	// COM の初期化
	if( FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) )
	{
		if( FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)) )
		{
			HJ_ASSERT_MSG( HJ_T("[XAudio2]COMの初期化に失敗しました"));
			return false;
		}
	}

	// XAudio2 インターフェースの作成
	if(!CreateXAudio2())
		return false;

	// マスターボイスの作成
	if(!CreateMVoice())
		return false;

//	OR(HJ_T("XAudio2 関連の初期化に成功しました"));
	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	システム終了処理
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSound::Uninitalize()
{
	// マスタリングヴォイスの破棄
	if(m_pMVoice){
		m_pMVoice->DestroyVoice();
		m_pMVoice = NULL;
	}

	HJ_SAFE_RELEASE(m_pXAudio2);		// XAudio2 インターフェースの破棄

	CoUninitialize();

	return true;
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	XAudio2インターフェース作成
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSound::CreateXAudio2()
{
	UINT wFlag = 0;

#ifdef HJ_DEBUG
		wFlag |= XAUDIO2_DEBUG_ENGINE;
#endif // HJ_DEBUG

	if( FAILED( XAudio2Create(&m_pXAudio2, wFlag)) )
	{
		HJ_ASSERT_MSG( HJ_T("XAudio2 インターフェスの作成に失敗しました"));
		return false;
	}
	
//	OR(HJ_T("XAudio2 インターフェスの作成に成功しました"));
	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	マスターヴォイス作成
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSound::CreateMVoice()
{
	if( FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMVoice)) )
	{
		HJ_ASSERT_MSG( HJ_T("マスタリングヴォイスの作成に失敗しました"));
		return false;
	}

//	OR(HJ_T("マスタリングヴォイスの作成に成功しました"));

	return true;
}

}// namespace hj
