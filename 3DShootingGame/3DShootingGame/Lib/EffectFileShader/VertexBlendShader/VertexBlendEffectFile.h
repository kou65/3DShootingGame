#pragma once
#include"../../Graphics/Graphics.h"
#include"../ShaderBase/EffectFileBase.h"


class VertexBlendEffectFile : public ShaderBase{
public:

	VertexBlendEffectFile();

	// ������
	void Init()override;

	// �X�V
	void Update()override;

	// �{�[���s��Z�b�g
	void SetBoneMatrix(D3DXMATRIX*bone_mat);

	void SetBoneMatrixArray(
		D3DXHANDLE&handle,
		D3DXMATRIX*mat,
		const int &array_num
	);

	// �{�[����
	void SetBoneCount(const int&count);

	// �s��C���f�b�N�X
	void SetMaxIndex(const int&index);

	// �s��o�^
	void RegisterMat(
		const int &start_regster,
		const D3DXMATRIX*mat,
		const int &vector4count
	);

	void UpdateParam();

	void SetColor(const D3DXVECTOR4 &color);
	void SetTexture(IDirect3DTexture9* p_tex);
	void SetWorldMatrix(const D3DXMATRIX&world_mat);

private:

	//void Register();

private:

	// �{�[����
	int m_bone_count;

	// �ő�u�����h��
	int m_max_index;

	// �r���[�s��n���h��
	D3DXHANDLE m_camera_view_mat_h;
	// �ˉe�s��n���h��
	D3DXHANDLE m_camera_proj_mat_h;
	// ���[���h�s��n���h��
	D3DXHANDLE m_world_mat_h;
	// �e�N�X�`���n���h��
	D3DXHANDLE m_texture_h;
	// �J���[�n���h��
	D3DXHANDLE m_color_h;


	// ���[���h�s��
	D3DXMATRIX m_world_mat;
	// �J�����r���[�s��
	D3DXMATRIX m_camera_view_mat;
	// �J�����ˉe�ϊ��s��
	D3DXMATRIX m_camera_proj_mat;
	// �J���[�l
	D3DXVECTOR4 m_color;



	// �{�[���s��
	D3DXMATRIX *m_bone_mat_list;

	// �{�[���s��n���h��
	D3DXHANDLE m_bone_mat_h;

	// �n���h��
	D3DXHANDLE m_total_index_h;

};