﻿#include"../DirectInput.h"
#include"../../Window/Window.h"



namespace KeyBoard {

	// DirectInputデバイス・オブジェクトポインタ
	LPDIRECTINPUTDEVICE8 p_key_device = NULL;

	// キーボードバッファ
	const int KEY_BUFFER = 256;

	// キー情報数
	const int KEY_INFO = 2;

	// キーボード情報を受け取る
	BYTE key_board_state[KEY_INFO][KEY_BUFFER];

	bool Init() {
		// DirectInputデバイスオブジェクト作成
		if (FAILED(
			DirectInput::GetLpDirectInput8()->CreateDevice(
				// キーボードを受け付ける
				GUID_SysKeyboard,
				// DirectInputDevice8のポインタ
				&p_key_device,
				// 使わない
				NULL)
		)
			) {
			return false;
		}

		// デバイスをキーボードに設定
		if (FAILED(p_key_device->SetDataFormat(&c_dfDIKeyboard)))
		{
			return false;
		}

		// 強調レベルの設定

		// 常時取得
		if (FAILED(p_key_device->SetCooperativeLevel(
			Window::GetWindowHandle(),
			DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)// 協調レベル
		)
			)
		{
			return false;
		}

		// 入力デバイスのアクセス権取得
		p_key_device->Acquire();

		return true;
	}


	void Release() {

		// キーデバイス解放
		p_key_device->Release();
	}


	void Update() {

		// 今のキー状態を後にコピー
		memcpy(key_board_state[1], key_board_state[0], KEY_BUFFER);

		// キーの状態をWindowsから取得
		p_key_device->GetDeviceState(
			sizeof(key_board_state[0]),
			&key_board_state[0]
		);
	}

	const int BIT_MASK = 0x80;

	bool IsKeyPush(int keycode) {

		// 1000 0000のビットが立っているかどうかを調べる
		// そして前回のインプットが押されていない場合
		if ((key_board_state[0][keycode] & BIT_MASK)
			&& (key_board_state[1][keycode] ^ BIT_MASK)) {
			return true;
		}
		return false;
	}

	bool IsKeyPushing(int keycode) {

		// 1000 0000のビットが立っているかどうかを調べる
		if ((key_board_state[0][keycode] & BIT_MASK)) {

			return true;
		}
		return false;
	}

	bool IsKeyExit(int keycode) {

		// 1000 0000のビットが立っているかどうかを調べる
		// そして前回のインプットが押されている場合かつ
		// 離したとき
		if ((key_board_state[0][keycode] ^ BIT_MASK)
			&& (key_board_state[1][keycode] & BIT_MASK)) {
			return true;
		}
		return false;
	}
}
