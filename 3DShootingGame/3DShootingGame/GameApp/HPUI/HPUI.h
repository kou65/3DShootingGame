#pragma once
#include"../../ObjectSystem/Object2D/Object2D.h"
#include"../Player/Player.h"
#include"../../Lib/Sprite2D/Sprite2DParameter/Sprite2DParameter.h"



class HPUI :public Object2D{
public:

	HPUI(Player*p);

	void Update()override;
	void Draw()override;

private:

	Sprite2DParameter m_sprite_param;
	Player*m_p_player;
};