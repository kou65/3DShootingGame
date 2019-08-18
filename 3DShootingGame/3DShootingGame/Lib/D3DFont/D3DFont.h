#pragma once
#include<d3dx9.h>
#include"../D3D9/D3D9.h"

#pragma comment(lib,"d3dx9.lib")


namespace D3DFont {


	// 初期化
	bool Init(
		LONG right = 100,
		LONG left = 100,
		LONG top = 100, 
		LONG bottom = 100
	);

	// フォントの色を変更する
	void SetFontColor(
		int r,
		int g,
		int b
	);

	// 描画
	void Draw(
		const float &pos_x,
		const float &pos_y,
		int format_size,
		const char*format, ...);

	// 描画開始
	void Begin();

	// 描画終了
	void End();
}