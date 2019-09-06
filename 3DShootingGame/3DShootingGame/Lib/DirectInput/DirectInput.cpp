#include"DirectInput.h"
#include"../Window/Window.h"
#include"../DirectInput/KeyBoard/KeyBoard.h"
#include"JoyStick/JoyStick.h"



namespace DirectInput {

	// DirectInputオブジェクトポインタ
	LPDIRECTINPUT8 p_direct_input8 = NULL;


	bool Init() {

		// DirectInputオブジェクト作成
		if (FAILED(DirectInput8Create(
			// インスタンスハンドル
			GetModuleHandle(NULL),
			// DirectInputのバージョン
			DIRECTINPUT_VERSION,
			// 取得インターフェイスの
			IID_IDirectInput8,
			// インターフェースポインタ
			(void**)&p_direct_input8,
			// 使わない
			NULL
		)
		)
			) {
			return false;
		}

		// キーボードの初期化
		if (KeyBoard::Init() == false) {
			return false;
		}

		// ジョイスティックの初期化
		if (JoyStick::Init() == false) {
			return false;
		}

		return true;
	}


	void Release() {

		// キーボードの解放
		KeyBoard::Release();

		// ダイレクトインプット解放
		p_direct_input8->Release();
	}


	void Update() {

		// キー状態の更新
		KeyBoard::Update();
	}


	LPDIRECTINPUT8 GetLpDirectInput8() {
		return p_direct_input8;
	}
}
