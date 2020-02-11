

// �O���[�o���萔
// �r���[�s��
float4x4 g_view_mat : VIEW;

// �ˉe�s��
float4x4 g_proj_mat : PROJECTION;

// ���[���h�s��
float4x4 g_world_mat;

// �{�[���p���}�g���b�N�X
float4x4 g_bone_mat[64] : WORLD;

// ���݂̃A�j���[�V������
int current_anim_num;

// �g�p����e�N�X�`��
texture g_tex;

// �T���v���u���b�N
sampler smp = sampler_state {
	texture = <g_tex>;
};


// ���_�V�F�[�_�[
void VS(
	float3 pos : POSITION,
	float3 normal : NORMAL,
	float4 color : COLOR0,
	float2 uv : TEXCOORD0,
	float4 blend : BLENDWEIGHT,
	int4 idx : BLENDINDICES,
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

	//// ���[���h�ϊ��s����u�����h
	//for(int i = 0; i < 4; i++){
	//
	//	// �e������{�[���v�Z
	//	mul_pos = mul(float4(pos,1.f),g_bone_mat[idx[i]]);
	//
	//	// �d�݂Â�(�e�����Ȃ����̂�0�ɂ��Ă���)
	//	out_pos += mul_pos * w[i];
	//}

	// ���[���h�ϊ��s����u�����h
	for(int i = 0; i < 4; i++){
	
		int b = idx[i];
	
		// �e������{�[���v�Z
		mul_pos = mul(float4(pos,1.f),g_bone_mat[b]);
	
		// �d�݂Â�(�e�����Ȃ����̂�0�ɂ��Ă���)
		out_pos += mul_pos * w[i];
	}

	//float4x4 comv = (float4x4)0;
	//
	//for (int i = 0; i < 4; i++) {
	//	comv += g_bone_mat[idx[i]] * w[i];
	//	comv += g_bone_mat[idx[3]] * (1.f - w[0] - w[1] - w[2] - w[3]);
	//}
	//
	//pos = mul(float4(pos, 1.f), comv);

	// ���[���h�ϊ�
	out_pos = mul(out_pos, g_world_mat);

	// �r���[�ϊ�
	out_pos = mul(out_pos,g_view_mat);

	// �ˉe�ϊ�
	out_pos = mul(out_pos,g_proj_mat);


	// �O�֏o��
	get_color = color;
	get_uv = uv;
	get_pos = out_pos;

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
