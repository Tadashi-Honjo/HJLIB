//--------------------------------------------------------------
/*!	@file
	@brief	colladaConv

	@author 本城 直志
	@date	14/01/04
*/
//	Copyright (C) 2014 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#include "stdafx.h"
#include "Exception.h"
#include "ColladaLoader.h"
#include "MeshData.h"
#include "MeshDataCreater.h"
#include "MeshPlatform.h"

namespace{

//本城 UNICODEだとコンソールに出力できないので
hj_c8 atcHelp[]={
"\
//--------------------------------------------------------------\n\
colladaConv\n\
\n\
  author 本城 直志\n\
  date	 14/01/04\n\
\n\
  Copyright (C) 2014 TadashiHonjo. All Rights Reserved. \n\
//--------------------------------------------------------------\n\
<param>\n\
arg1 ・・・ 変換ファイルパス\n\
-h ・・・　help\n\
-p ・・・　platform\n\
\n\
\n\
＜platform＞\n\
WINDX・・・Windows DirectX\n\
"};

//! 読み込みモード
enum enmReadMode{
	READ_MODE_MAIN,
	READ_MODE_PLATFORM,
};

//! 入力データタイプ
enum enmDataType{
	DATATYPE_COLLADA,
	DATATYPE_MAXNUM,
};

//! 入力データタイプごとのデータ
struct stcDataTypeData{
	hj_tchar atcMatchExtName[16]; // 入力ファイルチェック用の拡張子
};

const stcDataTypeData atosDataTypeData[DATATYPE_MAXNUM] = {
	// DATATYPE_COLLADA
	{
		HJ_T(".dae"),
	},
};


//typedef stcFormatWinDx_PNCT stcOutVertexFormat;

};


// ============== main
int _tmain(int argc, _TCHAR* argv[])
{
	hj_bool bHelp = false;
	hj_size_t sizeTmp = 0;
	hj_tchar atcConvPathName[MAX_PATH]={HJ_T("")};
	hj_tchar atcOutPathName[MAX_PATH+4]={HJ_T("")}; // 拡張子で若干伸びるので増やしとく
	enmReadMode eReadMode = READ_MODE_MAIN;
	enmDataType eDataType = DATATYPE_COLLADA;
	enmPlatform ePlatform = PLATFORM_MAXNUM;

	// 引数が足りない
	if(argc == 1){
		printf(atcHelp);
		_getch();
		return 1;
	}
	// 第1引数が変換ファイル名
	_tcscpy_s(atcConvPathName, argv[1]);

	for(hj_s32 s32i=2; s32i<argc; ++s32i){
		switch(eReadMode){
			case READ_MODE_MAIN:{
				// options
				if(argv[s32i][0] == '-'){
					switch(argv[s32i][1]){
						// help
						case 'h':{
							bHelp = true;
						}break;
						// platform
						case 'p':{
							eReadMode = READ_MODE_PLATFORM;
						}break;
					};
				}
			}break;
			case READ_MODE_PLATFORM:{
				ePlatform = convPlatform(argv[s32i]);
				eReadMode = READ_MODE_MAIN;
			}break;
		};
	}

	// print help
	if(bHelp){
		printf(atcHelp);
	}

	// model
	if(ePlatform == PLATFORM_MAXNUM){
		printf("プラットフォームが指定されていません。");
		_getch();
		return 1;
	}

	// データタイプチェック
	const stcDataTypeData* opsDataTypeData = &atosDataTypeData[eDataType];
	hj_tchar* ptcFindStr = NULL;
	ptcFindStr = _tcsstr(atcConvPathName, opsDataTypeData->atcMatchExtName);

	// 拡張子がない!?
	if(ptcFindStr == NULL){
		printf("入力ファイルと変換するデータ形式が正しくありません。");
		_getch();
		return 1;
	}

	// 出力ファイル名
	_tcscpy_s(atcOutPathName, atcConvPathName);
	hj_tchar *ptcFileEx = _tcsstr(atcOutPathName, HJ_T("."));
	sizeTmp = HJ_ARRAY_NUM(atcOutPathName) - (reinterpret_cast<hj_u8*>(ptcFileEx) - reinterpret_cast<hj_u8*>(&atcOutPathName))/sizeof(hj_tchar);
	_tcscpy_s(ptcFileEx, sizeTmp, HJ_T(".hjmd"));

	// ローダー
	CMeshDataCreater* pcMeshDataCreater = NEW CMeshDataCreater();
	CMeshData* pcMeshData = NULL;


	hj_s32 s32Result = 0;

	do{
		// ロード
		try
		{
			pcMeshDataCreater->LoadMesh( atcConvPathName, &pcMeshData );
		}
		catch( CException ex )
		{
			printf(ex.m_pErrorStr);
			_getch();
			s32Result = 1;
			break;
		}

		// 出力
		if(!outputFilePlatform(ePlatform, atcOutPathName, pcMeshData)){
			s32Result = 1;
			break;
		}
	}while(false);

	HJ_SAFE_DELETE(pcMeshData);
	HJ_SAFE_DELETE(pcMeshDataCreater);

	return 0;
}