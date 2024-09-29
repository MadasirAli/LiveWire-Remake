#pragma once

#include <string>

#include "Mesh.h"
#include "Device.h"	
#include "Logger.h"

namespace BaghdadCore
{
	class MeshLoader final
	{
	private:
		class STLLoader final
		{
		public:
			/// <summary>
			/// Loads the file to construct vertices.
			/// </summary>
			/// <param name="name">name of the file to load</param>
			/// <returns>loaded vertices</returns>
			/// <exception cref="BaghdadError">On Failture</exception>
			std::vector<Mesh::vertex> Load(const std::string& name) const;
		};
	public:
		/// <summary>
		/// Creates or loads the mesh.
		/// </summary>
		/// <returns>The Loaded mesh</returns>
		/// <exception cref="BaghdadError">On loading failed</exception>
		/// <exception cref="GraphicsError">On Creating buffers failed></exception>
		Mesh Load();

		MeshLoader& Load(const std::string& name) noexcept;

		MeshLoader(const Device& device);

	private:
		const Device& _device;

		std::string _name;

		STLLoader _stlLoader;
		const Logger& _logger;
	};
}

