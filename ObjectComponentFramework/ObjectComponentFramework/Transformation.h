#pragma once


#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <process.h>
#include <ppl.h>


class Transformation : public rabd::Component<Transformation>
{
public:
	Transformation();
	Transformation(Transformation&);
	~Transformation();

	const D3DXMATRIX& GetTransformation();
	void SetChanged();
	inline bool ViewHasChanged(){return viewChanged;}
	inline void SetViewUnchanged(){viewChanged = false;}
private:
	void CalculateLocalTransformation();
	void CalculateGlobalTransformation();
	void SetGlobalChanged();

	Concurrency::reader_writer_lock* changedLock;
	bool localChanged;
	bool globalChanged;
	bool viewChanged;

	Concurrency::reader_writer_lock* localLock;
	D3DXMATRIX localTransform;
	Concurrency::reader_writer_lock* globalLock;
	D3DXMATRIX globalTransform;
};