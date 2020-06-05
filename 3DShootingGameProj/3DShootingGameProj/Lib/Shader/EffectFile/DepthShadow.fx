
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
	float4 pos_w : POSITION;// ワールド座標
};


// 各バックバッファに返す用
struct PS_OUT {
	float4 col0 : COLOR0;// ノーマル
	float4 col1 : COLOR1;// 光付き
	float4 col2 : COLOR2;// 光と影付き
};


float4 FhoneShader(VS_OUT In) {

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

	n = normalize(In.nor_w.xyz);
	v = normalize(g_eye_pos.xyz - In.pos_w.xyz);
	l = g_pl_pos.xyz - In.pos_w.xyz;
	d = length(l);
	l = normalize(l);
	// 鏡面反射計算
	r = 2.0f * n * dot(n, l) - l;
	a = saturate(1.0f / (g_pl_attenuate.x + g_pl_attenuate.y * d +
		g_pl_attenuate.z * d * d));

	amb = g_mt_ambient.xyz * g_ambient.xyz;
	dif = g_mt_dif.xyz * g_pl_dif.xyz;
	spe = pow(saturate(dot(r, v)), g_mt_specular.w) *
		g_specular.xyz * g_pl_specular.xyz * a;

	// 総合計して返す
	return float4(saturate(amb + dif + spe), 1.0f);
}


// 頂点シェーダー
VS_OUT DepthBufferShadowVS(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL,
	float2 uv : TEXCOORD
) {

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

	// テクスチャ座標代入
	Out.uv = uv;

	// ライト変換
	{
		// ライト方向で入力されるので、頂点からライト位置とするために逆向きに変換する
		// 正規化を必ず行う
		Out.l = -g_dir_light.xyz;

		// 法線代入
		Out.n = normal.xyz;

		// ライトベクトルと法線ベクトルの内積を計算し、
		// 計算結果の色の最低値を環境光(Ambient)に制限する
		Out.col = min(max(g_ambient, dot(Out.n, Out.l)), 1.f);

		// 頂点から視点へのベクトル計算
		Out.e = g_eye_pos.xyz - pos.xyz;
	}

	return Out;
}


float4 DepthBufferShadowPS(
	VS_OUT In
) : COLOR0
{

	// ライト目線によるz値の再算出
	float z_value = In.z_calc_tex.z / In.z_calc_tex.w;

// テクスチャ座標に変換
float2 trans_tex_coord;

// テクスチャ計算
trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

// 同じ座標のz値を抽出
float sm_z = tex2D(smp, trans_tex_coord).x;

FhoneShader(In);

float bias = 0.005f;

// 算出点がシャドウマップのz値よりも大きければ影と判断
if (z_value > sm_z + bias) {

	// 色変更
	In.col.rgb = In.col.rgb * 0.5f;
}

return In.col;
}


float4 TextureDepthBufferShadowPS(
	float4 col : COLOR0,
	float4 z_calc_tex : TEXCOORD0,
	float2 uv : TEXCOORD1,
	float3 n : TEXCOORD2,// オブジェクトの法線ベクトル
	float3 l : TEXCOORD3,// 頂点からライト位置へのベクトル
	float3 e : TEXCOORD4// 頂点から視点へのベクトル
) : COLOR0
{

	// ライト目線によるz値の再算出
	float z_value = z_calc_tex.z / z_calc_tex.w;

// テクスチャ座標に変換
float2 trans_tex_coord;

// テクスチャ計算
trans_tex_coord.x = (1.f + z_calc_tex.x / z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - z_calc_tex.y / z_calc_tex.w) * 0.5f;

// 同じ座標のz値を抽出
float sm_z = tex2D(smp,trans_tex_coord).x;

// テクスチャ値計算
float4 tex = tex2D(tex_smp, uv);

// テクスチャ値代入
col = tex;

// バイアスを掛ける
float bias = 0.005f;

// 算出点がシャドウマップのz値よりも大きければ影と判断
if (z_value > sm_z + bias) {

	// 色変更
	col.rgb = col.rgb * 0.5f;
}

return col;

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
}