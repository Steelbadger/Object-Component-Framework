#include "Camera.h"
#include "GameObject.h"
#include "Position.h"
#include "Orientation.h"



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

D3DXMATRIX Camera::GetViewMatrix()
{
	if (GameObject::Get(GetParentID()).Modified()) {
		D3DXVECTOR3 up(0,1,0);
		D3DXVECTOR3 forward(0,0,1);
		D3DXVECTOR3 position = GameObject::GetComponent<Position>(GetParentID()).GetPosition();
		D3DXMATRIX orientationMatrix = GameObject::GetComponent<Orientation>(GetParentID()).GetMatrix();

		D3DXVec3TransformCoord(&forward, &forward, &orientationMatrix);
		D3DXVec3TransformCoord(&up, &up, &orientationMatrix);

		D3DXVECTOR3 lookAt = position + forward;

		GameObject::Get(GetParentID()).GetLocalMatrix();

		// Finally create the view matrix from the three updated vectors.
		D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
	}
	return viewMatrix;
}


D3DXMATRIX Camera::GetProjectionMatrix()
{
	if (projectionChange == true) {
		if (perspective) {
			D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, (viewportWidth/viewportHeight), nearCull, farCull);		
		} else {
			D3DXMatrixOrthoLH(&projectionMatrix, viewportWidth, viewportHeight, nearCull, farCull);
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