#pragma once
#include"../../../ObjectSystem/Object3D/Object3D.h"
#include"../../../ObjectSystem/Object3DInterface/Object3DInterface.h"



class MapObjectBase : public Object3D {
public:

	MapObjectBase(Object3DInterface*inter) {
		m_p_interface_obj = inter;
	}

	MapObjectBase() {}

	// �O���폜
	void FrontDelete() {

	}

	// ����폜
	void BackDelete() {

	}

	// �����֐��ɃA�N�Z�X�\
	void SetActive(bool is_active) {
		m_is_active = is_active;
	}

protected:

	Object3DInterface * m_p_interface_obj;
};