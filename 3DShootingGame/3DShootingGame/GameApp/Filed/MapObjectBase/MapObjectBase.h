#pragma once
#include"../../../ObjectSystem/Object3D/Object3D.h"
#include"../../../ObjectSystem/Object3DInterface/Object3DInterface.h"



class MapObjectBase : public Object3D {
public:

	MapObjectBase(Object3DInterface*inter) {
		m_p_interface_obj = inter;
	}

	MapObjectBase() {}

	// 前方削除
	void FrontDelete() {

	}

	// 後方削除
	void BackDelete() {

	}

	// 活動関数にアクセス可能
	void SetActive(bool is_active) {
		m_is_active = is_active;
	}

protected:

	Object3DInterface * m_p_interface_obj;
};