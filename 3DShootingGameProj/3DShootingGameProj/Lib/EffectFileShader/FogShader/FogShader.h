#pragma once
#include"../StandardTransformShader/StandardTransformShader.h"



// 距離フォグ
// 距離に応じてモデルに霧を掛ける。


class FogShader : public StandardTransformShader {
public:

	void Init()override;
	void Update()override;

	void SetColor(const D3DXVECTOR4 &color);
	void SetTexture(IDirect3DTexture9* p_tex);

private:

	void InitHandle();

private:

	// テクスチャハンドル
	D3DXHANDLE m_h_texture;
	// カラーハンドル
	D3DXHANDLE m_h_color;
	// ライトの方向
	D3DXHANDLE m_h_light_dir;
	// 環境光
	D3DXHANDLE m_h_ambient;

	// カラー値
	D3DXVECTOR4 m_color;
	// テクスチャポインタ
	IDirect3DTexture9*m_p_tex;

};

