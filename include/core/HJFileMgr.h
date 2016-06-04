//--------------------------------------------------------------
/*!	@file
	@brief	ファイル管理
			
	@author 本城 直志
	@date	12/10/01
*/
//	Copyright (C) 2012 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_FILE_MANAGER_H_
#define __HJ_FILE_MANAGER_H_

namespace hj{

// ファイル管理
class clsHJFileMgr
{
public:	
	clsHJFileMgr();
	virtual ~clsHJFileMgr();

	template <class Resource> hj_bool RequestLoadResource( const hj_tchar* otcFilePath, clsHJSharedPtr< Resource > spResource )
	{
		if( !spResource ){
			return false;
		}
		// パスで処理するものもあるので呼ぶ
		spResource->Build( otcFilePath );
		hj_bool ret = spResource->IsBuilded();
		if( !ret )
		{
			hj_u8* pu8Buff=NULL;
			hj_size_t size=0;
			if( RequestAllocInputFile(otcFilePath, &pu8Buff, &size) )
			{
				SpBinary spBin( NEW clsBinary(pu8Buff, size) );
				spResource->Build( spBin );
				ret = spResource->IsBuilded();
			}
		}
		return ret;
	}

	virtual hj_bool RequestOutputFile(const hj_tchar* otcFilePath, void* pvBuff, hj_size_t sizeFile) = 0;

protected:
	virtual hj_bool RequestInputFile(const hj_tchar* otcFilePath, void* pvBuff, hj_size_t sizeFile)=0;
	virtual hj_bool RequestAllocInputFile(const hj_tchar* otcFilePath, hj_u8** ppu8Buff, hj_size_t* psizeFile=NULL)=0;
};


} //namespace hj

#endif // __HJ_FILE_MANAGER_H_
