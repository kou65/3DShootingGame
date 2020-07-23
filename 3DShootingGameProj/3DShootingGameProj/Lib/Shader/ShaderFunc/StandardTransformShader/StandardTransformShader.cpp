#include"StandardTransformShader.h"



StandardTSShader::StandardTSShader() : ShaderBase() {

	D3DXMatrixIdentity(&m_mat_world);
	D3DXMatrixIdentity(&m_mat_view);
	D3DXMatrixIdentity(&m_mat_proj);
}


void StandardTSShader::InitTSHandle(
const std::string& world_mat_name,
const std::string& view_mat_name,
const std::string& proj_mat_name
) {

	// ワールドハンドル取得
	m_h_world = mp_effect->
		GetParameterByName(
			NULL,world_mat_name.c_str());

	// ビューハンドル取得
	m_h_view_mat = mp_effect->
		GetParameterByName(NULL, view_mat_name.c_str());

	// 射影ハンドル取得
	m_h_proj_mat = mp_effect->
		GetParameterByName(NULL, proj_mat_name.c_str());
}


void StandardTSShader::Update(
) {

	// マトリックスセット
	mp_effect->SetMatrix(m_h_world, &m_mat_world);
	mp_effect->SetMatrix(m_h_view_mat, &m_mat_view);
	mp_effect->SetMatrix(m_h_proj_mat, &m_mat_proj);

	// パラメータ更新
	mp_effect->CommitChanges();
}


void StandardTSShader::UpdateStandardCamera() {

	// 現在設定しているカメラを取り出してくる
	OutMatrixGrapicsCamera(&m_mat_view, &m_mat_proj);
}


void StandardTSShader::SetWorldMatrix(const D3DXMATRIX&world_mat) {
	m_mat_world = world_mat;
}


void StandardTSShader::SetViewMatrix(const D3DXMATRIX&view_mat) {
	m_mat_view = view_mat;
}


void StandardTSShader::SetProjMatrix(const D3DXMATRIX&proj_mat) {
	m_mat_proj = proj_mat;
}
