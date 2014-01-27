#pragma once
#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"

class PointLight : public Component<PointLight>
{
public:
	PointLight(){specularPower = 40;};
	~PointLight(){};
	void SetColour(float r, float g, float b, float a){m_Colour = D3DXVECTOR4(r,g,b,a);}
	void SetSpecularPower(float p) {specularPower = p;}

	D3DXVECTOR4 GetColour(){return m_Colour;}
	D3DXVECTOR3 GetPosition(){
		D3DXVECTOR4 pos(0,0,0, 1);
		D3DXMATRIX trans = GameObject::Get(GetParentID()).GetLocalMatrix();
		D3DXVec4Transform(&pos, &pos, &trans);
		pos = pos/pos.w;
		return D3DXVECTOR3(pos.x, pos.y, pos.z);	
	}
	float GetSpecularPower(){return specularPower;}
private:
	D3DXVECTOR4 m_Colour;
	D3DXVECTOR3 m_Position;
	float specularPower;
};