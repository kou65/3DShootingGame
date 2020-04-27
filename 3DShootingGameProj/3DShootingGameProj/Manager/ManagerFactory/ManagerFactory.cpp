#include"ManagerFactory.h"
#include"../ManagerControlOffcer/ManagerControlOffcer.h"



void ManagerFactory::CreateFileObjectDataManager(
	std::shared_ptr<ObjectFactory>object_factory,
	std::shared_ptr<Camera3D>camera,
	std::shared_ptr<ObjectData>p_data,
	std::weak_ptr<FileDataManager>p_out_mng
) {


	// 共有用作成
	std::shared_ptr<FileDataManager>p_player
		= std::make_shared<FileDataManager>
		(object_factory,
			camera,
			p_data
			);
	
	ManagerControlOffcer::GetInstance()->EntryShared(p_player);

	p_out_mng = p_player;
}


void ManagerFactory::CreateMapCharacterManager(
	std::shared_ptr<FileDataManager>p_manager,
	std::shared_ptr<ObjectFactory>p_factory,
	std::shared_ptr<ObjectData>p_data,
	std::weak_ptr<MapObjectManager>p_out_mng
) {


	// 共有用作成
	std::shared_ptr<MapObjectManager>p_player
		= std::make_shared<MapObjectManager>
		(p_manager,
			p_factory,
			p_data
			);

	// 共有エントリー
	ManagerControlOffcer::GetInstance()->EntryShared(p_player);

	p_out_mng = p_player;
}


void ManagerFactory::CreateMapObjectManager(
	std::shared_ptr<ObjectFactory>p_factory,
	std::shared_ptr<ObjectData>p_data,
	std::weak_ptr<MapStructureManager>p_out_mng
) {

	// 共有用作成
	std::shared_ptr<MapStructureManager>p_player
		= std::make_shared<MapStructureManager>
		(p_factory,
			p_data
		);

	// 共有エントリー
	ManagerControlOffcer::GetInstance()->EntryShared(p_player);

	p_out_mng = p_player;
}