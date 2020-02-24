#pragma once

#define	DIRECTINPUT_VERSION	0x0800
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")


namespace KeyBoard {


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
	* @brief キー状態のアップデート
	*/
	void Update();


	/**
	* @brief 押した瞬間
	* @return bool キーが押されたかどうか、押されたらtrue,押されていなかったらfalse
	*/
	bool IsKeyPush(int key_code);


	/**
	* @brief 押している状態
	* @return bool キーが押されたらtrue押されていなかったらfalse
	*/
	bool IsKeyPushing(int key_code);


	/**
	* @brief キーを離した瞬間
	* @return bool キーが離されたらtrue離されなかったらfalse
	*/
	bool IsKeyExit(int key_code);


	/*
	* @brief キー数字入力
	*/
	void InputKeyNum();

	/*
	* @brief キー入力数字出力
	*/
	void OutPutKeyNum();
}
