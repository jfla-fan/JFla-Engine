#pragma once
#include <OpenImageIO/imageio.h>
#include "Image.h"
#include <map>



namespace UE::Utils::Details
{
	/// use as ImageFormatToOIITable[(uint32)ERawImageFormat]
	static OIIO::TypeDesc ImageFormatToOIIOTable[] =
	{

		OIIO::TypeUInt8,	// L8
		OIIO::TypeUInt8,	// LA8
		OIIO::TypeUInt8,	// R8
		OIIO::TypeUInt8,	// RG8
		OIIO::TypeUInt8,	// RGB8
		OIIO::TypeUInt8,	// RGBA8

		OIIO::TypeFloat,	// RF
		OIIO::TypeFloat,	// RGBF
		OIIO::TypeFloat,	// RGBAF

		OIIO::TypeHalf,		// RH
		OIIO::TypeHalf,		// RGBH
		OIIO::TypeHalf,		// RGBAH

		OIIO::TypeUInt8,	// AUTO

	};

	static HashMap< String, ERawImageFormat > StringToImageFormatMap
	{

		{ "L8", ERawImageFormat::L8 },
		{ "LA8", ERawImageFormat::LA8 },

		{ "R8", ERawImageFormat::R8 },
		{ "RG8", ERawImageFormat::RG8 },
		{ "RGB8", ERawImageFormat::RGB8 },
		{ "RGBA8", ERawImageFormat::RGBA8 },

		{ "RF", ERawImageFormat::RF },
		{ "RGBF", ERawImageFormat::RGBF },
		{ "RGBAF", ERawImageFormat::RGBAF },

		{ "RH", ERawImageFormat::RH },
		{ "RGBH", ERawImageFormat::RGBH },
		{ "RGBAH", ERawImageFormat::RGBAH },

		{ "AUTO", ERawImageFormat::AUTO }

	};

	OIIO::TypeDesc ToOIIOImageDataType( ERawImageFormat Format );

	ERawImageFormat ToImageDataType( const OIIO::ImageSpec& Spec );
		
	// others as well
}

namespace UE::Utils {

	class ImageUtils
	{
	public:

		/**
		 * Converts a given image to another format.
		 * 
		 * \param InFrom	- The image to convert from.
		 * \param InFormat	- The format to convert to.
		 */
		static void Convert( Ref< Image > InFrom, ERawImageFormat InFormat );

		/**
		 * Copies a given image into a new one with the specified image data format.
		 * 
		 * \param InFrom	- The image to convert from.
		 * \param InTo		- The image to convert to.
		 * \param InFormat	- The format to convert to.
		 */
		static void Copy( Ref< Image > InFrom, Image& InTo, ERawImageFormat InFormat );

		/**
		 * Resizes the given image using a simple average filter.
		 * 
		 * \param InFrom
		 * \param InDest
		 * \param InDestSize
		 * \param InFormat
		 */
		static void Resize( Ref< Image > InFrom, Image& InDest, VectorUInt2 InDestSize, ERawImageFormat InDestFormat );


		static void VerticalFlip( Ref< Image > InFrom, Image& InDest );

		static void HorizontalFlip( Ref< Image > InFrom, Image& InDest );

		static void Rotate( Ref<Image > InFrom, Image& InDest, float InAngle );

		static void PixelSum( Ref< Image > InFirstOperand, Ref< Image > InSecondOperand, Image& InDest );

		static void PixelSum( Ref< Image > InFirstOperand, JSpan< float > InSecondOperand, Image& InDest );
	};

}

