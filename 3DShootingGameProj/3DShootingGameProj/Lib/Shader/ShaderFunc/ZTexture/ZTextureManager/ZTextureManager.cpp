#include"ZTextureManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../../Sprite2D/Sprite2D/Sprite2DUser.h"




ZTextureManager::ZTextureManager() 
{
	
};


void ZTextureManager::Init(
	const Vec3&camera_pos,
	const Vec3&look_pos
) {

	// 代入
	m_pos = camera_pos;
	m_look = look_pos;

	/* カメラの初期化 */

	D3DXMATRIX mat_light_view;
	D3DXMATRIX mat_light_proj;

	// ライトビュー
	D3DXMatrixLookAtLH(
		// ライトビュー行列
		&mat_light_view,
		// カメラの位置座標
		&camera_pos,
		// 注視点座標ポインタ
		&look_pos,
		// アップベクトル
		&D3DXVECTOR3(0, 1, 0)
	);


	// ライト射影
	D3DXMatrixPerspectiveFovLH(
		// カメラ射影行列
		&mat_light_proj,
		// 画角
		D3DXToRadian(45),
		// アスペクト比
		640.0f / 480.0f,
		// 視推台の最も近い距離()
		10.0f,
		// 視推台の最も遠い距離
		1000.0f
	);

	// 挿入
	m_unique_z_tex_data.mat_camera_view = mat_light_view;
	m_unique_z_tex_data.mat_camera_proj = mat_light_proj;
}


void ZTextureManager::Update() {

	// カメラの更新
	//UpdateCamera();

	// 各更新
	for (auto&itr : m_z_tex_list) {

		// パラメータセット
		itr.second->SetViewMatrix(
			m_unique_z_tex_data.mat_camera_view
		);
		itr.second->SetProjMatrix(
			m_unique_z_tex_data.mat_camera_proj
		);

		// 次
		itr.second->Update();
	}
}


void ZTextureManager::UpdateCamera() {


	D3DXMATRIX mat_light_view;
	D3DXMATRIX mat_light_proj;

	// ライトビュー
	D3DXMatrixLookAtLH(
		// ライトビュー行列
		&mat_light_view,
		// カメラの位置座標
		&m_pos,
		// 注視点座標ポインタ
		&m_look,
		// アップベクトル
		&D3DXVECTOR3(0, 1, 0)
	);

	// ライト射影
	D3DXMatrixPerspectiveFovLH(
		// カメラ射影行列
		&mat_light_proj,
		// 画角
		D3DXToRadian(45),
		// アスペクト比
		640.0f / 480.0f,
		// 視推台の最も近い距離()
		10.0f,
		// 視推台の最も遠い距離
		1000.0f
	);

	// 更新情報を代入
	m_unique_z_tex_data.mat_camera_view =
		mat_light_view;

	m_unique_z_tex_data.mat_camera_proj =
		mat_light_proj;
}


void ZTextureManager::Create(
const std::string&register_name,
const Vec2 &tex_size
) {

	// 生成
	m_z_tex_list[register_name] = 
		new ZTexture();

	// テクスチャサイズセット
	m_z_tex_list[register_name]->
		SetTextureSize((UINT)tex_size.x,(UINT)tex_size.y);

	// 初期化
	m_z_tex_list[register_name]->Init();

	m_z_tex_list[register_name]->Update();
}


void ZTextureManager::Destory(
	const std::string&register_name
) {

	// メモリ解放
	delete m_z_tex_list[register_name];

	// 要素削除
	m_z_tex_list.erase(register_name);
}


ZTexture* ZTextureManager::GetZTexturePtr(
	const std::string&register_name
) {
	return m_z_tex_list[register_name];
}


void ZTextureManager::ZTextureSpriteDraw(
	IDirect3DTexture9*p_tex,
	const TransformData2D&data
) {

	Sprite2DParameter param;

	param.pos.x = data.pos.x;
	param.pos.y = data.pos.y;

	param.tex_size.x = data.scale.x;
	param.tex_size.y = data.scale.y;

	param.p_tex = p_tex;

	// Zテクスチャをスプライト表示
	Sprite2DUser::GetInstance()->BoardDraw(
		param
	);
}


void ZTextureManager::SpriteDraw(
	const std::string&str,
	const TransformData2D&data
) {

	ZTextureSpriteDraw(
		m_z_tex_list[str]->GetZTexture(),
		data
	);
}


void ZTextureManager::AllDestory() {


	// メモリ解放
	for (auto&itr : m_z_tex_list) {
		delete itr.second;
	}

	// 要素の削除
	m_z_tex_list.clear();
}


void ZTextureManager::SetViewMatrix(const ZTextureData&data) {
	m_unique_z_tex_data.mat_camera_view = data.mat_camera_view;
}


void ZTextureManager::SetProjMatrix(const ZTextureData&data) {
	m_unique_z_tex_data.mat_camera_proj = data.mat_camera_proj;
}


void ZTextureManager::SetPos(const Vec3&pos) {
	m_pos = pos;
}


void ZTextureManager::SetLookPos(const Vec3&pos) {
	m_look = pos;
}


void ZTextureManager::AddPos(const Vec3&pos) {
	m_pos += pos;
}


void ZTextureManager::AddLookPos(const Vec3&pos) {
	m_look += pos;
}


ZTextureData ZTextureManager::GetZTexData() {
	return m_unique_z_tex_data;
}


void ZTextureManager::Release() {

	for (auto&itr : m_z_tex_list) {
		delete itr.second;
	}
}