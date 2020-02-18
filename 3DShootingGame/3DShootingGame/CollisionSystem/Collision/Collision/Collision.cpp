#include"Collision.h"


namespace Collision{

	bool HitSphereAndSphere(
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


	bool HitRayAndSphere(
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
}