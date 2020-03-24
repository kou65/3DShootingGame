#include "LoadResource.h"
#include "../Lib/Sound/Sound.h"
#include"../Lib/Texture/TextureManager/TextureManager.h"
#include"../Lib/3D/XFile/XFile.h"
#include"../Lib/3D/OBJ/OBJFile.h"
#include"../GameApp/GameConstant/GameConstant.h"




// テクスチャの読み込み
void LoadTexture();

// サウンドの読み込み
void LoadSound();

namespace  LoadResources {

	void Load() {

		LoadTexture();

		LoadSound();
	}

}


void LoadTexture() {

	// テクスチャ
	TextureManager::GetInstance()->Load("Resource/Texture/UI/HPGauge.png", "hp_gauge");
	TextureManager::GetInstance()->Load("Resource/Texture/Stage/Ground.jpg", "ground");
	TextureManager::GetInstance()->Load("Resource/Texture/Enemy/total_enemy1.png", "enemy1");
	TextureManager::GetInstance()->Load("Resource/3DModel/taiki/txtr1.png", "taiki_tex");
	
	TextureManager::GetInstance()->
		Load("Resource/Texture/Stage/Taile.jpg",Const::Graph::TAILE.c_str());

	TextureManager::GetInstance()->
		Load("Resource/Texture/Stage/dev.jpg", Const::Graph::BACK_GROUND.c_str());



	// Xファイル読み込み
	XFile::GetInstance()->Load("Resource/3DModel/Sample01.x", "Resource/Texture/Enemy/", "Sample01.x");

	// オブジェファイルの読み込み
	Obj::GetInstance()->Load(
		"Resource/3DModel/Player/Player.obj",
		Const::Obj::PLAYER
	);

	Obj::GetInstance()->Load(
		"Resource/3DModel/Bullet/bullet.obj",
		Const::Obj::SPEHER
	);

	Obj::GetInstance()->Load(
		"Resource/3DModel/Cube.obj",
		Const::Obj::CUBE
	);

	Obj::GetInstance()->Load(
		"Resource/3DModel/ShotEnemy/ShotEnemy.obj",
		Const::Obj::ENEMY_BULLETER
	);

	Obj::GetInstance()->Load(
		"Resource/3DModel/Plane/Plane.obj",
		Const::Obj::PLANE
	);

}

void LoadSound() {

}