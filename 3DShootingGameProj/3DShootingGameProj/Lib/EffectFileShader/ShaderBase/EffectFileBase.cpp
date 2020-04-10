#include"EffectFileBase.h"



ShaderBase::ShaderBase() {

}


void ShaderBase::Init() {

}


void ShaderBase::Update() {
	UpdateDecl();
}


bool ShaderBase::CreateEffectFile(
	const std::string&shader_name,
	const std::string&tech_name,
	const VertexDecl::Type&decl_type
) {

	SelectDeclCommit(decl_type);

	LPD3DXBUFFER error = nullptr;

	// エフェクトが入っているなら
	if (m_p_effect != nullptr){
		return false;
	}

	// シェーダーの作成
	D3DXCreateEffectFromFile(
		// デバイス
		Graphics::GetInstance()->GetDevice(),
		// シェーダーファイル名
		TEXT(shader_name.c_str()),
		0,
		0,
		// シェーダーコンパイル時のオプション
		D3DXSHADER_DEBUG,
		0,
		// 作成されたシェーダーデータを格納するeffect
		&m_p_effect,
		// エラーデータ
		&error
	);

	// エフェクトがないなら
	if (m_p_effect == nullptr){
		m_p_effect = nullptr;
		return false;
	}

	// テクニックハンドル取得
	m_h_technique = m_p_effect->
		GetTechniqueByName(tech_name.c_str());

	// テクニック設定
	m_p_effect->SetTechnique(m_h_technique);

	return true;
}


void ShaderBase::OutCurrentCamera(D3DXMATRIX*view,D3DXMATRIX*proj) {

	// ビュー行列
	Graphics::
		GetInstance()->
		GetDevice()
		->GetTransform(D3DTS_VIEW, view);

	// 射影行列 
	Graphics::
		GetInstance()->
		GetDevice()
		->GetTransform(D3DTS_PROJECTION, proj);

}


void ShaderBase::UpdateDecl() {
	m_decl.Set();
}


void ShaderBase::CommitTechnique() {

	m_p_effect->SetTechnique(m_h_technique);
}


void ShaderBase::CommitShader() {

	HRESULT hr = m_p_effect->CommitChanges();
	
	if (hr != S_OK) {
		hr = S_FALSE;
	}
}


void ShaderBase::SelectDeclCommit(const VertexDecl::Type&decl_type) {

	switch (decl_type) {

	case VertexDecl::Type::OBJ:
		m_decl.CreateObjFileDecl();
		break;

	case VertexDecl::Type::FBX:
		m_decl.CreateFbxFileDecl();
		break;

	case VertexDecl::Type::SHANDER_INDEX:
		m_decl.CreateShapeIndexDecl();
		break;
	}

	// コミット
	m_decl.Set();
}


void ShaderBase::Begin(
	UINT &total_pass_num,
	const DWORD &device_state_num
) {

	// シェーダー起動
	m_p_effect->Begin(
		// 起動したシェーダーのパスの数
		&total_pass_num,
		// デバイスステートの保存設定フラグ 
		device_state_num
	);

}


void ShaderBase::BeginPass(const int &pass_num) {

	// パス起動
	m_p_effect->BeginPass(
		// 起動したシェーダーのパスの数
		pass_num
	);
}


void ShaderBase::EndPass() {

	// パス終了
	if (m_p_effect->EndPass() != S_OK) {

	}
}


void ShaderBase::End() {

	// シェーダー終了
	if (m_p_effect->End() != S_OK) {

	}
}

