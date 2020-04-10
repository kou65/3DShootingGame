
#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include"../ObjectBase/ObjectBase.h"
#include<vector>
#include<memory>
#include<iostream>



class ObjectManager {
public:

	// アクセス権
	static ObjectManager*GetInstance(){
		static ObjectManager obj_mng;
		return &obj_mng;
	}

public:

	void Update();

	void Draw();

	void Entry(ObjectBase*object);

	void SharedEntry(std::shared_ptr<ObjectBase>obj);

	void AllDelete();

private:

	void EmplaceBack(ObjectBase*object);

	ObjectManager() {};

	void NotActiveAutoDelete();

	void SharedAutoDelete();

	void InsertObject();

	void InsertSharedObject();

private:

	// 共有代入用
	std::vector<std::shared_ptr<ObjectBase>>m_p_insert_obj_shared;

	// 共有用
	std::vector<std::shared_ptr<ObjectBase>>m_p_object_list_shared;

	// 代入用
	std::vector<std::unique_ptr<ObjectBase>>m_p_insert_obj;

	// オブジェクトリスト
	std::vector<std::unique_ptr<ObjectBase>>m_p_object_list;
};


#endif