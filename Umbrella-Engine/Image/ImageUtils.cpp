#include <OpenImageIO/filesystem.h>
#include <OpenImageIO/imagebufalgo.h>
#include "ImageUtils.h"
#include <boost/algorithm/string.hpp>


namespace UE::Utils::Details
{

	OIIO::TypeDesc ToOIIOImageDataType( ERawImageFormat Format )
	{
		return ImageFormatToOIIOTable[static_cast< uint32 >( Format )];
	}

	ERawImageFormat ToImageDataType( const OIIO::ImageSpec& Spec )
	{
		std::string combined = boost::algorithm::join( Spec.channelnames, "" );

		switch ( Spec.format.basetype )
		{
		case OIIO::TypeUInt8.basetype:
		{
			combined += '8';
			break;
		}

		case OIIO::TypeFloat.basetype:
		{
			combined += 'F';
			break;
		}

		case OIIO::TypeHalf.basetype:
		{
			combined += 'H';
			break;
		}

		default:
		{
			throw "Unsupported Image specification format!";
			break;
		}
		}

		return StringToImageFormatMap[combined];
	}
}

namespace UE::Utils 
{

	void ImageUtils::Copy( Ref< Image > InFrom, Image& InTo, ERawImageFormat InFormat )
	{
		auto ImSpec = OIIO::ImageSpec( InFrom->GetSize().x,
									   InFrom->GetSize().y,
									   InFrom->GetChannelsCount(),
									   Details::ToOIIOImageDataType( InFrom->GetFormat() ) );

		auto ImBufSource = OIIO::ImageBuf( ImSpec, InFrom->RawData() );

		auto Result = Image( InFrom->GetSize().x, InFrom->GetSize().y, InFormat );

		auto ImResultSpec = OIIO::ImageSpec( Result.GetSize().x,
											 Result.GetSize().y,
											 Result.GetChannelsCount(),
											 Details::ToOIIOImageDataType( InFormat ) );

		auto ImBufResult = OIIO::ImageBuf( ImResultSpec, Result.RawData() );
		
		if ( OIIO::ImageBufAlgo::copy( ImBufResult, ImBufSource, Details::ToOIIOImageDataType( InFormat ) ) )
		{
			Result.MarkInitialized();
		}
		
		InTo = std::move( Result );
	}

	void ImageUtils::Convert( Ref< Image > InFrom, ERawImageFormat InFormat )
	{
		if ( InFrom->GetBytesSize() == 0 )
		{
			return;
		}

		if ( InFrom->GetFormat() == InFormat )
		{
			return;
		}

		Image ImResult;

		Copy( InFrom, ImResult, InFormat );

		*InFrom = std::move( ImResult );
	}

	void ImageUtils::Resize( Ref< Image > InFrom, Image& InDest, VectorUInt2 InDestSize, ERawImageFormat InDestFormat )
	{
		auto Result = Image( InDestSize, InDestFormat );

		auto imSourceSpec = OIIO::ImageSpec( InFrom->GetSize().x,
											 InFrom->GetSize().y,
											 InFrom->GetChannelsCount(),
											 Details::ToOIIOImageDataType( InFrom->GetFormat() ) );

		auto imSourceBuf = OIIO::ImageBuf( imSourceSpec, Result.RawData() );

		auto imResultSpec = OIIO::ImageSpec( Result.GetSize().x,
											 Result.GetSize().y,
											 Result.GetChannelsCount(),
											 Details::ToOIIOImageDataType( Result.GetFormat() ) );

		auto imResultBuf = OIIO::ImageBuf( imResultSpec, Result.RawData() );

		if ( OIIO::ImageBufAlgo::resize( imResultBuf, imSourceBuf) )
		{
			Result.MarkInitialized();
		}

		InDest = Result;
	}

	void ImageUtils::VerticalFlip( Ref< Image > InFrom, Image& ToFlip )
	{
		auto imBothSpec = OIIO::ImageSpec( InFrom->GetSize().x,
										   InFrom->GetSize().y,
										   InFrom->GetChannelsCount(),
										   Details::ToOIIOImageDataType( InFrom->GetFormat() ) );

		auto imSourceBuf = OIIO::ImageBuf( imBothSpec, InFrom->RawData() );

		auto Result = Image( InFrom->GetSize(), InFrom->GetFormat() );

		auto imResultBuf = OIIO::ImageBuf( imBothSpec, Result.RawData() );

		if ( OIIO::ImageBufAlgo::flip( imResultBuf, imSourceBuf ) )
		{
			Result.MarkInitialized();
		}

		ToFlip = std::move( Result );
	}

	void ImageUtils::HorizontalFlip( Ref< Image > InFrom, Image& ToFlip )
	{
		auto imBothSpec = OIIO::ImageSpec( InFrom->GetSize().x,
										   InFrom->GetSize().y,
										   InFrom->GetChannelsCount(),
										   Details::ToOIIOImageDataType( InFrom->GetFormat() ) );

		auto imSourceBuf = OIIO::ImageBuf( imBothSpec, InFrom->RawData() );

		auto Result = Image( InFrom->GetSize(), InFrom->GetFormat() );

		auto imResultBuf = OIIO::ImageBuf( imBothSpec, Result.RawData() );

		if ( OIIO::ImageBufAlgo::flop( imResultBuf, imSourceBuf ) )
		{
			Result.MarkInitialized();
		}

		ToFlip = std::move( Result );
	}

	void ImageUtils::Rotate( Ref< Image > InFrom, Image& InDest, float InAngle )
	{
		auto imBothSpec = OIIO::ImageSpec( InFrom->GetSize().x,
										   InFrom->GetSize().y,
										   InFrom->GetChannelsCount(),
										   Details::ToOIIOImageDataType( InFrom->GetFormat() ) );

		auto imSourceBuf = OIIO::ImageBuf( imBothSpec, InFrom->RawData() );

		auto Result = Image( InFrom->GetSize(), InFrom->GetFormat() );

		auto imResultBuf = OIIO::ImageBuf( imBothSpec, Result.RawData() );

		if ( OIIO::ImageBufAlgo::rotate( imResultBuf, imSourceBuf, InAngle ) )
		{
			Result.MarkInitialized();
		}

		InDest = std::move( Result );
	}

	void ImageUtils::PixelSum( Ref< Image > InFirstOperand, Ref< Image > InSecondOperand, Image& InDest )
	{
		JF_ASSERT( InFirstOperand->GetSize() == InSecondOperand->GetSize(), "Images' sizes should the same." );

		auto imSpec = OIIO::ImageSpec( InFirstOperand->GetSize().x,
									   InFirstOperand->GetSize().y,
									   InFirstOperand->GetChannelsCount(),
									   Details::ToOIIOImageDataType( InFirstOperand->GetFormat() ) );

		auto imFirstBuf = OIIO::ImageBuf( imSpec, InFirstOperand->RawData() );

		auto imSecondBuf = OIIO::ImageBuf( imSpec, InSecondOperand->RawData() );

		auto Result = Image( InFirstOperand->GetSize(), InFirstOperand->GetFormat() );

		auto imResultBuf = OIIO::ImageBuf( imSpec, Result.RawData() );

		if ( OIIO::ImageBufAlgo::add( imResultBuf, imFirstBuf, imSecondBuf ) )
		{
			Result.MarkInitialized();
		}
		// else LOG ERROR

		InDest = std::move( Result );
	}

	void ImageUtils::PixelSum( Ref< Image > InFirstOperand, JSpan< float > InSecondOperand, Image& InDest )
	{
		auto imSpec = OIIO::ImageSpec( InFirstOperand->GetSize().x,
									   InFirstOperand->GetSize().y,
									   InFirstOperand->GetChannelsCount(),
									   Details::ToOIIOImageDataType( InFirstOperand->GetFormat() ) );

		auto imSourceBuf = OIIO::ImageBuf( imSpec, InFirstOperand->RawData() );

		auto Result = Image( InFirstOperand->GetSize(), InFirstOperand->GetFormat() );

		auto imResultBuf = OIIO::ImageBuf( imSpec, Result.RawData() );

		if ( OIIO::ImageBufAlgo::add( imResultBuf, imSourceBuf, InSecondOperand.data() ) )
		{
			Result.MarkInitialized();
		}
	
		InDest = std::move( Result );
	}

}
