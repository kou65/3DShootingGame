
// ���[���h�ϊ��s��
float4x4 mat_world : WORLD;

// �J�����r���[�ϊ��s��
float4x4 mat_camera_view : VIEW;

// �ˉe�ϊ��s��
float4x4 mat_camera_proj : PROJECTION;

// ���C�g�ڐ��̃r���[�ϊ��s��
float4x4 mat_light_view;

// ���C�g�ˉe�ϊ��s��
float4x4 mat_light_proj;

texture shadow_tex;

sampler smp = sampler_state {
	texture = (shadow_tex);
};

struct VS_OUT {
	float4 pos : POSITION;
	float4 z_calc_tex : TEXCOORD0;
	float4 col : COLOR0;
};


// ���_�V�F�[�_�[
VS_OUT DepthBufferShadowVS(
	float4 pos : POSITION,
	float3 normal : NORMAL
) {

	VS_OUT Out = (VS_OUT)0;

	// �J�����ڐ��̃��[���h�r���[�ˉe�ϊ�
	float4x4 mat;

	mat = mul(mat_world, mat_camera_view);

	mat = mul(mat, mat_camera_proj);

	Out.pos = mul(pos, mat);

	// ���C�g�ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�
	mat = mul(mat_world, mat_light_view);

	mat = mul(mat, mat_light_proj);

	Out.z_calc_tex = mul(pos, mat);

	// �@���ƃ��C�g�̕������璸�_�̐F������
	float3 n = normalize(mul(float4(normal,1.f), mat_world));

	// �g�k�𐳋K��
	float3 light_direct = normalize(float3(
		mat_light_view._13,
		mat_light_view._23,
		mat_light_view._33
		));

	Out.col = float4(0.f, 0.6f, 1.f, 1.f) *
		(0.3 + dot(n, -light_direct)*(1 - 0.3f));

	return Out;
}


float4 DepthBufferShadowPS
(float4 col : COLOR,float4 z_calc_tex : TEXCOORD) : COLOR
{

	// ���C�g�ڐ��ɂ��z�l�̍ĎZ�o
	float z_value = z_calc_tex.z / z_calc_tex.w;

// �e�N�X�`�����W�ɕϊ�
float2 trans_tex_coord;

// �e�N�X�`���v�Z
trans_tex_coord.x = (1.f + z_calc_tex.x / z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - z_calc_tex.y / z_calc_tex.w) * 0.5f;

// �������W��z�l�𒊏o
float sm_z = tex2D(smp, trans_tex_coord).x;

// �Z�o�_���V���h�E�}�b�v��z�l�����傫����Ήe�Ɣ��f
if (z_value > sm_z + 0.005f) {

	// �F�ύX
	col.rgb = col.rgb * 0.5f;
}

return col;
}


// �e�N�j�b�N
technique DepthBufferShadowTec {

	pass P0
	{
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 DepthBufferShadowPS();
	}
}