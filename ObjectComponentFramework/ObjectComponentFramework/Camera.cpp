#include "Camera.h"
#include "GameObject.h"
#include "Position.h"
#include "Orientation.h"
#include "Transformation.h"
#include "ObjectManager.h"


Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::Initialise(bool perspective, float fovDegrees, float viewportWidth, float viewportHeight, float nearPlane, float farPlane)
{
	SetPerspective(perspective);
	SetViewportDimensions(viewportWidth, viewportHeight);
	SetClipPlanes(nearPlane, farPlane);
	SetFieldOfView(fovDegrees);
}

void Camera::SetPerspective(bool mode)
{
	perspective = mode;
	projectionChange = true;
}

void Camera::SetViewportDimensions(float width, float height)
{
	viewportWidth = width;
	viewportHeight = height;
	projectionChange = true;
}

void Camera::SetClipPlanes(float nearPlane, float farPlane)
{
	nearCull = nearPlane;
	farCull = farPlane;
	projectionChange = true;
}

void Camera::SetFieldOfView(float degrees)
{
	fieldOfView = (degrees * (float)D3DX_PI)/180.0f;
	projectionChange = true;
}

const XMMATRIX& Camera::GetViewMatrix()
{
	if (manager->GetComponent<Transformation>(GetParentID()).ViewHasChanged()) {
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
		XMVECTOR position = manager->GetComponent<Position>(GetParentID()).GetPosition();
		XMMATRIX orientationMatrix = manager->GetComponent<Orientation>(GetParentID()).GetMatrix();

		forward = XMVector3TransformCoord(forward, orientationMatrix);
		up = XMVector3TransformCoord(up, orientationMatrix);

		XMVECTOR lookAt = position + forward;

		// Finally create the view matrix from the three updated vectors.
		viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

		manager->GetComponent<Transformation>(GetParentID()).SetViewUnchanged();
	}

	return viewMatrix;
}


const XMMATRIX& Camera::GetProjectionMatrix()
{
	if (projectionChange == true) {
		if (perspective) {
			projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, (viewportWidth/viewportHeight), nearCull, farCull);
		} else {
			projectionMatrix = XMMatrixOrthographicLH(viewportWidth, viewportHeight, nearCull, farCull);
		}
		projectionChange = false;
	} 

	return projectionMatrix;
}

float Camera::GetNearClipPlane()
{
	return nearCull;
}

float Camera::GetFarClipPlane()
{
	return farCull;
}

void Camera::Zoom(float zoom)
{
	float mod = ((zoom/120) * (float)D3DX_PI)/180.0f;
	fieldOfView -= mod;
	if (fieldOfView > (float)D3DX_PI/4.0f) {
		fieldOfView = (float)D3DX_PI/4.0f;
	} else if (fieldOfView < (float)D3DX_PI/90.0f) {
		fieldOfView = (float)D3DX_PI/90.0f;
	}
	projectionChange = true;
}