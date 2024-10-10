#include "Transform.h"

#include "Globals.h"

using namespace LiveWireRemake;
using namespace BaghdadCore;

Transform::Transform() :
	position(DirectX::XMVectorSet(0,0,0,0)),
	rotation(DirectX::XMVectorSet(0, 0, 0, 0)),
	scale(DirectX::XMVectorSet(1, 1, 1, 1))
{}
