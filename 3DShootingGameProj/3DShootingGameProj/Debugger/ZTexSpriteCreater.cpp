#include"ZTexSpriteCreater.h"
#include"../Lib/Shader/ShaderFunc/ZTexture/ZTextureManager/ZTextureManager.h"
#include"../Lib/Shader/ShaderFunc/ZTexture/FuncZTexture/FuncZTexture.h"



void ZTexSpriteCreater::Draw() {

	TransformData2D data;

	data.pos = Vec2(0.f, 580.f);
	data.scale = Vec2(500.f, 500.f);

	// zテクスチャ描画
	ZTextureManager::GetInstance()
		->SpriteDraw(FuncZTexture::Const::Z_TEX_1024,data);
}