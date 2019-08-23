#pragma once
#include"../../D3D9/D3D9.h"


// 前方宣言
struct TextureFormat2D;


class DrawPolygon2D {
public:

	enum SamplerStateType {

		// つなぎ目なし
		CLAMP,
		// 反転
		MIRROR,
	};

public:

	static DrawPolygon2D *GetInstance() {
		static DrawPolygon2D draw_polygon_2d;
		return &draw_polygon_2d;
	}

	// 板で描画する
	void BoardDraw(TextureFormat2D texture_format_2d);

private:

	// UV状態
	bool SetSamplerStateSelect(SamplerStateType type);

	// 行列演算
	D3DXMATRIX CalcMatrixTransform(
		const float & x,
		const float & y,
		const float & width_scale,
		const float & height_scale,
		const float & angle
	);

};
