// Variables
float4x4 worldMtx		: WORLD;		// W
float4x4 viewMtx 		: VIEW;			// V
float4x4 projectionMtx  : PROJECTION;	// P
float4x4 wvpMtx;						// WVP�}�g���N�X

float3 EyePos = float3(0.0f, 0.0f, -1.0f);
float3 LightDir = float3(-1.0f, -1.0f, -1.0f);


float Ambient  = 0.3f;			//�A���r�G���g
float Specular = 0.3f;			//�X�y�L�����[�͈̔�
float SpecularPower = 10.0f;	//�X�y�L�����[�̋��x shininess

texture TextureDiffuse;
texture TextureSpecular;
texture TextureNormal;

// Sampler
/*
sampler texture01 : register(s0) = sampler_state{
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
*/
sampler samplerDiffuse = sampler_state{
	Texture = TextureDiffuse;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler samplerSpecular = sampler_state{
	Texture = TextureSpecular;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler samplerNormal = sampler_state{
	Texture = TextureNormal;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// VertexShader
struct VS_OUTPUT
{
	float4 Pos   : POSITION;
	float2 Tex   : TEXCOORD0;
	float4 Color : COLOR0;
	float3 N	 : TEXCOORD1;   //�I�u�W�F�N�g�̖@���x�N�g��
	float3 L	 : TEXCOORD2;   //���_ -> ���C�g�ʒu �x�N�g��
	float3 E	 : TEXCOORD3;   //���_ -> ���_ �x�N�g��
};

VS_OUTPUT VS( 
	float4 Pos		: POSITION,
	float3 Normal	: NORMAL,
	float2 Tex 		: TEXCOORD0,
	float4 Color 	: COLOR0
){
	VS_OUTPUT Out;

	Out.Pos = mul( Pos, wvpMtx );
	Out.N = mul( Normal, wvpMtx);
	Out.Tex = Tex;

	//���C�g�����œ��͂����̂ŁA���_ -> ���C�g�ʒu�Ƃ��邽�߂ɋt�����ɕϊ�����B�Ȃ��A�v���P�[�V�����ŕK�����K�����邱��
	Out.L = normalize( LightDir.xyz );

	// Ambiend
	float A = min( max( Ambient, dot( Out.N, -Out.L ) ), 1.0f );
	Out.Color = float4(A,A,A,1.f) * Color;
	
	//���_ -> ���_ �ւ̃x�N�g�����v�Z
	Out.E = EyePos.xyz - Out.Pos.xyz;

	return Out;
}

// PixelShader
float4 PS( VS_OUTPUT In ) : COLOR0
{
	float4 Out;
	//�@���x�N�g���𐳋K������
	float3 N = normalize( In.N );
	float3 L = normalize( In.L );
	float3 ref = reflect(L, N);
	float3 E = normalize( In.E );
    float S = pow(min(max(dot(E, -ref), 0.0), 1.0 ), SpecularPower) * Specular;

//	Out = In.Color * tex2D( samplerDiffuse, In.Tex ) + float4(S,S,S,1.0);
	
	Out = In.Color * tex2D( samplerDiffuse, In.Tex ) + (tex2D( samplerSpecular, In.Tex ) * S);

	return Out;
}

// Technique
technique TModelShader
{
   pass P0
   {
		ShadeMode = GOURAUD;
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS();
   }
}
