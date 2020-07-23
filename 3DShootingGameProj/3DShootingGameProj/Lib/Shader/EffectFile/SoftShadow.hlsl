

// ワールドライトプロジェクション
float4x4 WLP;
// 座標変換行列(world)
float4x4 WVP;
// ワールド * ビュー * 射影 * テクスチャ
float4x4 WVPB;

// ライトの方向
float4 light_dir;

// シャドウマップサイズ
float MAP_SIZE;

// 全体カラー
float4 g_color;

// モデルテクスチャ
texture g_model_tex;

sampler tex1 = sampler_state {
	texture = <g_model_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
};


texture g_shadow_map;

sampler src_samp = sampler_state {
	texture = <g_shadow_map>;
	AddressU = WRAP;
	AddressV = WRAP;
};


struct VS_OUT_PASS0 {
	float4 pos : POSITION;
	float4 depth : TEXCOORD0;
};


// 1パス目の処理
VS_OUT_PASS0 VS_PASS0(
	float4 pos : POSITION
) {

	VS_OUT_PASS0 Out =
		(VS_OUT_PASS0)0;

	// 座標変換
	float4 mul_pos = mul(pos, WLP);

	// 位置代入
	Out.pos = mul_pos;

	// カメラ座標系での深度をテクスチャに入れる
	Out.depth = mul_pos.zzzw;

	return Out;
}


float4 PS_PASS0(
	VS_OUT_PASS0 In
) : COLOR0{

	// zをwで割る
	return (In.depth / In.depth.w);

}


struct VS_OUT_QUAD {
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 tex0 : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
};

// 深度の輪郭を抽出
VS_OUT_QUAD VS_PASS1(
	float4 pos : POSITION,
	float4 col : COLOR0,
	float4 tex : TEXCOORD0
) {

	VS_OUT_QUAD Out = (VS_OUT_QUAD)0;

	// 位置
	Out.pos = pos;

	Out.col = col;

	// 斜め上下左右のテクセルの中心を指定

	float map_size = 100.f;

	// 左上
	Out.tex0 = tex.xy + float2(-0.5f / map_size, -0.5f / map_size);

	// 右下
	Out.tex1 = tex.xy + float2(0.5f / map_size, 0.5f / map_size);

	// 左下
	Out.tex2 = tex.xy + float2(-0.5f / map_size, 0.5f / map_size);

	// 右上
	Out.tex3 = tex.xy + float2(0.5f / map_size, -0.5f / map_size);

	return Out;
}
float4 PS_PASS1(
	VS_OUT_QUAD In
) : COLOR0{

	float4 color;

// 差を測り、中心テクセルを抽出
float d0 = (float)(tex2D(src_samp,In.tex0) - tex2D(src_samp,In.tex1));
float d1 = (float)(tex2D(src_samp,In.tex2) - tex2D(src_samp,In.tex3));

// 差を2乗する
// 正負の符号を取り除く為に2乗する
color = (d0 * d0) + (d1 * d1);

//color = In.col;

return color;
}


// ぼかし処理
VS_OUT_QUAD VS_PASS2(
	float4 pos : POSITION, // モデルの頂点
	float4 tex : TEXCOORD0 // テクスチャ座標
) {

	// 出力データ
	VS_OUT_QUAD Out = (VS_OUT_QUAD)0;

	// 位置座標
	Out.pos = pos;

	Out.tex0 = (float2)tex + float2(-3.0f / MAP_SIZE, -3.0f / MAP_SIZE);
	Out.tex1 = (float2)tex + float2(-3.0f / MAP_SIZE, -1.0f / MAP_SIZE);
	Out.tex2 = (float2)tex + float2(-3.0f / MAP_SIZE, 1.0f / MAP_SIZE);
	Out.tex3 = (float2)tex + float2(-3.0f / MAP_SIZE, 3.0f / MAP_SIZE);
	Out.tex4 = (float2)tex + float2(-1.0f / MAP_SIZE, -3.0f / MAP_SIZE);
	Out.tex5 = (float2)tex + float2(-1.0f / MAP_SIZE, -1.0f / MAP_SIZE);
	Out.tex6 = (float2)tex + float2(-1.0f / MAP_SIZE, 1.0f / MAP_SIZE);
	Out.tex7 = (float2)tex + float2(-1.0f / MAP_SIZE, 3.0f / MAP_SIZE);

	return Out;
}
float4 PS_PASS2(VS_OUT_QUAD In) : COLOR0{

	float4 color;

	float2 dvu = float2(4.0f / MAP_SIZE, 0);

	// 16回
	color =
		+tex2D(src_samp, In.tex0)
		+ tex2D(src_samp, In.tex1)
		+ tex2D(src_samp, In.tex2)
		+ tex2D(src_samp, In.tex3)
		+ tex2D(src_samp, In.tex4)
		+ tex2D(src_samp, In.tex5)
		+ tex2D(src_samp, In.tex6)
		+ tex2D(src_samp, In.tex7)

		// ぼかしを調整
		+ tex2D(src_samp, In.tex0 + dvu)
		+ tex2D(src_samp, In.tex1 + dvu)
		+ tex2D(src_samp, In.tex2 + dvu)
		+ tex2D(src_samp, In.tex3 + dvu)
		+ tex2D(src_samp, In.tex4 + dvu)
		+ tex2D(src_samp, In.tex5 + dvu)
		+ tex2D(src_samp, In.tex6 + dvu)
		+ tex2D(src_samp, In.tex7 + dvu);

		return (0.7f * color);
}


struct VS_OUT {
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 diffuse : TEXCOORD0;
	float4 ambient : TEXCOORD1;
	float4 shadow_map_uv : TEXCOORD2;
	float4 depth : TEXCOORD3;
};


// シャドウマップの時と同じ計算
VS_OUT VS_PASS3(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL
) {

	VS_OUT Out = (VS_OUT)0;
	float4 uv;

	// 座標変換
	Out.pos = mul(pos, WVP);

	Out.color = color;

	// 色
	Out.diffuse = color * max(dot(light_dir.xyz, normal.xyz), 0);// 拡散色
	Out.ambient = color * 0.3f; // 環境色

	// テクスチャ座標変換
	uv = mul(pos, WVPB);
	Out.shadow_map_uv = uv;

	// 深度値算出
	uv = mul(pos, WLP);
	Out.depth = uv.zzzw;

	return Out;
}
float4 PS_PASS3(VS_OUT In) : COLOR0{

	// カラー
	float4 color = In.ambient;

	// シャドウマップ
	float shadow_map =
		tex2Dproj(tex1, In.shadow_map_uv).x;

	// 深度比較
	if (shadow_map < In.depth.z / In.depth.w - 0.01f) {

		color += tex2Dproj(src_samp, In.shadow_map_uv);
	}
	else {
		color += 1.f;
	}

	return color;
}


// テクニック
technique tech{

	pass P0
	{
		VertexShader = compile vs_2_0 VS_PASS0();
		PixelShader = compile ps_2_0 PS_PASS0();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS_PASS1();
		PixelShader = compile ps_2_0 PS_PASS1();
	}
	
	pass P2
	{
		VertexShader = compile vs_2_0 VS_PASS2();
		PixelShader = compile ps_2_0 PS_PASS2();
	}
	
	pass P3
	{
		VertexShader = compile vs_2_0 VS_PASS3();
		PixelShader = compile ps_2_0 PS_PASS3();
	}
}

/*
// パス０では影イメージを作成する
struct VS1_OUTPUT
{
	float4 Pos       : POSITION;
	float4 LightUV   : TEXCOORD0;      // Zバッファテクスチャーのテクセル座標
	float2 Depth     : TEXCOORD1;    // カメラ基準の行列変換により計算した深度値
};

VS1_OUTPUT VS1(
	float4 Pos     : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD0
)
{
	VS1_OUTPUT Out;

	Out.Pos = mul(Pos, m_WVP);

	// Zバッファの深度情報を取得するためにテクセル座標をセット
	Out.LightUV = mul(Pos, WVPB);

	// ライト基準の行列変換により深度値を計算する
	Out.Depth.xy = mul(Pos, WLP).zw;

	return Out;
}

float4 PS1(VS1_OUTPUT In) : COLOR0
{
	//出力するデフォルト色は白
	float4 Out = (float4)1;

	//Zバッファから深度値を取得する
	float d = tex2Dproj(tex1, In.LightUV).r;

	//影となるので影の色を出力
	if (d * In.Depth.y < In.Depth.x - m_Bias)
	   Out = m_ShadowColor;

	return Out;
}


// シャドウマップの時と同じ計算
VS_OUT VS_PASS1(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL
) {

	VS_OUT Out = (VS_OUT)0;
	float4 uv;

	// 座標変換
	Out.pos = mul(pos, WVP);

	Out.color = color;

	// 色
	Out.diffuse = color * max(dot(light_dir.xyz, normal.xyz), 0);// 拡散色
	Out.ambient = color * 0.3f; // 環境色

	// テクスチャ座標変換
	uv = mul(pos, WVPB);
	Out.shadow_map_uv = uv;

	// 深度値算出
	uv = mul(pos, WLP);
	Out.depth = uv.zzzw;

	return Out;
}
float4 PS_PASS1(VS_OUT In) : COLOR0{

	// カラー
	float4 color = In.ambient;

	// シャドウマップ
	float shadow_map =
		tex2Dproj(tex1, In.shadow_map_uv).x;

	// 深度比較
	if (shadow_map < In.depth.z / In.depth.w - 0.01f) {

		color += tex2Dproj(src_samp, In.shadow_map_uv);
	}
	else {
		color += 1.f;
	}

	return color;
}*/