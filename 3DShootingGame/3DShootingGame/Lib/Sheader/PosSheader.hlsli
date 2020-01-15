
// �v���O���}�u���V�F�[�_�[�̍쐬

// ���[���h�r���[���W�ϊ��s��
float4x4 mat_world_view_proj;

struct VS_OUTPUT {

	// �ˉe�ϊ����W(�ʒu�L���ɂ���)
	float4 pos : POSITION;

	// Z�o�b�t�@�e�N�X�`��
	float4 ShadowMapTex : TEXCOORDO;
};


VS_OUTPUT ZBufferCalc_Vs(float4 Pos : POSITION) {

	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ���`���
	// ���ʂɃ��[���h�r���[�ˉe�s�������(���z��w���v�Z�����)
	Out.Pos = mul(Pos, mat_world_view_proj);

	// �e�N�X�`���Ɉʒu��o�^
	Out.ShadowMapTex = Out.Pos;

	return Out;
}