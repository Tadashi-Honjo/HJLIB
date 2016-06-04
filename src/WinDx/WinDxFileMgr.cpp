//--------------------------------------------------------------
/*!	@file
	@brief	WinDxFileマネージャ
			
	@author 本城 直志
	@date	12/10/01
*/
//	Copyright (C) 2012 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxFileMgr::clsWinDxFileMgr():clsHJFileMgr()
{
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsWinDxFileMgr::~clsWinDxFileMgr()
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ファイルを読み込み
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxFileMgr::RequestInputFile(const hj_tchar* otcFilePath, void* pvBuff, hj_size_t sizeFile)
{
	FILE *fp;
	errno_t error = _tfopen_s(&fp, otcFilePath, HJ_T("rb"));
	if( error != 0 ){
		return false;
	}
	// ファイルサイズ
	fseek(fp, 0, SEEK_END); 
	fpos_t posFileSize=0;
	fgetpos(fp, &posFileSize); 
	fseek(fp, 0, SEEK_SET);		// 先頭に

	// サイズエラー
	if(posFileSize > SIZE_MAX || posFileSize != sizeFile){
		fclose( fp );
		return false;
	}
	// バッファ確保・読み込み
	fread(pvBuff, sizeFile, 1, fp);

	fclose( fp );

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ファイルを読み込み(内部メモリ確保)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxFileMgr::RequestAllocInputFile(const hj_tchar* otcFilePath, hj_u8** ppu8Buff, hj_size_t* psizeFile)
{
	FILE *fp;
	errno_t error = _tfopen_s(&fp, otcFilePath, HJ_T("rb"));
	if( error != 0 ){
		return false;
	}
	// ファイルサイズ
	fseek(fp, 0, SEEK_END); 
	fpos_t posFileSize=0;
	fgetpos(fp, &posFileSize); 
	fseek(fp, 0, SEEK_SET);		// 先頭に

	// サイズオーバー
	if(posFileSize >= SIZE_MAX){
		fclose( fp );
		return false;
	}
	// バッファ確保・読み込み
	hj_size_t sizeFile = static_cast<hj_size_t>(posFileSize);
	*ppu8Buff = NEW hj_u8[sizeFile];
	fread(*ppu8Buff, sizeFile, 1, fp);

	if(psizeFile) *psizeFile = static_cast<hj_size_t>(posFileSize);

	fclose( fp );

	return true;
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ファイルを書き込み
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
hj_bool clsWinDxFileMgr::RequestOutputFile(const hj_tchar* otcFilePath, void* pvBuff, hj_size_t sizeFile)
{
	FILE *fp;
	errno_t error = _tfopen_s(&fp, otcFilePath, HJ_T("wb"));
	if(error != 0 ){
		return false;
	}
	fwrite(pvBuff, sizeFile, 1, fp);
	fclose( fp );

	return true;
}



}// namespace hj