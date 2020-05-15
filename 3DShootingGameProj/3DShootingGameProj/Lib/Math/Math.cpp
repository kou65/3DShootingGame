#include"Math.h"


namespace Utility {

	namespace Math {



		void OutTransformMatrix(
			D3DXMATRIX&total_mat,
			D3DXVECTOR3&pos,
			D3DXVECTOR3&scale,
			D3DXVECTOR3&rota
		) {


			// ワールド座標初期化
			D3DXMATRIX rota_mat, scale_mat, move_mat;
			D3DXMatrixIdentity(&total_mat);

			D3DXMATRIX rota_x, rota_y, rota_z;

			// 回転
			D3DXMatrixRotationZ(&rota_z, D3DXToRadian(rota.z));
			D3DXMatrixRotationX(&rota_x, D3DXToRadian(rota.x));
			D3DXMatrixRotationY(&rota_y, D3DXToRadian(rota.y));

			// 回転行列作成
			rota_mat = (rota_z * rota_x * rota_y);

			// 拡縮
			D3DXMatrixScaling(
				&scale_mat, scale.x, scale.y, scale.z);

			// 移動
			D3DXMatrixTranslation(
				&move_mat, pos.x, pos.y, pos.z);

			total_mat = (scale_mat * rota_mat * move_mat);
		}


		D3DXMATRIX GetTransformMatrix(
			D3DXVECTOR3&pos,
			D3DXVECTOR3&scale,
			D3DXVECTOR3&rota
		) {
			D3DXMATRIX total_mat;

			OutTransformMatrix(
				total_mat,
				pos,
				scale,
				rota
			);

			return total_mat;
		}

		// x
		// y
		// z
		// 1

		// D3DMATRIX
		// _11, _12, _13, _14
		// _21, _22, _23, _24
		// _31, _32, _33, _34
		// _41, _42, _43, _44

		// x = (x * m._11) + (y * m._21) + (z * m._31) + (1 * m._41)
		// y = (x * m._12) + (y * m._22) + (z * m._32) + (1 * m._42)
		// z = (x * m._13) + (y * m._23) + (z * m._33) + (1 * m._43)

		void MatrixMultipleVec3(
			D3DXVECTOR3&out_vec3,
			const D3DXVECTOR3&base_vec3,
			const D3DXMATRIX&mat
		) {
			// 頂点x * 1行1列番目の行列...
			out_vec3.x
				=
				(
					base_vec3.x * mat._11
					+ base_vec3.y * mat._21
					+ base_vec3.z * mat._31
					+ 1 * mat._41);

			out_vec3.y
				=
				(
					base_vec3.x * mat._12
					+ base_vec3.y * mat._22
					+ base_vec3.z * mat._32
					+ 1 * mat._42);

			out_vec3.z
				= (base_vec3.x * mat._13
					+ base_vec3.y * mat._23
					+ base_vec3.z * mat._33
					+ 1 * mat._43);

		}



		void MatrixMultipleVec4(
			D3DXVECTOR4&out_vec3,
			const D3DXVECTOR4&base_vec3,
			const D3DXMATRIX&mat
		) {
			// 頂点x * 1行1列番目の行列...
			out_vec3.x
				=
				(
					base_vec3.x * mat._11
					+ base_vec3.y * mat._21
					+ base_vec3.z * mat._31
					+ 1 * mat._41);

			out_vec3.y
				=
				(
					base_vec3.x * mat._12
					+ base_vec3.y * mat._22
					+ base_vec3.z * mat._32
					+ 1 * mat._42);

			out_vec3.z
				= (base_vec3.x * mat._13
					+ base_vec3.y * mat._23
					+ base_vec3.z * mat._33
					+ 1 * mat._43);

			out_vec3.w = 1;
		}

		float GetLength(const Vec3&dir) {
			// 三平方の定理で距離を出す
			return(sqrtf((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z)));
		}


		Vec3 GetVec1ToVec2(const Vec3&vec1, const Vec3&vec2) {

			Vec3 out_vec;
			// 方向割り出し
			out_vec.x = vec1.x - vec2.x;
			out_vec.y = vec1.y - vec2.y;
			out_vec.z = vec1.z - vec2.z;

			return out_vec;
		}

	}
}