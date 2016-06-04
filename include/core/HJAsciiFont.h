//--------------------------------------------------------------
/*!	@file
	@brief	HJ Ascii Font
			
	@author 本城 直志
	@date	12/02/28
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#ifndef __HJ_ASCII_FONT_H_
#define __HJ_ASCII_FONT_H_

// HJLib namespace
namespace hj{

//---------------------------------------------------------------------//
//				フォント用テクスチャ管理クラス
//---------------------------------------------------------------------//
class clsHJAsciiFont
{
public:
	clsHJAsciiFont();
	~clsHJAsciiFont();

	enum{
		DRAW_MAXNUM = 0xFFFF,
	};

	// --- Method
	void Init( SpHJTexture spFontTexture, hj_u32 u32BlockWidth, hj_u32 u32CharaNum, hj_u32 u32BlockSizeX, hj_u32 u32BlockSizeY);
	void DrawText(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum=DRAW_MAXNUM);				//!< テキスト描画(直座標)
	void DrawTextCenter(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum=DRAW_MAXNUM);		//!< テキスト描画(直座標センタリング)
	void DrawTextFrame(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum=DRAW_MAXNUM);		//!< 枠テキスト描画(直座標)
	void DrawTextCenterFrame(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum=DRAW_MAXNUM);	//!< 枠テキスト描画(直座標センタリング)

	// --- Set
	void SetColor(hj_color32 c32Color){m_c32Color=c32Color;}				//!< カラー
	void SetColorFrame(hj_color32 c32Color){m_c32ColorFrame=c32Color;}		//!< 枠カラー
	void SetScale(hj_f32 f32Scale){m_f32Scale=f32Scale;}					//!< スケール
	void SetSpace(hj_f32 f32Spase){m_f32Space=f32Spase;}					//!< 文字間スペース
	void SetFrameOffset(hj_f32 f32Offset){m_f32FrameOffset=f32Offset;}		//!< 枠のオフセット

private:
	// --- Struct
	struct stcDrawArea{
		hj_f32 f32SizeW;
		hj_f32 f32SizeH;
	};

	// --- Method
	void _DrawText(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum=DRAW_MAXNUM, hj_color32 c32Color=HJ_WHITE);										//!< 内部用テキスト描画(直座標)
	void _DrawTextCenter(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum=DRAW_MAXNUM, hj_color32 c32Color=HJ_WHITE);								//!< 内部用テキスト描画(直座標センタリング)
	void DrawTextOrig(stcDrawArea* psOutDrawArea, const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum=DRAW_MAXNUM, hj_color32 c32Color=HJ_WHITE);		//!< テキスト描画(オリジナル)

	// --- Variable
	clsHJSprite**		m_ppcSprite;		//!< スプライトリスト
	hj_u32				m_u32CharaNum;		//!< 文字数
	hj_u32				m_u32SizeX;			//!< 文字サイズ
	hj_u32				m_u32SizeY;			//!< 文字サイズ
	hj_color32			m_c32Color, m_c32ColorFrame;	//!< 描画カラー
	hj_f32				m_f32Space;			//!< 描画スペース
	hj_f32				m_f32Scale;			//!< スケール
	hj_f32				m_f32FrameOffset;	//!< 枠のオフセット値
};

} //namespace hj

#endif // __HJ_ASCII_FONT_H_