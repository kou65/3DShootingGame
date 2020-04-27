
#ifndef PLAYER_H
#define PLAYER_H

#include"../CharacterBase/CharacterBase.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../CharacterInterface/CharacterInterface.h"



/**
* @brief 自機クラス
*/
class Player : public CharacterBase{
public:

	//! 最大hp
	static constexpr float MAX_HP = 4.f;
	//! hpなし
	static constexpr float NONE_HP = 0.f;

public:


	/**
	* @brief コンストラクタ
	* @param[in] pos 位置
	* @param[in] p_camera_3d カメラポインタ
	* @param[in] p_object_factory オブジェクト工場ポインタ
	*/
	Player(
		D3DXVECTOR3 pos,
		std::shared_ptr<Camera3D>p_camera_3d,
		ObjectFactory *p_object_factory
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


	/**
	* @brief 衝突後処理
	* @param[in] type CollisionObject3DBase参照
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief インタフェースを外部へ出す
	* @param[out] chara_interface 必要な情報を
	* 隠蔽化クラスに代入して外部に出す
	*/
	void OutInterface(CharacterInterface&chara_interface);


	/**
	* @brief 死んだかどうかを返す
	* @return bool 説明同上
	*/
	bool IsDeath();

private:

	//! 自機速度
	const float PLAYER_SPEED = 1.f;
	//! 弾速度
	const float BULLET_SPEED = 5.f;
	//! 弾が消える距離
	const float SHOT_DISTANCE = 300.f;


	//! 前に進む速度
	static constexpr float FRONT_SPEED = 0.5f;
	//! 横に進む速度
	static constexpr float WIDTH_SPEED = 0.5f;
	//! 縦に進む速度
	static constexpr float HEIGHT_SPEED = 0.5f;

	//! 壁から出ないように移動制限
	static constexpr float LIMIT_MOVE_LEFT = 6.f;
	static constexpr float LIMIT_MOVE_RIGHT = 26.f;
	static constexpr float LIMIT_MOVE_TOP = 16.f;
	static constexpr float LIMIT_MOVE_BOTTOM = 0.f;

	//! 連射間隔
	static constexpr float SHOT_INTERVAL = 10.f;
	//! 間隔時間
	static constexpr float INVINCIBLE_TIME = 60.f;


private:


	/**
	* @brief hp取得
	* @return float hpを返す
	*/
	float GetHp();


	/**
	* @brief キーによる移動
	*/
	void MoveByKey();


	/**
	* @brief キーによる回転
	*/
	void RotationByKey();


	/**
	* @brief 弾を撃つ
	*/
	void ShotBullet();


	/**
	* @brief 破壊弾を撃つ
	*/
	void ShotBreakBullet();


	/**
	* @brief 回転
	*/
	void Rotation();


	/**
	* @brief 移動値加算
	*/
	void AddMoveToPos();


	/**
	* @brief 移動値初期化
	*/
	void InitMove();


	/**
	* @brief 移動制限
	*/
	void MoveLimit();


	/**
	* @brief 前へ進む
	*/
	void MoveFront();


	/**
	* @brief 死亡チェック
	*/
	void CheckDeath();


	/**
	* @brief ダメージエフェクト
	*/
	void DamageEffect();


	/**
	* @brief 無敵時間確認
	*/
	void ChangeInvincibleTime();


	/**
	* @brief 無敵活動
	*/
	void ActivateInvincible();


	/**
	* @brief 無敵時間加算
	*/
	void AddInvincibleTimer();

private:

	//! 移動値
	Vec3 m_move;

	//! 回転値
	Vec3 m_vec_rot;

	//! 正規化回転値
	Vec3 m_nor_rot;

	//! プレイヤー行列
	D3DXMATRIX m_player_mat;

	//! 撃つ時間間隔
	float m_shot_timer;

	//! 無敵時間
	float m_invincible_timer;

	//! ダメージ時間
	float m_damege_timer;

	//! 無敵かどうか
	bool m_is_inv;

	//! ダメージエフェクトモード
	bool m_is_effect_mode;

	//! カラー
	D3DXVECTOR4 m_color;

	//! カメラの参照
	std::weak_ptr<Camera3D>m_p_camera_3d;

	//! 弾の生成
	ObjectFactory *m_p_obj_factory;
};

#endif