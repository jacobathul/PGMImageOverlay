#include "PGMImageData.h"


#include "Utils.h"

#include <iostream>
#include<string>
#include <sstream>

PGMImageData::PGMImageData( std::ifstream& image_file )
{
	std::string parsedLine;
	std::string format;

	std::getline( image_file, format );

	std::vector<std::string> formatParsed = GetParsedDataToString( format );

	std::getline( image_file, parsedLine );

	//check for optional section
	if ( parsedLine[0] == '#' ) 
	{
		std::getline( image_file, parsedLine );
	}

	std::vector<int> size = GetParsedData( parsedLine );

	if ( size.size() != 2 )
	{
		std::cout << "Error reading size value cant continue";
		return;
	}

	m_Size.width = size.at( 0 );
	m_Size.height = size.at( 1 );

	//we want to reserve this to ensure that avoid vector scaling and allocating memory
	m_ImageData.reserve( static_cast<double>(m_Size.width) * m_Size.height );

	std::getline( image_file, parsedLine );

	std::vector<int> maxValue = GetParsedData( parsedLine );

	if ( maxValue.size() != 1 && maxValue.at(0) < 256 && maxValue.at(0) > 0 )
	{
		std::cout << "Error reading max value cant continue\n";
		return;
	}

	m_MaxValue = maxValue.at( 0 );

	if ( formatParsed.size() != 1 )
	{
		std::cout << "Error in format";
		return;
	}

	if ( formatParsed.at( 0 ) == "P2" )
	{
		std::uint32_t val;
		int i = 0;
		while ( !image_file.eof() )
		{
			image_file >> val;
			if ( image_file.eof() )
				break;
			m_ImageData.push_back( ( val* ( 255/ m_MaxValue) ) ); //Convert the image to standard 255 max value
			i++;

		}
		int value = m_ImageData.back();
	}
	else if ( formatParsed.at( 0 ) == "P5" )
	{
		std::vector<unsigned char> buffer( std::istreambuf_iterator<char>( image_file ), {} );

		for ( auto& b : buffer )
		{
			m_ImageData.push_back( static_cast<std::uint32_t>(b) );
		}
	}
	else
	{
		std::cout << "Cannot recognize format\n";
		return;
	}

	if ( m_ImageData.size() != m_Size.height * m_Size.width )
	{
		std::cout << "Image data do not match width and height\n";
		return;
	}

	m_IsValid = true;
}

PGMImageData::PGMImageData( std::vector<std::uint32_t> imageData, int width, int height, int maxValue ):
	m_ImageData( std::move( imageData ) ),
	m_Size( width, height),
	m_MaxValue( maxValue )
{
	if ( m_ImageData.size() != m_Size.height * m_Size.width )
	{
		std::cout << "Image data do not match width and height\n";
		return;
	}

	m_IsValid = true;

}
void PGMImageData::AddOverlay( const PGMImageData& image , OverlayOperationType type )
{
	if ( image.IsValid() )
	{
		std::vector<std::uint32_t> imageDataOverlay = image.GetImageData();

		int widthOverlay = image.GetWidth();
		int heightOverlay = image.GetHeight();

		//This program assumes the image is overlapped with a shared center
		//The overlapping rectangle is iterated over and updated with new data depending on the overlay operation type. 
		if ( m_Size.width >= image.GetWidth() )
		{
			if ( m_Size.height >= image.GetHeight() )
			{
				//case 1: overlay image's width and height are less than main image
				int overlayItr = 0;
				for ( int y = ( m_Size.height / 2 ) - ( heightOverlay / 2 ); y < ( m_Size.height / 2 ) + ( heightOverlay / 2 ); y++ )
				{
					for ( int x = ( m_Size.width / 2 ) - ( widthOverlay / 2 ); x < ( m_Size.width / 2 ) + ( widthOverlay / 2 ); x++ )
					{
						OverlayOperation( m_ImageData[x + (y)*m_Size.width], imageDataOverlay[overlayItr], type );
						overlayItr++;
					}
				}
			}
			else
			{
				//case 2 : overlay image's height is greater than main image
				for ( int y = 0; y < m_Size.height; y++ )
				{
					int overlayRowItr = 0;

					for ( int x = (m_Size.width / 2) - (widthOverlay / 2); x < (m_Size.width / 2) + (widthOverlay / 2); x++ )
					{
						OverlayOperation(
							m_ImageData[x + (y)*m_Size.width],
							imageDataOverlay[overlayRowItr + (heightOverlay / 2 - m_Size.height / 2 + y) * widthOverlay],
							type );
						overlayRowItr++;
					}
				}
			}
		}
		else
		{
			if ( m_Size.height < image.GetHeight() )
			{
				//case 3: overlay image's width and height are greater than main image ( reverse of case 1 )
				int mainImageItr = 0;

				for ( int y = (heightOverlay / 2) - (m_Size.height / 2); y < (heightOverlay / 2) + (m_Size.height / 2); y++ )
				{
					for ( int x = (widthOverlay / 2) - (m_Size.width / 2); x < (widthOverlay / 2) + (m_Size.width / 2); x++ )
					{
						OverlayOperation( m_ImageData[mainImageItr++], imageDataOverlay[x + (y)*widthOverlay], type );
					}
				}
			}
			else
			{
				//case 4: overlay image's width is greater than main image
				int overlayColumnItr = 0;

				for ( int y = (m_Size.height / 2) - (heightOverlay / 2); y < (m_Size.height / 2) + (heightOverlay / 2); y++ )
				  {
				    for ( int x =0; x < m_Size.width; x++ )
				    {
						OverlayOperation(
							m_ImageData[x + (y)*m_Size.width],
							imageDataOverlay[x + (widthOverlay / 2 - m_Size.width / 2) + (overlayColumnItr)*widthOverlay],
							type );
				    }
					overlayColumnItr++;
				}
			}
		}

	}
}

void PGMImageData::OverlayOperation( std::uint32_t& mainImagePixel, std::uint32_t& overlayImagePixel, OverlayOperationType type )
{
	switch ( type )
	{
	case OverlayOperationType::all:
		mainImagePixel = overlayImagePixel;
		break;
	case OverlayOperationType::lesser:
		if ( mainImagePixel > overlayImagePixel )
		{
			mainImagePixel = overlayImagePixel;
		}
		break;
	case OverlayOperationType::greater:
		if ( mainImagePixel < overlayImagePixel )
		{
			mainImagePixel = overlayImagePixel;
		}
		break;
	case OverlayOperationType::average:
		mainImagePixel = (mainImagePixel + overlayImagePixel) / 2;
		break;
	default:
		break;
	}
}

void PGMImageData::WriteToFile( std::ofstream& outputFile )
{
	if ( outputFile.is_open() && m_IsValid )
	{
		outputFile << "P2\n" << m_Size.width << " " << m_Size.height << "\n" << 255 << "\n";
		for ( int y = 0; y < m_Size.height; y++ )
		{
			for ( int x = 0; x < m_Size.width; x++ )
			{
				outputFile << m_ImageData[x + (y)*m_Size.width] << ' ';
			}
			outputFile << '\n';
		}
		outputFile.close();
	}
	else
	{
		std::cout << "Cannot write to file\n";
	}
}