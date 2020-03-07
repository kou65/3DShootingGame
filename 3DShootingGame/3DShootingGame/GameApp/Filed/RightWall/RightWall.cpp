#include"RightWall.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"



void RightWall::Update() {


}


void RightWall::Draw() {

	ObjParameter param;

	param.register_obj_file_name = Const::Obj::PLANE;
	param.pos.x = 10.f;
	param.pos.y = 0.f;
	param.pos.z = 0.f;
	param.scale.x = 2.f;

	Obj::GetInstance()->ShaderDraw(param);
}