
#ifndef PLAYER_H
#define PLAYER_H

#include"../CharacterBase/CharacterBase.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Lib/Vec2/Vec2.h"



class Player : public CharacterBase{
public:

	static constexpr float MAX_HP = 4.f;
	static constexpr float NONE_HP = 0.f;

	static constexpr float FRONT_SPEED = 0.5f;
	static constexpr float WIDTH_SPEED = 0.5f;
	static constexpr float HEIGHT_SPEED = 0.5f;
	
	static constexpr float LIMIT_MOVE_LEFT = 6.f;
	static constexpr float LIMIT_MOVE_RIGHT = 26.f;
	static constexpr float LIMIT_MOVE_TOP = 16.f;
	static constexpr float LIMIT_MOVE_BOTTOM = 0.f;
	
	static constexpr float SHOT_INTERVAL = 10.f;
	static constexpr float INVINCIBLE_TIME = 60.f;

public:

	Player(
		D3DXVECTOR3 pos,
		Camera3D*camera_3d,
		ObjectFactory*bullet_factory
	);

	void Update()override;

	void Draw()override;

	void HitAction(const CollisionObjectType&type)override;

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

	// 移動制限
	void MoveLimit();

	// 前へ進む
	void MoveFront();

	// 死亡チェック
	void CheckDeath();

	// 無敵時間
	void ChangeInvincibleTime();

	// 無敵発動
	void ActivateInvincible();

	// ダメージエフェクト
	void DamageEffect();

	void AddInvincibleTimer();

private:

	// 移動値
	Vec3 m_move;

	// 回転値
	Vec3 m_vec_rot;

	// 正規化回転値
	Vec3 m_nor_rot;

	// プレイヤー行列
	D3DXMATRIX m_player_mat;

	// 撃つ時間間隔
	float m_shot_timer;

	// 無敵時間
	float m_invincible_timer;

	// ダメージ時間
	float m_damege_timer;

	// 無敵かどうか
	bool m_is_inv;

	// ダメージエフェクトモード
	bool m_is_effect_mode;

	// カラー
	D3DXVECTOR4 m_color;

	// カメラの参照
	Camera3D * m_p_camera_3d;

	// 弾の生成
	ObjectFactory * m_p_obj_factory;
};

#endif