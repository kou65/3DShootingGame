﻿#include"NormalShader.h"



void NormalShader::Init() {

	m_shader_name = "Lib/ShaderFile/ObjDraw.fx";
	m_tech_name = "tech1";

	// シェーダーファイル生成
	if (CreateEffectFile() == false) {
		return;
	}

	// デクル生成
	m_p_decl.CreateObjFileDecl();

	// セット
	m_p_decl.Set();

	// 白
	SetColor(D3DXVECTOR4(0.f, 0.f, 0.f, 0.f));

	// ハンドル初期化
	InitHandle();
}


void NormalShader::Update() {

	// デクルセット
	m_p_decl.Set();

	// デファルトのカメラをセット
	OutDefauleCamera(&m_camera_view_mat,&m_camera_proj_mat);

	// パラメータ更新
	UpdateParam();

	// シェーダー更新
	CommitShader();
}


void NormalShader::InitHandle() {

	// ビューハンドル取得
	m_camera_view_mat_h = m_p_effect->GetParameterByName(NULL,"g_view_mat");

	// 射影ハンドル取得
	m_camera_proj_mat_h = m_p_effect->GetParameterByName(NULL,"g_proj_mat");

	// ワールドハンドル取得
	m_world_mat_h = m_p_effect->GetParameterByName(NULL, "g_world_mat");

	// テクスチャハンドル取得
	m_texture_h = m_p_effect->GetParameterByName(NULL, "g_tex");

	// カラーハンドル取得
	m_color_h = m_p_effect->GetParameterByName(NULL, "g_color");
}


void NormalShader::UpdateParam() {

	m_p_effect->SetMatrix(m_camera_view_mat_h, &m_camera_view_mat);
	m_p_effect->SetMatrix(m_camera_proj_mat_h, &m_camera_proj_mat);
	m_p_effect->SetMatrix(m_world_mat_h, &m_world_mat);

	m_p_effect->SetVector(m_color_h, &m_color);
}


void NormalShader::SetWorldMatrix(D3DXMATRIX*world_mat) {
	m_world_mat = *world_mat;
}


void NormalShader::SetTexture(
	IDirect3DTexture9* p_tex
) {

	HRESULT hr = m_p_effect->SetTexture(m_texture_h, p_tex);

	if (hr != S_OK) {
		// 失敗
		hr = S_FALSE;
	}
}


void NormalShader::SetColor(const D3DXVECTOR4 &color) {
	m_color = color;
}