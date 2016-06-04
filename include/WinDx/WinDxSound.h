//--------------------------------------------------------------
/*!	@file
	@brief	WinDxSound
			
	@author 本城 直志
	@date	12/08/19
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_SOUND_H_
#define __WINDX_SOUND_H_

// HJLib namespace
namespace hj{

class clsWinDxSound : public clsHJSound
{
public:
	clsWinDxSound(){};
	virtual ~clsWinDxSound(){};

	// --- Static Method
	static hj_bool	Initalize();	//!< システム初期化
	static hj_bool	Uninitalize();	//!< システム終了処理

private:
	// --- private Static Method
	static hj_bool CreateXAudio2();		//!< XAudio2 インターフェース作成
	static hj_bool CreateMVoice();		//!< マスターヴォイス作成

protected:
	// --- Static Variable
	static IXAudio2					*m_pXAudio2;	//!< XAudio2インターフェース
	static IXAudio2MasteringVoice	*m_pMVoice;		//!< マスターヴォイス
};

}// namespace hj

#endif // __WINDX_SOUND_H_