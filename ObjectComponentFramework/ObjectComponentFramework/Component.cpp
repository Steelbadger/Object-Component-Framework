#include "Component.h"



ComponentType ComponentBase::componentCount = 0;

ComponentBase::ComponentBase()
{
}

ComponentBase::~ComponentBase()
{

}

ComponentType ComponentBase::GetNumberOfComponents()
{
	return componentCount;
}
