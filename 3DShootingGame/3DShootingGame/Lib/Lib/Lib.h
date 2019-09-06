#pragma once
#include"../../LoadResource/LoadResource.h"
#include"../../SetRenderStateFile/SetRenderStateFile.h"
#include"../../DebugMode/DebugMode.h"
#include"../../Lib/DirectInput/DirectInput.h"
#include"../../Lib/Sound/Sound.h"
#include"../../Lib/D3DFont/D3DFont.h"
#include"../../Lib/FPS/FPS.h"
#include"../../Lib/DirectInput/JoyStick/JoyStick.h"






namespace Lib {


	bool Init();

	void Release();
}