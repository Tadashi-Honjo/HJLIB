//--------------------------------------------------------------
/*!	@file
	@brief	Vector
			
	@author 本城 直志
	@date	11/05/05
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_VECTOR_H_
#define __HJ_LIB_VECTOR_H_

// HJLib namespace
namespace hj{

struct stcMatrix44;
	
// --- Typedef
#if defined(PF_WINDX)
	typedef D3DVECTOR	stcOrigDataVector3;	// データ
	typedef D3DXVECTOR3 stcOrigCalcVector3;	// 計算用

#elif defined(PF_WINGLES)
	// プラットフォーム依存のベクトルが無い場合はダミー
	struct stcOrigDataVector3{
		hj_f32 x, y, z;
	};
	typedef stcOrigDataVector3 stcOrigCalcVector3;	// 計算用
#else
	#error "Not Define stcOriginalVector3"
#endif

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3データ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcVector3Data{
	union{
		struct{ hj_f32	x, y, z; };
		hj_f32	a[3];
		stcOrigDataVector3 vec;
	};
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	vector3
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcVector3 : public stcVector3Data
{
public:
	stcVector3(){ Set(0.0f, 0.0f, 0.0f); }
	stcVector3(const hj_f32* opf32Vec){ Set(opf32Vec[0], opf32Vec[1], opf32Vec[2]);}
	stcVector3(hj_f32 f32x, hj_f32 f32y, hj_f32 f32z){ Set(f32x, f32y, f32z); }
	stcVector3(const stcVector3& orsVec){ Set(orsVec.x, orsVec.y, orsVec.z);}
	
	// --- Static Method
	static void 	Add(stcVector3* psOutVec, const stcVector3& orsVec1, const stcVector3& orsVec2);				//!< 加算
	static void 	Sub(stcVector3* psOutVec, const stcVector3& orsVec1, const stcVector3& orsVec2);				//!< 減算
	static void 	Scale(stcVector3* psOutVec, const stcVector3& orsVec, hj_f32 f32Scale);							//!< スケール
	static void 	Cross(stcVector3* psOutVec, const stcVector3& orsVec1, const stcVector3& orsVec2);				//!< 外積
	static hj_f32 	Dot(const stcVector3& orsVec1, const stcVector3& orsVec2);										//!< 内積
	static void 	Normalize3(stcVector3* psOutVec, const stcVector3& orsVec);										//!< 正規化(xyz)
	static hj_f32 	Length3(const stcVector3& orsVec);																//!< 大きさ(xyz)
	static hj_f32 	Length3Sq(const stcVector3& orsVec);															//!< 大きさ2乗(xyz)
	static void 	Normalize2(stcVector3* psOutVec, const stcVector3& orsVec);										//!< 正規化(xy)
	static hj_f32 	Length2(const stcVector3& orsVec);																//!< 大きさ(xy)
	static hj_f32 	Length2Sq(const stcVector3& orsVec);															//!< 大きさ2乗(xy)
	static void		TransformMatrix(stcVector3* psOutVec, const stcVector3& orsVec, const stcMatrix44& orsMtx);		//!< 行列変換

	// --- Method
	void	Set(hj_f32 f32x, hj_f32 f32y, hj_f32 f32z)	{ x=f32x; y=f32y; z=f32z;	}								//!< 設定
	void	Normalize3()								{ Normalize3(this, *this);	}								//!< 正規化(xyz)
	hj_f32	Length3() const								{ return Length3(*this);	}								//!< 大きさ(xyz)
	hj_f32 	Length3Sq() const							{ return Length3Sq(*this);	}								//!< 大きさ2乗(xyz)
	void	Normalize2()								{ Normalize2(this, *this);	}								//!< 正規化(xy)
	hj_f32	Length2() const								{ return Length2(*this);	}								//!< 大きさ(xy)
	hj_f32 	Length2Sq() const							{ return Length2Sq(*this);	}								//!< 大きさ2乗(xy)
	void	TransformMatrix(const stcMatrix44& orsMtx);																//!< 行列変換

	// --- Get
	stcOrigCalcVector3* GetOrigPtr()					{ return static_cast<stcOrigCalcVector3*>(&vec);		}
	const stcOrigCalcVector3* GetOrigConstPtr() const	{ return static_cast<const stcOrigCalcVector3*>(&vec);	}
		
	// --- assignment operators
	stcVector3& operator += ( const stcVector3& orsVec );
    stcVector3& operator -= ( const stcVector3& orsVec );
    stcVector3& operator *= ( hj_f32 f32Param );
    stcVector3& operator /= ( hj_f32 f32Param );
	
    // --- unary operators
//    stcVector3 operator + () const;
//    stcVector3 operator - () const;
	
    // --- binary operators
    stcVector3 operator + ( const stcVector3& orsVec) const;
    stcVector3 operator - ( const stcVector3& orsVec) const;
    stcVector3 operator * ( hj_f32 f32Param) const;
    stcVector3 operator / ( hj_f32 f32Param) const;
	
    hj_bool operator == ( const stcVector3& orsVec) const;
    hj_bool operator != ( const stcVector3& orsVec) const;
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ベクトル4データ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcVector4Data{
	union{
		struct{ hj_f32	x, y, z, w; };
		hj_f32	a[4];

#if defined(PF_WINDX) || defined(PF_WINGLES)
//		D3DXVECTOR4 vec;
#else
	#error "Not Define Vector4Data"
#endif
	};
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ベクトル4
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcVector4 : public stcVector4Data
{
public:
	stcVector4(){ Set(0.0f, 0.0f, 0.0f, 1.0f); }
	stcVector4(const hj_f32* opf32Vec){ Set(opf32Vec[0], opf32Vec[1], opf32Vec[2], opf32Vec[3]);}
	stcVector4(hj_f32 f32x, hj_f32 f32y, hj_f32 f32z, hj_f32 f32w=1.0f){ Set(f32x, f32y, f32z, f32w); }
	stcVector4(const stcVector4& orsVec){ Set(orsVec.x, orsVec.y, orsVec.z, orsVec.w);}
	
	// --- Method
	hj_f32* GetPtr(){return &a[0];}
	void Set(hj_f32 f32x, hj_f32 f32y, hj_f32 f32z, hj_f32 f32w=1.0f){x=f32x; y=f32y; z=f32z; w=f32w;}		//!< 設定

/*
	void Normalize();																						//!< 正規化
	
	// --- assignment operators
	stcVector4& operator += ( const stcVector4& orsVec );
    stcVector4& operator -= ( const stcVector4& orsVec );
    stcVector4& operator *= ( hj_f32 f32Param );
    stcVector4& operator /= ( hj_f32 f32Param );
	
    // --- unary operators
    stcVector4 operator + () const;
    stcVector4 operator - () const;
	
    // --- binary operators
    stcVector4 operator + ( const stcVector4& orsVec) const;
    stcVector4 operator - ( const stcVector4& orsVec) const;
    stcVector4 operator * ( hj_f32 f32Param) const;
    stcVector4 operator / ( hj_f32 f32Param) const;
	
    hj_bool operator == ( const stcVector4& orsVec) const;
    hj_bool operator != ( const stcVector4& orsVec) const;
*/
};


//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ベクトル2データ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcVector2Data{
	union{
		struct{ hj_f32	x, y; };
		hj_f32	a[2];

#if defined(PF_WINDX) || defined(PF_WINGLES)
//		D3DXVECTOR2 vec;
#else
	#error "Not Define Vector2Data"
#endif
	};
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	ベクトル2
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcVector2 : public stcVector2Data
{
public:
	stcVector2(){ Set(0.0f, 0.0f); }
	stcVector2(const hj_f32* opf32Vec){ Set(opf32Vec[0], opf32Vec[1]);}
	stcVector2(hj_f32 f32x, hj_f32 f32y){ Set(f32x, f32y); }
	stcVector2(const stcVector2& orsVec){ Set(orsVec.x, orsVec.y);}
	
	// --- Method
	hj_f32* GetPtr(){return &a[0];}
	void Set(hj_f32 f32x, hj_f32 f32y){x=f32x; y=f32y;}		//!< 設定

/*
	void Normalize();																						//!< 正規化
	
	// --- assignment operators
	stcVector2& operator += ( const stcVector2& orsVec );
    stcVector2& operator -= ( const stcVector2& orsVec );
    stcVector2& operator *= ( hj_f32 f32Param );
    stcVector2& operator /= ( hj_f32 f32Param );
	
    // --- unary operators
    stcVector2 operator + () const;
    stcVector2 operator - () const;
	
    // --- binary operators
    stcVector2 operator + ( const stcVector2& orsVec) const;
    stcVector2 operator - ( const stcVector2& orsVec) const;
    stcVector2 operator * ( hj_f32 f32Param) const;
    stcVector2 operator / ( hj_f32 f32Param) const;
	
    hj_bool operator == ( const stcVector2& orsVec) const;
    hj_bool operator != ( const stcVector2& orsVec) const;
*/
};
	
}// namespace hj

#endif // __HJ_LIB_VECTOR_H_