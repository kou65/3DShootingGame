﻿
#ifndef FILE_OBJECT_DATA_MANAGER_H
#define FILE_OBJECT_DATA_MANAGER_H

#include"../../ObjectSystem/Object3D/Object3D.h"
#include"../ObjectFactory/ObjectFactory.h"
#include"../ObjectData/ObjectData.h"
#include<string>
#include<stdio.h>
#include<unordered_map>




class FileObjectDataManager {
public:



public:

	// インスタンス
	FileObjectDataManager(
		ObjectFactory*of,
		Camera3D*camera,
		ObjectData*p_data
	);

	void CreatePlayerAndEnemy();

	// オブジェクトファイルを読み込み
	void Load(FILE*p_file);

	void Sort();

private:

	Camera3D*m_p_camera;
	ObjectFactory *m_p_mng;
	ObjectData*m_p_object_data;

	// オブジェクトのパラメータを入れて後で生成する用
	// 位置で比較し、位置が近いオブジェクトだけ生成する
	std::unordered_map<std::string, std::vector<Vec3>>m_obj_pos;
};

#endif