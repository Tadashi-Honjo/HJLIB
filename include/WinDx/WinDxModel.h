//--------------------------------------------------------------
/*!	@file
	@brief	WinDxモデル
			
	@author 本城 直志
	@date	14/02/24
*/
//	Copyright (C) 2014 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __WINDX_MODEL_H_
#define __WINDX_MODEL_H_

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	WinDx モデル
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
class clsWinDxModel : public clsHJModel
{
public:
	typedef clsHJModel MyClassParent;
	
	clsWinDxModel();
	virtual ~clsWinDxModel();

	virtual void Render();

private:
};

}// namespace hj

#endif // __WINDX_MODEL_H_