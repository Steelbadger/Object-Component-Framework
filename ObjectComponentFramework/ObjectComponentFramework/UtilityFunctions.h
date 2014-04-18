#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <d3dx11.h>
#include <d3dx10math.h>
#include <xnamath.h>

void Error(std::string output);
void Warning(std::string output);
void Info(std::string output);
void TimerMark(bool output = false);
inline D3DXVECTOR3 CvtXMVecToD3DXVec3(FXMVECTOR v) {
	D3DXVECTOR3 out;
	XMStoreFloat3((XMFLOAT3*)(&out), v);
	return out;
}
inline D3DXVECTOR4 CvtXMVecToD3DXVec4(FXMVECTOR v) {
	D3DXVECTOR4 out;
	XMStoreFloat4((XMFLOAT4*)(&out), v);
	return out;
}
inline D3DXMATRIX CvtXMMatToD3DXMat(CXMMATRIX m) {
	D3DXMATRIX out;
	XMStoreFloat4x4((XMFLOAT4X4*)(&out), m);
	return out;
}
