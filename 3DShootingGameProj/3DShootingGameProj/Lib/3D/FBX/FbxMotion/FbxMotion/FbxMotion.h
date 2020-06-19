#pragma once
#include<fbxsdk.h>
#include<vector>
#include<map>
#include"../../../../Graphics/Graphics.h"
#include"../../../SkinCustomVertex/SkinCustomVertex.h"
#include"../../FbxData/FbxData.h"


#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")




/**
* @brief fbxモーション関係クラス
*/
class FbxMotion {
public:


	/**
	* @brief コンストラクタ
	*/
	FbxMotion(
		FbxModuleModel*fbx_module
	);


	/**
	* @brief モーション情報をセットする
	* @param[in] name モーション名
	*/
	void SetMotion(std::string name = "default");


	/**
	* @brief アニメーション更新
	* @param[in] frame 回すフレーム数
	*/
	void Animate(
		const float &frame = 20.f
	);


	/**
	* @brief CPUのスキニング
	*/
	void CPUSkinning(
		std::vector<FbxMeshData>& mesh_data_list,
		const std::vector<SkinCustomVertex*>&mp_vertics
	);


	/**
	* @brief モーション読み込み
	* @param[in] name モーション名
	* @param[in] ファイル名
	*/
	void LoadFileMotion(
		std::string name,
		const char* pFilename);


	/**
	* @brief モーションの読み込み
	* @param[in] name モーション名
	* @param[in] select_morion_num モーション番号
	*/
	void LoadMotion(
		const std::string& name,
		const int &select_motion_num,
		std::vector<FbxMeshData>&m_mesh_data_list,
		FbxModuleModel&mod
	);


	/**
	* @brief モデル情報読み込み
	* @param[in] p_mesh_data_list メッシュデータ
	* @param[in] p_mesh メッシュ
	*/
	void LoadBone(
		const FbxModuleModel&mod,
		std::vector<FbxMeshData>*p_mesh_data_list,
		FbxMesh* p_mesh
	);



	/**
	* @brief 重み頂点読み込み
	* @param[in] p_mesh_data_list メッシュデータ配列
	* @param[in] p_mesh メッシュ
	*/
	void LoadWeightVertexPoint(
		std::vector<FbxMeshData>& p_mesh_data_list,
		FbxMesh* p_mesh
	);



private:


	/**
	* @brief アニメーションを選択
	* @param[in] select_anim_num
	* @return bool アニメーションbool型を返す
	*/
	bool SelectAnimation(
		int select_anim_num,
		FbxModuleModel&mod
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
	void LoadKeyFrame(
		FbxMesh* mesh,
		const std::string& motion_name,
		std::vector<FbxMatrix>& bone_mat_list,
		const int& bone_num,
		const FbxTime& start,
		const FbxTime& stop,
		const FbxTime& frame_count
	);


	/**
	* @brief アニメーション関連をセットする
	* @param[in] start 最初
	* @param[in] stop 最後
	* @param[in] frame_time フレーム時間
	* @param[in] anim_num アニメーション数
	*/
	void SetAnimation(
		FbxModuleModel&mod,
		FbxTime&start,
		FbxTime&stop,
		FbxTime&frame_time,
		const int&anim_num
	);



	/**
	* @brief 現在のアニメーションを受け取る
	* @param[in] mesh メッシュ
	* @param[in] count fbx時間
	* @param[in] deformer_num デフォーム数
	* @param[in] cluster_num クラスター数
	*/
	FbxMatrix GetAnimationMatrix(
		const FbxModuleModel&mod,
		FbxMesh* mesh,
		FbxTime& count,
		int deformer_num,
		int cluster_num
	);


	/**
	* @brief 重みのスキニングを行う 最新バージョン
	*/
	void WeightSkinning(
		FbxMesh* mesh,
		FbxMeshData& mesh_data,
		SkinCustomVertex* vertices,
		const std::vector<SkinCustomVertex*>&p_vertics,
		const int &mi
	);


private:


	/**
	* @brief fbxからd3dに変換
	*/
	void FbxMotion::FbxPolygon3Convert(
		const FbxModuleModel& mod
	);


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


private:

	/* 現在使わない */

	/**
	* @brief キースキニング
	*/
	void KeySkinning(
		std::vector<FbxMeshData>m_mesh_data_list,
		const std::vector<SkinCustomVertex*>&p_vertics
	);


	/**
	* @brief キーフレームアニメーション
	* @param[in] mesh メッシュ
	* @param[in] mesh_data メッシュデータ
	* @param[in] vertices カスタムバーテックスポインタ
	* @param[in] mi メッシュインデックス
	*/
	void KeyFrameSkinning(
		FbxMesh* mesh,
		FbxMeshData& mesh_data,
		SkinCustomVertex* vertices,
		const std::vector<SkinCustomVertex*>&p_vertics,
		const int &mi
	);


private:


	//! 現在のアニメーションフレーム数
	float m_key_frame;

	//! モーション名
	std::string m_current_motion_name;

	//! フレーム
	double m_frame;

	//! 停止数
	int m_stop_count;

	//! カウント
	int m_count;

	// モーション文字列
	//! モーション種類 > メッシュ
	std::map<std::string, std::vector<MotionData>> m_motion;

	//! fbxアクセスパーツが入っている部品
	FbxModuleModel *mp_module;
};
