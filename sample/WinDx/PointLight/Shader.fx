// Variables
float4x4 worldMtx		: WORLD;		// W
float4x4 viewMtx 		: VIEW;			// V
float4x4 projectionMtx  : PROJECTION;	// P
float4x4 wvpMtx;						// WVP�}�g���N�X

float3 EyePos = float3(0.0f, 0.0f, -1.0f);
float3 LightDir = float3(-1.f, -1.f, -1.f);

float3 PointLightColor = float3(1.f, 0.5f, 0.3f);
float3 PointLightPos = float3(1.f, 1.f, 1.f);
float PointLightLength = 5.f;
float PointLightPower = 0.5f;

float DirLight = 0.3f;			// ���s����(����)
float Ambient  = 0.3f;			// �A���r�G���g
float Specular = 0.3f;			// �X�y�L�����[�͈̔�
float SpecularPower = 10.0f;	// �X�y�L�����[�̋��x shininess

texture TextureDiffuse;

// Sampler
sampler samplerDiffuse = sampler_state{
	Texture = TextureDiffuse;
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
	float3 PosTex: TEXCOORD4;
};

VS_OUTPUT VS( 
	float4 Pos		: POSITION,
	float3 Normal	: NORMAL,
	float2 Tex 		: TEXCOORD0,
	float4 Color 	: COLOR0
){
	VS_OUTPUT Out;

	Out.Pos = mul( Pos, wvpMtx );
	Out.PosTex = mul( Pos, worldMtx);
	Out.N = normalize( mul( Normal, wvpMtx) );
	Out.Tex = Tex;

	//���C�g�����œ��͂����̂ŁA���_ -> ���C�g�ʒu�Ƃ��邽�߂ɋt�����ɕϊ�����B�Ȃ��A�v���P�[�V�����ŕK�����K�����邱��
	Out.L = normalize( -LightDir.xyz );

	// Diffuse x Ambiend
	float tone = min( max( Ambient, dot( Out.N, -Out.L ) * DirLight ), 1.0f );
	Out.Color = float4( tone, tone, tone, 1.f) * Color;
	
	//���_ -> ���_ �ւ̃x�N�g�����v�Z
	Out.E = EyePos.xyz - Out.Pos.xyz;

	return Out;
}

// PixelShader
float4 PS( VS_OUTPUT In ) : COLOR0
{
	float4 Out;
	float3 normal = normalize( In.N );
	float3 light_dir = normalize( In.L );
	float3 E = normalize( In.E );
	float3 ref;
	float S;

	// ���s����(���_�V�F�[�_�[�Ōv�Z�ς�)
	Out = In.Color * tex2D( samplerDiffuse, In.Tex );

	// ���s�����ւ̃X�y�L����
	ref = reflect( light_dir, normal );
	S = pow( min( max( dot(E, -ref), 0.0), 1.0 ), SpecularPower) * Specular;
	Out.xyz += float4(S,S,S,1.0);

	// �|�C���g���C�g
	float point_light = min( length( PointLightPos - In.PosTex ), PointLightLength ) / PointLightLength;
	Out.xyz += (1.0 - pow( max( min( point_light, 1.0), 0.0), PointLightPower )) * PointLightColor;

	// �|�C���g���C�g�̃X�y�L����
	float3 pl_light_dir = normalize( In.PosTex - PointLightPos );
	ref = reflect( pl_light_dir, normal );
	S = pow( min( max( dot(E, -ref), 0.0), 1.0 ), SpecularPower) * Specular;
	Out.xyz += float4(S,S,S,1.0);

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
