#include"Sound.h"


// サウンド


namespace Sound {


	// サウンドデバイス
	IDirectSound8 *p_direct_sound8 = NULL;

	// サウンドデータを保存するプライマリサウンドバッファリスト
	std::unordered_map<std::string, IDirectSoundBuffer8*>sound_data_list;

	void Play(const std::string name) {

		// 名前が登録されていないなら
		if (IsRegistrName(name.c_str()) == false) {
			return;
		}

		// 音を再生
		sound_data_list[name.c_str()]->Play(0, 0, 0);
	}

	void LoopPlay(const std::string name) {

		// 名前が登録されていないなら
		if (IsRegistrName(name.c_str()) == false) {
			return;
		}

		// ループ再生
		sound_data_list[name]->Play(0, 0, DSBPLAY_LOOPING);
	}


	void Stop(const std::string name) {

		// 名前が登録されていないなら
		if (IsRegistrName(name.c_str()) == false) {
			return;
		}

		sound_data_list[name.c_str()]->Stop();
	}


	void Replay(const std::string name) {

		// 名前が登録されていないなら
		if (IsRegistrName(name.c_str()) == false) {
			return;
		}

		sound_data_list[name.c_str()]->SetCurrentPosition(0);
	}


	void SetVolume(const std::string name, int volume) {

		// 名前が登録されていないなら
		if (IsRegistrName(name.c_str()) == false) {
			return;
		}

		sound_data_list[name.c_str()]->SetVolume(volume);
	}


	void Load(const std::string file_name, const std::string registr_name) {

		// すでに名前が登録されているなら
		if (IsRegistrName(file_name.c_str()) == true) {
			return;
		}

		// WAVフォーマットに含まれる音質やチャンネル数の情報を格納する構造体
		WAVEFORMATEX wave_format_ex;

		// WAVEデータの大きさ
		DWORD wave_size;

		// 波形データ
		char*p_wave_data;

		// WAVEファイル読み込み
		LoadWaveData(file_name.c_str(), &p_wave_data, wave_size, wave_format_ex);

		// セカンダリバッファ作成
		CreateSecondaryBuffer(registr_name, p_wave_data, wave_size, wave_format_ex);
	}


	bool Init() {

		HRESULT error;

		// COMの初期化
		CoInitialize(NULL);

		// DirectSound8を作成
		error = DirectSoundCreate8(
			// ハードウェアのGUID(NULLでデフォルトのサウンドデバイス)
			NULL,
			// サウンドのポインタ
			&p_direct_sound8,
			// NULLを指定
			NULL);

		if (FAILED(error)) {
			MessageBoxA(NULL, "サウンド生成失敗", NULL, NULL);
			return false;
		}

		// 協調レベル設定(Windowsと他のアプリとの整合性を取る)
		error = p_direct_sound8->SetCooperativeLevel(
			// ウィンドウハンドル
			Window::GetWindowHandle(),
			// フォーカスが移ったら鳴らさない
			DSSCL_EXCLUSIVE
			// そしてプライマリバッファのフォーマット指定
			| DSSCL_PRIORITY
		);

		if (FAILED(error)) {
			MessageBoxA(NULL, "強調レベル設定失敗", NULL, NULL);
			return false;
		}

		// 初期化に成功
		return true;
	}


	void Release() {

		// サウンドバッファ解放
		for (auto sound : sound_data_list) {
			sound.second->Release();
		}

		// ダイレクトサウンド解放
		p_direct_sound8->Release();
	}


	bool CreateSecondaryBuffer(
		const std::string registr_name,
		char*p_wave_data,
		DWORD wave_size,
		WAVEFORMATEX &wFmt
	) {

		HRESULT error;

		// プライマリサウンドバッファの作成(音の貯蔵庫,メモリ)
		DSBUFFERDESC dsdesc;

		// 変数の中身を0初期化
		ZeroMemory(&dsdesc, sizeof(DSBUFFERDESC));

		// 新しいバッファオブジェクトの特性を表す

		// 構造体のサイズ
		dsdesc.dwSize = sizeof(DSBUFFERDESC);
		// デバイスの付加能力、指定しない場合は0
		dsdesc.dwFlags = 0;
		// セカンダリバッファのサイズ(波形データのサイズを入れる)
		dsdesc.dwBufferBytes = wave_size;
		// 予約域で0にする
		dsdesc.dwReserved = 0;
		// WAVのフォーマットをWAVEFORATEX構造体で指定
		dsdesc.lpwfxFormat = &wFmt;
		// 仮想3DエフェクトのGUIDを設定。エフェクトを使用しない場合はGUID_NUMMにする
		dsdesc.guid3DAlgorithm = GUID_NULL;


		IDirectSoundBuffer * p_tmp_buffer = 0;

		// セカンダリバッファ生成
		error = p_direct_sound8->CreateSoundBuffer(
			&dsdesc,
			&p_tmp_buffer,
			NULL
		);

		// エラー
		if (FAILED(error)) {
			MessageBoxA(NULL, "プライマリサウンドバッファ作成失敗\n", NULL, NULL);
			return false;
		}

		// バッファのポインタ生成
		IDirectSoundBuffer8 *p_ds_buffer8;

		// サウンド8のインターフェース受け取り
		p_tmp_buffer->QueryInterface(
			IID_IDirectSoundBuffer8,
			(void**)&p_ds_buffer8
		);

		// バッファがない場合は解放処理
		if (p_ds_buffer8 == 0) {
			p_ds_buffer8->Release();
			return false;
		}

		// 解放
		p_tmp_buffer->Release();

		// メモリのロックとロック解除
		WaveDataLockAndUnlock(p_ds_buffer8,p_wave_data);

		// サウンドデータに登録
		sound_data_list[registr_name.c_str()] = p_ds_buffer8;

		// 成功
		return true;
	}


	void WaveDataLockAndUnlock(
		IDirectSoundBuffer8 *idirect_sound_buffer8,
		const char* p_wave_data
	) {

		// セカンダリバッファにWaveデータ書き込み
		LPVOID p_write = 0;
		DWORD dw_length = 0;

		// メモリを閉じる
		if (DS_OK == idirect_sound_buffer8->Lock(
			// ロックするオフセット位置
			0,
			// ロックするメモリの大きさ(バイト単位)
			0,
			// ロックした位置のポインタを返す
			&p_write,
			// ロックした大きさ
			&dw_length,
			// 循環バッファなので最初と最後をNULLにする
			NULL,
			// ことでメモリがまたがれることはなくなる
			NULL,
			// ロックの方法を指定
			DSBLOCK_ENTIREBUFFER
		)) {

			// 書き込み用にWAVEデータをメモリコピー
			memcpy(p_write, p_wave_data, dw_length);

			// ロック解除
			idirect_sound_buffer8->Unlock(
				p_write,
				dw_length,
				NULL,
				0
			);

			// 音データはすでにプライマリにセットしたのでいらない
			delete[] p_wave_data;
		}
	}


	bool LoadWaveData(
		const std::string file_name,
		char**p_wave_data,
		DWORD &wave_size,
		WAVEFORMATEX &wave_format_ex
	) {

		HMMIO hMmio = NULL;

		MMIOINFO mmio_info;

		// 渡す場合は0初期化
		memset(&mmio_info, 0, sizeof(MMIOINFO));

		// Waveファイルオープン
		hMmio = mmioOpen(
			(LPSTR)file_name.c_str(),
			&mmio_info,
			MMIO_READ);

		if (!hMmio) {
			// ファイルオープン失敗
			return false;
		}

		// 各フォーマット
		MMRESULT mmRes;
		MMCKINFO riff_chunk;

		// RIFFチャンクサーチ(ファイルの種類)
		{
			// 4文字の文字列でチャンクの種類を区別している
			riff_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');

			// 親から子チャンクへ降りる
			mmRes = mmioDescend(
				// オープンしたHMMIOハンドル 
				hMmio,
				// チャンクの情報を格納するMMCKINFO構造体のポインタ
				&riff_chunk,
				// 第二引数の親チャンクのチャンク情報を与える(探すだけなのでNULL)
				NULL,
				// 何をするか決めるフラグ(MMIO_FINDRIFFでRIFF情報を探す)
				MMIO_FINDRIFF
			);

			// エラーの場合
			if (mmRes != MMSYSERR_NOERROR) {
				mmioClose(hMmio, 0);
				return false;
			}
		}

		MMCKINFO format_chunk;

		// フォーマットチャンクサーチ
		{
			// fmtのチャンク形式で検索
			format_chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');

			mmRes = mmioDescend(
				// オープンしたHMMIOハンドル
				hMmio,
				// チャンクの情報を格納するMMCKINFO構造体へのポインタを渡す
				&format_chunk,
				// 上の親チャンク情報を与える
				&riff_chunk,
				// チャンクを探す
				MMIO_FINDCHUNK
			);

			// エラーの場合
			if (mmRes != MMSYSERR_NOERROR) {
				mmioClose(hMmio, 0);
				return false;
			}
		}

		// WAVEFORMATEX構造体格納
		DWORD fmsize = format_chunk.cksize;

		// 読み込んだサイズが返る
		DWORD size = mmioRead(
			// HMMIO型を指定
			hMmio,
			// WAVEFORMATEX構造体のポインタを渡す
			(HPSTR)&wave_format_ex,
			// データサイズを指定
			fmsize
		);

		// エラーの場合
		if (size != fmsize) {
			mmioClose(hMmio, 0);
			return false;
		}

		// WAVEFORMATEX構造体格納
		mmioAscend(hMmio, &format_chunk, 0);

		// データチャンク格納変数
		MMCKINFO data_chunk;

		// データチャンク検索
		{
			// dataのチャンク形式で検索
			data_chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');

			mmRes = mmioDescend(
				// オープンしたHMMIOハンドル
				hMmio,
				// チャンクの情報を格納するMMCKINFO構造体へのポインタを渡す
				&data_chunk,
				// 上の親チャンク情報を与える
				&riff_chunk,
				// チャンクを探す
				MMIO_FINDCHUNK
			);

			// エラー
			if (mmRes != MMSYSERR_NOERROR) {
				mmioClose(hMmio, 0);
				return false;
			}
		}

		// 音データ格納
		*p_wave_data = new char[data_chunk.cksize];

		// データサイズを受け取り
		size = mmioRead(hMmio, (HPSTR)*p_wave_data, data_chunk.cksize);

		// サイズが違う場合
		if (size != data_chunk.cksize) {
			delete[] * p_wave_data;
			return false;
		}

		// サイズを渡す
		wave_size = size;

		// ハンドルを閉じる
		mmioClose(hMmio, 0);

		return true;
	}


	bool IsRegistrName(const std::string name) {

		auto sound = sound_data_list.find(name.c_str());

		// 登録されている場合
		if (sound != sound_data_list.end()) {

			return true;
		}
		// 登録されていない場合
		else {
			return false;
		}
		return false;
	}

	// TODO まだできていません
	bool SetWaveFormat(IDirectSoundBuffer8*p_ds_buffer8, WAVEFORMATEX&wave_format) {

		HRESULT ret;

		// ウェーブフォームオーディオフォーマットのタイプを指定
		wave_format.wFormatTag = WAVE_FORMAT_PCM;
		// ステレオ・モノラルの選択,ステレオならば左右のスピーカーから独立した音がなるのでチャンネル数が2
		wave_format.nChannels = 2;
		// サンプリングレート(音質)をHz単位で指定(CDが再生できるなら44100Hzを扱える)
		wave_format.nSamplesPerSec = 44100;
		// 平均データ転送速度(wFormatTagにWAVE_FORMAT_PCM(非圧縮波形)がされている場合)
		wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;
		// wFormatTagで指定したフォーマット形式の最小単位(バイト単位)を指定
		wave_format.nBlockAlign = wave_format.nChannels * wave_format.wBitsPerSample / 8;
		// サンプリングビットを指定(8,16,32など数が大きいほどクリアな音質になる)
		wave_format.wBitsPerSample = (WORD)(wave_format.nSamplesPerSec * wave_format.nBlockAlign);
		// 付加的な情報がある場合にそのサイズを指定
		wave_format.cbSize = sizeof(WAVEFORMATEX);

		// プライマリサウンドバッファに対してSetFormatメソッドを実行
		ret = p_ds_buffer8->SetFormat(&wave_format);


		if (FAILED(ret)) {
			MessageBoxA(NULL, "プライマリバッファのステータス失敗", NULL, NULL);
			return false;
		}

		return true;
	}

}