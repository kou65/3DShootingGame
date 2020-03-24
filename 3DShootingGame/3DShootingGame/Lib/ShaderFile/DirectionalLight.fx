

// ���s�������C�e�B���O(�f�B���N�V���i�����C�g)

float4x4 g_world : WORLD;
float4x4 g_proj : PROJECTION;
float4x4 g_view : VIEW;

/* �O���O���[�o�� */
// ���C�g����
float4 g_dir_light;

// �_����
float4 g_point_light_pos;

// �J���[
float4 g_color;

// ���C�g�J���[
float4 g_light_color;

// ���_���W
float4 g_eye_dir;

// �J�������W
float4 g_eye_pos;

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
	//float4 pos2;
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

	Out.normal = mul(In.normal, g_world);

	return Out;
}


//// �t�H�����˃��f��
//VS_OUT PhongReflectionVS(VS_IN In) {
//
//}


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
}