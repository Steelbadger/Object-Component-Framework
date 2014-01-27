#pragma once

#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"


class Position : public Component<Position>
{
public:
	Position(){position = D3DXVECTOR3(0,0,0);}	
	~Position(){;}
	void SetPosition(float x, float y, float z){position = D3DXVECTOR3(x, y, z); GameObject::Get(GetParentID()).HasBeenModified();}
	void Translate(float x, float y, float z){position += D3DXVECTOR3(x, y, z); GameObject::Get(GetParentID()).HasBeenModified();}
	void Translate(D3DXVECTOR3 t){position += t; GameObject::Get(GetParentID()).HasBeenModified();}
	D3DXVECTOR3 GetPosition(){return position;}
private:
	D3DXVECTOR3 position;
};