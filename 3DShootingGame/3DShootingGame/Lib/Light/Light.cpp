#include"Light.h"


void Light::On() {

	// ライト
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_light.Direction = D3DXVECTOR3(-1, -20, 0);
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;
	m_light.Ambient.r = 0.5f;
	m_light.Ambient.g = 0.5f;
	m_light.Ambient.b = 0.5f;
	m_light.Range = 1000;

	D3D9::GetLpDirect3DDevice9()->SetLight(0, &m_light);
	// ライトオン
	D3D9::GetLpDirect3DDevice9()->LightEnable(0, true);
	D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_LIGHTING, TRUE);
	D3D9::GetLpDirect3DDevice9()->SetRenderState(D3DRS_AMBIENT, 0x00808080);   // アンビエントライト
}

void Light::NormalLightOn() {

	LPDIRECT3DDEVICE9 device = D3D9::GetLpDirect3DDevice9();

	// ソケットに豆電球をON
	device->SetLight(0, &m_light);

	// ソケットに繋がっているスイッチON
	device->LightEnable(0,TRUE);

	// 発電所を回す
	device->SetRenderState(D3DRS_LIGHTING,TRUE);

	// 世の中を少し白く照らす
	device->SetRenderState(D3DRS_AMBIENT, 0xff030303);

	m_light.Diffuse.a = 0.5f;
}