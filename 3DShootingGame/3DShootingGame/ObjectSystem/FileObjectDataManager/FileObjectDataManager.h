#pragma once
#include"../../ObjectSystem/Object3D/Object3D.h"
#include"../ObjectFactory/ObjectFactory.h"
#include<string>
#include<stdio.h>
#include<unordered_map>




class FileObjectDataManager {
public:

	// インスタンス
	FileObjectDataManager(ObjectFactory*of,Camera3D*camera);

	void CreateObject();

	// オブジェクトファイルを読み込み
	void Load(FILE*p_file);

	void Sort();

private:

	Camera3D*m_p_camera;
	ObjectFactory *m_p_mng;

	// オブジェクトのパラメータを入れて後で生成する用
	// 位置で比較し、位置が近いオブジェクトだけ生成する
	std::unordered_map<std::string, std::vector<Vec3>>m_obj_pos;
};