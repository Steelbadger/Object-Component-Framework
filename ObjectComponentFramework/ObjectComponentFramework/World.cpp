#include "World.h"
#include "GameObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "Position.h"
#include "Texture.h"
#include "TextureTypes.h"
#include "Position.h"
#include "Orientation.h"
#include "Controller.h"
#include "UtilityFunctions.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "ShaderLibrary.h"
#include "PointLight.h"
#include "ControllerFunctors.h"
#include "Transformation.h"
#include "Scale.h"
#include "ObjectManager.h"

World::World()
{
}


World::~World()
{

}


void World::CreateScene()
{
	manager->RegisterType<DirectionalLight>();
	manager->RegisterType<Mesh>();
	manager->RegisterType<Material>();

	textureManager->RegisterType<AmbientTexture>();
	textureManager->RegisterType<NormalMap>();
	textureManager->RegisterType<SpecularMap>();

	SpinController spinny;
	spinny.SetSpinSpeed(0.0f, 0.0f, 0.2f);

	rabd::ObjectID light = manager->CreateObjectAndComponents<DirectionalLight, Controller, Orientation, Transformation>();
	manager->GetComponent<DirectionalLight>(light).SetColour(1.0f, 1.0f, 1.0f, 1.0f);
	manager->GetComponent<DirectionalLight>(light).SetDirection(-0.0f, -1.0f, 0.0f);
	manager->GetComponent<DirectionalLight>(light).SetSpecularPower(10.0f);
	manager->GetComponent<Controller>(light).SetControlFunction(spinny);
	AddToScene(light);

	int num = 15;
	for (int i = 0 ; i < num; i++) {
		rabd::ObjectID test= manager->CreateObjectAndComponents<Position, Orientation, Controller, Transformation>();

		spinny.SetSpinSpeed(0.0f, float(rand()%1000)/1000.0f, 0.0f);

		manager->GetComponent<Position>(test).SetPosition(rand()%1000-100,0,rand()%1000-100);
		manager->GetComponent<Controller>(test).SetControlFunction(spinny);
		AddToScene(test);

		light = manager->CreateObjectAndComponents<Position, Orientation, PointLight, Mesh, Material, Transformation>();

		manager->GetComponent<PointLight>(light).SetColour(float(rand()%100)/100.0f, float(rand()%100)/100.0f, float(rand()%100)/100.0f, 1.0f);
		manager->GetComponent<PointLight>(light).SetSpecularPower(100.0f);
		manager->GetComponent<Position>(light).SetPosition(rand()%100-50,50,0);
		manager->GetComponent<Mesh>(light).SetMeshData(meshFactory->CreateMeshBuffersFromFile("outwardCube.obj", Mesh::NORMALMAPPED));
		manager->GetComponent<Material>(light).AddTexture<AmbientTexture>("brick1.dds");
		manager->GetComponent<Material>(light).AddTexture<NormalMap>("brick1norm.jpg");
		manager->GetComponent<Material>(light).AddTexture<SpecularMap>("brick1spec.jpg");
		manager->GetComponent<Material>(light).SetShader(ShaderLibrary::Shaders::NORMAL);
		Material* thingy = &manager->GetComponent<Material>(light);

		AddToScene(light);
		manager->SetParent<rabd::GameObject>(light, test);
	}

	rabd::ObjectID ground = manager->CreateObjectAndComponents<Position, Orientation, Mesh, Material, Transformation>();

	manager->GetComponent<Position>(ground).SetPosition(-100,-0.5,-100);
	manager->GetComponent<Mesh>(ground).SetMeshData(meshFactory->CreatePrimitive());
	manager->GetComponent<Material>(ground).AddTexture<AmbientTexture>("stone01.dds");
	manager->GetComponent<Material>(ground).AddTexture<NormalMap>("bump01.dds");
	manager->GetComponent<Material>(ground).AddTexture<SpecularMap>("spec01.jpg");
	manager->GetComponent<Material>(ground).SetShader(ShaderLibrary::Shaders::NORMAL);
	AddToScene(ground);
}

void World::SetCameraObject(rabd::ObjectID id)
{
	if (manager->HasComponent<Camera>(id)) {
		currentCamera = id;
	} else {
		Warning("Supposed Camera has no camera component and was not added to world");
	}

	if (manager->HasComponent<Controller>(id)) {
		updateList.push_back(id);
	} else {
		Warning("Camera has no controller component and was not added to the update list");
	}

	if (manager->HasComponent<PointLight>(id)) {
		lightList.push_back(id);
	} else {
//		Warning("Object has no controller component and was not added to the update list");
	}
}

rabd::ObjectID World::GetCameraObject()
{
	return currentCamera;
}

std::list<rabd::ObjectID> World::GetDrawList()
{
	return drawList;
}

std::list<rabd::ObjectID>& World::GetUpdateList()
{
	return updateList;
}

std::list<rabd::ObjectID> World::GetLightList()
{
	return lightList;
}


void World::AddToScene(rabd::ObjectID id)
{
	if (manager->HasComponent<Mesh>(id)) {
		drawList.push_back(id);
	} else {
		Warning("Object has no mesh component and was not added to draw List");
	}

	if (manager->HasComponent<Controller>(id)) {
		updateList.push_back(id);
	} else {
//		Warning("Object has no controller component and was not added to the update list");
	}

	if (manager->HasComponent<DirectionalLight>(id) || manager->HasComponent<PointLight>(id)) {
		lightList.push_back(id);
	} else {
//		Warning("Object has no light component and was not added to the light list");
	}
}

void World::PassMeshFactory(MeshFactory* factory)
{
	meshFactory = factory;
}

void World::PassObjectManager(rabd::ObjectManager* m)
{
	manager = m;
}

void World::PassTextureManager(rabd::Manager* m)
{
	textureManager = m;
}

rabd::ObjectManager* World::GetManager()
{
	return manager;
}
