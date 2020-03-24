

// 平行光源ライティング(ディレクショナルライト)

float4x4 g_world : WORLD;
float4x4 g_proj : PROJECTION;
float4x4 g_view : VIEW;

/* 外部グローバル */
// ライト方向
float4 g_dir_light;

// 点光源
float4 g_point_light_pos;

// カラー
float4 g_color;

// ライトカラー
float4 g_light_color;

// 視点座標
float4 g_eye_dir;

// カメラ座標
float4 g_eye_pos;

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
	//float4 pos2;
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
	ref_vec = 2.0f * normal_nor * dot(normal_nor, dir_light_nor) - dir_light_nor;

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

	Out.normal = mul(In.normal, g_world);

	return Out;
}


//// フォン反射モデル
//VS_OUT PhongReflectionVS(VS_IN In) {
//
//}


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
}