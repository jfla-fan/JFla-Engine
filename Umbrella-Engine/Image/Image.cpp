#include "Image.h"
#include <map>


namespace UE::Utils
{
	using namespace UE::Math;

	static uint8 _GetBytesPerChannel( ERawImageFormat InFormat )
	{
		switch ( InFormat )
		{
		case ERawImageFormat::L8:
		case ERawImageFormat::LA8:
		case ERawImageFormat::R8:
		case ERawImageFormat::RGB8:
		case ERawImageFormat::RGBA8:
			return 1;

		case ERawImageFormat::RH:
		case ERawImageFormat::RG8:
		case ERawImageFormat::RGBH:
		case ERawImageFormat::RGBAH:
			return 2;

		case ERawImageFormat::RF:
		case ERawImageFormat::RGBF:
		case ERawImageFormat::RGBAF:
			return 4;
		
		default:
			throw "Cannot define bytes per channel";
			break;
		}

		return 0;	// should never reach this
	}

	static uint32 _GetChannelsCount( ERawImageFormat InFormat )
	{
		switch ( InFormat )
		{
		case ERawImageFormat::L8:
		case ERawImageFormat::R8:
		case ERawImageFormat::RF:
		case ERawImageFormat::RH:
			return 1;

		case ERawImageFormat::LA8:
		case ERawImageFormat::RG8:
			return 2;

		case ERawImageFormat::RGB8:
		case ERawImageFormat::RGBF:
		case ERawImageFormat::RGBH:
			return 3;

		case ERawImageFormat::RGBA8:
		case ERawImageFormat::RGBAH:
		case ERawImageFormat::RGBAF:
			return 4;

		default:
			throw "Unknown image format";
			break;
		}

		return 0;	// should never reach this
	}

	static std::string _GetImageFormatString( ERawImageFormat format )
	{
#define CASE_LABEL( format_type )\
		case ERawImageFormat::format_type:\
			return #format_type

		switch ( format )
		{
			CASE_LABEL( L8 );
			CASE_LABEL( LA8 );
			CASE_LABEL( R8 );
			CASE_LABEL( RG8 );
			CASE_LABEL( RGB8 );
			CASE_LABEL( RGBA8 );
			CASE_LABEL( RF );
			CASE_LABEL( RGBF );
			CASE_LABEL( RGBAF );
			CASE_LABEL( RH );
			CASE_LABEL( RGBH );
			CASE_LABEL( RGBAH );
		default:
			return "Unknown";
		}

#undef CASE_LABEL
	}


	Image::Image()
		: SizeX( 0 )
		, SizeY( 0 )
		, ChannelsCount( 0 )
		, BytesPerChannel( 0 )
		, Format( ERawImageFormat::AUTO )
		, bInitialized( false )
	{
	}

	Image::Image( uint32 InSizeX, uint32 InSizeY, ERawImageFormat InImageFormat )
		: SizeX( InSizeX )
		, SizeY( InSizeY )
		, ChannelsCount( _GetChannelsCount( InImageFormat ) )
		, BytesPerChannel( _GetBytesPerChannel( InImageFormat ) )
		, Format( InImageFormat )
		, bInitialized( false )
	{
		Source.assign( ( SIZE_T )SizeX * SizeY * ChannelsCount * GetBytesPerPixel(), byte( 0x00 ) );
	}

	Image::Image( VectorUInt2 InSize, ERawImageFormat InImageFormat )
		: Image( InSize.x, InSize.y, InImageFormat )
	{
	}

	Image::Image( const byte* InData, uint32 InSizeX, uint32 InSizeY, ERawImageFormat InImageFormat )
		: Image( InSizeX, InSizeY, InImageFormat )
	{
		Source.assign( InData, InData + Source.size() );
		bInitialized = true;
	}

	Image::Image( const byte* InData, VectorUInt2 InSize, ERawImageFormat InImageFormat )
		: Image( InData, InSize.x, InSize.y, InImageFormat )
	{
	}

	Image::Image( const Image& another )
	{
		this->Source			= another.Source;
		this->SizeX				= another.SizeX;
		this->SizeY				= another.SizeY;
		this->ChannelsCount		= another.ChannelsCount;
		this->BytesPerChannel	= another.BytesPerChannel;
		this->Format			= another.Format;
		this->bInitialized		= another.bInitialized;
	}

	Image::Image( Image&& another ) NOEXCEPT
	{
		this->Source			= std::move(another.Source);
		this->SizeX				= another.SizeX;
		this->SizeY				= another.SizeY;
		this->ChannelsCount		= another.ChannelsCount;
		this->BytesPerChannel	= another.BytesPerChannel;
		this->Format			= another.Format;
		this->bInitialized	= another.bInitialized;

		another.SizeX			= 0;
		another.SizeY			= 0;
		another.ChannelsCount	= 0;
		another.BytesPerChannel = 0;
		another.bInitialized	= false;
	}

	Image& Image::operator = ( const Image& another )
	{
		if ( this == &another )
		{
			return *this;
		}


		this->Source = another.Source;
		this->SizeX = another.SizeX;
		this->SizeY = another.SizeY;
		this->ChannelsCount = another.ChannelsCount;
		this->BytesPerChannel = another.BytesPerChannel;
		this->Format = another.Format;
		this->bInitialized = another.bInitialized;

		return *this;
	}

	Image& Image::operator = ( Image&& another ) NOEXCEPT
	{
		this->Source			= std::move( another.Source );
		this->SizeX				= another.SizeX;
		this->SizeY				= another.SizeY;
		this->ChannelsCount		= another.ChannelsCount;
		this->BytesPerChannel	= another.BytesPerChannel;
		this->Format			= another.Format;
		this->bInitialized		= another.bInitialized;

		another.SizeX			= 0;
		another.SizeY			= 0;
		another.ChannelsCount	= 0;
		another.BytesPerChannel = 0;
		another.bInitialized	= false;

		return *this;
	}

	Image::~Image() { Release(); }

	void Image::Release()
	{
		JVector< byte >().swap( Source );	// clears and releases vector resources
		bInitialized = false;
	}

	void Image::SetData( byte* Data, SIZE_T Size )
	{
		Release();

		Source = JVector< byte >( Data, Data + Size );
	}

	void Image::MarkInitialized( bool initialized )
	{
		this->bInitialized = initialized;
	}

	void Image::PrintImageMetaData( std::ostream& os )
	{
		os << std::format( "Size - ({}, {})\n", SizeX, SizeY )
			<< "Channel count: " << ChannelsCount << '\n'
			<< "Bytes per channel: " << ( uint32 )BytesPerChannel << '\n'
			<< _GetImageFormatString( Format ) << '\n';
	}

	bool Image::IsInitialized() const { return bInitialized; }


	uint32 Image::GetBytesPerPixel() const
	{
		switch ( this->Format )
		{
			case ERawImageFormat::L8:
			case ERawImageFormat::R8:
				return 1;

			case ERawImageFormat::LA8:
			case ERawImageFormat::RG8:
			case ERawImageFormat::RH:
				return 2;

			case ERawImageFormat::RGB8:
				return 3;

			case ERawImageFormat::RGBA8:
			case ERawImageFormat::RF:
				return 4;

			case ERawImageFormat::RGBH:
				return 6;

			case ERawImageFormat::RGBAH:
				return 8;

			case ERawImageFormat::RGBF:
				return 12;

			case ERawImageFormat::RGBAF:
				return 16;

			default:
				// todo: warning or fatal assert: Unsupported file format
				break;
		}

		return 0;	// should never reach this
	}

	VectorUInt2		Image::GetSize() const { return { SizeX, SizeY }; }

	uint32			Image::GetWidth() const { return SizeX; }

	uint32			Image::GetHeight() const { return SizeY; }

	uint32			Image::GetBytesSize() const { return ( uint32 )Source.size(); }

	uint32			Image::GetChannelsCount() const { return ChannelsCount; }

	uint8			Image::GetBytesPerChannel() const { return BytesPerChannel; }

	ERawImageFormat	Image::GetFormat() const { return Format; }

	byte*			Image::RawData() { return Source.data(); }

	const byte*		Image::RawData() const { return Source.data(); }

	// data accessors

	std::span< byte >		Image::RawView()
	{
		return std::span< byte >( this->Source );
	}

	std::span< uint8 >		Image::AsL8()
	{
		check( this->Format == ERawImageFormat::L8 );
		return std::span( ( uint8* )this->Source.data(), this->Source.size() / sizeof( uint8 ) );
	}
					  
	std::span< uint8 >		Image::AsR8()
	{
		check( this->Format == ERawImageFormat::R8 );
		return std::span( ( uint8* )this->Source.data(), this->Source.size() / sizeof( uint8 ) );
	}

	std::span< uint16 >		Image::AsLA8()
	{
		check( this->Format == ERawImageFormat::LA8 );
		return std::span( ( uint16* )this->Source.data(), this->Source.size() / sizeof( uint16 ) );
	}

	std::span< float16 >		Image::AsRH()
	{
		check( this->Format == ERawImageFormat::RH );
		return std::span( ( float16* )this->Source.data(), this->Source.size() / sizeof( float16 ) );
	}

	std::span<uint8>		Image::AsRGB8()
	{
		check( this->Format == ERawImageFormat::RGB8 );
		return std::span( ( uint8* )this->Source.data(), this->Source.size() / sizeof( uint8 ) );
	}

	std::span< Color >		Image::AsRGBA8()
	{
		check( this->Format == ERawImageFormat::RGBA8 );
		return std::span( ( Color* )this->Source.data(), this->Source.size() / sizeof( Color ) );
	}

	std::span< float >		Image::AsRF()
	{
		check(this->Format == ERawImageFormat::RF );
		return std::span((float*)this->Source.data(), this->Source.size() / sizeof( float ) );
	}

	std::span< float16 >		Image::AsRGBH()
	{
		check( this->Format == ERawImageFormat::RGBH );
		return std::span( ( float16* )this->Source.data(), this->Source.size() / sizeof( float16 ) );
	}

	std::span< float16 >		Image::AsRGBAH()
	{
		check( this->Format == ERawImageFormat::RGBAH );
		return std::span( ( float16* )this->Source.data(), this->Source.size() / sizeof( float16 ) );
	}

	std::span< float >		Image::AsRGBF()
	{
		check( this->Format == ERawImageFormat::RGBF );
		return std::span( ( float* )this->Source.data(), this->Source.size() / sizeof( float ) );
	}

	std::span<LinearColor>	Image::AsRGBAF()
	{
		check( this->Format == ERawImageFormat::RGBAF );
		return std::span( ( LinearColor* )this->Source.data(), this->Source.size() / sizeof( LinearColor ) );
	}

	// const data accessors

	std::span< const byte >			Image::RawView() const
	{
		return std::span< const byte >( this->Source );
	}

	std::span< const uint8 >			Image::AsL8() const 
	{
		check(this->Format == ERawImageFormat::L8);
		return std::span((const uint8*)this->Source.data(), this->Source.size() / sizeof(uint8));
	}

	std::span<const uint8>			Image::AsR8() const 
	{
		check(this->Format == ERawImageFormat::R8);
		return std::span((const uint8*)this->Source.data(), this->Source.size() / sizeof(uint8));
	}

	std::span<const uint16>			Image::AsLA8() const
	{
		check(this->Format == ERawImageFormat::LA8);
		return std::span((const uint16*)this->Source.data(), this->Source.size() / sizeof(uint16));
	}

	std::span<const float16>		Image::AsRH() const
	{
		check(this->Format == ERawImageFormat::RH);
		return std::span((const float16*)this->Source.data(), this->Source.size() / sizeof(float16));
	}

	std::span<const uint8>			Image::AsRGB8() const
	{
		check(this->Format == ERawImageFormat::RGB8);
		return std::span((const uint8*)this->Source.data(), this->Source.size() / sizeof(uint8));
	}

	std::span<const Color>			Image::AsRGBA8() const
	{
		check(this->Format == ERawImageFormat::RGBA8);
		return std::span((const Color*)this->Source.data(), this->Source.size() / sizeof(Color));
	}

	std::span<const float>			Image::AsRF() const
	{
		check(this->Format == ERawImageFormat::RF);
		return std::span((const float*)this->Source.data(), this->Source.size() / sizeof(float));
	}

	std::span<const float16>		Image::AsRGBH() const
	{
		check(this->Format == ERawImageFormat::RGBH);
		return std::span((const float16*)this->Source.data(), this->Source.size() / sizeof(float16));
	}

	std::span<const float16>		Image::AsRGBAH() const
	{
		check(this->Format == ERawImageFormat::RGBAH);
		return std::span((const float16*)this->Source.data(), this->Source.size() / sizeof(float16));
	}

	std::span<const float>			Image::AsRGBF() const
	{
		check(this->Format == ERawImageFormat::RGBF);
		return std::span((const float*)this->Source.data(), this->Source.size() / sizeof(float));
	}

	std::span<const LinearColor>	Image::AsRGBAF() const
	{
		check(this->Format == ERawImageFormat::RGBAF);
		return std::span((const LinearColor*)this->Source.data(), this->Source.size() / sizeof(LinearColor));
	}

}
