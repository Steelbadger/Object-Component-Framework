#pragma once
#include "Component.h"
#include "LookupTable.h"
#include "Manager.h"
#include <list>

class MeshFactory;
class rabd::ObjectManager;

class World
{
public:
	World();
	~World();

	void CreateScene();
	void SetCameraObject(rabd::ObjectID id);
	rabd::ObjectID GetCameraObject();
	std::list<rabd::ObjectID> GetDrawList();
	std::list<rabd::ObjectID>& GetUpdateList();
	std::list<rabd::ObjectID> GetLightList();
	void AddToScene(rabd::ObjectID id);
	void PassMeshFactory(MeshFactory* factory);
	void PassObjectManager(rabd::ObjectManager* manager);
	void PassTextureManager(rabd::Manager* manager);
	rabd::ObjectManager* GetManager();
private:
	rabd::ObjectID currentCamera;

	std::list<rabd::ObjectID> drawList;
	std::list<rabd::ObjectID> updateList;
	std::list<rabd::ObjectID> lightList;
	MeshFactory* meshFactory;
	rabd::ObjectManager* manager;
	rabd::Manager* textureManager;
};