#pragma once
#include"../Window/Window.h"
#include<math.h>




struct Vec3 : public D3DXVECTOR3{

	Vec3() {
		Init();
	}

	Vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Init() {
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
	}

	Vec3 GetNormalize() {

		Vec3 vec3;

		// 単位ベクトル化(1)にし、回転値など受け取る
		vec3.x = x / GetLength();
		vec3.y = y / GetLength();
		vec3.z = z / GetLength();

		return vec3;
	}

	float GetLength() {
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	// 拡張機能
	void operator =(const D3DXVECTOR3& vec3) {
		this->x = vec3.x;
		this->y = vec3.y;
		this->z = vec3.z;
	}

	D3DXVECTOR3 operator *(const D3DXVECTOR3& vec3) {

		D3DXVECTOR3 d_vec3;
		d_vec3.x = this->x * vec3.x;
		d_vec3.y = this->y * vec3.y;
		d_vec3.z = this->z * vec3.z;

		return vec3;
	}

	D3DXVECTOR3 operator +(const D3DXVECTOR3& vec3) {

		D3DXVECTOR3 d_vec3;
		d_vec3.x = this->x + vec3.x;
		d_vec3.y = this->y + vec3.y;
		d_vec3.z = this->z + vec3.z;

		return vec3;
	}

	Vec3 operator +(const Vec3& vec3) {

		Vec3 d_vec3;
		d_vec3.x = this->x + vec3.x;
		d_vec3.y = this->y + vec3.y;
		d_vec3.z = this->z + vec3.z;

		return vec3;
	}

	D3DXVECTOR3 operator -(const D3DXVECTOR3& vec3) {

		D3DXVECTOR3 d_vec3;
		d_vec3.x = this->x - vec3.x;
		d_vec3.y = this->y - vec3.y;
		d_vec3.z = this->z - vec3.z;

		return vec3;
	}

	D3DXVECTOR3 operator /(const D3DXVECTOR3& vec3) {

		D3DXVECTOR3 d_vec3;
		d_vec3.x = this->x / vec3.x;
		d_vec3.y = this->y / vec3.y;
		d_vec3.z = this->z / vec3.z;

		return vec3;
	}


	void operator *=(const D3DXVECTOR3& vec3) {
		this->x *= vec3.x;
		this->y *= vec3.y;
		this->z *= vec3.z;
	}

	void operator +=(const D3DXVECTOR3& vec3) {
		this->x += vec3.x;
		this->y += vec3.y;
		this->z += vec3.z;
	}

	void operator -=(const D3DXVECTOR3& vec3) {
		this->x -= vec3.x;
		this->y -= vec3.y;
		this->z -= vec3.z;
	}

	void operator /=(const D3DXVECTOR3& vec3) {
		this->x /= vec3.x;
		this->y /= vec3.y;
		this->z /= vec3.z;
	}

};