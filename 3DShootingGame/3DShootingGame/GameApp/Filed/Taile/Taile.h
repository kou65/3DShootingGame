#pragma once
#include"../MapObjectBase/MapObjectBase.h"
#include"../../../Lib/TransformData/TransformData.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"



class Taile : public MapObjectBase {
public:

	Taile(const ObjParameter&data);

	void Update()override;
	void Draw()override;

private:


	ObjParameter m_data;
};