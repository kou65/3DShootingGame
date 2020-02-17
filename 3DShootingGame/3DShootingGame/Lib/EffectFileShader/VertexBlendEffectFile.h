#pragma once
#include"../Graphics/Graphics.h"
#include"EffectFileBase.h"


class VertexBlendEffectFile : public EffectFileBase{
public:

	VertexBlendEffectFile();

	// ������
	bool Init()override;

	// �X�V
	void Update()override;

	// �{�[���s��Z�b�g
	void SetBoneMatrix(D3DXMATRIX*bone_mat);

	void SetBoneMatrixArray(
		D3DXHANDLE&handle,
		D3DXMATRIX*mat,
		const int &array_num
	);

	void SetBoneCount(const int&count);

	void SetMaxIndex(const int&index);

private:

	// ���_�V�F�[�_�[�̏�����
	void InitVertexDecl();

private:

	// �{�[����
	int m_bone_count;

	// �ő�u�����h��
	int m_max_index;


	// �{�[���s��
	D3DXMATRIX *m_bone_mat_list;

	// �{�[���s��n���h��
	D3DXHANDLE m_bone_mat_h;

	// �n���h��
	D3DXHANDLE m_total_index_h;

};