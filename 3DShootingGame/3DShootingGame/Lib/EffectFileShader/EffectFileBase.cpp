#include"EffectFileBase.h"



EffectFileBase::EffectFileBase() {

	m_p_graphics = Graphics::GetInstance();

	D3DXMatrixIdentity(&m_world_mat);
}


bool EffectFileBase::Init(
) {
	
	// 通常のシェーダー初期化
	InitBaseShader(m_shader_name,m_tech_name);

	return true;
}


void EffectFileBase::SetShaderName(const std::string&shader_name) {
	m_shader_name = shader_name;
}

void EffectFileBase::SetTechnique(const std::string&tech_name) {
	m_tech_name = tech_name;
}

void EffectFileBase::Set3DParam() {

}

void EffectFileBase::Update() {

	// シェーダーの更新
	SetBaseParam();
}


void EffectFileBase::InitBaseShader(
	const std::string& shader_name,
	const std::string& tech_name
) {

	// それぞれの名前を代入
	this->m_shader_name = shader_name;
	this->m_tech_name = tech_name;

	// シェーダーファイル作成
	if (CreateBaseShader() == false) {
		return;
	}
}


bool EffectFileBase::CreateBaseShader(
const char*view_mat,
const char*proj_mat,
const char*world_mat,
const char*tex
) {


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

	// グローバル変数のハンドル取得

	// 3Dパラメータ
	m_camera_view_mat_h = 
		m_p_effect->GetParameterByName(NULL, view_mat);
	m_camera_proj_mat_h = 
		m_p_effect->GetParameterByName(NULL, "g_proj_mat");

	// ワールド
	m_world_mat_h =
		m_p_effect->GetParameterByName(NULL, "g_world_mat");

	// テクスチャ
	m_texture_h =
		m_p_effect->GetParameterByName(NULL, "g_tex");

	// テクニック取得
	m_tech_h = m_p_effect->GetTechniqueByName(m_tech_name.c_str());

	return true;
}


void EffectFileBase::SetBaseParam() {

	// パラメータセット
	m_p_effect->
		SetMatrix(m_camera_view_mat_h,&m_camera_view_mat);
	m_p_effect->
		SetMatrix(m_camera_proj_mat_h, &m_camera_proj_mat);
	m_p_effect->
		SetMatrix(m_world_mat_h, &m_world_mat);

	m_p_effect->SetTechnique(m_tech_h);

	// シェーダー更新
	CommitShader();
}


void EffectFileBase::SetWorldMatrix(const D3DXMATRIX &world_mat) {

	m_world_mat = world_mat;
}


void EffectFileBase::SetDefauleCamera() {

	D3DXMATRIX view, proj;

	// ビュー行列
	m_p_graphics->
		GetInstance()->
		GetDevice()
		->GetTransform(D3DTS_VIEW, &view);

	// 射影行列 
	m_p_graphics->
		GetInstance()->
		GetDevice()
		->GetTransform(D3DTS_PROJECTION, &proj);


	m_camera_view_mat = view;
	m_camera_proj_mat = proj;

}


void EffectFileBase::SetTexture(
	IDirect3DTexture9* p_tex
) {

	HRESULT hr = m_p_effect->SetTexture(m_texture_h, p_tex);

	if (hr != S_OK) {
		// 失敗
		hr = S_FALSE;
	}
}


void EffectFileBase::SetDeclaration() {

	if (m_p_decl == nullptr) {
		return;
	}

	HRESULT hr = m_p_graphics->GetInstance()->
		GetDevice()->
		SetVertexDeclaration(
			m_p_decl
		);

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}



void EffectFileBase::ShaderUpdateMatrixArray(
	const D3DXHANDLE& handle,
	const D3DXMATRIX* update_mat,
	const int &count
) {

	// 行列配列をセット
	//HRESULT hr 
	//	= m_p_effect->SetMatrixPointerArray(
	//		handle,
	//		(const D3DXMATRIX**)&update_mat,
	//		count);

	HRESULT hr
		= m_p_effect->SetMatrixArray(
			"g_bone_mat",
			update_mat,
			count
		);

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
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

