//�{�� ��{�ϐ�
float4x4 worldMtx		: WORLD;		//�{�� ���[���h�}�g���N�X
float4x4 viewMtx 		: VIEW;			//�{�� �r���[�}�g���N�X
float4x4 projectionMtx  : PROJECTION;	//�{�� �v���W�F�N�V�����}�g���N�X
float4x4 wvpMtx;						//�{�� WVP�}�g���N�X

//float DistortionParam = 0.2f;			//�{�� �c�݌W��
float3 speed; //�{�� ���x
float blurParam = 0.1f;

//�{�� �V�X�e���p�̃e�N�X�`��
sampler texture01 : register(s0) = sampler_state{
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//�{�� �V�[���`��e�N�X�`��
texture textureScene;
sampler textureSceneSampler = sampler_state{
    Texture   = (textureScene);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//�{�� �V�[���`��e�N�X�`��
texture textureSpeed;
sampler textureSpeedSampler = sampler_state{
    Texture   = (textureSpeed);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// ===================== ���x��������
//�{�� ���_�V�F�[�_�[
struct VS_OUTPUT_SPEED
{
	float4 Pos   : POSITION;
	float2 Tex   : TEXCOORD0;
	float4 Color : COLOR0;
};

VS_OUTPUT_SPEED VS_Speed( float4 Pos : POSITION, float2 Tex : TEXCOORD0, float4 Color : COLOR0){
	VS_OUTPUT_SPEED Out;
	Out.Pos = mul(Pos, wvpMtx);
	Out.Tex = Tex;
	Out.Color = Color;
	return Out;
}

//�{�� �s�N�Z���V�F�[�_�[
float4 PS_Speed( VS_OUTPUT_SPEED In ) : COLOR0
{
	float4 color = tex2D( texture01, In.Tex ) * In.Color;
	return float4(speed.xyz, color.a);	//�{�� ���x���̂܂ܕ`��(�A���t�@�l�͔��f)
}

// ===================== ���x�𔽉f������������
//�{�� ���_�V�F�[�_�[
struct VS_OUTPUT_SPEED_BLUR
{
	float4 Pos   : POSITION;
	float2 Tex   : TEXCOORD0;
	float4 Color : COLOR0;
};

VS_OUTPUT_SPEED_BLUR VS_SpeedBlur( float4 Pos : POSITION, float2 Tex : TEXCOORD0, float4 Color : COLOR0){
	VS_OUTPUT_SPEED_BLUR Out;
	Out.Pos = mul(Pos, wvpMtx);
	Out.Tex = Tex;
	Out.Color = Color;
	return Out;
}

//�{�� �s�N�Z���V�F�[�_�[
float4 PS_SpeedBlur( VS_OUTPUT_SPEED_BLUR In ) : COLOR0
{
	//�{�� ���x�e�N�X�`��
	float4 speedColor = tex2D( textureSpeedSampler, In.Tex );
	//�{�� -1.f~1.f���ɕϊ�����
	float2 speed2 = ( speedColor.xy - 0.5f ) * 2.0f * blurParam;

	//�{�� ���x��������T���v�����O
	float4 Color[10];
	Color[0]  = tex2D( textureSceneSampler, In.Tex						) * 0.19f;
	Color[1]  = tex2D( textureSceneSampler, In.Tex + (speed2)			) * 0.17f;
	Color[2]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 2.0f)	) * 0.15f;
	Color[3]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 3.0f)	) * 0.13f;
	Color[4]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 4.0f)	) * 0.11f;
	Color[5]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 5.0f)	) * 0.09f;
	Color[6]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 6.0f)	) * 0.07f;
	Color[7]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 7.0f)	) * 0.05f;
	Color[8]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 8.0f)	) * 0.03f;
	Color[9]  = tex2D( textureSceneSampler, In.Tex + (speed2 * 9.0f)	) * 0.01f;
	return Color[0] + Color[1] + Color[2] + Color[3] + Color[4] + Color[5] + Color[6] + Color[7] + Color[8] + Color[9];
}



//�{�� ========== �e�N�j�b�N
//�{�� ���x�}�b�v��������
technique TShaderSpeedMap
{
   pass P0
   {
      VertexShader = compile vs_1_1 VS_Speed();
      PixelShader  = compile ps_2_0 PS_Speed();
   }
}

//�{�� ���x�u���[
technique TShaderSpeedBlur
{
   pass P1
   {
      VertexShader = compile vs_1_1 VS_SpeedBlur();
      PixelShader  = compile ps_2_0 PS_SpeedBlur();
   }
}