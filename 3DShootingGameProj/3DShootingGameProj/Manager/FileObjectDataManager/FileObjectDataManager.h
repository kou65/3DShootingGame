
#ifndef FILE_OBJECT_DATA_MANAGER_H
#define FILE_OBJECT_DATA_MANAGER_H

#include"../../ObjectSystem/Object3D/Object3D.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include<string>
#include<stdio.h>
#include<unordered_map>




class FileObjectDataManager {
public:

	// インスタンス
	FileObjectDataManager(
		ObjectFactory*of,
		Camera3D*camera,
		ObjectData*p_data
	);

	void CreateObject();

	// オブジェクトファイルを読み込み
	void Load(FILE*p_file);



	// 位置配列の最大数を返す
	int GetMaxPosListNum(const std::string&res_name);

	// 位置情報を返す
	Vec3 OutPosList(const std::string&res_name,const int &i);

private:

	// ソート
	void Sort();

private:

	// カメラ
	Camera3D*m_p_camera;

	// 工場
	ObjectFactory *m_p_mng;

	// オブジェクトデータ
	ObjectData*m_p_object_data;

	// オブジェクトのパラメータを入れて後で生成する用
	// 位置で比較し、位置が近いオブジェクトだけ生成する
	std::unordered_map<std::string, std::vector<Vec3>>m_obj_pos_list;
};

#endif