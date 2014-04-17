#pragma once
#include "Component.h"
#include "Transformation.h"
#include "GameObject.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <xnamath.h>
#include "ObjectManager.h"

class DirectionalLight : public rabd::Component<DirectionalLight>
{
public:
	DirectionalLight(){specularPower = 40;};
	~DirectionalLight(){};
	void SetColour(float r, float g, float b, float a){m_Colour = XMVectorSet(r, g, b, a);}
	void SetDirection(float x, float y, float z){m_Direction = XMVectorSet(x, y, z, 1.0f);}
	void SetSpecularPower(float p) {specularPower = p;}

	XMVECTOR GetColour(){return m_Colour;}
	XMVECTOR GetDirection(){
		return XMVector3Transform(m_Direction, manager->GetComponent<Transformation>(GetParentID()).GetTransformation());
	}
	float GetSpecularPower(){return specularPower;}
private:
	XMVECTOR m_Colour;
	XMVECTOR m_Direction;
	float specularPower;
};