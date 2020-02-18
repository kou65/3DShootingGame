#include"ObjectManager.h"




void ObjectManager::Update() {


	for (auto &i_obj : m_p_insert_obj) {

		m_p_object_list.emplace_back(std::move(i_obj));
	}

	if (m_p_insert_obj.size() > 0) {
		m_p_insert_obj.clear();
	}

	for (auto &object : m_p_object_list) {

		if (object.get()->IsActive() == true) {
			object.get()->Update();
		}
	}

	ObjListAutoDelete();
}



void ObjectManager::Draw() {

	for (auto &object : m_p_object_list) {

		if (object.get() != nullptr) {
			object.get()->Draw();
		}
	}
}


void ObjectManager::ObjListAutoDelete() {

	for (auto itr = m_p_object_list.begin();
		itr != m_p_object_list.end();) {

		// 活動していないなら
		if ((*itr).get()->IsActive() == false) {

			ObjectBase * obj = (*itr).get();

			// objにメモリが存在するなら
			if (obj != nullptr)
			{
				// メモリ解放
				//delete(obj);
				(*itr).reset();
			}

			// 要素解放
			itr = m_p_object_list.erase(itr);
			continue;
		}

		// イテレータ加算
		itr++;
	}
}


void ObjectManager::ObjListAllDelete() {

	for (auto itr = m_p_object_list.begin();
		itr != m_p_object_list.end(); itr++) {

		ObjectBase*obj = (*itr).get();

		// メモリ解放
		itr->reset();
	}

	// 要素を全て解放
	m_p_object_list.clear();
}


void ObjectManager::Entry(ObjectBase*object) {

	m_p_object_list.emplace_back(object);
}


void ObjectManager::InsertListEntry(ObjectBase*object) {
	m_p_insert_obj.emplace_back(object);
}

