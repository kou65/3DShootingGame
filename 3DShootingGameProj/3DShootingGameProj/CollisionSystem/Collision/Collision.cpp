#include"Collision.h"


namespace Collision {

	bool IsHitSphereAndSphere(
		Vec3 v1,
		Vec3 v2,
		float rad_1,
		float rad_2
	) {

		float a;
		float b;
		float c;
		float d;

		a = v1.x - v2.x;
		b = v1.y - v2.y;
		c = v1.z - v2.z;
		d = sqrtf(a * a + b * b + c * c);

		if (d <= sqrtf(rad_1 + rad_2)) {
			return true;
		}
		return false;

	}


	bool IsHitRayAndSphere(
		float lx, float ly, float lz,
		float vx, float vy, float vz,
		float px, float py, float pz,
		float r,
		float &q1x, float &q1y, float &q1z,
		float &q2x, float &q2y, float &q2z
	) {
		px = px - lx;
		py = py - ly;
		pz = pz - lz;

		float A = vx * vx + vy * vy + vz * vz;
		float B = vx * px + vy * py + vz * pz;
		float C = px * px + py * py + pz * pz - r * r;

		if (A == 0.0f) {
			return false; // レイの長さが0
		}
		float s = B * B - A * C;
		if (s < 0.0f) {
			return false; // 衝突していない
		}
		s = sqrtf(s);
		float a1 = (B - s) / A;
		float a2 = (B + s) / A;

		if (a1 < 0.0f || a2 < 0.0f) {
			return false; // レイの反対で衝突
		}
		q1x = lx + a1 * vx;
		q1y = ly + a1 * vy;
		q1z = lz + a1 * vz;
		q2x = lx + a2 * vx;
		q2y = ly + a2 * vy;
		q2z = lz + a2 * vz;

		return true;
	}


	bool IsHitCubeAndCube(
		const Rect3D&rect1,
		const Rect3D&rect2
	) {

		// 中心から測って

		// 左右
		float r1_left_p = rect1.x - rect1.width / 2;
		float r1_right_p = rect1.x + rect1.width / 2;

		// 縦
		float r1_top_p = rect1.y + rect1.height / 2;
		float r1_bottom_p = rect1.y - rect1.height / 2;

		// 奥
		float r1_front_p = rect1.z + rect1.depth / 2;
		float r1_back_p = rect1.z - rect1.depth / 2;

		// r2

		// 左右
		float r2_left_p = rect2.x - rect2.width / 2;
		float r2_right_p = rect2.x + rect2.width / 2;

		// 縦
		float r2_top_p = rect2.y + rect2.height / 2;
		float r2_bottom_p = rect2.y - rect2.height / 2;

		// 奥
		float r2_front_p = rect2.z + rect2.depth / 2;
		float r2_back_p = rect2.z - rect2.depth / 2;


		bool is_hit_x = false;
		bool is_hit_y = false;
		bool is_hit_z = false;

		// 左辺に入っているか
		if (r1_left_p <= r2_left_p) {

			// 右辺に入っているか
			if (r2_right_p <= r1_right_p) {
				is_hit_x = true;
			}
		}

		// 下辺に入っているか
		if (r1_bottom_p <= r2_bottom_p) {

			// 上辺に入っているか
			if (r2_top_p <= r1_top_p) {
				is_hit_y = true;
			}
		}

		// 奥辺に入っているか
		if (r1_front_p <= r2_front_p) {

			// 後ろの辺に入っているか
			if (r2_back_p <= r1_back_p) {
				is_hit_z = true;
			}
		}

		// 全て入っているなら衝突
		if (is_hit_x == true && is_hit_y == true && is_hit_z == true) {
			return true;
		}

		return false;
	};



	bool IsHitRayAndPoint(
		Vec3&sv,
		Vec3&ev,
		Vec3&p
	) {

		// 線分の方向ベクトル
		Vec3 dir_v;

		// 始点と終点から方向算出
		dir_v.x = ev.x - sv.x;
		dir_v.y = ev.y - sv.y;
		dir_v.z = ev.z - sv.z;

		// 線分(start)から点へのベクトル
		Vec3 dir_v2;

		dir_v2.x = p.x - sv.x;
		dir_v2.y = p.y - sv.y;
		dir_v2.z = p.z - sv.z;

		//if ((dir_v2 * dir_v) == 0) {
		//
		//	if()
		//}


		return true;
	}
}