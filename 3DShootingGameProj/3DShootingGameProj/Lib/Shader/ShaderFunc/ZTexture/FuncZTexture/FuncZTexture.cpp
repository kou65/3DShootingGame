#include"FuncZTexture.h"
#include"../../../../Vec2/Vec2.h"


namespace FuncZTexture {

	void Create() {

		// zテクスチャ生成
		ZTextureManager::GetInstance()->Create(
			Const::Z_TEX_1024,
			Vec2(1024,1024)
		);
	}
}