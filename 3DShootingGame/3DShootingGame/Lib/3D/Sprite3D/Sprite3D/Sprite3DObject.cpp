#include"../Sprite3D/Sprite3DObject.h"
#include"../../../Graphics/Graphics.h"
#include"../../../UV/UV.h"
#include"../../Sprite3D/Sprite3DData/Sprite3DData.h"
#include"../../../../SetRenderStateFile/SetRenderStateFile.h"




void Sprite3DObject::BoardDraw(
	Sprite3DData sprite_data
) {

	// ライトオフ
	SetRenderStateFile::LightMode(FALSE);
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetRenderState(D3DRS_LIGHTING, NULL);

	// trueなら背面カリングモード無し
	//Graphics::GetInstance()->GetLpDirect3DDevice9()->SetRenderState(
	//	D3DRS_CULLMODE, sprite_data.is_back_cull_mode ?
	//	D3DCULL_CCW : D3DCULL_NONE
	//);

	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	// 0.5fで中心にする。
	float x1 = sprite_data.ofset.x - 1.f;
	float x2 = sprite_data.ofset.x;
	float y1 = sprite_data.ofset.y - 1.f;
	float y2 = sprite_data.ofset.y;

	// UV切り取り
	UV uv(
		sprite_data.tu_cut_num,
		sprite_data.tv_cut_num);

	if (sprite_data.is_graph_uv_cut == true) {
		uv.AnimationToTheRightDivGraph(sprite_data.graph_num);
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
		sprite_data.pos,
		D3DXVECTOR3(sprite_data.scale_width,
		sprite_data.scale_height,1.f),
		sprite_data.polygon_dir
	);


	// ワールド座標変換
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetTransform(D3DTS_WORLD,&matrix_world);

	// どのように描画するか
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetFVF(FVF_3D);

	// テクスチャ描画しない
	if (sprite_data.texture_name != NULL) {

		// テクスチャをセット
		Graphics::GetInstance()->GetLpDirect3DDevice9()->SetTexture(
			0,
			TextureManager::GetInstance()->GetTextureData2D(
			sprite_data.texture_name).p_texture_buffer);
	}
	else {
		Graphics::GetInstance()->GetLpDirect3DDevice9()->SetTexture(0, NULL);
	}
	
	// プリミティブを形成(プリミティブの数は極力減らした方がいい)
	Graphics::GetInstance()->GetLpDirect3DDevice9()->
		DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			2,
			custom_vertex,
			sizeof(CustomVertex3D)
		);

	// テクスチャ設定リセット
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetTexture(0, NULL);

	D3DXMATRIX init_matrix;

	D3DXMatrixIdentity(&init_matrix);
	
	// ワールド座標初期化
	Graphics::GetInstance()->GetLpDirect3DDevice9()->SetTransform(D3DTS_WORLD,&init_matrix);
}


void Sprite3DObject::ShapeBoard(
	CustomVertex3D * custom_vertex3D,
	int vertex_num,
	float right,
	float left,
	float top,
	float bottom,
	DWORD color
) {

	// 1ポリゴン
	{
		custom_vertex3D[0].vertex.x = left;
		custom_vertex3D[0].vertex.y = top;
		custom_vertex3D[0].color = color;
		custom_vertex3D[0].uv.x = -1.f;
		custom_vertex3D[0].uv.y = -1.f;


		custom_vertex3D[1].vertex.x = right;
		custom_vertex3D[1].vertex.y = top;
		custom_vertex3D[1].color = color;
		custom_vertex3D[1].uv.x = 0.f;
		custom_vertex3D[1].uv.y = -1.f;


		custom_vertex3D[2].vertex.x = left;
		custom_vertex3D[2].vertex.y = bottom;
		custom_vertex3D[2].color = color;
		custom_vertex3D[2].uv.x = -1.f;
		custom_vertex3D[2].uv.y = 0.f;
	}

	// 2ポリゴン
	{
		custom_vertex3D[3].vertex.x = left;
		custom_vertex3D[3].vertex.y = top;
		custom_vertex3D[3].uv.x = -1.f;
		custom_vertex3D[3].uv.x = 0.f;
		custom_vertex3D[3].color = color;


		custom_vertex3D[4].vertex.x = right;
		custom_vertex3D[4].vertex.y = top;
		custom_vertex3D[4].uv.x = 0.f;
		custom_vertex3D[4].uv.y = -1.f;
		custom_vertex3D[4].color = color;


		custom_vertex3D[5].vertex.x = right;
		custom_vertex3D[5].vertex.y = bottom;
		custom_vertex3D[5].uv.x = 0.f;
		custom_vertex3D[5].uv.y = 0.f;
		custom_vertex3D[5].color = color;
	}


}


D3DXMATRIX Sprite3DObject::CalcMatrixTransform(
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


D3DXMATRIX Sprite3DObject::CalcMatrixRotation(
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