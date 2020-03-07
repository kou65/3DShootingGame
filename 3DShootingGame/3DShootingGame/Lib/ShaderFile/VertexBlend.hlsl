

// グローバル定数
// ビュー行列
float4x4 g_view_mat : register(c0);

// 射影行列
float4x4 g_proj_mat : register(c4);

// ワールド行列
float4x4 g_world_mat : register(c8);

// ボーン姿勢マトリックス
float4x4 g_bone_mat[50] : register(c12);

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
	float3 normal : NORMAL;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	uint4 index : BLENDINDICES;
	float4 blend : BLENDWEIGHT;
};

struct VS_OUT {
	float4 get_pos : POSITION;
	float4 get_color : COLOR0;
	float2 get_uv : TEXCOORD;
};

// 頂点シェーダー
VS_OUT VS(
	VS_IN In
)
{

	int4 index_vec = D3DCOLORtoUBYTE4(In.index);

	int  IndexArray[4] = (int[4])index_vec;

	// vs_out定義
	float4 out_pos = (float4)0.f;

	// 行列計算結果位置
	float4 mul_pos;

	float w[4] = (float[4])In.blend;

	VS_OUT out2 = (VS_OUT)0.f;

	// 応急措置
	{
		for (int i = 0; i < 4; i++) {

			if (In.index[i] > 49) {
				return out2;
			}
		}
	}

		float4 proj_pos;

		{
			// 単位行列化
			float4x4 skin_transform = float4x4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);


			skin_transform += g_bone_mat[In.index[0]];// * In.blend.x;
			skin_transform += g_bone_mat[In.index[1]];// * In.blend.y;
			skin_transform += g_bone_mat[In.index[2]];// * In.blend.z;
			skin_transform += g_bone_mat[In.index[3]];// * In.blend.w;

			float4 trans_pos = mul(In.pos, skin_transform);
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

	VS_OUT Out;

	// 外へ出力
	Out.get_color = In.color;
	Out.get_uv = In.uv;
	Out.get_pos = proj_pos;

	return Out;
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
