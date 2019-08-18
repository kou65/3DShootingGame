#pragma once
// DirectInputバージョンの宣言
// (どのバージョンの直接入力インターフェースを使用したいかを設定する為に使用される)
#define	DIRECTINPUT_VERSION	0x0800
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")



// ===========================
/**
* @file XInput.h
* @brief キー入力を行う
*/
// ===========================


namespace DirectInput {


	/**
	* @brief 初期化
	* @return bool 初期化に成功すればtrue,失敗すればfalse
	*/
	bool Init();


	/**
	* @brief 解放処理
	*/
	void Release();


	/**
	* @brief インプット状態のアップデート
	*/
	void Update();


	/**
	* @brief LPDIRECTINPUT8を返す
	*/
	LPDIRECTINPUT8 GetLpDirectInput8();

}