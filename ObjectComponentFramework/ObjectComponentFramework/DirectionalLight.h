#pragma once
#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>

class DirectionalLight : public Component<DirectionalLight>
{
public:
	DirectionalLight(){specularPower = 40;};
	~DirectionalLight(){};
	void SetColour(float r, float g, float b, float a){m_Colour = D3DXVECTOR4(r,g,b,a);}
	void SetDirection(float x, float y, float z){m_Direction = D3DXVECTOR3(x, y, z);}
	void SetSpecularPower(float p) {specularPower = p;}

	D3DXVECTOR4 GetColour(){return m_Colour;}
	D3DXVECTOR3 GetDirection(){
		D3DXVECTOR4 output(m_Direction);
		D3DXVec3Transform(&output, &m_Direction, &GameObject::Get(GetParentID()).GetLocalMatrix());
		return D3DXVECTOR3(output);	
	}
	float GetSpecularPower(){return specularPower;}
private:
	D3DXVECTOR4 m_Colour;
	D3DXVECTOR3 m_Direction;
	float specularPower;
};