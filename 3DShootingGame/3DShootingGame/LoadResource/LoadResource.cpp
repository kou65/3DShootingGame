#include "LoadResource.h"
#include "../Lib/Sound/Sound.h"
#include"../Lib/Texture/TextureManager/TextureManager.h"
#include"../Lib/3D/XFile/XFile.h"
#include"../Lib/3D/OBJ/OBJFile.h"



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
	TextureManager::GetInstance()->Load("Resource/Texture/UI/clear_logo.png","test");
	TextureManager::GetInstance()->Load("Resource/Texture/UI/title.png","title");
	TextureManager::GetInstance()->Load("Resource/Texture/Stage/Ground.jpg","ground");
	TextureManager::GetInstance()->Load("Resource/Texture/Enemy/total_enemy1.png","enemy1");
	TextureManager::GetInstance()->Load("Resource/3DModel/taiki/txtr1.png","taiki_tex");

	// Xファイル読み込み
	XFile::GetInstance()->Load("Resource/3DModel/Sample01.x","Resource/Texture/Enemy/","Sample01.x");

	// オブジェファイルの読み込み
	//ObjFile::GetInstance()->Load(
	//	"Resource/3DModel/NoteBook/Lowpoly_Notebook_2.obj",
	//	"note_book",
	//	"Resource/3DModel/NoteBook/textures/"
	//);


	//ObjFile::GetInstance()->Load(
	//	"Resource/3DModel/85-cottage_obj/cottage_obj.obj",
	//	"1",
	//	"Resource/3DModel/85-cottage_obj/"
	//);

	
	//ObjFile::GetInstance()->Load(
	//	"Resource/3DModel/deer/deer.obj",
	//	"deer",
	//	"Resource/3DModel/deer/"
	//);


	ObjFile::GetInstance()->Load(
		"Resource/3DModel/untitled.obj",
		"cube",
		"Resource/3DModel/"
	);
}

void LoadSound() {

}