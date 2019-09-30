#include"Light.h"



Light::Light(
	Graphics*graphics,
	const D3DLIGHT9 &set_light
) {

	this->graphics = graphics;
	m_light = set_light;
}


Light::Light(
	Graphics*graphics
) {

	// グラフィックスを代入させる
	this->graphics = graphics;

}


void Light::DefaultLightMode() {

	// メモリをゼロにする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	// ライトの色情報
	float r = 4.f;
	float g = 4.f;
	float b = 4.f;

	// ライトのタイプ
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	// 原色
	m_light.Diffuse.r = r;
	m_light.Diffuse.g = g;
	m_light.Diffuse.b = b;
	// スペキュラー光
	m_light.Specular.r = r;
	m_light.Specular.g = g;
	m_light.Specular.b = b;
	// アンビエント光
	m_light.Ambient.r = r / 2.0f;
	m_light.Ambient.g = g / 2.0f;
	m_light.Ambient.b = b / 2.0f;
	// ライトの位置
	m_light.Position.x = 100.f;
	m_light.Position.y = 100.f;
	m_light.Position.z = 100.f;

	// 方向
	m_light.Direction = D3DXVECTOR3(0.f, 100.f, 0.f);
	D3DXVec3Normalize(
		(D3DXVECTOR3*)& m_light.Direction,
		&D3DXVECTOR3(10.0f, -1.5f, 0.7f));
}


bool Light::On() {

	// デバイスがない場合
	if (graphics == nullptr) {
		return false;
	}

	LPDIRECT3DDEVICE9 device = graphics->GetLpDirect3DDevice9();

		device->SetLight(0, &m_light);
		device->LightEnable(0, true);

		// 発電所を回す
		device->SetRenderState(D3DRS_LIGHTING, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);
		device->SetRenderState(D3DRS_AMBIENT, 0xfffffff);

		return true;
}