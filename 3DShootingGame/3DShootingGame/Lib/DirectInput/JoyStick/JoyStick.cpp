#include"JoyStick.h"
#include"../DirectInput.h"
#include"../../Window/Window.h"



// ジョイスティックが一つも無いとコールバック関数は一切呼ばれない
// また全てのジョイスティックの列挙が終わればこの関数は呼ばれない
// DIENUM_STOPをしないと止まらないというわけではない



namespace JoyStick {


	LPDIRECTINPUTDEVICE8 p_joy_stick_device;

	// ジョイスティック生成コールバック関数
	BOOL PASCAL EnumJoySticksCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	bool JoyStickDeviceSet();

	// ジョイスティックの入力
	DIJOYSTATE joy[2];

	// 軸設定関数
	void EnumAxisSet();

	void InputUpdate();

	bool EnumDeviceUpdate();


	void Init() {

		p_joy_stick_device = NULL;

		// ジョイステート初期化
		for (int i = 0; i < 2; i++) {
			ZeroMemory(&joy[i], sizeof(joy[i]));
		}

	}


	void Update() {


		// 毎回列挙判定をする
		EnumDeviceUpdate();

		// ここでもう起動する
		if (p_joy_stick_device != NULL) {

			// 起動準備完了
			p_joy_stick_device->Poll();
			// 起動
			p_joy_stick_device->Acquire();
		}

		// キーアップデート
		InputUpdate();
	}


	bool IsBottomPush(int joy_code) {
		
		if ((joy[0].rgbButtons[joy_code] & 0x80) &&
			!(joy[1].rgbButtons[joy_code] & 0x80)
			) {
			// 押された
			return true;
		}

		return false;
	}

	
	bool IsBottomPushing(int joy_code) {

		if ((joy[0].rgbButtons[joy_code] & 0x80) ||
			(joy[1].rgbButtons[joy_code] & 0x80)
			) {
			// 押されている
			return true;
		}
		return false;
	}


	bool IsBottomExit(int joy_code) {

		if (!(joy[0].rgbButtons[joy_code] & 0x80) &&
			(joy[1].rgbButtons[joy_code] & 0x80)
			) {
			// キーが離された
			return true;
		}
		return false;
	}


	// 列挙の更新
	bool EnumDeviceUpdate() {

		HRESULT ret;

		// ジョイスティックの列挙
		ret = DirectInput::GetLpDirectInput8()->EnumDevices(
			// デバイスクラス(他のクラスに分類されない全てのデバイス)
			DI8DEVCLASS_GAMECTRL,
			// 列挙された各デバイスに対して一回呼び出されるコールバック関数のアドレス
			EnumJoySticksCallBack,
			// 呼び出されるたびに列挙コールバックに渡される32ビット
			NULL,
			// 列挙の範囲を指定するフラグ値(接続およびインストールされているデバイスのみ)
			DIEDFL_ATTACHEDONLY
		);


		if (FAILED(ret)) {
			// ジョイスティックの列挙失敗
			return false;
		}

		if (!p_joy_stick_device) {
			// ジョイスティックが一つも見つからない
			return false;
		}
		
		return true;
	}


	void InputUpdate() {

		if (p_joy_stick_device == nullptr) {
			return;
		}

		// コピー
		memcpy(&joy[1], &joy[0], sizeof(DIJOYSTATE));

		// キー状態を受け取り
		HRESULT ret = p_joy_stick_device->GetDeviceState(
			sizeof(joy[0]),
			&joy[0]
		);


		if (FAILED(ret)) {
			// 失敗ならもう一度動作開始を行う
			p_joy_stick_device->Acquire();
		}
	}


	// 見つかったジョイスティック一つずつに対して呼び出すコールバック関数
	// また引数のlpddiにはその時のジョイスティックの情報が入っている
	BOOL PASCAL EnumJoySticksCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
	{
		// リザルト
		HRESULT result;

		if (p_joy_stick_device == nullptr) {
			// ジョイスティックデバイス作成
			result = DirectInput::GetLpDirectInput8()->CreateDevice(
				lpddi->guidInstance,
				&p_joy_stick_device,
				NULL
			);


			if (FAILED(result)) {
				// 次のジョイスティックを続けて列挙させたい場合
				return DIENUM_CONTINUE;
			}

			if (JoyStickDeviceSet() == false) {

				return DIENUM_CONTINUE;
			}

			// 軸をセット
			EnumAxisSet();
		}

		// 最初の一つのみで終わる
		return DIENUM_STOP;
	}


	bool JoyStickDeviceSet() {

		// デバイスの操作準備

		HRESULT result;

		// 入力データのフォーマット設定方法をDirectInputに指示
		if (FAILED(result = p_joy_stick_device->SetDataFormat(
			// データ取得にDIJOYSTATE構造体を使用する
			// DIJOYSTATE専用の物を使わなくてはいけなくなる
			&c_dfDIJoystick)
		)
			) {
			// 設定失敗
			return false;
		}

		// ここまででジョイスティックデバイス作成、
		// データフォーマットが設定されている状態

		// 協調レベルの設定
		if (FAILED(result = p_joy_stick_device->SetCooperativeLevel(
			// ウィンドウハンドル
			Window::GetWindowHandle(),
			// 協調レベル
			DISCL_EXCLUSIVE | DISCL_FOREGROUND)
		)
			) {
			return false;
		}

		// 最初の一つのみで終わる
		return true;
	}


	void EnumAxisSet() {


		DIPROPRANGE diprg;

		// 軸設定
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMax = 1000;
		diprg.lMin = -1000;

		// X軸
		diprg.diph.dwObj = DIJOFS_X;
		p_joy_stick_device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y軸
		diprg.diph.dwObj = DIJOFS_Y;
		p_joy_stick_device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Z軸
		diprg.diph.dwObj = DIJOFS_Z;
		p_joy_stick_device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// RX軸
		diprg.diph.dwObj = DIJOFS_RX;
		p_joy_stick_device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// RY軸
		diprg.diph.dwObj = DIJOFS_RY;
		p_joy_stick_device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// RZ軸
		diprg.diph.dwObj = DIJOFS_RZ;
		p_joy_stick_device->SetProperty(DIPROP_RANGE, &diprg.diph);
	}
}