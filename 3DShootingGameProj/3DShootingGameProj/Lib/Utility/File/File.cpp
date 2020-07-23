#include"File.h"



namespace Utility {

	namespace File {

		bool Open(
			FILE**p_fp,
			const std::string&load_file_name,
			const char*mode
		) {

			// ファイルを開く
			fopen_s(&*p_fp, load_file_name.c_str(), mode);

			if (p_fp == NULL) {
				return false;
			}

			return true;
		}


		void Close(FILE**p_file) {

			if (p_file == nullptr) {
				return;
			}

			// ファイルを閉じる
			fclose(*p_file);
		}


		void LoadFloatVec3(FILE*p_fp, Vec3&out_vec3, char break_str) {

			std::string str;

			// 3つの%fを作る
			for (int i = 0; i < 2; i++) {
				str += "%f";
				str += break_str;
			}
			// 最後の%f
			str += "%f";

			// x軸,y軸,z軸を取得
			fscanf_s(
				p_fp, str.c_str(),
				&out_vec3.x,
				&out_vec3.y,
				&out_vec3.z);
		}


		bool LoadIntNum(
			FILE*p_file,
			std::vector<int>&out_num_list,
			const int&load_total_num,
			const char *break_str
		) {

			std::string str;

			// 文字列連結
			str += "%d";
			str += break_str;

			int num = -1;
			bool is_load = true;
			int ret_num = -1;

			for (int i = 0; i < load_total_num; i++) {

				// 値(intかfloatか)を取得
				ret_num = fscanf_s(
					p_file,
					str.c_str(),
					&num
				);

				// 加える
				out_num_list.push_back(num);
			}

			// 正式に読み込めてなかったら
			if (ret_num == 0) {
				is_load = false;
			}

			return is_load;
		}
	}
}