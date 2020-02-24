#include"Object3DInterface.h"


Object3DInterface::Object3DInterface(Object3D*object) {
	m_p_object = object;
}


Vec3 Object3DInterface::GetPos() {
	return m_p_object->GetPos();
}