#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

#include "Component.h"

class Position : public Component<Position>
{
public:
	Position();
	~Position();
	void SetPosition(float x, float y, float z);
	void Translate(float x, float y, float z);
	void Translate(D3DXVECTOR3 t);
	D3DXVECTOR3 GetPosition();
private:
	D3DXVECTOR3 position;
};