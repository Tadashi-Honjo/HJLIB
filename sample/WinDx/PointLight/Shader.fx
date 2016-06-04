// Variables
float4x4 worldMtx		: WORLD;		// W
float4x4 viewMtx 		: VIEW;			// V
float4x4 projectionMtx  : PROJECTION;	// P
float4x4 wvpMtx;						// WVPマトリクス

float3 EyePos = float3(0.0f, 0.0f, -1.0f);
float3 LightDir = float3(-1.f, -1.f, -1.f);

float3 PointLightColor = float3(1.f, 0.5f, 0.3f);
float3 PointLightPos = float3(1.f, 1.f, 1.f);
float PointLightLength = 5.f;
float PointLightPower = 0.5f;

float DirLight = 0.3f;			// 平行光源(強さ)
float Ambient  = 0.3f;			// アンビエント
float Specular = 0.3f;			// スペキュラーの範囲
float SpecularPower = 10.0f;	// スペキュラーの強度 shininess

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
	float3 N	 : TEXCOORD1;   //オブジェクトの法線ベクトル
	float3 L	 : TEXCOORD2;   //頂点 -> ライト位置 ベクトル
	float3 E	 : TEXCOORD3;   //頂点 -> 視点 ベクトル
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

	//ライト方向で入力されるので、頂点 -> ライト位置とするために逆向きに変換する。なおアプリケーションで必ず正規化すること
	Out.L = normalize( -LightDir.xyz );

	// Diffuse x Ambiend
	float tone = min( max( Ambient, dot( Out.N, -Out.L ) * DirLight ), 1.0f );
	Out.Color = float4( tone, tone, tone, 1.f) * Color;
	
	//頂点 -> 視点 へのベクトルを計算
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

	// 平行光源(頂点シェーダーで計算済み)
	Out = In.Color * tex2D( samplerDiffuse, In.Tex );

	// 平行光源へのスペキュラ
	ref = reflect( light_dir, normal );
	S = pow( min( max( dot(E, -ref), 0.0), 1.0 ), SpecularPower) * Specular;
	Out.xyz += float4(S,S,S,1.0);

	// ポイントライト
	float point_light = min( length( PointLightPos - In.PosTex ), PointLightLength ) / PointLightLength;
	Out.xyz += (1.0 - pow( max( min( point_light, 1.0), 0.0), PointLightPower )) * PointLightColor;

	// ポイントライトのスペキュラ
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
