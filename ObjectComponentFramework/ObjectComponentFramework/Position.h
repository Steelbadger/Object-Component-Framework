#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <xnamath.h>

#include "Component.h"

class Position : public rabd::Component<Position>
{
public:
	Position();
	~Position();
	void SetPosition(float x, float y, float z);
	void Translate(float x, float y, float z);
	void Translate(XMVECTOR t);
	XMVECTOR GetPosition();
private:
	XMVECTOR position;
};