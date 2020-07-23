

// ���[���h���C�g�v���W�F�N�V����
float4x4 WLP;
// ���W�ϊ��s��(world)
float4x4 WVP;
// ���[���h * �r���[ * �ˉe * �e�N�X�`��
float4x4 WVPB;

// ���C�g�̕���
float4 light_dir;

// �V���h�E�}�b�v�T�C�Y
float MAP_SIZE;

// �S�̃J���[
float4 g_color;

// ���f���e�N�X�`��
texture g_model_tex;

sampler tex1 = sampler_state {
	texture = <g_model_tex>;
	AddressU = WRAP;
	AddressV = WRAP;
};


texture g_shadow_map;

sampler src_samp = sampler_state {
	texture = <g_shadow_map>;
	AddressU = WRAP;
	AddressV = WRAP;
};


struct VS_OUT_PASS0 {
	float4 pos : POSITION;
	float4 depth : TEXCOORD0;
};


// 1�p�X�ڂ̏���
VS_OUT_PASS0 VS_PASS0(
	float4 pos : POSITION
) {

	VS_OUT_PASS0 Out =
		(VS_OUT_PASS0)0;

	// ���W�ϊ�
	float4 mul_pos = mul(pos, WLP);

	// �ʒu���
	Out.pos = mul_pos;

	// �J�������W�n�ł̐[�x���e�N�X�`���ɓ����
	Out.depth = mul_pos.zzzw;

	return Out;
}


float4 PS_PASS0(
	VS_OUT_PASS0 In
) : COLOR0{

	// z��w�Ŋ���
	return (In.depth / In.depth.w);

}


struct VS_OUT_QUAD {
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 tex0 : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
};

// �[�x�̗֊s�𒊏o
VS_OUT_QUAD VS_PASS1(
	float4 pos : POSITION,
	float4 col : COLOR0,
	float4 tex : TEXCOORD0
) {

	VS_OUT_QUAD Out = (VS_OUT_QUAD)0;

	// �ʒu
	Out.pos = pos;

	Out.col = col;

	// �΂ߏ㉺���E�̃e�N�Z���̒��S���w��

	float map_size = 100.f;

	// ����
	Out.tex0 = tex.xy + float2(-0.5f / map_size, -0.5f / map_size);

	// �E��
	Out.tex1 = tex.xy + float2(0.5f / map_size, 0.5f / map_size);

	// ����
	Out.tex2 = tex.xy + float2(-0.5f / map_size, 0.5f / map_size);

	// �E��
	Out.tex3 = tex.xy + float2(0.5f / map_size, -0.5f / map_size);

	return Out;
}
float4 PS_PASS1(
	VS_OUT_QUAD In
) : COLOR0{

	float4 color;

// ���𑪂�A���S�e�N�Z���𒊏o
float d0 = (float)(tex2D(src_samp,In.tex0) - tex2D(src_samp,In.tex1));
float d1 = (float)(tex2D(src_samp,In.tex2) - tex2D(src_samp,In.tex3));

// ����2�悷��
// �����̕�������菜���ׂ�2�悷��
color = (d0 * d0) + (d1 * d1);

//color = In.col;

return color;
}


// �ڂ�������
VS_OUT_QUAD VS_PASS2(
	float4 pos : POSITION, // ���f���̒��_
	float4 tex : TEXCOORD0 // �e�N�X�`�����W
) {

	// �o�̓f�[�^
	VS_OUT_QUAD Out = (VS_OUT_QUAD)0;

	// �ʒu���W
	Out.pos = pos;

	Out.tex0 = (float2)tex + float2(-3.0f / MAP_SIZE, -3.0f / MAP_SIZE);
	Out.tex1 = (float2)tex + float2(-3.0f / MAP_SIZE, -1.0f / MAP_SIZE);
	Out.tex2 = (float2)tex + float2(-3.0f / MAP_SIZE, 1.0f / MAP_SIZE);
	Out.tex3 = (float2)tex + float2(-3.0f / MAP_SIZE, 3.0f / MAP_SIZE);
	Out.tex4 = (float2)tex + float2(-1.0f / MAP_SIZE, -3.0f / MAP_SIZE);
	Out.tex5 = (float2)tex + float2(-1.0f / MAP_SIZE, -1.0f / MAP_SIZE);
	Out.tex6 = (float2)tex + float2(-1.0f / MAP_SIZE, 1.0f / MAP_SIZE);
	Out.tex7 = (float2)tex + float2(-1.0f / MAP_SIZE, 3.0f / MAP_SIZE);

	return Out;
}
float4 PS_PASS2(VS_OUT_QUAD In) : COLOR0{

	float4 color;

	float2 dvu = float2(4.0f / MAP_SIZE, 0);

	// 16��
	color =
		+tex2D(src_samp, In.tex0)
		+ tex2D(src_samp, In.tex1)
		+ tex2D(src_samp, In.tex2)
		+ tex2D(src_samp, In.tex3)
		+ tex2D(src_samp, In.tex4)
		+ tex2D(src_samp, In.tex5)
		+ tex2D(src_samp, In.tex6)
		+ tex2D(src_samp, In.tex7)

		// �ڂ����𒲐�
		+ tex2D(src_samp, In.tex0 + dvu)
		+ tex2D(src_samp, In.tex1 + dvu)
		+ tex2D(src_samp, In.tex2 + dvu)
		+ tex2D(src_samp, In.tex3 + dvu)
		+ tex2D(src_samp, In.tex4 + dvu)
		+ tex2D(src_samp, In.tex5 + dvu)
		+ tex2D(src_samp, In.tex6 + dvu)
		+ tex2D(src_samp, In.tex7 + dvu);

		return (0.7f * color);
}


struct VS_OUT {
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 diffuse : TEXCOORD0;
	float4 ambient : TEXCOORD1;
	float4 shadow_map_uv : TEXCOORD2;
	float4 depth : TEXCOORD3;
};


// �V���h�E�}�b�v�̎��Ɠ����v�Z
VS_OUT VS_PASS3(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL
) {

	VS_OUT Out = (VS_OUT)0;
	float4 uv;

	// ���W�ϊ�
	Out.pos = mul(pos, WVP);

	Out.color = color;

	// �F
	Out.diffuse = color * max(dot(light_dir.xyz, normal.xyz), 0);// �g�U�F
	Out.ambient = color * 0.3f; // ���F

	// �e�N�X�`�����W�ϊ�
	uv = mul(pos, WVPB);
	Out.shadow_map_uv = uv;

	// �[�x�l�Z�o
	uv = mul(pos, WLP);
	Out.depth = uv.zzzw;

	return Out;
}
float4 PS_PASS3(VS_OUT In) : COLOR0{

	// �J���[
	float4 color = In.ambient;

	// �V���h�E�}�b�v
	float shadow_map =
		tex2Dproj(tex1, In.shadow_map_uv).x;

	// �[�x��r
	if (shadow_map < In.depth.z / In.depth.w - 0.01f) {

		color += tex2Dproj(src_samp, In.shadow_map_uv);
	}
	else {
		color += 1.f;
	}

	return color;
}


// �e�N�j�b�N
technique tech{

	pass P0
	{
		VertexShader = compile vs_2_0 VS_PASS0();
		PixelShader = compile ps_2_0 PS_PASS0();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS_PASS1();
		PixelShader = compile ps_2_0 PS_PASS1();
	}
	
	pass P2
	{
		VertexShader = compile vs_2_0 VS_PASS2();
		PixelShader = compile ps_2_0 PS_PASS2();
	}
	
	pass P3
	{
		VertexShader = compile vs_2_0 VS_PASS3();
		PixelShader = compile ps_2_0 PS_PASS3();
	}
}

/*
// �p�X�O�ł͉e�C���[�W���쐬����
struct VS1_OUTPUT
{
	float4 Pos       : POSITION;
	float4 LightUV   : TEXCOORD0;      // Z�o�b�t�@�e�N�X�`���[�̃e�N�Z�����W
	float2 Depth     : TEXCOORD1;    // �J������̍s��ϊ��ɂ��v�Z�����[�x�l
};

VS1_OUTPUT VS1(
	float4 Pos     : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD0
)
{
	VS1_OUTPUT Out;

	Out.Pos = mul(Pos, m_WVP);

	// Z�o�b�t�@�̐[�x�����擾���邽�߂Ƀe�N�Z�����W���Z�b�g
	Out.LightUV = mul(Pos, WVPB);

	// ���C�g��̍s��ϊ��ɂ��[�x�l���v�Z����
	Out.Depth.xy = mul(Pos, WLP).zw;

	return Out;
}

float4 PS1(VS1_OUTPUT In) : COLOR0
{
	//�o�͂���f�t�H���g�F�͔�
	float4 Out = (float4)1;

	//Z�o�b�t�@����[�x�l���擾����
	float d = tex2Dproj(tex1, In.LightUV).r;

	//�e�ƂȂ�̂ŉe�̐F���o��
	if (d * In.Depth.y < In.Depth.x - m_Bias)
	   Out = m_ShadowColor;

	return Out;
}


// �V���h�E�}�b�v�̎��Ɠ����v�Z
VS_OUT VS_PASS1(
	float4 pos : POSITION,
	float4 color : COLOR0,
	float3 normal : NORMAL
) {

	VS_OUT Out = (VS_OUT)0;
	float4 uv;

	// ���W�ϊ�
	Out.pos = mul(pos, WVP);

	Out.color = color;

	// �F
	Out.diffuse = color * max(dot(light_dir.xyz, normal.xyz), 0);// �g�U�F
	Out.ambient = color * 0.3f; // ���F

	// �e�N�X�`�����W�ϊ�
	uv = mul(pos, WVPB);
	Out.shadow_map_uv = uv;

	// �[�x�l�Z�o
	uv = mul(pos, WLP);
	Out.depth = uv.zzzw;

	return Out;
}
float4 PS_PASS1(VS_OUT In) : COLOR0{

	// �J���[
	float4 color = In.ambient;

	// �V���h�E�}�b�v
	float shadow_map =
		tex2Dproj(tex1, In.shadow_map_uv).x;

	// �[�x��r
	if (shadow_map < In.depth.z / In.depth.w - 0.01f) {

		color += tex2Dproj(src_samp, In.shadow_map_uv);
	}
	else {
		color += 1.f;
	}

	return color;
}*/