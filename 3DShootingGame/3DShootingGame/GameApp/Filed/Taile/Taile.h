#pragma once
#include"../MapObjectBase/MapObjectBase.h"
#include"../../../Lib/TransformData/TransformData.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"



class Taile : public MapObjectBase {
public:

	// テクスチャサイズ
	constexpr static float TAILE_SIZE_X = 4.f;
	constexpr static float TAILE_SIZE_Y = 4.f;

public:

	enum class Direction {
		LEFT,
		RIGHT,
		FLOOR,
	};

public:

	Taile(const ObjParameter&data,
		const Direction &dir = Direction::FLOOR);

	void Update()override;
	void Draw()override;

private:

	void SetDirection(const Direction&dir);

private:

	ObjParameter m_data;
};