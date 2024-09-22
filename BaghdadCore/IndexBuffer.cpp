#include "IndexBuffer.h"

#include <memory>

using namespace BaghdadCore;

IndexBuffer::IndexBuffer(Buffer&& buffer) :
	Buffer(std::move(buffer))
{
	// TODO: ASSERTION HERE FOR THE BUFFER TYPE.
}
