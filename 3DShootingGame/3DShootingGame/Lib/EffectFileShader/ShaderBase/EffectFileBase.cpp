#include"EffectFileBase.h"



EffectFileBase::EffectFileBase() {

	m_p_graphics = Graphics::GetInstance();

}


void EffectFileBase::Init(
) {
	
	// 通常のシェーダー初期化
	CreateEffectFile();
}


void EffectFileBase::SetShaderName(const std::string&shader_name) {
	m_shader_name = shader_name;
}

void EffectFileBase::SetTechnique(const std::string&tech_name) {
	m_tech_name = tech_name;
}


void EffectFileBase::Update() {

}


bool EffectFileBase::CreateEffectFile() {


	LPD3DXBUFFER error = nullptr;

	if (m_p_graphics == nullptr){
		return false;
	}

	// エフェクトが入っているなら
	if (m_p_effect != nullptr){
		return false;
	}

	// シェーダーの作成
	D3DXCreateEffectFromFile(
		// デバイス
		m_p_graphics->GetInstance()->GetDevice(),
		// シェーダーファイル名
		TEXT(m_shader_name.c_str()),
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
	if (m_p_effect == nullptr) {
		m_p_effect = nullptr;
		return false;
	}

	// テクニックハンドル取得
	m_tech_h = m_p_effect->GetTechniqueByName(m_tech_name.c_str());

	return true;
}




void EffectFileBase::OutDefauleCamera(D3DXMATRIX*view,D3DXMATRIX*proj) {

	// ビュー行列
	m_p_graphics->
		GetInstance()->
		GetDevice()
		->GetTransform(D3DTS_VIEW, view);

	// 射影行列 
	m_p_graphics->
		GetInstance()->
		GetDevice()
		->GetTransform(D3DTS_PROJECTION, proj);

}



void EffectFileBase::CommitShader() {

	HRESULT hr = m_p_effect->CommitChanges();
	
	if (hr != S_OK) {
		hr = S_FALSE;
	}
}




void EffectFileBase::ShaderBegin(
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


void EffectFileBase::BeginPass(const int &pass_num) {

	// パス起動
	m_p_effect->BeginPass(
		// 起動したシェーダーのパスの数
		pass_num
	);
}


void EffectFileBase::EndPass() {

	// パス終了
	if (m_p_effect->EndPass() != S_OK) {

	}
}


void EffectFileBase::ShaderEnd() {

	// シェーダー終了
	if (m_p_effect->End() != S_OK) {

	}
}

