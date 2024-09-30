#pragma once

#include <string>

#include "Device.h"
#include "Texture2D.h"
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

				PPMFile() = default;
				PPMFile(
					const Header& header, std::unique_ptr<char[]>&& pFile,
					const unsigned int pixelOffset);

			private:
				Header _header;
				std::unique_ptr<char[]> _pPixels;
				std::unique_ptr<char[]> _pFile;
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
		/// <summary>
		/// Creates the specified texture.
		/// </summary>
		/// <returns>The created texture.</returns>
		/// <exception cref="BaghdadError">On Failture</exception>
		/// <exception cref="Graphics Error">On Failture</exception>
		Texture2D Build();

		TextureBuilder& Clear() noexcept;
		TextureBuilder& ViewFlag(Resource::View::Type viewFlag) noexcept;
		TextureBuilder& Size(const unsigned int width, const unsigned int height) noexcept;
		TextureBuilder& InitialData(const char* const pData);
		TextureBuilder& Format(const DXGI_FORMAT format);
		TextureBuilder& ReadWrite() noexcept;
		TextureBuilder& RenderTexture() noexcept;
		TextureBuilder& FromFile(const std::string& name) noexcept;

		TextureBuilder(const Device& device);

	private:
		const Device& _device;

		std::string _name;

		bool _fromFile = false;
		bool _readWrite = false;
		bool _renderTexture = false;

		unsigned int _width = 0u;
		unsigned int _height = 0u;

		const char* _pData = nullptr;
		DXGI_FORMAT _format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

		unsigned int _viewFlags = 0u;

		PPMLoader _ppmLoader;
		const Logger& _logger;
	};
}