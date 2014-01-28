#include "World.h"



World::World()
{
}


World::~World()
{

}


void World::CreateScene()
{

}

void World::SetCameraObject(ObjectID id)
{
}

ObjectID World::GetCameraObject()
{
	return currentCamera;
}

std::list<ObjectID> World::GetDrawList()
{
	return drawList;
}

std::list<ObjectID> World::GetUpdateList()
{
	return updateList;
}

std::list<ObjectID> World::GetLightList()
{
	return lightList;
}


void World::AddToScene(ObjectID id)
{
}

void World::PassMeshFactory(MeshFactory* factory)
{
	meshFactory = factory;
}

void World::Remove(ObjectID id)
{
}