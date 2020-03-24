#pragma once
#include"../StandardTransformShader/StandardTransformShader.h"



class Light : public StandardTransformShader{
public:

	enum Type {
		DIRECTIONAL,        // 平行光源
		DIFFUSE_REFLECTION, // 拡散反射
		SPECULAR_REFLECTION // 鏡面反射
	};

public:

	Light() : StandardTransformShader() {};

	// 初期化
	void Init()override;

	// 更新
	void Update()override;

	// ライトセット
	void SetDirLight(const D3DXVECTOR4&vec);

	// 点光源位置セット
	void SetPointLight(const D3DXVECTOR4&pos);

	// テクスチャセット
	void SetTexture(IDirect3DTexture9*tex);

	// カラーセット
	void SetColor(D3DXVECTOR4 &color);

	// ライトカラー
	void SetLightColor(D3DXVECTOR4&light_color);

	// 注視点座標
	void SetEyeDir(D3DXVECTOR4&eye_dir);

	// カメラ座標
	void SetEyePos(D3DXVECTOR4&eye_pos);

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
	// テクスチャシェーダー名
	std::string m_tex_name;
	// カラー名
	std::string m_color_name;
	// ライトカラー名
	std::string m_light_color_name;
	// 視点座標名
	std::string m_eye_dir_name;
	// カメラ座標
	std::string m_eye_pos_name;

	// ハンドル
	D3DXHANDLE m_h_dir_light;
	D3DXHANDLE m_h_tex;
	D3DXHANDLE m_h_color;
	D3DXHANDLE m_h_point_light_pos;
	D3DXHANDLE m_h_light_color;
	D3DXHANDLE m_h_eye_dir;
	D3DXHANDLE m_h_eye_pos;
	
	// ライトの方向
	D3DXVECTOR4 m_dir_light;

	// 点ライト位置
	D3DXVECTOR4 m_point_light_pos;

	// カラー値
	D3DXVECTOR4 m_color;

	// ライトカラー値
	D3DXVECTOR4 m_light_color;

	// 注視点座標
	D3DXVECTOR4 m_eye_dir;

	// カメラ座標
	D3DXVECTOR4 m_eye_pos;

	// テクスチャポインタ
	IDirect3DTexture9*m_p_tex;
	
};