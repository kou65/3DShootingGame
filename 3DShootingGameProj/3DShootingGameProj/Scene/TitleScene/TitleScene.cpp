﻿#include"TitleScene.h"
#include"../../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"


void TitleScene::Update(SceneType&scene_type){

	ChangeGameScene(scene_type);
}


void TitleScene::Draw(){

	Sprite2DParameter param;

	param.texture_name = "ground";
	param.tex_size = D3DXVECTOR2(1920.f, 1080.f);

	Sprite2DUser::GetInstance()->BoardDraw(param);
}


void TitleScene::ChangeGameScene(SceneType&scene_type) {

	if (KeyBoard::IsKeyPush(DIK_RETURN) == true) {
		scene_type = SceneType::GAME;
	}
}