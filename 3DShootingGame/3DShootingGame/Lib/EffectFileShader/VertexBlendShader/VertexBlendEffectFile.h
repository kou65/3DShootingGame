#pragma once
#include"../../Graphics/Graphics.h"
#include"../ShaderBase/EffectFileBase.h"


class VertexBlendEffectFile : public ShaderBase{
public:

	VertexBlendEffectFile();

	// 初期化
	void Init()override;

	// 更新
	void Update()override;

	// ボーン行列セット
	void SetBoneMatrix(D3DXMATRIX*bone_mat);

	void SetBoneMatrixArray(
		D3DXHANDLE&handle,
		D3DXMATRIX*mat,
		const int &array_num
	);

	// ボーン数
	void SetBoneCount(const int&count);

	// 行列インデックス
	void SetMaxIndex(const int&index);

	// 行列登録
	void RegisterMat(
		const int &start_regster,
		const D3DXMATRIX*mat,
		const int &vector4count
	);

	void UpdateParam();

	void SetColor(const D3DXVECTOR4 &color);
	void SetTexture(IDirect3DTexture9* p_tex);
	void SetWorldMatrix(const D3DXMATRIX&world_mat);

private:

	//void Register();

private:

	// ボーン数
	int m_bone_count;

	// 最大ブレンド数
	int m_max_index;

	// ビュー行列ハンドル
	D3DXHANDLE m_camera_view_mat_h;
	// 射影行列ハンドル
	D3DXHANDLE m_camera_proj_mat_h;
	// ワールド行列ハンドル
	D3DXHANDLE m_world_mat_h;
	// テクスチャハンドル
	D3DXHANDLE m_texture_h;
	// カラーハンドル
	D3DXHANDLE m_color_h;


	// ワールド行列
	D3DXMATRIX m_world_mat;
	// カメラビュー行列
	D3DXMATRIX m_camera_view_mat;
	// カメラ射影変換行列
	D3DXMATRIX m_camera_proj_mat;
	// カラー値
	D3DXVECTOR4 m_color;



	// ボーン行列
	D3DXMATRIX *m_bone_mat_list;

	// ボーン行列ハンドル
	D3DXHANDLE m_bone_mat_h;

	// ハンドル
	D3DXHANDLE m_total_index_h;

};