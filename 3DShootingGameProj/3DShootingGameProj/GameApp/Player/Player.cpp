#include"../Player/Player.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../../Lib/D3DFont/D3DFont.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../CollisionSystem/CollisionManager/CollisionManager.h"
#include<math.h>



Player::Player(
	D3DXVECTOR3 pos,
	Camera3D*camera_3d,
	ObjectFactory*bullet_factory
) {

	// 各オブジェクト代入
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
	
	// hp
	m_hp = MAX_HP;

	// 撃つ間隔時間
	m_shot_timer = SHOT_INTERVAL;

	m_damege_timer = 0;

	// エフェクトモード
	m_is_effect_mode = false;

	// カラー代入
	m_color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	CollisionManager::GetInstance()->Entry(
		CollisionObjectType::PLAYER, this
	);

	// 球
	m_shape_type = ShapeType::SPHERE;
}


void Player::Update() {

	// 移動値加算
	AddMoveToPos();

	// 無敵加算
	AddInvincibleTimer();

	// 無敵時間
	ChangeInvincibleTime();

	// 移動値初期化
	InitMove();

	// キー移動
	MoveByKey();

	// 前へ移動
	MoveFront();

	// キー回転
	RotationByKey();

	// 回転
	Rotation();

	// 弾発射
	ShotBullet();

	// 移動制限
	MoveLimit();

	// 死んだかどうか
	CheckDeath();

	// ダメージエフェクト
	DamageEffect();
}


void Player::Draw() {

	ObjParameter param;

	param.pos.z = 0.f;
	param.pos = m_pos;
	param.register_obj_file_name = Const::Obj::PLAYER;

	param.rota.x = m_vec_rot.y;
	param.rota.y = 90.f + m_vec_rot.x;
	param.color = m_color;

	// 描画
	Obj::GetInstance()->ShaderDraw(
		param
	);

	D3DFont::Draw(150.f, 100.f, 100,"p_pos_x=>%f",m_pos.x);
	D3DFont::Draw(150.f, 110.f, 100,"p_pos_y=>%f", m_pos.y);
	D3DFont::Draw(150.f, 120.f, 100,"p_pos_z=>%f", m_pos.z);
}


void Player::HitAction(const CollisionObjectType&type) {

	// 敵の弾なら
	if (type == CollisionObjectType::ENEMY_BULLET) {
		
		// 無敵でないなら
		if (m_is_inv == false) {
			// 体力減算
			AddDamage(1.f);
			ActivateInvincible();
			m_is_effect_mode = true;
		}
	}

	// 敵なら
	if (type == CollisionObjectType::ENEMY) {

		if (m_is_inv == false) {
			// 体力減算
			AddDamage(1.f);
			ActivateInvincible();
			m_is_effect_mode = true;
		}
	}

	// ブロックなら
	if (type == CollisionObjectType::BLOCK) {

		// 無敵なら
		if (m_is_inv == false) {

			// 体力減算
			AddDamage(1.f);
			ActivateInvincible();
			m_is_effect_mode = true;
		}
	}
}


void Player::MoveLimit() {

	if (m_pos.x < LIMIT_MOVE_LEFT) {

		m_pos.x = LIMIT_MOVE_LEFT;
	}

	if (m_pos.x > LIMIT_MOVE_RIGHT) {

		m_pos.x = LIMIT_MOVE_RIGHT;
	}

	if (m_pos.y < LIMIT_MOVE_BOTTOM) {
		m_pos.y = LIMIT_MOVE_BOTTOM;
	}

	if (m_pos.y > LIMIT_MOVE_TOP) {
		m_pos.y = LIMIT_MOVE_TOP;
	}
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
		if (KeyBoard::IsKeyPushing(DIK_UP)) {
			m_move.y = HEIGHT_SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_DOWN)) {
			m_move.y = -HEIGHT_SPEED;
		}

		// 左右
		if (KeyBoard::IsKeyPushing(DIK_RIGHT)) {
			m_move.x = WIDTH_SPEED;
		}
		else if (KeyBoard::IsKeyPushing(DIK_LEFT)) {
			m_move.x = -WIDTH_SPEED;
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

	m_pos += m_move;
}


void Player::MoveFront() {

	m_p_camera_3d->AddPos(D3DXVECTOR3(0.f, 0.f, FRONT_SPEED));
	m_move.z += FRONT_SPEED;
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


void Player::AddInvincibleTimer() {

	if(m_is_inv == false){
		return;
	}

	// 指定時間になるまで加算
	if (m_invincible_timer <= INVINCIBLE_TIME) {
		m_invincible_timer++;
	}
	
}


void Player::ChangeInvincibleTime() {

	// 指定時間になったら無敵を解除
	if (m_invincible_timer >= INVINCIBLE_TIME) {
		m_is_inv = false;
		m_invincible_timer = 0;
	}
	
}


float Player::GetHp() {
	return m_hp;
}


void Player::CheckDeath() {

	// hpがないなら活動停止
	if (GetHp() <= 0) {
		m_is_active = false;
	}
}


void Player::ActivateInvincible() {

	// 無敵時間
	m_is_inv = true;
}


void Player::DamageEffect() {


	// エフェクトモードでないなら
	if (m_is_effect_mode == false) {
		m_can_draw = true;
		return;
	}
	

	// 描画するしないを繰り返す
	m_can_draw =! m_can_draw;

	// 無敵時間が解除されたらダメージエフェクトモードも解除
	m_is_effect_mode = m_is_inv;

}


void Player::ShotBullet() {

	// Sキーで撃つ
	if (KeyBoard::IsKeyPushing(DIK_X)) {

		if (m_shot_timer >= SHOT_INTERVAL) {

			m_p_obj_factory->CreateBullet(
				Vec3(m_pos.x,
					m_pos.y,
					m_pos.z),
				BULLET_SPEED,
				SHOT_DISTANCE
			);

			// 時間を0にする
			m_shot_timer = 0;
		}
	}

	// 最大まで
	if (m_shot_timer <= SHOT_INTERVAL) {
		// 加算
		m_shot_timer++;
	}

}