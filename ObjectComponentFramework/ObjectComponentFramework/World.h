#pragma once
#include "Component.h"
#include <list>

class MeshFactory;
class World
{
public:
	World();
	~World();

	void CreateScene();
	void SetCameraObject(ObjectID id);
	ObjectID GetCameraObject();
	std::list<ObjectID> GetDrawList();
	std::list<ObjectID> GetUpdateList();
	std::list<ObjectID> GetLightList();
	void AddToScene(ObjectID id);
	void PassMeshFactory(MeshFactory* factory);

private:
	ObjectID currentCamera;

	std::list<ObjectID> drawList;
	std::list<ObjectID> updateList;
	std::list<ObjectID> lightList;
	MeshFactory* meshFactory;
};