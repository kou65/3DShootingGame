

// ���s�������C�e�B���O(�f�B���N�V���i�����C�g)

float4x4 g_world : WORLD;
float4x4 g_proj : PROJECTION;
float4x4 g_view : VIEW;

/* �O���O���[�o�� */
// ���C�g����
float4 g_dir_light;

// �_����
float4 g_point_light_pos;

// ���邳���������p�����[�^
float4 g_attenuation;

// ���C�g�J���[
float4 g_light_color;

// �J���[
float4 g_color;

// ���_���W
float4 g_eye_dir;

// �J�������W
float4 g_eye_pos;

// ���C�g�r���[
float4x4 g_light_view;

// ���C�g�ˉe
float4x4 g_light_proj;

// �g�p����e�N�X�`��
texture g_tex;

// �V���h�E�}�b�v
texture g_shadow_tex;



// �T���v���u���b�N
sampler smp = sampler_state {
	texture = <g_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
	// CLAMP
	// BORDER
};

// ���_�f�[�^�ɖ@��������O��

struct VS_IN {
	float4 pos : POSITION0;
	float4 normal : NORMAL;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};


struct VS_OUT {
	float4 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float4 normal : TEXCOORD1;
};

struct VS_OUT_WORLD {
	float4 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;

	// ���[���h���W�̃s�N�Z��
	float4 normalw : TEXCOORD1;
	float4 posw : TEXCOORD2;
};

struct VS_OUT_SHADOW {
	float4 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float4 normal : TEXCOORD1;
	float4 z_calc_tex : TEXCOORD2;
};


// ���s����
VS_OUT DirectionalLightVS(VS_IN In) {

	VS_OUT Out;
	float4 nor;
	float col;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	nor = mul(In.normal, g_world).xyzw;
	// ���K��
	nor = normalize(nor);

	// ����t����
	col = saturate(dot(nor,g_dir_light));

	// �P�x
	float luminance = 0.5f;

	col = col * 0.5f + luminance;

	// �O���[�o���J���[�l�ύX
	col *= g_color;

	Out.color = float4(col, col, col, 1.f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}

// �g�U����
VS_OUT DiffuseReflectionVS(VS_IN In) {

	VS_OUT Out;

	float4 light_nor_vec;
	float4 normal_nor_vec;
	float col;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	// �s�N�Z�����W�ƌ����̍��W��������֌��������K���x�N�g�������߂�
	// �������W����s�N�Z�����W�������A�������x�N�g��(������)�𐳋K������
	light_nor_vec = normalize(g_point_light_pos.xyzw - In.pos.xyzw);

	// �@���x�N�g�����K��
	normal_nor_vec = normalize(In.normal.xyzw);

	// -�܂ōs������0�ɖ߂��悤�ɂ���,���ςŊp�x�����߂�
	col = saturate(dot(light_nor_vec, normal_nor_vec));

	Out.color = float4(col * 0.5f * 0.5f,col ,col,1.0f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// ���ʔ���
VS_OUT SpecularReflectionVS(VS_IN In) {

	VS_OUT Out;

	// ���K���ς݃��C�g����s�N�Z�����W�ւ̕���
	float4 dir_light_nor;

	// ���K���ςݖ@��
	float4 normal_nor;

	// �����˃x�N�g��
	float4 ref_vec;

	// ���_���W����s�N�Z���ʒu�̕���
	float4 view_point;

	// �v�Z��̌���
	float col;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	// �_��������s�N�Z���ʒu�܂ł̒P�ʃx�N�g���擾
	dir_light_nor = normalize(g_point_light_pos.xyzw - In.pos.xyzw);

	// �@���x�N�g�����K��
	normal_nor = normalize(In.normal);

	// ���˃x�N�g��
	ref_vec = 2.0f * normal_nor * dot(normal_nor, dir_light_nor) - dir_light_nor;

	// ���_���W����s�N�Z���ʒu�̒P�ʃx�N�g���擾
	view_point = normalize(g_eye_dir.xyzw - In.pos.xyzw);

	// �P�x
	float specular = 0.5f;

	// ���˂Ǝ����x�N�g���̓��ς��擾�A����x���W��a�łׂ��悷��
	col = pow(saturate(dot(ref_vec, view_point)),specular);

	Out.color = float4(col *g_color.xyz * specular, 1.0f);
	//Out.col = float4(col * 0.5f * 0.5f, col, col, 1.0f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// ������(���E�������������邭����)
VS_OUT AmbientReflectionVS(VS_IN In) {

	VS_OUT Out;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	float4 col;

	col = g_color.xyzw * g_light_color.xyzw;

	Out.color = col;
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// �_�������C�e�B���O
VS_OUT PointLightVS(VS_IN In) {

	VS_OUT Out;
	float4 nor;
	float col;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	// �_��������
	float4 point_light_dir;
	// �_�����܂ł̋���
	float point_light_len;
	// �g�U
	float col_dif;
	// ��
	float col_a;
	// ���v
	float total_col;


	// �_�����܂ł̕����Ƒ傫��
	point_light_dir = g_point_light_pos - In.pos;

	// �_�����̋������o��
	point_light_len = length(point_light_dir);

	// �_�����̕����𐳋K��
	point_light_dir = point_light_dir / point_light_len;

	// �g�U����
	col_dif = saturate(dot(normalize(In.normal), point_light_dir));

	float4 atten = g_attenuation;

	// �����ɂ���Č�������������(������2��ɔ���Ⴗ��A���`���g���Č���������)
	col_a = saturate(1.0f / (atten.x + atten.y * point_light_len
		+ atten.z * point_light_len * point_light_len));

	// �����v�Z
	total_col = col_dif * col_a;

	Out.color = float4(total_col, total_col, total_col, 1.0f);
	Out.uv = In.uv;
	Out.normal = In.normal;

	return Out;
}


// �t�H�����˃��f��(�Ǐ��Ɩ����f��)
VS_OUT_WORLD PhongReflectionVS(VS_IN In) {

	VS_OUT_WORLD Out;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.pos = mul(In.pos, g_world);
	Out.pos = mul(Out.pos, g_view);
	Out.pos = mul(Out.pos, g_proj);

	Out.color = In.color;
	Out.uv = In.uv;
	Out.posw = In.pos;
	Out.normalw = In.normal;

	return Out;
}



// �ʏ�s�N�Z���V�F�[�_�[
float4 ColorPS(VS_OUT In) : COLOR0{
	
return In.color;
}
float4 TextureColorPS(VS_OUT In) : COLOR0 {

	float4 out_put;

	// �e�N�X�`�����W * �J���[�l
	out_put = tex2D(smp, In.uv);

	out_put *= In.color;

	return out_put;
}


// �t�H�����˃s�N�Z���V�F�[�_
float4 PhongReflectionPS(VS_OUT_WORLD In) : COLOR0 {

	float4 normal;
	float4 eye_pixel_dir;
	float4 p_light_pixel_dir;
	float4 len;
	float ref;
	float col_a;

	// ����
	float4 ambient;
	float4 diffuse;
	float4 specular;

	// �@���𐳋K��
	normal = normalize(In.normalw.xyzw);

	// ���_����s�N�Z���܂ł̕������K��
	eye_pixel_dir = normalize(g_eye_dir - In.posw);

	// �_��������s�N�Z���܂ł̕���
	p_light_pixel_dir = g_point_light_pos - In.posw;

	// �_�����܂ł̋���
	len = length(p_light_pixel_dir);

	// �����𐳋K�����ĕ��������擾
	p_light_pixel_dir = normalize(p_light_pixel_dir);

	// ���ʔ���
	ref = 2.0 * normal * dot(normal, p_light_pixel_dir) - p_light_pixel_dir;

	float4 atten = g_attenuation;

	// �����ɂ���Č�������������(������2��ɔ���Ⴗ��A���`���g���Č���������)
	col_a = saturate(1.0f / (atten.x + atten.y * p_light_pixel_dir
		+ atten.z * p_light_pixel_dir * p_light_pixel_dir));

	// ��
	ambient = 0.1f;//g_color * In.color;

	// �g�U����
	diffuse = saturate(dot(normal, p_light_pixel_dir));

	float specular_light = 0.2f;

	// ���ʔ���
	specular = pow(saturate(dot(ref,eye_pixel_dir)), specular_light)
		* specular_light * specular_light * col_a;

	// ���C�g�J���[
	float light_col = saturate(
		ambient + diffuse + specular_light);

	// �t�H�����˂�Ԃ�
	return float4(
		light_col * g_color.x,
		light_col * g_color.y,
		light_col * g_color.z, 1.0);
}



// ���_�V�F�[�_�[
VS_OUT_SHADOW DepthBufferShadowVS(
	float4 pos : POSITION,
	float3 normal : NORMAL
) {

	VS_OUT_SHADOW Out = (VS_OUT_SHADOW)0;

	// �J�����ڐ��̃��[���h�r���[�ˉe�ϊ�
	float4x4 mat;

	mat = mul(g_world, g_view);
	mat = mul(mat, g_proj);
	Out.pos = mul(pos, mat);

	// ���C�g�ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�
	mat = mul(g_world, g_light_view);
	mat = mul(mat, g_light_proj);
	Out.z_calc_tex = mul(pos, mat);

	// �@���ƃ��C�g�̕������璸�_�̐F������
	float3 n = normalize(mul(float4(normal, 1.f), g_world));

	// �g�k�𐳋K��
	float3 light_direct = normalize(float3(
		g_light_view._13,
		g_light_view._23,
		g_light_view._33
		));

	Out.color = float4(0.f, 0.6f, 1.f, 1.f) *
		(0.3 + dot(n, -light_direct)*(1 - 0.3f));

	return Out;
}


float4 DepthBufferShadowPS
(float4 col : COLOR, float4 z_calc_tex : TEXCOORD2) : COLOR
{

	// ���C�g�ڐ��ɂ��z�l�̍ĎZ�o
	float z_value = z_calc_tex.z / z_calc_tex.w;

// �e�N�X�`�����W�ɕϊ�
float2 trans_tex_coord;

// �e�N�X�`���v�Z(�ˉe��Ԃ���e�N�X�`���ɕϊ�)
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
technique tech1 {


	// ���s����
	// �J���[����
	pass DirectionalLightColor
	{
		VertexShader = compile vs_2_0 DirectionalLightVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	// �e�N�X�`���t��
	pass DirectionalLightTexture
	{
		VertexShader = compile vs_2_0 DirectionalLightVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}


	// �g�U����
	// �J���[����
	pass DiffuseReflectionColor
	{
		VertexShader = compile vs_2_0 DiffuseReflectionVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	// �e�N�X�`���t��
	pass DiffuseReflectionTexture
	{
		VertexShader = compile vs_2_0 DiffuseReflectionVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}

	// ���ʔ���
	// �J���[�t��
	pass SpecularReflectionColor
	{
		VertexShader = compile vs_2_0 SpecularReflectionVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	// �e�N�X�`���t��
	pass SpecularReflectionTexture
	{
		VertexShader = compile vs_2_0 SpecularReflectionVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}

	// �_�������C�e�B���O
	// �J���[
	pass PointLightColor
	{
		VertexShader = compile vs_2_0 PointLightVS();
		PixelShader = compile ps_2_0 ColorPS();
	}

	pass PointLightTexture
	{
		VertexShader = compile vs_2_0 PointLightVS();
		PixelShader = compile ps_2_0 TextureColorPS();
	}


	// �t�H������
	pass FhoneRef {

		VertexShader = compile vs_2_0 PhongReflectionVS();
		PixelShader = compile ps_2_0 PhongReflectionPS();
	}
	

	// �e
	pass Shade {

		VertexShader = compile vs_2_0 DepthBufferShadowVS();
		PixelShader = compile ps_2_0 DepthBufferShadowPS();
	}

}