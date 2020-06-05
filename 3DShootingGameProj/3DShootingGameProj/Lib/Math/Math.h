#pragma once
#include<d3dx9.h>
#include<math.h>
#include"../Vec3/Vec3.h"
#include"../Vec2/Vec2.h"


namespace Utility {

	namespace Math {


		/*
		@ brief 移動　回転　拡縮の行列変換を行う
		*/
		void OutTransformMatrix(
			D3DXMATRIX&total_mat,
			D3DXVECTOR3&pos,
			D3DXVECTOR3&scale = D3DXVECTOR3(1.f,1.f,1.f),
			D3DXVECTOR3&rota = D3DXVECTOR3(0.f,0.f,0.f)
		);


		/**
		* @brief 移動 回転 拡縮の行列変換を行う
		*/
		D3DXMATRIX GetTransformMatrix(
			D3DXVECTOR3&pos,
			D3DXVECTOR3&scale = D3DXVECTOR3(1.f,1.f,1.f),
			D3DXVECTOR3&rota = D3DXVECTOR3(0.f,0.f,0.f)
		);

		void MatrixMultipleVec3(
			D3DXVECTOR3&out_vec3,
			const D3DXVECTOR3&base_vec3,
			const D3DXMATRIX&mat
		);

		void MatrixMultipleVec4(
			D3DXVECTOR4&out_vec3,
			const D3DXVECTOR4&base_vec3,
			const D3DXMATRIX&mat
		);

		float GetLength(const Vec3&dir);

		Vec3 GetVec1ToVec2(const Vec3&vec1, const Vec3&vec2);


		/**
		* @brief 単位ベクトルを返す(長さが1の状態)
		*/
		Vec3 CalcNormalize(const Vec3&vec1);


		/**
		* @brief 正規化方向を返す(単位ベクトル)
		*/
		Vec3 CalcDirNormalize(const Vec3&vec1, const Vec3&vec2);


		/**
		* @brief 内積を返す(cosを取りたい場合は単位ベクトル化する,スカラー積)
		*/
		float Vec3Dot(const Vec3&vec1, const Vec3&vec2);


		/**
		* @brief 外積を返す(ベクトル積,垂直なベクトルを出せる)
		*/
		Vec3 Vec3Cross(const Vec3&vec1, const Vec3&vec2);


		/**
		* @brief 2D回転
		*/
		Vec2 Vec2Rota(const Vec2&vec1,const float&rad);


		/**
		* @brief 2D拡縮
		*/
		Vec2 Vec2Scale(const Vec2&vec1, const float&scale_value);
	}
}