#include"D3DFont.h"
#include<string>
#include<stdarg.h>
#include"../../RenderState/RenderState.h"



namespace D3DFont {

	// スプライトインターフェース
	LPD3DXSPRITE p_sprite;

	// フォントインターフェース
	LPD3DXFONT p_d3dx_font;

	// フォントの大きさ
	RECT rect;

	// カラー
	D3DXCOLOR color = {};

	// フォントカラーの初期化
	void InitFontColor();


	bool Init(LONG right, LONG left, LONG top, LONG bottom) {

		// リザルト
		HRESULT result;

		// スプライト生成
		D3DXCreateSprite(Graphics::GetInstance()->GetDevice(), &p_sprite);

		// フォント生成
		result = D3DXCreateFont(
			Graphics::GetInstance()->GetDevice(),
			// 高さ
			16,
			// 平均文字幅
			0,
			// 太さ
			FW_NORMAL,
			// ミップマップレベル
			1,
			// 斜体
			FALSE,
			// キャラクタ・セット(シフトJISを指定)
			SHIFTJIS_CHARSET,
			// 出力精度
			OUT_DEFAULT_PRECIS,
			// 品質
			DEFAULT_QUALITY,
			// ピッチ
			DEFAULT_PITCH || FF_DONTCARE,
			// フォント名
			"ＭＳ　Ｐゴシック",
			// フォントインターフェース
			&p_d3dx_font
		);

		if (FAILED(result)) {
			// 失敗
			return false;
		}

		// フォントの大きさ変更
		rect.right = right;
		rect.left = left;
		rect.top = top;
		rect.bottom = bottom;

		// フォントカラーの初期化
		InitFontColor();

		return true;
	}


	void SetFontColor(int r, int g, int b) {
		// カラーの変更
		color.r = (float)r;
		color.g = (float)g;
		color.b = (float)b;
	}


	void InitFontColor() {
		// カラーの初期化
		color.r = 255;
		color.g = 255;
		color.b = 255;
	}


	void Draw(
		const float &pos_x,
		const float &pos_y,
		int format_size,
		const char*format, ...) {

		// 受け取り用の文字列メモリ確保
		char * string = new char[format_size + 1];

		// 可変長引数処理
		{
			// 可変長引数を操作するために必要な構造体
			va_list list;
		
			// va_list構造体変数と直前にある引数を入れる
			va_start(list, format);
		
			// va_list構造体変数をformatに入れる
			vsprintf_s(string,format_size, format, list);
		
			// 終了
			va_end(list);
		}


		// α値有効
		RenderState::AlphaEnable(TRUE);
		RenderState::AlphaTest(TRUE);

		// スプライト描画開始
		p_sprite->Begin(0);

		// 行列
		D3DXMATRIX d3dx_matrix;

		// 移動値
		D3DXMatrixTranslation(&d3dx_matrix,pos_x,pos_y,0.f);

		// 行列変換
		p_sprite->SetTransform(&d3dx_matrix);

		// 描画
		p_d3dx_font->DrawText(
			p_sprite,
			string,
			-1,
			&rect,
			DT_LEFT | DT_NOCLIP,
			D3DCOLOR_XRGB((int)color.r,(int)color.g,(int)color.b)
		);

		// スプライト描画終了
		p_sprite->End();

		// α値無効
		RenderState::AlphaEnable(FALSE);
		RenderState::AlphaTest(FALSE);

		// 文字列メモリ解放
		delete[] string;

		// フォントカラー初期化
		InitFontColor();
	}
}