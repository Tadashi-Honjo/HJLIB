//--------------------------------------------------------------
/*!	@file
	@brief	HJSound
			
	@author 本城 直志
	@date	12/08/19
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_SOUND_H_
#define __HJ_SOUND_H_

// HJLib namespace
namespace hj{

// サウンドリソース(オンメモリ)
class clsSoundResource : public clsHJResource
{
	typedef clsHJResource MyClassParent;
public:
	clsSoundResource():MyClassParent(){}
	virtual ~clsSoundResource(){}

	SpHJSound GetSound()const{ return m_spSound; }

protected:
	virtual hj_bool Builded( const hj_tchar* otcFilePath );
	virtual hj_bool Builded( SpBinary spBinary ){ return false; }
private:
	SpHJSound	m_spSound;
};
typedef clsHJSharedPtr< clsSoundResource > SpSoundResource;

// サウンドリソース(ストリーミング)
class clsSoundStreamResource : public clsHJResource
{
	typedef clsHJResource MyClassParent;
public:
	clsSoundStreamResource():MyClassParent(){}
	virtual ~clsSoundStreamResource(){}

	SpHJSound GetSound()const{ return m_spSound; }

protected:
	virtual hj_bool Builded( const hj_tchar* otcFilePath );
	virtual hj_bool Builded( SpBinary spBinary ){ return false; }
private:
	SpHJSound	m_spSound;
};
typedef clsHJSharedPtr< clsSoundStreamResource > SpSoundStreamResource;

// HJサウンド
class clsHJSound
{
public:
	clsHJSound():m_bLoop(false), m_bPause(false), m_f32Volume(1.0f){};
	virtual ~clsHJSound(){};

	// --- Method
	virtual hj_bool	LoadFile(const hj_tchar* ptcFilePath)=0;
	virtual void	Update(){};	// ストリーミング更新用
	virtual void	Play(hj_bool bLoop=false){m_bLoop = bLoop;}
	virtual void	Stop()=0;
	virtual void	Pause(){m_bPause=true;};

	// --- Set
	virtual void 	SetVolume(hj_f32 f32Volume){m_f32Volume=f32Volume;}
	// --- Get
	virtual hj_f32 	GetVolume()const{return m_f32Volume;};

protected:
	hj_bool		m_bLoop, m_bPause;
	hj_f32		m_f32Volume;
};

}// namespace hj

#endif // __HJ_SOUND_H_