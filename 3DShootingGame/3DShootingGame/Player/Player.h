#pragma once
#include"../Object3D/Object3D.h"
#include"../Lib/3D/Camera3D/Camera3D.h"
#include"../BulletFactory/BulletFactory.h"



class Player : public Object3D{
public:

	Player(
		D3DXVECTOR3 pos,
		Camera3D*camera_3d,
		BulletFactory*bullet_factory);


	void Update()override;

	void Draw()override;

private:

	// 移動
	void Move();

	// 弾を撃つ
	void BulletShot();

private:

	int material_num;

	// カメラの参照
	Camera3D * mp_camera_3d;

	BulletFactory * mp_bullet_factory;
};