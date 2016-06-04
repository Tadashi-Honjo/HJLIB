// bin2Tbl.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

//�{�� UNICODE���ƃR���\�[���ɏo�͂ł��Ȃ��̂�
hj_c8 atcHelp[]={
"\
//--------------------------------------------------------------\n\
/*!	@file\n\
	@brief	bin2Tbl\n\
\n\
	@author �{�� ���u\n\
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
		fpRead = _tfopen(argv[s32i], HJ_T("rb") ); //�t�@�C�����J��

		if(fpRead==NULL)
			continue;

		//�{�� �o�͖�
		_tcscpy( atcFileName, argv[s32i]);
		ptcSplit = _tcsrchr(atcFileName, HJ_T('.'));
		
		if(ptcSplit==NULL){
			_tcscat(atcFileName, OUT_EXT);
		}else{
			*ptcSplit = HJ_T('\0');
			_tcscat(atcFileName, OUT_EXT);
		}
		
		//�{�� �o��
		fpWrite = _tfopen(atcFileName, HJ_T("w") ); //�t�@�C�����J��

		//�{�� �o�C�i����������
		hj_u8 u8DataBuf = 0;
		hj_bool bBegin=true;
		while(fread( &u8DataBuf, sizeof(hj_u8), 1, fpRead) > 0){
			if(bBegin){
				_ftprintf(fpWrite, _T("0x%02x"), u8DataBuf);
				bBegin = false;
			}else{
				_ftprintf(fpWrite, _T(",0x%02x"), u8DataBuf);
			}
			//�{�� ���s
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
		printf("�Ȃ񂩓��͂���");
		getch();
	}
	return 0;
}