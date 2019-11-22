#pragma once
#include"../../Graphics/Graphics.h"
#include"../../Vec2/Vec2.h"
#include"../UvRect/UvRect.h"


//Uv基底クラス


class UvBase {
public:

	virtual void Update() = 0;

	UvRect GetUvRect();


protected:

	UvRect m_uv_rect;
};