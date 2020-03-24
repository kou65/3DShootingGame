#pragma once
#include"../ShaderBase/EffectFileBase.h"



// 標準変換用継承シェーダー


class StandardTransformShader : public ShaderBase{
public:

	StandardTransformShader();

	void SetWorldMatrix(const D3DXMATRIX&world_mat);
	void SetViewMatrix(const D3DXMATRIX&view_mat);
	void SetProjMatrix(const D3DXMATRIX&proj_mat);

protected:

	// 各ハンドル初期化
	void InitTransformHandle(
		const std::string& world_mat_name = "",
		const std::string& view_mat_name = "",
		const std::string& proj_mat_name = ""
	);

	// 各パラメータを更新
	void UpdateTransfromMatrix();

	// 標準カメラ更新
	void UpdateStandardCamera();

protected:

	// シェーダーに登録されている名前を入れる
	std::string m_world_mat_name;
	std::string m_view_mat_name;
	std::string m_proj_mat_name;

	// 変換ハンドル
	D3DXHANDLE m_h_world_mat;
	D3DXHANDLE m_h_view_mat;
	D3DXHANDLE m_h_proj_mat;

	// 各行列保持
	D3DXMATRIX m_world_mat;
	D3DXMATRIX m_view_mat;
	D3DXMATRIX m_proj_mat;
};