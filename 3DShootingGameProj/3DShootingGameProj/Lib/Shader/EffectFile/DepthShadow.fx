
// ワールド変換行列
float4x4 g_world : WORLD;

// カメラビュー変換行列
float4x4 g_view : VIEW;

// 射影変換行列
float4x4 g_proj : PROJECTION;

// ワールドカメラ射影変換行列(外部から)
float4x4 g_wvp : WVP;

// ライト目線のビュー変換行列
float4x4 g_light_view;

// ライト射影変換行列
float4x4 g_light_proj;

// シャドウテクスチャ
texture g_shadow_tex;

// テクスチャ
texture g_tex;

// カラー
float4 g_color;


/*--- ライト系 ---*/

/* マテリアル */
// 環境光
float4 g_ambient;
// マテリアルの環境光
float4 g_mt_ambient;
// 拡散光
float4 g_mt_diffuse;
// 反射光
float4 g_mt_specular;


/* その他 */
// 視点座標(正規化済み)
float4 g_eye_dir;
// カメラ座標
float4 g_eye_pos;
// ライト方向(平行光源)
float4 g_light_dir;
// ライトカラー
float4 g_light_color;

/* 点光源(pl = point_light) */
// 座標
float4 g_pl_pos;
// 拡散
float4 g_pl_diffuse;
// 反射
float4 g_pl_specular;
// 減衰
float4 g_pl_attenuate;


// テクスチャサンプラ
sampler smp = sampler_state {
	texture = (g_shadow_tex);
};

sampler tex_smp = sampler_state {
	texture = (g_tex);
};

// 頂点返す用
struct VS_OUT {
	float4 pos : POSITION;
	float4 z_calc_tex : TEXCOORD0;
	float4 col : COLOR0;
	float2 uv : TEXCOORD1; // w=影の濃さ

	// ライト用
	float4 nor_w : TEXCOORD2;// ワールド法線ベクトル
	float4 pos_w : TEXCOORD3;// ワールド座標
};


// 各バックバッファに返す用
struct PS_OUT {
	float4 col0 : COLOR0;// ノーマル
	float4 col1 : COLOR1;// 光付き
	float4 col2 : COLOR2;// 光と影付き
};


float4 FhoneShaderPS(VS_OUT In) : COLOR0{

	// 法線
	float n;
	float v;
	float l;
	float r;
	float d;
	float a;
	// 環境、鏡面、反射
	float3 amb;
	float3 dif;
	float3 spe;

	// ワールド法線正規化
	n = normalize(In.nor_w.xyz);

	// そのオブジェクト間の方向を出す
	v = normalize(g_eye_pos.xyz - In.pos_w.xyz);

	// 点光源からワールド位置まで
	l = g_pl_pos.xyz - In.pos_w.xyz;

	// 距離を出す
	d = length(l);

	// plからwまで方向を出す
	l = normalize(l);

	// 鏡面反射計算
	r = 2.0f * n * dot(n, l) - l;

	// 鏡面計算
	a = saturate(1.0f / (g_pl_attenuate.x + g_pl_attenuate.y * d +
		g_pl_attenuate.z * d * d));

	// 環境光
	amb = g_mt_ambient.xyz * g_ambient.xyz;

	// 拡散光
	dif = saturate(dot(l,n)) * g_mt_diffuse.xyz * g_pl_diffuse.xyz;

	// 反射光
	spe = pow(saturate(dot(r, v)), g_mt_specular.w) * 
		g_mt_specular.xyz * g_pl_specular.xyz * a;

	// 総合計して返す
	return float4(saturate(amb + dif + spe), 1.0f);
}


VS_OUT FhoneShaderVS(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL,
	float2 uv : TEXCOORD
) {

	VS_OUT Out = (VS_OUT)0;

	// ワールド、射影、ビュー座標変換
	Out.pos = mul(pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);


	Out.uv = uv;
	Out.pos_w = pos;
	Out.nor_w = float4(normal.x,normal.y,normal.z,1.0f);

	// 色乗算
	Out.col = color;
	Out.col *= FhoneShaderPS(Out);
	

	return Out;
}


float4 FhoneShaderPS2(VS_OUT In) : COLOR0{

	return In.col;
}


// バーテックス
VS_OUT DepthBufferShadowVS(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL,
	float2 uv : TEXCOORD
) {

	// 初期化
	VS_OUT Out = (VS_OUT)0;

	// カメラ目線のワールドビュー射影変換
	float4x4 mat;
	mat = mul(mul(g_world, g_view), g_proj);
	Out.pos = mul(pos, mat);

	// ライト目線によるワールドビュー射影変換
	mat = mul(g_world, g_light_view);
	mat = mul(mat, g_light_proj);
	Out.z_calc_tex = mul(pos, mat);

	// 法線とライトの方向から頂点の色を決定
	float3 n = normalize(mul(float4(normal,1.f), g_world).xyz);

	// ライト行列位置を正規化(転置行列)
	float3 light_direct = normalize(float3(
		g_light_view._13,
		g_light_view._23,
		g_light_view._33
	));

	// 最終的なカラーを出す
	Out.col = float4(0.f, 0.6f, 1.f, 1.f) * 
		(0.3f + dot(n, -light_direct)*(1 - 0.3f));

	Out.nor_w = float4(normal.x,normal.y,normal.z,1.f);
	Out.pos_w = float4(pos.x,pos.y,pos.z,1.f);

	// テクスチャ座標代入
	Out.uv = uv;

	// 色乗算
	Out.col = color;

	// ライトデータを取ってくる
	Out.col *= FhoneShaderPS(Out);

	return Out;
}


// ピクセル
float4 DepthBufferShadowPS(
	VS_OUT In
) : COLOR0
{

	// カラーを返す
	float4 recolor = 0.f;
	// ライト目線によるz値の再算出
	float z_value = In.z_calc_tex.z / In.z_calc_tex.w;

// テクスチャ座標に変換
float2 trans_tex_coord;

// テクスチャ計算
trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

// 同じ座標のz値を抽出
float sm_z = tex2D(smp, trans_tex_coord).x;

// バイアスを掛ける
float bias = 0.005f;

// カラー値加算
recolor = In.col;

// 算出点がシャドウマップのz値よりも大きければ影と判断
if (z_value > sm_z + bias) {

	// 色変更
	recolor.rgb = In.col.rgb * 0.5f;
}

return recolor;
}


float4 TextureDepthBufferShadowPS(
	VS_OUT Out
) : COLOR0
{
	float4 recolor = 0;

	// ライト目線によるz値の再算出
	float z_value = Out.z_calc_tex.z / Out.z_calc_tex.w;

// テクスチャ座標に変換
float2 trans_tex_coord;

// テクスチャ座標変換計算
trans_tex_coord.x = (1.f + Out.z_calc_tex.x / Out.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - Out.z_calc_tex.y / Out.z_calc_tex.w) * 0.5f;

// 同じ座標のz値を抽出
float sm_z = tex2D(smp,trans_tex_coord).x;

// テクスチャ値計算
float4 tex = tex2D(tex_smp, Out.uv);

// カラー値加算
recolor = Out.col;

// テクスチャ値加算
recolor += tex;

// バイアスを掛ける
float bias = 0.005f;

// 算出点がシャドウマップのz値よりも大きければ影と判断
if (z_value > sm_z + bias) {

	// 色変更
	recolor.rgb = Out.col.rgb * 0.5f;
}

return recolor;

}


// シャドウマップをソフトシャドウに変換
float4 SoftShadowVS(
	float4 pos : POSITION,
float4 NORMAL : NORMAL,
float2 tex : TEXCOORD0
) {

	
}


// テクニック
technique DepthBufferShadowTec {

	// テクスチャ無し
	pass P0
	{
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 DepthBufferShadowPS();
	}

	// テクスチャ有
	pass P1
	{
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 TextureDepthBufferShadowPS();
	}

	// フォンシェーダー
	pass P2
	{
		VertexShader = compile vs_2_0 FhoneShaderVS();
		PixelShader = compile ps_2_0 FhoneShaderPS();
	}

	pass P3
	{
		VertexShader = compile vs_2_0 FhoneShaderVS();
		PixelShader = compile ps_2_0 FhoneShaderPS2();
	}
}