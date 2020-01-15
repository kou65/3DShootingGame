#pragma once
#include"../../Graphics/Graphics.h"


class ZTexture {
public:

	ZTexture();

	void Create(const char*effect_file_name);


	bool GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight);

	void Begin();

	void BeginPass();

	void End() {};

private:

	// z�l�e�N�X�`��
	IDirect3DTexture9* m_p_tex;

	// z�l�e�N�X�`���T�[�t�F�C�X
	IDirect3DSurface9* m_p_tex_suf;

	// �[�x�o�b�t�@
	IDirect3DSurface9* m_p_depth_buffer;

	// �f�o�C�X�o�b�N�o�b�t�@
	IDirect3DSurface9* m_p_device_buffer;

	// �f�o�C�X�[�x�o�b�t�@
	IDirect3DSurface9* m_p_device_depth;

	// z�v���b�g�G�t�F�N�g
	ID3DXEffect* m_p_effect;

	// ���[���h
	D3DXMATRIX m_mat_world;

	// �r���[
	D3DXMATRIX m_mat_view;

	// �ˉe�ϊ�
	D3DXMATRIX m_mat_proj;

	// ���[���h�ϊ��s��n���h��
	D3DXHANDLE m_world_mat_handle;

	// �r���[�ϊ��s��n���h��
	D3DXHANDLE m_view_mat_handle;

	// �ˉe�ϊ��s��n���h��
	D3DXHANDLE m_proj_mat_handle;

	// �e�N�j�b�N�ւ̃n���h��
	D3DXHANDLE m_technique_handle;

	// �f�o�C�X
	IDirect3DDevice9 *m_p_device;
};