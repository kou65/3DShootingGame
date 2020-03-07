#include"../../Uv/UvAnimation/UvAnimation.h"
#include"../../Texture/TextureManager/TextureManager.h"
#include"../Sprite2DParameter/Sprite2DParameter.h"
#include"../../Sprite2D/Sprite2D/Sprite2DUser.h"



// ---頂点を設定する構造体---
struct CustomVertex2D
{
	D3DXVECTOR3 vertex;  // 頂点座標(Z座標は定義しておかないと3Dでおかしくなる)
	FLOAT rhw;           // 除算数
	DWORD color;         // カラー
	D3DXVECTOR2 uv;      // テクスチャ座標
};


// テンプレートFVF2D
#define FVF_2D (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE)


Sprite2DUser::~Sprite2DUser() {

}


void Sprite2DUser::BoardDraw(Sprite2DParameter&data) {
	

	// テクスチャデータの参照受け取り
	TextureData *tex_data = 
		&TextureManager::GetInstance()->GetTextureData(
			data.texture_name
		);

	// uvの範囲を設定する数
	float u_range_num = 0.f;

	const float x1 = -data.ofset_x;
	const float x2 = 1.f - data.ofset_x;
	const float y1 = -data.ofset_y;
	const float y2 = 1.f - data.ofset_y;

	// uv値
	UvRect rect;

	// tex画像サイズ
	Vec2 graph_size(tex_data->width_size, tex_data->height_size);

	// uv設定
	{
		// ロードで読み込んだUVを加算
		data.tu += tex_data->uv.x;
		data.tv += tex_data->uv.y;

		AnimationParamter param = data.animation_param;

		// UVの分割
		UvAnimation uv_anim(
			param.division_width,
			param.division_height,
			param.current_graph_num,
			param.is_animation,
			param.animation_frame,
			param.use_type,
			param.turn_type
		);

		// カットがオンなら
		if (param.is_uv_cut == true) {

			if (data.type == Graphics::CLAMP) {
				uv_anim.ChangeRightAnimation();
			}
			else if (data.type == Graphics::MIRROR) {
				uv_anim.ChangeLeftAnimation();
			}

			// アニメーションがオンなら
			if (param.is_animation == true) {
				// 更新
				uv_anim.Update();
				// アニメーション変更更新
				uv_anim.UpdateAnimation(data.animation_param);
			}
		}

		// スライダーオンなら
		if (data.is_uv_slider == true) {

			// 画像サイズ And UVサイズ
			graph_size = m_uv_slider.
				CalcGraphSizeSetUv(
					data.max_uv_slider,
					data.min_uv_slider,
					graph_size
				);
		}

		// アニメーション
		UvRect rect1 = uv_anim.GetUvRect();
		// スライダーの矩形受け取り
		UvRect rect2 = m_uv_slider.GetUvRect();

		// スライダーの方を優先にする
		if (data.is_uv_slider == true) {
			rect1 = rect2;
		}

		// 本代入
		rect = rect1;

		// サンプラーステートの設定
		if (SetSamplerStateSelect(data.type) == true) {
			u_range_num = 1.f;
		}

	}
	
	// VERTEX3Dの初期化,UV.hからUVをずらして受け取り
	CustomVertex2D custom_vertex[] =
	{
	// 左上
	{ { x1,y1,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ rect.top_left_pos.x + u_range_num,rect.top_left_pos.y} },
	// 右上
	{ { x2,y1,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ rect.top_right_pos.x + data.tu + u_range_num,rect.top_right_pos.y } },
	// 右下
	{ { x2,y2,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ rect.bottom_right_pos.x + data.tu + u_range_num,rect.bottom_right_pos.y + data.tv } },
	// 左下
	{ { x1,y2,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ rect.bottom_left_pos.x + u_range_num,rect.bottom_left_pos.y + data.tv } },
	};

	// 行列計算
	D3DXMATRIX mat_world = CalcMatrixTransform(
		data.x,
		data.y,
		graph_size.x * data.scale_width,
		graph_size.y * data.scale_height,
		data.angle);

	// 行列を頂点配列に変換
	D3DXVec2TransformCoordArray(
		// 操作の結果であるD3DXVECTOR3のポインタ
		(D3DXVECTOR2*)custom_vertex,
		// 出力データストリーム内のベクトル間を移動する
		sizeof(CustomVertex2D),
		// D3DXVECTOR3配列へのポインタ
		(D3DXVECTOR2*)custom_vertex,
		sizeof(CustomVertex2D),
		// 変換する行列
		&mat_world, 
		// 配列内の要素数
		(UINT)std::size(custom_vertex));


	// VERTEX3Dの構造情報をDirectXへ通知。
	Graphics::GetInstance()->GetDevice()->SetFVF(FVF_2D);

	// テクスチャが存在するなら
	if (TextureManager::GetInstance()->FindTexture(
		data.texture_name
	) == true) {

		// デバイスにそのまま渡すことができる。
		Graphics::GetInstance()->GetDevice()->SetTexture(
			0,
			TextureManager::GetInstance()->GetTextureData(
				data.texture_name));

	}

	// 図形を元にポリゴン作成
	Graphics::GetInstance()->GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		custom_vertex,
		sizeof(CustomVertex2D)
	);

	// テクスチャ設定リセット
	Graphics::GetInstance()->GetDevice()->SetTexture(0, NULL);
	Graphics::GetInstance()->GetDevice()->SetFVF(NULL);

}


void Sprite2DUser::GraphSizeConvertUvSize(
	float &out_width_graph_size,
	float &out_height_graph_size,
	const int &div_num_x,
	const int &div_num_y
) {

	// 分割画像サイズ出力
	out_width_graph_size = out_width_graph_size / div_num_x;
	out_height_graph_size = out_height_graph_size / div_num_y;
}


bool Sprite2DUser::SetSamplerStateSelect(Graphics::SamplerStateType type) {

	switch (type) {

	case Graphics::CLAMP:

		Graphics::GetInstance()->SamplerStateClamp();

		return true;

	case Graphics::WARP:

		Graphics::GetInstance()->SamplerStateWrap();

		return true;

	case Graphics::MIRROR:

		Graphics::GetInstance()->SamplerStateUMirror();

		return true;
	}

	return false;
}


D3DXMATRIX Sprite2DUser::CalcMatrixTransform(
	const float &x,
	const float &y,
	const float &width_scale,
	const float &height_scale,
	const float &angle
){

	// ワールド座標変換系
	D3DXMATRIX mat_trans, mat_scale, mat_rotz;

	D3DXMatrixIdentity(&mat_rotz);
	D3DXMatrixIdentity(&mat_trans);
	D3DXMatrixIdentity(&mat_scale);

	D3DXMatrixTranslation(&mat_trans, x, y, 0.f);
	D3DXMatrixScaling(&mat_scale, width_scale, height_scale, 0.0f);
	D3DXMatrixRotationZ(&mat_rotz, D3DXToRadian(angle));

	// 拡縮 * 回転 * 移動
	return (mat_scale * mat_rotz * mat_trans);
}

