#ifndef FBX_DATA_H_
#define FBX_DATA_H_

#include<fbxsdk.h>

#include<string>
#include<vector>
#include<map>

#include"../../../Graphics/Graphics.h"
#include"../../Model/Model.h"

#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


/**
* @brief fbxで使う部品モデル構造体
*/
struct FbxModuleModel {

	//! SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * mp_manager;

	//! シーンの作成
	FbxScene * mp_fbx_scene;

	//! インポーター
	FbxImporter*mp_importer;

};

//! 最大ボーン数
const int MAX_BONE_MATRIX_NUM = 64;


/**
* @brief ボーン構造体
*/
struct Bone {

	//! ボーン行列
	D3DXMATRIX bone_list[MAX_BONE_MATRIX_NUM];
};


/**
* @brief モーションデータ
*/
struct MotionData {

	//! フレーム数
	UINT frame_num;

	//! キーフレーム
	//! フレーム数 > ボーン数
	std::vector<std::vector<FbxMatrix>>animation_matrix;

	//! d3d用(GPUスキニングにする場合はこちらにする)
	std::vector<Bone>d3d_animation_mat;
};



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

	//! ボーン配列
	std::vector<FbxMatrix>bone_list;

	//! モーション配列
	std::vector<FbxMatrix>motion_list;

	//! ボーン行列
	std::vector<D3DXMATRIX>d3d_bone_mat_list;

	//! アニメーション行列
	std::vector<D3DXMATRIX>d3d_anim_mat_list;

	//! 影響数を保持
	std::vector <std::vector<double>>weight_list;

	//! 影響インデックス
	std::vector<std::vector<int>>weight_index_list;

};



#endif