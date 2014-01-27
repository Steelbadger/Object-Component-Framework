#include "Scale.h"

//Scale::Scale()
//{
//	scale = D3DXVECTOR3(1,1,1);
//}
//
//Scale::~Scale()
//{
//}
//
//void Scale::SetScale(float x, float y, float z)
//{
//	scale = D3DXVECTOR3(x, y, z);
//}
//
//void Scale::ModifyScale(float x, float y, float z)
//{
//	scale = D3DXVECTOR3(x*scale.x,y*scale.y,z*scale.z);
//}
//
//D3DXMATRIX Scale::GetMatrix()
//{
//	D3DXMATRIX output;
//	D3DXMatrixScaling(&output, scale.x, scale.y, scale.z);
//	return output;
//}
//
//D3DXVECTOR3 Scale::GetScale()
//{
//	return scale;
//}