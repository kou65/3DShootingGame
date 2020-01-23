

// �O���[�o���萔
// �r���[�s��
float4x4 g_view_mat;

// �ˉe�s��
float4x4 g_proj_mat;

// ���[���h�s��
float4x4 g_world_mat;

// �{�[���p���}�g���b�N�X
float4x4 g_bone_mat[64];

// �A�j���[�V�����}�g���b�N�X
//float4x4 g_anim_mat[64];


// �g�p����e�N�X�`��
texture g_tex;

sampler smp = sampler_state {
	texture = <g_tex>;
};


// �o�͗p
struct VS_IN {
	float4 pos : POSITION;
	float4 uv : TEXCOORD0;
};

struct VS_OUT {
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
};


// ���_�V�F�[�_�[
VS_OUT VS(
	float4 pos : POSITION,
	float4 normal : NORMAL,
	float4 color : COLOR0,
	float4 uv : TEXCOORD,
	float4 blend : BLENDWEIGHT,
	int4 blend_idx : BLENDINDICES
)
{

	// �d�݂��󂯎��
	float w[4] = (float[4])blend;

float total_blend_weight = 0.f;
float4x4 mat_comb = 0.f;

// ���[���h�ϊ��s����u�����h
//for (int i = 0; i < 4; i++) {
//
//	// �Ō�̏d�݂��v�Z
//	total_blend_weight += w[i];
//	
//	// �}�g���b�N�X
//	mat_comb += g_bone_mat[blend_idx] * w[i]
//
//
//}

	// vs_out��`
	float4 out_pos;

	// ���[���h�ϊ�
	out_pos = mul(pos,g_world_mat);

	// �r���[�ϊ�
	out_pos = mul(out_pos,g_view_mat);

	// �ˉe�ϊ�
	out_pos = mul(out_pos,g_proj_mat);
	
	// ���_�V�F�[�_�[�̖߂�l
	VS_OUT vs_out = (VS_OUT)0;

	vs_out.pos = out_pos;

	vs_out.uv = uv;

	return vs_out;
}


// �s�N�Z���V�F�[�_
float4 PS(
	VS_OUT vs_out
) : COLOR0
{

	return tex2D(smp,vs_out.uv) * vs_out.color;
}


// �e�N�j�b�N
technique tech1{

	pass blend
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
