#pragma once
#include"Lib/D3D9/D3D9.h"


struct CustormVertex {
	D3DXVECTOR3 vertex;
};


class OBJFile {
public:

	static OBJFile *GetInstance() {
		static OBJFile obj_file;
		return &obj_file;
	}


	void Load(const std::string &file_name);
};