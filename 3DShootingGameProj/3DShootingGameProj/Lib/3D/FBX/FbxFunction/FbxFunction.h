#pragma once

#include"../FbxMotionData/FbxMotionData.h"




// LP型作成
typedef class FbxScene *LP_FBX_SCENE;
typedef class FbxManager*LP_FBX_MANAGER;
typedef class FbxImporter*LP_FBX_IMPORTER;


/**
* @brief fbx関数群
*/
namespace FbxFunction {


	/**
	* @brief fbxを扱うにつれての部品生成初期化読み込み諸々
	*/
	bool CreateLoadFbxModule(
		LP_FBX_MANAGER&p_manager,
		LP_FBX_SCENE&p_scene,
		LP_FBX_IMPORTER&p_importer,
		const std::string&file_name,
		bool is_non_blocking = false
	);


	/**
	* @brief fbx部品削除
	*/
	bool Destory(
		LP_FBX_SCENE&p_scene,
		LP_FBX_IMPORTER&p_importer,
		LP_FBX_MANAGER&p_manager
	);

	/**
	* @brief fbx管理者作成
	*/
	bool CreateFbxManager(LP_FBX_MANAGER &out_p_manager);


	/**
	* @brief fbxシーンの作成
	*/
	bool CreateFbxScene(
		LP_FBX_MANAGER &p_manager,
		LP_FBX_SCENE &p_scene
	);


	/**
	* @brief インポーターの作成,ついでにファイル名を入れて初期化も行う
	*/
	bool CreateFbxImporter(
		LP_FBX_IMPORTER&p_out_impoter,
		LP_FBX_MANAGER&p_manager,
		const std::string&file_name
	);


	/**
	* @brief インポーターからシーンを読み込む
	* @param[in] is_non_blocking 読み込みモード,同期 = true,非同期 = false
	*/
	bool LoadFbxSceneToImporter(
		LP_FBX_IMPORTER &p_importer,
		LP_FBX_SCENE&p_scene,
		bool is_non_blocking = false
	);


	/**
	* @brief 行列補間
	*/
	void MatrixInterporate(
		D3DXMATRIX& out,
		D3DXMATRIX& A,
		D3DXMATRIX B,
		float rate
	);


	/**
	* @brief 行列補間
	*/
	void MatrixInterporate2(
		D3DXMATRIX& out,
		D3DXMATRIX& A,
		D3DXMATRIX B,
		float rate
	);


	void MatrixInterporate3(
		D3DXMATRIX& out,
		D3DXMATRIX& A,
		D3DXMATRIX B, 
		float rate
	);


	namespace Node {

		/**
		* @brief 再帰関数
		*/
		void RecursiveNode
		(FbxNode*p_node, const FbxScene&p_scene);


		/**
		* @brief ノード探索
		*/
		void SerchNode(
			const FbxScene&fbx_scene
		);
	}


	namespace Skeleton {


		/**
		* @brief モーション名が合っているならそのスケルトンを返す
		*/
		bool MatchSkeletonMotion(
			FbxScene*p_scene,
			const std::string&skel_name,
			FbxSkeleton**p_out_skel,
			const int&skeleton_num_max,
			int&current_skeleton_num
		);


		/**
		* @brief スケルトン名とメッシュスケルトン名が合っているかどうか
		*/
		bool MatchSkelNameAndMeshSkelName(
			const std::string&mesh_name,
			const std::string&mesh_skeleton_name,
			FbxSkeleton*s
		);


		/**
		* @brief スケルトンの姿勢行列取り出し
		*/
		FbxAMatrix GetSkeletonAnimationMatrix(
			FbxSkeleton*p_node,
			FbxScene*p_scene,
			const FbxTime&time
		);

	}

}