float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 ZBufferPlot_PS(float4 ShadowMapTex : TEXCOORD) : COLOR
{
	// ��������n����Ă�����ԃe�N�X�`�����W����z�l���Z�o
	// �����F�Ƃ��ďo��

	// z�l�Z�o
	return ShadowMapTex.z / ShadowMapTex.w;
}