#include"XFile.h"
#include"../../Texture/TextureManager/TextureManager.h"
#include"../Light/Light.h"




bool XFile::Load(
	const std::string &x_file_name,
	const std::string &file_hierarchy,
	const std::string &mesh_name) {

	// マテリアル情報
	LPD3DXBUFFER material_info;

	// マテリアル数
	DWORD material_num;

	// メッシュ
	LPD3DXMESH mesh;

	// メッシュ読み込み
	if (E_FAIL == D3DXLoadMeshFromX(
		// ファイル名
		x_file_name.c_str(),
		// メッシュ作成用のオプションを指定
		NULL,
		// デバイス
		Graphics::GetInstance()->GetLpDirect3DDevice9(),
		// 基本未使用
		NULL,
		// マテリアル情報を指すポインタのアドレス
		&material_info,
		// 基本未使用
		NULL,
		// XFileに使用されているマテリアルの数が返ってくる
		&material_num,
		// メッシュ情報を指すポインタのアドレス
		&mesh
	)) {
		// ファイル生成失敗
		return false;
	}

	/* 
	この時点でメッシュの読み込みは完了
	次に読み込んだメッシュとマテリアル
	情報をメンバ変数にコピーする
	 */

	// マテリアルリスト追加
	m_material_num_list[mesh_name] = material_num;

	// サイズ生成
	m_material_list[mesh_name] = new D3DMATERIAL9[material_num];

	// メッシュ追加
	m_mesh_list[mesh_name] = mesh;

	// サイズ生成
	//m_p_material_info_list = new D3DMATERIAL9[material_num];

	// メッシュに使用されているテクスチャ用の配列を用意する
	//m_p_texture_list = new LPDIRECT3DTEXTURE9[material_num];

	D3DXMATERIAL *p_material_list = 
		(D3DXMATERIAL*)material_info->GetBufferPointer();

	// マテリアルはマテリアル情報とマテリアルの数に代入されている
	for (DWORD i = 0; i < material_num; i++) {

		// マテリアル取得
		m_material_list[mesh_name.c_str()][i] = p_material_list[i].MatD3D;
		

		// マテリアルで設定されているテクスチャ読み込み
		// テクスチャ情報があるなら
		if (p_material_list[i].pTextureFilename != NULL) {

			// ファイル名を受け取り
			std::string file_name = 
				p_material_list[i].pTextureFilename;

			// テクスチャの管理者受け取り
			TextureManager &texture_manager
				= *TextureManager::GetInstance();

			// テクスチャが登録されていないなら
			if (texture_manager.FindMultipleTexture(
				mesh_name.c_str()) == false
				) {

				// 文字列結合
				file_name = file_hierarchy + file_name;

				// テクスチャの読み込み
				texture_manager.LoadMultiple(
					file_name.c_str(),
					mesh_name.c_str(),
					i
				);

			}
		}

		// コピーが終わったのでバッファを解放
		material_info->Release();

	}

	return true;

}


void XFile::Draw(std::string mesh_name,const D3DXVECTOR3 &pos) {

	LPDIRECT3DDEVICE9 device = Graphics::GetInstance()->GetLpDirect3DDevice9();

	D3DXMATRIX matrix_total;
	D3DXMATRIX matrix_trans;

	// 行列移動
	D3DXMatrixTranslation(
		&matrix_trans,
		pos.x,
		pos.y,
		pos.z
	);

	// 行列合成
	matrix_total = matrix_trans;

	// 一つ一つのメッシュを描画
	for (DWORD i = 0; i < m_material_num_list[mesh_name]; i++) {

		// マテリアルの反映
		device->SetMaterial(&m_material_list[mesh_name][i]);
		
		// マテリアルで使用しているテクスチャをセット
		device->SetTexture(
			0,
			TextureManager::GetInstance()->GetTextureDataMultiple(mesh_name).texture_list[i]
		);

		// ワールド座標を軸に変換
		device->SetTransform(
			D3DTS_WORLD,
			&matrix_total
		);

		// サブセットとは分割された一つ一つのメッシュ
		m_mesh_list[mesh_name]->DrawSubset(i);
	}
}


void XFile::Release() {
	
	// メッシュデータ解放
	for (auto mesh : m_mesh_list) {
		// 解放
		mesh.second->Release();
		// メモリ削除
		delete[] &mesh;
	}

	// マテリアル解放
	for (auto material : m_material_list) {
		
		delete[] material.second;
	}
}