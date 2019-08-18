#pragma once

#define	DIRECTINPUT_VERSION	0x0800
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")



namespace JoyStick {

	void Init();

	void Update();

	bool IsBottomPush(int joy_code);

	bool IsBottomPushing(int joy_code);

	bool IsBottomExit(int joy_code);

}