
#ifndef FILE_OBJECT_DATA_MANAGER_H
#define FILE_OBJECT_DATA_MANAGER_H

#include"../../ObjectSystem/Object3D/Object3D.h"
#include"../../Factory/ObjectFactory/ObjectFactory.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../ManagerBase/ManagerBase.h"
#include<string>
#include<stdio.h>
#include<unordered_map>




/**
* @brief ファイルを読み込んでデータを管理するファイルデータ管理者
*/
class FileDataManager : public ManagerBase{
public:


	/**
	* @brief コンストラクタ
	* @param[in] object_factory
	* @param[in] camera
	* @param[in] p_data
	*/
	FileDataManager(
		std::shared_ptr<ObjectFactory>object_factory,
		std::shared_ptr<Camera3D>camera,
		std::shared_ptr<ObjectData>p_data
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override {};


	/**
	* @brief 位置配列の最大数を返す
	* @param[in] res_name
	* @return int 位置配列の最大数
	*/
	int GetMaxPosListNum(const std::string&res_name);


	/**
	* @brief 位置情報を返す
	* @param[in] res_name リソース名
	* @param[in] i 配列数
	*/
	Vec3 GetPosList(const std::string&res_name,const int &i);


	/**
	* @brief それぞれ個のピース数量を返す
	*/
	int GetPieceNum(
		const int&map_size_num,
		const int&vector_num
	);


	/**
	* @brief ピースのサイズを返す
	*/
	int GetPieceNumListSize(const int &map_size_num);


	/**
	* @brief ピースリストサイズを返す
	*/
	int GetPieceListSize();


	/**
	* @brief 配列のサイズを消す
	*/
	void Clear();

private:


	/**
	* @brief 生成オブジェクト
	*/
	void CreateGameObject();


	/**
	* @brief オブジェクトファイルを読み込み
	* @param[in] p_file FILEポインタ
	*/
	void LoadVec3(FILE*p_file);


	/**
	* @brief 乱数設置位置番号読み込み
	*/
	void LoadRandomPosNum(FILE*p_file);


	/**
	* @brief ソート
	*/
	void Sort();


private:

	//! カメラ
	std::weak_ptr<Camera3D>mp_camera;

	//! オブジェクト工場
	std::weak_ptr<ObjectFactory>mp_mng;

	//! オブジェクトデータ
	std::weak_ptr<ObjectData>mp_object_data;

	//! オブジェクトのパラメータを入れて後で生成する用
	//! 位置で比較し、位置が近いオブジェクトだけ生成する
	std::unordered_map<std::string, std::vector<Vec3>>m_obj_pos_list;

	//! 乱数ピースリスト
	std::vector<std::vector<int>>m_piece_list;
};

#endif