// bin2Tbl.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

//本城 UNICODEだとコンソールに出力できないので
hj_c8 atcHelp[]={
"\
//--------------------------------------------------------------\n\
/*!	@file\n\
	@brief	bin2Tbl\n\
\n\
	@author 本城 直志\n\
	@date	11/11/15\n\
*/\n\
//	Copyright (C) 2011 TadashiHonjo. All Rights Reserved. \n\
//--------------------------------------------------------------\n\
"};

int _tmain(int argc, _TCHAR* argv[])
{
	hj_tchar atcFileName[MAX_PATH]={HJ_T("")};
	hj_tchar* ptcSplit = NULL;
	FILE *fpRead;
	FILE *fpWrite;
	hj_s32 s32i;
	hj_s32 s32EnterCnt=0;

	for(s32i=1; s32i<argc; ++s32i){
		fpRead = _tfopen(argv[s32i], HJ_T("rb") ); //ファイルを開く

		if(fpRead==NULL)
			continue;

		//本城 出力名
		_tcscpy( atcFileName, argv[s32i]);
		ptcSplit = _tcsrchr(atcFileName, HJ_T('.'));
		
		if(ptcSplit==NULL){
			_tcscat(atcFileName, OUT_EXT);
		}else{
			*ptcSplit = HJ_T('\0');
			_tcscat(atcFileName, OUT_EXT);
		}
		
		//本城 出力
		fpWrite = _tfopen(atcFileName, HJ_T("w") ); //ファイルを開く

		//本城 バイナリ書き込み
		hj_u8 u8DataBuf = 0;
		hj_bool bBegin=true;
		while(fread( &u8DataBuf, sizeof(hj_u8), 1, fpRead) > 0){
			if(bBegin){
				_ftprintf(fpWrite, _T("0x%02x"), u8DataBuf);
				bBegin = false;
			}else{
				_ftprintf(fpWrite, _T(",0x%02x"), u8DataBuf);
			}
			//本城 改行
			++s32EnterCnt;
			if(s32EnterCnt >= ENTER_NUM){
				s32EnterCnt = 0;
				_ftprintf(fpWrite, _T("\n"));
			}
		}

		fclose(fpWrite);
		fclose(fpRead);
	}

	if(argc <= 1){
		printf(atcHelp);
		printf("なんか入力して");
		getch();
	}
	return 0;
}