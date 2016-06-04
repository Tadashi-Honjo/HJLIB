//--------------------------------------------------------------
/*!	@file
	@brief	WinDxFileマネージャ
			
	@author 本城 直志
	@date	12/10/01
*/
//	Copyright (C) 2012 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_FILE_MANAGER_H_
#define __WINDX_FILE_MANAGER_H_


// HjLib namespace
namespace hj{

// ----- Class
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ファイル管理クラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxFileMgr : public clsHJFileMgr
{
public:	
	clsWinDxFileMgr();
	virtual ~clsWinDxFileMgr();

protected:
	virtual hj_bool RequestInputFile(const hj_tchar* otcFilePath, void* pvBuff, hj_size_t sizeFile);
	virtual hj_bool RequestAllocInputFile(const hj_tchar* otcFilePath, hj_u8** ppu8Buff, hj_size_t* psizeFile=NULL);
	virtual hj_bool RequestOutputFile(const hj_tchar* otcFilePath, void* pvBuff, hj_size_t sizeFile);
};


} //namespace hj

#endif // __WINDX_FILE_MANAGER_H_
