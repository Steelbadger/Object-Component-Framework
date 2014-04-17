#pragma once

#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <xnamath.h>

class Transformation : public rabd::Component<Transformation>
{
public:
	Transformation();
	~Transformation();

	const XMMATRIX& GetTransformation();
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

	XMMATRIX localTransform;
	XMMATRIX globalTransform;
};