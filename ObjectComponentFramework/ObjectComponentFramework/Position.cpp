#include "Position.h"
#include "Transformation.h"
#include "GameObject.h"
#include "ObjectManager.h"

Position::Position()
{
	position = D3DXVECTOR3(0,0,0);
}

Position::~Position()
{
}

void Position::SetPosition(float x, float y, float z)
{
	position = D3DXVECTOR3(x, y, z);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Position::Translate(float x, float y, float z)
{
	position += D3DXVECTOR3(x, y, z);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Position::Translate(D3DXVECTOR3 t)
{
	position += t;
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

D3DXVECTOR3 Position::GetPosition()
{
	return position;
}
