#pragma once
#include"../StandardTransformShader/StandardTransformShader.h"
#include"../LightData/LightData.h"



// 距離フォグ
// 距離に応じてモデルに霧を掛ける。


class FogShader : public StandardTSShader {
public:

	void Init();
	void Update()override;

	void SetColor(const D3DXVECTOR4 &color);
	void SetTexture(IDirect3DTexture9* p_tex);
	void SetLight(const LightData&data);
	void SetNear(const float&num);
	void SetFar(const float&num);

private:

	void InitHandle();
	void UpdateFog();

private:

	// 名前
	std::string m_eye_pos_name;
	std::string m_light_dir_name;
	std::string m_texture_name;
	std::string m_color_name;
	std::string m_ambient_name;
	std::string m_far_name;
	std::string m_near_name;

	// テクスチャハンドル
	D3DXHANDLE m_h_texture;
	// カラーハンドル
	D3DXHANDLE m_h_color;
	// ライトの方向
	D3DXHANDLE m_h_light_dir;
	// 環境光
	D3DXHANDLE m_h_ambient;
	// 注視点位置
	D3DXHANDLE m_h_eye_pos;

	// 奥と手前
	D3DXHANDLE m_h_near;
	D3DXHANDLE m_h_far;

	// カラー値
	D3DXVECTOR4 m_color;

	// テクスチャポインタ
	IDirect3DTexture9*mp_tex;

	// ライトデータ
	LightData m_light_data;

	float m_near;
	float m_far;
};

