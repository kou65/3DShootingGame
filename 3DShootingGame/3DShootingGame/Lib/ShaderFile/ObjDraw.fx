
// �O���[�o���萔
// �r���[�s��
float4x4 g_view_mat : VIEW;

// �ˉe�s��
float4x4 g_proj_mat : PROJECTION;

// ���[���h�s��
float4x4 g_world_mat : WORLD;

// �J���[
float4 g_color;

// �g�p����e�N�X�`��
texture g_tex;

// �T���v���u���b�N
sampler smp = sampler_state {
	texture = <g_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
	// CLAMP
	// BORDER
};


// ���_�V�F�[�_�[
void VS(
float3 pos : POSITION,
float3 normal : NORMAL,
float4 color : COLOR0,
float2 uv : TEXCOORD0,
	out float4 get_pos : POSITION,
	out float4 get_color : COLOR0,
	out float2 get_uv : TEXCOORD0
)
{
	// �o�͗p
	float4x4 mat;

	// ���[���h�ϊ�
	mat = mul(mul(g_world_mat,g_view_mat),g_proj_mat);

	float4 out_pos = mul(float4(pos,1.0f),mat);

	// �O�֏o��
	get_color = color;// + g_color;
	get_uv = uv;
	get_pos = out_pos;
}



float4 PS1(
	float4 color : COLOR0,
	float2 uv : TEXCOORD0
) : COLOR0
{
	float4 out_put;

// �e�N�X�`�����W * �J���[�l
out_put = color;

return out_put;
};


// �s�N�Z���V�F�[�_
float4 PS2(
	float4 color : COLOR0,
	float2 uv : TEXCOORD0
) : COLOR0
{

float4 out_put;

// �e�N�X�`�����W * �J���[�l
out_put = tex2D(smp, uv);

// �J���[�l�|���Z
out_put *= color;

return out_put;
};


// �e�N�j�b�N
technique tech1 {

	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS1();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();
	}
}
