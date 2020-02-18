
#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include"../ObjectBase/ObjectBase.h"
#include<vector>
#include<memory>
#include<iostream>



class ObjectManager {
public:

	ObjectManager() {}

	void Update();

	void Draw();

	void Entry(ObjectBase*object);

	void InsertListEntry(ObjectBase*object);

	void ObjListAutoDelete();

	void ObjListAllDelete();

private:

	std::vector<std::unique_ptr<ObjectBase>>m_p_insert_obj;
	std::vector<std::unique_ptr<ObjectBase>>m_p_object_list;
};


#endif