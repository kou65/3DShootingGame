#include"ObjectManager.h"
#include"../../Lib/3D/OBJ/OBJFile.h"




void ObjectManager::Update() {

	// 削除
	NotActiveAutoDelete();
	
	// オブジェクトを挿入
	InsertObject();

	// オブジェクトを回す
	for (auto &object : mp_object_list) {
	
		// 更新
		object.get()->Update();
	}

	for (auto &object : mp_object_list_shared) {

		object->Update();
	}
}


void ObjectManager::Draw() {

	// zテクスチャの描画を先に行う
	DrawZTexture();

	DrawUniqueObj();

	DrawSharedObj();
}


void ObjectManager::DrawSharedObj() {

	// オブジェクトを描画
	for (auto &object : mp_object_list_shared) {

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


void ObjectManager::DrawUniqueObj() {

	// オブジェクトを描画
	for (auto &object : mp_object_list) {

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


void ObjectManager::DrawZTexture() {

	UINT i;

	// zテクスチャ書き込み開始
	ZTextureManager::GetInstance()->GetZTexturePtr(
		FuncZTexture::Const::Z_TEX_1024
	)->Begin(i, 0);

	// 一つだけのobjを回す
	for (auto &object : mp_object_list) {
	
		// 活動していないなら
		if (object.get()->IsActive() != true) {
			continue;
		}
	
		// 描画できないなら
		if (object.get()->CanDraw() != true) {
			continue;
		}
	
		// 描画
		object.get()->DrawZTexture();
	}

	// 共有用を回す
	for (auto&object : mp_object_list_shared) {

		// 活動していないなら
		if (object.get()->IsActive() != true) {
			continue;
		}

		// 描画できないなら
		if (object.get()->CanDraw() != true) {
			continue;
		}

		// 描画
		object.get()->DrawZTexture();
	}

	ZTextureManager::GetInstance()->GetZTexturePtr(
		FuncZTexture::Const::Z_TEX_1024
	)->End();

}


void ObjectManager::InsertObject() {

	InsertUniqueObj();

	InsertSharedObj();
}



void ObjectManager::InsertUniqueObj() {


	// 配列に溜めていたオブジェクトを代入
	for (auto &i_obj : mp_insert_obj) {

		// オブジェクトポインタを委譲
		mp_object_list.emplace_back(std::move(i_obj));
	}

	// オブジェクトが存在するなら
	if (mp_insert_obj.size() > 0) {
		mp_insert_obj.clear();
	}

}


void ObjectManager::InsertSharedObj() {


	// 配列に溜めていたオブジェクトを代入
	for (auto &i_obj : mp_insert_obj_shared) {

		// オブジェクトポインタを委譲
		mp_object_list_shared.emplace_back(std::move(i_obj));
	}

	// オブジェクトが存在するなら
	if (mp_insert_obj_shared.size() > 0) {
		mp_insert_obj_shared.clear();
	}
}


void ObjectManager::NotActiveAutoDelete() {

	// 活動していなかった削除する関数
	UniqueObjAutoDelete();

	// delete
	SharedObjAutoDelete();
}


void ObjectManager::UniqueObjAutoDelete() {

	for (auto itr = mp_object_list.begin();
		itr != mp_object_list.end();) {

		// 活動していないなら
		if ((*itr).get()->IsActive() == false) {

			ObjectBase * obj = (*itr).get();

			// objにメモリが存在するなら
			if (obj != nullptr)
			{
				// メモリ解放
				(*itr).reset();

				// nullptr代入
				(*itr) = nullptr;
			}

			// 要素解放
			itr = mp_object_list.erase(itr);
			continue;
		}

		// イテレータ加算
		itr++;
	}
}


void ObjectManager::SharedObjAutoDelete() {


	// 定期的に消す
	for (auto itr = mp_object_list_shared.begin();
		itr != mp_object_list_shared.end();) {

		// 活動していないなら
		if ((*itr).get()->IsActive() == false) {

			ObjectBase * obj = (*itr).get();

			// objにメモリが存在するなら
			if (obj != nullptr)
			{
				// メモリ解放
				(*itr).reset();

				// nullptr代入
				(*itr) = nullptr;
			}

			// 要素解放
			itr = mp_object_list_shared.erase(itr);
			continue;
		}

		// イテレータ加算
		itr++;
	}
}


void ObjectManager::AllDelete() {

	for (auto itr = mp_object_list.begin();
		itr != mp_object_list.end(); itr++) {

		ObjectBase*obj = (*itr).get();

		if (obj != nullptr) {

			// メモリ解放
			itr->reset();
		}
	}

	// 要素を全て解放
	mp_object_list.clear();
}


void ObjectManager::EntryUniqueObj(std::unique_ptr<ObjectBase>object) {

	// 所有権を委譲しないとそのままでは上書きエラーになる
	mp_insert_obj.emplace_back(std::move(object));
}


void ObjectManager::EntrySharedObj(std::shared_ptr<ObjectBase>obj) {

	// ポインタ所有権移動
	mp_insert_obj_shared.emplace_back(std::move(obj));
}
