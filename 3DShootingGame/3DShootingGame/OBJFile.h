#pragma once



class OBJFile {
public:

	static OBJFile *GetInstance() {
		static OBJFile obj_file;
		return &obj_file;
	}

};