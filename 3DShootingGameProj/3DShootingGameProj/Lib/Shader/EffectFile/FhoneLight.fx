
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

	Out.col = color;
	Out.uv = uv;
	Out.pos_w = pos;
	Out.nor_w = float4(normal.x, normal.y, normal.z, 1.0f);

	return Out;
}


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

