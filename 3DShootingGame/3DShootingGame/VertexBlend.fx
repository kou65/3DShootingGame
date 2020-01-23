

// グローバル定数
// ビュー行列
float4x4 g_view_mat;

// 射影行列
float4x4 g_proj_mat;

// ワールド行列
float4x4 g_world_mat;

// ボーン姿勢マトリックス
float4x4 g_bone_mat[64];

// アニメーションマトリックス
//float4x4 g_anim_mat[64];


// 使用するテクスチャ
texture g_tex;

sampler smp = sampler_state {
	texture = <g_tex>;
};


// 出力用
struct VS_IN {
	float4 pos : POSITION;
	float4 uv : TEXCOORD0;
};

struct VS_OUT {
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
};


// 頂点シェーダー
VS_OUT VS(
	float4 pos : POSITION,
	float4 normal : NORMAL,
	float4 color : COLOR0,
	float4 uv : TEXCOORD,
	float4 blend : BLENDWEIGHT,
	int4 blend_idx : BLENDINDICES
)
{

	// 重みを受け取る
	float w[4] = (float[4])blend;

float total_blend_weight = 0.f;
float4x4 mat_comb = 0.f;

// ワールド変換行列をブレンド
//for (int i = 0; i < 4; i++) {
//
//	// 最後の重みを計算
//	total_blend_weight += w[i];
//	
//	// マトリックス
//	mat_comb += g_bone_mat[blend_idx] * w[i]
//
//
//}

	// vs_out定義
	float4 out_pos;

	// ワールド変換
	out_pos = mul(pos,g_world_mat);

	// ビュー変換
	out_pos = mul(out_pos,g_view_mat);

	// 射影変換
	out_pos = mul(out_pos,g_proj_mat);
	
	// 頂点シェーダーの戻り値
	VS_OUT vs_out = (VS_OUT)0;

	vs_out.pos = out_pos;

	vs_out.uv = uv;

	return vs_out;
}


// ピクセルシェーダ
float4 PS(
	VS_OUT vs_out
) : COLOR0
{

	return tex2D(smp,vs_out.uv) * vs_out.color;
}


// テクニック
technique tech1{

	pass blend
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
