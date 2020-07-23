
#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include"../../ObjectSystem/ObjectBase/ObjectBase.h"
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
	* @brief 一つだけのobjを描画
	*/
	void DrawUniqueObj();


	/**
	* @brief 共有されているobjの描画
	*/
	void DrawSharedObj();


	/**
	* @brief zテクスチャ書き込み
	*/
	void DrawZTexture();


	/**
	* @brief 一つだけのオブジェクト追加
	* @brief 同時にunique_ptrの所有権の移動も行う(つまりこちらのptrが本体になる)
	*/
	void EntryUniqueObj(std::unique_ptr<ObjectBase>object);


	/**
	* @brief 共有オブジェクト追加
	* @brief 同時にshered_ptrの所有権の移動も行う(つまりこちらのptrが本体になる)
	*/
	void EntrySharedObj(std::shared_ptr<ObjectBase>obj);


	/**
	* @brief 全て削除
	*/
	void AllRelease();

private:


	/**
	* @brief コンストラクタ
	*/
	ObjectManager() {};


	/**
	* @brief 活動停止していたら自動削除
	*/
	void NotActiveAutoDelete();

	/**
	* @brief 活動停止していたら自動削除
	*/
	void UniqueObjAutoDelete();


	/**
	* @brief 共有自動削除
	*/
	void SharedObjAutoDelete();


	/**
	* @brief オブジェクト挿入
	*/
	void InsertUniqueObj();


	/**
	* @brief 共有オブジェクト挿入
	*/
	void InsertSharedObj();


	/**
	* @brief オブジェクト挿入
	*/
	void InsertObject();

private:

	//! オブジェクトリスト
	std::vector<std::unique_ptr<ObjectBase>>mp_object_list;
	//! 共有用
	std::vector<std::shared_ptr<ObjectBase>>mp_object_list_shared;

	//! unique代入用
	std::vector<std::unique_ptr<ObjectBase>>mp_insert_obj;
	//! 共有代入用
	std::vector<std::shared_ptr<ObjectBase>>mp_insert_obj_shared;

};


#endif