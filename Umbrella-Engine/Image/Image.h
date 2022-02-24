#pragma once

#include "../Core.h"
#include "../Math/Math.h"




namespace UE::Utils
{
	using namespace UE::Math;
	
	class Image;
	using ImagePtr = Ref< Image >;


	// Pixel format
	enum class ERawImageFormat
	{
		L8,
		LA8,
		
		R8,
		RG8,
		RGB8,
		RGBA8,

		RF,
		RGBF,
		RGBAF,

		RH,
		RGBH,
		RGBAH,

		AUTO,
	};


	/**
	 * 2D Image class.
	 */
	class Image : std::enable_shared_from_this< Image >
	{
	private:

		/**
		 * Image data.
		 * 
		 * #todo Should use custom allocator (memory pool) ??
		 */
		JVector< byte >	Source;

		/* Image width. */
		uint32			SizeX;

		/* Image height. */
		uint32			SizeY;

		/* Number of channels. */
		uint32			ChannelsCount;

		/* Number of bytes each channel takes. */
		uint8			BytesPerChannel;

		/* Image format. */
		ERawImageFormat Format;

		/* Does Source contain valid data ? */
		bool			bInitialized;


	public:
		
		Image();

		/**
		 * Create non-initialized image object with allocated memory of a calculated size.
		 * 
		 * \param InSizeX		- The image width.
		 * \param InSizeY		- The image height.
		 * \param InImageFormat	- The image format.
		 */
		Image( uint32 InSizeX, uint32 InSizeY, ERawImageFormat InImageFormat );

		/**
		 * Create non-initialized image object with allocated memory of a calculated size.
		 * 
		 * \param InSize		- The image size.
		 * \param InImageFormat	- The image format.
		 */
		Image( VectorUInt2 InSize, ERawImageFormat InImageFormat );

		/**
		 * Creates image object initialized with data from [InData, InData + size] range,
		 * where size is equal to the number of bytes that the image of the given format should obtain.
		 * 
		 * 
		 * \param data			- The pointer to image data.
		 * \param InSizeX		- The Image width
		 * \param InSizeY		- The Image height
		 * \param InImageFormat	- The Image format
		 */
		Image( const byte* InData, uint32 InSizeX, uint32 InSizeY, ERawImageFormat InImageFormat );

		/**
		 * Creates image object initialized with data from [InData, InData + size] range,
		 * where size is equal to the number of bytes that the image of the given format should obtain.
		 * 
		 * \param InData		- The pointer to image data.
		 * \param InSize		- The image width and height.
		 * \param InImageFormat	- The image format.
		 */
		Image( const byte* InData, VectorUInt2 InSize, ERawImageFormat InImageFormat );

		Image( const Image& another );

		Image( Image&& another ) NOEXCEPT;

		Image& operator = ( const Image& another );

		Image& operator = ( Image&& another ) NOEXCEPT;

		~Image();

		
	public:

		/**
		 * Releases all the obtained memory.
		 */
		void Release();

		
		/* #todo Implement SetPixel, GetPixel */

		/**
		 * Sets .
		 * 
		 * \param Data
		 * \param Size
		 */
		void SetData( byte* Data, SIZE_T Size );

		void MarkInitialized( bool initialized = true );

		void PrintImageMetaData( std::ostream& os );

	public:
		
		bool			IsInitialized() const;

		uint32			GetBytesPerPixel() const;

		VectorUInt2		GetSize() const;

		uint32			GetWidth() const;

		uint32			GetHeight() const;

		uint32			GetBytesSize() const;

		uint32			GetChannelsCount() const;

		uint8			GetBytesPerChannel() const;	// GetBytesPerPixel() / GetChannelsCount();

		ERawImageFormat	GetFormat() const;

		byte*			RawData();

		const byte*		RawData() const;

		// Convenience accessors for raw data

		std::span<byte>						RawView();

		std::span<uint8>					AsL8();

		std::span<uint8>					AsR8();

		std::span<uint16>					AsLA8();

		std::span<float16>					AsRH();

		std::span<uint8>					AsRGB8();

		std::span<Color>					AsRGBA8();

		std::span<float>					AsRF();

		std::span<float16>					AsRGBH();

		std::span<float16>					AsRGBAH();

		std::span<float>					AsRGBF();

		std::span<LinearColor>				AsRGBAF();


		// Convenience accessors for const raw data

		std::span<const byte>				RawView() const;

		std::span<const uint8>				AsL8() const;

		std::span<const uint8>				AsR8() const;

		std::span<const uint16>				AsLA8() const;

		std::span<const float16>			AsRH() const;

		std::span<const uint8>				AsRGB8() const;

		std::span<const Color>				AsRGBA8() const;

		std::span<const float>				AsRF() const;

		std::span<const float16>			AsRGBH() const;

		std::span<const float16>			AsRGBAH() const;

		std::span<const float>				AsRGBF() const;

		std::span<const LinearColor>		AsRGBAF() const;

	};

}