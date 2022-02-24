#pragma once
#include "OIIOUtils.h"
#include "Image.h"


namespace UE::Utils
{

	class ImageLoader
	{
	public:

		struct ImageLoadMetaData
		{
			uint32 SizeX;

			uint32 SizeY;

			ERawImageFormat ImageFormat;
		};

	public:

		static Scope< Image > Load( const system::FilePath& InPath, bool vertical_flip = false );

		static Scope< Image > LoadFromMemory( const String& InFileName, CMemPtr InSource, SIZE_T InSize, bool vertical_flip = false );

		/**
		 * Create an image with a given image meta data from the given source of memory.
		 * 
		 * \param InData	- The image meta data that contains information about image format, size and channels.
		 * \param InSource	- The source of pixels.
		 * \return			- Pointer to a newly constructed Image object or nullptr if an error occurred.
		 */
		static Scope< Image > LoadFromMemory( const ImageLoadMetaData& InData, CMemPtr InSource );

		static bool			Save( const system::FilePath& InPath, Ref< Image > InImage, ERawImageFormat InFormat = ERawImageFormat::AUTO );


	private:

// #todo: MAKE CUSTOM PROXY SUPPORT
#if JF_SUPPORT_OIIO_CUSTOM_PROXY
		using IOFileProxy = OIIO::Filesystem::IOFileSTLProxy;
#else
		using IOFileProxy = OIIO::Filesystem::IOFile;
		using IOMemoryProxy = OIIO::Filesystem::IOMemReader;
#endif

	};

}
