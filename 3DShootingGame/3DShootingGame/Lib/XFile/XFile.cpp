#include"XFile.h"
#include"../Texture/TextureManager.h"
#include"../Texture/Texture.h"
#include"../Light/Light.h"




bool XFile::Load(std::string x_file_name,std::string mesh_name) {

	// マテリアル情報
	LPD3DXBUFFER material_info;

	// メッシュ読み込み
	if (E_FAIL == D3DXLoadMeshFromX(
		// ファイル名
		x_file_name.c_str(),
		// メッシュ作成用のオプションを指定
		NULL,
		// デバイス
		D3D9::GetLpDirect3DDevice9(),
		// 基本未使用
		NULL,
		// マテリアル情報を指すポインタのアドレス
		&material_info,
		// 基本未使用
		NULL,
		// XFileに使用されているマテリアルの数が返ってくる
		&m_material_num,
		// メッシュ情報を指すポインタのアドレス
		&m_p_mesh
	)) {
		// ファイル生成失敗
		return false;
	}

	/* 
	この時点でメッシュの読み込みは完了
	次に読み込んだメッシュとマテリアル
	情報をメンバ変数にコピーする
	 */

	// サイズ生成
	m_p_material_info_list = new D3DMATERIAL9[m_material_num];

	// メッシュに使用されているテクスチャ用の配列を用意する
	m_p_texture_list = new LPDIRECT3DTEXTURE9[m_material_num];

	D3DXMATERIAL *p_material_list = 
		(D3DXMATERIAL*)material_info->GetBufferPointer();

	// マテリアルはマテリアル情報とマテリアルの数に代入されている
	for (DWORD i = 0; i < m_material_num; i++) {

		// マテリアル取得
		m_p_material_info_list[i] = p_material_list[i].MatD3D;
		m_p_texture_list[i] = NULL;

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
			if (texture_manager.Find3DTexture(
				mesh_name.c_str()) == false
				) {

				// テクスチャの読み込み
				texture_manager.Load3D(
					file_name.c_str(),
					mesh_name.c_str()
				);

				m_p_texture_list[i] = 
				texture_manager.GetInstance()->
					GetTextureData3D(mesh_name);

			}
		}

		// コピーが終わったのでバッファを解放
		material_info->Release();
	}
}


void XFile::Draw(std::string mesh_name,const D3DXVECTOR3 &pos) {

	LPDIRECT3DDEVICE9 device = D3D9::GetLpDirect3DDevice9();

	Light light;

	light.NormalLightOn();

	D3DXMATRIX matrix_total;
	D3DXMATRIX matrix_trans;

	// 行列移動
	D3DXMatrixTranslation(
		&matrix_trans,
		pos.x,
		pos.y,
		pos.z
	);

	matrix_total = matrix_trans;

	for (DWORD i = 0; i < m_material_num; i++) {

		// マテリアルの反映
		device->SetMaterial(&m_p_material_info_list[i]);
		
		// マテリアルで使用しているテクスチャをセット
		device->SetTexture(
			0,
			m_p_texture_list[i]
		);

		// ワールド座標を軸に変換
		device->SetTransform(
			D3DTS_WORLD,
			&matrix_total
		);

		// サブセットとは分割された一つ一つのメッシュ
		m_p_mesh->DrawSubset(i);

	}
}