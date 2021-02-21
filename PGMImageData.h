#pragma once
#include "Utils.h"
#include <vector>
#include <fstream>

struct ImageSize
{
	int width = 0;
	int height = 0;

	ImageSize() = default;
	ImageSize( int w, int h )
	{
		width = w;
		height = h;
	}
};
class PGMImageData
{
	public:
		PGMImageData( std::ifstream& image_file );
		PGMImageData( std::vector<std::uint32_t> imageData, int width, int height, int maxValue );

		~PGMImageData() = default;

		const std::vector<std::uint32_t>& GetImageData() const { return m_ImageData; }
		int GetWidth() const { return m_Size.width; }
		int GetHeight() const { return m_Size.height;  }
		
		bool IsValid() const { return m_IsValid; }

		void WriteToFile( std::ofstream& outputFile );

		void AddOverlay( const PGMImageData& image, OverlayOperationType type );

		void OverlayOperation( std::uint32_t& mainImagePixel, std::uint32_t& overlayImagePixel, OverlayOperationType type );

	private:
		std::vector<std::uint32_t> m_ImageData;
		ImageSize m_Size;
		int m_MaxValue = 0;
		bool m_IsValid = false;
};