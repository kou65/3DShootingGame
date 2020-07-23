
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
	AddRessU = WRAP;
	AddRessV = WRAP;
};

sampler tex_smp = sampler_state {
	texture = (g_tex);
	AddRessU = WRAP;
	AddRessV = WRAP;
};


struct LIGHT_OUT {

	// ��
	float4 ambient : TEXCOORD5;
	float4 specular : TEXCOORD6;
	float4 diffuse : TEXCOORD7;
};

// ���_�Ԃ��p
struct VS_OUT {
	float4 pos : POSITION;
	float4 z_calc_tex : TEXCOORD0;
	float4 col : COLOR0;
	float2 uv : TEXCOORD1; // w=�e�̔Z��
	float4 nor_w : TEXCOORD2;
	float4 pos_w : TEXCOORD3;

	// ���C�g����
	float4 L : TEXCOORD4;
	float3 V : TEXCOORD5;
	float3 H : TEXCOORD6;
	float3 normal : TEXCOORD7;
};


// �e�o�b�N�o�b�t�@�ɕԂ��p
struct PS_OUT {
	float4 col0 : COLOR0;// �m�[�}��
	float4 col1 : COLOR1;// ���t��
	float4 col2 : COLOR2;// ���Ɖe�t��
};

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
	//Out.col = float4(0.f, 0.6f, 1.f, 1.f) * 
	//	(0.3f + dot(n, -light_direct)*(1 - 0.2f));

	Out.pos_w = mul(pos, g_world);
	Out.nor_w = mul(normal.xyz, g_world);

	// �ʒu����̃��C�g�������o��
	float3 L = normalize(g_pl_pos.xyz - Out.pos_w.xyz);

	// �g�U����
	{
		// �e�N�X�`�����W���
		Out.uv = uv;
	
		// �J���[�l
		Out.col = float4(1.f, 1.f, 1.f, 1.f);
	
		float3 nor_w = mul(normal.xyz, g_world);
	
		// ����
		nor_w.xyz = normalize(Out.nor_w.xyz);

		float i = saturate(dot(L, nor_w));

		// �g�U���˂̌v�Z��VS�ōs��
		float3 diffuse = i * g_mt_diffuse.xyz * g_pl_diffuse.xyz;

		Out.col = Out.col * 
			float4(diffuse.x, diffuse.y, diffuse.z, 1.f);
	}

	// ���ʔ���
	{
		float3 l = L.xyz;
		float3 eye = normalize(g_eye_pos - Out.pos_w);
		float3 H = normalize(l + eye);

		Out.L = float4(L.x,L.y,L.z,0.f);
		Out.V = eye;
		Out.H = H;
		Out.normal = normal;
		
	}

	// ��
	{
		// �ʂ̐F * ���P�x
		Out.col = (Out.col * g_mt_ambient * g_ambient);
	}

	return Out;
}


float4 PS(VS_OUT In) : COLOR0{
	// ����
{
	//In.col = In.col *  max(0.3f,dot(nor_w,-g_light_dir))
}
	return In.col;
}


float4 TextureDepthBufferShadowPS(
	VS_OUT In
) : COLOR0
{
	float4 recolor = (float4)0.f;

	// ���C�g�ڐ��ɂ��z�l�̍ĎZ�o
	float z_value = In.z_calc_tex.z / In.z_calc_tex.w;

// �e�N�X�`�����W�ɕϊ�
float2 trans_tex_coord;

// �e�N�X�`�����W�ϊ��v�Z
trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

// �������W��z�l�𒊏o
float sm_z = tex2D(smp,trans_tex_coord).x;

// �e�N�X�`���l�v�Z
float4 tex = tex2D(tex_smp,In.uv);

// �u�����t�H�����˃��f��
In.col = In.col * max(1.f, dot(In.normal, -In.L))
+ pow(max(0, dot(In.normal, In.H)), 10);

//float i = pos(saturate(dot(In.H, In.N)), g_mt_specular.w);

// ���ʔ���
//In.col = float4(i * g_mt_specular.xyz * g_light_color.xyz, 1.f);

// �J���[
recolor = In.col * tex;

// �o�C�A�X���|����
float bias = 0.005f;

// �Z�o�_���V���h�E�}�b�v��z�l�����傫����Ήe�Ɣ��f
if (z_value > sm_z + bias) {

	// �F�ύX
	recolor.rgb = recolor.rgb * 0.5f;
}

return recolor;
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

	// �ˉe��Ԃ�xy���W���e�N�X�`�����W�ɕϊ�
	trans_tex_coord.x = (1.f + In.z_calc_tex.x / In.z_calc_tex.w) * 0.5f;
	trans_tex_coord.y = (1.f - In.z_calc_tex.y / In.z_calc_tex.w) * 0.5f;

	// �������W��z�l�𒊏o
	float sm_z = tex2D(smp,trans_tex_coord).x;

	// �o�C�A�X���|����
	float bias = 0.005f;

	// �J���[�l���Z
	recolor = In.col;

	//// �Z�o�_���V���h�E�}�b�v��z�l�����傫����Ήe�Ɣ��f
	//if (z_value > sm_z + bias) {
	//
	//	// �F�ύX
	//	recolor.rgb = In.col.rgb * 0.5f;
	//}

	return recolor;
}

// �e�N�j�b�N
technique DepthBufferShadowTec {

	// �e�N�X�`������
	pass P0
	{
		AlphaBlendEnable = false;
		
		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 DepthBufferShadowPS();
	}

	// �e�N�X�`���L
	pass P1
	{
		AlphaBlendEnable = false;

		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 TextureDepthBufferShadowPS();
		//PixelShader = compile ps_2_0 PS();
	}

}