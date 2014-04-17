#include "Position.h"
#include "Transformation.h"
#include "GameObject.h"
#include "ObjectManager.h"

Position::Position()
{
	position = XMVectorZero();
}

Position::~Position()
{
}

void Position::SetPosition(float x, float y, float z)
{
	position = XMVectorSet(x, y, z, 1.0f);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Position::Translate(float x, float y, float z)
{
	position += XMVectorSet(x, y, z, 1.0f);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Position::Translate(FXMVECTOR t)
{
	position += t;
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

XMVECTOR Position::GetPosition()
{
	return position;
}
