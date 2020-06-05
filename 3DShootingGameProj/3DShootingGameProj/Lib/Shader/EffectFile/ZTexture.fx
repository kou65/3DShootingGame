float4x4 matWorld : WORLD;		// ���[���h�ϊ��s��
float4x4 matView : VIEW;		// �r���[�ϊ��s��
float4x4 matProj : PROJECTION;		// �ˉe�ϊ��s��


struct VS_OUTPUT
{
	// �ˉe�ϊ����W
	float4 Pos : POSITION;
	// Z�o�b�t�@�e�N�X�`��
	float4 ShadowMapTex : TEXCOORD0;
};

// ���_�V�F�[�_
VS_OUTPUT ZBufferCalc_VS(float4 Pos : POSITION)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ���ʂɃ��[���h�r���[�ˉe�s�������
	float4x4 mat;
	mat = mul(matWorld, matView);
	mat = mul(mat, matProj);
	Out.Pos = mul(Pos, mat);

	//mat = mul(Pos,matWorld);
	//mat = mul(mat, matView);
	//Out.Pos = mul(mat, matProj);
	
	// �e�N�X�`�����W�𒸓_�ɍ��킹��
	// ���_�ϊ���̒��_���W�����̂܂܃e�N�X�`�����W
	// �Ƃ��ăs�N�Z���V�F�[�_�[�ɓn��
	Out.ShadowMapTex = Out.Pos;

	return Out;
}


// �s�N�Z���V�F�[�_
float4 ZBufferPlot_PS(
	float4 pos : POSITION,
	float4 ShadowMapTex : TEXCOORD0) : COLOR
{
	// Z�l�Z�o
	return ShadowMapTex.z / ShadowMapTex.w;
}


technique ZValuePlotTec
{
	pass P0
	{
		VertexShader = compile vs_2_0 ZBufferCalc_VS();
		PixelShader = compile ps_2_0 ZBufferPlot_PS();
	}

	// �e�N�X�`���p�X�F���p
	pass P1
	{
		VertexShader = compile vs_2_0 ZBufferCalc_VS();
		PixelShader = compile ps_2_0 ZBufferPlot_PS();
	}
}
