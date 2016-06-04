//--------------------------------------------------------------
/*!	@file
	@brief	シングルトン
			
	@author 本城 直志
	@date	10/10/14
*/
//	Copyright (C) 2010 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_SINGLETON_H_
#define __HJ_SINGLETON_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	シングルトンテンプレートクラス
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
template< class T >
class clsHJSingleton{
	public:
		clsHJSingleton(){
			HJ_ASSERT( m_tpcSingleton==NULL, HJ_T("シングルトンが2つ以上作成されました。"));
			m_tpcSingleton = static_cast<T*>(this);
		};
		virtual ~clsHJSingleton(){
			m_tpcSingleton = NULL;
		};

		// ----- Get
		static T* GS(){return m_tpcSingleton;}
		static T& RS(){return *m_tpcSingleton;}
	protected:
	private:
		// シングルトンなのでコピーコンストラクタや代入は禁止
		clsHJSingleton(const clsHJSingleton& rcSingleton)				{ HJ_ASSERT_MSG( HJ_T("シングルトンに対してコピーコンストラクタを行いました。")); }
		clsHJSingleton& operator=(const clsHJSingleton& rcSingleton)	{ HJ_ASSERT_MSG( HJ_T("シングルトンに対してコピーコンストラクタを行いました。")); }

		// ----- Variable
		static T* m_tpcSingleton;
};

template< class T >T* clsHJSingleton< T >::m_tpcSingleton = NULL;

} //namespace hj


#endif // __HJ_SINGLETON_H_
