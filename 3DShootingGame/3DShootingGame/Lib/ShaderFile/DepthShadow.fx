
// ワールド変換行列
float4x4 mat_world : WORLD;

// カメラビュー変換行列
float4x4 mat_camera_view : VIEW;

// 射影変換行列
float4x4 mat_camera_proj : PROJECTION;

// ライト目線のビュー変換行列
float4x4 mat_light_view;

// ライト射影変換行列
float4x4 mat_light_proj;

texture shadow_tex;

sampler smp = sampler_state {
	texture = (shadow_tex);
};

struct VS_OUT {
	float4 pos : POSITION;
	float4 z_calc_tex : TEXCOORD0;
	float4 col : COLOR0;
};


// 頂点シェーダー
VS_OUT DepthBufferShadowVS(
	float4 pos : POSITION,
	float3 normal : NORMAL
) {

	VS_OUT Out = (VS_OUT)0;

	// カメラ目線のワールドビュー射影変換
	float4x4 mat;

	mat = mul(mat_world, mat_camera_view);

	mat = mul(mat, mat_camera_proj);

	Out.pos = mul(pos, mat);

	// ライト目線によるワールドビュー射影変換
	mat = mul(mat_world, mat_light_view);

	mat = mul(mat, mat_light_proj);

	Out.z_calc_tex = mul(pos, mat);

	// 法線とライトの方向から頂点の色を決定
	float3 n = normalize(mul(float4(normal,1.f), mat_world));

	// 拡縮を正規化
	float3 light_direct = normalize(float3(
		mat_light_view._13,
		mat_light_view._23,
		mat_light_view._33
		));

	Out.col = float4(0.f, 0.6f, 1.f, 1.f) *
		(0.3 + dot(n, -light_direct)*(1 - 0.3f));

	return Out;
}


float4 DepthBufferShadowPS
(float4 col : COLOR,float4 z_calc_tex : TEXCOORD) : COLOR
{

	// ライト目線によるz値の再算出
	float z_value = z_calc_tex.z / z_calc_tex.w;

// テクスチャ座標に変換
float2 trans_tex_coord;

// テクスチャ計算
trans_tex_coord.x = (1.f + z_calc_tex.x / z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - z_calc_tex.y / z_calc_tex.w) * 0.5f;

// 同じ座標のz値を抽出
float sm_z = tex2D(smp, trans_tex_coord).x;

// 算出点がシャドウマップのz値よりも大きければ影と判断
if (z_value > sm_z + 0.005f) {

	// 色変更
	col.rgb = col.rgb * 0.5f;
}

return col;
}


// テクニック
technique DepthBufferShadowTec {

	pass P0
	{
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 DepthBufferShadowPS();
	}
}