#include"HierarchyXFile.h"
#include<string>
#include<stdio.h>
#include"../Texture/TextureManager.h"




void HierarchyXFile::LoadFrame(const std::string file_name) {


	D3DXLoadMeshHierarchyFromXA(
		file_name.c_str(),
		m_mesh_container.NumMaterials,
		D3D9::GetLpDirect3DDevice9(),
		m_allocate_hierarchy
		);
}



HRESULT HierarchyXFile::CreateFrame(
	LPSTR name,
	LPD3DXFRAME *p_new_frame) {

	// 文字列コピー
	m_frame_data->Name = new char[lstrlenA(name) + 1];
	strcpy_s(m_frame_data->Name, lstrlenA(name) + 1, name);
}

HRESULT HierarchyXFile::CreateMeshContainer
(THIS_ LPCSTR name,
	const D3DXMESHDATA * p_mesh_data,
	const D3DXMATERIAL * p_materials,
	const D3DXEFFECTINSTANCE * p_effect_instances,
	DWORD material_num,
	const DWORD * p_adjacency,
	LPD3DXSKININFO p_skin_info,
	LPD3DXMESHCONTAINER *pp_new_mesh_container
) {

	// 文字列コピー
	{
		m_mesh_container.Name = new char[lstrlenA(name) + 1];
		strcpy_s(m_mesh_container.Name, lstrlenA(name) + 1, name);
	}

	// メッシュのコピー
	{
		m_mesh_container.MeshData.pMesh = p_mesh_data->pMesh;
		m_mesh_container.MeshData.Type = D3DXMESHTYPE_MESH;
	}

	// 参照カウンタを加算する
	p_mesh_data->pMesh->AddRef();

	// 隣接ポリゴン番号のコピー(一つのポリゴンに隣接しているポリゴン番号のこと)
	{
		m_mesh_container.pAdjacency =
			new DWORD[p_mesh_data->pMesh->GetNumFaces() * 3];

		// コピー
		memcpy(
			m_mesh_container.pAdjacency,
			p_adjacency,
			sizeof(DWORD) * (p_mesh_data->pMesh->GetNumFaces() * 3)
		);
	}

	// マテリアルのコピー
	m_mesh_container.NumMaterials = material_num;

	// マテリアルが無い場合
	if (material_num > 0) {

		m_mesh_container.pMaterials = 
			new D3DXMATERIAL[m_mesh_container.NumMaterials];

		m_mesh_container.p_texture_list =
			new LPDIRECT3DTEXTURE9[m_mesh_container.NumMaterials];

		memcpy(
			m_mesh_container.pMaterials,
			p_mesh_data,
			sizeof(D3DXMATERIAL) * material_num);

		// ファイル読み込み
		for (int i = 0; i < material_num; i++) {

			if(m_mesh_container.pMaterials[i].pTextureFilename != NULL &&
				TextureManager::GetInstance()->GetTextureData3D(
					m_mesh_container.pMaterials[i].pTextureFilename != NULL)
				)
		}
	}
}
