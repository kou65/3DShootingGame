
// ���[���h�ϊ��s��
float4x4 g_world : WORLD;

// �J�����r���[�ϊ��s��
float4x4 g_view : VIEW;

// �ˉe�ϊ��s��
float4x4 g_proj : PROJECTION;

// ���[���h�J�����ˉe�ϊ��s��(�O������)
float4x4 g_wvp : WVP;

// ���C�g�ڐ��̃r���[�ϊ��s��
float4x4 g_light_view;

// ���C�g�ˉe�ϊ��s��
float4x4 g_light_proj;

// �V���h�E�e�N�X�`��
texture g_shadow_tex;

// �e�N�X�`��
texture g_tex;

// �J���[
float4 g_color;


/*--- ���C�g�n ---*/

/* �}�e���A�� */
// ����
float4 g_ambient;
// �}�e���A���̊���
float4 g_mt_ambient;
// �g�U��
float4 g_mt_diffuse;
// ���ˌ�
float4 g_mt_specular;


/* ���̑� */
// ���_���W(���K���ς�)
float4 g_eye_dir;
// �J�������W
float4 g_eye_pos;
// ���C�g����(���s����)
float4 g_light_dir;
// ���C�g�J���[
float4 g_light_color;

/* �_����(pl = point_light) */
// ���W
float4 g_pl_pos;
// �g�U
float4 g_pl_diffuse;
// ����
float4 g_pl_specular;
// ����
float4 g_pl_attenuate;


// �e�N�X�`���T���v��
sampler smp = sampler_state {
	texture = (g_shadow_tex);
};

sampler tex_smp = sampler_state {
	texture = (g_tex);
};

// ���_�Ԃ��p
struct VS_OUT {
	float4 pos : POSITION;
	float4 z_calc_tex : TEXCOORD0;
	float4 col : COLOR0;
	float2 uv : TEXCOORD1; // w=�e�̔Z��

	// ���C�g�p
	float4 nor_w : TEXCOORD2;// ���[���h�@���x�N�g��
	float4 pos_w : TEXCOORD3;// ���[���h���W
};


// �e�o�b�N�o�b�t�@�ɕԂ��p
struct PS_OUT {
	float4 col0 : COLOR0;// �m�[�}��
	float4 col1 : COLOR1;// ���t��
	float4 col2 : COLOR2;// ���Ɖe�t��
};


float4 FhoneShaderPS(VS_OUT In) : COLOR0{

	// �@��
	float n;
	float v;
	float l;
	float r;
	float d;
	float a;
	// ���A���ʁA����
	float3 amb;
	float3 dif;
	float3 spe;

	// ���[���h�@�����K��
	n = normalize(In.nor_w.xyz);

	// ���̃I�u�W�F�N�g�Ԃ̕������o��
	v = normalize(g_eye_pos.xyz - In.pos_w.xyz);

	// �_�������烏�[���h�ʒu�܂�
	l = g_pl_pos.xyz - In.pos_w.xyz;

	// �������o��
	d = length(l);

	// pl����w�܂ŕ������o��
	l = normalize(l);

	// ���ʔ��ˌv�Z
	r = 2.0f * n * dot(n, l) - l;

	// ���ʌv�Z
	a = saturate(1.0f / (g_pl_attenuate.x + g_pl_attenuate.y * d +
		g_pl_attenuate.z * d * d));

	// ����
	amb = g_mt_ambient.xyz * g_ambient.xyz;

	// �g�U��
	dif = saturate(dot(l,n)) * g_mt_diffuse.xyz * g_pl_diffuse.xyz;

	// ���ˌ�
	spe = pow(saturate(dot(r, v)), g_mt_specular.w) * 
		g_mt_specular.xyz * g_pl_specular.xyz * a;

	// �����v���ĕԂ�
	return float4(saturate(amb + dif + spe), 1.0f);
}


VS_OUT FhoneShaderVS(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL,
	float2 uv : TEXCOORD
) {

	VS_OUT Out = (VS_OUT)0;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.pos = mul(pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);


	Out.uv = uv;
	Out.pos_w = pos;
	Out.nor_w = float4(normal.x,normal.y,normal.z,1.0f);

	// �F��Z
	Out.col = color;
	Out.col *= FhoneShaderPS(Out);
	

	return Out;
}


float4 FhoneShaderPS2(VS_OUT In) : COLOR0{

	return In.col;
}


// �o�[�e�b�N�X
VS_OUT DepthBufferShadowVS(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL,
	float2 uv : TEXCOORD
) {

	// ������
	VS_OUT Out = (VS_OUT)0;

	// �J�����ڐ��̃��[���h�r���[�ˉe�ϊ�
	float4x4 mat;
	mat = mul(mul(g_world, g_view), g_proj);
	Out.pos = mul(pos, mat);

	// ���C�g�ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�
	mat = mul(g_world, g_light_view);
	mat = mul(mat, g_light_proj);
	Out.z_calc_tex = mul(pos, mat);

	// �@���ƃ��C�g�̕������璸�_�̐F������
	float3 n = normalize(mul(float4(normal,1.f), g_world).xyz);

	// ���C�g�s��ʒu�𐳋K��(�]�u�s��)
	float3 light_direct = normalize(float3(
		g_light_view._13,
		g_light_view._23,
		g_light_view._33
	));

	// �ŏI�I�ȃJ���[���o��
	Out.col = float4(0.f, 0.6f, 1.f, 1.f) * 
		(0.3f + dot(n, -light_direct)*(1 - 0.3f));

	Out.nor_w = float4(normal.x,normal.y,normal.z,1.f);
	Out.pos_w = float4(pos.x,pos.y,pos.z,1.f);

	// �e�N�X�`�����W���
	Out.uv = uv;

	// �F��Z
	Out.col = color;

	// ���C�g�f�[�^������Ă���
	Out.col *= FhoneShaderPS(Out);

	return Out;
}


// �s�N�Z��
float4 DepthBufferShadowPS(
	VS_OUT In
) : COLOR0
{

	// �J���[��Ԃ�
	float4 recolor = 0.f;
	// ���C�g�ڐ��ɂ��z�l�̍ĎZ�o
	float z_value = In.z_calc_tex.z / In.z_calc_tex.w;

// �e�N�X�`�����W�ɕϊ�
float2 trans_tex_coord;

// �e�N�X�`���v�Z
trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

// �������W��z�l�𒊏o
float sm_z = tex2D(smp, trans_tex_coord).x;

// �o�C�A�X���|����
float bias = 0.005f;

// �J���[�l���Z
recolor = In.col;

// �Z�o�_���V���h�E�}�b�v��z�l�����傫����Ήe�Ɣ��f
if (z_value > sm_z + bias) {

	// �F�ύX
	recolor.rgb = In.col.rgb * 0.5f;
}

return recolor;
}


float4 TextureDepthBufferShadowPS(
	VS_OUT Out
) : COLOR0
{
	float4 recolor = 0;

	// ���C�g�ڐ��ɂ��z�l�̍ĎZ�o
	float z_value = Out.z_calc_tex.z / Out.z_calc_tex.w;

// �e�N�X�`�����W�ɕϊ�
float2 trans_tex_coord;

// �e�N�X�`�����W�ϊ��v�Z
trans_tex_coord.x = (1.f + Out.z_calc_tex.x / Out.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - Out.z_calc_tex.y / Out.z_calc_tex.w) * 0.5f;

// �������W��z�l�𒊏o
float sm_z = tex2D(smp,trans_tex_coord).x;

// �e�N�X�`���l�v�Z
float4 tex = tex2D(tex_smp, Out.uv);

// �J���[�l���Z
recolor = Out.col;

// �e�N�X�`���l���Z
recolor += tex;

// �o�C�A�X���|����
float bias = 0.005f;

// �Z�o�_���V���h�E�}�b�v��z�l�����傫����Ήe�Ɣ��f
if (z_value > sm_z + bias) {

	// �F�ύX
	recolor.rgb = Out.col.rgb * 0.5f;
}

return recolor;

}


// �V���h�E�}�b�v���\�t�g�V���h�E�ɕϊ�
float4 SoftShadowVS(
	float4 pos : POSITION,
float4 NORMAL : NORMAL,
float2 tex : TEXCOORD0
) {

	
}


// �e�N�j�b�N
technique DepthBufferShadowTec {

	// �e�N�X�`������
	pass P0
	{
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 DepthBufferShadowPS();
	}

	// �e�N�X�`���L
	pass P1
	{
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 TextureDepthBufferShadowPS();
	}

	// �t�H���V�F�[�_�[
	pass P2
	{
		VertexShader = compile vs_2_0 FhoneShaderVS();
		PixelShader = compile ps_2_0 FhoneShaderPS();
	}

	pass P3
	{
		VertexShader = compile vs_2_0 FhoneShaderVS();
		PixelShader = compile ps_2_0 FhoneShaderPS2();
	}
}