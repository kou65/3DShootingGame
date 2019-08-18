#include"SetRenderStateFile.h"




namespace SetRenderStateFile {

	void Init() {
		
		// Zバッファ有効値
		ZBufferMode(TRUE);

		// α有効値
		AlphaEnable(TRUE);

		// ライトモード
		LightMode(NULL);

		// αテスト
		AlphaTest(TRUE);

		AlphaBlend();

		// カリングを行わない
		CullMode(FALSE);
	}


	void ZBufferMode(BOOL is_enable) {

		// Zバッファ
		D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_ZENABLE,is_enable);
		//D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		//D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	}


	void AlphaEnable(BOOL is_enable) {

		// アルファチャンネル
		D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_ALPHABLENDENABLE,is_enable);
		// // 通常透過処理
		D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//Graphics::GetLpDirect3DDevice9()->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);
	}


	void AlphaTest(BOOL is_enable,int value) {
		// アルファテスト(3Dポリゴンの時使う)
		D3D9::GetLpDirect3DDevice9()->SetRenderState(
			D3DRS_ALPHATESTENABLE,
			is_enable
		);
		D3D9::GetLpDirect3DDevice9()->SetRenderState(
			D3DRS_ALPHAREF, 
			value
		);
		// 比較方法
		D3D9::GetLpDirect3DDevice9()->SetRenderState(
			D3DRS_ALPHAFUNC,
			D3DCMP_GREATER);
	}


	void AlphaBlend() {

		// テクスチャαブレンド
		D3D9::GetLpDirect3DDevice9()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		D3D9::GetLpDirect3DDevice9()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		D3D9::GetLpDirect3DDevice9()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		D3D9::GetLpDirect3DDevice9()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		D3D9::GetLpDirect3DDevice9()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		D3D9::GetLpDirect3DDevice9()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	}


	void ShadeMode() {

		// シェードモード:グラデーション
		D3D9::GetLpDirect3DDevice9()->SetRenderState(
			D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
	}


	void LightMode(
		BOOL is_enable
	) {
		// ライトをオンにするかどうか
		D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_LIGHTING,is_enable);
		// スぺきゅら光オン
		//D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	}


	// カリングモード
	void CullMode(
		BOOL is_enable
	) {
		// カリングモード無し
		if (is_enable == false) {
			D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}
	}
}