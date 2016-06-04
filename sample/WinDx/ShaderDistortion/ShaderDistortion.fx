//本城 基本変数
float4x4 worldMtx		: WORLD;		//本城 ワールドマトリクス
float4x4 viewMtx 		: VIEW;			//本城 ビューマトリクス
float4x4 projectionMtx  : PROJECTION;	//本城 プロジェクションマトリクス
float4x4 wvpMtx;						//本城 WVPマトリクス

//float DistortionParam = 0.2f;			//本城 歪み係数

//本城 歪みテクスチャ
sampler texture01 : register(s0) = sampler_state{
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//本城 シーン描画テクスチャ
texture textureScene;
sampler textureSceneSampler = sampler_state{
    Texture   = (textureScene);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//本城 頂点シェーダー
struct VS_OUTPUT
{
	float4 Pos   : POSITION;
	float2 Tex   : TEXCOORD0;
	float4 Color : COLOR0;
};

VS_OUTPUT VS( float4 Pos : POSITION, float2 Tex : TEXCOORD0, float4 Color : COLOR0){
	VS_OUTPUT Out;
   
	//本城 頂点
//	Out.Pos = mul(Pos, worldMtx);
//	Out.Pos = mul(Out.Pos, viewMtx);
//	Out.Pos = mul(Out.Pos, projectionMtx);
	Out.Pos = mul(Pos, wvpMtx);
	Out.Tex = Tex;
	Out.Color = Color;

	return Out;
}

//本城 ピクセルシェーダー
struct PS_OUTPUT
{
	float2 Tex   : TEXCOORD0;
	float4 Color : COLOR0;
};
float4 PS( PS_OUTPUT In ) : COLOR0
{
	//本城 歪みテクスチャの色取得
	float4 Color = tex2D( texture01, In.Tex );
	//本城 -1.f~1.f内に変換する
	Color.xy = ( Color.xy - 0.5f ) * 2.0f;
//	return tex2D( textureSceneSampler, In.Tex-(Color.xy) * Color.z * DistortionParam );
	return tex2D( textureSceneSampler, In.Tex-(Color.xy) * Color.z );
}
//本城 テクニック
technique TShaderDistortion
{
   pass P0
   {
      VertexShader = compile vs_1_1 VS();
      PixelShader  = compile ps_2_0 PS();
   }
}
