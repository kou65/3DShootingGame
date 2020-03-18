#ifndef OBJ_FILE_H
#define OBJ_FILE_H

#include<iostream>
#include<vector>
#include"../Object3DCustomVertex/MeshCustomVertex.h"
#include<unordered_map>
#include"../../../Utility/Utility.h"
#include"../Model/Model.h"
#include"../../Vec3/Vec3.h"
#include"../../EffectFileShader/NormalShader/NormalShader.h"


// HACK リファクタリング対象(複数のstructなど)


// Drawサブセット用
struct ObjectSubset {
	unsigned int material_index;
	unsigned int face_start;
	unsigned int face_count;
};


// オブジェクトパラメータ
struct ObjParameter {

	ObjParameter() {
		pos.x = pos.y = pos.z = 0.f;
		rota = pos;
		scale.x = scale.y = scale.z = 1.f;
	}

	Vec3 pos;
	Vec3 scale;
	Vec3 rota;
	std::string register_obj_file_name;
	std::string texture_name;
};


// オブジェファイルで保存するデータ
struct ObjFileData {

	// マテリアル数
	unsigned int material_num;
	// Objの方のマテリアル名配列
	std::vector<std::string>m_usemtl_name_list;
	// 描画を入れ替えるサブセット
	std::vector<ObjectSubset>m_object_sub_set_list;
	// マテリアルデータ配列(マテリアル名,マテリアルデータ)
	std::unordered_map<std::string, MaterialInfo>m_material_data_list;
	// バーテックスバッファ
	IDirect3DVertexBuffer9 * m_p_vertex_buffer;
	// インデックスバッファ
	IDirect3DIndexBuffer9 * m_p_index_buffer;
};



class Obj : public  Model{
public:

	static Obj *GetInstance(){
		static Obj obj_file;
		return &obj_file;
	}

public:

	// 解放処理
	void Release() {};

	// 読み込み
	bool Load(
		const std::string &obj_file_path,
		const std::string &registr_name,
		const std::string &mtl_file_path = ""
	);

	// GPU
	void ShaderDraw(
		const ObjParameter &param
	);

	// 固定機能
	void NormalDraw(
		const ObjParameter&param
	);

private:


	Obj();


	// メッシュ情報読み込み
	bool MeshLoad(
		const std::string &file_path,
		const std::string &registr_name,
		std::string&out_material_name,
		int &out_total_material_num,
		std::vector<INT>&indices,
		std::vector<MeshCustomVertex>&mesh_list
	);



	// 面情報からカスタムバーテックス作成
	void FaceInfoLoad(
		FILE*p_file,
		const std::string &register_name,
		std::vector<INT>&indices,
		std::vector<MeshCustomVertex>&mesh_list,
		const std::vector<D3DXVECTOR3>&vertex_list,
		const std::vector<D3DXVECTOR2>&uv_list,
		const std::vector<D3DXVECTOR3>&normal_list
		);


	// 頂点情報読み込み
	void VertexInfoLoad(
		FILE *p_file,
		const char*front_str,
		std::vector<D3DXVECTOR3>&out_vertex_list,
		std::vector<D3DXVECTOR2>&out_uv_list,
		std::vector<D3DXVECTOR3>&out_normal_list
	);


	// マテリアルファイル読み込み
	bool MaterialFileLoad(
		const std::string &mtl_file_name,
		const std::string &obj_file_name,
		const std::string &texture_file_path
	);


	// usemtl読み込み
	void UseMaterialInfoLoad(
		FILE*p_file,
		const std::string &obj_file_name,
		int &out_total_material_num,
		std::vector<INT>&indices,
		char*front_str,
		int load_buffer
	);


	// 面情報代入
	void InsertFaceList(
		const std::vector<std::vector<std::string>>&face_info_str,
		std::vector<MeshCustomVertex>&custom_vertex,
		std::vector<INT>&m_indices,
		const std::vector<D3DXVECTOR3>&vertex_list,
		const std::vector<D3DXVECTOR2>&uv_list,
		const std::vector<D3DXVECTOR3>&normal_list,
		std::vector<ObjectSubset>&out_draw_sub_set
	);


	// 整数値変換
	bool Vector3ConversionByString(
		int &axis,
		const std::string &string
	);


	// 頂点バッファ生成
	void VertexBufferCreate(
		const int &vertex_num,
		const std::string &obj_file_name,
		const std::vector<MeshCustomVertex>&mesh_vertex_list
	);


	// 面情報から頂点情報埋め込み
	bool IndexBufferCreateFaceBase(
		const int &face_num,
		const std::string &obj_file_name,
		const std::vector<INT>&indices
		);


	// 4面を3面にする
	std::vector<INT> Vertex4CutToVertex3Face(
		const std::vector<INT>&vertex4_polygon_list
	);


	// 頂点ファイル読み込み
	void VertexFscanfLoad(
		FILE*p_file,
		D3DXVECTOR3 &vec3);


	// 変換行列を返す
	D3DXMATRIX GetTransformMatrix(const ObjParameter&param);

private:

	// オブジェファイルリスト
	std::unordered_map<std::string, ObjFileData*>m_obj_file_data;

	NormalShader m_ns;
};

#endif