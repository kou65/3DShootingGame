#pragma once

#include"../FbxMotionData/FbxMotionData.h"
#include"../../SkinCustomVertex/SkinCustomVertex.h"
#include"../../../Shader/ShaderFunc/VertexBlendShader/VertexBlendEffectFile.h"


typedef unsigned int uint;


/**
* @brief fbxモーション関係クラス
*/
class FbxMotion {
public:


	/**
	* @brief コンストラクタ
	*/
	FbxMotion();


	/**
	* @brief エフェクト描画
	* @param[in] vertex_num 頂点数
	* @param[in] polygon_num ポリゴン数
	* @param[in] world_mat ワールド行列
	* @param[in] bone_mat_list ボーン行列
	* @param[in] max_bone_index 最大ボーンインデックス数
	*/
	void ShaderMotionDraw(
		const std::string&mesh_name,
		const int&vertex_num,
		const int&polygon_num,
		const int&mesh_idx,
		const D3DXMATRIX&world_mat
	);


	/**
	* @brief テクスチャセット
	*/
	void SetTexture(
		const std::string&texture_name
	);


	/**
	* @brief モーション情報をセットする
	* @param[in] name モーション名
	*/
	void SetMotion(
		const std::string &bone_name,
		const std::string& name
	);


	/**
	* @brief モーション入れ替え
	*/
	void ChangeMotion(
		const std::string &bone_name,
		const std::string& name
	);


	/**
	* @brief トランジションの初期化
	*/
	void InitTransition(
		const std::string&mesh_name
	);


	/**
	* @brief アニメーション更新
	* @param[in] frame 回すフレーム数
	*/
	void Animate(
		const std::string&mesh_name,
		const float &frame = 20.f
	);


	/**
	* @brief CPUのスキニング
	*/
	void CPUSkinning(
		const std::string &mesh_name,
		std::vector<FbxMeshData>& mesh_data_list
	);


	/**
	* @brief モーション数を返す
	*/
	int GetMotionNum(
		FbxImporter*importer
	);


	/**
	* @brief モーションの読み込み
	* @param[in] name モーション名
	* @param[in] select_morion_num モーション番号
	*/
	bool LoadMotion(
		FbxScene*p_scene,
		FbxImporter*p_importer,
		const std::string&bone_name,
		const std::string& motion_name,
		const int &select_motion_num
	);


	/**
	* @brief スケルトンに対応したモーションを読み込む
	*/
	bool FbxMotion::LoadSkeletonMotion(
		FbxScene*p_scene,
		FbxImporter*p_importer,
		const std::string&bone_name,
		const std::string& motion_name,
		const int &select_motion_num
	);


	/**
	* @brief 重み頂点読み込み
	* @param[in] p_mesh_data_list メッシュデータ配列
	* @param[in] p_mesh メッシュ
	*/
	void LoadWeight(
		std::vector<FbxMeshData>& p_mesh_data_list,
		FbxMesh* p_mesh
	);


	/**
	* @brief モデル情報読み込み
	* @param[in] p_mesh_data_list メッシュデータ
	* @param[in] p_mesh メッシュ
	*/
	bool LoadBone(
		FbxScene*p_scene,
		FbxImporter*p_importer,
		const std::string&bone_name,
		FbxMesh* p_mesh,
		const int&mesh_num,
		std::vector<std::string>&out_skeleton_name_list
	);


	/**
	* @brief モーション変化行列補間
	*/
	void TransitionMotion(
		const std::string &mesh_name,
		FbxMeshData& mesh_data,
		SkinCustomVertex* vertices,
		SkinCustomVertex*mp_vertics,
		const int &cur_bone_index,
		const std::string&next_motion_name,
		const int&start_time
	);

private:


	/**
	* @brief トランジション更新
	*/
	void UpdateTransition(
		const std::string&mesh_name,
		const int&frame = 20.f
	);


	/**
	* @brief アニメーションを選択
	* @param[in] select_anim_num
	* @return bool アニメーションbool型を返す
	*/
	bool SelectAnimation(
		FbxScene*p_scene,
		FbxImporter*importer,
		int select_anim_num
	);


	/**
	* @brief アニメーション関連をセットする
	* @param[in] start 最初
	* @param[in] stop 最後
	* @param[in] frame_time フレーム時間
	* @param[in] anim_num アニメーション数
	*/
	bool SetAnimation(
		FbxScene*p_scene,
		FbxTime&start,
		FbxTime&stop,
		FbxTime&frame_time,
		const int&anim_num
	);


	/**
	* @brief アニメーション合計時間の取得
	*/
	void GetAnimInfo(
		FbxImporter*importer,
		int index
	);


	/**
	* @brief アニメーション行列読み込み
	* @param[out] mesh メッシュポインタ
	* @param[in] motion_name モーション名
	* @param[in] bone_mat_list ボーン行列配列
	* @param[in] bone_num ボーン数
	* @param[in] start 最初の時間
	* @param[in] stop 最後の時間
	* @param[in] frame_count フレーム数
	*/
	void LoadAnimation(
		FbxScene*p_scene,
		const std::string& motion_name,
		std::vector<FbxMatrix>& bone_mat_list,
		const int& mesh_num,
		const int &skin_num,
		const int& bone_num,
		const FbxTime&start,
		const FbxTime&stop,
		const FbxTime&frame_count
	);


	/**
	* @brief アニメーション読み込み
	*/
	AnimationData LoadAnimation(
		FbxScene*p_scene,
		const int&mesh_num,
		const int &skin_num,
		const int& bone_num,
		const FbxTime&start,
		const FbxTime&stop,
		const FbxTime&frame_count
	);


	/**
	* @brief メッシュのアニメーションデータ
	*/
	bool FbxMotion::LoadMeshAnimationData(
		FbxScene*p_scene,
		const int&mesh_num,
		const std::string&mesh_node_name,
		const FbxTime&start,
		const FbxTime&stop,
		const FbxTime&frame_count,
		AnimationData&out_anim_data
	);


	/**
	* @brief モーション変換データ
	*/
	MotionData GetMotionTransformData(
		AnimationData&anim_data
	);


	/**
	* @brief モーションとボーンを合成
	*/
	MotionData CompositeMotionBoneMatrix(
		const std::string&motion_name,
		AnimationData&data,
		std::vector<FbxMatrix>&bone_mat_list
	);

	/**
	* @brief モーション
	*/
	MotionData CompositeMotionBoneMatrix(
		const std::string&motion_name,
		AnimationData&data,
		std::vector<D3DXMATRIX>&bone_mat_list
	);


	/**
	* @brief 現在のアニメーションを受け取る
	* @param[in] mesh メッシュ
	* @param[in] count 受け取るfbx時間
	* @param[in] deformer_num デフォーム数
	* @param[in] cluster_num クラスター数
	*/
	FbxMatrix GetAnimationMatrix(
		const FbxScene&fbx_scene,
		const FbxTime& count,
		const int&mesh_num,
		const int &skin_num,
		const int &bone_num
	);


	/**
	* @brief 重みのスキニングを行う 最新バージョン
	*/
	void WeightSkinning(
		const std::string &mesh_name,
		FbxMeshData& mesh_data,
		SkinCustomVertex* vertices,
		SkinCustomVertex*p_vertics,
		const int &mi
	);


private:


	/**
	* @brief fbx行列をDirectXの行列に変換
	*/
	void FbxMatConvertD3DMat(
		D3DXMATRIX*p_out_mat,
		FbxMatrix&fbx_mat
	);


	/**
	* @brief 位置を修正する行列取得
	*/
	FbxAMatrix GetGeometry(FbxNode* pNode);


	/**
	* @brief　移動行列算出
	*/
	void FbxMatLConvert(
		FbxMatrix&out_fbx_mat
	);


	/**
	* @brief 移動行列算出
	*/
	void FbxMatLConvert(
		FbxAMatrix&out_fbx_mat
	);


	/**
	* @brief スケルトンから姿勢行列を取得
	*/
	AnimationData GetSkeletonAnimMat(
		FbxScene*p_scene,
		const std::string &mesh_name,
		const int &mi,
		const FbxTime&start,
		const FbxTime&stop,
		const FbxTime&frame_count
	);


private:

	/* 現在使わない */

	///**
	//* @brief キースキニング
	//*/
	//void KeySkinning(
	//	std::vector<FbxMeshData>m_mesh_data_list,
	//	const std::vector<SkinCustomVertex*>&p_vertics,
	//	FbxScene*p_scene
	//);
	//
	//
	///**
	//* @brief キーフレームアニメーション
	//* @param[in] mesh メッシュ
	//* @param[in] mesh_data メッシュデータ
	//* @param[in] vertices カスタムバーテックスポインタ
	//* @param[in] mi メッシュインデックス
	//*/
	//void KeyFrameSkinning(
	//	FbxMesh* mesh,
	//	FbxMeshData& mesh_data,
	//	SkinCustomVertex* vertices,
	//	const std::vector<SkinCustomVertex*>&p_vertics,
	//	const int &mi
	//);


private:

		// 初期化アニメーションカウント数
		static constexpr int INIT_ANIMATION_COUNT = 1;

		// トランジション平均フレーム数
		static constexpr float TRANSITION_FRAME = 0.2f;

private:


	//! 現在のアニメーションフレーム数
	float m_key_frame;

	//! 行列補間用のフレーム
	float m_transition_frame;

	//! フレーム
	double m_frame;

	//! 停止数
	int m_stop_count;

	//! カウント
	int m_anim_count;

	//! 初期カウント
	int m_init_count;

	//! アニメーション変更中かどうか
	bool m_is_transition;

	//! 次のモーション
	std::string m_next_motion_name;

	//! シェーダー
	VertexBlendEffectFile m_vb_effect;

	//! 現在のアニメーションを変更する
	//! ボーン名 > モーション名
	std::map<std::string, std::string>m_current_motion_name_list;

	//! 最終的なモーション情報を入れる配列
	//! モーション種類 > メッシュ
	std::map<std::string, std::vector<MotionData>>m_motion;

	//! ボーン行列(メッシュごとに分ける)
	std::map<std::string,std::vector<BoneData>>m_bone_mat_list;

	//! ボーンかメッシュか識別用属性配列
	std::map<std::string,std::vector<ElementData>>m_elem_type_list;
};
