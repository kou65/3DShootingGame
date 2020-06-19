

/**

�ڂ���

�o�C���j�A�̕�Ԗ@

�s�N�Z���V�F�[�_�[�@
�ڂ����ɂ͎���̃s�N�Z���̊������������ďo�͂���悤�ɂ���

���S�̃s�N�Z��0.2
���ӂ̃s�N�Z��0.1�ō���
���v��1.0�ɂȂ�悤�ɂ���
0.2 + 0.1 * 8 = 1.0

*/

float4x4 g_world;
float4x4 g_view;
float4x4 g_proj;



float g_tu[5];   //TU�����ׂ̗̃e�N�Z���ʒu
float g_tv[5];   //UV�����ׂ̗̃e�N�Z���ʒu

//sampler s0 : register(s0);  //�u���[�������錳�ƂȂ�e�N�X�`���[

// �e�N�X�`��
texture g_tex;

// �e�N�X�`���T���v��
sampler smp = sampler_state {
	texture = (g_tex);
};


struct VS_OUTPUT
{
	float4 Pos    : POSITION;
	float2 Tex    : TEXCOORD0;
};

VS_OUTPUT VS(
	float4 Pos    : POSITION,
	float2 Tex : TEXCOORD0
)
{
	VS_OUTPUT Out;

	Out.Pos = Pos;// �s��ϊ����Ȃ��̂ł��̂܂܃s�N�Z���V�F�[�_�[�ɓn��
	Out.Tex = Tex;

	return Out;
}


VS_OUTPUT VS2(
	float4 pos : POSITION,
	float2 tex : TEXCOORD0
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ���[���h�A�ˉe�A�r���[���W�ϊ�
	Out.Pos = mul(pos, g_world);
	Out.Pos = mul(Out.Pos, g_view);
	Out.Pos = mul(Out.Pos, g_proj);

	Out.Tex = tex;
}


//X�����ɂڂ���
float4 PS1(VS_OUTPUT In) : COLOR0
{
	//�e�N�Z�����擾
	float2 Texel0 = In.Tex + float2(-g_tu[0], 0.0f);
	float2 Texel1 = In.Tex + float2(-g_tu[1], 0.0f);
	float2 Texel2 = In.Tex + float2(-g_tu[2], 0.0f);
	float2 Texel3 = In.Tex + float2(-g_tu[3], 0.0f);
	float2 Texel4 = In.Tex + float2(-g_tu[4], 0.0f);

	float2 Texel5 = In.Tex + float2(g_tu[0], 0.0f);
	float2 Texel6 = In.Tex + float2(g_tu[1], 0.0f);
	float2 Texel7 = In.Tex + float2(g_tu[2], 0.0f);
	float2 Texel8 = In.Tex + float2(g_tu[3], 0.0f);
	float2 Texel9 = In.Tex + float2(g_tu[4], 0.0f);

	//�擾�����e�N�Z���ʒu�̃J���[�����擾����B
	//���ꂼ��̃J���[�l�ɏd�݂������Ă���B���̏d�ݒl�͂��ׂĂ̍��v�� 1.0f �ɂȂ�悤�ɒ�������B
	float4 p0 = tex2D(smp, In.Tex) * 0.20f;

	float4 p1 = tex2D(smp, Texel0) * 0.12f;
	float4 p2 = tex2D(smp, Texel1) * 0.10f;
	float4 p3 = tex2D(smp, Texel2) * 0.08f;
	float4 p4 = tex2D(smp, Texel3) * 0.06f;
	float4 p5 = tex2D(smp, Texel4) * 0.04f;

	float4 p6 = tex2D(smp, Texel5) * 0.12f;
	float4 p7 = tex2D(smp, Texel6) * 0.10f;
	float4 p8 = tex2D(smp, Texel7) * 0.08f;
	float4 p9 = tex2D(smp, Texel8) * 0.06f;
	float4 p10 = tex2D(smp, Texel9) * 0.04f;

	//�J���[����������
	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}


//Y�����ɂڂ���
float4 PS2(VS_OUTPUT In) : COLOR0
{
	//�e�N�Z�����擾   
	float2 Texel0 = In.Tex + float2(0.0f, -g_tv[0]);
	float2 Texel1 = In.Tex + float2(0.0f, -g_tv[1]);
	float2 Texel2 = In.Tex + float2(0.0f, -g_tv[2]);
	float2 Texel3 = In.Tex + float2(0.0f, -g_tv[3]);
	float2 Texel4 = In.Tex + float2(0.0f, -g_tv[4]);

	float2 Texel5 = In.Tex + float2(0.0f, g_tv[0]);
	float2 Texel6 = In.Tex + float2(0.0f, g_tv[1]);
	float2 Texel7 = In.Tex + float2(0.0f, g_tv[2]);
	float2 Texel8 = In.Tex + float2(0.0f, g_tv[3]);
	float2 Texel9 = In.Tex + float2(0.0f, g_tv[4]);

	//�擾�����e�N�Z���ʒu�̃J���[�����擾����B
	//���ꂼ��̃J���[�l�ɏd�݂������Ă���B���̏d�ݒl�͂��ׂĂ̍��v�� 1.0f �ɂȂ�悤�ɒ�������B
	float4 p0 = tex2D(smp, In.Tex) * 0.20f;

	float4 p1 = tex2D(smp, Texel0) * 0.12f;
	float4 p2 = tex2D(smp, Texel1) * 0.10f;
	float4 p3 = tex2D(smp, Texel2) * 0.08f;
	float4 p4 = tex2D(smp, Texel3) * 0.06f;
	float4 p5 = tex2D(smp, Texel4) * 0.04f;

	float4 p6 = tex2D(smp, Texel5) * 0.12f;
	float4 p7 = tex2D(smp, Texel6) * 0.10f;
	float4 p8 = tex2D(smp, Texel7) * 0.08f;
	float4 p9 = tex2D(smp, Texel8) * 0.06f;
	float4 p10 = tex2D(smp, Texel9) * 0.04f;

	//�J���[����������
	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

technique TShader
{
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
