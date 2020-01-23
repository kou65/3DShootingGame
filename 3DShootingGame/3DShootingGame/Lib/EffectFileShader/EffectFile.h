#pragma once
#include"../Graphics/Graphics.h"
#include<string>




class EffectFile {
public:

	// �R���X�g���N�^
	EffectFile();

	// �V�F�[�_�[�̏�����
	void InitShader(
		const std::string& shader_name,
		const std::string& tech_name
	);

	// �V�F�[�_�[�̍X�V
	void UpdateShader(
		const D3DXMATRIX& world_mat,
		const D3DXMATRIX& bone_mat
	);

	void SetTexture(
		const std::string& tex_name,
		IDirect3DTexture9*p_tex
	);

	// �V�F�[�_�[�̊J�n
	void ShaderBegin();

	// �V�F�[�_�[�̏I��
	void ShaderEnd();

private:

	// �V�F�[�_�[�̍X�V
	void CommitShader();

	// ���_�V�F�[�_�[�̏�����
	void InitVertexDecl();

	// �V�F�[�_�[�쐬
	bool CreateShader();

	// ���_�t�H�[�}�b�g�̐ݒ�
	void ShaderUpdateDecl();

	// �e�N�j�b�N�̍X�V
	void ShaderUpdateTech(
		const D3DXHANDLE& handle
	);

	// �p�����[�^�[�̐ݒ�A�X�V
	void ShaderUpdateMatrix(
		const D3DXHANDLE& handle,
		const D3DXMATRIX& update_mat
	);

	// �s��z��
	void ShaderUpdateMatrixArray(
		const D3DXHANDLE& handle,
		const D3DXMATRIX* update_mat,
		const int& count
	);

	void ShaderUpdateVector(
		const D3DXHANDLE& handle,
		D3DXVECTOR4& vec4
	);

private:


	// �G�t�F�N�g�V�F�[�_�[
	ID3DXEffect* m_p_effect;
	// �f�N��
	LPDIRECT3DVERTEXDECLARATION9 m_p_decl;
	// �e�N�j�b�N�n���h��
	D3DXHANDLE m_tech_h;
	// �A�j���[�V�����s��n���h��
	D3DXHANDLE m_anim_mat_h;
	// �{�[���s��n���h��
	D3DXHANDLE m_bone_mat_h;
	// �r���[�s��n���h��
	D3DXHANDLE m_view_mat_h;
	// �ˉe�s��n���h��
	D3DXHANDLE m_proj_mat_h;
	// ���[���h�s��n���h��
	D3DXHANDLE m_world_mat_h;
	// �e�N�X�`���n���h��
	D3DXHANDLE m_texture_h;


	// �e�N�j�b�N��
	std::string tech_name;

	// �V�F�[�_�[��
	std::string shader_name;

	// �O���t�B�b�N�X
	Graphics* m_p_graphics;
};