#include"../../UV/UV.h"
#include"../../Texture/TextureManager/TextureManager.h"
#include"../Sprite2DData/Sprite2DData.h"
#include"../Sprite2D/Sprite2DObject.h"



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


void Sprite2DObject::BoardDraw(Sprite2DData &texture_format) {
	
	// テクスチャが存在しているかどうか
	if (TextureManager::GetInstance()->Find2DTexture(
		texture_format.texture_name
	) == false) {
		return;
	}

	// テクスチャデータの参照受け取り
	TextureData2D *texture_data = 
		&TextureManager::GetInstance()->GetTextureData2D(
			texture_format.texture_name
		);

	// uvの範囲を設定する数
	float u_range_num = 0.f;

	const float x1 = -texture_format.ofset_x;
	const float x2 = 1.f - texture_format.ofset_x;
	const float y1 = -texture_format.ofset_y;
	const float y2 = 1.f - texture_format.ofset_y;

	// ロードで読み込んだUVを加算
	texture_format.tu += texture_data->uv.x;
	texture_format.tv += texture_data->uv.y;

	// UVの分割
	Uv uv(texture_format.tu_cut_num, texture_format.tv_cut_num);

	// uvカットがオンならば
	if (texture_format.tu_cut_num > 0 || texture_format.tv_cut_num > 0) {

		if (texture_format.type == Graphics::CLAMP) {
			uv.AnimationToTheRightDivGraph(texture_format.graph_num);
		}
		else if (texture_format.type == Graphics::MIRROR) {
			uv.AnimationToTheLeftDivGraph(texture_format.graph_num);
		}
	}

	// サンプラーステートの設定
	if (SetSamplerStateSelect(texture_format.type) == true) {
		u_range_num = 1.f;
	}

	// VERTEX3Dの初期化,UV.hからUVをずらして受け取り
	CustomVertex2D custom_vertex[] =
	{
	// 左上
	{ { x1,y1,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ uv.GetUvUpLeftPos().x + u_range_num,uv.GetUvUpLeftPos().y } },
	// 右上
	{ { x2,y1,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ uv.GetUvUpRightPos().x + texture_format.tu + u_range_num,uv.GetUvUpRightPos().y } },
	// 右下
	{ { x2,y2,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ uv.GetUvDownRightPos().x + texture_format.tu + u_range_num,uv.GetUvDownRightPos().y + texture_format.tv } },
	// 左下
	{ { x1,y2,0.f},1.0f,D3DCOLOR_ARGB(255,255,255,255),
	{ uv.GetUvDownLeftPos().x + u_range_num,uv.GetUvDownLeftPos().y + texture_format.tv } },
	};

	// 行列計算
	D3DXMATRIX mat_world = CalcMatrixTransform(
		texture_format.x,
		texture_format.y,
		texture_data->width_size * texture_format.scale_width,
		texture_data->height_size * texture_format.scale_height,
		texture_format.angle);

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
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetFVF(FVF_2D);

	// デバイスにそのまま渡すことができる。
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetTexture(
		0,
		TextureManager::GetInstance()->GetTextureData2D(
			texture_format.texture_name));

	// 図形を元にポリゴン作成
	Graphics::GetInstance()->GetLpDirect3DDevice9()->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		custom_vertex,
		sizeof(CustomVertex2D)
	);

	// テクスチャ設定リセット
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetTexture(0, NULL);

}


bool Sprite2DObject::SetSamplerStateSelect(Graphics::SamplerStateType type) {

	switch (type) {

	case Graphics::CLAMP:

		Graphics::GetInstance()->SamplerStateClamp();

		return false;

	case Graphics::MIRROR:

		Graphics::GetInstance()->SamplerStateUMirror();

		return true;
	}

	return false;
}


D3DXMATRIX Sprite2DObject::CalcMatrixTransform(
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

