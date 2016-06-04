// Variables
float4x4 worldMtx		: WORLD;		// W
float4x4 viewMtx 		: VIEW;			// V
float4x4 projectionMtx  : PROJECTION;	// P
float4x4 wvpMtx;						// WVPマトリクス

float3 EyePos = float3(0.0f, 0.0f, -1.0f);
float3 LightDir = float3(-1.0f, -1.0f, -1.0f);


float Ambient  = 0.3f;			//アンビエント
float Specular = 0.3f;			//スペキュラーの範囲
float SpecularPower = 10.0f;	//スペキュラーの強度 shininess

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
	float3 N	 : TEXCOORD1;   //オブジェクトの法線ベクトル
	float3 L	 : TEXCOORD2;   //頂点 -> ライト位置 ベクトル
	float3 E	 : TEXCOORD3;   //頂点 -> 視点 ベクトル
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

	//ライト方向で入力されるので、頂点 -> ライト位置とするために逆向きに変換する。なおアプリケーションで必ず正規化すること
	Out.L = normalize( LightDir.xyz );

	// Ambiend
	float A = min( max( Ambient, dot( Out.N, -Out.L ) ), 1.0f );
	Out.Color = float4(A,A,A,1.f) * Color;
	
	//頂点 -> 視点 へのベクトルを計算
	Out.E = EyePos.xyz - Out.Pos.xyz;

	return Out;
}

// PixelShader
float4 PS( VS_OUTPUT In ) : COLOR0
{
	float4 Out;
	//法線ベクトルを正規化する
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
