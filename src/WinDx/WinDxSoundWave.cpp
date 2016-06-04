//--------------------------------------------------------------
/*!	@file
	@brief	HJSoundWave
			
	@author 本城 直志
	@date	12/08/19
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxSoundWaveData::clsWinDxSoundWaveData() : m_hMmio(0)
//, m_pc8WaveData(NULL)
{
	System::Memset(&m_MmioInfo, 0, sizeof(m_MmioInfo));
	System::Memset(&m_WaveFmtEx, 0, sizeof(m_WaveFmtEx));
	System::Memset(m_atcFilePath, 0, sizeof(hj_tchar)*HJ_MAX_PATH);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxSoundWaveData::~clsWinDxSoundWaveData()
{
	Close();
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WAVEファイルオープン
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWaveData::Open(const hj_tchar* ptcFilePath)
{
	System::Memcpy(m_atcFilePath, ptcFilePath, sizeof(hj_tchar)*HJ_MAX_PATH);
	System::Memset(&m_MmioInfo, 0, sizeof(m_MmioInfo));

	if( !(m_hMmio = mmioOpen(m_atcFilePath, &m_MmioInfo, MMIO_READ)) ){
		HJ_ASSERT_MSG( HJ_T("File[ %s ]を開く事が出来ませんでした。"), m_atcFilePath);
		return false;
	}

	return true;
}


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WAVEファイルクローズ
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWaveData::Close()
{
	if(mmioClose(m_hMmio, 0) == MMIOERR_CANNOTWRITE){
		HJ_ASSERT_MSG( HJ_T("File[ %s ]を閉じる事が出来ませんでした。"), m_atcFilePath);
		return false;
	}

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WAVEヘッダがあるかチェック
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWaveData::CheckWAVEHeader()
{
	m_RIFFChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if( mmioDescend(m_hMmio, &m_RIFFChunk, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR ){
		HJ_ASSERT_MSG( HJ_T("File[ %s ]は WAVE ヘッダが存在しません。"), m_atcFilePath);
		return false;
	}

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	FMT チャンクがあるかチェック
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWaveData::CheckFMTChunk()
{
	m_FMTChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	
	if( mmioDescend(m_hMmio, &m_FMTChunk, &m_RIFFChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
	{
		HJ_ASSERT_MSG( HJ_T("File[ %s ]は FMT チャンクが存在しません。"), m_atcFilePath);
		return false;
	}

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	Dataチャンクがあるかチェック
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWaveData::CheckDATAChunk()
{
	m_DATAChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if( mmioDescend(m_hMmio, &m_DATAChunk, &m_RIFFChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
	{
		HJ_ASSERT_MSG( HJ_T("File[ %s ]は DATA チャンクが存在しません。"), m_atcFilePath);
		return false;
	}

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WAVEFORMATEXにデータを読み込む
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWaveData::ReadToWaveFmtEx()
{
	// mmioRead は成功すると読みこんだサイズを返す
	LONG wReadSize = mmioRead(m_hMmio, reinterpret_cast<HPSTR>(&m_WaveFmtEx), m_FMTChunk.cksize);

	// 0:それ以上読み込めない　-1:ファイルの読み取りに失敗
	switch(wReadSize)
	{
		//-----------------------------------------------------
		case 0:{
			OR( HJ_T("File[ %s ]はこれ以上読み込めません。"), m_atcFilePath );
		}break;
		//-----------------------------------------------------
		case -1:{
			OR( HJ_T("File[ %s ]は読みとれませんでした。"), m_atcFilePath );
		}break;
		//-----------------------------------------------------
		default:{
			// データサイズが合ってるかチェックする
			if(wReadSize != m_FMTChunk.cksize){
				HJ_ASSERT_MSG( HJ_T("File[ %s ]の読みこんだデータサイズが異なります。"), m_atcFilePath);
				return false;
			}
		}break;
	}

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	m_WaveData に WAVE データを読み込む
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
LONG clsWinDxSoundWaveData::ReadToWaveData(hj_u8* pu8Data, DWORD size)
{
	return mmioRead(m_hMmio, reinterpret_cast<HPSTR>(pu8Data), size);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ファイル名を取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
const hj_tchar* clsWinDxSoundWaveData::GetFileName()const
{
	return m_atcFilePath;
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WAVE フォーマットを取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
const WAVEFORMATEX& clsWinDxSoundWaveData::GetWaveFmtEx()const
{
	return m_WaveFmtEx;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WAVE データのサイズを取得
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
const hj_size_t clsWinDxSoundWaveData::GetWaveSize()const
{
	return m_DATAChunk.cksize;
}


//  ============================================ clsWinDxSoundWave

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxSoundWave::clsWinDxSoundWave() : clsWinDxSound()
, clsWinDxSoundWaveData()
, m_pSVoice(NULL)
, m_pc8WaveData(NULL)
{
	System::Memset(&m_WaveFmt, 0, sizeof(m_WaveFmt));
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxSoundWave::~clsWinDxSoundWave()
{
	Close();
	Exit();
	HJ_SAFE_DELETE_ARRAY(m_pc8WaveData);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	サウンドデータ読み込み
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWave::LoadFile(const hj_tchar* ptcFilePath)
{
	// ファイルオープン
	if(!Open(ptcFilePath))
		return false;

	// WAVE ヘッダのチェック
	if(!CheckWAVEHeader())
		return false;

	// FMT チャンクのチェック
	if(!CheckFMTChunk())
		return false;

	// WAVEFORMATEX 構造体にデータを読み込む
	if(!ReadToWaveFmtEx())
		return false;

	// 読み込み位置を先頭に戻す
	mmioAscend(m_hMmio, &m_FMTChunk, SEEK_SET);

	// DATA チャンクのチェック
	if(!CheckDATAChunk())
		return false;

	// WAVE データを読み込む
	HJ_SAFE_DELETE_ARRAY(m_pc8WaveData);
	hj_size_t size = GetWaveSize();
	m_pc8WaveData = NEW hj_u8[size];
	if( ReadToWaveData(m_pc8WaveData, size) != size ){
		HJ_ASSERT_MSG( HJ_T("File[ %s ]の読みこんだデータサイズが異なります。"), m_atcFilePath);
		return false;
	}

	// ファイルを閉じる
	Close();

	// ソースヴォイスの作成
	if(!CreateSVoice())
		return false;

	// 読み込んだ音楽データをキューに送信
//	Submit();

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ソースヴォイス作成
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWave::CreateSVoice()
{
	if( FAILED(m_pXAudio2->CreateSourceVoice(&m_pSVoice, &GetWaveFmtEx())) )
	{
		HJ_ASSERT_MSG( HJ_T("ソースヴォイスの作成に失敗しました"));
		return false;
	}
	
//	OR(HJ_T("ソースヴォイスの作成に成功しました"));
	return true;
}



//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ソースヴォイスキューにサウンドデータを送信
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxSoundWave::Submit()
{
	XAUDIO2_BUFFER wSubmit = { 0 };
	wSubmit.AudioBytes		= GetWaveSize(); 
	wSubmit.pAudioData		= m_pc8WaveData;
	wSubmit.Flags			= XAUDIO2_END_OF_STREAM;
	if(m_bLoop)	wSubmit.LoopCount = XAUDIO2_LOOP_INFINITE;

	if( m_pSVoice->SubmitSourceBuffer(&wSubmit) != S_OK ){
		HJ_ASSERT_MSG( HJ_T("音楽データの送信に失敗しました"));
		return false;
	}

//	OR(HJ_T("音楽データの送信に成功しました"));
	return true;	
}



//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	終了処理
	@param	none
	@return result
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxSoundWave::Exit()
{
	// ソースヴォイスの破棄
	if(m_pSVoice){
		m_pSVoice->Stop(0);
		m_pSVoice->DestroyVoice();
		m_pSVoice = NULL;
	}
}

// 再生
void clsWinDxSoundWave::Play(hj_bool bLoop){
	if(!m_pSVoice) return;

	
	if(!m_bPause){
		// 状態
		XAUDIO2_VOICE_STATE state;
		m_pSVoice->GetState( &state );
		// バッファ最大数超えないように
		if( state.BuffersQueued < XAUDIO2_MAX_QUEUED_BUFFERS ){
			clsHJSound::Play(bLoop);
//			m_pSVoice->Stop(0);
			m_pSVoice->Stop(XAUDIO2_PLAY_TAILS);
			m_pSVoice->FlushSourceBuffers();	// 違うバッファは消しておく
			Submit();
		}
	}
	m_bPause = false;
	m_pSVoice->Start(0);
}

// 停止
void clsWinDxSoundWave::Stop()
{
	if(!m_pSVoice) return;
	
	m_bPause = false;	// ポーズ解除
	m_pSVoice->Stop(0);
}

// 一時停止
void clsWinDxSoundWave::Pause()
{
	if(!m_pSVoice) return;
	
	clsHJSound::Pause();
	m_pSVoice->Stop(0);
}

// ボリュームの設定
void clsWinDxSoundWave::SetVolume(hj_f32 f32Volume)
{
	if(!m_pSVoice) return;
	
	clsHJSound::SetVolume(f32Volume);
	m_pSVoice->SetVolume(f32Volume);
}


//  ============================================ clsWinDxStreamSoundWave

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxStreamSoundWave::clsWinDxStreamSoundWave() : clsWinDxSound()
, clsWinDxSoundWaveData()
, m_pSVoice(NULL)
, m_s8WaveBuffCnt(-1)
, m_sizeWaveBuff(0)
, m_sizeReadWaveCnt(0)
, m_sizeWaveBegin(0)
, m_sizeLoopPoint(0)
, m_bPlay(false)
{
	System::Memset(&m_WaveFmt, 0, sizeof(m_WaveFmt));
	System::Memset(m_apc8WaveDataBuff, 0, sizeof(hj_u8*)*m_tos8WaveDataMaxNum);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxStreamSoundWave::~clsWinDxStreamSoundWave()
{
	Close();
	Exit();
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	サウンドデータ読み込み
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxStreamSoundWave::LoadFile(const hj_tchar* ptcFilePath)
{
	Exit();	// 消しておく

	// ファイルオープン
	if(!Open(ptcFilePath))
		return false;

	// WAVE ヘッダのチェック
	if(!CheckWAVEHeader())
		return false;

	// FMT チャンクのチェック
	if(!CheckFMTChunk())
		return false;

	// WAVEFORMATEX 構造体にデータを読み込む
	if(!ReadToWaveFmtEx())
		return false;

	// 読み込み位置を先頭に戻す
	mmioAscend(m_hMmio, &m_FMTChunk, SEEK_SET);

	// DATA チャンクのチェック
	if(!CheckDATAChunk())
		return false;

	// WAVE データを読み込む
	hj_s32 s32i=0;
	m_sizeWaveBuff = m_WaveFmtEx.nAvgBytesPerSec * 1;
	for( s32i=0 ; s32i<m_tos8WaveDataMaxNum ; ++s32i ){
		m_apc8WaveDataBuff[s32i] = NEW hj_u8[m_sizeWaveBuff];
	}
	m_s8WaveBuffCnt = -1;	// バッファNoを初期化
	m_sizeReadWaveCnt = GetWaveSize();
	m_sizeWaveBegin = mmioSeek(m_hMmio, 0, SEEK_CUR);

	// ファイルを閉じる
//	Close();

	// ソースヴォイスの作成
	if(!CreateSVoice())
		return false;

	// バッファ分SubMitしとく
	for( s32i=0 ; s32i<m_tos8WaveDataMaxNum ; ++s32i ){
		Submit();
	}

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	更新
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxStreamSoundWave::Update()
{
	if(!m_pSVoice) return;

	if( m_bPlay ){
		// 状態
		XAUDIO2_VOICE_STATE state;
		m_pSVoice->GetState( &state );

		// 数が減っていたら追加
		if( state.BuffersQueued < m_tos8WaveDataMaxNum ){
			Submit();
		}
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ソースヴォイス作成
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxStreamSoundWave::CreateSVoice()
{
//	if( FAILED(m_pXAudio2->CreateSourceVoice(&m_pSVoice, &GetWaveFmtEx(), 0, XAUDIO2_DEFAULT_FREQ_RATIO, (IXAudio2VoiceCallback*)this )) )
	if( FAILED(m_pXAudio2->CreateSourceVoice(&m_pSVoice, &GetWaveFmtEx(), 0, XAUDIO2_DEFAULT_FREQ_RATIO)) )
	{
		HJ_ASSERT_MSG( HJ_T("ソースヴォイスの作成に失敗しました"));
		return false;
	}
	
//	OR(HJ_T("ソースヴォイスの作成に成功しました"));
	return true;
}



//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ソースヴォイスキューにサウンドデータを送信
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxStreamSoundWave::Submit()
{
	m_s8WaveBuffCnt = ++m_s8WaveBuffCnt >= m_tos8WaveDataMaxNum ? 0 : m_s8WaveBuffCnt;
	hj_bool bEnd = false;
	hj_size_t sizeWaveData = m_sizeWaveBuff;
	if(m_sizeReadWaveCnt < m_sizeWaveBuff ){
		sizeWaveData = m_sizeReadWaveCnt;
		bEnd = true;
	}
	if( ReadToWaveData(m_apc8WaveDataBuff[m_s8WaveBuffCnt], m_sizeWaveBuff) == 0 ){
		bEnd = true;
	}
	m_sizeReadWaveCnt -= m_sizeWaveBuff;

	if( bEnd ){
		// 一番始めに戻る
		if(m_bLoop){
			m_sizeReadWaveCnt = GetWaveSize();
			m_sizeReadWaveCnt -= m_sizeLoopPoint;
			if( -1 == mmioSeek( m_hMmio, (m_sizeWaveBegin+m_sizeLoopPoint), SEEK_SET ) ){
				HJ_ASSERT_MSG( HJ_T("サウンドループポイントへのシークに失敗しました"));
				return false;
			}
		// ループしない
		}else{
			m_bPlay = false;
		}
	}

	XAUDIO2_BUFFER wSubmit = { 0 };
	wSubmit.AudioBytes		= sizeWaveData; 
	wSubmit.pAudioData		= m_apc8WaveDataBuff[m_s8WaveBuffCnt];
	wSubmit.Flags			= XAUDIO2_END_OF_STREAM;
//	wSubmit.LoopCount		= XAUDIO2_LOOP_INFINITE;

	if( m_pSVoice->SubmitSourceBuffer(&wSubmit) != S_OK ){
		HJ_ASSERT_MSG( HJ_T("音楽データの送信に失敗しました"));
		return false;
	}

//	OR(HJ_T("音楽データの送信に成功しました"));
	return true;	
}



//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	終了処理
	@param	none
	@return result
	@*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsWinDxStreamSoundWave::Exit()
{
	// ソースヴォイスの破棄
	if(m_pSVoice){
		m_pSVoice->Stop(0);
		m_pSVoice->DestroyVoice();
		m_pSVoice = NULL;
	}

	hj_s32 s32i=0;
	for( s32i=0 ; s32i<m_tos8WaveDataMaxNum ; ++s32i ){
		HJ_SAFE_DELETE_ARRAY(m_apc8WaveDataBuff[s32i]);
	}
}

// 再生
void clsWinDxStreamSoundWave::Play(hj_bool bLoop){
	if(!m_pSVoice) return;

	if(!m_bPause){
		// 状態
		XAUDIO2_VOICE_STATE state;
		m_pSVoice->GetState( &state );
		// バッファ最大数超えないように
		if( state.BuffersQueued < XAUDIO2_MAX_QUEUED_BUFFERS){
			// 最初に戻る
			m_sizeReadWaveCnt = GetWaveSize();
			m_sizeReadWaveCnt -= m_sizeLoopPoint;
			if( -1 == mmioSeek( m_hMmio, (m_sizeWaveBegin+m_sizeLoopPoint), SEEK_SET ) ){
				HJ_ASSERT_MSG( HJ_T("サウンドループポイントへのシークに失敗しました"));
				return;
			}
			clsHJSound::Play(bLoop);
			m_pSVoice->FlushSourceBuffers();	// バッファはクリア

			// バッファ分SubMitしとく
			hj_s32 s32i=0;
			for( s32i=0 ; s32i<m_tos8WaveDataMaxNum ; ++s32i ){
				Submit();
			}
		}
	}
	m_bPlay = true;
	m_bPause = false;
	m_pSVoice->Start(0);
}

// 停止
void clsWinDxStreamSoundWave::Stop(){
	if(!m_pSVoice) return;
	
	m_bPlay = false;
	m_bPause = false;	// ポーズ解除
	m_pSVoice->Stop(0);
}

// 一時停止
void clsWinDxStreamSoundWave::Pause()
{
	if(!m_pSVoice) return;
	
	m_bPlay = false;
	clsHJSound::Pause();
	m_pSVoice->Stop(0);
}

// ボリュームの設定
void clsWinDxStreamSoundWave::SetVolume(hj_f32 f32Volume)
{
	if(!m_pSVoice) return;
	
	clsHJSound::SetVolume(f32Volume);
	m_pSVoice->SetVolume(f32Volume);
}



}// namespace hj