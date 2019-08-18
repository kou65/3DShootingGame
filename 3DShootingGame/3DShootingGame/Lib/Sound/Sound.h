#pragma once
#include<dsound.h>
#include"../Window/Window.h"
#include<string>
#include<mmsystem.h>
#include<unordered_map>


#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")


namespace Sound {


	/**
	* @brief サウンドの初期化
	* @return bool 成功すればtrue,失敗すればfalseを返す
	*/
	bool Init();


	/**
	* @brief サウンドの解放
	*/
	void Release();


	/**
	* @brief サウンドの再生
	* @param[in] name サウンド登録時の名前を入れて再生
	*/
	void Play(const std::string name);


	/**
	* @brief ループ付き再生
	* @param[in] name サウンド登録時の名前を入れて再生
	*/
	void LoopPlay(const std::string name);


	/**
	* @brief サウンドの再生を停止
	* @param[in] name サウンド登録時の名前を入れて停止
	*/
	void Stop(const std::string name);


	/**
	* @brief サウンドを最初から再生する
	* @param[in] name サウンド登録時の名前を入れて最初から再生
	*/
	void Replay(const std::string name);


	/**
	* @brief サウンドのボリュームを変更する
	* @param[in] name サウンド登録時の名前を入れてボリューム変更
	*/
	void SetVolume(const std::string name, int volume);


	/**
	* @brief サウンドの読み込み
	* @param[in] file_name WAVEファイル名を入れる
	* @param[in] registr_name 登録時の名前
	*/
	void Load(const std::string file_name,const std::string registr_name);


	/**
	* @brief 名前が登録されているかどうか
	*/
	bool IsRegistrName(const std::string name);

	bool CreateSecondaryBuffer(const std::string registr_name,char*p_wave_data, DWORD second_buffer,WAVEFORMATEX &wFmt);

	void WaveDataLockAndUnlock(IDirectSoundBuffer8 *idirect_sound_buffer8, const char* p_wave_data);

	bool LoadWaveData(const std::string file_name, char**p_wave_data, DWORD &wave_size,WAVEFORMATEX &wave_format_ex);
}