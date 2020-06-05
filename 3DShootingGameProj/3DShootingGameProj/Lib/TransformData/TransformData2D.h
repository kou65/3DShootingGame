#pragma once
#include"../Vec2/Vec2.h"


/**
* @brief 変換用2Dデータ
*/
struct TransformData2D {

	TransformData2D() {

		scale.x = 1.f;
		scale.y = 1.f;
	}

	TransformData2D(
		const Vec2&pos,
		const Vec2 &scale,
		const Vec2&rota
	) {
		this->pos = pos;
		this->scale = scale;
		this->rota = rota;
	}

	Vec2 pos;
	Vec2 scale;
	Vec2 rota;
};