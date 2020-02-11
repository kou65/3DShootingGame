#include"D3DDefaultMesh.h"
#include"../Light/Light.h"



D3DDefaultMesh::D3DDefaultMesh() {

	// メッシュ生成
	CreateTeapot();
	CreateTorus();
	CreateCone();
}


void D3DDefaultMesh::AllDraw() {

	Light light(Graphics::GetInstance());

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
	
	Graphics::GetInstance()->GetDevice()->SetTransform(
		D3DTS_WORLD,
		&matrix_world
		);

	// マテリアルをセット
	Graphics::GetInstance()->GetDevice()->SetMaterial(&material);

	p_mesh->DrawSubset(0);
}


void D3DDefaultMesh::CreateTeapot() {

	D3DXCreateTeapot(
		Graphics::GetInstance()->GetDevice(),
		&m_p_teapot,
		NULL
	);
}


void D3DDefaultMesh::CreateTorus() {
	D3DXCreateTorus(
		Graphics::GetInstance()->GetDevice(),
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
		Graphics::GetInstance()->GetDevice(),
		0.7f,
		0.0f,
		1.5f,
		30,
		30,
		&m_p_cone,
		NULL);
}