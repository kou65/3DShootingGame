
// プログラマブルシェーダーの作成

// ワールドビュー座標変換行列
float4x4 mat_world_view_proj;

struct VS_OUTPUT {

	// 射影変換座標(位置記号にする)
	float4 pos : POSITION;

	// Zバッファテクスチャ
	float4 ShadowMapTex : TEXCOORDO;
};


VS_OUTPUT ZBufferCalc_Vs(float4 Pos : POSITION) {

	VS_OUTPUT Out = (VS_OUTPUT)0;

	// 線形補間
	// 普通にワールドビュー射影行列をする(補間zとwが計算される)
	Out.Pos = mul(Pos, mat_world_view_proj);

	// テクスチャに位置を登録
	Out.ShadowMapTex = Out.Pos;

	return Out;
}