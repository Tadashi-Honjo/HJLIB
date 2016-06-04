// Variables
float4x4 worldMtx		: WORLD;		// W
float4x4 viewMtx 		: VIEW;			// V
float4x4 projectionMtx  : PROJECTION;	// P
float4x4 wvpMtx;						// WVPマトリクス

static const int SAMPLE_MAXNUM = 4;
static const float offset_x = 1.0f / 1280.f;
static const float offset_y = 1.0f / 1024.f;
static const float EdgeBias = 0.08f;
static const float PI = 3.14159f;
static float2 sampOffset[SAMPLE_MAXNUM] = {
	float2(-offset_x, 0.f),
	float2(-offset_x, offset_y),
	float2(-offset_x, -offset_y),
	float2(0.f, -offset_y),
/*
	float2(-offset_x*2, 0.f),
	float2(-offset_x*2, offset_y*2),
	float2(-offset_x*2, -offset_y*2),
	float2(0.f, -offset_y*2),
*/
};

texture TextureDiffuse;
texture TextureDepth;

// Sampler
sampler samplerDiffuse = sampler_state{
	Texture = TextureDiffuse;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler samplerDepth = sampler_state{
	Texture = TextureDepth;
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
	float4 Depth		: TEXCOORD1;
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
	Out.Depth = Out.Pos;
	return Out;
}

// PixelShader
float tangent(float3 p, float3 s) {
    return (p.z - s.z) / length(s.xy - p.xy);
}

float4 PS( VS_OUTPUT In ) : COLOR0
{
	float4 color;
	color = tex2D( samplerDiffuse, In.Tex );

	float d = 0.0f;
	for( int i=0; i<SAMPLE_MAXNUM; ++i )
	{
		// サンプリングオフセット値
//		float2 samp_offset = sampOffset[i] * ((1.0f-In.Depth.z) * 10.f);
		float2 samp_offset = sampOffset[i];
         
        // サンプリング点Aの深度値取得
        float2 sl = In.Tex + samp_offset;
        float3 pl = float3( sl.xy, tex2D(samplerDepth, sl).r );
         
        // モデルのエッジを検出
        if( pl.z > In.Depth.z - EdgeBias ) continue;
         
        // 中心点からの角度を計算
        float tl = atan( tangent(In.Depth, pl) );
 
        // サンプリング点A’の深度値取得
        float2 sr = In.Tex - samp_offset;
        float3 pr = float3( sl.xy, tex2D(samplerDepth, sr).r );
 
        if( pr.z > In.Depth.z - EdgeBias ) continue;
 
        float tr = atan( tangent(In.Depth, pr) );
         
        // 遮蔽率を足し込む
        d += clamp( (tl+tr) / PI, 0.0f, 1.0f );
	}

	d = clamp( d / SAMPLE_MAXNUM, 0.0f, 1.0f );
	float tone = clamp( (1.0f - d) * 2.0f, 0.f, 1.f);
//	color.rgb *= tone;
	color.rgb = (tone * 0.7f) + 0.3f;
//	color = (In.Depth.z / In.Depth.w);
	color.a = 1.0f;
	return color;
}

// Technique
technique TShader
{
   pass P0
   {
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_3_0 PS();
   }
}



// ===== Depth
// VertexShader
struct DEPTH_VS_OUTPUT
{
	float4 Pos   : POSITION;
	float4 Depth : TEXCOORD0;
	float4 Color : COLOR0;
};

DEPTH_VS_OUTPUT DepthVS( 
	float4 Pos		: POSITION,
	float4 Color 	: COLOR0
){
	DEPTH_VS_OUTPUT Out;
	Out.Pos = mul( Pos, wvpMtx );
	Out.Depth = Out.Pos;
	Out.Color = Color;
	return Out;
}

// PixelShader
float4 DepthPS( DEPTH_VS_OUTPUT In ) : COLOR0
{
	float4 color = (In.Depth.z / In.Depth.w);
	color.a = 1.0f;
	color.b = 0.0f;
	return color;
}

// Technique
technique TDepth
{
   pass P0
   {
		VertexShader = compile vs_2_0 DepthVS();
		PixelShader  = compile ps_2_0 DepthPS();
   }
}
