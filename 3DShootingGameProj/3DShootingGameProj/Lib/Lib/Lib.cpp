#include"Lib.h"
#include"../Texture/TextureManager/TextureManager.h"



namespace Lib {

	bool Init() {

		// 初期化
		if (Window::Init() == false) {
			return false;
		}

		if (Graphics::GetInstance()->Init() == false) {
			return false;
		}

		if (DirectInput::Init() == false) {
			return false;
		}

		if (Sound::Init() == false) {
			return false;
		}

		if (D3DFont::Init() == false) {
			return false;
		}

		return true;
	}


	void Release() {

		// 解放処理
		Debugger::GetInstance().Release();
		Sound::Release();
		DirectInput::Release();
		Graphics::GetInstance()->Release();
		TextureManager::GetInstance()->AllRelease();
	}
}