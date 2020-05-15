
float4x4 g_world : WORLD;
float4x4 g_view : VIEW;
float4x4 g_proj : PROJECTION;

float4 g_light_dir;
float4 g_eye_pos;

// ����
float4 g_ambient;

// �t�H�O�J���[
float4 g_fog_color;

// �t�H�O�̊J�n�ʒu
float g_near = 0.f;

// �t�H�O�̏I���ʒu
float g_far = 1.f;

// �e�N�X�`��
texture g_tex;

sampler smp = sampler_state {
	texture = <g_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
	// CLAMP
	// BORDER
};

struct VS_OUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float4 pos_wvp : TEXCOORD1;
};

struct VS_IN {
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


VS_OUT VS(VS_IN In) {

	VS_OUT Out;

	float4x4 mat;
	mat = mul(g_world, g_view);
	mat = mul(mat, g_proj);
	Out.pos = mul(In.pos, mat);

	// ���C�g�̕�������t����
	float3 l_d = -g_light_dir.xyz;

	float3 normal = normalize(In.normal.xyz);

	// �@���ƃ��C�g�̕�������F�ϊ�
	Out.color = max(g_ambient, dot(normal, l_d));
	Out.uv = In.uv;
	Out.pos_wvp = In.pos;

	return Out;
}


float4 PS1(VS_OUT In) : COLOR0{

	float4 Out;

// ���_�Ǝ��_�Ƃ̋������v�Z����(�����X�J���[���o��)
float d = distance(In.pos_wvp.xyz, g_eye_pos.xyz);

// ��������F����o��
float f = (g_far - d) / (g_far - g_near);

// �ŏ��ő�ɃN�����v����
f = clamp(f, 0.0f, 1.0f);

// �e�N�X�`��uv�Z�b�g
Out = In.color * tex2D(smp,In.uv) * f + g_fog_color * (1.0f - f);

return Out;
}


float4 PS2(VS_OUT In) : COLOR0{

	return float4(1.f,1.f,1.f,1.f);
}


technique tech1 {

	pass Fog1 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS1();
	}
};