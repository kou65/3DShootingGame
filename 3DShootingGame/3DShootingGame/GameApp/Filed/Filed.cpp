#include"Filed.h"
#include"../../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"
#include"../../Lib/3D/Sprite3D/Sprite3D/Sprite3DUser.h"


void Filed::Update(){

}


void Filed::Draw() {


	Sprite3DParameter ground(0.f, 0.f, 0.f, "ground");
	
	ground.scale_width = 10000.f;
	ground.scale_height = 10000.f;
	ground.polygon_dir = FLOOR;
	ground.pos.y = -5.f;
	ground.ofset.x = 0.5f;
	ground.ofset.y = 0.5f;
	
	Sprite3DUser sprite_obj;
	
	
	sprite_obj.BoardDraw(ground);
}
