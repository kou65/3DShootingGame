#ifndef FBX_DATA_H_
#define FBX_DATA_H_

#include<fbxsdk.h>

#include<string>
#include<vector>
#include<map>

#include"../../../Graphics/Graphics.h"
#include"../../Model/Model.h"
#include"../../SkinCustomVertex/SkinCustomVertex.h"

#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


/**
* @brief メッシュデータ
*/
struct FbxMeshData {


	/**
	* @brief コンストラクタ
	*/
	FbxMeshData() {

		polygon_num = 0;
		vertex_num = 0;
		p_index_buffer = nullptr;
		p_vertex_buffer = nullptr;
		is_bone = false;
	}

	//! インデックスバッファ
	IDirect3DIndexBuffer9 * p_index_buffer;

	//! バーテックスバッファ
	IDirect3DVertexBuffer9 * p_vertex_buffer;

	//! マテリアル配列
	MaterialInfo material_info;

	//! ポリゴン数
	UINT polygon_num;

	//! 頂点数
	UINT vertex_num;

	//! ボーン数
	UINT bone_num;

	//! メッシュローカル座標変換リスト
	D3DXMATRIX m_local_mat;

	//! メッシュノード名
	std::string mesh_node_name;

	//! マテリアル名
	std::string material_name;

	//! モーションが存在するかどうか
	bool is_bone;

	//! 動かす元となる頂点をいれておく配列
	SkinCustomVertex* mp_vertics;

};



#endif