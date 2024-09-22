#include "ConstantBuffer.h"

#include <memory>

using namespace BaghdadCore;

ConstantBuffer::ConstantBuffer(Buffer&& buffer) :
	Buffer(std::move(buffer))
{
	// TODO: ASSERTION HERE FOR THE BUFFER TYPE.
}
