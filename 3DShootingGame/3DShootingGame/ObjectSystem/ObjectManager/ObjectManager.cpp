#include"ObjectManager.h"




void ObjectManager::Update() {


	// 活動していなかった削除する関数
	NotActiveAutoDelete();

	// オブジェクトを挿入
	InsertObject();

	// オブジェクトを回す
	for (auto &object : m_p_object_list) {

		// 更新
		object.get()->Update();
	}
}



void ObjectManager::Draw() {

	// オブジェクトを描画
	for (auto &object : m_p_object_list) {

		// 活動していないなら
		if (object.get()->IsActive() != true) {
			continue;
		}

		// 描画できないなら
		if (object.get()->CanDraw() != true) {
			continue;
		}

		// 描画
		object.get()->Draw();
	}
}


void ObjectManager::InsertObject() {


	// 配列に溜めていたオブジェクトを代入
	for (auto &i_obj : m_p_insert_obj) {

		// オブジェクトポインタを委譲
		m_p_object_list.emplace_back(std::move(i_obj));
	}

	// オブジェクトが存在するなら
	if (m_p_insert_obj.size() > 0) {
		m_p_insert_obj.clear();
	}

	for (auto &obj : m_p_insert_obj_shared) {

		m_p_object_list_shared.emplace_back(std::move(obj));
	}

	if (m_p_insert_obj_shared.size() > 0) {
		m_p_insert_obj_shared.clear();
	}
}


void ObjectManager::NotActiveAutoDelete() {

	for (auto itr = m_p_object_list.begin();
		itr != m_p_object_list.end();) {

		// 活動していないなら
		if ((*itr).get()->IsActive() == false) {

			ObjectBase * obj = (*itr).get();

			// objにメモリが存在するなら
			if (obj != nullptr)
			{
				// メモリ解放
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


void ObjectManager::SharedEntry(std::shared_ptr<ObjectBase>obj) {

	m_p_insert_obj_shared.emplace_back(obj);
}


void ObjectManager::InsertSharedObject() {


	// 配列に溜めていたオブジェクトを代入
	for (auto &i_obj : m_p_insert_obj) {

		// オブジェクトポインタを委譲
		m_p_object_list_shared.emplace_back(std::move(i_obj));
	}

	// オブジェクトが存在するなら
	if (m_p_insert_obj.size() > 0) {
		m_p_insert_obj.clear();
	}
}

void ObjectManager::SharedAutoDelete() {

}


void ObjectManager::AllDelete() {

	for (auto itr = m_p_object_list.begin();
		itr != m_p_object_list.end(); itr++) {

		ObjectBase*obj = (*itr).get();

		// メモリ解放
		itr->reset();
	}

	// 要素を全て解放
	m_p_object_list.clear();
}


void ObjectManager::EmplaceBack(ObjectBase*object) {

	m_p_object_list.emplace_back(object);
}


void ObjectManager::Entry(ObjectBase*object) {
	m_p_insert_obj.emplace_back(object);
}

