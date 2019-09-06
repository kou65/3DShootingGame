#include"VertexBuffer.h"




VertexBuffer::VertexBuffer(int vertex_num) {

	// グラフィック代入
	this->graphics = Graphics::GetInstance();

	// 頂点バッファ生成
	Create(vertex_num,NONE);
}


VertexBuffer::VertexBuffer(Shape shape) {

	// グラフィック代入
	this->graphics = Graphics::GetInstance();

	// 正方形頂点バッファ作成
	Create(8, shape);
}


void VertexBuffer::Create(int vertex_num,Shape shape) {
	
	// 頂点バッファ作成
	graphics->GetLpDirect3DDevice9()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		sizeof(Object3DCustomVertex) * vertex_num,
		// リソースの使用法
		D3DUSAGE_WRITEONLY,
		// 柔軟な頂点フォーマットの型を指定する
		D3DFMT_INDEX16,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&m_p_vertex_buffer9,
		// phandleは現在使用されていない
		NULL
	);

	// 頂点数分用意する
	Object3DCustomVertex * v;

	m_p_vertex_buffer9->Lock(0, 0, (void**)&v, 0);

	// 形状ごとに頂点を作成する
	if (shape == BOX) {

		Cube(v);
	}
	// 独自の頂点情報を作成する 
	else {

		
	}

	m_p_vertex_buffer9->Unlock();
}



void VertexBuffer::Draw() {

	if (m_p_vertex_buffer9 == nullptr) {
		return;
	}
	
	// テクスチャセット
	//graphics->GetLpDirect3DDevice9()->SetTexture()


	// 頂点処理の流れに頂点バッファを実際にセットできる
	graphics->GetLpDirect3DDevice9()->SetStreamSource(
		0,
		m_p_vertex_buffer9,
		0,
		sizeof(Object3DCustomVertex)
	);

	graphics->GetLpDirect3DDevice9()->SetFVF(FVF_CUSTOM);

	graphics->GetLpDirect3DDevice9()->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2
	);
}


void VertexBuffer::Cube(Object3DCustomVertex v[8]) {


	// 正方形

	// 手前
	{
		// 左上(原点)
		v[0].vertex = D3DXVECTOR3(0, 0, 0);

		// 左下
		v[1].vertex = D3DXVECTOR3(0, -1, 0);

		// 右上
		v[2].vertex = D3DXVECTOR3(1, 0, 0);

		// 右下
		v[3].vertex = D3DXVECTOR3(1, -1, 0);
	}

	// 奥
	{
		// 左上
		v[4].vertex = D3DXVECTOR3(0, 0, 1);

		// 左下
		v[5].vertex = D3DXVECTOR3(0, -1, 1);

		// 右上
		v[6].vertex = D3DXVECTOR3(1, 0, 1);

		// 右下
		v[7].vertex = D3DXVECTOR3(1, -1, 1);
	}
}

/*
bool VertexBuffer::Load() {


	const int VERTEX_NUM = 4;

	// 頂点情報埋め込み
	//CustomVertex v[VERTEX_NUM] =
	//{
	//{ 1.f, 0.f, 0.f,1.f,0xffffff,},// 0.f, 0.f },
	//{ 1.f, 1.f, 0.f,1.f,0xffffff,},// 0.f, 1.f },
	//{ 0.f, 0.f, 0.f,1.f,0xffffff,},// 1.f, 0.f },
	//{ 0.f, 1.f, 0.f,1.f,0xffffff,}// 1.f, 1.f }
	//};

	// リザルト変数
	HRESULT result;

	// 頂点バッファ作成
	result = graphics->GetLpDirect3DDevice9()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		sizeof(CustomVertex) * VERTEX_NUM,
		// リソースの使用法
		D3DUSAGE_WRITEONLY,
		// 柔軟な頂点フォーマットの型を指定する
		FVF_CUSTOM,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&m_p_vertex_buffer9,
		// phandleは現在使用されていない
		NULL
	);

	if (result != D3D_OK) {
		return false;
	}

	// バッファデータ
	void *p_data;

	// バッファデータをロック
	result = m_p_vertex_buffer9->Lock(
		0,
		sizeof(CustomVertex) * VERTEX_NUM,
		(void**)&p_data,
		0
	);

	// 成功していなかったら失敗を返す
	if (result != D3D_OK) {
		return false;
	}

	// メモリコピー
	//memcpy(
	//	p_data,
	//	v,
	//	sizeof(CustomVertex) * VERTEX_NUM
	//);

	// ロック解除して使えるようにする
	m_p_vertex_buffer9->Unlock();

	return true;
}
*/