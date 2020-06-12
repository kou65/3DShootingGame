#ifndef FBX_H
#define FBX_H


#include"../../SkinCustomVertex/SkinCustomVertex.h"
#include"../../../Texture/TextureData2D/TextureData2D.h"
#include"../../Model/Model.h"
#include"../../../Shader/ShaderFunc/VertexBlendShader/VertexBlendEffectFile.h"
#include"../../../Shader/ShaderFunc/DepthShadowShader/DepthShadowEffectFile.h"
#include"../FbxData/FbxData.h"




/**
* @brief fbxモデルを形成するクラス
*/
class Fbx : public Model{
public:


	/**
	* @brief インスタンスを返す
	*/
	static Fbx *GetInstance() {
		static Fbx fbx;
		return &fbx;
	}

public:


	/**
	* @brief コンストラクタ
	*/
	Fbx();


	/**
	* @brief デストラクタ
	*/
	~Fbx() {

		// マネージャの破壊
		m_fbx_module.mp_manager->Destroy();
	}
	

	/**
	* @brief 読み込み 
	* @param[in] fbx_file_path fbxのファイルパス
	* @return bool
	*/
	bool Load(const std::string &fbx_file_path);


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 描画
	*/
	void Draw(TextureData*td = nullptr);


	/**
	* @brief アニメーション更新
	* @param[in] frame 回すフレーム数
	*/
	void Animate(
		const float &frame = 20.f
	);


	/**
	* @brief モーション情報をセットする
	* @param[in] name モーション名
	*/
	void SetMotion(std::string name = "default");


	/**
	* @brief モーション読み込み
	* @param[in] name モーション名
	* @param[in] ファイル名
	*/
	void LoadFileMotion(
		std::string name,
		const char* pFilename);


	/**
	* @brief 解放
	*/
	void Release();

private:


	/**
	* @brief ワールド変換行列を返す
	* @param[in] mesh_index
	* @return D3DXMATRIX 行列を返す
	*/
	D3DXMATRIX GetFbxWorldD3DMatrix(int mesh_index);


	/**
	* @brief 通常描画
	* @param[in] vertex_num 頂点数
	* @param[in] polygon_num ポリゴン数
	* @param[in] world_mat ワールド行列
	*/
	void NormalDraw(
		const int&vertex_num,
		const int&polygon_num,
		D3DXMATRIX&world_mat
	);


	/**
	* @brief エフェクト描画
	* @param[in] vertex_num 頂点数
	* @param[in] polygon_num ポリゴン数
	* @param[in] world_mat ワールド行列
	* @param[in] bone_mat_list ボーン行列
	* @param[in] max_bone_index 最大ボーンインデックス数
	*/
	void EffectDraw(
		const int&vertex_num,
		const int&polygon_num,
		const D3DXMATRIX&world_mat,
		D3DXMATRIX*bone_mat_list,
		const int&max_bone_index
	);

private:


	/**
	* @brief メッシュ読み込み
	*/
	void LoadMesh();


	/**
	* @brief インデックス読み込み
	* @param[in] p_vertex_data_list
	* @p_mesh[in] p_mesh
	*/
	void LoadIndeces(
		std::vector<FbxMeshData>&p_vertex_data_list,
		FbxMesh*p_mesh
	);

	/**
	* @brief 頂点情報初期化
	* @param[in] p_mesh メッシュ
	* @param[in] mesh_data メッシュデータ
	* @param[in] size サイズ
	*/
	void InitVertexInfo(
		FbxMesh*p_mesh,
		FbxMeshData*mesh_data,
		const UINT &size
	);


	/**
	* @brief 頂点読み込み
	* @param[out] p_mesh メッシュデータ
	* @param[in] mesh_data_list メッシュデータ配列
	*/
	void LoadVertexInfo(
		FbxMesh*p_mesh,
		std::vector<FbxMeshData>&mesh_data_list
	);


	/**
	* @brief uv読み込み
	* @param[in] p_mesh_data_list メッシュデータ配列
	* @param[in] p_mesh Fbxメッシュ 
	*/
	void LoadUvInfo(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);


	/**
	* @brief 法線読み込み
	* @param[in] p_mesh_data_list メッシュデータ配列
	* @param[in] p_mesh fbxメッシュ
	*/
	void LoadNormal(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);

private:



	/**
	* @brief マテリアル読み込み
	* @param[in] p_mesh_data_list fbxメッシュデータ
	* @param[out] p_mesh メッシュ
	*/
	void LoadMaterial(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);


	/**
	* @brief テクスチャ読み込み
	* @param[in] p_mesh fbxメッシュ
	* @param[in] p_material_info マテリアル情報
	* @return bool
	*/
	bool LoadTexture(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info
	);


	/**
	* @brief カラー読み込み
	* @param[in] fbxメッシュデータ
	* @param[in] p_mesh メッシュ
	*/
	bool LoadColor(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);


	/**
	* @brief エントリー済みのマテリアルテクスチャを読み込む
	* @param[in] p_mesh メッシュ
	* @param[out] p_material_info マテリアル情報
	*/
	void LoadEntryTexture(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info
	);

private:


	/**
	* @brief アニメーションを選択
	* @param[in] select_anim_num
	* @return bool アニメーションbool型を返す
	*/
	bool SelectAnimation(
		int select_anim_num
	);


	/**
	* @brief モーションの読み込み
	* @param[in] name モーション名
	* @param[in] select_morion_num モーション番号
	*/
	void LoadMotion(
		const std::string& name,
		const int &select_motion_num
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
		FbxTime&start,
		FbxTime&stop,
		FbxTime&frame_time,
		const int&anim_num
	);


	/**
	* @brief モデル情報読み込み
	* @param[in] p_mesh_data_list メッシュデータ
	* @param[in] p_mesh メッシュ
	*/
	void LoadModelInfo(
		std::vector<FbxMeshData>& p_mesh_data_list,
		FbxMesh* p_mesh
	);


	/**
	* @brief 現在のアニメーションを受け取る
	* @param[in] mesh メッシュ
	* @param[in] count fbx時間
	* @param[in] deformer_num デフォーム数
	* @param[in] cluster_num クラスター数
	*/
	FbxMatrix GetAnimationMatrix(
		FbxMesh* mesh,
		FbxTime&count,
		int deformer_num,
		int cluster_num
	);


	/**
	* @brief CPUのスキニング
	*/
	void CPUSkinning();


	/**
	* @brief 重みのスキニングを行う 最新バージョン
	*/
	void WeightSkinning(
		FbxMesh* mesh,
		FbxMeshData& mesh_data,
		SkinCustomVertex* vertices,
		const int &mi
	);


	/**
	* @brief キースキニング
	*/
	void KeySkinning();


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
		const int &mi
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

	// 相対パス(CP932) → 絶対パス(UTF-8)
	std::string GetUTF8Path(const std::string& path);

	// カレントパスの読み込み
	void LoadCurrentPath(const std::string &path_name);

	// Fbx関数でポリゴンを3つに分割する
	void FbxPolygon3Convert();

	// fbx行列をDirectXの行列に変換
	void FbxMatConvertD3DMat(
		D3DXMATRIX*p_out_mat,
		FbxMatrix&fbx_mat
	);

	// 位置を修正する行列取得
	FbxAMatrix GetGeometry(FbxNode* pNode);
	
	// 左手系変換
	void FbxMatLConvert(
		FbxMatrix&out_fbx_mat
	);

	// 左手系変換
	void FbxMatLConvert(
		FbxAMatrix&out_fbx_mat
	);

private:

	// fbxsdk部品
	FbxModuleModel m_fbx_module;

	// カスタムバーテックスの配列
	std::vector<FbxMeshData>m_mesh_data_list;

	// ルートパス
	std::string m_root_path;

	// 変更用頂点配列
	std::vector<SkinCustomVertex*>mp_vertics;

	// シェーダー
	VertexBlendEffectFile m_effect;
	
	/* アニメーション関連 */
	
	// モーション名
	std::string m_current_motion_name;

	// フレーム
	double m_frame;

	// 停止数
	int m_stop_count;

	// カウント
	int m_count;

	// モーション文字列
	// モーション種類 > メッシュ 
	std::map<std::string,std::vector<MotionData>> m_motion;

	// シェーダーするかどうか
	bool m_is_shader;

	// アニメーションするかどうか
	bool m_is_skinning;

	// 現在のアニメーションフレーム数
	float m_key_frame;

};

#endif