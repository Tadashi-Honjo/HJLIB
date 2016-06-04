//--------------------------------------------------------------
/*!	@file
	@brief	colladaConv

	@author �{�� ���u
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

//�{�� UNICODE���ƃR���\�[���ɏo�͂ł��Ȃ��̂�
hj_c8 atcHelp[]={
"\
//--------------------------------------------------------------\n\
colladaConv\n\
\n\
  author �{�� ���u\n\
  date	 14/01/04\n\
\n\
  Copyright (C) 2014 TadashiHonjo. All Rights Reserved. \n\
//--------------------------------------------------------------\n\
<param>\n\
arg1 �E�E�E �ϊ��t�@�C���p�X\n\
-h �E�E�E�@help\n\
-p �E�E�E�@platform\n\
\n\
\n\
��platform��\n\
WINDX�E�E�EWindows DirectX\n\
"};

//! �ǂݍ��݃��[�h
enum enmReadMode{
	READ_MODE_MAIN,
	READ_MODE_PLATFORM,
};

//! ���̓f�[�^�^�C�v
enum enmDataType{
	DATATYPE_COLLADA,
	DATATYPE_MAXNUM,
};

//! ���̓f�[�^�^�C�v���Ƃ̃f�[�^
struct stcDataTypeData{
	hj_tchar atcMatchExtName[16]; // ���̓t�@�C���`�F�b�N�p�̊g���q
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
	hj_tchar atcOutPathName[MAX_PATH+4]={HJ_T("")}; // �g���q�Ŏ኱�L�т�̂ő��₵�Ƃ�
	enmReadMode eReadMode = READ_MODE_MAIN;
	enmDataType eDataType = DATATYPE_COLLADA;
	enmPlatform ePlatform = PLATFORM_MAXNUM;

	// ����������Ȃ�
	if(argc == 1){
		printf(atcHelp);
		_getch();
		return 1;
	}
	// ��1�������ϊ��t�@�C����
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
		printf("�v���b�g�t�H�[�����w�肳��Ă��܂���B");
		_getch();
		return 1;
	}

	// �f�[�^�^�C�v�`�F�b�N
	const stcDataTypeData* opsDataTypeData = &atosDataTypeData[eDataType];
	hj_tchar* ptcFindStr = NULL;
	ptcFindStr = _tcsstr(atcConvPathName, opsDataTypeData->atcMatchExtName);

	// �g���q���Ȃ�!?
	if(ptcFindStr == NULL){
		printf("���̓t�@�C���ƕϊ�����f�[�^�`��������������܂���B");
		_getch();
		return 1;
	}

	// �o�̓t�@�C����
	_tcscpy_s(atcOutPathName, atcConvPathName);
	hj_tchar *ptcFileEx = _tcsstr(atcOutPathName, HJ_T("."));
	sizeTmp = HJ_ARRAY_NUM(atcOutPathName) - (reinterpret_cast<hj_u8*>(ptcFileEx) - reinterpret_cast<hj_u8*>(&atcOutPathName))/sizeof(hj_tchar);
	_tcscpy_s(ptcFileEx, sizeTmp, HJ_T(".hjmd"));

	// ���[�_�[
	CMeshDataCreater* pcMeshDataCreater = NEW CMeshDataCreater();
	CMeshData* pcMeshData = NULL;


	hj_s32 s32Result = 0;

	do{
		// ���[�h
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

		// �o��
		if(!outputFilePlatform(ePlatform, atcOutPathName, pcMeshData)){
			s32Result = 1;
			break;
		}
	}while(false);

	HJ_SAFE_DELETE(pcMeshData);
	HJ_SAFE_DELETE(pcMeshDataCreater);

	return 0;
}