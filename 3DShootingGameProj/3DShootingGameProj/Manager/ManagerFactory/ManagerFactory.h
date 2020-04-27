#pragma once
#include"../FileDataManager/FileDataManager.h"
#include"../MapObjectManager/MapObjectManager.h"
#include"../MapStructureManager/MapStructureManager.h"
#include<memory>


class ManagerFactory {
public:


	void CreateFileObjectDataManager(
		std::shared_ptr<ObjectFactory>object_factory,
		std::shared_ptr<Camera3D>camera,
		std::shared_ptr<ObjectData>p_data,
		std::weak_ptr<FileDataManager>p_out_mng
	);


	void CreateMapCharacterManager(
		std::shared_ptr<FileDataManager>p_manager,
		std::shared_ptr<ObjectFactory>p_factory,
		std::shared_ptr<ObjectData>p_data,
	std::weak_ptr<MapObjectManager>p_out_mng
	);


	void CreateMapObjectManager(
		std::shared_ptr<ObjectFactory>p_factory,
		std::shared_ptr<ObjectData>p_data,
	std::weak_ptr<MapStructureManager>p_out_mng
	);
};