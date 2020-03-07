
#ifndef PLAYER_H
#define PLAYER_H

#include"../CharacterBase/CharacterBase.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Lib/Vec2/Vec2.h"



class Player : public CharacterBase{
public:

	static constexpr float MAX_HP = 10;

public:

	Player(
		D3DXVECTOR3 pos,
		Camera3D*camera_3d,
		ObjectFactory*bullet_factory
	);

	void Update()override;

	void Draw()override;

	float GetHp();

private:

	const float PLAYER_SPEED = 1.f;
	const float BULLET_SPEED = 5.f;
	const float SHOT_DISTANCE = 300.f;

private:

	// キーによる移動
	void MoveByKey();

	// キーによる回転
	void RotationByKey();

	// 弾を撃つ
	void ShotBullet();

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
	ObjectFactory * m_p_obj_factory;

	// 移動値
	Vec3 m_move;

	// 回転値
	Vec3 m_vec_rot;

	// 正規化回転値
	Vec3 m_nor_rot;

	// プレイヤー行列
	D3DXMATRIX m_player_mat;
};

#endif