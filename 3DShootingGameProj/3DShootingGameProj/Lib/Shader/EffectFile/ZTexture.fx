float4x4 matWorld : WORLD;		// ワールド変換行列
float4x4 matView : VIEW;		// ビュー変換行列
float4x4 matProj : PROJECTION;		// 射影変換行列


struct VS_OUTPUT
{
	// 射影変換座標
	float4 Pos : POSITION;
	// Zバッファテクスチャ
	float4 ShadowMapTex : TEXCOORD0;
};

// 頂点シェーダ
VS_OUTPUT ZBufferCalc_VS(float4 Pos : POSITION)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// 普通にワールドビュー射影行列をする
	float4x4 mat;
	mat = mul(matWorld, matView);
	mat = mul(mat, matProj);
	Out.Pos = mul(Pos, mat);

	//mat = mul(Pos,matWorld);
	//mat = mul(mat, matView);
	//Out.Pos = mul(mat, matProj);
	
	// テクスチャ座標を頂点に合わせる
	// 頂点変換後の頂点座標をそのままテクスチャ座標
	// としてピクセルシェーダーに渡す
	Out.ShadowMapTex = Out.Pos;

	return Out;
}


// ピクセルシェーダ
float4 ZBufferPlot_PS(
	float4 pos : POSITION,
	float4 ShadowMapTex : TEXCOORD0) : COLOR
{
	// Z値算出
	return ShadowMapTex.z / ShadowMapTex.w;
}


technique ZValuePlotTec
{
	pass P0
	{
		VertexShader = compile vs_2_0 ZBufferCalc_VS();
		PixelShader = compile ps_2_0 ZBufferPlot_PS();
	}

	// テクスチャパス認識用
	pass P1
	{
		VertexShader = compile vs_2_0 ZBufferCalc_VS();
		PixelShader = compile ps_2_0 ZBufferPlot_PS();
	}
}
