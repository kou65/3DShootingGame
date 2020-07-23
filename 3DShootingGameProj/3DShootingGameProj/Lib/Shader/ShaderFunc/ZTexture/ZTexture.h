#pragma once
#include"../../../Graphics/Graphics.h"
#include"../StandardTransformShader/StandardTransformShader.h"
#include"../../MultRenderTarget/MultRenderTarget.h"

// Z�l�������Ōv�Z����Z�e�N�X�`�����쐬����N���X


/**
* @brief z�e�N�X�`��
*/
class ZTexture : public StandardTSShader{
public:

	// �R���X�g���N�^
	ZTexture(
		const VertexDecl::Type &type = VertexDecl::Type::OBJ
	);

	~ZTexture();

	// ������
	void Init();

	// �X�V
	void Update()override;

	void Begin(
		UINT &total_pass_num,
		const DWORD &device_state_num)override;

	void End()override;

	// ������
	bool InitZTexture();

	// �e�N�X�`���𑜓x�T�C�Y
	void SetTextureSize(
		UINT z_tex_width,
		UINT z_tex_height
	);

	// �e�N�X�`���̃|�C���^��Ԃ�
	IDirect3DTexture9* GetZTexture();

private:


	void InitHandle();

	// �e�N�X�`���쐬
	bool CreateTexture(
		UINT z_tex_width,
		UINT z_tex_height,
		D3DFORMAT z_tex_format
	);

	// �T�[�t�F�C�X�T�C�Y�擾
	bool GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight);

private:

	// �𑜓x
	UINT m_z_tex_width;
	UINT m_z_tex_height;

	// �e�N�X�`���t�H�[�}�b�g
	D3DFORMAT m_z_tex_format;

	// z�l�e�N�X�`��
	IDirect3DTexture9* mp_tex;

	// z�l�e�N�X�`���T�[�t�F�C�X
	IDirect3DSurface9* mp_tex_suf;

	// �[�x�o�b�t�@
	IDirect3DSurface9* mp_depth_buffer;

	// �f�o�C�X�o�b�N�o�b�t�@
	IDirect3DSurface9* mp_device_buffer;

	// �f�o�C�X�[�x�o�b�t�@
	IDirect3DSurface9* mp_depth;

	// �X�^�[�g�p�X��
	int m_start_pass_num;

	// �f�o�C�X
	IDirect3DDevice9 *mp_device;

};