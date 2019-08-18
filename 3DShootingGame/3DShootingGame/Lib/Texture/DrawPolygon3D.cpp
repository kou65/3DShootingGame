#include"DrawPolygon3D.h"
#include"../D3D9/D3D9.h"
#include"../UV/UV.h"
#include"TextureFormat3D.h"
#include"../../SetRenderStateFile/SetRenderStateFile.h"



// 回転順序は固定


struct CustomVertex3D {

	// 頂点情報
	D3DXVECTOR3 vertex;

	// 頂点カラー
	DWORD color;

	// テクスチャ座標
	D3DXVECTOR2 uv;
};


// テンプレートFVF3D
#define FVF_3D (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)


void DrawPolygon3D::BoardDraw(
	TextureFormat3D texture_format_3d
) {

	// ライトオフ
	SetRenderStateFile::LightMode(FALSE);
	D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_LIGHTING, NULL);

	// trueなら背面カリングモード無し
	D3D9::GetLpDirect3DDevice9()->SetRenderState(
		D3DRS_CULLMODE, texture_format_3d.is_back_cull_mode ?
		D3DCULL_CCW : D3DCULL_NONE
	);

	D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	// 0.5fで中心にする。
	float x1 = 1.f - texture_format_3d.ofset.x;
	float x2 = texture_format_3d.ofset.x;
	float y1 = 1.f - texture_format_3d.ofset.y;
	float y2 = texture_format_3d.ofset.y;


	// UV切り取り
	UV uv(
		texture_format_3d.tu_cut_num,
		texture_format_3d.tv_cut_num);

	if (texture_format_3d.is_graph_uv_cut == true) {
		uv.AnimationToTheRightDivGraph(texture_format_3d.graph_num);
	}


	// トライアングルリスト
	CustomVertex3D custom_vertex[6] =
	{

	// 一つ目のポリゴン
	{{ x1,y1,0.f },
	D3DXCOLOR(255,255,255,1),
	{-1.f,-1.f},
	},

	{
	{ x2,y1,0.f },
	D3DXCOLOR(255,255,255,1),
	{0.f,-1.f}
	},

	{
	{ x1,y2,0.f },
	D3DXCOLOR(255,255,255,1),
	{-1.f,0.f}
	},
		
	
		// 二つ目のポリゴン
	{
	{ x1,y2,0.f },
	D3DXCOLOR(255,255,255,1),
	{ -1.f,0.f }
	},



	{ { x2,y1,0.f },
		D3DXCOLOR(255,255,255,1),
	{ 0.f,-1.f }
	},

	{
		{ x2,y2,0.f },
		D3DXCOLOR(255,255,255,1),
	{ 0.f,0.f }
	},

	};

	// 行列変換
	D3DXMATRIX matrix_world;

	// 行列計算
	matrix_world = CalcMatrixTransform(
		texture_format_3d.pos,
		D3DXVECTOR3(texture_format_3d.scale_width,
		texture_format_3d.scale_height,1.f),
		texture_format_3d.polygon_dir
	);


	// ワールド座標変換
	D3D9::GetLpDirect3DDevice9()->SetTransform(D3DTS_WORLD,&matrix_world);

	// どのように描画するか
	D3D9::GetLpDirect3DDevice9()->SetFVF(FVF_3D);

	// テクスチャ描画しない
	if (texture_format_3d.texture_name != NULL) {

		// テクスチャをセット
		D3D9::GetLpDirect3DDevice9()->SetTexture(
			0,
			TextureManager::GetInstance()->GetTextureData2D(
			texture_format_3d.texture_name).p_texture_buffer);
	}
	else {
		D3D9::GetLpDirect3DDevice9()->SetTexture(0, NULL);
	}
	
	// プリミティブを形成(プリミティブの数は極力減らした方がいい)
	D3D9::GetLpDirect3DDevice9()->
		DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			2,
			custom_vertex,
			sizeof(CustomVertex3D)
		);

	// テクスチャ設定リセット
	D3D9::GetLpDirect3DDevice9()->SetTexture(0, NULL);
}



D3DXMATRIX DrawPolygon3D::CalcMatrixTransform(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 scale,
	PolygonDirection dir) {

	// 移動行列
	D3DXMATRIX matrix_trans;

	// 拡縮行列
	D3DXMATRIX matrix_scale;

	// 回転合成行列
	D3DXMATRIX matrix_rotation_total;

	// 世界行列
	D3DXMATRIX matrix_world;

	// 移動
	D3DXMatrixTranslation(
		&matrix_trans,
		pos.x,
		pos.y,
		pos.z
	);

	// 拡縮
	D3DXMatrixScaling(
		&matrix_scale,
		scale.x,
		scale.y,
		scale.z
	);

	// 回転後の行列を返す
	matrix_rotation_total = CalcMatrixRotation(dir);

    // 行列合成 回転 * 拡縮 * 移動
	matrix_world = 
		matrix_scale *
		matrix_rotation_total *  
		matrix_trans;

	// 世界行列を返す
	return matrix_world;
}


D3DXMATRIX DrawPolygon3D::CalcMatrixRotation(
	PolygonDirection dir
) {

	// 回転値(デフォルトで逆転させる)
	D3DXVECTOR3 rotation(0.f,180.f,0.f);

	switch (dir) {

	case FRONT:
		rotation.z = 0.f;
		break;

	case BACK:
		rotation.y = 180.f;
		break;

	case RIGHT:
		rotation.y = -90.f;
		break;

	case LEFT:
		rotation.y = 90.f;
		break;

	case FLOOR:
		rotation.x = -90.f;
		break;
	}

	// 回転行列
	D3DXMATRIX matrix_rotation_x;
	D3DXMATRIX matrix_rotation_y;
	D3DXMATRIX matrix_rotation_z;
	D3DXMATRIX matrix_total;

	// 回転
	D3DXMatrixRotationX(
		&matrix_rotation_x,
		D3DXToRadian(rotation.x)
	);

	D3DXMatrixRotationY(
		&matrix_rotation_y,
		D3DXToRadian(rotation.y)
	);

	D3DXMatrixRotationZ(
		&matrix_rotation_z,
		D3DXToRadian(rotation.z)
	);

	// 回転行列合成 Z * X * Y
	return(
		matrix_rotation_z *
		matrix_rotation_x *
		matrix_rotation_y
		);

}