#pragma once
#include"../Graphics/Graphics.h"



class VertexDecl {
public:

	// objファイル送信頂点作成
	bool CreateObjFileDecl();

	// デバイスにセットする
	void Set();

	// 初期状態に戻す
	void Reset();

	operator IDirect3DVertexDeclaration9*()const {
		return m_p_decl;
	};

private:

	// デバイスに送信
	bool SubmitDevice(
		D3DVERTEXELEMENT9*elem
	);

private:

	IDirect3DVertexDeclaration9*m_p_decl;
};