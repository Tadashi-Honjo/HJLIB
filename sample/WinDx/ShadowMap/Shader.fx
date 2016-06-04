// Variables
float4x4 worldMtx		: WORLD;		// W
float4x4 viewMtx 		: VIEW;			// V
float4x4 projectionMtx  : PROJECTION;	// P
float4x4 wvpMtx;						// WVPマトリクス
float4x4 shadowMtx;						// shadowマトリクス

texture TextureDiffuse;
texture TextureShadowMap;

// Sampler
sampler samplerDiffuse = sampler_state{
	Texture = TextureDiffuse;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler samplerShadowMap = sampler_state{
	Texture = TextureShadowMap;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

// VertexShader
struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float2 Tex			: TEXCOORD0;
	float4 Color		: COLOR0;
	float4 ShadowMap	: TEXCOORD1;
};

VS_OUTPUT VS( 
	float4 Pos		: POSITION,
	float2 Tex 		: TEXCOORD0,
	float4 Color 	: COLOR0
){
	VS_OUTPUT Out;

	Out.Pos = mul( Pos, wvpMtx );
	Out.Tex = Tex;
	Out.Color = Color;
	Out.ShadowMap = mul( Pos, worldMtx );
	Out.ShadowMap = mul( Out.ShadowMap, shadowMtx );
	return Out;
}

// PixelShader
float4 PS( VS_OUTPUT In ) : COLOR0
{
	float4 color;
	color = tex2D( samplerDiffuse, In.Tex );

    //シャドウマップ
	float w = 1.0f / In.ShadowMap.w;
    float2 tex;
    tex.x = ( 1.0f + In.ShadowMap.x * w ) * 0.5f;
    tex.y = ( 1.0f - In.ShadowMap.y * w ) * 0.5f;
    float z = tex2D( samplerShadowMap, tex ).x;
    if( In.ShadowMap.z * w > z + 0.005f )
        color.xyz = color.xyz * 0.5f;

	return color;
}

// Technique
technique TShader
{
   pass P0
   {
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS();
   }
}



// ===== ShadowMap
// VertexShader
struct SHADOW_MAP_VS_OUTPUT
{
	float4 Pos   : POSITION;
	float4 Shadow: TEXCOORD0;
	float4 Color : COLOR0;
};

SHADOW_MAP_VS_OUTPUT ShadowMapVS( 
	float4 Pos		: POSITION,
	float4 Color 	: COLOR0
){
	SHADOW_MAP_VS_OUTPUT Out;
	Out.Pos = mul( Pos, wvpMtx );
	Out.Shadow = Out.Pos;
	Out.Color = Color;
	return Out;
}

// PixelShader
float4 ShadowMapPS( SHADOW_MAP_VS_OUTPUT In ) : COLOR0
{
	float4 color = (In.Shadow.z / In.Shadow.w);
//	float4 color = float4(In.Shadow.z / In.Shadow.w, 0.f, 0.f, 1.f);
//	float4 color = In.Shadow.z / 50.0;
//	float4 color = 0.5;
	color.a = 1.0;
	return color;
}

// Technique
technique TShadowMap
{
   pass P0
   {
		VertexShader = compile vs_2_0 ShadowMapVS();
		PixelShader  = compile ps_2_0 ShadowMapPS();
   }
}
