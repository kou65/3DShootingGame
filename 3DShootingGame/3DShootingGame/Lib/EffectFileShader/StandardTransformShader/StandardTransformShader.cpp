#include"StandardTransformShader.h"



StandardTransformShader::StandardTransformShader() : ShaderBase() {

	D3DXMatrixIdentity(&m_world_mat);
	D3DXMatrixIdentity(&m_view_mat);
	D3DXMatrixIdentity(&m_proj_mat);
}


void StandardTransformShader::InitTransformHandle(
const std::string& world_mat_name,
const std::string& view_mat_name,
const std::string& proj_mat_name
) {

	// デフォルト引数に名前が入っていたらそちらを代入
	if (world_mat_name.size() > 0) {
		m_world_mat_name = world_mat_name;
	}
	if (view_mat_name.size() > 0) {
		m_view_mat_name = view_mat_name;
	}
	if (proj_mat_name.size() > 0) {
		m_proj_mat_name = proj_mat_name;
	}


	// ワールドハンドル取得
	m_h_world_mat = m_p_effect->
		GetParameterByName(NULL, m_world_mat_name.c_str());

	// ビューハンドル取得
	m_h_view_mat = m_p_effect->
		GetParameterByName(NULL, m_view_mat_name.c_str());

	// 射影ハンドル取得
	m_h_proj_mat = m_p_effect->
		GetParameterByName(NULL, m_proj_mat_name.c_str());
}


void StandardTransformShader::UpdateTransfromMatrix(
) {

	// マトリックスセット
	m_p_effect->SetMatrix(m_h_world_mat, &m_world_mat);
	m_p_effect->SetMatrix(m_h_view_mat, &m_view_mat);
	m_p_effect->SetMatrix(m_h_proj_mat, &m_proj_mat);

	// パラメータ更新
	m_p_effect->CommitChanges();
}


void StandardTransformShader::UpdateStandardCamera() {

	// 現在設定しているカメラを取り出してくる
	OutCurrentCamera(&m_view_mat, &m_proj_mat);
}


void StandardTransformShader::SetWorldMatrix(const D3DXMATRIX&world_mat) {
	m_world_mat = world_mat;
}


void StandardTransformShader::SetViewMatrix(const D3DXMATRIX&view_mat) {
	m_view_mat = view_mat;
}


void StandardTransformShader::SetProjMatrix(const D3DXMATRIX&proj_mat) {
	m_proj_mat = proj_mat;
}
