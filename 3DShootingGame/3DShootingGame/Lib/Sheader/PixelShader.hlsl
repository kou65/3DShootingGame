float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 ZBufferPlot_PS(float4 ShadowMapTex : TEXCOORD) : COLOR
{
	// 引数から渡されてきた補間テクスチャ座標からz値を算出
	// それを色として出力

	// z値算出
	return ShadowMapTex.z / ShadowMapTex.w;
}