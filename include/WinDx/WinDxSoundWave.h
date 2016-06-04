//--------------------------------------------------------------
/*!	@file
	@brief	HJSoundWave
			
	@author 本城 直志
	@date	12/08/19
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_SOUND_WAVE_H_
#define __WINDX_SOUND_WAVE_H_

// HJLib namespace
namespace hj{

//! Waveサウンドデータ
class clsWinDxSoundWaveData
{
public:
	clsWinDxSoundWaveData();
	~clsWinDxSoundWaveData();

	// --- Method
	// --- Get
	const WAVEFORMATEX&		GetWaveFmtEx()const;				//!< WAVE フォーマットを取得
	const hj_tchar*			GetFileName()const;					//!< ファイル名の取得
	const hj_size_t			GetWaveSize()const;					//!< Wave データのサイズを取得

public:
	// --- Method
	hj_bool Open(const hj_tchar* ptcFilePath);			//!< WAVE ファイルオープン
	hj_bool Close();									//!< WAVE ファイルクローズ
	hj_bool ReadToWaveFmtEx();							//!< WAVEFORMATEX にデータを読み込む
	LONG 	ReadToWaveData(hj_u8* pu8Data, DWORD size);	//!< m_WaveData に WAVE データを読み込む
	hj_bool CheckWAVEHeader();							//!< WAVE ヘッダがあるかチェック
	hj_bool CheckFMTChunk();							//!< FMT チャンクがあるかチェック
	hj_bool CheckDATAChunk();							//!< DAtA チャンクがあるかチェック

	// --- Variable
	HMMIO				m_hMmio;					//!< MMIO ハンドル
	MMIOINFO			m_MmioInfo;					//!< MMIO 情報構造体
	MMCKINFO			m_RIFFChunk;				//!< RIFF チャンク
	MMCKINFO			m_FMTChunk;					//!< FMT チャンク
	MMCKINFO			m_DATAChunk;				//!< DATA チャンク
	WAVEFORMATEX		m_WaveFmtEx;				//!< WAVE フォーマット構造体
	hj_tchar			m_atcFilePath[HJ_MAX_PATH];	//!< ファイル名

};

//! Waveサウンド
class clsWinDxSoundWave : public clsWinDxSound, public clsWinDxSoundWaveData
{
public:
	clsWinDxSoundWave();
	virtual ~clsWinDxSoundWave();
	
	// --- Method
	virtual hj_bool	LoadFile(const hj_tchar* ptcFilePath);	//!< サウンドデータ読み込み
	virtual void	Play(hj_bool bLoop);					//!< 再生
	virtual void	Stop();									//!< 停止
	virtual void	Pause();								//!< 一時停止
	virtual void	Update(){};

	// --- Set
	virtual void	SetVolume(hj_f32 f32Volume);	//!< ボリュームの設定

private:
	// --- private Method
	void 	Exit();						//!< 終了
	hj_bool Submit();					//!< 音楽データをキューに送信
	hj_bool CreateSVoice();				//!< ソースヴォイス作成

private:
	// --- Variable
	IXAudio2SourceVoice		*m_pSVoice;				//!< ソースヴォイス
	WAVEFORMATEX			m_WaveFmt;				//!< WAVE フォーマット構造体
	hj_u8*					m_pc8WaveData;			//!< WAVE データ
};


//class clsMyXAudioCallback :  {
//};

//! StreamWaveサウンド
class clsWinDxStreamSoundWave : public clsWinDxSound, public clsWinDxSoundWaveData
{
public:
	clsWinDxStreamSoundWave();
	virtual ~clsWinDxStreamSoundWave();
	
	// --- Method
	virtual hj_bool	LoadFile(const hj_tchar* ptcFilePath);	//!< サウンドデータ読み込み
	virtual void	Update();								//!< 更新
	virtual void	Play(hj_bool bLoop);					//!< 再生
	virtual void	Stop();									//!< 停止
	virtual void	Pause();								//!< 一時停止

	// --- Set
	virtual void	SetVolume(hj_f32 f32Volume);	//!< ボリュームの設定

private:
	// --- private Method
	void 	Exit();						//!< 終了
	hj_bool Submit();					//!< 音楽データをキューに送信
	hj_bool CreateSVoice();				//!< ソースヴォイス作成
	
private:
	// --- Static Variable
	static const hj_s8		m_tos8WaveDataMaxNum=3;
	// --- Variable
	IXAudio2SourceVoice		*m_pSVoice;									//!< ソースヴォイス
	WAVEFORMATEX			m_WaveFmt;									//!< WAVE フォーマット構造体
	hj_u8*					m_apc8WaveDataBuff[m_tos8WaveDataMaxNum];	//!< WAVE データ
	hj_s8					m_s8WaveBuffCnt;
	hj_size_t				m_sizeWaveBuff, m_sizeReadWaveCnt, m_sizeWaveBegin, m_sizeLoopPoint;
	hj_bool					m_bPlay;									//!< 再生中
};


}// namespace hj

#endif // __WINDX_SOUND_WAVE_H_