#pragma once
#include"../Sprite2DParameter/Sprite2DParameter.h"
#include"../../UV/UvSlider/UvSlider.h"
#include"../../Texture/TextureData2D/TextureData2D.h"
#include<map>
#include<memory>



class Sprite2DUser {
public:

	// シングルトン
	static Sprite2DUser *GetInstance() {
		static Sprite2DUser obj;
		return &obj;
	}

public:


	~Sprite2DUser();

	// 板で描画する
	void BoardDraw(Sprite2DParameter&out_param);


	// uv値を基準に画像サイズを変換
	void GraphSizeConvertUvSize(
		float &out_width_graph_size,
		float &out_height_graph_size,
		const int &div_num_x,
		const int &div_num_y
	);

private:

	// デフォルト
	Sprite2DUser() {};

	// uvセット
	void SetUv(
		Sprite2DParameter&data,
		const TextureData&tex_data,
		Vec2 &out_graph_size,
		UvRect&rect
	);

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

private:

	UvSlider m_uv_slider;
};
