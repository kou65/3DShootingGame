#include"Player.h"
#include"Lib\3D\OBJ\OBJFile.h"
#include"Lib\DirectInput\KeyBoard\KeyBoard.h"



Player::Player(
	D3DXVECTOR3 pos,
	Camera3D*camera_3d,
	BulletFactory*bullet_factory) {

	m_pos = pos;
	mp_camera_3d = camera_3d;
	mp_bullet_factory = bullet_factory;
}


void Player::Update() {

	Move();
}


void Player::Draw() {

	ObjFile::GetInstance()->DrawSubSet(
		"note_book",
		mp_camera_3d->GetPos().x,
		mp_camera_3d->GetPos().y,
		mp_camera_3d->GetPos().z
		);
}


void Player::Move() {

	// キーボード操作
	{
		// 移動
		if (KeyBoard::IsKeyPushing(DIK_UP)) {
			mp_camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_DOWN)) {
			mp_camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_RIGHT)) {
			mp_camera_3d->AddMove(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_LEFT)) {
			mp_camera_3d->AddMove(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}


		// 回転
		if (KeyBoard::IsKeyPushing(DIK_D)) {
			mp_camera_3d->AddRotation(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_A)) {
			mp_camera_3d->AddRotation(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_W)) {
			mp_camera_3d->AddRotation(D3DXVECTOR3(0.f, -1.f, 0.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_S)) {
			mp_camera_3d->AddRotation(D3DXVECTOR3(0.f, 1.f, 0.f));
		}
	}
}


void Player::BulletShot() {

	if (KeyBoard::IsKeyPush(DIK_RETURN)) {
		mp_bullet_factory->Create(
			m_pos.x,
			m_pos.y,
			m_pos.z,
			2.f
		);
	}
}