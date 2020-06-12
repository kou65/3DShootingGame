#pragma once
#include"../ShaderBase/EffectFileBase.h"



// 標準変換用継承シェーダー


/**
* @brief 標準変換シェーダー(継承しても良い)
*/
class StandardTSShader : public ShaderBase{
public:

	StandardTSShader();

	void UpdateTS();

	void SetWorldMatrix(const D3DXMATRIX&world_mat);
	void SetViewMatrix(const D3DXMATRIX&view_mat);
	void SetProjMatrix(const D3DXMATRIX&proj_mat);

protected:

	// 各ハンドル初期化
	void InitTSHandle(
		const std::string& world_mat_name,
		const std::string& view_mat_name,
		const std::string& proj_mat_name
	);

	// 標準カメラ更新
	void UpdateStandardCamera();

protected:

	// 変換ハンドル
	D3DXHANDLE m_h_world;
	D3DXHANDLE m_h_view_mat;
	D3DXHANDLE m_h_proj_mat;

	// 各行列保持
	D3DXMATRIX m_mat_world;
	D3DXMATRIX m_mat_view;
	D3DXMATRIX m_mat_proj;
};