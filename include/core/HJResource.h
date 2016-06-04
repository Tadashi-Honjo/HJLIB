//--------------------------------------------------------------
/*!	@file
	@brief	HJリソース
			
	@author 本城 直志
	@date	15/02/01
*/
//	Copyright (C) 2015 HonjoTadashi. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_RESOURCE_H_
#define __HJ_RESOURCE_H_

namespace hj{

// バイナリ
class clsBinary
{
public:
	clsBinary(hj_u8* buffer, hj_size_t size):m_buffer(buffer), m_size(size){}
	~clsBinary(){ HJ_SAFE_DELETE_ARRAY(m_buffer); }

	const hj_u8*	GetBuffer()const	{ return m_buffer; }
	hj_size_t		GetSize()const		{ return m_size; }

private:
	hj_u8*		m_buffer;
	hj_size_t	m_size;

	friend class clsHJFileMgr;
};

// リソース
class clsHJResource
{
public:
	clsHJResource() : m_builded(false){}
	virtual ~clsHJResource(){}

	hj_bool IsBuilded()const{ return m_builded; }

protected:
	virtual hj_bool Builded( const hj_tchar* otcFilePath ){ return false; }	// 優先的に読まれる(falseならバイナリが呼ばれる。)
	virtual hj_bool Builded( SpBinary spBinary )=0;

private:
	// clsHJFileMgrから呼ぶ用
	void Build( const hj_tchar* otcFilePath ); 
	void Build( SpBinary spBinary );

	hj_bool m_builded;

	friend class clsHJFileMgr;
};

} //namespace hj

#endif // __HJ_RESOURCE_H_

