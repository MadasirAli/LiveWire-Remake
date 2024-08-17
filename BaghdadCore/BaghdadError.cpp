#include "BaghdadError.h"


using namespace BaghdadCore;

BaghdadError::BaghdadError(std::string msg) :
	runtime_error(msg) {}