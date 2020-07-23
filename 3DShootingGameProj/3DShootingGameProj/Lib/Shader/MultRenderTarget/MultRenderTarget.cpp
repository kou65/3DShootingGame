#include"MultRenderTarget.h"



MultRenderTarget::MultRenderTarget(
const int&render_index
) {
	
}


void MultRenderTarget::SetBackBuffer() {

	// バックバッファ保存
	mp_back_buf.SaveBuffer();
}


void MultRenderTarget::SetRender(
	RenderTarget&rt
) {


	// マルチレンダリングに必要な情報をセット
	rt.SetRenderTarget();
	rt.SetDepth();

	//Graphics::GetInstance()->Clear();

	// テクスチャサーフェイスのクリア
	Graphics::GetInstance()->GetDevice()->Clear(
		0, NULL, D3DCLEAR_TARGET |
		D3DCLEAR_ZBUFFER,
		// 背景色も変更
		D3DCOLOR_ARGB(255, 255, 255, 255),
		1.0f,
		0
	);

	// ここから描画開始

}


void MultRenderTarget::GetBackBuffer() {


	// 保存しておいたバックバッファを戻す
	mp_back_buf.SetBuffer();

	// 画面をクリア
	// テクスチャサーフェイスのクリア
	Graphics::GetInstance()->Clear();
}


void MultRenderTarget::SetTexture(RenderTarget&rt) {
	rt.GetTexture(mp_tex);
}


void MultRenderTarget::SetDeviceTexture(LPDIRECT3DTEXTURE9&tex) {

	Graphics::GetInstance()->
		GetDevice()->SetTexture(0,tex);
}