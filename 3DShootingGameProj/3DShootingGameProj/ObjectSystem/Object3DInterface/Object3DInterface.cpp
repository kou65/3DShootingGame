#include"Object3DInterface.h"


Object3DInterface::Object3DInterface(Object3D*object) {
	mp_object = object;
}


Vec3 Object3DInterface::GetPos() {
	return mp_object->GetPos();
}