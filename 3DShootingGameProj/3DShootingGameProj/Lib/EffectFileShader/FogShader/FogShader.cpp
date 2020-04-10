#include"FogShader.h"




void FogShader::Init() {
	// シェーダーファイル生成
	if (CreateEffectFile(
		"Lib/ShaderFile/ObjDraw.fx",
		"tech1",
		VertexDecl::OBJ
	) == false) {
		return;
	}

	// 白
	SetColor(D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));

	// トランスフォームハンドル取得
	StandardTransformShader::InitTransformHandle(
		"matWorld",
		"matView",
		"matProj"
	);

	// ハンドル初期化
	InitHandle();
}


void FogShader::Update() {

}


void FogShader::SetColor(const D3DXVECTOR4 &color) {

}


void FogShader::SetTexture(IDirect3DTexture9* p_tex) {
	// テクスチャセット
	HRESULT hr = m_p_effect->SetTexture(m_h_texture, p_tex);
}


void FogShader::InitHandle() {
	
}