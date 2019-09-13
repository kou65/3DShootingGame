#pragma once
#include<stdio.h>
#include<iostream>
#include"../../Graphics/Graphics.h"
#include<vector>
#include"../Object3DCustomVertex/Object3DCustomVertex.h"
#include<unordered_map>


// HACK リファクタリング対象(複数のstructなど)



// Drawサブセット用
struct ObjectSubset {
	unsigned int material_index;
	unsigned int face_start;
	unsigned int draw_face_count;
};


// 面ポリゴン情報
struct FacePolygon {

	int pos_num;
	int uv_num;
	int normal_num;
};


// マテリアルのデータ
struct MaterialData {

	// マテリアル名
	std::string material_name;
	// テクスチャ名
	std::string texture_name;

	// カラー
	//D3DXVECTOR3 ambient;
	//D3DXVECTOR3 diffuse;
	//D3DXVECTOR3 specular;

	MaterialData() {
		//ambient.x = ambient.y = ambient.z = 0.f;
		//diffuse.x = diffuse.y = diffuse.z = 0.f;
		//specular.x = specular.y = specular.z = 0.f;
		material_name = "";
		texture_name = "";
	}
};


// メッシュ情報
struct ObjMeshInfo {
	std::vector<std::vector<FacePolygon>>face_list;
};


class ObjFile {
public:

	// 面情報の種類
	enum FaceInfoType {
		VERTEX,
		UV,
		NORMAL,
		TOTAL_FACE_INFO
	};

public:

	ObjFile();

	void Release() {};

	// 読み込み
	bool Load(
		std::string obj_file_path,
		std::string texture_file_path,
		int &out_total_material_num
	);

	// 表示
	void DrawSubSet(int material_num);

private:

	void VertexFscanfLoad(
		FILE*p_file,
		D3DXVECTOR3 &vec3);


	// メッシュ情報読み込み
	bool MeshLoad(
		std::string file_name,
		std::string texture_file_path,
		int &out_total_material_num,
		std::vector<D3DXVECTOR3>&out_vertex_list,
		std::vector<D3DXVECTOR3>&out_normal_list,
		std::vector<D3DXVECTOR2>&out_uv_list,
		std::vector<std::vector<FacePolygon>>&out_face_list
	);


	// 面情報からカスタムバーテックス作成
	void FaceInfoLoad(
		FILE*p_file,
		std::vector<std::vector<FacePolygon>>&face_list,
		std::vector<ObjectSubset>&out_object_sub_set
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
		std::string mtl_file_name,
		std::string texture_file_path
	);


	// usemtl読み込み
	void UseMaterialInfoLoad(
		FILE*p_file
	);


	// 面情報代入
	std::vector<FacePolygon> InsertFaceList(
		std::vector<std::vector<std::string>>face_info_str
	);


	// 文字列分割
	std::vector<std::string> SplitStr(
		char cut_base_str,
		const std::string &string
	);


	// 整数値変換
	bool Vector3ConversionByString(
		int &axis,
		const std::string &string
	);


	// 頂点バッファ生成
	void VertexBufferCreate(
		int vertex_num,
		std::vector<D3DXVECTOR3>vertex_list,
		std::vector<D3DXVECTOR3>normal_list,
		std::vector<D3DXVECTOR2>uv_list,
		std::vector<std::vector<FacePolygon>>face_list
	);


	// 面情報から頂点情報埋め込み
	bool IndexBufferCreateFaceBase(
		int face_num,
		std::vector<std::vector<FacePolygon>>face_list
		);


	// 4面を3面にする
	std::vector<FacePolygon> Face4IsCutToFace3(
		std::vector<FacePolygon>face_polygon_list
	);

private:

	// 総マテリアル数
	int m_total_face_num;

	std::vector<UINT>m_index;

	// Objの方のマテリアル名配列
	std::vector<std::string>m_usemtl_name_list;

	// 描画を入れ替えるサブセット
	std::vector<ObjectSubset>m_object_sub_set_list;

	// マテリアルデータ配列(マテリアル名,マテリアルデータ)
	std::unordered_map<std::string,MaterialData>m_material_data_list;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * m_p_vertex_buffer;

	// インデックスバッファ
	IDirect3DIndexBuffer9 * m_p_index_buffer;

	// DirectGraphics
	Graphics *m_p_graphics;
};