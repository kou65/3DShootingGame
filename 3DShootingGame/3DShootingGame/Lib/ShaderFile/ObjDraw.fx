
// グローバル定数
// ビュー行列
float4x4 g_view_mat : VIEW;

// 射影行列
float4x4 g_proj_mat : PROJECTION;

// ワールド行列
float4x4 g_world_mat : WORLD;

// カラー
float4 g_color;

// 使用するテクスチャ
texture g_tex;

// サンプラブロック
sampler smp = sampler_state {
	texture = <g_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
	// CLAMP
	// BORDER
};


// 頂点シェーダー
void VS(
float3 pos : POSITION,
float3 normal : NORMAL,
float4 color : COLOR0,
float2 uv : TEXCOORD0,
	out float4 get_pos : POSITION,
	out float4 get_color : COLOR0,
	out float2 get_uv : TEXCOORD0
)
{
	// 出力用
	float4x4 mat;

	// ワールド変換
	mat = mul(mul(g_world_mat,g_view_mat),g_proj_mat);

	float4 out_pos = mul(float4(pos,1.0f),mat);

	// 外へ出力
	get_color = color;// + g_color;
	get_uv = uv;
	get_pos = out_pos;
}



float4 PS1(
	float4 color : COLOR0,
	float2 uv : TEXCOORD0
) : COLOR0
{
	float4 out_put;

// テクスチャ座標 * カラー値
out_put = color;

return out_put;
};


// ピクセルシェーダ
float4 PS2(
	float4 color : COLOR0,
	float2 uv : TEXCOORD0
) : COLOR0
{

float4 out_put;

// テクスチャ座標 * カラー値
out_put = tex2D(smp, uv);

// カラー値掛け算
out_put *= color;

return out_put;
};


// テクニック
technique tech1 {

	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS1();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();
	}
}
