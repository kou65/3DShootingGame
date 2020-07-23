

// 平行光源ライティング(ディレクショナルライト)

float4x4 g_world : WORLD;
float4x4 g_proj : PROJECTION;
float4x4 g_view : VIEW;

/* 外部グローバル */
// ライト方向
float4 g_dir_light;

// 点光源(ライト座標)
float4 g_point_light_pos;

// 明るさ減衰調整パラメータ
float4 g_attenuation;

// 環境光
float4 g_ambient;
// スペキュラーの範囲
float g_specular;
// スペキュラーの強度
float g_specular_power;

// ライトカラー
float4 g_light_color;

// カラー
float4 g_color;

// 視点座標
float4 g_eye_dir;

// カメラ座標
float4 g_eye_pos;

// 使用するテクスチャ
texture g_tex;

// シャドウマップ
texture g_shadow_tex;


// サンプラブロック
sampler smp = sampler_state {
	texture = <g_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
	// CLAMP
	// BORDER
};

// 頂点データに法線がある前提

struct VS_IN {
	float4 pos : POSITION0;
	float4 normal : NORMAL;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};


struct VS_OUT {
	float4 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float4 normal : TEXCOORD1;
};

struct VS_OUT_WORLD {
	float4 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;

	// ワールド座標のピクセル
	float4 normalw : TEXCOORD1;
	float4 posw : TEXCOORD2;
};



// 平行光源
VS_OUT DirectionalLightVS(VS_IN In) {

	VS_OUT Out;
	float4 nor;
	float col;

	// ワールド、射影、ビュー座標変換
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	// 法線
	nor = mul(In.normal, g_world).xyzw;
	// 正規化
	nor = normalize(nor);

	// 光を付ける
	col = saturate(dot(nor,g_dir_light));

	// 輝度
	float luminance = 0.5f;

	col = col * 0.5f + luminance;

	// グローバルカラー値変更
	col *= g_color;

	Out.color = float4(col, col, col, 1.f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}

// 拡散反射
VS_OUT DiffuseReflectionVS(VS_IN In) {

	VS_OUT Out;

	float4 light_nor_vec;
	float4 normal_nor_vec;
	float col;

	// ワールド、射影、ビュー座標変換
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	// ピクセル座標と光源の座標から光源へ向かう正規化ベクトルを求める
	// 光源座標からピクセル座標を引き、向かうベクトル(足すと)を正規化する
	light_nor_vec = normalize(g_point_light_pos.xyzw - In.pos.xyzw);

	// 法線ベクトル正規化
	normal_nor_vec = normalize(In.normal.xyzw);

	// -まで行ったら0に戻すようにする,内積で角度を求める
	col = saturate(dot(light_nor_vec, normal_nor_vec));

	Out.color = float4(col * 0.5f * 0.5f,col ,col,1.0f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// 鏡面反射
VS_OUT SpecularReflectionVS(VS_IN In) {

	VS_OUT Out;

	// 正規化済みライトからピクセル座標への方向
	float4 dir_light_nor;

	// 正規化済み法線
	float4 normal_nor;

	// 正反射ベクトル
	float4 ref_vec;

	// 視点座標からピクセル位置の方向
	float4 view_point;

	// 計算後の結果
	float col;

	// ワールド、射影、ビュー座標変換
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	// 点光源からピクセル位置までの単位ベクトル取得
	dir_light_nor = normalize(g_point_light_pos.xyzw - In.pos.xyzw);

	// 法線ベクトル正規化
	normal_nor = normalize(In.normal);

	// 反射ベクトル
	ref_vec = 2.0f * normal_nor * dot(normal_nor, dir_light_nor)
		- dir_light_nor;

	// 視点座標からピクセル位置の単位ベクトル取得
	view_point = normalize(g_eye_dir.xyzw - In.pos.xyzw);

	// 輝度
	float specular = 0.5f;

	// 反射と視線ベクトルの内積を取得、光沢度を係数aでべき乗する
	col = pow(saturate(dot(ref_vec, view_point)),specular);

	Out.color = float4(col *g_color.xyz * specular, 1.0f);
	//Out.col = float4(col * 0.5f * 0.5f, col, col, 1.0f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// 環境反射(世界を少しだけ明るくする)
VS_OUT AmbientReflectionVS(VS_IN In) {

	VS_OUT Out;

	// ワールド、射影、ビュー座標変換
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	float4 col;

	col = g_color.xyzw * g_light_color.xyzw;

	Out.color = col;
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// 点光源ライティング
VS_OUT PointLightVS(VS_IN In) {

	VS_OUT Out;
	float4 nor;
	float col;

	// ワールド、射影、ビュー座標変換
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	// 点光源方向
	float4 point_light_dir;
	// 点光源までの距離
	float point_light_len;
	// 拡散
	float col_dif;
	// 環境
	float col_a;
	// 合計
	float total_col;


	// 点光源までの方向と大きさ
	point_light_dir = g_point_light_pos - In.pos;

	// 点光源の距離を出す
	point_light_len = length(point_light_dir);

	// 点光源の方向を正規化
	point_light_dir = point_light_dir / point_light_len;

	// 拡散反射
	col_dif = saturate(dot(normalize(In.normal), point_light_dir));

	float4 atten = g_attenuation;

	// 距離によって光を減衰させる(距離の2乗に反比例する、線形を使って減数させる)
	col_a = saturate(1.0f / (atten.x + atten.y * point_light_len
		+ atten.z * point_light_len * point_light_len));

	// 光源計算
	total_col = col_dif * col_a;

	Out.color = float4(total_col, total_col, total_col, 1.0f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// フォン反射モデル(局所照明モデル)
VS_OUT_WORLD PhongReflectionVS(VS_IN In) {

	VS_OUT_WORLD Out;

	// ワールド、射影、ビュー座標変換
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	Out.color = In.color;
	Out.uv = In.uv;
	Out.posw = In.pos;
	Out.normalw = In.normal;

	return Out;
}



// 通常ピクセルシェーダー
float4 ColorPS(VS_OUT In) : COLOR0{
	
return In.color;
}
float4 TextureColorPS(VS_OUT In) : COLOR0 {

	float4 out_put;

	// テクスチャ座標 * カラー値
	out_put = tex2D(smp, In.uv);

	out_put *= In.color;

	return out_put;
}


// フォン反射ピクセルシェーダ
float4 PhongReflectionPS(VS_OUT_WORLD In) : COLOR0 {

	float4 normal;
	float4 eye_pixel_dir;
	float4 p_light_pixel_dir;
	float4 len;
	float ref;
	float col_a;

	// 光源
	float4 ambient;
	float4 diffuse;
	float4 specular;

	// 法線を正規化
	normal = normalize(In.normalw.xyzw);

	// 視点からピクセルまでの方向正規化
	eye_pixel_dir = normalize(g_eye_dir - In.posw);

	// 点光源からピクセルまでの方向
	p_light_pixel_dir = g_point_light_pos - In.posw;

	// 点光源までの距離
	len = length(p_light_pixel_dir);

	// 距離を正規化して方向だけ取得
	p_light_pixel_dir = normalize(p_light_pixel_dir);

	// 鏡面反射
	ref = 2.0 * normal * dot(normal, p_light_pixel_dir)
		- p_light_pixel_dir;

	float4 atten = g_attenuation;

	// 距離によって光を減衰させる(距離の2乗に反比例する、線形を使って減数させる)
	col_a = saturate(1.0f / (atten.x + atten.y * p_light_pixel_dir
		+ atten.z * p_light_pixel_dir * p_light_pixel_dir));

	// 環境
	ambient = 0.1f;//g_color * In.color;

	// 拡散反射
	diffuse = saturate(dot(normal, p_light_pixel_dir));

	float specular_light = 0.2f;

	// 鏡面反射
	specular = pow(saturate(dot(ref,eye_pixel_dir)), specular_light)
		* specular_light * specular_light * col_a;

	// ライトカラー
	float light_col = saturate(
		ambient + diffuse + specular_light);

	// フォン反射を返す
	return float4(
		light_col * g_color.x,
		light_col * g_color.y,
		light_col * g_color.z, 1.0);
}



/* フォンシェーディング */
struct VS_OUT_L {
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float3 n : TEXCOORD1;// オブジェクトの法線ベクトル
	float3 l : TEXCOORD2;// 頂点からライト位置へのベクトル
	float3 e : TEXCOORD3;// 頂点から視点へのベクトル
};



VS_OUT_L PhoneVS2(
	float4 pos : POSITION,
float4 normal : NORMAL,
float2 tex : TEXCOORD0
)
{
	VS_OUT_L Out;

	float4x4 mat;

	// 変換
	mat = mul(g_world, g_view);
	mat = mul(mat, g_proj);
	Out.pos = mul(pos, mat);

	// テクスチャ頂点
	Out.tex = tex;

	// ライト方向で入力されるので、頂点からライト位置とするために逆向きに変換する
	// 正規化を必ず行う
	Out.l = -g_dir_light.xyz;

	// 法線代入
	Out.n = normal.xyz;

	// ライトベクトルと法線ベクトルの内積を計算し、
	// 計算結果の色の最低値を環境光(Ambient)に制限する
	Out.col = min(max(g_ambient,dot(Out.n,Out.l)),1.f);

	// 頂点から視点へのベクトル計算
	Out.e = g_eye_pos.xyz - pos.xyz;

	return Out;
}



float4 PhonePS2(VS_OUT_L In) : COLOR0
{

float4 Out;

// 法線ベクトルを正規化する
float3 n = normalize(In.n);

// 頂点からライト位置ベクトル + 頂点から視点ベクトル
float3 h = normalize(In.l + normalize(In.e));

// スペキュラーカラーを計算する
float s = pow(max(0.0f,dot(n,h)),g_specular) * g_specular_power;

// スペキュラーカラーを加算する
//Out = In.col * tex2D(smp,In.tex) + s;
Out = In.col * s;

//Out = float4(0.f, 0.f, 0.f, 0.f);

return Out;
}



// テクニック
technique tech1 {


	// 平行光源
	// カラーだけ
	pass DirectionalLightColor
	{
		VertexShader = compile vs_2_0 DirectionalLightVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	// テクスチャ付き
	pass DirectionalLightTexture
	{
		VertexShader = compile vs_2_0 DirectionalLightVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}


	// 拡散反射
	// カラーだけ
	pass DiffuseReflectionColor
	{
		VertexShader = compile vs_2_0 DiffuseReflectionVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	// テクスチャ付き
	pass DiffuseReflectionTexture
	{
		VertexShader = compile vs_2_0 DiffuseReflectionVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}

	// 鏡面反射
	// カラー付き
	pass SpecularReflectionColor
	{
		VertexShader = compile vs_2_0 SpecularReflectionVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	// テクスチャ付き
	pass SpecularReflectionTexture
	{
		VertexShader = compile vs_2_0 SpecularReflectionVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}

	// 点光源ライティング
	// カラー
	pass PointLightColor
	{
		VertexShader = compile vs_2_0 PointLightVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	pass PointLightTexture
	{
		VertexShader = compile vs_2_0 PointLightVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}


	// フォン反射(8pass)
	pass PhoneRef {

		VertexShader = compile vs_2_0 PhongReflectionVS();
		PixelShader = compile ps_2_0 PhongReflectionPS();
	}
	
	// フォン反射2
	pass PhoneRef2 {
		VertexShader = compile vs_2_0 PhoneVS2();
		PixelShader = compile ps_2_0 PhonePS2();
	}


}