#pragma once

#include <string>

#include "Device.h"
#include "Texture.h"

namespace BaghdadCore
{
	class TextureBuilder final
	{
	private:
		class TGALoader final
		{
		private:
			class Header final
			{

			};
		public:
			class TGAFile final
			{

			};
		public:
			TGAFile Load(const std::string& name) const;
		};

	public:
		Texture Build();
		TextureBuilder& FromFile(const std::string& name) noexcept;

		TextureBuilder(const Device& device);

	private:
		const Device& _device;

		std::string _name;
	};
}

