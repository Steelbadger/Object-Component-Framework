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


World::World()
{
}


World::~World()
{

}


void World::CreateScene()
{
	SpinController spinny;
	spinny.SetSpinSpeed(0.0f, 0.0f, 0.2f);

	ObjectID light = GameObject::New();
	GameObject::AddComponent<DirectionalLight>(light);
	GameObject::AddComponent<Controller>(light);
	GameObject::GetComponent<DirectionalLight>(light).SetColour(1.0f, 1.0f, 1.0f, 1.0f);
	GameObject::GetComponent<DirectionalLight>(light).SetDirection(-0.0f, -1.0f, 0.0f);
	GameObject::GetComponent<DirectionalLight>(light).SetSpecularPower(10.0f);
	GameObject::GetComponent<Controller>(light).SetControlFunction(spinny);
	AddToScene(light);

	ObjectID quad = GameObject::New();
	GameObject::AddComponent<Position>(quad);
	GameObject::AddComponent<Orientation>(quad);
	GameObject::AddComponent<Mesh>(quad);
	GameObject::AddComponent<Material>(quad);
	GameObject::AddComponent<Controller>(quad);

	spinny.SetSpinSpeed(0.0f, 0.1f, 0.0f);

	GameObject::GetComponent<Position>(quad).SetPosition(0,0,5);
	GameObject::GetComponent<Mesh>(quad).SetMeshData(meshFactory->CreateMeshBuffersFromFile("crate.obj", Mesh::NORMALMAPPED));
	GameObject::GetComponent<Material>(quad).AddTexture<AmbientTexture>("crateDiffuse.jpg");
	GameObject::GetComponent<Material>(quad).AddTexture<NormalMap>("crateNormal.jpg");
	GameObject::GetComponent<Material>(quad).AddTexture<SpecularMap>("crateSpec.jpg");
	GameObject::GetComponent<Material>(quad).SetShader(ShaderLibrary::Shaders::NORMAL);
	GameObject::GetComponent<Controller>(quad).SetControlFunction(spinny);

	ObjectID test= GameObject::New();
	GameObject::AddComponent<Position>(test);
	GameObject::AddComponent<Orientation>(test);
	GameObject::AddComponent<Mesh>(test);
	GameObject::AddComponent<Material>(test);
	GameObject::AddComponent<Controller>(test);

	spinny.SetSpinSpeed(0.0f, 1.0f, 0.0f);

	GameObject::GetComponent<Position>(test).SetPosition(0,0,3);
	GameObject::GetComponent<Mesh>(test).SetMeshData(meshFactory->CreateMeshBuffersFromFile("outwardCube.obj", Mesh::NORMALMAPPED));
	GameObject::GetComponent<Material>(test).AddTexture<AmbientTexture>("brick1.dds");
	GameObject::GetComponent<Material>(test).AddTexture<NormalMap>("brick1norm.jpg");
	GameObject::GetComponent<Material>(test).AddTexture<SpecularMap>("brick1spec.jpg");
	GameObject::GetComponent<Material>(test).SetShader(ShaderLibrary::Shaders::NORMAL);
	GameObject::GetComponent<Controller>(test).SetControlFunction(spinny);
	GameObject::SetParentChild(quad, test);

	AddToScene(quad);
	AddToScene(test);

	for (int i = 0 ; i < 500; i++) {
		ObjectID test= GameObject::New();
		GameObject::AddComponent<Position>(test);
		GameObject::AddComponent<Orientation>(test);
		GameObject::AddComponent<Mesh>(test);
		GameObject::AddComponent<Material>(test);
		GameObject::AddComponent<Controller>(test);

		spinny.SetSpinSpeed(0.0f, float(rand()%1000)/1000.0f, 0.0f);

		GameObject::GetComponent<Position>(test).SetPosition(rand()%200-100,0,rand()%200-100);
		GameObject::GetComponent<Mesh>(test).SetMeshData(meshFactory->CreateMeshBuffersFromFile("outwardCube.obj", Mesh::NORMALMAPPED));
		GameObject::GetComponent<Material>(test).AddTexture<AmbientTexture>("brick1.dds");
		GameObject::GetComponent<Material>(test).AddTexture<NormalMap>("brick1norm.jpg");
		GameObject::GetComponent<Material>(test).AddTexture<SpecularMap>("brick1spec.jpg");
		GameObject::GetComponent<Material>(test).SetShader(ShaderLibrary::Shaders::NORMAL);
		GameObject::GetComponent<Controller>(test).SetControlFunction(spinny);
		AddToScene(test);

		if (i%5 == 0) {
			ObjectID light3 = GameObject::New();
			GameObject::AddComponent<Position>(light3);
			GameObject::AddComponent<Orientation>(light3);
			GameObject::AddComponent<PointLight>(light3);
			GameObject::GetComponent<PointLight>(light3).SetColour(float(rand()%100)/100.0f, float(rand()%100)/100.0f, float(rand()%100)/100.0f, 1.0f);
			GameObject::GetComponent<PointLight>(light3).SetSpecularPower(100.0f);
			GameObject::GetComponent<Position>(light3).SetPosition(rand()%20-10,0.5,0);
			AddToScene(light3);
			GameObject::SetParentChild(test, light3);
		}
	}

	//for (int i = 0 ; i < 50; i++) {
	//	ObjectID light3 = GameObject::New();
	//	GameObject::AddComponent<Position>(light3);
	//	GameObject::AddComponent<Orientation>(light3);
	//	GameObject::AddComponent<PointLight>(light3);
	//	GameObject::GetComponent<PointLight>(light3).SetColour(float(rand()%100)/100.0f, float(rand()%100)/100.0f, float(rand()%100)/100.0f, 1.0f);
	//	GameObject::GetComponent<PointLight>(light3).SetSpecularPower(100.0f);
	//	GameObject::GetComponent<Position>(light3).SetPosition(rand()%200-100,0.5,rand()%200-100);
	//	AddToScene(light3);
	//}

	ObjectID ground = GameObject::New();
	GameObject::AddComponent<Position>(ground);
	GameObject::AddComponent<Orientation>(ground);
	GameObject::AddComponent<Mesh>(ground);
	GameObject::AddComponent<Material>(ground);

	GameObject::GetComponent<Position>(ground).SetPosition(-100,-0.5,-100);
	GameObject::GetComponent<Mesh>(ground).SetMeshData(meshFactory->CreatePrimitive());
	GameObject::GetComponent<Material>(ground).AddTexture<AmbientTexture>("stone01.dds");
	GameObject::GetComponent<Material>(ground).AddTexture<NormalMap>("bump01.dds");
	GameObject::GetComponent<Material>(ground).AddTexture<SpecularMap>("spec01.jpg");
	GameObject::GetComponent<Material>(ground).SetShader(ShaderLibrary::Shaders::NORMAL);
	AddToScene(ground);

	ObjectID light2;
	light2 = GameObject::New();
	GameObject::AddComponent<Position>(light2);
	GameObject::AddComponent<Orientation>(light2);
	GameObject::AddComponent<PointLight>(light2);
	GameObject::GetComponent<PointLight>(light2).SetColour(0.0f, 1.0f, 0.0f, 1.0f);
	GameObject::GetComponent<PointLight>(light2).SetSpecularPower(100.0f);
	GameObject::GetComponent<Position>(light2).SetPosition(0,1,0);
	AddToScene(light2);

	light2 = GameObject::New();
	GameObject::AddComponent<Position>(light2);
	GameObject::AddComponent<Orientation>(light2);
	GameObject::AddComponent<PointLight>(light2);
	GameObject::GetComponent<PointLight>(light2).SetColour(0.0f, 0.0f, 1.0f, 1.0f);
	GameObject::GetComponent<PointLight>(light2).SetSpecularPower(100.0f);
	GameObject::GetComponent<Position>(light2).SetPosition(8,0.5,-12);
	AddToScene(light2);

	light2 = GameObject::New();
	GameObject::AddComponent<Position>(light2);
	GameObject::AddComponent<Orientation>(light2);
	GameObject::AddComponent<PointLight>(light2);
	GameObject::GetComponent<PointLight>(light2).SetColour(0.0f, 1.0f, 1.0f, 1.0f);
	GameObject::GetComponent<PointLight>(light2).SetSpecularPower(100.0f);
	GameObject::GetComponent<Position>(light2).SetPosition(0,0.5,21);
	AddToScene(light2);
}

void World::SetCameraObject(ObjectID id)
{
	if (GameObject::HasComponent<Camera>(id)) {
		currentCamera = id;
	} else {
		Warning("Object has no camera component and was not added to world");
	}

	if (GameObject::HasComponent<Controller>(id)) {
		updateList.push_back(id);
	} else {
		Warning("Object has no controller component and was not added to the update list");
	}

	if (GameObject::HasComponent<PointLight>(id)) {
		lightList.push_back(id);
	} else {
//		Warning("Object has no controller component and was not added to the update list");
	}
}

ObjectID World::GetCameraObject()
{
	return currentCamera;
}

std::list<ObjectID> World::GetDrawList()
{
	return drawList;
}

std::list<ObjectID>& World::GetUpdateList()
{
	return updateList;
}

std::list<ObjectID> World::GetLightList()
{
	return lightList;
}


void World::AddToScene(ObjectID id)
{
	if (GameObject::HasComponent<Mesh>(id)) {
		drawList.push_back(id);
	} else {
		Warning("Object has no mesh component and was not added to draw List");
	}

	if (GameObject::HasComponent<Controller>(id)) {
		updateList.push_back(id);
	} else {
//		Warning("Object has no controller component and was not added to the update list");
	}

	if (GameObject::HasComponent<DirectionalLight>(id) || GameObject::HasComponent<PointLight>(id)) {
		lightList.push_back(id);
	} else {
//		Warning("Object has no light component and was not added to the light list");
	}
}

void World::PassMeshFactory(MeshFactory* factory)
{
	meshFactory = factory;
}
