#pragma once

#include"../FbxMeshData/FbxMeshData.h"
#include"../../../Graphics/Graphics.h"



//! 最大ボーン数
const int MAX_BONE_MATRIX_NUM = 64;

/**
* @brief ボーンとモーションが合成された行列構造体
*/
struct BoneMotion {

	//! ボーン行列
	std::vector<D3DXMATRIX>anim_mat_list;
};


/**
* @brief ボーン構造体
*/
struct Bone {

	// ボーン行列
	D3DXMATRIX mat;

	// ボーン名
	std::string skeleton_name;
};


/**
* @brief ボーン情報構造体
*/
struct BoneData {

	//! スキン数
	int skin_num;
	//! ボーン数
	int bone_num;
	//! ボーン行列
	std::vector<Bone>bone_list;
	//! fbxボーン行列
	std::vector<FbxMatrix>fbx_mat_list;
};


/**
* @brief モーションデータ
*/
struct MotionData {

	MotionData() {
		fbx_stack_motion_num = 0;
	}

	//! フレーム数
	//UINT frame_num;

	// 以下ボーン

	//! フレーム数 > ボーン数
	std::vector<std::vector<FbxMatrix>>animation_matrix;

	//! d3d用(GPUスキニングにする場合はこちらにする)(カウントアニメ行列)
	std::vector<BoneMotion>d3d_animation_mat;

	//! fbxモーション数
	int fbx_stack_motion_num;

};


/**
* @brief アニメーションデータ
*/
struct AnimationData {

	//! モーションデータの行列 フレーム > ボーン
	std::vector<std::vector<FbxMatrix>>mat;

};


/**
* @brief 属性識別
*/
enum class ElementType {
	MESH,      // メッシュだけ
	BONE_MESH, // ボーンメッシュ
};


/**
* @brief 属性データ
*/
struct ElementData {

	ElementType type;
	std::string mesh_node_name; // 大体メッシュ名を代入する
};
