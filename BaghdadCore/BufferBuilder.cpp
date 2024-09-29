#include "BufferBuilder.h"

#include "Globals.h"

using namespace BaghdadCore;

BufferBuilder::BufferBuilder(const Device& device) :
	_device(device),
	_logger(Globals::GetLogger())
{}
