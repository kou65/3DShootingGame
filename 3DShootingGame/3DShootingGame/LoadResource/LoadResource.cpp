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

	// 勝利UI
	TextureManager::GetInstance()->Load("Resource/Texture/UI/clear_logo.png", "test");
	TextureManager::GetInstance()->Load("Resource/Texture/UI/title.png", "title");
	TextureManager::GetInstance()->Load("Resource/Texture/Stage/Ground.jpg", "ground");
	TextureManager::GetInstance()->Load("Resource/Texture/Enemy/total_enemy1.png", "enemy1");
	TextureManager::GetInstance()->Load("Resource/3DModel/taiki/txtr1.png", "taiki_tex");

	// Xファイル読み込み
	XFile::GetInstance()->Load("Resource/3DModel/Sample01.x", "Resource/Texture/Enemy/", "Sample01.x");

	// オブジェファイルの読み込み
	Obj::GetInstance()->Load(
		"Resource/3DModel/Player/Player.obj",
		Const::Graph::PLAYER
	);

	Obj::GetInstance()->Load(
		"Resource/3DModel/Bullet/bullet.obj",
		Const::Graph::BULLET
	);

	Obj::GetInstance()->Load(
		"Resource/3DModel/Cube.obj",
		Const::Graph::ENEMY
	);

	Obj::GetInstance()->Load(
		"Resource/3DModel/ShotEnemy/ShotEnemy.obj",
		Const::Graph::ENEMY_BULLETER
	);
}

void LoadSound() {

}