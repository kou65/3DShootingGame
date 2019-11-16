#include"Filed.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2DObject.h"
#include"../Lib/3D/Sprite3D/Sprite3D/Sprite3DObject.h"


void Filed::Update(){

}


void Filed::Draw() {


	Sprite3DData ground(0.f, 0.f, 0.f, "ground");

	ground.scale_width = 10000.f;
	ground.scale_height = 10000.f;
	ground.polygon_dir = FLOOR;
	ground.pos.y = -5.f;
	ground.ofset.x = 0.5f;
	ground.ofset.y = 0.5f;

	Sprite3DObject sprite_obj;


	sprite_obj.BoardDraw(ground);
}
