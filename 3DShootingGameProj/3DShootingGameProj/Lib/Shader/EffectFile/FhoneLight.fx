
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

	Out.col = color;
	Out.uv = uv;
	Out.pos_w = pos;
	Out.nor_w = float4(normal.x, normal.y, normal.z, 1.0f);

	return Out;
}


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

