
#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include"../ObjectBase/ObjectBase.h"
#include<vector>
#include<memory>
#include<iostream>



class ObjectManager {
public:

	// アクセス権
	static ObjectManager*GetInstance() {
		static ObjectManager obj_mng;
		return &obj_mng;
	}

public:

	void Update();

	void Draw();

	void Entry(ObjectBase*object);

	void AllDelete();

private:

	void EmplaceBack(ObjectBase*object);

	ObjectManager() {};

	void NotActiveAutoDelete();

	void InsertObject();

private:


	std::vector<std::unique_ptr<ObjectBase>>m_p_insert_obj;
	std::vector<std::unique_ptr<ObjectBase>>m_p_object_list;
};


#endif