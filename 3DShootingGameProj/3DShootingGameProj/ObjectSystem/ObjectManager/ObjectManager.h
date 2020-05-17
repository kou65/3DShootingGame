
#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include"../ObjectBase/ObjectBase.h"
#include<vector>
#include<memory>
#include<iostream>


/**
* @brief シングルトンobject管理者クラス
*/
class ObjectManager {
public:

	/**
	* @brief クラスへのアクセス権
	* @return ObjectManager ポインタクラス
	*/
	static ObjectManager*GetInstance(){
		static ObjectManager obj_mng;
		return &obj_mng;
	}

public:


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 描画
	*/
	void Draw();


	/**
	* @brief エントリー
	*/
	void Entry(ObjectBase*object);


	/**
	* @brief 共有エントリー
	*/
	void SharedEntry(std::shared_ptr<ObjectBase>obj);


	/**
	* @brief 全て削除
	*/
	void AllDelete();

private:


	/**
	* @brief コンストラクタ
	*/
	ObjectManager() {};


	/**
	* @brief オブジェクトへ挿入
	*/
	void EmplaceBack(ObjectBase*object);


	/**
	* @brief 活動停止していたら自動削除
	*/
	void NotActiveAutoDelete();


	/**
	* @brief 共有自動削除
	*/
	void SharedAutoDelete();


	/**
	* @brief オブジェクト挿入
	*/
	void InsertObject();


	/**
	* @brief 共有オブジェクト挿入
	*/
	void InsertSharedObject();

private:

	//! 共有代入用
	std::vector<std::shared_ptr<ObjectBase>>mp_insert_obj_shared;

	//! 共有用
	std::vector<std::shared_ptr<ObjectBase>>mp_object_list_shared;

	//! 代入用
	std::vector<std::unique_ptr<ObjectBase>>mp_insert_obj;

	//! オブジェクトリスト
	std::vector<std::unique_ptr<ObjectBase>>mp_object_list;
};


#endif