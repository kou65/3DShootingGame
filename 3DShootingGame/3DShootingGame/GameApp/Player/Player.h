
#ifndef PLAYER_H
#define PLAYER_H

#include"../CharacterBase/CharacterBase.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"
#include"../../GameApp/BulletFactory/BulletFactory.h"
#include"../../Lib/Vec2/Vec2.h"



class Player : public CharacterBase{
public:

	Player(
		D3DXVECTOR3 pos,
		Camera3D*camera_3d,
		BulletFactory*bullet_factory);

	void Update()override;

	void Draw()override;

private:

	const float SPEED = 1.f;

private:

	// 移動
	void Move();

	// 弾を撃つ
	void ShotBullet();

	// 角度
	void SetRadius(const float&rad);

	// 回転
	void Rotation();

	// 移動値加算
	void AddMoveToPos();

	// 移動値初期化
	void InitMove();

private:

	// カメラの参照
	Camera3D * m_p_camera_3d;

	// 弾の生成
	BulletFactory * m_p_bullet_factory;

	// 移動値
	Vec3 m_move;

	// 半径
	Vec2 m_radius;

	// プレイヤー行列
	D3DXMATRIX m_player_mat;

	// 体力
	float m_hp;
};

#endif