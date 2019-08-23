#pragma once
#include"../../D3D9/D3D9.h"
#include<string>
#include<vector>
#include<unordered_map>



class XFile {
public:

	// アクセス権
	static XFile *GetInstance() {
		static XFile x_file;
		return &x_file;
	}

public:

	// 読み込み
	bool Load(
		// Xファイル名
		const std::string &x_file_name,
		// テクスチャファイルの階層
		const std::string &texture_file_hierarchy,
		// メッシュ名
		const std::string &rename
	);

	// 描画
	void Draw(std::string mesh_name,const D3DXVECTOR3 &pos);

	// 解放処理
	void Release();

private:

	// マテリアル数
	std::unordered_map<std::string, DWORD>m_material_num_list;

	// マテリアル情報
	std::unordered_map<std::string, D3DMATERIAL9*>m_material_list;

	// メッシュ
	std::unordered_map<std::string,LPD3DXMESH>m_mesh_list;
};