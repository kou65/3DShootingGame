#pragma once
#include"../Sprite2DData/Sprite2DData.h"



class Sprite2D {
public:

	static Sprite2D *GetInstance() {
		static Sprite2D draw_polygon_2d;
		return &draw_polygon_2d;
	}

	// 板で描画する
	void BoardDraw(Sprite2DData texture_format_2d);

private:

	// UV状態
	bool SetSamplerStateSelect(Graphics::SamplerStateType type);

	// 行列演算
	D3DXMATRIX CalcMatrixTransform(
		const float & x,
		const float & y,
		const float & width_scale,
		const float & height_scale,
		const float & angle
	);

};
