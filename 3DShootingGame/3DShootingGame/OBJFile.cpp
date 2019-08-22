#include"OBJFile.h"
#include<iostream>



void OBJFile::Load(const std::string &file_name) {

	FILE * p_file;

	D3DXVECTOR3 vertex;

	fopen_s(&p_file, file_name.c_str(), "r");

	if (p_file == nullptr) {
		return;
	}

	// 先頭の情報を取得する
	fscanf_s(,)

	fclose(p_file);
}