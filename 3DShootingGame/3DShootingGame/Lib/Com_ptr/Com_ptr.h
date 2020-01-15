#pragma once


// Com_ptrクラス

// DirectXのインターフェイスの参照カウンタやコピーなどの問題を解決する。
// デバッグ時にはインターフェイスの使用状況をDEBUG.txtというファイル名で
// 外部に出力します。
// デバッグ時にこの機能をOFFにするには下の
// OUTPUT_INTERFACEUSAGE定数をコメントアウトしてください。

#define OUTPUT_INTERFACEUSAGE

#ifdef OUTPUT_INTERFACEUSAGE
#if _DEBUG
// デバッグ時にファイル出力
#include <fstream>
using namespace std;
#endif
#endif



template <class T>
class Com_ptr
{
private:
	T* m_pInterface;   // インターフェイスへのポインタ

protected:
	// 参照カウンタ増加
	void AddRef(T* pInterface) {
#if _DEBUG
		// デバッグ版では参照カウントを出力する
		ULONG c = pInterface->AddRef();
		ofstream ofs;
		ofs.open("DEBUG.txt", ios::app);
		ofs << pInterface << "\t[" << c << "]" << endl;
		ofs.close();
#else
		pInterface->AddRef();
#endif
	}

	// 参照カウンタ減少
	void Release(T* pInterface) {
#if _DEBUG
		ULONG c = m_pInterface->Release();
		ofstream ofs;
		ofs.open("DEBUG.txt", ios::app);
		ofs << m_pInterface << "\t[" << c << "]" << endl;
		ofs.close();
#else
		m_pInterface->Release();
#endif
	}

public:
	// デフォルトコンストラクタ
	// コンストラクタはexplicit宣言します。
	// よってfunc(Com_ptr<IHoge> v)という関数に対して、
	// func(pIHoge)のような暗黙型変換代入は出来ません。
	// 通常、ポインタの引渡し時にすでに参照カウンタは
	// インクリメントされているので、AddRefは行いません。
	explicit Com_ptr(T* pInterface = NULL, BOOL add = FALSE) {
		if (pInterface && add) {
			AddRef(pInterface);
		}
		m_pInterface = pInterface;
	}

	// コピーコンストラクタ
	Com_ptr(const Com_ptr& src) {
		// コピー元の参照カウンタを1つ増やす
		if (src.m_pInterface)
			AddRef(src.m_pInterface);

		// コピー
		m_pInterface = src.m_pInterface;
	}

	// デストラクタ
	virtual ~Com_ptr() {
		if (m_pInterface)
			Release(m_pInterface);
	}

	// 演算子
	// =代入演算子（明示的コピー）
	Com_ptr& operator =(const Com_ptr& src) {
		// 自分のインターフェイスの参照カウンタを1つ減らす
		if (m_pInterface)
			Release(m_pInterface);

		// コピー元の参照カウンタを1つ増やす
		if (src.m_pInterface)
			AddRef(src.m_pInterface);

		// コピー
		m_pInterface = src.m_pInterface;

		return *this;
	}

	// =代入演算子（インターフェイス登録）
	void operator =(T* pInterface) {
		// 自分のインターフェイスの参照カウンタを1つ減らす
		if (m_pInterface)
			Release(m_pInterface);

		// コピー
		m_pInterface = pInterface;
	}

	// !=比較演算子
	BOOL operator !=(int val) {
		if (val != (int)m_pInterface)
			return TRUE;
		return FALSE;
	}

	// ==比較演算子
	BOOL operator ==(int val) {
		if (val == (int)m_pInterface)
			return TRUE;
		return FALSE;
	}

	// メンバ関数
	   // ポインタ取得
	T* GetPtr() { return m_pInterface; }

	// ポインタのアドレスを取得
	T** GetPtrPtr() { return &m_pInterface; }

	// インターフェイス生成関数へ渡す専用関数
	T** ToCreator() {
		if (m_pInterface)
			m_pInterface->Release();

		return &m_pInterface;
	}
};