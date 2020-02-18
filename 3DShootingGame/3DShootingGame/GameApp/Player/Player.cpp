#include"../Player/Player.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../../Lib/D3DFont/D3DFont.h"
#include<math.h>



Player::Player(
	D3DXVECTOR3 pos,
	Camera3D*camera_3d,
	BulletFactory*bullet_factory) {


	m_pos = pos;
	m_p_camera_3d = camera_3d;
	m_p_bullet_factory = bullet_factory;
}


void Player::Update() {


	// 移動値加算
	AddMoveToPos();

	// 移動値初期化
	InitMove();

	// 移動
	Move();

	// 回転
	//Rotation();

	// 弾発射
	ShotBullet();

}


void Player::Draw() {

	ObjParameter param;

	param.pos.z = 0.f;
	param.pos = m_pos;
	param.register_name = Const::Graph::PLAYER;
	param.rota.y = 90.f;
	
	// 位置
	//param.pos = m_p_camera_3d->GetPos();

	// 描画
	Obj::GetInstance()->Draw(
		param
	);

	D3DFont::Draw(150.f, 100.f, 100,"p_pos_x=>%f",m_pos.x);
	D3DFont::Draw(150.f, 110.f, 100,"p_pos_y=>%f", m_pos.y);
	D3DFont::Draw(150.f, 120.f, 100,"p_pos_z=>%f", m_pos.z);
}


void Player::Move() {

	// キーボード操作
	{

		// 移動
		if (KeyBoard::IsKeyPushing(DIK_UP)) {
			m_p_camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, SPEED));
			m_move.z = SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_DOWN)) {
			m_p_camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, -SPEED));
			m_move.z = -SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_RIGHT)) {
			m_p_camera_3d->AddMove(D3DXVECTOR3(SPEED, 0.f, 0.f));
			m_move.x = SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_LEFT)) {
			m_p_camera_3d->AddMove(D3DXVECTOR3(-SPEED, 0.f, 0.f));
			m_move.x = -SPEED;
		}


		// 回転
		if (KeyBoard::IsKeyPushing(DIK_D)) {
			m_p_camera_3d->AddRotation(D3DXVECTOR3(1.f, 0.f, 0.f));
			m_radius.x++;
		}
		if (KeyBoard::IsKeyPushing(DIK_A)) {
			m_p_camera_3d->AddRotation(D3DXVECTOR3(-1.f, 0.f, 0.f));
			m_radius.x--;
		}
		if (KeyBoard::IsKeyPushing(DIK_W)) {
			m_p_camera_3d->AddRotation(D3DXVECTOR3(0.f, -1.f, 0.f));
			m_radius.y++;
		}
		if (KeyBoard::IsKeyPushing(DIK_S)) {
			m_p_camera_3d->AddRotation(D3DXVECTOR3(0.f, 1.f, 0.f));
			m_radius.y--;
		}
	}
}


void Player::AddMoveToPos() {
	m_pos += m_move;
}

void Player::SetRadius(const float&rad) {
	m_radius.x = rad;
}

void Player::InitMove() {
	m_move.Init();
}


void Player::Rotation() {

	m_pos.x = m_pos.x * cosf(m_radius.x / 180) 
		- m_pos.y * sinf(m_radius.x / 180);

	m_pos.z = m_pos.x * sinf(m_radius.y / 180) 
		- m_pos.y * cosf(m_radius.y / 180);
}

void Player::ShotBullet() {

	if (KeyBoard::IsKeyPush(DIK_RETURN)) {
		m_p_bullet_factory->Create(
			Vec3(m_pos.x,
			m_pos.y,
			m_pos.z),
			2.f,
			200.f
		);
	}
}