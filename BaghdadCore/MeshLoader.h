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
		/// <summary>
		/// Creates or loads the mesh.
		/// </summary>
		/// <returns>The Loaded mesh</returns>
		/// <exception cref="BaghdadError">On loading failed</exception>
		/// <exception cref="GraphicsError">On Creating buffers failed></exception>
		Mesh Load();
		MeshLoader& Clear() noexcept;

		MeshLoader& FromFile(const std::string& name) noexcept;
		MeshLoader& PrimitiveQuad() noexcept;
		MeshLoader& PrimitiveTriangle() noexcept;
		MeshLoader& PrimitiveCube() noexcept;

		MeshLoader(const Device& device);

	private:
		const Device& _device;

		std::string _name;
		bool _primitiveQuad = false;
		bool _primitiveTriangle = false;
		bool _primitiveCube = false;

		const Logger& _logger;
	};
}

