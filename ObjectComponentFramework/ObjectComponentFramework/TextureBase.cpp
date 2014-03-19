#include "TextureBase.h"

namespace rabd
{
	Manager* TextureBase::manager = nullptr;
	ID3D11Device* TextureBase::device = nullptr;
	int TextureBase::idCounter = 0;
};