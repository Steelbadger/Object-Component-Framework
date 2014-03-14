#pragma once


#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>

class Transformation : public rabd::Component<Transformation>
{
public:
	Transformation();
	~Transformation();

	const D3DXMATRIX& GetTransformation();
	void SetChanged();
	inline bool ViewHasChanged(){return viewChanged;}
	inline void SetViewUnchanged(){viewChanged = false;}
private:
	void CalculateLocalTransformation();
	void CalculateGlobalTransformation();
	void SetGlobalChanged();

	bool localChanged;
	bool globalChanged;
	bool viewChanged;

	D3DXMATRIX localTransform;
	D3DXMATRIX globalTransform;
};