
float4x4 g_world;
float4x4 g_view;
float4x4 g_proj;


struct VS_OUT {
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 shadow_map : TEXCOORD;
};


// バーテックス
VS_OUT VS(
	float4 pos : POSITION,
	float3 normal : NORMAL
) {

	VS_OUT out_put = (VS_OUT)0;
	out_put.pos = mul(pos,g_world);
	out_put.pos = mul(out_put.pos, g_view);
	out_put.pos = mul(out_put.pos, g_proj);

	return out_put;
}