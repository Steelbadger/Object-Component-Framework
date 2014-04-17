#include "Transformation.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"
#include "Orientation.h"
#include "Position.h"
#include "Scale.h"
#include "simdUtility.h"
#include "ObjectManager.h"
#include "LookupTable.h"



Transformation::Transformation():
	localChanged(true), globalChanged(true), viewChanged(true)
{
	changedLock = new Concurrency::reader_writer_lock;
	globalLock = new Concurrency::reader_writer_lock;
	localLock = new Concurrency::reader_writer_lock;
}

Transformation::Transformation(Transformation& t):
	localChanged(t.localChanged), globalChanged(t.globalChanged), viewChanged(t.viewChanged)
{
	changedLock = new Concurrency::reader_writer_lock;
	globalLock = new Concurrency::reader_writer_lock;
	localLock = new Concurrency::reader_writer_lock;
}

Transformation::~Transformation()
{
	delete changedLock;
	delete globalLock;
	delete localLock;
}

const D3DXMATRIX& Transformation::GetTransformation()
{
	changedLock->lock_read();
	if (globalChanged) {
		changedLock->unlock();
		CalculateGlobalTransformation();
		changedLock->lock();
		viewChanged = true;
	}
	changedLock->unlock();
	return globalTransform;
}

void Transformation::SetChanged()
{
	changedLock->lock();
	localChanged = true;
	SetGlobalChanged();
	changedLock->unlock();
}

void Transformation::SetGlobalChanged()
{
	globalChanged = true;
	viewChanged = true;
	std::vector<rabd::ObjectID>* children = &manager->Get(GetParentID()).GetChildren();
	for (int i = 0; i < children->size(); i++) {
		manager->GetComponent<Transformation>(children->at(i)).SetGlobalChanged();
	}
}

void Transformation::CalculateLocalTransformation()
{
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	D3DXMATRIX translate;

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&translate);

	if (manager->HasComponent<Orientation>(GetParentID())) {
		rotation = manager->GetComponent<Orientation>(GetParentID()).GetMatrix();
	}
	if (manager->HasComponent<Scale>(GetParentID())) {
		scale = manager->GetComponent<Scale>(GetParentID()).GetMatrix();
	}
	if (manager->HasComponent<Position>(GetParentID())) {
		D3DXVECTOR3 pos = manager->GetComponent<Position>(GetParentID()).GetPosition();
		D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	}
	localLock->lock();
	localTransform = rotation * scale * translate;
	localLock->unlock();
	changedLock->lock();
	localChanged = false;
	changedLock->unlock();
}

void Transformation::CalculateGlobalTransformation()
{
	changedLock->lock_read();
	if (localChanged) {
		changedLock->unlock();
		CalculateLocalTransformation();
		changedLock->lock_read();
	}
	changedLock->unlock();
	D3DXMATRIX parent;
	D3DXMatrixIdentity(&parent);
	if (manager->Get(GetParentID()).HasParent()) {
		parent = manager->GetComponent<Transformation>(manager->Get(GetParentID()).GetParentID()).GetTransformation();
	}
	globalLock->lock();
	globalTransform = localTransform * parent;
	globalLock->unlock();
	changedLock->lock();
	globalChanged = false;
	changedLock->unlock();
}
