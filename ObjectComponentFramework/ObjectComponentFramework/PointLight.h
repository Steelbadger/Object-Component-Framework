#pragma once
#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"
#include "Transformation.h"
#include "ObjectManager.h"
#include <xnamath.h>


class PointLight : public rabd::Component<PointLight>
{
public:
	PointLight(){specularPower = 40;};
	~PointLight(){};
	void SetColour(float r, float g, float b, float a){m_Colour = XMVectorSet(r,g,b,a);}
	void SetSpecularPower(float p) {specularPower = p;}

	XMVECTOR GetColour(){return m_Colour;}
	XMVECTOR GetPosition(){
		return XMVector3TransformCoord(XMVectorSet(0,0,0,1), manager->GetComponent<Transformation>(GetParentID()).GetTransformation());
	}

	float GetSpecularPower(){return specularPower;}
private:
	XMVECTOR m_Colour;
	XMVECTOR m_Position;
	float specularPower;
};