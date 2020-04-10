#pragma once
#include"../../Graphics/Graphics.h"
#include"../../Vec2/Vec2.h"
#include"../UvRect/UvRect.h"


//Uv基底クラス


class UvBase {
public:

	UvRect GetUvRect();

protected:

	void SetMaxMinUvParam(
		const Vec2&max_uv,
		const Vec2&min_uv
	);

protected:

	UvRect m_uv_rect;
};