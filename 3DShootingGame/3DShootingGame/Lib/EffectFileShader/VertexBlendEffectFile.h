#pragma once
#include"../Graphics/Graphics.h"
#include"EffectFileBase.h"



class VertexBlendEffectFile : public EffectFileBase{
public:

	VertexBlendEffectFile();

	// 初期化
	bool Init()override;

	// 更新
	void Update()override;

	// ボーン行列セット
	void SetBoneMatrix(D3DXMATRIX*bone_mat);

private:

	// 頂点シェーダーの初期化
	void InitVertexDecl();

private:

	// ボーン数
	int m_bone_count;

	// ボーン行列
	D3DXMATRIX *m_bone_mat_list;

	// ボーン行列ハンドル
	D3DXHANDLE m_bone_mat_h;
};