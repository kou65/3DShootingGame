#pragma once
#include"../../LoadResource/LoadResource.h"
#include"../RenderState/RenderState.h"
#include"../../Debugger/Debugger.h"
#include"../../Lib/DirectInput/DirectInput.h"
#include"../../Lib/Sound/Sound.h"
#include"../../Lib/D3DFont/D3DFont.h"
#include"../../Lib/FPS/FPS.h"
#include"../../Lib/DirectInput/JoyStick/JoyStick.h"



/**
* @brief ライブラリをまとめるLib
*/
namespace Lib {

	/**
	* @brief ライブラリの初期化
	*/
	bool Init();


	/**
	* @brief ライブラリの解放
	*/
	void Release();
}