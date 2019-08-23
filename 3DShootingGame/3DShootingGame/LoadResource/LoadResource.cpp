#include "LoadResource.h"
#include "../Lib/Sound/Sound.h"
#include"../Lib/Texture/TextureManager/TextureManager.h"
#include"../Lib/3D/XFile/XFile.h"



// テクスチャの読み込み
void LoadTexture();

// サウンドの読み込み
void LoadSound();


void Resource::LoadResource() {

	LoadTexture();
	
	LoadSound();
}

void LoadTexture() {

	// 勝利UI
	TextureManager::GetInstance()->Load2D("Resource/Texture/UI/clear_logo.png","test");
	TextureManager::GetInstance()->Load2D("Resource/Texture/UI/title.png","title");
	TextureManager::GetInstance()->Load2D("Resource/Texture/Stage/Ground.jpg","ground");

	// Xファイル読み込み
	XFile::GetInstance()->Load("Resource/3DModel/Sample01.x","Resource/Texture/Enemy/","Sample01.x");
}

void LoadSound() {

}