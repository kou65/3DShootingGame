
float4x4 g_world : WORLD;
float4x4 g_view : VIEW;
float4x4 g_proj : PROJECTION;

float4 g_light_dir;
float4 g_eye_pos;

// 環境光
float4 g_ambient;

// フォグカラー
float4 g_fog_color;

// フォグの開始位置
float g_near = 0.f;

// フォグの終了位置
float g_far = 1.f;

// テクスチャ
texture g_tex;

sampler smp = sampler_state {
	texture = <g_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
	// CLAMP
	// BORDER
};

struct VS_OUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float4 pos_wvp : TEXCOORD1;
};

struct VS_IN {
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


VS_OUT VS(VS_IN In) {

	VS_OUT Out;

	float4x4 mat;
	mat = mul(g_world, g_view);
	mat = mul(mat, g_proj);
	Out.pos = mul(In.pos, mat);

	// ライトの方向から逆方向
	float3 l_d = -g_light_dir.xyz;

	float3 normal = normalize(In.normal.xyz);

	// 法線とライトの方向から色変換
	Out.color = max(g_ambient, dot(normal, l_d));
	Out.uv = In.uv;
	Out.pos_wvp = In.pos;

	return Out;
}


float4 PS1(VS_OUT In) : COLOR0{

	float4 Out;

// 頂点と視点との距離を計算する(距離スカラーを出す)
float d = distance(In.pos_wvp.xyz, g_eye_pos.xyz);

// 距離から色割り出し
float f = (g_far - d) / (g_far - g_near);

// 最小最大にクランプする
f = clamp(f, 0.0f, 1.0f);

// テクスチャuvセット
Out = In.color * tex2D(smp,In.uv) * f + g_fog_color * (1.0f - f);

return Out;
}


float4 PS2(VS_OUT In) : COLOR0{

	return float4(1.f,1.f,1.f,1.f);
}


technique tech1 {

	pass Fog1 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS1();
	}
};