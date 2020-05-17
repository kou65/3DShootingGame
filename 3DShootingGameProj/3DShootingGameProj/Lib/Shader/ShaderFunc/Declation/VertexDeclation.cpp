#include"VertexDeclation.h"





	bool VertexDecl::CreateObjFileDecl() {

		D3DVERTEXELEMENT9 elem[5];

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

		// カラー
		elem[2].Stream = 0;
		elem[2].Offset = 24;
		elem[2].Type = D3DDECLTYPE_D3DCOLOR;
		elem[2].Method = D3DDECLMETHOD_DEFAULT;
		elem[2].Usage = D3DDECLUSAGE_COLOR;
		elem[2].UsageIndex = 0;

		// uv
		elem[3].Stream = 0;
		elem[3].Offset = 28;
		elem[3].Type = D3DDECLTYPE_FLOAT2;
		elem[3].Method = D3DDECLMETHOD_DEFAULT;
		elem[3].Usage = D3DDECLUSAGE_TEXCOORD;
		elem[3].UsageIndex = 0;

		// 終端宣言
		elem[4].Stream = 0xff;
		elem[4].Offset = 0;
		elem[4].Type = D3DDECLTYPE_UNUSED;
		elem[4].Method = 0;
		elem[4].Usage = 0;
		elem[4].UsageIndex = 0;


		if (CreateDeclByDevice(elem) == false) {
			return false;
		}

		return true;
	};


	bool VertexDecl::CreateShapeIndexDecl() {
		D3DVERTEXELEMENT9 elem[5];

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

		// カラー
		elem[2].Stream = 0;
		elem[2].Offset = 24;
		elem[2].Type = D3DDECLTYPE_D3DCOLOR;
		elem[2].Method = D3DDECLMETHOD_DEFAULT;
		elem[2].Usage = D3DDECLUSAGE_COLOR;
		elem[2].UsageIndex = 0;

		// uv
		elem[3].Stream = 0;
		elem[3].Offset = 28;
		elem[3].Type = D3DDECLTYPE_FLOAT2;
		elem[3].Method = D3DDECLMETHOD_DEFAULT;
		elem[3].Usage = D3DDECLUSAGE_TEXCOORD;
		elem[3].UsageIndex = 0;

		// 終端宣言
		elem[4].Stream = 0xff;
		elem[4].Offset = 0;
		elem[4].Type = D3DDECLTYPE_UNUSED;
		elem[4].Method = 0;
		elem[4].Usage = 0;
		elem[4].UsageIndex = 0;


		if (CreateDeclByDevice(elem) == false) {
			return false;
		}

		return true;
	};


	bool VertexDecl::CreateFbxFileDecl() {

		// 頂点要素定義
		D3DVERTEXELEMENT9 elem[7];

		// 位置
		elem[0].Stream = 0;
		elem[0].Offset = 0;
		elem[0].Type = D3DDECLTYPE_FLOAT4;
		elem[0].Method = D3DDECLMETHOD_DEFAULT;
		elem[0].Usage = D3DDECLUSAGE_POSITION;
		elem[0].UsageIndex = 0;

		// 法線
		elem[1].Stream = 0;
		elem[1].Offset = 16;
		elem[1].Type = D3DDECLTYPE_FLOAT3;
		elem[1].Method = D3DDECLMETHOD_DEFAULT;
		elem[1].Usage = D3DDECLUSAGE_NORMAL;
		elem[1].UsageIndex = 0;

		// カラー
		elem[2].Stream = 0;
		elem[2].Offset = 28;
		elem[2].Type = D3DDECLTYPE_D3DCOLOR;
		elem[2].Method = D3DDECLMETHOD_DEFAULT;
		elem[2].Usage = D3DDECLUSAGE_COLOR;
		elem[2].UsageIndex = 0;

		// uv
		elem[3].Stream = 0;
		elem[3].Offset = 32;
		elem[3].Type = D3DDECLTYPE_FLOAT2;
		elem[3].Method = D3DDECLMETHOD_DEFAULT;
		elem[3].Usage = D3DDECLUSAGE_TEXCOORD;
		elem[3].UsageIndex = 0;

		// 重み
		elem[4].Stream = 0;
		elem[4].Offset = 40;
		elem[4].Type = D3DDECLTYPE_FLOAT4;
		elem[4].Method = D3DDECLMETHOD_DEFAULT;
		elem[4].Usage = D3DDECLUSAGE_BLENDWEIGHT;
		elem[4].UsageIndex = 0;

		// 重みインデックス
		elem[5].Stream = 0;
		elem[5].Offset = 56;
		elem[5].Type = D3DDECLTYPE_UBYTE4;
		elem[5].Method = D3DDECLMETHOD_DEFAULT;
		elem[5].Usage = D3DDECLUSAGE_BLENDINDICES;
		elem[5].UsageIndex = 0;

		// 終端宣言
		elem[6].Stream = 0xff;
		elem[6].Offset = 0;
		elem[6].Type = D3DDECLTYPE_UNUSED;
		elem[6].Method = 0;
		elem[6].Usage = 0;
		elem[6].UsageIndex = 0;

		if (CreateDeclByDevice(elem) == false) {
			return false;
		}

		return true;
	}


	bool VertexDecl::CreateDeclByDevice(
		D3DVERTEXELEMENT9*elem) {

		// 頂点データをシェーダー用のデータに変換
		HRESULT hr = Graphics::GetInstance()->
			GetDevice()->
			CreateVertexDeclaration(
				elem,
				&mp_decl
			);

		if (hr != S_OK) {
			hr = S_FALSE;
			return false;
		}

		return true;
	}


	void VertexDecl::Set() {

		if (mp_decl == nullptr) {
			return;
		}

		HRESULT hr = Graphics::GetInstance()->
			GetInstance()->
			GetDevice()->
			SetVertexDeclaration(
				mp_decl
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

		// null代入
		mp_decl = nullptr;

		if (hr != S_OK) {
			return;
		}
	}
