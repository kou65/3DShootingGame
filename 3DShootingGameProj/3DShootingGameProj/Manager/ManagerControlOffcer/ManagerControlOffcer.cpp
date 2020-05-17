#include"ManagerControlOffcer.h"
#include"../ManagerFactory/ManagerFactory.h"



ManagerControlOffcer::ManagerControlOffcer() {

}


void ManagerControlOffcer::EntryShared(
	std::shared_ptr<ManagerBase>p_mng
) {

	mp_shared_mng_list.emplace_back(p_mng);
}


void ManagerControlOffcer::EntryUnique(
	std::unique_ptr<ManagerBase>p_mng
) {

	// 所有権を委譲
	mp_unique_mng_list.emplace_back(std::move(p_mng));
}


void ManagerControlOffcer::Update() {

	for (auto&mng : mp_shared_mng_list) {
		mng->Update();
	}

	for (auto&mng : mp_unique_mng_list) {
		mng->Update();
	}
}


void ManagerControlOffcer::AllRelease() {


	// 共有用
	for (auto itr = mp_shared_mng_list.begin();
		itr != mp_shared_mng_list.end(); itr++) {

		if ((*itr) != nullptr) {
			// メモリ解放
			itr->reset();
		}
	}

	// 要素を全て解放
	mp_shared_mng_list.clear();

	// ユニーク
	for (auto itr = mp_unique_mng_list.begin();
		itr != mp_unique_mng_list.end(); itr++) {

		ManagerBase*mng = (*itr).get();

		if (mng != nullptr) {
			// メモリ解放
			itr->reset();
		}
	}

	// 要素を全て解放
	mp_unique_mng_list.clear();
}
