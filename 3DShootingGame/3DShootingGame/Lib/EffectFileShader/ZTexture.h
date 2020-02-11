#pragma once
#include"../Graphics/Graphics.h"
#include"../EffectFileShader/EffectFileBase.h"


// Z�l�������Ōv�Z����Z�e�N�X�`�����쐬����N���X


class ZTexture{
public:

	ZTexture();

	bool Init(
		const char*effect_file_name,
		UINT z_tex_width,
		UINT z_tex_height,
		D3DFORMAT z_tex_format = D3DFMT_A8B8G8R8
	);

	bool GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight);

	void SetWorldMatrix(D3DXMATRIX*mat);

	void SetViewMatrix(D3DXMATRIX*mat);

	void SetProjMatrix(D3DXMATRIX*mat);

	bool SetParameter();

	void Begin();

	void BeginPass();

	void End();

	void EndPass();

	IDirect3DTexture9* GetZTexture();

private:

	// �G�t�F�N�g
	ID3DXEffect*m_p_effect;

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

	// �X�^�[�g�p�X��
	int m_start_pass_num;

	// �n���h��
	D3DXHANDLE m_h_world_mat;
	D3DXHANDLE m_h_view_mat;
	D3DXHANDLE m_h_proj_mat;
	D3DXHANDLE m_h_tech;

	// �s��
	D3DXMATRIX m_world_mat;
	D3DXMATRIX m_view_mat;
	D3DXMATRIX m_proj_mat;

	// �f�o�C�X
	IDirect3DDevice9 *m_p_device;
};