#include "VertexBuffer.h"

#include <memory>

using namespace BaghdadCore;

VertexBuffer::VertexBuffer(Buffer&& buffer) :
	Buffer(std::move(buffer))
{
	// TODO: ASSERTION HERE FOR THE BUFFER TYPE.
}
