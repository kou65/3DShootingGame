#pragma once
#include"../../Graphics/Graphics.h"



class Light {
public:

	Light(Graphics*graphics) {

		this->graphics = graphics;
	}

	void NormalLightOn();

	void On();

private:

	// D3Dライト
	D3DLIGHT9 m_light;

	// DirectGraphics
	Graphics *graphics;
};