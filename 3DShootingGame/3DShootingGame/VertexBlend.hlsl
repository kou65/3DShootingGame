

// グローバル定数
// ビュー行列
float4x4 g_view_mat : VIEW;

// 射影行列
float4x4 g_proj_mat : PROJECTION;

// ワールド行列
float4x4 g_world_mat : WORLD;

// ボーン姿勢マトリックス
float4x4 g_bone_mat[64];

// ブレンドする配列の数
int g_max_blend_idx;

// 使用するテクスチャ
texture g_tex;

// サンプラブロック
sampler smp = sampler_state {
	texture = <g_tex>;
};

struct VS_IN {
	float4 pos : POSITION;
	float4 blend : BLENDWEIGHT;
	uint4 idx : BLENDINDICES;
	float3 normal : NORMAL;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};


// 頂点シェーダー
void VS(
	float4 pos : POSITION,
	float4 blend : BLENDWEIGHT,
	int4 idx : BLENDINDICES,
	float3 normal : NORMAL,
	float4 color : COLOR0,
	float2 uv : TEXCOORD0,
	out float4 get_pos : POSITION,
	out float4 get_color : COLOR0,
	out float2 get_uv : TEXCOORD0
)
{
	// vs_out定義
	float4 out_pos = (float4)0.f;

	// 行列計算結果位置
	float4 mul_pos;

	float w[4] = (float[4])blend;

	//float b_idx[4] = (float[4])idx;

	//int4 i = D3DCOLORtoUBYTE4(idx);

	//int idx_[4] = (int[4])i;

	// ワールド変換行列をブレンド

	//if (idx[2] == 3.f) {
	//	pos = 0.f;
	//	return;
	//}

	//int4 id = (int4)0;
	//
	//for (int b = 0; b < 64; b++) {
	//	for (int i = 0; i < 4; i++) {
	//
	//		if (idx[i] == b) {
	//			id = b;
	//		}
	//	}
	//}

	if (idx[0] > 60 || idx[0] <= 0) {
		return;
	}

		float4 proj_pos;

		{
			// 単位行列化
			float4x4 skin_transform = float4x4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			skin_transform += g_bone_mat[0] * blend.x;
			skin_transform += g_bone_mat[1] * blend.y;
			skin_transform += g_bone_mat[2] * blend.z;
			skin_transform += g_bone_mat[3] * blend.w;

			float4 trans_pos = mul(pos, skin_transform);
			float4 world_pos = mul(trans_pos, g_world_mat);
			float4 view_pos = mul(world_pos, g_view_mat);
			proj_pos = mul(view_pos, g_proj_mat);
		}

	{
		//	float4x4 comb = (float4x4)0;
		//	for ( int i = 0; i < 3; i++ )
		//	    comb += g_bone_mat[idx_[i]] * w[i];
		//	comb += g_bone_mat[idx_[3]] * 
		//		(1.0f - w[0] - w[1] - w[2]);
		//
		//float4 trans_pos = mul(pos,comb);
		//float4 world_pos = mul(trans_pos,g_world_mat);
		//float4 view_pos = mul(world_pos, g_view_mat);
		//float4 proj_pos = mul(view_pos,g_proj_mat);

	}
	//for (int j = 0; j < 4; j++) {
		//// 影響するボーン計算
		//mul_pos = mul(pos, g_bone_mat[idx.x]);
		//// 重みづけ(影響しないものは0にしている)
		//out_pos += mul_pos * w[b_idx[0]];
	//}


	//pos = mul(float4(pos, 1.f), comv);

	//// ワールド変換
	//out_pos = mul(out_pos, g_world_mat);
	//
	//// ビュー変換
	//out_pos = mul(out_pos,g_view_mat);
	//
	//// 射影変換
	//out_pos = mul(out_pos,g_proj_mat);


	// 外へ出力
	get_color = color;
	get_uv = uv;
	get_pos = proj_pos;

}


// ピクセルシェーダ
float4 PS(
	float4 color : COLOR0,
	float2 uv : TEXCOORD0
) : COLOR0
{
	float4 out_put;

	// テクスチャ座標
	out_put = tex2D(smp,uv);

	// カラー値掛け算
	out_put *= color;

	return out_put;
}


// テクニック
technique tech1{

	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
