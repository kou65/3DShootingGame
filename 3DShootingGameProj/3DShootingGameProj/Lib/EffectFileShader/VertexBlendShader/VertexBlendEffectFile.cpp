#include"VertexBlendEffectFile.h"



VertexBlendEffectFile::VertexBlendEffectFile() : ShaderBase(){

	m_bone_count = 64;

	D3DXMatrixIdentity(&m_world_mat);
}


void VertexBlendEffectFile::Init(){

	// 通常のシェーダー初期化
	CreateEffectFile(
		"Lib/ShaderFile/VertexBlend.hlsl",
		"tech1",
		VertexDecl::FBX
	);

	// デクラレーション初期化
	m_decl.CreateFbxFileDecl();

	// 頂点フォーマットの設定
	m_decl.Set();

	// アニメーション用
	m_bone_mat_h = 
		m_p_effect->
		GetParameterByName(NULL, "g_bone_mat");

	// インデックス用
	m_total_index_h =
		m_p_effect->GetParameterByName(NULL, "g_max_blend_idx");
}


void VertexBlendEffectFile::Update() {


	// 頂点フォーマットの設定
	m_decl.Set();
	
	// 現在設定しているカメラを入れる
	OutCurrentCamera(&m_camera_view_mat,&m_camera_proj_mat);


	m_p_effect->SetInt(m_total_index_h,m_max_index);

	// シェーダーの更新
	//SetBaseParam();

	//// ボーン姿勢行列セット
	//m_p_effect->SetMatrixTransposeArray(
	//	m_bone_mat_h,
	//	m_bone_mat_list,
	//	64
	//);

	// レジスタセット
	RegisterMat(0, &m_camera_view_mat, 4);
	RegisterMat(4, &m_camera_proj_mat, 4);
	RegisterMat(8, &m_world_mat, 4);
	RegisterMat(12, m_bone_mat_list, 50);

	CommitShader();
}

void VertexBlendEffectFile::UpdateParam() {

	// パラメータセット
	m_p_effect->
		SetMatrixTranspose(m_camera_view_mat_h, &m_camera_view_mat);
	m_p_effect->
		SetMatrixTranspose(m_camera_proj_mat_h, &m_camera_proj_mat);
	m_p_effect->
		SetMatrixTranspose(m_world_mat_h, &m_world_mat);
	m_p_effect->
		SetVector(m_color_h, &m_color);

	// テクニックをセット
	m_p_effect->SetTechnique(m_h_technique);

	// シェーダー更新
	CommitShader();
}


void VertexBlendEffectFile::SetMaxIndex(const int&index) {

	m_max_index = index;
}


void VertexBlendEffectFile::SetBoneMatrix(
	D3DXMATRIX*bone_mat
) {
	m_bone_mat_list = bone_mat;
}

void VertexBlendEffectFile::SetBoneMatrixArray(
	D3DXHANDLE&handle,
	D3DXMATRIX*mat,
	const int &array_num
) {

	// 行列配列をセット
	HRESULT hr 
		= m_p_effect->SetMatrixPointerArray(
			handle,
			(const D3DXMATRIX**)&mat,
			array_num
		);

}


void VertexBlendEffectFile::SetBoneCount(const int&count) {

	m_bone_count = count;
}



void VertexBlendEffectFile::RegisterMat(
	const int &start_regster,
	const D3DXMATRIX*mat,
	const int &vector4count){

	Graphics::
		GetInstance()->
		GetDevice()->
		SetVertexShaderConstantF(start_regster, (const float*)mat,vector4count);
}



void VertexBlendEffectFile::SetTexture(
	IDirect3DTexture9* p_tex
) {

	HRESULT hr = m_p_effect->SetTexture(m_texture_h, p_tex);

	if (hr != S_OK) {
		// 失敗
		hr = S_FALSE;
	}
}


void VertexBlendEffectFile::SetColor(const D3DXVECTOR4 &color) {
	m_color = color;
}

void VertexBlendEffectFile::SetWorldMatrix(const D3DXMATRIX&world_mat) {
	m_world_mat = world_mat;
}
