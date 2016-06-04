//--------------------------------------------------------------
/*!	@file
	@brief	packer

	@author �{�� ���u
	@date	13/12/18
*/
//	Copyright (C) 2013 TadashiHonjo. All Rights Reserved. 
//--------------------------------------------------------------
#include "stdafx.h"

namespace{
	// ������ String �� nShift �����ړ�����B
	// �ړ���ւ̃|�C���^��Ԃ��B
	hj_tchar *StrShift( hj_tchar *String, size_t nShift )
	{
		hj_tchar *start = String;
		hj_tchar *stop  = String + _tcslen( String );
		memmove( start + nShift, start, stop-start+sizeof(hj_tchar) );
		return String + nShift;
	}//StrShift

	// ������ String �̕����� From �𕶎��� To �Œu������B
	// �u����̕����� String �̃T�C�Y�� String �̋L���̈�𒴂���ꍇ�̓���͖���`�B
	hj_tchar *StrReplace( hj_tchar *String, const hj_tchar *From, const hj_tchar *To )
	{
		hj_s32 nToLen;   // �u�����镶����̒���
		hj_s32 nFromLen; // �������镶����̒���
		hj_s32 nShift;
		hj_tchar *start;    // �������J�n����ʒu
		hj_tchar *stop;     // ������ String �̏I�[
		hj_tchar *p;

		nToLen   = _tcslen( To );
		nFromLen = _tcslen( From );
		nShift   = nToLen - nFromLen;
		start    = String;
		stop     = String + _tcslen( String );

		// ������ String �̐擪���當���� From ������
		while( NULL != ( p = _tcsstr( start, From ) ) )
		{
			// ������ To �����ʂł���悤�ɂ���
			start = StrShift( p + nFromLen, nShift );
			stop  = stop + nShift;

			// ������ To �𕡎�
			memmove( p, To, nToLen );
		}

		return String;
	}//StrReplace
}


//�{�� UNICODE���ƃR���\�[���ɏo�͂ł��Ȃ��̂�
hj_c8 atcHelp[]={
"\
//--------------------------------------------------------------\n\
  packer\n\
\n\
  author �{�� ���u\n\
  date	 13/12/18\n\
\n\
  Copyright (C) 2013 TadashiHonjo. All Rights Reserved. \n\
//--------------------------------------------------------------\n\
arg1 �E�E�E Pack Name\n\
arg2 �E�E�E Pack Directory Name\n\
-h �E�E�E�@help\n\
-b �E�E�E�@big endian\n\
-a �E�E�E�@alighnment(default=4)\n\
"};

enum enmReadMode{
	READ_MODE_MAIN,
	READ_MODE_ALIGNMENT,
};

static const hj_u32 tou32PackFileMaxNum = 256;
struct stcPackHeader{
	hj_c8		header[4];						// 'HJPK'
	hj_u32		u32FileNum;						// FileNum
	hj_size_t	aOffset[tou32PackFileMaxNum];	// Offsets
	hj_size_t	aSize[tou32PackFileMaxNum];		// Sizes
};

int _tmain(int argc, _TCHAR* argv[])
{
	hj_tchar atcDirectoryName[MAX_PATH]={HJ_T("")};
	hj_tchar atcPackName[MAX_PATH]={HJ_T("")};
	hj_tchar atcPackNameArchive[MAX_PATH]={HJ_T("")};
	hj_tchar atcPackNameHeader[MAX_PATH]={HJ_T("")};

	hj_tchar* ptcSplit = NULL;
	FILE *fpRead;
	FILE *fpWrite;
	hj_s32 s32i;
	hj_s32 s32EnterCnt=0;
	hj_bool bHelp=false;
	hj_bool bBigEndian=false;
	hj_u32	alighment = 4;
	enmReadMode eReadMode = READ_MODE_MAIN;

#if defined(_DEBUG)
	// �Œ�Ńp�b�N������1�����A�p�b�N����f�B���N�g������2����
	_tcscpy_s(atcPackName, HJ_T("test_pack"));
//	_tcscpy_s(atcDirectoryName, HJ_T("B:\\project\\HJLIB\\tool\\packer\\test_dir"));
	_tcscpy_s(atcDirectoryName, HJ_T("B:\\project\\TSG\\resource\\work"));
	
#else
	// ����������Ȃ�
	if(argc < 3){
		printf(atcHelp);
		getch();
		return 1;
	}
	// �Œ�Ńp�b�N������1�����A�p�b�N����f�B���N�g������2����
	_tcscpy_s(atcPackName, argv[1]);
	_tcscpy_s(atcDirectoryName, argv[2]);
#endif
	_tcscpy_s(atcPackNameArchive, atcPackName);
	_tcscpy_s(atcPackNameHeader, atcPackName);
	_tcscat_s(atcPackNameArchive, HJ_T(".pk"));
	_tcscat_s(atcPackNameHeader, HJ_T(".pkh"));
	_tcscat_s(atcDirectoryName, HJ_T("\\"));

//	for(s32i=1; s32i<argc; ++s32i){
	for(s32i=3; s32i<argc; ++s32i){
		switch(eReadMode){
			case READ_MODE_MAIN:{
				// options
				if(argv[s32i][0] == '-'){
					switch(argv[s32i][1]){
						// help
						case 'h':{
							bHelp = true;
						}break;
						// big endian
						case 'b':{
							bBigEndian = true;
						}break;
						// alignment
						case 'a':{
							eReadMode = READ_MODE_ALIGNMENT;
						}break;
					};
				}
			}break;
			case READ_MODE_ALIGNMENT:{
				alighment = _ttoi(argv[s32i]);
				eReadMode = READ_MODE_MAIN;
			}break;
		};
	}

	// print help
	if(bHelp){
		printf(atcHelp);
	}

	stcPackHeader sPackHeader={};
	sPackHeader.header[0] = 'H';
	sPackHeader.header[1] = 'J';
	sPackHeader.header[2] = 'P';
	sPackHeader.header[3] = 'K';

	// �S�Ẵt�@�C����񋓂���
	HANDLE hSearch;
	WIN32_FIND_DATA fd;
	hj_bool bError = false;
	hj_size_t size_tmp;
	hj_u32 u32FileCnt = 0;
	hj_size_t sizeFileCnt = sizeof(stcPackHeader);
	hj_tchar atcFindDirectoryName[MAX_PATH]={HJ_T("")};

	_tcscpy_s( atcFindDirectoryName, atcDirectoryName );
	_tcscat_s(atcFindDirectoryName, HJ_T("*.*"));
	hSearch = FindFirstFile( atcFindDirectoryName, &fd );
	hj_tchar PackFilePathList[tou32PackFileMaxNum][MAX_PATH+10]={0};
	hj_tchar PackFileNameList[tou32PackFileMaxNum][MAX_PATH+10]={0};

	if( hSearch != INVALID_HANDLE_VALUE ){
		while( true )
		{
			if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && _tcscmp(fd.cFileName,HJ_T(".."))!=0 && _tcscmp(fd.cFileName,HJ_T("."))!=0)
			{
			   //subpath�Ńf�B���N�g���̃t���p�X��������
//				wsprintf(subpath,"%s%s\\*",temp,lp.cFileName);
//				Directory(subpath);
			}
			if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY){
				// copy file name
				_tcscpy_s( PackFilePathList[u32FileCnt], atcDirectoryName );
				_tcscat_s( PackFilePathList[u32FileCnt], fd.cFileName );
				_tcscpy_s( PackFileNameList[u32FileCnt], fd.cFileName );

				// get file size
				size_tmp = fd.nFileSizeHigh*MAXDWORD+fd.nFileSizeLow;
				sPackHeader.aOffset[u32FileCnt] = sizeFileCnt;
				sPackHeader.aSize[u32FileCnt] = size_tmp;
				sizeFileCnt += size_tmp;
				++u32FileCnt;

				// max pack file over
				if(u32FileCnt > tou32PackFileMaxNum){
					bError = true;
					break;
				}
			}
			
			if( !FindNextFile( hSearch, &fd ) ){
				if( GetLastError() == ERROR_NO_MORE_FILES ){
					break;
				}else{
					bError = true;
					break;
				}
			}
		}

		FindClose( hSearch );
	}

	if(!bError){
		sPackHeader.u32FileNum = u32FileCnt;

		do{
			// --- Archive List Header
			{
				fpWrite = NULL;
				if(_tfopen_s(&fpWrite, atcPackNameHeader, HJ_T("w")) != 0 ) break;
				if(fpWrite==NULL) break;

				fwprintf(fpWrite, HJ_T("// %s\n"), atcPackNameHeader );
				fwprintf(fpWrite, HJ_T("enum {\n"));
				hj_tchar atcString[MAX_PATH]={HJ_T("")};
				hj_tchar *p=NULL;

				for(hj_u32 u32i=0; u32i<sPackHeader.u32FileNum; ++u32i){
					_tcscpy_s( atcString, atcPackName );
					_tcscat_s( atcString, HJ_T("_"));
					_tcscat_s( atcString, PackFileNameList[u32i]);
//					StrReplace( atcString, HJ_T("."), HJ_T("\0") );
					// �g���q�͊܂܂Ȃ�
					if((p=_tcsstr(atcString, HJ_T("."))) != NULL ){
						*p = HJ_T('\0');
					}

					fwprintf(fpWrite, HJ_T("\t%s\t\t\t= %d,\n"), atcString, u32i);
				}
				fwprintf(fpWrite, HJ_T("};\n"));

				fclose(fpWrite);
			}

			// --- Data Archive
			fpWrite = NULL;
			if(_tfopen_s(&fpWrite, atcPackNameArchive, HJ_T("w+b")) != 0 ) break;
			if(fpWrite==NULL) break;

			// header
			fwrite(&sPackHeader, sizeof(sPackHeader), 1, fpWrite);

			// files
			hj_u8* au8Buff = NULL;
			hj_size_t size_tmp = 0;
			for(hj_u32 u32i=0; u32i<sPackHeader.u32FileNum; ++u32i){
				fpRead = NULL;
				if(_tfopen_s(&fpRead, PackFilePathList[u32i], HJ_T("rb")) != 0 ) break;
				if(fpRead==NULL) break;

				if(au8Buff!=NULL){
					delete[] au8Buff;
					au8Buff = NULL;
				}
				size_tmp = sPackHeader.aSize[u32i];
				au8Buff = new hj_u8[size_tmp];
				fread(au8Buff, size_tmp, 1, fpRead);
				fclose(fpRead);

				// write file
				fwrite(au8Buff, size_tmp, 1, fpWrite);
			}
			if(au8Buff!=NULL){
				delete[] au8Buff;
				au8Buff = NULL;
			}

			fclose(fpWrite);

		}while(false);
	}

	return 0;
}