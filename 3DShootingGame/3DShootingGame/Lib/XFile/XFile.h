#pragma once
#include"../D3D9/D3D9.h"
#include<string>
#include<vector>



class XFile {
public:

	// アクセス権
	static XFile *GetInstance() {
		static XFile x_file;
		return &x_file;
	}

public:

	// 読み込み
	bool Load(std::string x_file_name,std::string mesh_name);

	// 描画
	void Draw(std::string mesh_name,const D3DXVECTOR3 &pos);

private:

	// マテリアル数
	DWORD m_material_num;

	// マテリアル情報
	D3DMATERIAL9*m_p_material_info_list;

	// テクスチャリスト
	LPDIRECT3DTEXTURE9 *m_p_texture_list;
	
	// メッシュ
	LPD3DXMESH m_p_mesh;
};