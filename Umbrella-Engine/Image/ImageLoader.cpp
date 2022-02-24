#include <OpenImageIO/filesystem.h>
#include "ImageUtils.h"
#include "ImageLoader.h"


namespace UE::Utils
{

	Scope<Image> ImageLoader::Load( const system::FilePath& InPath, bool vertical_flip )
	{
		auto imInput = OIIO::ImageInput::open( InPath.string() );
		
		if ( !imInput )
		{
			return nullptr;
		}

		auto imSpec = imInput->spec();

		// #todo make it out
		JF_ASSERT( imSpec.tile_width == 0, "No support for tiled images for now." );


		auto image = MakeScoped< Image >( VectorUInt2{ imSpec.width, imSpec.height }, Details::ToImageDataType( imSpec ) );

		if ( !imInput->read_image( imSpec.format, image->RawData() ) )
		{
			return nullptr;
		}

		imInput->close();

		image->MarkInitialized();

		if ( vertical_flip )
		{
			auto flipped = MakeScoped< Image >();

			/*
				#todo	Couldn't find out how to read image from bottom to top efficiently using OIIO.
						The approach below works but it is extremely slow.


				SIZE_T scanlineBytesLength = SIZE_T(image->GetWidth()) * image->GetChannelsCount() * image->GetBytesPerChannel();
				for (SIZE_T y = 0; y < image->GetHeight(); ++y)
				{
					imInput->read_scanline(image->GetHeight() - y - 1, 0, imSpec.format, image->RawData() + y * scanlineBytesLength);
				}

			*/

			// resources of moved image should get released in VerticalFlip function.
			ImageUtils::VerticalFlip( std::move( image ), *flipped );
		
			return flipped;
		}
	

		return image;
	}


	Scope< Image > ImageLoader::LoadFromMemory( const std::string& InFileName, CMemPtr InSource, SIZE_T InSize, bool vertical_flip )
	{
		auto ioMemReader = IOMemoryProxy( const_cast< MemPtr >( InSource ), InSize );
		auto imInput = OIIO::ImageInput::open( InFileName, nullptr, &ioMemReader );
		
		if ( !imInput )
		{
			return nullptr;
		}

		auto imSpec = imInput->spec();

		auto image = MakeScoped< Image >( VectorUInt2{ imSpec.width, imSpec.height }, Details::ToImageDataType( imSpec ) );

		if ( !imInput->read_image( imSpec.format, image->RawData() ) )
		{
			return nullptr;
		}

		imInput->close();

		image->MarkInitialized();

		return image;
	}

	Scope< Image > ImageLoader::LoadFromMemory( const ImageLoadMetaData& InData, CMemPtr InSource )
	{
		return MakeScoped< Image >( reinterpret_cast< const byte* >( InSource ), InData.SizeX, InData.SizeY, InData.ImageFormat );
	}

	bool ImageLoader::Save( const system::FilePath& InPath, Ref< Image > InImage, ERawImageFormat InFormat )
	{
		const std::string PathString = InPath.string();

		auto imOutput = OIIO::ImageOutput::create( PathString );
		auto imSpec = OIIO::ImageSpec( InImage->GetSize().x,
									   InImage->GetSize().y,
									   InImage->GetChannelsCount(),
									   Details::ToOIIOImageDataType( InImage->GetFormat() ) );
		
		if ( !imOutput )
		{
			return false;
		}

		if ( !imOutput->open( PathString, imSpec ) )
		{
			std::cout << imOutput->geterror() << '\n';
			return false;
		}


		return 
			imOutput->write_image( ( InFormat == ERawImageFormat::AUTO )
									 ? imSpec.format
									 : Details::ToOIIOImageDataType( InFormat ),
									 InImage->RawData() );
	}
}
