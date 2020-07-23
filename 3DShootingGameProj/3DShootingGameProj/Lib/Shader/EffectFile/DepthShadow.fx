
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
	AddRessU = WRAP;
	AddRessV = WRAP;
};

sampler tex_smp = sampler_state {
	texture = (g_tex);
	AddRessU = WRAP;
	AddRessV = WRAP;
};


struct LIGHT_OUT {

	// 光
	float4 ambient : TEXCOORD5;
	float4 specular : TEXCOORD6;
	float4 diffuse : TEXCOORD7;
};

// 頂点返す用
struct VS_OUT {
	float4 pos : POSITION;
	float4 z_calc_tex : TEXCOORD0;
	float4 col : COLOR0;
	float2 uv : TEXCOORD1; // w=影の濃さ
	float4 nor_w : TEXCOORD2;
	float4 pos_w : TEXCOORD3;

	// ライト方向
	float4 L : TEXCOORD4;
	float3 V : TEXCOORD5;
	float3 H : TEXCOORD6;
	float3 normal : TEXCOORD7;
};


// 各バックバッファに返す用
struct PS_OUT {
	float4 col0 : COLOR0;// ノーマル
	float4 col1 : COLOR1;// 光付き
	float4 col2 : COLOR2;// 光と影付き
};

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
	//Out.col = float4(0.f, 0.6f, 1.f, 1.f) * 
	//	(0.3f + dot(n, -light_direct)*(1 - 0.2f));

	Out.pos_w = mul(pos, g_world);
	Out.nor_w = mul(normal.xyz, g_world);

	// 位置からのライト方向を出す
	float3 L = normalize(g_pl_pos.xyz - Out.pos_w.xyz);

	// 拡散反射
	{
		// テクスチャ座標代入
		Out.uv = uv;
	
		// カラー値
		Out.col = float4(1.f, 1.f, 1.f, 1.f);
	
		float3 nor_w = mul(normal.xyz, g_world);
	
		// 方向
		nor_w.xyz = normalize(Out.nor_w.xyz);

		float i = saturate(dot(L, nor_w));

		// 拡散反射の計算はVSで行う
		float3 diffuse = i * g_mt_diffuse.xyz * g_pl_diffuse.xyz;

		Out.col = Out.col * 
			float4(diffuse.x, diffuse.y, diffuse.z, 1.f);
	}

	// 鏡面反射
	{
		float3 l = L.xyz;
		float3 eye = normalize(g_eye_pos - Out.pos_w);
		float3 H = normalize(l + eye);

		Out.L = float4(L.x,L.y,L.z,0.f);
		Out.V = eye;
		Out.H = H;
		Out.normal = normal;
		
	}

	// 環境
	{
		// 面の色 * 環境輝度
		Out.col = (Out.col * g_mt_ambient * g_ambient);
	}

	return Out;
}


float4 PS(VS_OUT In) : COLOR0{
	// 反射
{
	//In.col = In.col *  max(0.3f,dot(nor_w,-g_light_dir))
}
	return In.col;
}


float4 TextureDepthBufferShadowPS(
	VS_OUT In
) : COLOR0
{
	float4 recolor = (float4)0.f;

	// ライト目線によるz値の再算出
	float z_value = In.z_calc_tex.z / In.z_calc_tex.w;

// テクスチャ座標に変換
float2 trans_tex_coord;

// テクスチャ座標変換計算
trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

// 同じ座標のz値を抽出
float sm_z = tex2D(smp,trans_tex_coord).x;

// テクスチャ値計算
float4 tex = tex2D(tex_smp,In.uv);

// ブリンフォン反射モデル
In.col = In.col * max(1.f, dot(In.normal, -In.L))
+ pow(max(0, dot(In.normal, In.H)), 10);

//float i = pos(saturate(dot(In.H, In.N)), g_mt_specular.w);

// 鏡面反射
//In.col = float4(i * g_mt_specular.xyz * g_light_color.xyz, 1.f);

// カラー
recolor = In.col * tex;

// バイアスを掛ける
float bias = 0.005f;

// 算出点がシャドウマップのz値よりも大きければ影と判断
if (z_value > sm_z + bias) {

	// 色変更
	recolor.rgb = recolor.rgb * 0.5f;
}

return recolor;
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

	// 射影空間のxy座標をテクスチャ座標に変換
	trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
	trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

	// 同じ座標のz値を抽出
	float sm_z = tex2D(smp,trans_tex_coord).x;

	// バイアスを掛ける
	float bias = 0.005f;

	// カラー値加算
	recolor = In.col;

	//// 算出点がシャドウマップのz値よりも大きければ影と判断
	//if (z_value > sm_z + bias) {
	//
	//	// 色変更
	//	recolor.rgb = In.col.rgb * 0.5f;
	//}

	return recolor;
}

// テクニック
technique DepthBufferShadowTec {

	// テクスチャ無し
	pass P0
	{
		AlphaBlendEnable = false;
		
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 DepthBufferShadowPS();
	}

	// テクスチャ有
	pass P1
	{
		AlphaBlendEnable = false;

		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 TextureDepthBufferShadowPS();
		//PixelShader = compile ps_2_0 PS();
	}

}