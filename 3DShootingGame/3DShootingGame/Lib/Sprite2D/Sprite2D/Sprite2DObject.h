#pragma once
#include"../Sprite2DParameter/Sprite2DParameter.h"
#include<map>
#include<memory>



class Sprite2DObject {
public:

	// シングルトン
	static Sprite2DObject *GetInstance() {
		static Sprite2DObject obj;
		return &obj;
	}

public:


	~Sprite2DObject();

	// 板で描画する
	void BoardDraw(Sprite2DData&out_param);


	// uv値を基準に画像サイズを変換
	void GraphSizeConvertUvSize(
		float &out_width_graph_size,
		float &out_height_graph_size,
		const int &div_num_x,
		const int &div_num_y
	);

private:

	// デフォルト
	Sprite2DObject() {};

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
