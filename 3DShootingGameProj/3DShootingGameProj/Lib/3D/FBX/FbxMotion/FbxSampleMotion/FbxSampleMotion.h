#pragma once


#include<fbxsdk.h>
#include<vector>
#include<map>
#include"../../../../Graphics/Graphics.h"
#include"../../../SkinCustomVertex/SkinCustomVertex.h"
#include"../../FbxData/FbxData.h"


#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


// 最大ボーン数
static constexpr int BONE_MAX = 256;


// ボーン情報
struct BoneData
{
	char		name[64];
	D3DXMATRIX	offset;
	D3DXMATRIX	transform;
};

//	アニメーション
struct Motion
{
	Motion() {}

	UINT					numFrame;		// フレーム数
	std::vector<D3DXMATRIX> Key[BONE_MAX];	// キーフレーム
};

/**
* @brief モーション用情報
*/
class FbxSampleMotion {
public:

	/* 外で使用するもの */


	/**
	* @brief アニメーションの再生
	*/
	void Play(std::string name);


	/**
	* @brief アニメーションの更新
	*/
	void Animate(float sec = 1.0f / 60.0f);


public:


	/**
	* @brief モデルを動かす
	*/
	void Skinning(
		const int &total_mesh,
		std::vector<FbxMeshData>&mesh_data_list,
		std::vector<SkinCustomVertex*>&skin_vertex
	);


	/**
	* @brief ボーン読み込み
	*/
	void LoadBone(
		UINT matIdx,
		FbxMesh* pMesh,
		std::vector<FbxMeshData>&mesh_data_list
	);


	/**
	* @brief モーション読み込み
	*/
	void LoadMotion(std::string name, const char* pFilename);


private:


	/**
	* @brief ボーン捜索
	*/
	int FindBone(const char* pName);


	/**
	* @brief キーフレーム読み込み
	*/
	void LoadKeyFrames(std::string name, int bone, FbxNode* pBoneNode);

private:

	/**
	* @brief 移動行列算出
	*/
	void FbxMatLConvert(
		FbxAMatrix&out_fbx_mat
	);


	/**
	* @brief 移動行列算出
	*/
	void FbxMatLConvert(
		FbxMatrix&out_fbx_mat
	);


	/**
	* @brief fbxからd3d行列に変換
	*/
	void FbxMatConvertD3DMat(
		D3DXMATRIX*p_out_mat,
		FbxMatrix &fbx_mat
	);

private:

	//! 現在のプレイモーション
	std::string m_play_motion;

	//! 現在のフレーム
	int m_frame;

	//! スタートフレーム
	int m_start_frame;

	//! 総ボーン数
	int m_total_bone;

	//! ボーンリスト
	BoneData m_bone_list[BONE_MAX];

	//! モーションリスト
	std::map<std::string, Motion>m_motion_list;
};