//�{�� ��{�ϐ�
float4x4 worldMtx		: WORLD;		//�{�� ���[���h�}�g���N�X
float4x4 viewMtx 		: VIEW;			//�{�� �r���[�}�g���N�X
float4x4 projectionMtx  : PROJECTION;	//�{�� �v���W�F�N�V�����}�g���N�X
float4x4 wvpMtx;						//�{�� WVP�}�g���N�X

//float DistortionParam = 0.2f;			//�{�� �c�݌W��

//�{�� �c�݃e�N�X�`��
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

//�{�� ���_�V�F�[�_�[
struct VS_OUTPUT
{
	float4 Pos   : POSITION;
	float2 Tex   : TEXCOORD0;
	float4 Color : COLOR0;
};

VS_OUTPUT VS( float4 Pos : POSITION, float2 Tex : TEXCOORD0, float4 Color : COLOR0){
	VS_OUTPUT Out;
   
	//�{�� ���_
//	Out.Pos = mul(Pos, worldMtx);
//	Out.Pos = mul(Out.Pos, viewMtx);
//	Out.Pos = mul(Out.Pos, projectionMtx);
	Out.Pos = mul(Pos, wvpMtx);
	Out.Tex = Tex;
	Out.Color = Color;

	return Out;
}

//�{�� �s�N�Z���V�F�[�_�[
struct PS_OUTPUT
{
	float2 Tex   : TEXCOORD0;
	float4 Color : COLOR0;
};
float4 PS( PS_OUTPUT In ) : COLOR0
{
	//�{�� �c�݃e�N�X�`���̐F�擾
	float4 Color = tex2D( texture01, In.Tex );
	//�{�� -1.f~1.f���ɕϊ�����
	Color.xy = ( Color.xy - 0.5f ) * 2.0f;
//	return tex2D( textureSceneSampler, In.Tex-(Color.xy) * Color.z * DistortionParam );
	return tex2D( textureSceneSampler, In.Tex-(Color.xy) * Color.z );
}
//�{�� �e�N�j�b�N
technique TShaderDistortion
{
   pass P0
   {
      VertexShader = compile vs_1_1 VS();
      PixelShader  = compile ps_2_0 PS();
   }
}
