#pragma once

#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <xnamath.h>

class Camera : public rabd::Component<Camera>
{
public:
	Camera();
	~Camera();
	void Initialise(bool perspective, float fovDegrees, float viewportWidth, float viewportHeight, float nearPlane, float farPlane);
	void SetPerspective(bool p);
	void SetViewportDimensions(float width, float height);
	void SetClipPlanes(float nearPlane, float farPlane);
	void SetFieldOfView(float degrees);
	float GetNearClipPlane();
	float GetFarClipPlane();
	void Zoom(float zoom);
	const XMMATRIX& GetViewMatrix();
	const XMMATRIX& GetProjectionMatrix();

private:
	bool perspective;
	float fieldOfView, nearCull, farCull, viewportWidth, viewportHeight;
	XMMATRIX projectionMatrix;
	bool projectionChange;
	XMMATRIX viewMatrix;
};