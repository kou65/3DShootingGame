#pragma once
#include"../Object/Object.h"
#include<vector>
#include<memory>



class ObjectManager {
public:

	ObjectManager() {
	}

	void Update() {

		for (auto &object : m_p_object_list) {
			object.get()->Update();
		}
	}

	void Draw() {

		for (auto &object : m_p_object_list) {
			object.get()->Draw();
		}
	}

	void Entry(Object*object) {

		m_p_object_list.emplace_back(object);
	}

	void ObjListAutoDelete() {

		for (auto itr = m_p_object_list.begin();
			itr != m_p_object_list.end();) {

			// 活動していないなら
			if ((*itr).get()->IsActive() == false) {
				
				Object * obj = (*itr).get();

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


	void ObjListAllDelete() {

		for (auto itr = m_p_object_list.begin();
			itr != m_p_object_list.end(); itr++) {

			Object*obj = (*itr).get();

			// メモリ解放
			itr->reset();
		}

		// 要素を全て解放
		m_p_object_list.clear();
	}

private:

	std::vector<std::unique_ptr<Object>>m_p_object_list;
};