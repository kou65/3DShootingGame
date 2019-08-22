#pragma once
#include"../D3D9/D3D9.h"

// フレーム
struct FrameData : public D3DXFRAME {

	// フレーム描画用の行列
	D3DXMATRIX m_frame_draw_matrix;
};


// メッシュコンテナ
struct MeshContainer : public D3DXMESHCONTAINER {

	LPDIRECT3DTEXTURE9 *p_texture_list;
};


class HierarchyXFile {
public:

	// アクセス権
	static HierarchyXFile GetInstance() {
		static HierarchyXFile hierarchy_x_file;
		return hierarchy_x_file;
	}

public:

	// 読み込み
	void LoadFrame(const std::string file_name);

private:

	// フレームを生成(自動で呼ばれるようにする)
	STDMETHOD(CreateFrame)(THIS_ LPSTR,LPD3DXFRAME *p_new_frame);

	// メッシュコンテナ生成
	STDMETHOD(CreateMeshContainer)
		(THIS_ LPCSTR name,
			const D3DXMESHDATA * p_mesh_data,
			const D3DXMATERIAL * p_materials,
			const D3DXEFFECTINSTANCE * p_effect_instances,
			DWORD material_num,
			const DWORD * p_adjacency,
			LPD3DXSKININFO p_skin_info,
			LPD3DXMESHCONTAINER *pp_new_mesh_container
			);

private:

	// フレームデータ
	FrameData *m_frame_data;

	// メッシュコンテナ
	MeshContainer m_mesh_container;

	// インターフェース
	ID3DXAllocateHierarchy *m_allocate_hierarchy;
};
