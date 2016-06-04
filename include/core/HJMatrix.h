//--------------------------------------------------------------
/*!	@file
	@brief	Matrix
			
	@author 本城 直志
	@date	11/05/23
*/
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_LIB_MATRIX_H_
#define __HJ_LIB_MATRIX_H_

// HJLib namespace
namespace hj{
	
// --- Typedef
#if defined(PF_WINDX)
	typedef D3DMATRIX	stcOrigDataMatrix44;	// データ
	typedef D3DXMATRIX  stcOrigCalcMatrix44;	// 計算用

#elif defined(PF_WINGLES)
	// プラットフォーム依存のベクトルが無い場合はダミー
	struct stcOrigDataMatrix44{
        hj_f32 m[4][4];
    };
	typedef stcOrigDataMatrix44 stcOrigCalcMatrix44;	// 計算用
#else
	#error "Not Define stcOriginalVector3"
#endif

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix4x4データ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcMatrix44Data{
	union{
		struct {
            hj_f32	m11, m12, m13, m14;
            hj_f32	m21, m22, m23, m24;
            hj_f32	m31, m32, m33, m34;
            hj_f32	m41, m42, m43, m44;
        };
        hj_f32 m[4][4];
		stcOrigDataMatrix44 mtx;
	};
};

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	matrix4x4
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
struct stcMatrix44 : public stcMatrix44Data
{
public:
    stcMatrix44() {};
//    stcMatrix44( const hj_f32* opf32Mtx );
	stcMatrix44( const stcMatrix44& orsMtx );
    stcMatrix44( hj_f32 _11, hj_f32 _12, hj_f32 _13, hj_f32 _14,
                hj_f32 _21, hj_f32 _22, hj_f32 _23, hj_f32 _24,
                hj_f32 _31, hj_f32 _32, hj_f32 _33, hj_f32 _34,
                hj_f32 _41, hj_f32 _42, hj_f32 _43, hj_f32 _44 );

	// --- Static Method
	static void		Initialize();																			//!< 初期化(静的メンバ)
	static void		Copy(stcMatrix44* psOutMtx, const stcMatrix44& orsMtx);									//!< matrix44コピー
	static void		Identity(stcMatrix44* psOutMtx);														//!< matrix44単位行列化
//	static void 	Add(stcMatrix44* psOutMtx, const stcMatrix44& orsMtx1, const stcMatrix44& orsMtx2);		//!< matrix44加算
//	static void 	Sub(stcMatrix44* psOutMtx, const stcMatrix44& orsMtx1, const stcMatrix44& orsMtx2);		//!< matrix44減算
	static void 	Multiply(stcMatrix44* psOutMtx, const stcMatrix44& orsMtx1, const stcMatrix44& orsMtx2);//!< matrix44乗算
//	static void 	Div(stcMatrix44* psOutMtx, const stcMatrix44& orsMtx1, const stcMatrix44& orsMtx2);		//!< matrix44除算
//	static void		Matrix44RotationX()

	// --- Static Get
	static const stcMatrix44* GetIndentityMatrix(){ return &m_tsIdentityMtx; }								//!< 単位行列取得

	// --- Method
	stcOrigCalcMatrix44* GetOrigPtr()					{ return static_cast<stcOrigCalcMatrix44*>(&mtx); }
	const stcOrigCalcMatrix44* GetOrigConstPtr() const	{ return static_cast<const stcOrigCalcMatrix44*>(&mtx); }

	void Identity();					//!< 単位行列化
	void Inverse();						//!< 逆行列化

	// 直接変換
	void SetTranslate(const stcVector3& orsPos);
	void SetRotationX(hj_f32 f32Deg);
	void SetRotationY(hj_f32 f32Deg);
	void SetRotationZ(hj_f32 f32Deg);
	void SetRotationXyz(const stcVector3& orsRot);
	void SetScale(const stcVector3& orsScale);

	// 行列と乗算変換
	void Translate(const stcVector3& orsPos);
	void RotationX(hj_f32 f32Deg);
	void RotationY(hj_f32 f32Deg);
	void RotationZ(hj_f32 f32Deg);
	void RotationXyz(const stcVector3& orsRot);
	void Scale(const stcVector3& orsScale);

	// --- other
	void SetCameraView(const stcVector3& orsEye, const stcVector3& orsAt, const stcVector3& orsUp);	//!< ビューマトリクス
	void SetCameraProjection(hj_f32 f32Fovy, hj_f32 f32Aspect, hj_f32 f32NearZ, hj_f32 f32FarZ );	//!< プロジェクションマトリクス

    // --- assignment operators
    stcMatrix44& operator *= ( const stcMatrix44& orsMtx );
//    stcMatrix44& operator += ( const stcMatrix44& orsMtx );
//    stcMatrix44& operator -= ( const stcMatrix44& orsMtx );
//    stcMatrix44& operator *= ( hj_f32 f32Param );
//    stcMatrix44& operator /= ( hj_f32 f32Param );

    // --- binary operators
    stcMatrix44 operator * ( const stcMatrix44& orsMtx ) const;
//    stcMatrix44 operator + ( const stcMatrix44& orsMtx ) const;
//    stcMatrix44 operator - ( const stcMatrix44& orsMtx ) const;
//    stcMatrix44 operator * ( hj_f32 f32Param) const;
//    stcMatrix44 operator / ( hj_f32 f32Param) const;
	
	// --- compare operators
//    hj_bool operator == ( const stcMatrix44& orsMtx ) const;
//    hj_bool operator != ( const stcMatrix44& orsMtx ) const;


private:
	// --- Static Const Variable
	static stcMatrix44	m_tsIdentityMtx;
};

}// namespace hj

#endif // __HJ_LIB_MATRIX_H_