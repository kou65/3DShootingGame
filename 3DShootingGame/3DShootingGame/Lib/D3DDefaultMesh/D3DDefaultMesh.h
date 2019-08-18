#pragma once
#include"../D3D9/D3D9.h"


// DirectX標準のメッシュを描画


class D3DDefaultMesh {
public:

	D3DDefaultMesh();

	void AllDraw();

	void Draw(const LPD3DXMESH &p_mesh,const D3DXVECTOR3 &pos);

private:

	void CreateTeapot();

	void CreateTorus();

	void CreateCone();

private:
	// メッシュ1
	LPD3DXMESH m_p_teapot;

	// メッシュ2
	LPD3DXMESH m_p_tours;

	// メッシュ3
	LPD3DXMESH m_p_cone;
};