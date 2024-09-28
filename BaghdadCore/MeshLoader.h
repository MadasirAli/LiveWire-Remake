#pragma once

#include <string>

#include "Mesh.h"
#include "Device.h"	
#include "Logger.h"

namespace BaghdadCore
{
	class MeshLoader final
	{
	public:
		Mesh Create();

		MeshLoader& AllowReadWrite() noexcept;
		MeshLoader& Load(const std::string& name) noexcept;

		MeshLoader(const Device& device);

	private:
		const Device& _device;

		std::string _name;
		bool _readWrite;

		Logger _logger;
	};
}

