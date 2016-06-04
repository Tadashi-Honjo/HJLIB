//--------------------------------------------------------------
/*!	@file
	@brief	HJ Ascii Font
			
	@author 本城 直志
	@date	12/02/28
*/
//	Copyright (C) 2012 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------

// HJLib namespace
namespace hj{

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	コンストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJAsciiFont::clsHJAsciiFont():
m_ppcSprite(NULL)
, m_u32SizeX(32)
, m_u32SizeY(32)
, m_c32Color(HJ_WHITE)
, m_c32ColorFrame(HJ_GREEN)
, m_f32Space(0.f)
, m_f32Scale(1.f)
, m_f32FrameOffset(3.0f)
{
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	デストラクタ
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
clsHJAsciiFont::~clsHJAsciiFont()
{
	for(hj_u32 u32i=0; u32i<m_u32CharaNum; ++u32i){
		HJ_SAFE_DELETE(m_ppcSprite[u32i]);
	}
	HJ_SAFE_DELETE_ARRAY(m_ppcSprite);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	初期化
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::Init( SpHJTexture spFontTexture, hj_u32 u32BlockWidth, hj_u32 u32CharaNum, hj_u32 u32BlockSizeX, hj_u32 u32BlockSizeY)
{
	m_u32CharaNum = u32CharaNum;
	m_u32SizeX = u32BlockSizeX;
	m_u32SizeY = u32BlockSizeY;
	m_ppcSprite = NEW clsHJSprite*[m_u32CharaNum];
	hj_u32 u32Width = spFontTexture->GetWidth();
	hj_u32 u32Height = spFontTexture->GetHeight();
	hj_f32 f32ParamX = static_cast<hj_f32>(m_u32SizeX) / static_cast<hj_f32>(u32Width);
	hj_f32 f32ParamY = static_cast<hj_f32>(m_u32SizeY) / static_cast<hj_f32>(u32Height);
	hj_u32 u32X = 0;
	hj_u32 u32Y = 0;
	clsHJSprite* pcSprite=NULL;
	for(hj_u32 u32i=0; u32i<m_u32CharaNum; ++u32i)
	{
		pcSprite = NEW clsPfSprite();
		pcSprite->SetTexture( spFontTexture );
		u32X = u32i % u32BlockWidth;
		u32Y = u32i / u32BlockWidth;
		pcSprite->SetTextureUv( f32ParamX*u32X, f32ParamY*u32Y, f32ParamX*(u32X+1), f32ParamY*(u32Y+1) );
		m_ppcSprite[u32i] = pcSprite;
	}
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テキスト描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::DrawText(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum)
{
	_DrawText(opcText, f32PosX, f32PosY, u32DrawNum, m_c32Color);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テキスト描画(センタリング)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::DrawTextCenter(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum)
{
	_DrawTextCenter(opcText, f32PosX, f32PosY, u32DrawNum, m_c32Color);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	枠テキスト描画
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::DrawTextFrame(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum)
{
	hj_f32 f32Offset = m_f32Scale * 1.f * m_f32FrameOffset;
	_DrawText(opcText, f32PosX+f32Offset,	f32PosY,			u32DrawNum, m_c32ColorFrame);
	_DrawText(opcText, f32PosX-f32Offset,	f32PosY,			u32DrawNum, m_c32ColorFrame);
	_DrawText(opcText, f32PosX,				f32PosY+f32Offset,	u32DrawNum, m_c32ColorFrame);
	_DrawText(opcText, f32PosX,				f32PosY-f32Offset,	u32DrawNum, m_c32ColorFrame);

	// 四つ角
	hj_f32 f32TiltOffset = m_f32Scale * 0.70710678f * m_f32FrameOffset;
	_DrawText(opcText, f32PosX+f32TiltOffset,	f32PosY+f32TiltOffset,	u32DrawNum, m_c32ColorFrame);
	_DrawText(opcText, f32PosX+f32TiltOffset,	f32PosY-f32TiltOffset,	u32DrawNum, m_c32ColorFrame);
	_DrawText(opcText, f32PosX-f32TiltOffset,	f32PosY+f32TiltOffset,	u32DrawNum, m_c32ColorFrame);
	_DrawText(opcText, f32PosX-f32TiltOffset,	f32PosY-f32TiltOffset,	u32DrawNum, m_c32ColorFrame);

	_DrawText(opcText, f32PosX, f32PosY, u32DrawNum, m_c32Color);
}
//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	枠テキスト描画(センタリング)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::DrawTextCenterFrame(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum)
{
	hj_f32 f32Offset = m_f32Scale * 1.f * m_f32FrameOffset;
	_DrawTextCenter(opcText, f32PosX+f32Offset,	f32PosY,			u32DrawNum, m_c32ColorFrame);
	_DrawTextCenter(opcText, f32PosX-f32Offset,	f32PosY,			u32DrawNum, m_c32ColorFrame);
	_DrawTextCenter(opcText, f32PosX,			f32PosY+f32Offset,	u32DrawNum, m_c32ColorFrame);
	_DrawTextCenter(opcText, f32PosX,			f32PosY-f32Offset,	u32DrawNum, m_c32ColorFrame);

	// 四つ角
	
	hj_f32 f32TiltOffset = m_f32Scale * 0.70710678f * m_f32FrameOffset;
	_DrawTextCenter(opcText, f32PosX+f32TiltOffset,	f32PosY+f32TiltOffset,	u32DrawNum, m_c32ColorFrame);
	_DrawTextCenter(opcText, f32PosX+f32TiltOffset,	f32PosY-f32TiltOffset,	u32DrawNum, m_c32ColorFrame);
	_DrawTextCenter(opcText, f32PosX-f32TiltOffset,	f32PosY+f32TiltOffset,	u32DrawNum, m_c32ColorFrame);
	_DrawTextCenter(opcText, f32PosX-f32TiltOffset,	f32PosY-f32TiltOffset,	u32DrawNum, m_c32ColorFrame);

	_DrawTextCenter(opcText, f32PosX, f32PosY, u32DrawNum, m_c32Color);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	内部用テキスト描画(直座標)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::_DrawText(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum, hj_color32 c32Color)
{
	DrawTextOrig(NULL, opcText, f32PosX, f32PosY, u32DrawNum, c32Color);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	内部用テキスト描画(直座標センタリング)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::_DrawTextCenter(const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum, hj_color32 c32Color)
{
	stcDrawArea sDrawArea = {0.f, 0.f};
	hj_f32 f32Space = (m_u32SizeX + m_f32Space) * m_f32Scale;
	DrawTextOrig(&sDrawArea, opcText, f32PosX, f32PosY, DRAW_MAXNUM);	// サイズを取るときは描画最大数
	DrawTextOrig(NULL, opcText, f32PosX - ((sDrawArea.f32SizeW-m_u32SizeX)*0.5f), f32PosY - ((sDrawArea.f32SizeH-m_u32SizeY)*0.5f), u32DrawNum, c32Color);
}

//┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏ 
/*!	@brief	テキスト描画(オリジナル)
*///┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏┏
void clsHJAsciiFont::DrawTextOrig(stcDrawArea* psOutDrawArea, const hj_c8* opcText, hj_f32 f32PosX, hj_f32 f32PosY, hj_u32 u32DrawNum, hj_color32 c32Color)
{
	clsHJDraw2d& rcDraw2d = *clsHJCoreMgr::GS()->GetDraw2d();
	const hj_c8* opc8Chara = opcText;
	hj_bool bBegin = !rcDraw2d.IsBegin() && psOutDrawArea==NULL;	// 外部ですでに開始されている可能性があるのでチェック
	
	hj_f32 f32SpaceX = (m_u32SizeX + m_f32Space) * m_f32Scale;
	hj_f32 f32SpaceY = (m_u32SizeY + m_f32Space) * m_f32Scale;

	hj_u32 u32Strlen = String::Strlen( opcText );
	hj_u32 u32FontDrawNum = u32Strlen;
	if( u32DrawNum <= u32Strlen ){
		u32FontDrawNum = u32DrawNum;
	}
	
	// 高速化の為begin~endはここで行う
	if(bBegin) rcDraw2d.Begin();

	clsHJSprite* pcSprite = NULL;
	if(psOutDrawArea==NULL){
		hj_f32 f32PosNowX = f32PosX;
		hj_f32 f32PosNowY = f32PosY;

		for(hj_u32 u32i=0; u32i<u32FontDrawNum; ++u32i, ++opc8Chara){
			switch(*opc8Chara){
				case ' ':{
					f32PosNowX += f32SpaceX;
				}break;
				case '\t':{
				}break;
				case '\n':{
					f32PosNowX = f32PosX;
					f32PosNowY -= f32SpaceY;
				}break;
				default:{
					pcSprite = m_ppcSprite[*opc8Chara-' '];
					pcSprite->SetPosition(f32PosNowX, f32PosNowY);
					pcSprite->SetColor(c32Color);
					pcSprite->SetScale(m_f32Scale);
					pcSprite->Render();
					f32PosNowX += f32SpaceX;
				}break;
			};
		}
	// 描画なし
	}else{
		hj_f32 f32TmpW = 0.f;
		hj_f32 f32TmpH = 0.f;
		psOutDrawArea->f32SizeW = 0.f;
		psOutDrawArea->f32SizeH = 0.f;
		for(hj_u32 u32i=0; u32i<u32FontDrawNum; ++u32i, ++opc8Chara){
			switch(*opc8Chara){
				case ' ':{
					f32TmpW += f32SpaceX;
				}break;
				case '\t':{
				}break;
				case '\n':{
					f32TmpW = 0;
					f32TmpH += f32SpaceY;
				}break;
				default:{
					f32TmpW += f32SpaceX;
				}break;
			};
			// 最大サイズを超えた場合に更新
			if( f32TmpW > psOutDrawArea->f32SizeW) psOutDrawArea->f32SizeW = f32TmpW;
			if( f32TmpH > psOutDrawArea->f32SizeH) psOutDrawArea->f32SizeH = f32TmpH;
		}
	}
	if(bBegin) rcDraw2d.End();
}


} //namespace hj