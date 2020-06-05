
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
	float4 pos_w : POSITION;// ���[���h���W
};


// �e�o�b�N�o�b�t�@�ɕԂ��p
struct PS_OUT {
	float4 col0 : COLOR0;// �m�[�}��
	float4 col1 : COLOR1;// ���t��
	float4 col2 : COLOR2;// ���Ɖe�t��
};


float4 FhoneShader(VS_OUT In) {

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

	n = normalize(In.nor_w.xyz);
	v = normalize(g_eye_pos.xyz - In.pos_w.xyz);
	l = g_pl_pos.xyz - In.pos_w.xyz;
	d = length(l);
	l = normalize(l);
	// ���ʔ��ˌv�Z
	r = 2.0f * n * dot(n, l) - l;
	a = saturate(1.0f / (g_pl_attenuate.x + g_pl_attenuate.y * d +
		g_pl_attenuate.z * d * d));

	amb = g_mt_ambient.xyz * g_ambient.xyz;
	dif = g_mt_dif.xyz * g_pl_dif.xyz;
	spe = pow(saturate(dot(r, v)), g_mt_specular.w) *
		g_specular.xyz * g_pl_specular.xyz * a;

	// �����v���ĕԂ�
	return float4(saturate(amb + dif + spe), 1.0f);
}


// ���_�V�F�[�_�[
VS_OUT DepthBufferShadowVS(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL,
	float2 uv : TEXCOORD
) {

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

	// �e�N�X�`�����W���
	Out.uv = uv;

	// ���C�g�ϊ�
	{
		// ���C�g�����œ��͂����̂ŁA���_���烉�C�g�ʒu�Ƃ��邽�߂ɋt�����ɕϊ�����
		// ���K����K���s��
		Out.l = -g_dir_light.xyz;

		// �@�����
		Out.n = normal.xyz;

		// ���C�g�x�N�g���Ɩ@���x�N�g���̓��ς��v�Z���A
		// �v�Z���ʂ̐F�̍Œ�l������(Ambient)�ɐ�������
		Out.col = min(max(g_ambient, dot(Out.n, Out.l)), 1.f);

		// ���_���王�_�ւ̃x�N�g���v�Z
		Out.e = g_eye_pos.xyz - pos.xyz;
	}

	return Out;
}


float4 DepthBufferShadowPS(
	VS_OUT In
) : COLOR0
{

	// ���C�g�ڐ��ɂ��z�l�̍ĎZ�o
	float z_value = In.z_calc_tex.z / In.z_calc_tex.w;

// �e�N�X�`�����W�ɕϊ�
float2 trans_tex_coord;

// �e�N�X�`���v�Z
trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

// �������W��z�l�𒊏o
float sm_z = tex2D(smp, trans_tex_coord).x;

FhoneShader(In);

float bias = 0.005f;

// �Z�o�_���V���h�E�}�b�v��z�l�����傫����Ήe�Ɣ��f
if (z_value > sm_z + bias) {

	// �F�ύX
	In.col.rgb = In.col.rgb * 0.5f;
}

return In.col;
}


float4 TextureDepthBufferShadowPS(
	float4 col : COLOR0,
	float4 z_calc_tex : TEXCOORD0,
	float2 uv : TEXCOORD1,
	float3 n : TEXCOORD2,// �I�u�W�F�N�g�̖@���x�N�g��
	float3 l : TEXCOORD3,// ���_���烉�C�g�ʒu�ւ̃x�N�g��
	float3 e : TEXCOORD4// ���_���王�_�ւ̃x�N�g��
) : COLOR0
{

	// ���C�g�ڐ��ɂ��z�l�̍ĎZ�o
	float z_value = z_calc_tex.z / z_calc_tex.w;

// �e�N�X�`�����W�ɕϊ�
float2 trans_tex_coord;

// �e�N�X�`���v�Z
trans_tex_coord.x = (1.f + z_calc_tex.x / z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - z_calc_tex.y / z_calc_tex.w) * 0.5f;

// �������W��z�l�𒊏o
float sm_z = tex2D(smp,trans_tex_coord).x;

// �e�N�X�`���l�v�Z
float4 tex = tex2D(tex_smp, uv);

// �e�N�X�`���l���
col = tex;

// �o�C�A�X���|����
float bias = 0.005f;

// �Z�o�_���V���h�E�}�b�v��z�l�����傫����Ήe�Ɣ��f
if (z_value > sm_z + bias) {

	// �F�ύX
	col.rgb = col.rgb * 0.5f;
}

return col;

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
}