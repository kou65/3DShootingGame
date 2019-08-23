#include"D3DDefaultMesh.h"
#include"../Light/Light.h"



D3DDefaultMesh::D3DDefaultMesh() {

	// メッシュ生成
	CreateTeapot();
	CreateTorus();
	CreateCone();
}


void D3DDefaultMesh::AllDraw() {

	Light light;

	light.NormalLightOn();

	Draw(m_p_teapot, D3DXVECTOR3(-10.f, 0.f, 0.f));
}


void D3DDefaultMesh::Draw(const LPD3DXMESH &p_mesh,const D3DXVECTOR3 &pos) {


	D3DXMATRIX matrix_world;

	D3DMATERIAL9 material;

	D3DCOLORVALUE color_value;

	color_value.a = 2.f;
	color_value.b = 2.f;
	color_value.g = 2.f;
	color_value.r = 1.f;

	material.Diffuse = color_value;
	material.Ambient = color_value;

	// 移動
	D3DXMatrixTranslation(
		&matrix_world,
		pos.x,
		pos.y,
		pos.z
	);
	
	D3D9::GetLpDirect3DDevice9()->SetTransform(
		D3DTS_WORLD,
		&matrix_world
		);

	// マテリアルをセット
	D3D9::GetLpDirect3DDevice9()->SetMaterial(&material);

	p_mesh->DrawSubset(0);
}


void D3DDefaultMesh::CreateTeapot() {

	D3DXCreateTeapot(
		D3D9::GetLpDirect3DDevice9(),
		&m_p_teapot,
		NULL
	);
}


void D3DDefaultMesh::CreateTorus() {
	D3DXCreateTorus(
		D3D9::GetLpDirect3DDevice9(),
		0.3f,
		0.8f,
		32,
		32,
		&m_p_tours,
		NULL
	);
}


void D3DDefaultMesh::CreateCone() {
	D3DXCreateCylinder(
		D3D9::GetLpDirect3DDevice9(),
		0.7f,
		0.0f,
		1.5f,
		30,
		30,
		&m_p_cone,
		NULL);
}