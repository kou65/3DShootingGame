#pragma once
#include"../StandardTransformShader/StandardTransformShader.h"
#include"../LightData/LightData.h"



struct ShadowData {

	// ライト用
	D3DXMATRIX light_view;
	D3DXMATRIX light_proj;

	// カメラ用
	D3DXMATRIX camera_view;
	D3DXMATRIX camera_proj;
};


enum class ShaderType {
	NORMAL,             // 通常
	ZTEXTURE,
	DEPTH_SHADOW,       // 深度影
	NORMAL_TEXTURE,     // 通常 + テクスチャ
	DIRECTIONAL,        // 平行光源
	DIFFUSE_REFLECTION, // 拡散反射
	SPECULAR_REFLECTION,// 鏡面反射
	POINT_LIGHT,        // 点光源
	PHONE_REFLECTION,   // フォン反射
	SHADOW,             // 光と影
};


class Light : public StandardTSShader{
public:

	Light() : StandardTSShader() {};

	// 初期化
	void Init()override;

	// 更新
	void Update()override;

	// テクスチャセット
	void SetTexture(IDirect3DTexture9*tex);

	// カラーセット
	void SetColor(const D3DXVECTOR4 &color);

	// ライトデータ
	void SetLightData(const LightData&light_data);

	// ライトビュー
	void SetLightView(const D3DXMATRIX&light_view);

	// ライト射影
	void SetLightProj(const D3DXMATRIX&light_proj);

	// シャドウテクスチャ
	void SetShadowTexture(IDirect3DTexture9*tex);


private:

	// ハンドルの初期化
	void InitHandle();

	// ライト更新
	void UpdateLight();

private:

	// ライト名
	// 平行光源
	std::string m_dir_light_name;
	// 点光源座標名
	std::string m_point_light_pos_name;
	// 点光源減衰
	std::string m_attenuation_name;
	// テクスチャシェーダー名
	std::string m_tex_name;
	// 視点座標名
	std::string m_eye_dir_name;
	// カメラ座標
	std::string m_eye_pos_name;

	// ライトカメラ
	std::string m_light_view_name;
	// ライト射影
	std::string m_light_proj_name;

	// カラー名
	std::string m_color_name;
	// ライトカラー名
	std::string m_light_color_name;
	// シャドウテクスチャ
	std::string m_shadow_tex_name;

	// ハンドル
	D3DXHANDLE m_h_dir_light;
	D3DXHANDLE m_h_tex;
	D3DXHANDLE m_h_color;
	D3DXHANDLE m_h_point_light_pos;
	D3DXHANDLE m_h_light_color;
	D3DXHANDLE m_h_eye_dir;
	D3DXHANDLE m_h_eye_pos;
	D3DXHANDLE m_h_attenuation;

	// ライトビューハンドル
	D3DXHANDLE m_h_light_view;
	D3DXHANDLE m_h_light_proj;

	// シャドウハンドル
	D3DXHANDLE m_h_shadow_tex;
	
	// カラー値
	D3DXVECTOR4 m_color;

	// テクスチャポインタ
	IDirect3DTexture9*m_p_tex;

	// シャドウマップ用zテクスチャ
	IDirect3DTexture9*m_p_shadow_map_tex;

	// ライトビュー
	D3DXMATRIX m_light_view_mat;

	// ライト射影
	D3DXMATRIX m_light_proj_mat;

	// ライトデータ
	LightData m_light_data;
	
};