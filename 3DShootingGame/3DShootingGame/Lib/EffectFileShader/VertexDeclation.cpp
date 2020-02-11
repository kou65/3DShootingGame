#include"VertexDeclation.h"




	bool VertexDecl::CreateObjFileDecl() {

		D3DVERTEXELEMENT9 elem[4];

		if (elem == nullptr) {
			return false; 
		}

		// 位置
		elem[0].Stream = 0;
		elem[0].Offset = 0;
		elem[0].Type = D3DDECLTYPE_FLOAT3;
		elem[0].Method = D3DDECLMETHOD_DEFAULT;
		elem[0].Usage = D3DDECLUSAGE_POSITION;
		elem[0].UsageIndex = 0;

		// 法線
		elem[1].Stream = 0;
		elem[1].Offset = 12;
		elem[1].Type = D3DDECLTYPE_FLOAT3;
		elem[1].Method = D3DDECLMETHOD_DEFAULT;
		elem[1].Usage = D3DDECLUSAGE_NORMAL;
		elem[1].UsageIndex = 0;

		// uv
		elem[2].Stream = 0;
		elem[2].Offset = 24;
		elem[2].Type = D3DDECLTYPE_FLOAT2;
		elem[2].Method = D3DDECLMETHOD_DEFAULT;
		elem[2].Usage = D3DDECLUSAGE_TEXCOORD;
		elem[2].UsageIndex = 0;

		// 終端宣言
		elem[3].Stream = 0xff;
		elem[3].Offset = 0;
		elem[3].Type = D3DDECLTYPE_UNUSED;
		elem[3].Method = 0;
		elem[3].Usage = 0;
		elem[3].UsageIndex = 0;


		if (SubmitDevice(elem) == false) {
			return false;
		}

		return true;
	};


	bool VertexDecl::SubmitDevice(
		D3DVERTEXELEMENT9*elem) {

		// 頂点データをシェーダー用のデータに変換
		HRESULT hr = Graphics::GetInstance()->
			GetDevice()->
			CreateVertexDeclaration(
				elem,
				&m_p_decl
			);

		if (hr != S_OK) {
			hr = S_FALSE;
			return false;
		}

		return true;
	}


	void VertexDecl::Set() {

		if (m_p_decl == nullptr) {
			return;
		}

		HRESULT hr = Graphics::GetInstance()->
			GetInstance()->
			GetDevice()->
			SetVertexDeclaration(
				m_p_decl
			);

		if (hr != S_OK) {
			return;
		}
	}

	void VertexDecl::Reset() {


		HRESULT hr = Graphics::GetInstance()->
			GetInstance()->
			GetDevice()->
			SetVertexDeclaration(
				NULL
			);

		if (hr != S_OK) {
			return;
		}
	}



