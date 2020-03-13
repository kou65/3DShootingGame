#include"../Player/Player.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../../Lib/D3DFont/D3DFont.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include<math.h>



Player::Player(
	D3DXVECTOR3 pos,
	Camera3D*camera_3d,
	ObjectFactory*bullet_factory) {


	m_pos = pos;
	m_p_camera_3d = camera_3d;
	m_p_obj_factory = bullet_factory;

	// カメラ位置を補正
	m_p_camera_3d->SetPos(Vec3(
		m_p_camera_3d->GetPos().x,
		m_p_camera_3d->GetPos().y,
		m_p_camera_3d->GetPos().z
	)
	);
	
	// 3つ分ある
	m_hp = MAX_HP;
}

static float ref_hp = 1.f;
static int fps = 0;

void Player::Update() {

	// 移動値加算
	AddMoveToPos();

	// 移動値初期化
	InitMove();

	// キー移動
	MoveByKey();

	// キー回転
	RotationByKey();

	// 回転
	Rotation();

	// 弾発射
	ShotBullet();

}


void Player::Draw() {

	ObjParameter param;

	param.pos.z = 0.f;
	param.pos = m_pos;
	param.register_obj_file_name = Const::Obj::PLAYER;

	param.rota.x = m_vec_rot.y;
	param.rota.y = 90.f + m_vec_rot.x;
	//param.rota.z = m_vec_rot.y;

	// 描画
	Obj::GetInstance()->ShaderDraw(
		param
	);

	D3DFont::Draw(150.f, 100.f, 100,"p_pos_x=>%f",m_pos.x);
	D3DFont::Draw(150.f, 110.f, 100,"p_pos_y=>%f", m_pos.y);
	D3DFont::Draw(150.f, 120.f, 100,"p_pos_z=>%f", m_pos.z);
}


void Player::MoveByKey() {

	// キーボード操作
	{

		// 前後移動
		if (KeyBoard::IsKeyPushing(DIK_V)) {
			m_p_camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, PLAYER_SPEED));
			m_move.z = PLAYER_SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_SPACE)) {
			m_p_camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, -PLAYER_SPEED));
			m_move.z = -PLAYER_SPEED;
		}

		// 上下
		else if (KeyBoard::IsKeyPushing(DIK_UP)) {
			//m_p_camera_3d->AddMove(D3DXVECTOR3(0.f, PLAYER_SPEED,0.f));
			m_move.y = PLAYER_SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_DOWN)) {
			//m_p_camera_3d->AddMove(D3DXVECTOR3(0.f, -PLAYER_SPEED, 0.f));
			m_move.y = -PLAYER_SPEED;
		}

		// 左右
		else if (KeyBoard::IsKeyPushing(DIK_RIGHT)) {
			//m_p_camera_3d->AddMove(D3DXVECTOR3(PLAYER_SPEED, 0.f, 0.f));
			m_move.x = PLAYER_SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_LEFT)) {
			//m_p_camera_3d->AddMove(D3DXVECTOR3(-PLAYER_SPEED, 0.f, 0.f));
			m_move.x = -PLAYER_SPEED;
		}

	}
}

void Player::RotationByKey() {


	// 回転
	if (KeyBoard::IsKeyPushing(DIK_D)) {
		m_p_camera_3d->AddRotation(D3DXVECTOR3(1.f, 0.f, 0.f));
		m_vec_rot.x++;
	}
	if (KeyBoard::IsKeyPushing(DIK_A)) {
		m_p_camera_3d->AddRotation(D3DXVECTOR3(-1.f, 0.f, 0.f));
		m_vec_rot.x--;
	}
	if (KeyBoard::IsKeyPushing(DIK_W)) {
		m_p_camera_3d->AddRotation(D3DXVECTOR3(0.f, -1.f, 0.f));
		m_vec_rot.y--;
	}
	if (KeyBoard::IsKeyPushing(DIK_S)) {
		m_p_camera_3d->AddRotation(D3DXVECTOR3(0.f, 1.f, 0.f));
		m_vec_rot.y++;
	}
}


void Player::AddMoveToPos() {

	//m_pos += m_nor_rot * m_move;
	m_pos += m_move;
}


void Player::InitMove() {
	m_move.Init();
}


void Player::Rotation() {

	// 行列
	D3DXMATRIX rot_m_x, rot_m_y, rot_m_z, total;
	D3DXMATRIX trans_m;

	// 初期化
	D3DXMatrixIdentity(&total);

	// 移動値
	Vec3 move;

	move.x = 0.f;
	move.y = 0.f;
	move.z = 1.f;

	// 正規化
	D3DXVec3Normalize(&move, &move);

	// 回転
	D3DXMatrixRotationX(&rot_m_x, D3DXToRadian(m_vec_rot.x));
	D3DXMatrixRotationY(&rot_m_y, D3DXToRadian(m_vec_rot.y));

	// 頂点変換
	D3DXVec3TransformNormal(&move, &move, &rot_m_y);
	D3DXVec3TransformNormal(&move, &move, &rot_m_x);

	// 移動値
	m_nor_rot = move;

	// 移動値
	D3DXMatrixTranslation(
		&trans_m,
		m_pos.x,
		m_pos.y,
		m_pos.z
	);

	D3DXMATRIX mat = rot_m_x * rot_m_y * trans_m;

}


float Player::GetHp() {
	return m_hp;
}


void Player::ShotBullet() {

	if (KeyBoard::IsKeyPush(DIK_RETURN)) {

		m_p_obj_factory->CreateBullet(
			Vec3(m_pos.x,
			m_pos.y,
			m_pos.z),
			BULLET_SPEED,
			SHOT_DISTANCE
		);
	}
}