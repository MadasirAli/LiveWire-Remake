#pragma once

#include <string>

#include "Device.h"
#include "Texture.h"
#include "Logger.h"

namespace BaghdadCore
{
	class TextureBuilder final
	{
	private:
		class PPMLoader final
		{
		private:
			struct Header final
			{
				wchar_t magic;
				unsigned short width;
				unsigned short height;
				char depth;
			};
		public:
			class PPMFile final
			{
			public:
				const Header& GetHeader() const noexcept;
				const std::unique_ptr<char[]>& GetBufferpPtr() const noexcept;

				PPMFile(
					const Header& header, std::unique_ptr<char[]>&& pFile,
					const unsigned int pixelOffset);

			private:
				Header _header;
				std::unique_ptr<char[]> _pFile;
				std::unique_ptr<char[]> _pPixels;
			};
		public:
			/// <summary>
			/// Loads the tgs specified tga file
			/// </summary>
			/// <param name="name">file name</param>
			/// <exception cref="BaghdadError">On Failture.</exception>
			/// <returns>PPM File</returns>
			PPMFile Load(const std::string& name) const;
		};

	public:
		Texture Build();
		TextureBuilder& FromFile(const std::string& name) noexcept;

		TextureBuilder(const Device& device);

	private:
		const Device& _device;

		std::string _name;
		const Logger& _logger;
	};
}