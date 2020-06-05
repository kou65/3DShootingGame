#include"ZTextureUser.h"
#include"../../Lib/Math/Math.h"



ZTextureUser::ZTextureUser() {

	Init();
}


void ZTextureUser::Init() {

	// zテクスチャ管理者の初期化(真上から下)
	ZTextureManager::GetInstance()->Init(
		Vec3(0.f, 50.f, 50.f),
		Vec3(0.f, -20.f, 50.f)
	);
}


void ZTextureUser::Update() {

	D3DXMATRIX view, proj;

	view = Graphics::GetInstance()->GetTSMatrix(D3DTS_VIEW);

	// ライト射影
	D3DXMatrixPerspectiveFovLH(
		// ライト射影行列
		&proj,
		// 画角
		D3DXToRadian(45),
		// アスペクト比
		1.0f,
		// 視推台の最も近い距離
		40.0f,
		// 視推台の最も遠い距離
		600.0f
	);


	D3DXMATRIX total;

	// カメラのz座標で動かす
	total = Utility::Math::GetTransformMatrix(
		Vec3(0.f, 150.f, 50.f - view._43),
		Vec3(1.f,1.f,1.f),
		Vec3(90.f,0.f,0.f)
	);

	// 逆行列変換
	D3DXMatrixInverse(&total, NULL, &total);

	ZTextureData data;
	data.mat_camera_view = total;
	data.mat_camera_proj = proj;

	// 行列をセット
	ZTextureManager::GetInstance()->SetViewMatrix(data);
	ZTextureManager::GetInstance()->SetProjMatrix(data);

	// Zテクスチャ管理者更新
	ZTextureManager::GetInstance()->Update();
}


void ZTextureUser::Draw() {

	// 描画
	m_z_tex_sprite.Draw();
}