#pragma once
#include"../../D3D9/D3D9.h"



class Light {
public:

	void NormalLightOn();

	void On();

private:

	D3DLIGHT9 m_light;
};