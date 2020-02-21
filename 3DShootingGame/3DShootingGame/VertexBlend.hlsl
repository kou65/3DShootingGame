

// �O���[�o���萔
// �r���[�s��
float4x4 g_view_mat : VIEW;

// �ˉe�s��
float4x4 g_proj_mat : PROJECTION;

// ���[���h�s��
float4x4 g_world_mat : WORLD;

// �{�[���p���}�g���b�N�X
float4x4 g_bone_mat[64];

// �u�����h����z��̐�
int g_max_blend_idx;

// �g�p����e�N�X�`��
texture g_tex;

// �T���v���u���b�N
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


// ���_�V�F�[�_�[
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
	// vs_out��`
	float4 out_pos = (float4)0.f;

	// �s��v�Z���ʈʒu
	float4 mul_pos;

	float w[4] = (float[4])blend;

	//float b_idx[4] = (float[4])idx;

	//int4 i = D3DCOLORtoUBYTE4(idx);

	//int idx_[4] = (int[4])i;

	// ���[���h�ϊ��s����u�����h

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
			// �P�ʍs��
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
		//// �e������{�[���v�Z
		//mul_pos = mul(pos, g_bone_mat[idx.x]);
		//// �d�݂Â�(�e�����Ȃ����̂�0�ɂ��Ă���)
		//out_pos += mul_pos * w[b_idx[0]];
	//}


	//pos = mul(float4(pos, 1.f), comv);

	//// ���[���h�ϊ�
	//out_pos = mul(out_pos, g_world_mat);
	//
	//// �r���[�ϊ�
	//out_pos = mul(out_pos,g_view_mat);
	//
	//// �ˉe�ϊ�
	//out_pos = mul(out_pos,g_proj_mat);


	// �O�֏o��
	get_color = color;
	get_uv = uv;
	get_pos = proj_pos;

}


// �s�N�Z���V�F�[�_
float4 PS(
	float4 color : COLOR0,
	float2 uv : TEXCOORD0
) : COLOR0
{
	float4 out_put;

	// �e�N�X�`�����W
	out_put = tex2D(smp,uv);

	// �J���[�l�|���Z
	out_put *= color;

	return out_put;
}


// �e�N�j�b�N
technique tech1{

	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
